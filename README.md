# Simple BFO with si5351 controlled by Arduino

## Table contents


## 
The Si5351 is an I2C configurable clock generator that is very appropriate for receivers and transceivers projects in amateur radio applications. It is also suited for replacing crystal oscillators. It has three outputs that you can get three distinct frequencies at the same time. A great feature of the Si5351A is the possibility of using it with a microcontroller or platform like Arduino, PIC family and others. This small project is about a BFO that you can control the output of the Si5351A by using the Arduino Micro (Atmega32u4).  The BFO tested here ocilates from 400KHz to 500KHz. However, you can change this range for other between 8Kz to 160MHz. 


## BFO interface
The user can control the BFO  by using tow buttons and an encoder. 

- The button __Step__ changes the increment and decrement step. It can be 10Hz, 100Hz or  1KHz;
- The button __Reset VFO__ makes the BFO to oscilateswitches central frequency;
- The __Encoder__ is used to increment or decrement the current frequency by using the step value as a
	reference.



