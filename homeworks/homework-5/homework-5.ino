#include <LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte downArrowChar[8] = {
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b10101,
	0b01110,
	0b00100
};

byte downAndUpArrowChar[8] = {
	0b00100,
	0b01110,
	0b10101,
	0b00100,
	0b00100,
	0b10101,
	0b01110,
	0b00100
};

byte upArrowChar[8] = {
	0b00100,
	0b01110,
	0b10101,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100
};

int optionMenu = 0;
int mainMenuLine = 1;
int settingLine = 1;
int aboutLine = 1;
const String menus[][5] = {
  {
  "Mini-game",
  "Start",
  "Settings",
  "About"
  },

  {
  "Settings",
  "Brightness",
  "Contrast",
  "Name",
  "Back"
  },

  {
  "Adrian V.",
  "t.ly/inmV"
  }  
};

const int mainMenuIndex = 0;
const int settingsIndex = 1;
const int aboutIndex = 2;


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
const int joystickButton = 13;
bool joystickButtonState = false;

unsigned long lastDebounceTime = 0;
const int debounceTime = 250;

unsigned long currentTime = 0;

bool displayUpdated = true;


void setup() {
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.createChar(0, downArrowChar);
  lcd.createChar(1, upArrowChar);
  lcd.createChar(2, downAndUpArrowChar);

  pinMode(joystickButton, INPUT_PULLUP);
  pinMode(joystickAxisX, INPUT);
  pinMode(joystickAxisY, INPUT);
// initialize the serial communications:
  Serial.begin(9600);
}
void loop() {
  if (optionMenu == mainMenuIndex) {
    checkForMovement(mainMenuLine);
  } 
  if (optionMenu == settingsIndex) {
    checkForMovement(settingLine);
  }
  
  
  currentTime = millis();
  if ((currentTime - lastDebounceTime) > debounceTime && !digitalRead(joystickButton)) {
    changePhase();
    lastDebounceTime = millis();
  }

  if (displayUpdated == true) {
    if (optionMenu == mainMenuIndex) {
      printMenu(mainMenuLine);
      displayUpdated = false;
    } 
    if (optionMenu == settingsIndex) {
      printMenu(settingLine);
      displayUpdated = false;
    } 
    if (optionMenu == aboutIndex) {
      printMenu(aboutLine);
      displayUpdated = false;
    }
    
  }
}

void changePhase() {
  if (optionMenu == mainMenuIndex) {
    optionMenu = mainMenuLine - 1;
  } else if (optionMenu == settingsIndex) {
    settingsMenu();
  } else if (optionMenu == aboutIndex) {
    optionMenu = mainMenuIndex;
  }
  displayUpdated = true;
}

const int lowLimit = 1;
int highLimit = 3;
void checkForMovement(int& index) {
  axisValueX = analogRead(joystickAxisX);
  axisValueY = analogRead(joystickAxisY);

  if (axisValueX > minThreshold && axisValueX < maxThreshold &&
      axisValueY > minThreshold && axisValueY < maxThreshold) {
        joystickNeutral = true;
  }

  if (joystickNeutral == true) {
    if (axisValueX < minThreshold) {
      if (index > lowLimit) {
        index--;
        displayUpdated = true;
      }
      joystickNeutral = false;
    }
    if (axisValueX > maxThreshold) {
      if (optionMenu == 0) {
        highLimit = 3;
      } else {
        highLimit = 4;
      }
      if (index < highLimit) {
        index++;
        displayUpdated = true;
      }
      joystickNeutral = false;
    }
  }
}

void printMenu(int index) {
  lcd.clear();

  String mainText = menus[optionMenu][0];
  for (auto chr : mainText) {
    lcd.write((byte) chr);
  }

  lcd.setCursor(0, 1);
  lcd.write((byte) ('>'));
  lcd.setCursor(2, 1);
  
  String text = menus[optionMenu][index];
  for (auto chr : text) {
    lcd.write((byte) chr);
  }
  lcd.setCursor(15, 1);

  if (index == lowLimit) {
    lcd.write(byte(0));
  } else if (index == highLimit) {
    lcd.write(byte(1));
  } else {
    lcd.write(byte(2));
  }
}

void settingsMenu() {
  // wip
}