# Simple BFO with si5351 controlled by Arduino

## Table contents

1. [Introduction]()
1. [Schematic](https://github.com/pu2clr/BFO#this-bfo-interface)
1. [This BFO interface](https://github.com/pu2clr/BFO#this-bfo-interface)
1. [Arduino pins and  Encoder, Step, Reset BFO and Turt On or Off commands]()
1. 


## Introduction

The Si5351 is an I2C configurable clock generator that is very appropriate for receivers and transceivers projects in amateur radio applications. It is also suited for replacing crystal oscillators. It has three outputs that you can get three distinct frequencies at the same time. A great feature of the Si5351A is the possibility of using it with a microcontroller or platform like Arduino, PIC family and others. This small project is about a BFO that you can control the output of the Si5351A by using the Arduino Micro (Atmega32u4).  The BFO tested here ocilates from 4520KHz to 458KHz. However, you can change this range for other between 8Kz to 160MHz. Click [here](https://youtu.be/AG9XZ8bdaNM) to watch a video about this BFO.

See also the similar project ["VFO and BFO with Si5351 and OLED,  controlled by Arduino (Micro)"](https://github.com/pu2clr/VFO_BFO_OLED_ARDUINO) 


## This BFO interface

The user can control the BFO  by using tow buttons and an encoder. 

- The button __Step__ changes the increment and decrement step. It can 10Hz, 50Hz or 100Hz;
- The button __Reset VFO__ makes the BFO to oscilateswitches on central frequency;
- The __Turn On or Off the BFO__ if it pressed, alternates On and off. BFO off means no signal   

## Schematic

 <img src="https://github.com/pu2clr/BFO/blob/master/schematic/bfo_schematic.png" alt="BFO schematic">


## Arduino pins and  Encoder, Step, Reset BFO and Turt On or Off commands

This section show some aspect of the BFO Arduino sketch](https://github.com/pu2clr/BFO/blob/master/source/si5351_bfo.ino) implementation. 

The pins for encoder and push buttons are defined bellow. If you need to change some push button pin, you should be aware that the push buttons are connected to  pins with external interrupts support. On Atmega32u4 you can use the pins 0,1,2,3 and 7, on 328-based (Nano, Mini, Uno) you can use the pins 2 and 3.

The short code below is part of the [Arduino sketch](https://github.com/pu2clr/BFO/blob/master/source/si5351_bfo.ino) and shows the enconder and push buttons setup.

```cpp
#define ENCODER_PIN_A 8 // Arduino  D8
#define ENCODER_PIN_B 9 // Arduino  D9

#define BUTTON_STEP 0   // Control the frequency increment and decrement
#define BUTTON_RST 1    // Set the frequency to central position
#define BUTTON_ON_OFF 7 // Turn BFO ON or OFF
```


The short code below shows the BFO frequency setup.

```cpp
// BFO range for this project is 4520KHz to 458KHz. The central frequency is 455KHz. 
#define MAX_BFO     45800000LU    // BFO max. frequency
#define CENTER_BFO  45500000LU    // BFO center frequency
#define MIN_BFO     45200000LU    // BFO min. frequency
```

The unit of frequency is 0.01Hz (1/100 Hz). See [Etherkit Library](https://github.com/etherkit/Si5351Arduino). 

if you want to modify the frequency of BFO to 10MHz (for example), just change the lines below.

```cpp
#define MAX_BFO     1100000000LU   // BFO max. frequency (11MHz)
#define CENTER_BFO  1000000000LU   // BFO center frequency (10MHz)
#define MIN_BFO      990000000LU   // BFO min. frequency (9MHz)
```

The buttons were implemented by using Arduino interrupts resource. The code bellow shows it. 

```cpp
 // Will stop what Arduino is doing call the function associated to the button
  attachInterrupt(digitalPinToInterrupt(BUTTON_STEP), changeStep, RISING); // whenever the BUTTON_STEP is pressed call changeStep
  attachInterrupt(digitalPinToInterrupt(BUTTON_RST), resetBfo, RISING);    // whenever the BUTTON_RST is pressed  call resetBfo
  attachInterrupt(digitalPinToInterrupt(BUTTON_ON_OFF), bfoOnOff, RISING); // whenever the BUTTON_ON_OFF is pressed  call bfoOnOff
```


