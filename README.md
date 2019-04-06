# Simple BFO with si5351 controlled by Arduino

## Table contents


The Si5351 is an I2C configurable clock generator that is very appropriate for receivers and transceivers projects in amateur radio applications. It is also suited for replacing crystal oscillators. It has three outputs that you can get three distinct frequencies at the same time. A great feature of the Si5351A is the possibility of using it with a microcontroller or platform like Arduino, PIC family and others. This small project is about a BFO that you can control the output of the Si5351A by using the Arduino Micro (Atmega32u4).  The BFO tested here ocilates from 400KHz to 500KHz. However, you can change this range for other between 8Kz to 160MHz. 

See also the project ["VFO and BFO with Si5351 and OLED,  controlled by Arduino (Micro)"](https://github.com/pu2clr/VFO_BFO_OLED_ARDUINO) 


## BFO interface
The user can control the BFO  by using tow buttons and an encoder. 

- The button __Step__ changes the increment and decrement step. It can be 10Hz, 100Hz or  1KHz;
- The button __Reset VFO__ makes the BFO to oscilateswitches central frequency;
- The __Encoder__ is used to increment or decrement the current frequency by using the step value as a
	reference.


## Schematic

 <img src="https://github.com/pu2clr/BFO/blob/master/schematic/bfo_schematic.png" alt="BFO schematic">



## Arduino pins and  Encoder, Step and Reset BFO commands

The pins for encoder and push buttons are defined bellow. If you need to change some push button pin, you should be aware that the push buttons are connected to  pins with external interrupts support. On Atmega32u4 you can use the pins 0,1,2,3 and 7, on 328-based (Nano, Mini, Uno) you can use the pins 2 and 3.

The short code below is part of the Arduino sketch and shows the enconder and push buttons setup.

```cpp
#define ENCODER_PIN_A 8 // Arduino  D8
#define ENCODER_PIN_B 9 // Arduino  D9

#define BUTTON_STEP 0    // Control the frequency increment and decrement step
#define BUTTON_BAND 1    // it makes the BFO go to the central frequency 

```

The short code below shows the BFO frequency setup.

```cpp
// BFO range for this project is 400KHz to 500KHz. The central frequency is 455KHz. 
#define MAX_BFO     50000000LU    // BFO max. frequency (500KHz)
#define CENTER_BFO  45500000LU    // BFO center frequency (455KHz)
#define MIN_BFO     40000000LU    // BFO min. frequency (400KHz)
```

The unit of frequency is 0.01Hz (1/100 Hz). See [Etherkit Library](https://github.com/etherkit/Si5351Arduino). 

if you want to modify the frequency of BFO to 10MHz (for example), just change the lines below.

```cpp
#define MAX_BFO     1100000000LU   // BFO max. frequency (11MHz)
#define CENTER_BFO  1000000000LU   // BFO center frequency (10MHz)
#define MIN_BFO      990000000LU   // BFO min. frequency (9MHz)
```

