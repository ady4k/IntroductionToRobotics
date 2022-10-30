// includes all the different buzzer pitches, based on piano keys
#include "buzzerTones.h"

// led pins and states - digital
const int redCarLightPin = 8;
const int yellowCarLightPin = 9;
const int greenCarLightPin = 10;
byte redCarLightState = LOW;
byte yellowCarLightState = LOW;
byte greenCarLightState = HIGH;

const int redPedestrianLightPin = 12;
const int greenPedestrianLightPin = 13;
byte redPedestrianLightState = HIGH;
byte greenPedestrianLightState = LOW;

unsigned long lastGreenPedestrianLightBlinkDelay = 0;
const int greenPedestrianLightBlinkDelay = 500;

// button - digital
const int pushButtonPin = 2;
volatile byte buttonState = LOW;
volatile byte lastButtonState = LOW;
volatile byte buttonActive = LOW;

// buzzer - digital PWM
const int buzzerPin = 3;
unsigned int buzzerDuration = 500;
unsigned long lastBuzzerDelay = 0;
const int buzzerDelay = 1000;

// debounce
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 500;

// state delay
unsigned long lastStateDelayTimer = 0;
const int state1DelayTimer = 8000;
const int state2DelayTimer = 3000;
const int state3DelayTimer = 8000;
const int state4DelayTimer = 4000;

// variable for button activation
int stateActive = 0;
int state = 1;

void setup() {
  pinMode(redCarLightPin, OUTPUT);
  pinMode(yellowCarLightPin, OUTPUT);
  pinMode(greenCarLightPin, OUTPUT);

  pinMode(redPedestrianLightPin, OUTPUT);
  pinMode(greenPedestrianLightPin, OUTPUT);

  pinMode(pushButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  changeLights();
  buttonState = !digitalRead(pushButtonPin);
  
  // checks button press and adds a debounce timer, if button is still pressed, activates it
  if (lastButtonState != buttonState) {
    lastButtonState = buttonState;
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay && state == 1) {
    if (buttonActive != buttonState) {
      buttonActive = buttonState;
    }
  }

  // button gets activated, so states can now cycle
  if (buttonActive) {
    stateActive = 1;
    lastStateDelayTimer = millis();
  }

  // state 2 starts here
  if ((millis() - lastStateDelayTimer) > state1DelayTimer && state == 1 && stateActive == 1) {
    state = 2;
    greenCarLightState = LOW;
    yellowCarLightState = HIGH;
    lastStateDelayTimer = millis();    
  }

  // state 3 starts here
  if ((millis() - lastStateDelayTimer) > state2DelayTimer && state == 2) {
    state = 3;
    yellowCarLightState = LOW;
    redCarLightState = HIGH;
    
    greenPedestrianLightState = HIGH;
    redPedestrianLightState = LOW;

    lastStateDelayTimer = millis();
  }

  // noise for 3rd state, 0.5s interval, green ped light is on
  if ((millis() - lastBuzzerDelay) > buzzerDelay && state == 3) {
    tone(buzzerPin, NOTE_G2, buzzerDuration);
    lastBuzzerDelay = millis();
  }

  // state 4 starts here, greed ped light starts blinking
  if ((millis() - lastStateDelayTimer) > state3DelayTimer && state == 3) {
    state = 4;    

    lastGreenPedestrianLightBlinkDelay = millis();
    lastStateDelayTimer = millis();
  }

  if ((millis() - lastGreenPedestrianLightBlinkDelay) > greenPedestrianLightBlinkDelay && state == 4) {
    greenPedestrianLightState = !greenPedestrianLightState;
    lastGreenPedestrianLightBlinkDelay = millis();
  }

  // noise for the 4th state, interval and delay is halfed, 0.25s
  if ((millis() - lastBuzzerDelay) > (buzzerDelay / 2) && state == 4) {
    tone(buzzerPin, NOTE_A4, buzzerDuration / 2);
    lastBuzzerDelay = millis();
  }

  // state turns to the first idle, button can now be activated once again
  if ((millis() - lastStateDelayTimer) > state4DelayTimer && state == 4) {
    stateActive = 0;
    state = 1;
    greenPedestrianLightState = LOW;
    redPedestrianLightState = HIGH;

    greenCarLightState = HIGH;
    redCarLightState = LOW;

  }
  // cycle ends
}

void changeLights() {
  digitalWrite(greenPedestrianLightPin, greenPedestrianLightState);
  digitalWrite(redPedestrianLightPin, redPedestrianLightState);
  digitalWrite(greenCarLightPin, greenCarLightState);
  digitalWrite(yellowCarLightPin, yellowCarLightState);
  digitalWrite(redCarLightPin, redCarLightState);
}