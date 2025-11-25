#include "userSettings.h"



UserSettings::UserSettings(int addressB0, int addressB1, int engMode) {
  //  _console = &_nullStream;
  this->b0 = addressB0;
  this->b1 = addressB1;
  this->engMode = engMode;

  pinMode(b0, INPUT_PULLUP);  //Set pins as input using internal pull-up
  pinMode(b1, INPUT_PULLUP);
  pinMode(engMode, INPUT_PULLUP);

}

int UserSettings::pcbAddress()
{
  int address = digitalRead(b0) | (digitalRead(b1) << 1);
  address = 3 - address;
  return address;
}

bool UserSettings::engineeringMode()
{
  bool res = digitalRead(engMode);
  return !res;
}
