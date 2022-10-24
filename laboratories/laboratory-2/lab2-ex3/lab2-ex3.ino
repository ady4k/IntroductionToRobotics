const int ledPin = 9;
bool ledState = 0;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

unsigned long currentMillis;

// storing the last second in a variable, when a second has passed it will be > interval => led state changes
void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    digitalWrite(ledPin, ledState);
    ledState = !ledState;
  }
  
}
