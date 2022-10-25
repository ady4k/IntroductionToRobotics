# Homework 1
## Using seperate potentiometers for controlling each of the color of a RGB led.

Deadline: 27th October 2022, Laboratory 3<br>
Completion date: 24th October 2022

## Components:
* Arduino Uno Board
* Breadboard
* 1 RGB LED
* 3 potentiometers (up to 50K Ohm)
* 3 rezistors (220 Ohm)
* a couple of wires

## Implementation:
Analog input data is read from the potentiometers as a number from the interval 0 to 1023, which is then "processed" in a percentage based formula to obtain the output LED value (number from 0 to 255). The LED has a common anode, each color pin is denoted by a colored wire (Red, Green, Blue). Each of the wire is connected to a PWM digital pin. The value of the each potentiometer is obtained with the white wire, connected to the potentiometer's middle pin and to an analog input pin on the Arduino Uno board. 

## Picture of the setup:
![circuit setup](https://i.imgur.com/NwHtBPc.jpg)

## Showcase video:
[Homework 1 - Controlling a RGB LED using potentiometers](https://youtu.be/SCht0vshSvY)
