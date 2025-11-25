#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Arduino.h>

#include <Wire.h>                     
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//Fonts
#include <Fonts/FreeSans24pt7b.h>
#include "FreeSans19pt7b.h"

#include "yagLogo.h"                //YAG logo for splash screen



#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // No OLED reset

class oledDisplay{
  public:  
    /// @brief Constructor for the AnalogSpeedDirection object
    /// @param maxCmdBuffer Optional - maximum number of bytes for the command buffer (default A0)   
    oledDisplay(int i2c_Address = 0x3c);
    void doSplash(const char *versionTxt);
    void doOTA();
    
    void loop();
    void showHeart();
    void hideHeart();
    void displaySleep();
    
    void displayUpdate(int aSpeed, bool isFwd, const char *trackLabel);
    void displayStatus(const char *statusTxt);
  private:
    unsigned long previousMillis = 0; 
    unsigned long currentMillis = 0;
    Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    void drawDirection(bool isFwd);
    void drawSpeed(int aSpeed);
};



#endif
