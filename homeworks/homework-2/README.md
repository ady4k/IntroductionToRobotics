#  Homework 2
## Crosswalk

Deadline: 3rd of November 2022, Laboratory 4<br>
Completion Date: 30th of October 2022

## Components:
* Arduino Uno Board
* Breadboard
* 2 red LEDs, 1 yellow LED, 2 green LEDs
* 6 rezistors (1 x 100 Ohm buzzer, 1 x 330 Ohm green car light, rest 4 x 220 Ohm))
* push button
* passive buzzer
* a couple of wires

## Implementation:
Data is digitally read from the push button, if pressed more than 0.5s it will turn to an "active" state, which will initialize state 2 of the circuit.

States are very similar to a realistic crosswalk:
1. State 2 is started 8 seconds after pressing the button, where the yellow light will stay on for about 3 seconds. 
2. After this period ends, state 3 comes in, turns the car light red and the pedestrian light to green. The buzzer starts making noise in intervals of 0.5s. State ends after 8 seconds, where state 4 gets called. 
3. In state 4, the green pedestrian light will blink, the buzzer will buzz in lower intervals, of 0.25s. 
4. The entire state cycle ends after about 3 seconds of state 4, restarting from the first idle state.

There is no usage of the function "delay()" in the entire project, everything is done dynamically using "millis()" differences.

The button will turn into an active state only when the circuit is on the initial idle state.

## Showcase video: [Homework 2 - Crosswalk]([TBA](https://youtu.be/tjITBHbUue4))

## Picture of the setup: [Pictures Album](https://imgur.com/a/CEPxUwc)
![circuit setup](https://i.imgur.com/TfqcUkW.jpg)
## Picture of the schema: ![circuit schema](https://i.imgur.com/b4kvLRM.png)
