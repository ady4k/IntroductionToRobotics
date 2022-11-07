// 7-segment display + dot
const int segmentPinA = 4;
const int segmentPinB = 5;
const int segmentPinC = 6;
const int segmentPinD = 7;
const int segmentPinE = 8;
const int segmentPinF = 9;
const int segmentPinG = 10;
const int segmentPinDP = 11;

// array of all segments pin
const int segmentsSize = 8;
const int segments[segmentsSize] = {
  segmentPinA, segmentPinB, segmentPinC, segmentPinD, segmentPinE, segmentPinF, segmentPinG, segmentPinDP
};

// array of all segments state, used for "saving", will be reset
int segmentsState[segmentsSize] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

// joystick pins, threshold and current values held
const int joystickAxisX = A0;
const int joystickAxisY = A1;

const int minThreshold = 400;
const int maxThreshold = 600;

int axisValueX = 0;
int axisValueY = 0;

// if joystick is beteween the threshold interval
bool joystickNeutral = true;

// joystick button pin, interrupt flag and state
const int joystickButton = 2;
volatile bool joystickButtonFlag = false;
volatile int joystickButtonState = 0;

// holding the button reset time
unsigned long resetTimer = 0;
const int resetTime = 3000;

// interrupt debounce time
unsigned long lastDebounceTime = 0;
const int debounceTime = 250;


const int movementDirections = 4; // up, down, left, right
// 4 movements on each segment            | => 4 columns
// row denotes segment, columns movement. | => 8 rows
const int movementsAllowed[segmentsSize][movementDirections] {
// UP  DOWN  LEFT  RIGHT
  {NULL, 10,   9,    5},     // [0] SEGMENT A  - pin 4
  {4,    10,   9,    NULL},  // [1] SEGMENT B  - pin 5
  {10,   7,    8,    11},    // [2] SEGMENT C  - pin 6
  {10,   NULL, 8,    6},     // [3] SEGMENT D  - pin 7
  {10,   7,    NULL, 6},     // [4] SEGMENT E  - pin 8
  {4,    10,   NULL, 5},     // [5] SEGMENT F  - pin 9
  {4,    7,    NULL, NULL},  // [6] SEGMENT G  - pin 10
  {NULL, NULL, 6,    NULL}   // [7] SEGMENT DP - pin 11
}; // number is pin number, NULL is no movement allowed
int currentPosition = 11; // default position will be on segment DP
int previousPosition = currentPosition;

// used in state 1, when moving the position
unsigned long lastBlinkTime = 0;
const int blinkDelay = 500;

void setup() {
  for (auto segment : segments) {
    pinMode(segment, OUTPUT);
  }
  pinMode(joystickButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(joystickButton), changeState, FALLING);

  Serial.begin(9600);
}

void loop() {
  // button press routine
  if (joystickButtonFlag == true) {
    resetTimer = millis(); // activates the reset timer in case of long button press
    joystickButtonState = !joystickButtonState;
    joystickButtonFlag = false;

    Serial.println("State changed to: " + (String) joystickButtonState);
  }

  // reseting routine
  if ((millis() - resetTimer) > resetTime && digitalRead(joystickButton) == LOW && joystickButtonState == true) {
    resetDisplay();
  }
  
  checkForMovement(joystickButtonState); 
 
  // led blinking routine
  // changing the program state or moving on the display will overwrite the current LED state with the saved one
  if (joystickButtonState == false) {
    if (previousPosition != currentPosition) {
      Serial.println("Position changed, LED state restored.");
      rewriteLed(previousPosition);
    }
    previousPosition = currentPosition;
    blink(currentPosition);
  } else {
    rewriteLed(previousPosition);
  }
}

// triggers an interrupt flag on button press
void changeState() {
    if ((millis() - lastDebounceTime) > debounceTime) {
      joystickButtonFlag = !joystickButtonFlag;
    }
    lastDebounceTime = millis();
}

// segments matrix gets reset to 0
// program state gets set to initial 1st state
// position gets reset to the dot 
void resetDisplay() {
  Serial.println("Reset routine called.");
  joystickButtonState = false;
  for (auto segment : segments) {
    setSegmentState(segment, LOW);
  }
  currentPosition = 11;
}

void movement(String move) {
  // offset of real pin in the movement matrix is 4
  const int matrixPosition = currentPosition - 4;
  int positionToMoveTo;
  if (move == "up") {
      positionToMoveTo = movementsAllowed[matrixPosition][0];
  }
  if (move == "down") {
      positionToMoveTo = movementsAllowed[matrixPosition][1];
  }
  if (move == "left") {
      positionToMoveTo = movementsAllowed[matrixPosition][2];
  }
  if (move == "right") {
      positionToMoveTo = movementsAllowed[matrixPosition][3];
  }
  // if there is a possible move in the selected direction
  if (positionToMoveTo != NULL) {
    currentPosition = positionToMoveTo;
  } else {
    Serial.println("Movement not allowed in specified direction.");
  }
  Serial.println("Moved position to: " + (String) currentPosition);
}


void setSegmentState(const int segmentPin, const byte state) {
  const int matrixPosition = segmentPin - 4; // offset of pin vs matrix index
  digitalWrite(segmentPin, state);
  segmentsState[matrixPosition] = state;
}

// used to check if the button is "enabled" or not
const int disabled = 0;
const int enabled = 1;
void checkForMovement(int buttonState) {
  axisValueY = analogRead(joystickAxisY);
  axisValueX = analogRead(joystickAxisX);

  if (axisValueX > minThreshold && axisValueX < maxThreshold && 
      axisValueY > minThreshold && axisValueY < maxThreshold) {
        joystickNeutral = true;
      }

  if (buttonState == disabled && joystickNeutral == true) {
    if (axisValueX < minThreshold) {
      movement("down");
      joystickNeutral = false;
    }
    if (axisValueX > maxThreshold) {
      movement("up");
      joystickNeutral = false;
    }
    if (axisValueY < minThreshold) {
      movement("left");
      joystickNeutral = false;
    } 
    if (axisValueY > maxThreshold) {
      movement("right");
      joystickNeutral = false;
    }
  }

  if (buttonState == enabled && joystickNeutral == true) {
    if (axisValueY < minThreshold || axisValueY > maxThreshold) {
      const int matrixPosition = currentPosition - 4; // offset
      // toggles the selected segment state on Y-axis movement detect
      setSegmentState(currentPosition, !segmentsState[matrixPosition]);
      
      Serial.println((String) currentPosition + " set to: " + (String) segmentsState[matrixPosition]);
      joystickNeutral = false;
    }
  }
}

void blink(int calledPosition) {
  if ((millis() - lastBlinkTime) > blinkDelay) {
    const byte currentState = digitalRead(calledPosition);
    digitalWrite(calledPosition, !currentState);
    lastBlinkTime = millis();
  }
}

void rewriteLed(int calledPosition) {
  int matrixPosition = calledPosition - 4;
  digitalWrite(calledPosition, segmentsState[matrixPosition]);
}