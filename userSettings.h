#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <Arduino.h>


class UserSettings{
  public:  
    /// @brief Constructor for the UserSettings object
    /// @param maxCmdBuffer Optional - maximum number of bytes for the command buffer (default A0)   
  UserSettings(int addressB0, int addressB1, int engMode);

    /// @brief Set the stream object for console output
    /// @param console 
    //void setLogStream(Stream* console);
    
    /// @brief Get controller address from DIP switches, zero based. Need to add offset to get DCC address.
    /// @return address of controller. 
    int pcbAddress();

    /// @brief Get Engineering mode flag
    /// @return boolean indicating if engineering mode is set
    bool engineeringMode();
  private:
    int b0 = 0;     //Address LSB
    int b1 = 1;     //Address MSB
    int engMode = 0;
  
    //Stream* _console;                   // Stream object for console output
    //NullStream _nullStream;             // Send streams to null if no object provided
    
};




#endif
