#include "oledDisplay.h"
#include <Wire.h>                     
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


oledDisplay::oledDisplay(int i2c_Address)           //constructor. Start OLED on I2C address
{  
  display.begin(i2c_Address, true); // Address 0x3C default
}


void oledDisplay::doSplash(const char *versionTxt)        //display Splash screen
{
  display.clearDisplay();
  display.drawBitmap(0, 0, OLED_YAG_Logo_40x40_INV, 40, 40, SH110X_WHITE);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(45, 0);
  display.print("Yorkshire");
  display.setCursor(45, 10);
  display.print("Area Group");
  display.setCursor(45, 20);
  display.print("T-TRAK");
  display.setCursor(45, 30);
  display.print(versionTxt);
  display.display();  
}

void oledDisplay::loop(){
  //do timer based stuff
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    hideHeart();
  }
}


void oledDisplay::displayUpdate(int aSpeed, bool isFwd, const char *trackLabel) 
{
  drawDirection(isFwd);
  drawSpeed(aSpeed);
  displayStatus(trackLabel);
  display.display();
}

void oledDisplay::drawSpeed(int aSpeed)   //DCC speed, 0-127
{
  display.setFont(&FreeSans19pt7b);   
  display.clearDisplay();             //TO DO Just clear speed dislay area

  display.setCursor(0, 32);
  int perc = map(aSpeed, 0, 127, 0, 100);   // scale to %
   
  display.print(perc);
  if (aSpeed != 0)
  {
    display.print("%");
//    displayDirection(isFwd);
  }
  
}

void oledDisplay::drawDirection(bool isFwd){

 //const TOP 20
  //const RIGHT 20            //TO DO Just clear direction dislay area
  //const HEIGHT 20

  int x0 = 100;   //94
  int y0 = 6;     //0
  int W = 16;     //32
  int H = 26;     //32

  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  int x3 = 0;
  int y3 = 0;

  if (isFwd == true)
  {
    x1 = x0;
    y1 = y0;
    x2 = x0 + W;
    y2 = y0 + H / 2;
    x3 = x0;
    y3 = y0 + H;
  }
  else
  {
    x1 = x0 + W;
    y1 = y0;
    x2 = x0 + W;
    y2 = y0 + H;
    x3 = x0;
    y3 = y0 + H / 2;
  }


  display.fillTriangle(x1, y1, x2, y2, x3, y3, SH110X_WHITE);
  //display.display();
}


void oledDisplay::displayStatus(const char *statusTxt)
{
  display.fillRect(0, 48, 127 - 16, 63, SH110X_BLACK);   //clear area text is displayed
  display.setFont();
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print(statusTxt);
  display.display();
}


void oledDisplay::showHeart()
{
  display.drawBitmap(127 - 16, 63 - 16, myBitmapheart, 16, 16, SH110X_WHITE);
  display.display();
  currentMillis = millis();   //save time icon was displayed
}

void oledDisplay::hideHeart()
{
  display.fillRect(127-16, 63-16, 127, 63, SH110X_BLACK);
}

void oledDisplay::displaySleep()
{
  display.clearDisplay(); 
  //display.setFont(&FreeSans19pt7b);   
  //display.setCursor(0, 32);
  //display.print("Zzzz");
  //displayStatus("Gone to sleep");

  display.setFont();
  display.setTextSize(0);
  display.setCursor(0, 48);

 
  display.print("Zzz");
  //display.print("Zzz");
  display.display();
}

void oledDisplay::doOTA()
{
  display.clearDisplay();
  display.setFont();
  display.setTextSize(2);
  display.setCursor(0, 48);
  display.print("OTA");
  display.display();
  delay(2000);
}
