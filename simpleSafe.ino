#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Simplepush.h>

//program constants

// constants for simplepush
char* SIMPLE_KEY   = "xrHSvg";
char* SIMPLE_TITLE = "Simple-Safe";
char* SIMPLE_EVENT = "ff01";

char* SIMPLE_MSGS[] = {
  "SimpleSafe has been started",
  "Safe has been opened",
  "Safe has been closed"  
};

//constants for wifi
const char* WIFI_SSID   = "YourWifiSSID";
const char* WIFI_PASSWD = "WifiPassword";

// constants for misc
const int     LED_PIN       = D1;
const int     POLLING_CYCLE = 5000;
// constants for sensors
const int SENSOR_PIN         = D0;
const int SENSOR_OPEN        = 1;
const int SENSOR_CLOSED      = 0;
const boolean SENSOR_USEPULL = false;
//end program constants


// data types
enum SimpleMessageType {
  SafeStart = 0,
  SafeOpen,
  SafeClosed
  
};

// function definitions
void blinkLED(int ledPin, int count,int duration);

// main global variables
ESP8266WiFiMulti WiFiMulti;
Simplepush       simple;
int          lastSafeStatus;    // true  = open, false = close
int          currentSafeStatus; // true = open, false = close

void setup() {
  if(SENSOR_USEPULL){
   pinMode(SENSOR_PIN,INPUT_PULLUP);
  }else{
   pinMode(SENSOR_PIN,INPUT);
  }
  pinMode(LED_PIN,OUTPUT);
  
  // Connect ESP8266 to WiFi
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWD);

  if((WiFiMulti.run() == WL_CONNECTED)) {
    simple.send(SIMPLE_KEY,SIMPLE_TITLE,SIMPLE_MSGS[SafeStart],SIMPLE_EVENT);
    blinkLED(LED_PIN,3,1000);
  }else{
    blinkLED(LED_PIN,3,5000);
  }
  lastSafeStatus = digitalRead(SENSOR_PIN);
  digitalWrite(LED_PIN,HIGH);
}

void loop(){
  currentSafeStatus = digitalRead(SENSOR_PIN);
  if(currentSafeStatus != lastSafeStatus) {
    if(currentSafeStatus == SENSOR_OPEN) {
      simple.send(SIMPLE_KEY,SIMPLE_TITLE,SIMPLE_MSGS[SafeOpen],SIMPLE_EVENT);
    }else{
      simple.send(SIMPLE_KEY,SIMPLE_TITLE,SIMPLE_MSGS[SafeClosed],SIMPLE_EVENT);
    }
  }
  lastSafeStatus = currentSafeStatus;
  delay(POLLING_CYCLE);
}

// strobe ledPin high/low count number of times
// with each high/low lasting for duration
void blinkLED(int ledPin, int count,int duration){
 for(int i=0;i<count;i++) {
  digitalWrite(ledPin,HIGH);
  delay(duration/2);
  digitalWrite(ledPin,LOW);
  delay(duration/2);
 }
}

