# YAG_DCC-EX_Controller_Code
ESP8266 code for use with DCC-EX based model railway control system. Used by Yorkshire Area Group of the N Gauge Society.

Arduino project. Ensure ESP8266 is available as target device. Code should also run on ESP32 but you'll need to ensure the WiFi library is changed.

# analogSpeedDirection.cpp & analogSpeedDirection.h
Class AnalogSpeedDirection to generate speed and direction values from position of a potentiometer, determined by AnalogRead. Analog value is read in several times and averaged. Averaged value is then added to a basic leaky average calculator providing a basic low pass filter.

# config.h
System settings. Predefined Wi-FI AP SSID & password. Pre-set IP Addresses, pin allocations and text constants.

# Controller_v0.6.ino
Arduino IDE project. Main code loop.

## Initialisation
Load in user settings by reading DIP switches. Set-up track number and if engineering mode is enabled.
Initialise display with splash screen.
Set-up WiFi connection to command station. IP address settings are pre-set to speed things up a bit.
Connect to command station. Track output forced to DC and pseudo DCC address set.
Initialise DCC-EX - add loco to system based on DIP switch setting.


## Main Loop
Check engineering mode and trigger OTA update mode if active.
If in control mode, check Wi-Fi is still connected (reconnecting if not), process DCC-EX protocol messages, update OLED and check for speed and direction changes.
Speed and direction are checked every 0.5 seconds and only sent to DCC-EX if there's been a change. System will go into low power mode if speed is zero and isn't changed for 15 minutes.


# FreeSans19pt7b.h
Font library.

# oledDisplay.cpp oledDisplay.h
oledDisplay class interfaces with I2C OLED display.
Provides basic interface function for updating status, speed and direction values on display.


# updateOTA.cpp & updateOTA.h
Class wrapping up OTA firmware updates. 

# userSettings.cpp & userSettings.h
Class structure for reading in user settings - convert DIP switch inputs to sensible values, e.g. track output number, enable OTA update mode etc.

# yagLogo.h
Nice but low-res versionof YAG logo for displaying on OLED during power-up.
