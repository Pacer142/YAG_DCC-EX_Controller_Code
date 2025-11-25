
#ifndef STASSID
#define STASSID "YAG-T-TRAK"    //"your-ssid"
#endif

#ifndef STAPSK
#define STAPSK "password"       //"your-password"
#endif




const char* ssid = STASSID;
const char* password = STAPSK;

const char* txtHello = "Hello";

int DCC_EX_PORT = 10001;

IPAddress serverAddress(192, 168, 4, 1);    // IP address of your EX-CommandStation

IPAddress subnet(255, 255, 255, 0);

IPAddress myAddress[] = 
  {IPAddress(192, 168, 4, 101),
  IPAddress(192, 168, 4, 102),
  IPAddress(192, 168, 4, 103),
  IPAddress(192, 168, 4, 104)
  };     // IP address of your EX-CommandStation


int serverPort = 2560;

#define ADD_1 14              //D7
#define ADD_2 12              //D6
#define ENG_PIN 13            //Pin to enable engineering output
#define addressOffset 10001   //Added to value read in from switches to give virtual DCC address.



const char * trackLabels[] PROGMEM = {
    (char*) "White",
    (char*) "Black",
    (char*) "Red",
    (char*) "Yellow"
};

const char* trackID[] PROGMEM = {
    (char*) "A",
    (char*) "B",
    (char*) "C",
    (char*) "D"
};


const char trckID[] = {
    (char) 'A',
    (char) 'B',
    (char) 'C',
    (char) 'D'
};
