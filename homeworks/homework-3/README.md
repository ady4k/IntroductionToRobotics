# Homework 3
## Drawing on a 7-segment display

Deadline: 10th of November 2022, Laboratory 5<br>
Completion Date: 7th of November 2022

## Components:
* Arduino Uno Board
* Breadboard
* Small breadboard
* Joystick
* 7-segment display
* 2 x 1kOhm rezistors
* a couple of wires

## Implementation:
Board looks for joystick axis movement every loop and gets interrupted when the button is pressed. 

When a movement is detected, it chooses a specific segment of the 7-segment display, written in a movement array. 

1. Pressing the button causes the program to move to the second state, where the movement is now restricted and any Y-axis movement will toggle the state of the selected segment. 
2. Pressing the button while in the second program state, will save the state of the selected segment and grants movement on the display again. 
3. Holding the button pressed while in the first state for 3 seconds will reset the display to its initial form.

There is no usage of the function "delay()" in the entire project, everything is timed dynamically using "millis()" differences.

Pressing the button will cause an interrupt from the main loop to the program state cycling function. This will trigger an interrupt flag which will be used in the main loop to change the state of the program.

## Showcase video: [Homework 3 - Drawing on a 7-segment display](https://youtu.be/iQ8VWJ878Us)

## Picture of the setup: [Pictures Album](https://imgur.com/a/P1i75Jt)
![circuit setup](https://i.imgur.com/jqkDzOY.jpg)

Unfortunately Tinkercad doesn't offer a Joystick so there is no schema this time :(.