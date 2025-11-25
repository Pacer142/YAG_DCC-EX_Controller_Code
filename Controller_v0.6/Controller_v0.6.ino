//Main libraries

#include <DCCEXProtocol.h>
#include <ESP8266WiFi.h>


//Local libraries
// If we haven't got a custom config.h, use the example
#if __has_include("config.h")
#include "config.h"
#else
#warning config.h not found. Using defaults from config.example.h
#include "config.example.h"
#endif


#include "userSettings.h"           //User settings - read from PCB switches
#include "analogSpeedDirection.h"   //read analog input and convert to speed and direction.
#include "oledDisplay.h"
#include "updateOTA.h"
#include "History.h"

//OLED
#define i2c_Address 0x3c


UserSettings userSettings = UserSettings(ADD_1, ADD_2, ENG_PIN);
AnalogSpeedDirection speedDir = AnalogSpeedDirection(A0);
oledDisplay oled = oledDisplay(i2c_Address);

otaUpdate OTA = otaUpdate();

int virtualAddress = 0;            //Virtual DCC address
bool engineeringMode = false;
void setup_display();
void setupWiFi();
void displayUpdate(int aSpeed, bool isFwd, const char *trackLabel);

int previousSpeed = -999;
bool previousDirection = false;

unsigned long previousMillis = 0;

int Speed = 0;
bool isFwd = false;

unsigned long PowerOffTick = 0;
int SecondTick = 0;

const int loopdelay = 500;
#define SLEEP_AFTER 15*60       //Go to sleep after n seconds of inactivity

DCCEXProtocol dccexProtocol;
WiFiClient client;


// define our loco object
Loco *loco = nullptr;

// Delegate class for DCC-EX callbacks
class MyDelegate : public DCCEXProtocolDelegate {
  public:
    void receivedServerVersion(int major, int minor, int patch) {
      char VersionString[20];
      Serial.print("\n\nReceived version: ");
      Serial.print(major);
      Serial.print(".");
      Serial.print(minor);
      Serial.print(".");
      Serial.println(patch);
      sprintf(VersionString, "DCC-EX v%d.%d.%d", major, minor, patch);
      Serial.println(VersionString);
      //oled.displayStatus(VersionString);
      //DIAG(F("Delaying startup for %dms"), STARTUP_DELAY);
      //delay(500);
    }

    void receivedTrackPower(TrackPower state) {
      Serial.print("\n\nReceived Track Power: ");
      Serial.println(state);
      Serial.println("\n\n");
    }

    void receivedLocoUpdate(Loco *loco) {
      Serial.print("Received Loco update for DCC address: ");
      Serial.println(loco->getAddress());
      oled.showHeart();
    }
    void receivedRosterList() {
      Serial.println("\n\nReceived Roster");
    }

};


MyDelegate myDelegate;

//-------------- setup() -------------------
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");

  virtualAddress = userSettings.pcbAddress();           //virtual loco address
  engineeringMode = userSettings.engineeringMode();     //engineering mode switch set?

  oled.doSplash(FW_VERSION);                               //display logo with version number
  //oled.displayStatus(txtHello);                         //say hello
  //oled.displayStatus(VERSION);
  //delay(750);


  Serial.println("previousSpeed,Speed,isFwd");
  previousSpeed = 0;

  setupWiFi();                                            //connect Wi-Fi



  oled.displayStatus(trackLabels[virtualAddress]);      //display track name

  if (!loco) {
    // add a loco with DCC address 11 - LocoSourceEntry means it's not from the roster
    loco = new Loco(virtualAddress + addressOffset, LocoSource::LocoSourceEntry);
    Serial.print("Added loco: ");
    Serial.println(loco->getAddress());

    // turn track power on or the loco won't move
    dccexProtocol.powerOn();
  }

  oled.displayUpdate(0, isFwd, trackLabels[virtualAddress]);
  if (engineeringMode)
  {
    oled.doOTA();
    OTA.init();
  }
  displayAddress();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (engineeringMode)
  {
    OTA.loop();
  }
  else
  {
    checkWiFiConnection();
    dccexProtocol.check();              //update from DCC-EX base-station
    oled.loop();
    checkSpeedDirection();
  }
  //delay(10);
}

void displayAddress() {
  Serial.print("Connecting as: ");
  Serial.print(trackLabels[virtualAddress]);
  Serial.print(" using output <");
  Serial.print(trackID[virtualAddress]);
  Serial.print("> (");
  Serial.print(virtualAddress);
  Serial.println(")");
}



void setupWiFi()
{
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  oled.displayStatus("Connecting 1/2");

  WiFi.mode(WIFI_STA);

  WiFi.setOutputPower(5);              //WiFi power    WiFi.setOutputPower(0->20.5)

  WiFi.config(myAddress[virtualAddress], serverAddress, subnet);    //Preconfigure my IP, gateway & sub net
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  oled.displayStatus("Connected");
  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Connect to the server
  oled.displayStatus("Connecting 2/2");
  Serial.println(F("Connecting to the server..."));
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println(F("connection failed"));
    while (1)
      delay(1000);
  }


  Serial.println(F("Connected to the server"));
  dccexProtocol.setLogStream(&Serial);

  // Pass the delegate instance to wiThrottleProtocol
  dccexProtocol.setDelegate(&myDelegate);

  // Pass the communication to wiThrottleProtocol
  dccexProtocol.connect(&client);
  Serial.println(F("DCC-EX connected"));
  dccexProtocol.requestServerVersion();
  
  dccexProtocol.check();
  
  oled.displayStatus("Connected OK");
  char trkID = trackID[virtualAddress][0];

  Serial.print("TRKID: ");
  Serial.print(trackID[virtualAddress]);
  Serial.print(trckID[virtualAddress]);
  Serial.println(trkID);

  dccexProtocol.setTrackType(trckID[virtualAddress], DC, virtualAddress + addressOffset);  //Set track to DC with virtual address
  dccexProtocol.powerTrackOn(trckID[virtualAddress]);      //Turn on track power

  Serial.println("WIFI_LIGHT_SLEEP");
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3);

}

///////////////////////////////////////////

void shutDown()
{
  Serial.println("Enter Power Save Mode");
  dccexProtocol.setThrottle(loco, 0, Direction::Forward);
  dccexProtocol.powerTrackOff(trckID[virtualAddress]);
  oled.displaySleep();
  ESP.deepSleep(0);   //Enter low power mode
}

///////////////////////////////////////////

void checkWiFiConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Lost");
    setupWiFi();
  }
}

///////////////////////////////////////////
void checkSpeedDirection()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= loopdelay)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    speedDir.getSpeed(Speed, isFwd);    //get current speed & direction
    /*
      Serial.print(previousSpeed);
      Serial.print(",");
      Serial.print(Speed);
      Serial.print(",");
      Serial.println(isFwd * 50);
    */

    if ((previousSpeed == 0) && (Speed == 0))
    {
      //speed is zero and was zero
      SecondTick = SecondTick + 1;

      if (SecondTick >= (1000 / loopdelay))
      {
        SecondTick = 0;
        PowerOffTick += 1;

        if (PowerOffTick > SLEEP_AFTER)
        {
          shutDown();
        }


        Serial.print("PowerOffTick:");
        Serial.println(SLEEP_AFTER - PowerOffTick);
      }
    }
    else
    {
      PowerOffTick = 0;
      SecondTick = 0;
    }


    if ((previousSpeed != Speed) || (previousDirection != isFwd))
    {
      Serial.println("Speed changed");
      oled.displayUpdate(Speed, isFwd, trackLabels[virtualAddress]);
      if (loco) {
        if (isFwd == true)
        {
          dccexProtocol.setThrottle(loco, Speed, Direction::Forward);
          Serial.println("FWD");
        }
        else
        {
          dccexProtocol.setThrottle(loco, Speed, Direction::Reverse);
          Serial.println("REV");
        }
      }
      previousSpeed = Speed;
      previousDirection = isFwd;
    }

  }
}


///////////////////////////////////////////
