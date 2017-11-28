//*****************************************************************************
///   
///   Arduino_Garage_Detector.ino
///   Arduino SmartThings Garage Door Status Detector
///   Created by: Peter Sun 
///   Version 1: Initial Version
///   Version 2: Changed the height threshold
///   Version 3: Added ability to manually check 
///
///   Based on Dan Ogorchock on "SmartThings v2.0" Library
///   
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************

#include <SmartThingsESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
//******************************************************************************************
//NodeMCU ESP8266 Pin Definitions (makes it much easier as these match the board markings)
//******************************************************************************************

#define D6 12
#define D7 13

//Defining Echo/Trig Pin
#define echoPin D6 // Echo Pin
#define trigPin D7 // Trigger Pin


//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
SmartThingsCallout_t messageCallout;    // call out function forward decalaration
//Application Global Variables
const double MIN_HEIGHT = 25; //in INCHES
const int DELAY_MIN = 1; //in minutes
boolean isOpen = false;
long duration, inches, cm;
//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "WeGotDeathStar_24";                           //  <---You must edit this line!
String str_password = "mollygolly";                   //  <---You must edit this line!
IPAddress ip(192, 168, 1, 203);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 1, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 1, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings Hub Information
IPAddress hubIp(192, 168, 1, 201 );    // smartthings hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39500;   // smartthings hub port


//Create a SmartThings Ethernet ESP8266WiFi object
st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

bool isDebugEnabled;    // enable or disable debug in this example


//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void sendOn()
{
  smartthing.send("on");        // send message to cloud
}

//*****************************************************************************
void sendOff()
{
  smartthing.send("off");       // send message to cloud
}

//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{
  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }
  
  // setup hardware pins 
  //HC-SR04 Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  //synch up the ST cloud
  smartthing.send("off");       // send message to cloud
}

//*****************************************************************************
void loop()
{
  smartthing.run();
  checkHeight();
  //Delay for DELAY_MIN
  delay(DELAY_MIN * 60 * 1000);
}
//*****************************************************************************
void checkHeight()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  if((inches < MIN_HEIGHT) && (!isOpen)) {
    isOpen = true;
  }
  if((inches > MIN_HEIGHT) && (isOpen)) {
    isOpen = false;
  }
  if(isDebugEnabled)
  {
    Serial.print("Inches: ");
    Serial.println(inches);
    Serial.print("Garage Door Status: ");
    Serial.print("isOpen = ");
    Serial.println(isOpen);
  }  
  sendData(isOpen);
}

//*****************************************************************************
void sendData(boolean status)
{
  if(status)
  {
    if(isDebugEnabled)
    {
      Serial.println("Send: on");
    }
    smartthing.send("on");        // send message to cloud
  }
  else
  {
    if(isDebugEnabled)
    {
      Serial.println("Send: off");
    }
    smartthing.send("off");        // send message to cloud
  }
}


//*****************************************************************************
void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  
  if (message.equals("ping"))
  {
    checkHeight()
  }
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
