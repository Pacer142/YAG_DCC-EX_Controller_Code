#ifndef ANALOGSPEEDDIRECTION_H
#define ANALOGSPEEDDIRECTION_H

#include <Arduino.h>


#if defined(ESP32)      //ESP32 has 12 bit ADC, Arduino and ESP8266 have 10 bit ADC
#define ADC_RES 4096    //replace with ESP.getVcc()?
#else
#define ADC_RES 1024
#endif

static const int MIN_SPEED = 0;
static const int MAX_SPEED = 127;

//v0.3 = deadMin = 490; deadMax = 550;
//v0.4 = deadMin = 490; deadMax = 550;
//v0.5 = deadMin = 117; deadMax = 137;
const int deadBand = 15;
const int deadMin = 127 - deadBand;          
const int deadMax = 127 + deadBand;



class AnalogSpeedDirection{
  public:  
    /// @brief Constructor for the AnalogSpeedDirection object
    /// @param maxCmdBuffer Optional - maximum number of bytes for the command buffer (default A0)   
    AnalogSpeedDirection(int analogPin = A0);

    /// @brief Set the stream object for console output
    /// @param console 
//    void setLogStream(Stream* console);
    
    /// @brief Get speed & direction of train
    /// @return void; Speed and direction returned via variables passed to function 
    void getSpeed(int &Speed, bool &Direction);

  private:
    int _analogPin;
    bool _invert;
    const float _alpha = 0.01; //0.15;   //leaky average weighting
    float _leakyAverage = 0.0;
    
    //int _deadSection;
    //int _centreScale;
    //Stream* _console;                   // Stream object for console output
    //NullStream _nullStream;             // Send streams to null if no object provided
    
};


#endif
