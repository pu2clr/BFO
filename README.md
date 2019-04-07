# Simple BFO with si5351 controlled by Arduino

## Table contents

1. [Introduction](https://github.com/pu2clr/BFO#introduction)
1. [Components](https://github.com/pu2clr/BFO#components)
1. [Schematic](https://github.com/pu2clr/BFO#this-bfo-interface)
1. [BFO interface](https://github.com/pu2clr/BFO#bfo-interface)
1. [Arduino pins and  Encoder, Step, Reset BFO and Turt On or Off commands](https://github.com/pu2clr/BFO#arduino-pins-and--encoder-step-reset-bfo-and-turn-on-or-off-commands)
1. [References](https://github.com/pu2clr/BFO#references)
1. [Videos about this project](https://github.com/pu2clr/BFO#videos-about-this-project)


## Introduction

The Si5351 is an I2C configurable clock generator that is very appropriate for receivers and transceivers projects in amateur radio applications. It is also suited for replacing crystal oscillators. It has three outputs that you can get three distinct frequencies at the same time. A great feature of the Si5351A is the possibility of using it with a microcontroller or platform like Arduino, PIC family and others. This small project is about a BFO that you can control the output of the Si5351A by using the Arduino Micro (Atmega32u4).  The BFO tested here ocilates from 4520KHz to 458KHz. However, you can change this range for other between 8Kz to 160MHz. Click [here](https://youtu.be/AG9XZ8bdaNM) to watch a video about this BFO.

See also the similar project ["VFO and BFO with Si5351 and OLED,  controlled by Arduino (Micro)"](https://github.com/pu2clr/VFO_BFO_OLED_ARDUINO) 


### Using this BFO to listening to 15 meters ham radio station on USB mode

![Photo One](/images/BFO_01.png)

![Photo Two](/images/BFO_02.png)

![Photo Three](/images/BFO_03.png)




## Components

- AZDelivery 1 x OLED Display Arduino 128 x 64 Pixels White 0.96 Inch I2C IIC Module for Arduino. This project uses the [Text only Arduino Library for SSD1306 OLED displays](https://github.com/greiman/SSD1306Ascii) Arduino library.
- Adafruit Si5351A Clock Generator Breakout Board - 8KHz to 160MHz. This project uses the [Si5351 Library for Arduino](https://github.com/etherkit/Si5351Arduino).
- One regular encoder.
- Three regular Push Button.
- Two 10nF ceramic capacitor
- Six 10K resistor
- One 1K resistor  
- Arduino Micro ([Atmega32u4](https://www.microchip.com/wwwproducts/en/ATmega32u4))

## BFO interface

The user can control the BFO  by using tow buttons and an encoder. 

- The button __Step__ changes the increment and decrement step. It can 10Hz, 50Hz or 100Hz;
- The button __Reset VFO__ makes the BFO to oscilateswitches on central frequency;
- The __Turn On or Off the BFO__. If it is pressed, alternates On and off. BFO off means no output signal   

## Schematic

The schematic was built by using [Fritzing](http://fritzing.org/home/) Software, an initiative open-source software tools to design circuits. See folder [schematic](https://github.com/pu2clr/BFO/tree/master/schematic).

 <img src="https://github.com/pu2clr/BFO/blob/master/schematic/bfo_schematic.png" alt="BFO schematic">


## Arduino pins and  Encoder, Step, Reset BFO and Turn On or Off commands

This section shows some aspects of the BFO [Arduino sketch](https://github.com/pu2clr/BFO/blob/master/source/si5351_bfo.ino) implementation. 

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

You might need a different BFO frequency. If you want to modify the central frequency of BFO to 10MHz (for example), just change the lines below.

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


## References

- [Arduino](https://www.arduino.cc)
- [Arduino Micro Pinout](http://pinoutguide.com/Electronics/arduino_micro_pinout.shtml)
- [Atmega32u4](https://www.microchip.com/wwwproducts/en/ATmega32u4)
- [Arduino Interrupts](https://www.arduino.cc/reference/en/language/functions/interrupts/interrupts/)
- [Text only Arduino Library for SSD1306 OLED displays](https://github.com/greiman/SSD1306Ascii)
- [Si5351 Library for Arduino](https://github.com/etherkit/Si5351Arduino)
- [Fritzing](http://fritzing.org/home/)


## Videos about this project
- [BFO with SI5351 and Arduino test with REDSUN RP2100](https://youtu.be/AG9XZ8bdaNM)
- [VFO and BFO with Si5351A controlled by Arduino](https://youtu.be/pFDvcIk5EAk)
- [VFO e BFO com Si5351A e Arduino - Calibração do Si5351](https://youtu.be/BJ83uvDcfIo)
- [VFO e BFO com o Si5351 e OLED controlado por Arduino - (Portuguese)](https://youtu.be/0sGL2KpOJH4)


