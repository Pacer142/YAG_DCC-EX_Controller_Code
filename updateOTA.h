#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class otaUpdate{
  public:  
    /// @brief Constructor for the AnalogSpeedDirection object
    /// @param none   
    otaUpdate();

    /// @brief Set the stream object for console output
    /// @param console 
//    void setLogStream(Stream* console);
    
    /// @brief Initialise OTA
    /// @param none 
    void init();

    /// @brief Monitor OTA
    /// @param none 
    void loop();

  private:
   
    //int _deadSection;
    //int _centreScale;
    //Stream* _console;                   // Stream object for console output
    //NullStream _nullStream;             // Send streams to null if no object provided
    
};


#endif
