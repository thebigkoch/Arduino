#include <limits.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// select the pins used on the LCD panel
LiquidCrystal s_lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
String    s_availableChars = " ABCDEFGHIJKLMNOPQRSTUVWXYZ,.!";
int       s_currentCharIndex = 0;
String    s_currentString = "";

int       s_iOldKeyInput = btnNONE;
int       s_iRepeatCount = 0;
time_t    old_millis = INT_MIN;

// read the buttons
int read_LCD_buttons()
{
  int iKeyInput    = analogRead(0);      // read the value from the sensor

  if (iKeyInput > 1000) return btnNONE;
  if (iKeyInput < 50)   return btnRIGHT;
  if (iKeyInput < 250)  return btnUP;
  if (iKeyInput < 450)  return btnDOWN;
  if (iKeyInput < 650)  return btnLEFT;
  if (iKeyInput < 850)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}

void RemoveLetter()
{
  int iStrLength = s_currentString.length();
  if (iStrLength > 0)
  {
    s_currentString.remove(iStrLength - 1);
  }

  s_lcd.setCursor(0, 1);
  s_lcd.print("               ");
}

void AddLetter()
{
  s_currentString = s_currentString + s_availableChars[s_currentCharIndex];
  s_currentCharIndex = 0;
}

void ProcessButtonUp()
{
  s_currentCharIndex += 1;
  if (s_currentCharIndex == s_availableChars.length())
    s_currentCharIndex = 0;
}

void ProcessButtonDown()
{
  s_currentCharIndex -= 1;
  if (s_currentCharIndex < 0)
    s_currentCharIndex = s_availableChars.length() - 1;
}

void ProcessKey(int iLCDKey)
{
  // Default case, so make it work quickly.
  if (iLCDKey == btnNONE)
  {
    s_iRepeatCount = 0;
    s_iOldKeyInput = iLCDKey;
    return;
  }

  bool bReturn = false;
  time_t current_now = now();
  time_t current_millis = millis();

  if (iLCDKey == s_iOldKeyInput)
  {
    int iLimit = 500;
    if (s_iRepeatCount > 1)
    {
      iLimit = 250;
    }

    if (abs(current_millis - old_millis) < iLimit)
    {
      bReturn = true;
    }
  }
  else
  {
    s_iRepeatCount = 0;
  }

  s_iOldKeyInput = iLCDKey;
  if (bReturn)
  {
    return;
  }

  old_millis = current_millis;
  s_iRepeatCount++;

  switch (iLCDKey)
  {
    case btnLEFT:
      RemoveLetter();
      break;
    case btnRIGHT:
      AddLetter();
      break;
    case btnUP:
      ProcessButtonUp();
      break;
    case btnDOWN:
      ProcessButtonDown();
      break;
    case btnSELECT:
      break;
  }
}
void setup()
{
  Serial.begin(9600);
  s_lcd.begin(16, 2);              // start the library
  s_lcd.clear();
  s_lcd.setCursor(0, 0);
  s_lcd.print("Hello World!"); // print a simple message
}

void loop()
{
  // Update the current character.
  int iLCDKey = read_LCD_buttons();
  ProcessKey(iLCDKey);

  String currentString = s_currentString + s_availableChars[s_currentCharIndex];
  s_lcd.setCursor(0, 1);
  s_lcd.print(currentString);
}
