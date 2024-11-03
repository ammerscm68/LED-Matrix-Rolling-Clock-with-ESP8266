/*******************************************************************************************
 ** Author:  Mario Ammerschuber                                                           **
 ** Date:    November 2024                                                                **
 ** Version: 9.9.3 (English-UK) - without Holidays                                        ** 
 ** Board:   LOLIN (WEMOS) D1 R2 and mini                                                 **  
 ** Sensor:  DHT11 or DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Yellow                                                                   **  
 ** Website: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          **  
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "9.9.3 uk";
/************************( Importing the libraries )************************/

#include <ESP8266WiFi.h>                    // Install via library
#include <WiFiClientSecure.h>               // Install via library
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>              // HTTPClient - Install via library
#include <ESP8266WebServer.h>               // Webserver - Install via library
#include <ArduinoOTA.h>                     // Install via library
#include <ArduinoJson.h>                    // Install via library Version: 7
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include "LittleFS.h"                       // Little Filesystem - Install via library
#include <DHTesp.h>                         // DHT-11 / DHT-22 Temperature Sensor - Install via library
#include <pitches.h>                        // for passiv Buzzer

ESP8266WebServer server(80); // Webserver on Port 80     border-radius: 50\%

/********************( Defining Global Variables and Constants )********************/
String BoardName = "Wemos/Lolin D1 mini"; // Boardname from ESP8266

// ###################################### Owner of the Clock ######################################################
// ************************ Main Owner ************************************  [for void Greetings_and_Holiday()]
String WatchOwner1FirstName = ""; // First Name of the Main Owner - Example: Mike
String WatchOwner1LastName  = ""; // Main Owner's last Name - Example: Johnson
String Gender1 = "";  // male=masculine | female=female
String Birthday1 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 2. Owner (Optional) *****************************************************
String WatchOwner2FirstName = ""; // First Name of the second Owner
String WatchOwner2LastName  = ""; // Last Name of the 2nd Owner (Optional) - is not yet used
String Gender2 = "";  // male=masculine | female=female
String Birthday2 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 3. Owner (Optional) *****************************************************
String WatchOwner3FirstName = ""; // First Name of the 3rd Owner
String WatchOwner3LastName  = ""; // Last Name of the 3rd Owner (Optional) - is not yet used
String Gender3 = "";  // male=masculine | female=female
String Birthday3 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************

// ########################## Access Point (Captive Portal) IP-Address #########################################################################
int APIPA1 = 192; int APIPA2 = 168; int APIPA3 = 4; int APIPA4 = 1; // Default = 192.168.4.1
// #############################################################################################################################################

// #############################################################################################################################################
int ScrollClockTime  = 80; // Scroll Delay when the Time is Animated    80 = Default
// #############################################################################################################################################

// #################### Temperature and Humidity Sensor (DHT 11 oder DHT 22) ###################################################################
bool DHT11Sensor = true; // true = DHT11  /  false = DHT22   Sensor    (Output the Temperature in Celsius)
// #############################################################################################################################################

// ########################## Show a Ghost at Midnight #########################################################################################
bool MidnightGhost = true; // true = show a ghost at midnight / false= not show a ghost at midnight
// #############################################################################################################################################

// #############################################################################################################################################
bool SerialMonitor = false; // Serial-Monitor   true = On / false = Off
// #############################################################################################################################################

// ############################################################################################################################################# 
int MinComfortHum = 32; // Minimum Comfort Humidity (default = 32)  - Value in %
int MaxComfortHum = 68; // Maximum comfort Humidity (default = 68)  - Value in %
// #############################################################################################################################################

// ########################### !!!!!!!!!!! #####################################################################################################
bool FormatFileSystem = false; // true = Deletes all stored Files in the Filesystem !!!  /  false = default
// #############################################################################################################################################

// #############################################################################################################################################
const int PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const int buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const int HWLEDPin = D5; // LED Pin for to High Humidty Warning (D5 = GPIO 14)
// #############################################################################################################################################

// ###### Display Times for "OpenWeatherMAP" and "NewsData" as well as Indoor/Outdoor Temperature and Humidity (DHT-Sensor) ####
// ********** Minute 0 is Reserved for Evaluating the Humidity - Do not use it! ***********

// Actions every 3 Minutes
int OnlyTempViewTimeCount = 6; // Number of Times
String OnlyTempViewTime[] = {"3","12","21","36","48","57"}; // Only Show Indoor and Outdoor Temperature (Minute)
int OnlyHumViewTimeCount = 5; // Number of Times
String OnlyHumViewTime[] = {"6","24","33","39","51"}; // Only Display Indoor and Outdoor Air Humidity (Minute)
int DateNewsViewTimeCount = 5; // Number of Times
String DateNewsViewTime[] = {"9","18","27","42","54"}; // Current Date, Greetings and News (Minute) 
int FullWeatherViewTimeCount = 3; // Number of Times
String FullWeatherViewTime[] = {"15","30","45"}; // Full Weather (Minute)

/// Actions every 5 Minutes
/*int OnlyTempViewTimeCount = 3; // Number of Times
String OnlyTempViewTime[] = {"20", "35", "50"}; // Only Show Indoor and Outdoor Temperature (Minute)
int OnlyHumViewTimeCount = 3; // Number of Times
String OnlyHumViewTime[] = {"5", "30", "55"}; // Only Display Indoor and Outdoor Air Humidity (Minute)
int DateNewsViewTimeCount = 3; // Number of Times
String DateNewsViewTime[] = {"10", "25", "40"}; // Current Date, Greetings and News (Minute) 
int FullWeatherViewTimeCount = 2; // Number of Times
String FullWeatherViewTime[] = {"15", "45"}; // Full Weather (Minute)*/
//##############################################################################################################################################

DHTesp dht; // DHT Sensor for Temperature Measurement Inside (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = "";     // OpenWeatherMap City-ID 
bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;

String WeatherLocation = "";
String StatusOpenWeatherMap = "";
bool FirstWeatherData = true;

String SecureAppToken = ""; // Pushover SecureAppToken
String UserToken = ""; // Pushover UserToken
String PushoverDevice1 = ""; // Pushover Device (Example: Smartphone)
String PushoverDevice2 = ""; // Pushover Device
String PushoverDevice3 = ""; // Pushover Device
String PushoverMessageSound = "none"; // Pushover Message Sound
bool IsPushover = false; // Pushover default disabled

// NTP Server Pool for United Kindom:
static const char ntpServerName[] = "uk.pool.ntp.org";    //Find local Server under http://www.pool.ntp.org/zone/uk
const int timeZone = 0;                     // 0 when working with <Timezone.h>!
WiFiUDP Udp;
unsigned int localPort = 1701; // local port for listening to UDP packets
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// GreetingsToday
bool GreetingsToday = false;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Start Time LED Matrix Clock

String newsKey = ""; // NewsData Key;
String APINews = "";
String WPAPINews = "";
String StatusNewsData = "";
bool NewsOutOfCredits = false;

String DisplayOnTime = "";
String DisplayOffTime = "";
String FDOnTime = "";
String FDOffTime = "";
String Brightness = "L1";
bool MEP = false;
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool ClockWiFiConnectFail = false; // false standard
bool SIC = false; // Scroll In Clock 

float temp;
String SText = "";
String weatherString = "";
String WPweatherString = "";
String ATemp = "";
String AHum = "";
String InHumMessage = "";
String InHumidityStatus = "";
String DateString = "";
String cip = "";
String cip2 = "";

#define NUM_MAX 4

// for ESP-01 module - PINs MAX7219 LED-Matrix Display
//#define DIN_PIN 2 // D4 - ESP-01
//#define CS_PIN  3 // D9/RX - ESP-01
//#define CLK_PIN 0 // D3 - ESP-01

// for NodeMCU 1.0 and WEMOS D1 mini - PINs MAX7219 LED-Matrix Display
#define DIN_PIN 15  // D8 - NodeMCU or WEMOS D1 mini
#define CS_PIN  13  // D7 - NodeMCU or WEMOS D1 mini
#define CLK_PIN 12  // D6 - NodeMCU or WEMOS D1 mini

#include "max7219.h" // MAX7219 LED-Matrix Display
#include "fonts.h"

int  WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound = "";
String HourSound = "";

String GT =  ""; // Greetings
String GT1 = ""; // Greetings
String HT =  ""; // Holliday
String HT1 = ""; // Holliday
int GNCounter = 1; // Counter for Greetings/Holidays and News

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
unsigned long hwsTime1 = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
String OWMLastConnectionDateTime = ""; // last Connection Time OpenWeatherMap
String NewsDataLastConnectionDateTime = ""; // last Connection Time NewsData
long localMillisAtUpdate = 0;
int ScrollTextTime = 0; 
int waitloop = 0;
int WiFiConnectLoop = 0;
int MaxWiFiConnectLoop = 30;  // Maximum Number of Loops when Connected to WiFi
bool StartFail = false;
bool Display = true; // Displaymode - ON
int DoublePointStatus = 1; // Doublepoint Status - Default = Permanent ON
bool PDP = false;
bool EnhancedFunctions = true; // Additional Functions ON / OFF - Standard = ON (Date, Holidays, Birthdays, etc.)
bool WeatherFunctions = true; // Weather Data Function ON / OFF - Default = ON (OpenWeatherMap API Key and City ID Available)
bool NewsDataFunction = true; // Current News Function ON / OFF - Default = ON (NewsData API Key Available)
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
int IntInTempValue = -1;
int IntInHumValue = -1;
int humidity = -1; // Humidity of OpenWeatherMAP
int InTempCounter = 0; // Indoor Temperature Counter
int InHumCounter = 0; // Humidity
String InTempValue = "-"; // Indoor Temperature
String InHumValue = ""; // Humidity
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset = "";   // Please Refer Captive Portal
String InHumOffset = "";    // Please Refer Captive Portal
int InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad - in Captive Portal)
int InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1% - in Captive Portal)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "n/a";

/***************************************** ( Setup ) ****************************************/
void setup() {
// Init LED Matrix
initMAX7219();

LoadMatrixAdjustments(); // Load LED Matrix Clock settings....

if (SerialMonitor) {
InitSerial(115200, 120); Serial.println("");} // Wait for COM Connection  /  Wait Default= 120

// #####################################################################################
// für autobrightness (Photoresistor)
// pinMode(A0, INPUT); 
  // Initialize Button
   pinMode(PushButtonPin, INPUT_PULLUP); // (default PushButtonPin = D3)
  // Initialize LEDclock
   pinMode(HWLEDPin, OUTPUT); // (default HWLEDPin = D5)
   // Connect DHT sensor to GPIO 5 (D1) - Indoor Temperature and Humidity Sensor
   if (DHT11Sensor) {// DHT 11 Sensor
   dht.setup(5, DHTesp::DHT11);} else { // DHT 22 Sensor
   dht.setup(5, DHTesp::DHT22);}
// #####################################################################################

   if (SerialMonitor) {
   Serial.println(F("*** LED-Matrix-Clock ***"));
   Serial.println("   (Version: "+LMCVersion+")");
   Serial.println(F(""));}

    // Init passiv Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (default: buzzerPin = D2)
    noTone(buzzerPin);} // no Sound

  // Format Little-Filesystem
  if (FormatFileSystem) {
  if (littleFSInit) {
  LittleFS.format(); 
  printStringWithShift("     +++       The Filesystem has been formatted !  \
  --- Set the -FormatFileSystem- Variable back to -false- and \
  Upload the Sketch again.....",ScrollTextTime);
  printStringWithShift("                              +++\
         The Clock will Restart      +++           ",ScrollTextTime);
  MyWaitLoop(5000); // short wait
  ESP.restart(); // Restart Clock
  }}

  if (SerialMonitor) {
  if (ssid == "") {
    Serial.println(F(""));
    Serial.println(F("***** First Start Clock *****")); // First Start Clock
    Serial.println(F(""));
    }}

  WiFi.hostname("LEDMatrixClock"); // Clock Hostname

  if (ssid != ""){
  if (SerialMonitor) {  
  Serial.print(F("Connect with WiFi SSID: "));
  Serial.println(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // Making wireless Connection
  } else {ClockWiFiConnectFail = true;} // Bypass the WLAN registration when you start the Clock for the first time! - Start the Access Point immediately
  
  if (!ClockWiFiConnectFail) { 
  // Show assigned IP Address on Matrix
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Address:  "+cip+"                "; // Determined IP address (DHCP)

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Display IP Address  MyWaitLoop(10); // short wait

  if (SerialMonitor) {Serial.println(F("Start UDP-Server  ..."));}
  WiFiUDPStart();  // Start UPD Server

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {Serial.println(F("Start Arduino OTA/DNS-Server ... - URL: http://ledmatrixclock.local"));}
  ArduinoOTA.setHostname("ledmatrixclock");
  ArduinoOTA.setPassword("74656"); // default Passwort for OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {Serial.println("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; 
  digitalWrite(HWLEDPin, LOW); ESP.restart();}); // Restart on OTA Error
  // Now start Arduino now OTA/DNS
  ArduinoOTA.begin();
  // Start TCP (HTTP) Server
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  Serial.println(F(""));
  Serial.println(F("OpenWeatherMap has been disabled - No Weather Data"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}

  if (!NewsDataFunction) {
  Serial.println(F(""));
  Serial.println(F("NewsData has been disabled - No current News"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}}
   
    // Get all information of your LittleFS
    if (littleFSInit) {  
    if (SerialMonitor) {  
    Serial.println(F("Little Filesystem Init - done."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.println(F("File sistem info."));
    Serial.print(F("Total space:      "));
    Serial.print(fs_info.totalBytes);
    Serial.println(F("byte"));
    Serial.print(F("Total space used: "));
    Serial.print(fs_info.usedBytes);
    Serial.println(F("byte"));
    Serial.print(F("Block size:       "));
    Serial.print(fs_info.blockSize);
    Serial.println(F("byte"));
    Serial.print(F("Page size:        "));
    Serial.print(fs_info.totalBytes);
    Serial.println(F("byte"));
    Serial.print(F("Max open files:   "));
    Serial.println(fs_info.maxOpenFiles);
    Serial.print(F("Max path length:  "));
    Serial.println(fs_info.maxPathLength);
    Serial.println(F(""));}
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        if (SerialMonitor) {Serial.print(dir.fileName());
        Serial.print(F(" - "));}
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            if (SerialMonitor) {Serial.println(f.size());}
            f.close();
        }else{if (SerialMonitor) {Serial.println(F("0"));}}}
   if (SerialMonitor) {
   Serial.println(F("_______________________________"));
   Serial.println(F(""));}} else {
   if (SerialMonitor) {  
   Serial.println(F("Little Filesystem Init - Error."));}}
  if (SerialMonitor) {   
  Serial.println("StartTime: "+FormatedDateTimeEN(false));
  Serial.println(F(""));
  Serial.println(F(""));}

  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Check Whether DHT Sensor is Connected (3 attempts)
  // Check Temperature
   for (int i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "-") {
   break;}} 
   // Check Humity
   for (int i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

  if (InTempValue == "-") {InTempValue = "!";}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("DHT11 Sensor is Connected. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("DHT22 Sensor is Connected. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("DHT Sensor is malfunction !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("No DHT Sensor connected"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

    // Web server - if the link is wrong, back to the Homepage
  server.onNotFound(SiteNotFound);

  // Web server - Homepage, brightness modes and Display test
  server.on("/", handleRoot);
  server.on("/moff", MatrixOff);
  server.on("/mon",MatrixOn);
  server.on("/mb1", BrightnessLevel1);
  server.on("/mb2", BrightnessLevel2);
  server.on("/mb3", BrightnessLevel3);
  server.on("/mb4", BrightnessLevel4);
  server.on("/mb5", BrightnessLevel5);
  server.on("/cdof", ChangeDisplayAutoOnOff);
  server.on("/cdofc", ChangeDisplayAutoOnOffCancel);
  server.on("/mdt", DisplayCheck);

  // Doublepoint Status
  server.on("/sdflash", DoublePointFlash);
  server.on("/sdon", DoublePointOn);
  server.on("/sdoff", DoublePointOff);

  // Scrolling Speed
  server.on("/vlt1", ScrollTextTimeLevel1);
  server.on("/vlt2", ScrollTextTimeLevel2);
  server.on("/vlt3", ScrollTextTimeLevel3);
  server.on("/vlt4", ScrollTextTimeLevel4);
  server.on("/vlt5", ScrollTextTimeLevel5);
  server.on("/vlt6", ScrollTextTimeLevel6);
  server.on("/vlt7", ScrollTextTimeLevel7);
  server.on("/vlt8", ScrollTextTimeLevel8);

  // Activate/deactivate additional Functions
  server.on("/zfon", EnhancedFunctionsOn);
  server.on("/zfoff", EnhancedFunctionsOff);

  // Enable/Disable Weather Data
  if (AccessOpenWeatherMap) {
  server.on("/owmfon", WeatherFunctionsOn);
  server.on("/owmfoff", WeatherFunctionsOff);
  server.on("/aowmn", ActualWeather);}

   // Activate/Deactivate current News
  if (AccessNewsData) {
  server.on("/nafon", NewsDataFunctionOn);
  server.on("/nafoff", NewsDataFunctionOff);
  server.on("/aapin", ActualAPINews);}

  // Pushover Test Message
  if (IsPushover) {
  server.on("/potmdevice1", TMPushoverDevice1);
  server.on("/potmdevice2", TMPushoverDevice2);
  server.on("/potmdevice3", TMPushoverDevice3);}

  // Buzzer Sound config
  if (IsBuzzer) { 
  server.on("/sspbson", StartSoundOn);
  server.on("/sspbsoff", StartSoundOff);
  server.on("/hspbson", HourSoundOn);
  server.on("/hspbsoff", HourSoundOff);}

  // Clear settings - Reset
  server.on("/reset", ResetClockSettings); 
  server.on("/goreset", ResetClockYes); 

  // Restart the Clock
  server.on("/reboot", RebootClock); 

  // Indoor Temperature Measurement
  if (InTempValue != "!" && InHumValue != "-") {
  server.on("/tempvalue", DHTHTML); 
  server.on("/dhtcalibrate", DHTSensorCalibrate);}  

  // View API Keys
  server.on("/cpadjust", GetAdjustments); 

  // Switch off Display if "Automatic OFF" time is exceeded
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime() && String(d) + "." + String(mo) != "31.12") {
  Display = false; 
  ClockStart = false;
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Display off
  if (SerialMonitor) {Serial.println("Matrix switched off at "+ String(h) + ":" + String(m));}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println(F("Buzzer not active"));}} else {
  if (SerialMonitor) {Serial.println(F("Buzzer is active"));}}

  DateString = ExpandedDateEN(); // Calculate Date and Calendar Week

  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(DateString);
  Serial.println(F(""));}

  //saveing millis after setup for Runtime Modul - WLAN connection OK - start the clock
  bootTime = millis();} else { 
  // ++++ If WiFi connection failed ++++
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println(F("WiFi Connection failed!"));}  
  printStringWithShift("        +++  WiFi Connection failed !  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // short Pause
  else { 
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {  
  // Before starting the Access Point - Check Whether DHT Sensor is connected (3 attempts)
    // Check Temperature
   for (int i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(500);
   if (InTempValue != "-") {
   break;}}
   // Check Humity
   for (int i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

  if (InTempValue == "-") {InTempValue = F("!");}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("DHT11 Sensor is Connected. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("DHT22 Sensor is Connected. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("DHT Sensor is malfunction !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("No DHT Sensor connected"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

  // No saved Data - Start Access Point (Captive Portal).
  if (SerialMonitor) {  
  Serial.println(F("Run Access Point ... [LED-Matrix-Clock]")); 
  Serial.println(F(""));}
  String APIPAdresse = "        +++       Access Point is starting      +++       IP-Address: "+String(APIPA1)+"."+String(APIPA2)+"."+String(APIPA3)+"."+String(APIPA4)+"      +++           ";
  printStringWithShift(APIPAdresse.c_str(),ScrollTextTime);  
  printCharWithShift('.',ScrollTextTime);
    printCharWithShift('.',ScrollTextTime);
      printCharWithShift('.',ScrollTextTime);
      // ########################################################
       CaptivePortal();}}} // Start Captive Portal (WiFi-Manager)
      // ######################################################## 

/*************************************(Main Program)**************************************/

void loop() {  
if (ssid == "") {getTimeLocal(); server.handleClient();} else {

// contrast(); //AutoBrightness

 // Switch the Display on and off using a Push Button
 if (!await) {MatrixButtonOnOff();} 

  // Check whether there is still an Internet Connection
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // Making wireless connection
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Stop server only once
  StartStopServers += 1;  
  WebServerStop(); // Stop web server
  WiFiUDPStop(); // Stop UDP server
  }
  if (Display){
  printStringWithShift("        +++       WiFi Connection: disconnected       +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. wait
  if (SerialMonitor) {Serial.println("Please wait ... "+FormatedDateTimeEN(false));}
  printStringWithShift("        +++       Please wait ...      +++           ",ScrollTextTime);}
  MyWaitLoop(1000);}} else { // 1 sek. wait
  ClockWiFiConnectFail = false;  // // WLAN-Connection OK
  if (StartStopServers == 1)
  { // Restart server only once
  StartStopServers -= 1;
  if (SerialMonitor) {Serial.println("WiFi Connection: restored - "+FormatedDateTimeEN(false));}
  WiFiUDPStart(); // start UDP-Server
  WebServerStart(); // start Webserver
  if (Display) {printStringWithShift("        +++       WiFi Connection: restored...       +++           ",ScrollTextTime);}
  MyWaitLoop(1000);
  sendCmdAll(CMD_SHUTDOWN,0); // Display off
  if (DoublePointStatus == 0) {
  dots = 0;} else {dots = 1;}  // Doublepoint Status
  for (int i=0; i <= 60; i++){
  MyWaitLoop(10); // short wait
  getTimeLocal(); // get current local time     
  HandleOTAandWebServer();  // OTA and web server query  
  getAnimClock(false);} // Show / Calculate Time
  printStringWithShift("           ",1);
  sendCmdAll(CMD_SHUTDOWN,1); // Display on
  ClockScrollIn();}} // Scroll the time in the Display 

  // Only if there is a WiFi Connection
  if (!ClockWiFiConnectFail) { 
  HandleOTAandWebServer();  // OTA and Web Server query 
  if (StartFail) {if (SerialMonitor) {Serial.println(F("The Clock will restart..."));}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
  printStringWithShift("        +++       The Clock will restart      +++           ",ScrollTextTime); 
  digitalWrite(HWLEDPin, LOW); // LED turn off
  ESP.restart();} // Restart the Clock if there are WiFi problems

if (!await) {
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
/*if (String(s) == "5" || String(s) == "30") {
await = true;
getNews();
// getWeatherData();
MyWaitLoop(1000); // short wait 
await = false;} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*if (String(s) == "5" || String(s) == "15"  || String(s) == "30" || String(s) == "45") { // FONT Test
await = true; 
FT++;
char  test = '~' + FT; // Sign
String TestString = "     "+String(test)+"     ";
if (FT > 7) {FT = 0;} 
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(8500); // short wait
await = false;
} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Start the Clock
if (ClockStart) {
ClockStart = false; // Setup beendet 
await = true; 
// *********************  A Hello at the start to the Main Owner **********************************************************
if (WatchOwner1FirstName != "") {
String SHT= "       +++   Hello  "+cleanText(WatchOwner1FirstName)+"    ---    The Clock is started...       +++                    "; 
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
printStringWithShift("       +++     The Clock is started...     +++                    ", ScrollTextTime);} 
// ***************************************************************************************************************************
Greetings(true); // First, greetings and Holidays if available 
sendCmdAll(CMD_SHUTDOWN,0); // Display switch off
if (DoublePointStatus == 0)
{dots = 0;} else {dots = 1;}  // Doublepoint Status
// First regular measurement when sensor ready 
InHumidityStatus = CheckInHumidity(); // Indoor Humidity measurement
if (InHumidityStatus != "FAIL") {
// Show whether LED is OK
if (InTempOffset != "999") {digitalWrite(HWLEDPin, HIGH);} // LED switch on 
MyWaitLoop(100);
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") { 
digitalWrite(HWLEDPin, LOW);} else {
if (IsPushover) {PushOverAlert(InHumidityStatus);}}} else {digitalWrite(HWLEDPin, LOW);}
if (IsBuzzer && StartSound == "On") {StartBuzzerBeep();} // Sound at startup     
ClockScrollIn(); // Scroll the time in the Display 
await = false;} // Start of the clock - End

  // Midnight Action
  if (MidnightGhost || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "5" && Display) {PDP = true;} // DoublePoint on    
  if (String(h) == "0" && String(m) == "0" && String(s) == "6") {
  await = true;
  MyWaitLoop(1000); // short wait
  if (MidnightGhost && String(d) + "." + String(mo) != "1.1") {
  if (SerialMonitor) {Serial.println(F("Show Ghost"));}  
  ShowGhost();} // Ghost at Midnight
  getTimeLocal(); 
  if (EnhancedFunctions) {Greetings(false); // Determine Greetings for the new Day  
  DateString = ExpandedDateEN();}
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ****** Switch off Matrix automatically *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Leave the display on on New Year's Eve
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":7") {PDP = true;} // DoublePoint on 
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":8") {     
  await = true;
  Display = false; 
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800); MyWaitLoop(1000);} // Matrix switch off Beep  
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  digitalWrite(HWLEDPin, LOW); // LED off then on 
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix switch off
  if (SerialMonitor) {Serial.println("Matrix switched off at "+ String(h) + ":" + String(m));} 
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ***** Switch on Matrix automatically ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":8") {   
  await = true; 
  for (int i=0; i <= 50; i++){ 
  MyWaitLoop(10);     
  HandleOTAandWebServer();  // OTA and Web Server query  
  getAnimClock(false);} // Show / Calculate Time   
  printStringWithShift("           ",1);  
  sendCmdAll(CMD_SHUTDOWN,1); // Matrix turn on
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800);} // Matrix turn on Beep
  MyWaitLoop(1500); // short wait
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  ClockScrollIn(); // Scroll the Time in the Display
  if (SerialMonitor) {Serial.println("Matrix switched on at "+ String(h) + ":" + String(m));} 
  Display = true;
  CheckInHumidity();  // Indoor Humidity measurement
  await = false;}}

  // ******************** Begin actual Program *******************************
  if (Display) {
  // Sound at full Hour when activated 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // full Hour Beep
  await = false;}} 

  if (GoMatrixAction()) { // No more Actions 10 minutes before the Matrix switches off automatically!
                          // (for Safe switching off of the Matrix - No possible overlap with an Action)
  // Evaluation of the Humidity on the Hour if the Sensor is installed
  if  (String(m)+":"+String(s) == "0:9") { 
  InHumidityStatus = CheckInHumidity(); // Indoor Humidity measurement
  if (InHumidityStatus != "FAIL") {   
  PDP = true; // Doublepoint on     
  await = true; 
  // Indoor Humidity too low
  if (InHumidityStatus == "MIN") { 
  for (int i=0; i <= 1; i++) { // Repeat message 2x
  printStringWithShift("        +++       Attention:  The Humidity at the location of the Clock is too low ... !      +++           ",ScrollTextTime);
  MyWaitLoop(2500);}
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Send PushOver Message
  ClockScrollIn();}
  if (InHumidityStatus == "MAX") {   
  MyWaitLoop(1500);  
  // Indoor Humidity too high
  for (int i=0; i <= 1; i++){ // Repeat message 2x
  printStringWithShift("        +++       Attention: The Humidity at the location of the Clock is too high...!      +++           ",ScrollTextTime);
  MyWaitLoop(2500);}
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Send PushOver Message
  ClockScrollIn();}
  if (InHumidityStatus != "NORMAL") {digitalWrite(HWLEDPin, LOW);} 
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // // only Indoor and Outdoor Temperature --- 3x per Hour (default)
  if ((InTempValue != "-" && InTempValue != "!") || WeatherFunctions) {
  for (int i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Doublepoint on
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
     // *** Indoor Temperature ***
    if (InTempValue != "-" && InTempValue != "!") { // DHT active
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();}  // Temperature measurement 
    MyWaitLoop(100); // short wait
    String InTempString = "";
    if (InTempValue != "-") {
    IntInTempValue = InTempValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Replace Minus with "short" Minus
    if (IntInTempValue > -10) {
    InTempString = "           Indoor Temperature:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           Indoor Temperature:          "+String(IT2) + " " + InTempValue + deg + "C";}} else {
    if (IntInTempValue > 9) {  
    InTempString = "           Indoor Temperature:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           Indoor Temperature:          "+String(IT) + "  " + InTempValue + deg + "C ";}} 
    SIC = true; // ScrollIn Clock
    if (InTempOffset != "999") {printStringWithShift(InTempString.c_str(), ScrollTextTime); // Display Indoor Temperature
    MyWaitLoop(10000); // short wait (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Let the inside Temperature run from the display
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println(F("DHT Sensor disabled or broken"));}
    digitalWrite(HWLEDPin, LOW); // LED turn off
    InTempValue = F("-");}} else {digitalWrite(HWLEDPin, LOW);} // LED turn off

    // *** Outdoor Temperature ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData(); // Get current weather data (download)
    SIC = true; // ScrollIn Clock
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Show Outdoor Temperature
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // short wait (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Let the Outdoor Temperature run from the display
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Only Indoor and Outdoor Air Humidity --- 3x per Hour (default)
  if ((InHumValue != "-" && InHumValue != "!") || WeatherFunctions) {
  for (int i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":10") {PDP = true;} // Doublepoint on
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":11") { 
    await = true;
    // *** Indoor Air Humidity ***
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
    if (InHumidityStatus != "FAIL") {   
    String InHumString = "";
    if (IntInHumValue > 99) {
    InHumString = "           Indoor Air Humidity:          "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "           Indoor Air Humidity:          "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "           Indoor Air Humidity:          "+ String(IL) + " " + InHumValue + " % ";}}
    SIC = true; // ScrollIn Clock
    if (InTempOffset != "999") {printStringWithShift(InHumString.c_str(), ScrollTextTime); // Indoor Humidity measurement
    MyWaitLoop(10000); // short wait (10 sec. - default)
    printStringWithShift("             ", ScrollTextTime);} // Let the inside Outdoor Humidity run from the display
    } else {if (SerialMonitor) {Serial.println(F("DHT Sensor disabled or broken"));}}

    // *** Outdoor Air Humidity ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData();  // Get current Weather Data (download)
    SIC = true; // ScrollIn Clock
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Display Outdoor Humidity
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // short wait (10 sec. - default)
    printStringWithShift("             ", ScrollTextTime);} // Let the outside air humidity run from the display
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Additional Functions (Date, Holidays, Birthdays) and/or News Displays --- every 10 Minutes
  if (NewsDataFunction || EnhancedFunctions) {
  for (int i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Doublepoint on      
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
    await = true;
    CheckInHumidity(); // Indoor Humidity measurement
    if (NewsDataFunction && AccessNewsData) {getNews();} // get actual News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    DateString = ExpandedDateEN(); // Calculate date with calendar week
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Output date with calendar week
    if (GNCounter == 0) {Greetings(false);} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Alternately display Greetings and Holidays as well as News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsData Messages 
    // Show Message on Matrix Display if no Greetings
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    CheckInHumidity(); // Indoor Humidity measurement
    if (APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Show News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock 
    waitloop = 0; // Variable reset
    PDP = false; //Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Detailed Weather --- 2x per Hour (default)
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Doublepoint on   
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  CheckInHumidity(); // Indoor Humidity measurement
  getWeatherData(); // Get current Weather Data (download)
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  ClockScrollIn(); // ScrollIn Clock 
  PDP = false; //Doublepoint permanently off again when flashing mode
  await = false;}}}
}  // OnMatrixAction() = True

// Let the Doublepoint flash
if (DoublePointStatus == 2) { 
 if (millis() - dotTime > 500) { 
 dotTime = millis();
 if (!PDP) {dots = !dots;} else {dots = 1;}
 // short wait
 delay(1);}}
// Doublepoint off 
if (DoublePointStatus == 0) {dots = 0;} 
// Doppelpunkt Dauerleuchten
if (DoublePointStatus == 1) {dots = 1;} 
} // Display = true 
getAnimClock(true); // Show / Calculate Time
ResetAPCounter(); // Access Point Button Reset every 2 Minutes
}}}
} // Loop - Ende