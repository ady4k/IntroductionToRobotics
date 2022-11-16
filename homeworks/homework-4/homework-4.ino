// joystick pins
const int joystickAxisX = A0;
const int joystickAxisY = A1;

// axis threshold values
const int minThreshold = 400;
const int maxThreshold = 600;

// current axis values;
int axisValueX = 0;
int axisValueY = 0;

bool joystickNeutral = true; // a check if the joystick is in neutral position to allow movement

// pin, current state, debounce time for the joystick button
const int joystickButton = 2;
bool joystickButtonState = false;

unsigned long lastDebounceTime = 0;
const int debounceTime = 250;

// the current position digit, 
//digit1 digit2 digit3 digit4
//  0      1      2      3
int currentPosition = 3; // default starts at the least significant digit

// used in state 1, when moving the position
unsigned long lastBlinkTime = 0;
const int blinkDelay = 500;
bool isActive = false;

unsigned long currentTime = 0;

// shift register pins
const int dataPin = 12; // DS -> 14 on shift reg
const int latchPin = 11; // STCP -> 12 on shift reg
const int clockPin = 10; // SHCP -> 11 on shift reg

// 4-digit 7-segment display pins
const int segDig1 = 7;
const int segDig2 = 6;
const int segDig3 = 5;
const int segDig4 = 4;

const int displayCount = 4; // the numbers of digits on the 7-segment display
// from the right digit to the left digit of the 4-digit 7-segment display
const int displayDigits[displayCount] = {
  segDig4, segDig3, segDig2, segDig1
};

// what numbers to write on the display
int displayNumbers[displayCount] = {
  0, 0, 0, 0
};

const int encodings = 16; // from 0 to F in hexa
const byte byteEncodings[encodings] = {
// ABCDEFG DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

const byte regSize = 8; // 1 byte -> 8 bits
byte encoding = 0;


void setup() {
  pinMode(joystickButton, INPUT_PULLUP);
  pinMode(joystickAxisX, INPUT);
  pinMode(joystickAxisY, INPUT);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for (auto displayDigit : displayDigits) {
    pinMode(displayDigit, OUTPUT);
    digitalWrite(displayDigit, LOW);
  }

  Serial.begin(9600);
}

void loop() {
  Serial.println(currentPosition);
  currentTime = millis();

  // debounce for the button press
  if ((currentTime - lastDebounceTime) > debounceTime && !digitalRead(joystickButton)) {
    joystickButtonState = !joystickButtonState;
    lastDebounceTime = millis();
  }

  checkForMovement();
  writeNumber();
}


const bool enabled = true;
const bool disabled = false;
void checkForMovement() {
  axisValueX = analogRead(joystickAxisX);
  axisValueY = analogRead(joystickAxisY);

  if (axisValueX > minThreshold && axisValueX < maxThreshold &&
      axisValueY > minThreshold && axisValueY < maxThreshold) {
        joystickNeutral = true;
  }

  if (joystickNeutral == true) {
    if (joystickButtonState == disabled) {
      if (axisValueY < minThreshold) {
        if (currentPosition == 0) {
          currentPosition = 3;
        } else {
          currentPosition = currentPosition - 1;
        }
        joystickNeutral = false;
      }
      if (axisValueY > maxThreshold) {
        currentPosition = (currentPosition + 1) % displayCount;
        joystickNeutral = false;
      }
    }

    if (joystickButtonState == enabled) {
      int &currentNumber = displayNumbers[currentPosition];
      if (axisValueX < minThreshold) {
        if (currentNumber == 0) {
          currentNumber = 15;
        } else {
          currentNumber = currentNumber - 1; 
        }
        joystickNeutral = false;
      }
      if (axisValueX > maxThreshold) {
        currentNumber = (currentNumber + 1) % encodings;
        joystickNeutral = false;
      }
    }
  }
}

void activateDisplay(int digit) {
  for(auto displayDigit : displayDigits) {
    digitalWrite(displayDigit, HIGH);
  }
  digitalWrite(displayDigits[digit], LOW);
}

// transferring the number encoding to the shift reg which sends a signal to the 7-segment display
void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

// the encoding to be sent to the shift reg for each display
void writeNumber() {
  int displayIndex = 0;
  for (auto displayNumber : displayNumbers) {
    encoding = byteEncodings[displayNumber];
    
    // if the current digit that we write to is the same as our digit selected and we are in the movement state
    // blink the DP by adding 1 bit to the encoding (last bit is always 0 - DP)
    if (joystickButtonState == 0 && displayIndex == currentPosition) {
      if ((currentTime - lastBlinkTime) > blinkDelay) {
        isActive = !isActive;
        lastBlinkTime = millis();
      }
      if (isActive == true) {
        encoding = encoding + 1;
      }
    }

    writeReg(encoding);
    activateDisplay(displayIndex);
    displayIndex++;
    delay(5);
  }
}