
/*
  Program to control the "Adafruit Si5351A Clock Generator" or similar via Arduino.
 
  BFO controlled by Arduino 
  
  See on https://github.com/etherkit/Si5351Arduino  and know how to calibrate your Si5351
  See also the example Etherkit/si5251_calibration
  Author: Ricardo Lima Caratti (PU2CLR) -   2019/03/07
*/

#include <si5351.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// Enconder PINs
#define ENCODER_PIN_A 8 // Arduino  D08
#define ENCODER_PIN_B 9 // Arduino  D09

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Change this value bellow  (CORRECTION_FACTOR) to 0 if you do not know the correction factor of your Si5351A.
#define CORRECTION_FACTOR 80000 // See how to calibrate your Si5351A (0 if you do not want).

#define BUTTON_STEP 0   // Control the frequency increment and decrement
#define BUTTON_RST 1    // Set the frequency to central position
#define BUTTON_ON_OFF 7 // Turn BFO ON or OFF

// BFO range for this project is 452KHz to 458KHz. The central frequency is 455KHz.
#define MAX_BFO 45800000LU    // BFO max. frequency
#define CENTER_BFO 45500000LU // BFO center frequency
#define MIN_BFO 45200000LU    // BFO min. frequency

#define STATUS_LED 10 // Arduino status LED Pin 10
#define STATUSLED(ON_OFF) digitalWrite(STATUS_LED, ON_OFF)
#define MIN_ELAPSED_TIME 300
#define MIN_ENCODE_ELAPSED 5 // min delay to procces next encoder action

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
SSD1306AsciiAvrI2c display;

// The Si5351 instance.
Si5351 si5351;

// Struct for step database
typedef struct
{
  char *name; // step label: 50Hz, 10Hz, 500Hz and 100KHz
  long value; // Frequency value (unit 0.01Hz See documentation) to increment or decrement
} Step;

// Steps database. You can change the Steps and numbers of steps here if you need.
Step step[] = {
    {"10Hz  ", 1000}, // Minimum Frequency step (incremente or decrement) 10Hz
    {"50Hz  ", 5000},
    {"100Hz  ", 10000}}; // Maximum frequency step 100Hz
// Calculate the index of last position of step[] array
const int lastStepBFO = (sizeof step / sizeof(Step)) - 1;
volatile long currentStep = 0;

volatile boolean isFreqChanged = false;
volatile boolean clearDisplay = false;

volatile uint64_t bfoFreq    = CENTER_BFO; // 455 KHz for this project
volatile uint64_t bfoLastValue;            // will save the bfoFreq before turn BFO off

volatile uint8_t bfoOn = 1; // If 1 then BFO is enable, 0 disable
volatile boolean isBfoOnOff = false;

long volatile elapsedTimeInterrupt = millis(); // will control the minimum time to process an interrupt action
long elapsedTimeEncoder = millis();

// Encoder variable control
unsigned char encoder_pin_a;
unsigned char encoder_prev = 0;
unsigned char encoder_pin_b;

void setup()
{
  // LED Pin
  pinMode(STATUS_LED, OUTPUT);
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  // Si5351 contrtolers pins

  pinMode(BUTTON_STEP, INPUT);

  // The sistem is alive
  blinkLed(STATUS_LED, 100);
  STATUSLED(LOW);
  // Initiating the OLED Display
  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  display.set2X();
  display.clear();
  display.print("\nBFO TEST");
  display.print("\n\nBY PU2CLR");

  delay(3000);
  display.clear();
  displayDial();
  // Initiating the Signal Generator (si5351)
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  // Adjusting the frequency (see how to calibrate the Si5351 - example si5351_calibration.ino)
  si5351.set_correction(CORRECTION_FACTOR, SI5351_PLL_INPUT_XO);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(bfoFreq, SI5351_CLK0); // Start CLK0 (VFO)
  si5351.update_status();
  // Show the initial system information
  delay(500);

  // Will stop what Arduino is doing call the function associated to the button
  attachInterrupt(digitalPinToInterrupt(BUTTON_STEP), changeStep, RISING); // whenever the BUTTON_STEP is pressed call changeStep
  attachInterrupt(digitalPinToInterrupt(BUTTON_RST), resetBfo, RISING);    // whenever the BUTTON_RST is pressed  call resetBfo
  attachInterrupt(digitalPinToInterrupt(BUTTON_ON_OFF), bfoOnOff, RISING); // whenever the BUTTON_ON_OFF is pressed  call bfoOnOff

  delay(1000);
}

// Blink the STATUS LED
void blinkLed(int pinLed, int blinkDelay)
{
  for (int i = 0; i < 5; i++)
  {
    STATUSLED(HIGH);
    delay(blinkDelay);
    STATUSLED(LOW);
    delay(blinkDelay);
  }
}

// Show Signal Generator Information
// Verificar setCursor() em https://github.com/greiman/SSD1306Ascii/issues/53
void displayDial()
{
  double bfo = bfoFreq / 100000.0;

  // display.setCursor(0,0)
  // display.clear();
  display.set2X();
  display.setCursor(0, 0);
  display.print(" ");
  display.print(bfo);
  display.print(" KHz");

  display.print("\n\nStep: ");
  display.print(step[currentStep].name);

  display.set1X();
  display.print("\n\nBFO is ");
  display.print((bfoOn) ? "On" : "Off");
}

// Change the frequency (increment or decrement)
// direction parameter is 1 (clockwise) or -1 (counter-clockwise)
void changeFreq(int direction)
{
  bfoFreq += step[currentStep].value * direction; // currentStep * direction;
  // Check the BFO limits
  if (bfoFreq > MAX_BFO || bfoFreq < MIN_BFO) // BFO goes to center if it is out of the limits
  {
    bfoFreq = CENTER_BFO;     // Go to center
    blinkLed(STATUS_LED, 50); // Alert the user that the range is over
  }
  isFreqChanged = true;
}

// Change frequency increment rate
void changeStep()
{
  if ((millis() - elapsedTimeInterrupt) < MIN_ELAPSED_TIME)
    return;       // nothing to do if the time less than MIN_ELAPSED_TIME milisecounds
  noInterrupts(); // disable interrupts
  currentStep = (currentStep < lastStepBFO) ? (currentStep + 1) : 0;
  isFreqChanged = true;
  clearDisplay = true;
  elapsedTimeInterrupt = millis();
  interrupts(); // enable interrupts
}

// Set BFO to central position (CENTER_BFO)
void resetBfo()
{
  if ((millis() - elapsedTimeInterrupt) < MIN_ELAPSED_TIME)
    return;       // nothing to do if the time less than MIN_ELAPSED_TIME milisecounds
  noInterrupts(); // disable interrupts

  bfoFreq = CENTER_BFO;
  isFreqChanged = true;
  clearDisplay = true;

  elapsedTimeInterrupt = millis();
  interrupts(); // enable interrupts
}

void bfoOnOff()
{
  if ((millis() - elapsedTimeInterrupt) < MIN_ELAPSED_TIME)
    return;       // nothing to do if the time less than MIN_ELAPSED_TIME milisecounds
  noInterrupts(); // disable interrupts

  // If BFO is On it becomes Off. If BFO is Off it becomes On
 
  bfoOn = !bfoOn;

  bfoFreq = (bfoOn) ? bfoLastValue : 0;
  isBfoOnOff = true;

  isFreqChanged = true;
  clearDisplay = true;

  elapsedTimeInterrupt = millis();
  interrupts(); // enable interrupts
}

// main loop
void loop()
{
  if (bfoOn) // porcess just if BFO is on
  {
    // Next enconder action only can be processed after MIN_ENCODE_ELAPSED (5) milisecounds
    if ((millis() - elapsedTimeEncoder) > MIN_ENCODE_ELAPSED)
    {
      encoder_pin_a = digitalRead(ENCODER_PIN_A);
      encoder_pin_b = digitalRead(ENCODER_PIN_B);
      if ((!encoder_pin_a) && (encoder_prev)) // has ENCODER_PIN_A gone from high to low?
      {                                       // if so,  check ENCODER_PIN_B. It is high then clockwise (1) else counter-clockwise (-1)
        changeFreq(((encoder_pin_b) ? 1 : -1));
      }
      encoder_prev = encoder_pin_a;
      elapsedTimeEncoder = millis(); // keep elapsedTimeEncoder updated
    }
    // check if some action changed the frequency
    if (isFreqChanged)
    {
      if (isBfoOnOff) // if the user turn off the BFO (by pressing the On OFF button)
      {
        si5351.output_enable(SI5351_CLK0, bfoOn); // Stop gereneting signal if bfoOn is 0 or start genereting if it is 1
        isBfoOnOff = false;                       // Reset the BUTTON_ON_OFF status (not pressed)
      }
      else
      {
        bfoLastValue = bfoFreq;
        si5351.set_freq(bfoFreq, SI5351_CLK0);
      }
      isFreqChanged = false;
      displayDial();
    }
    else if (clearDisplay)
    {
      display.clear();
      displayDial();
      clearDisplay = false;
    }
  }
}