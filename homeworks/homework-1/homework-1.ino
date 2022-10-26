// analog pins
const int redAnalogInputPin = A0;
const int greenAnalogInputPin = A1;
const int blueAnalogInputPin = A2;

float redAnalogInputValue = 0;
float greenAnalogInputValue = 0;
float blueAnalogInputValue = 0;

// digital PWM pins
const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;

float redLedValue = 0;
float greenLedValue = 0;
float blueLedValue = 0;

// led output interval goes from 0 to 255
const float maxLedValue = 255.0;
// analog input interval goes from 0 to 1023
const float maxAnalogValue = 1023.0;


void setup() {
  pinMode(redAnalogInputPin, INPUT);
  pinMode(greenAnalogInputPin, INPUT);
  pinMode(blueAnalogInputPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  redAnalogInputValue = analogRead(redAnalogInputPin);
  greenAnalogInputValue = analogRead(greenAnalogInputPin);
  blueAnalogInputValue = analogRead(blueAnalogInputPin);

  redLedValue = calculateLedValue(redAnalogInputValue);
  greenLedValue = calculateLedValue(greenAnalogInputValue);
  blueLedValue = calculateLedValue(blueAnalogInputValue);

  analogWrite(redLedPin, redLedValue);
  analogWrite(greenLedPin, greenLedValue);
  analogWrite(blueLedPin, blueLedValue);
}

// based on the percentage of the analog input, calculates the led output value
float calculateLedValue(float analogInputValue) {
  return analogInputValue * (maxLedValue / maxAnalogValue);
}