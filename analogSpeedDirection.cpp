#include "analogSpeedDirection.h"


AnalogSpeedDirection::AnalogSpeedDirection(int analogPin) {
  //_console=&_nullStream;
  _analogPin = abs(analogPin);
  _invert = analogPin < 0;
  _leakyAverage = 0.0;

  pinMode(_analogPin, INPUT);

  int tmpSpeed = 0;
  bool tmpDir = false;
  //Read _analogPin a few times to settle integrator
  for (int i = 0; i < 5; i++)
  {
    getSpeed(tmpSpeed, tmpDir);
  }
}

/*
void AnalogSpeedDirection::getSpeed(int &Speed, bool &Direction) {
  int sensorValue = analogRead(_analogPin);
  int scaledValue = 0;
  _leakyAverage = _alpha * _leakyAverage + (1 - _alpha) * sensorValue;

  if (_leakyAverage < deadMin)
  {
    scaledValue = map(_leakyAverage, 8, deadMin, 127, 0);
  }
  else if (_leakyAverage > deadMax)
  {
    scaledValue = map(_leakyAverage, deadMin, 1023, 0, 127);    //985 on USB, 1024 when on batteries!
  }
  else
  {
    scaledValue = 0;
  }
  if (scaledValue > 127)
    scaledValue = 127;
  if (scaledValue < 0)
    scaledValue = 0;

  Direction = _leakyAverage > 512;
  //Speed = (int)leakyAverage;
  Speed = abs(scaledValue);
}
*/

void AnalogSpeedDirection::getSpeed(int &Speed, bool &Direction) {
  //int sensorValue = analogRead(_analogPin) / 4;

  unsigned long sensorValue = 0;
  int C = 10;
  for (int i=0; i<C; i++)
  {
      sensorValue = sensorValue + (analogRead(_analogPin) / 4);
  }
  sensorValue = sensorValue / C;


  
  _leakyAverage = _alpha * _leakyAverage + (1 - _alpha) * sensorValue;

  int scaledValue = _leakyAverage;

  if ((scaledValue >= deadMin) && (scaledValue <= deadMax))   //dead spot in middle of pot 127 +/- 10
  {
    scaledValue = 0;
  }
  else if (scaledValue < 117)                   //map reverse speeds
  {
    scaledValue = map(scaledValue, 0, deadMin,-127,0);
  }
  else
  {
    scaledValue = map(scaledValue, deadMax, 240, 0, 127);
  }
 
  Direction = scaledValue < 0;
  Speed = abs(scaledValue);


  if (Speed > 127)
    Speed = 127;
  if (Speed < 0)
    Speed = 0;
}
