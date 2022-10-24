const int potPin = A0;
const int ledPin = 9;

int potValue;

void setup() {
  Serial.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // put your setup code here, to run once:

}

// with the same formula for voltage, we can change the numerator to change the interval
// result with vary from 0.0 to 255.0
void loop() {
  potValue = analogRead(potPin);
  analogWrite(ledPin, potValue * (255.0 / 1023.0));
}
