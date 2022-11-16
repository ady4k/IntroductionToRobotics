#  Homework 4
## Writing a number on a 4 digit 7-segment display

Deadline: 17th of November 2022, Laboratory 6<br>
Completion Date: 16th of November 2022

## Components:
* Arduino Uno Board
* Breadboard
* Small breadboard
* Joystick
* 4-digit 7-segment display
* 74HC595 8-bit shift register device
* a lot of wires

## Implementation:
Board looks for joystick axis movement and button presses every loop.  <br>
A button press changes the state of the program:

1. Pressing the button initially enters state 2, where we can change the number that's being written to the selected digit.
2. Pressing the button again will revert to the initial state, where we can move from digit to digit.

When the program is the first state, the dot will blink every half a second. This functionality is done directly into the writing function, adding 1 bit to the specified encoding.

We used the "delay()" function to control how many times per second the digits are written to the display. Display update delay is basically each digit's delay multiplied by the number of digits. In our program, display updates every 20 ms. (5 ms delay per digit * 4 digits).


## Showcase video: [Homework 4 - Writing a number on a 4 digit 7-segment display](https://youtu.be/ehOOpvbYJzU)

## Picture of the setup: [Pictures Album](https://imgur.com/a/LHzHRZU)
![circuit setup](https://i.imgur.com/tYNdkv8.jpg)

