/*******************************************************************************************
 ** Author:  Mario Ammerschuber                                                           **
 ** Date:    December 2025                                                                **
 ** Version: 11.0.9 (English-IE)                                                          ** 
 ** Board:   LOLIN (WEMOS) D1 R2 and mini                                                 **  
 ** Sensor:  DHT11 or DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Yellow                                                                   **  
 ** Website: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          **  
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
const char* LMCVersion = "11.0.9 ie";
/************************( Importing the libraries )************************/

#include <ESP8266WiFi.h>                    // Install via library
#include <WiFiClientSecure.h>               // Install via library
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
const char* BoardName = "Wemos/Lolin D1 mini"; // Boardname from ESP8266

/********************( Arduino OTA/DNS Password)********************/
const char* OTAPassWD = "74656"; // Default = 74656

// ############## Owner of the Clock ######## Can be changed in the Captive Portal from Version 10.0.1 #########
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
byte APIPA1 = 192; byte APIPA2 = 168; byte APIPA3 = 4; byte APIPA4 = 1; // Default = 192.168.4.1
// #############################################################################################################################################

// #############################################################################################################################################
byte ScrollClockTime  = 80; // Scroll Delay when the Time is Animated    80 = Default   (Maximum : 255)
// #############################################################################################################################################

// #################### Temperature and Humidity Sensor (DHT 11 oder DHT 22) ###################################################################
String DHTSensor = "NoDHT"; // DHT11 / DHT22 / NoDHT   Sensor    (Output the Temperature in Celsius)
// #############################################################################################################################################

// ########################## Show Ghosts at Midnight ########### Can be Changed in the Captive Portal from Version 10.1.0 #####################
String MidnightGhost = "YES"; // YES = Show Ghost at Midnight / NO = Not Show Ghosts at Midnight
// #############################################################################################################################################

// #############################################################################################################################################
#define DEBUG 0   // Debugging  -  1=on, 0=off
// #############################################################################################################################################

// ############################################################################################################################################# 
int16_t MinComfortHum = 32; // Minimum Comfort Humidity (default = 32)  - Value in %
int16_t MaxComfortHum = 68; // Maximum comfort Humidity (default = 68)  - Value in %
// #############################################################################################################################################

// #############################################################################################################################################
int16_t MinRAMMem   = 15784; // Minimum RAM in Byte  Default = 15784
// #############################################################################################################################################

// ########################## GMCMapMidnightAccessDelay >> Default = 30. Minute | Minimum = 0 | Maximum = 59 ###################################
const char* GMCMMAD = "30";  // Example: 30 --> So no Download of the Current GMCMap Data until 00:30 o'clock
// #############################################################################################################################################

// ########################### !!!!!!!!!!! #####################################################################################################
bool FormatFileSystem = false; // true = Deletes all stored Files in the Filesystem !!!  /  false = default
// #############################################################################################################################################

// #############################################################################################################################################
byte MaxWiFiConnectLoop = 20;  // Maximum Number of Loops when Connected to WiFi  -  Minimum = 15    Maximum = 255
// #############################################################################################################################################

// #############################################################################################################################################
String MCTZ = "London, Belfast, Dublin"; // Presetting the Time Zone in which the Clock is located (Changes possible in the Captiv Portal)
// #############################################################################################################################################

// #############################################################################################################################################
String NTPSN = "ie.pool.ntp.org";
const char* ntpServerName = "ie.pool.ntp.org";  // Finde lokale Server unter http://www.pool.ntp.org/zone
// #############################################################################################################################################

// #############################################################################################################################################
const byte PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const byte buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const byte HWLEDPin = D5; // LED pin for Warning of too High or too Low Humidity or too High Radioactivity (D5 = GPIO 14) | -1 = no LED
// #############################################################################################################################################

DHTesp dht; // DHT Sensor for Temperature Measurement Inside (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID;     // OpenWeatherMap City-ID 

String rtime = "Please wait ..."; // Runtime of the Clock
String ClockStatus = ""; // Clock Status

bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;
bool AccessGMCMap = false;

String WeatherLocation = "";
String StatusOpenWeatherMap;
bool FirstWeatherData = true;

String SecureAppToken = ""; // Pushover SecureAppToken
String UserToken = ""; // Pushover UserToken
String PushoverDevice1 = ""; // Pushover Device (Example: Smartphone)
String PushoverDevice2 = ""; // Pushover Device
String PushoverDevice3 = ""; // Pushover Device
String PushoverMessageSound = "none"; // Pushover Message Sound
bool IsPushover = false; // Pushover default disabled
bool FailPushOver = false; // In case of PushOver Error
bool PushOverTest = false; // Test Puschover Device Receiver
int16_t  WSR = 800; // Webserver Refreshrate | default = 800

// NTP-Init (First Part)
const byte timeZone = 0; // 0 when working with <Timezone.h>!
WiFiUDP Udp;
unsigned int localPort = 1701; // local Port for Listening to UDP packets

// GreetingsToday
bool GreetingsToday = false;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Start Time LED Matrix Clock

String newsKey; // NewsData Key;
String APINews = "";
String WPAPINews = "";
String StatusNewsData;
bool NewsOutOfCredits = false;

String DisplayOnTime;
String DisplayOffTime;
String FDOnTime;
String FDOffTime;
String Brightness;
bool MatrixAction = true;
bool MEP = false;
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool GMCMapAdjustChange = false;
bool ClockWiFiConnectFail = false; // false standard
bool SIC = false; // Scroll In Clock 
bool SerialMonitor = false; // For Debuging - Default = off

float temp;
String SText = "";
String weatherString = "";
String WPweatherString = "";
String ATemp = "";
String AHum = "";
String InHumMessage = "";
String InHumidityStatus;
String DateString = "";
String cip = "";
String cip2 = "";

// GMCMap Data
String GMCMapValue = "";
String WPGMCMapValue = "";
String MXGMCMapValue = "";
String GMCMapHistoryID = "";
String GMCMapGMTOffset = "";
String StatusGMCMap = "";
String GMCMapToHigh = "";

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

byte  WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound;
String HourSound;

String GT =  ""; // Greetings
String GT1 = ""; // Greetings
String HT =  ""; // Holliday
String HT1 = ""; // Holliday
byte GNCounter = 1; // Counter for Greetings/Holidays and News

uint32_t ESPRAM; // ESP8266-Heap
bool OneTimeCall = false;

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
byte dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
unsigned long hwsTime1 = 0;
int16_t dx = 0; // must be of type "int16_t"
int16_t dy = 0; // must be of type "int16_t"
byte del = 0; // must be of type "int16_t"
int16_t h, m, s, w, mo, ye, d; // must be of type "int16_t"
String OWMLastConnectionDateTime = ""; // last Connection Time OpenWeatherMap
String NewsDataLastConnectionDateTime = ""; // last Connection Time NewsData
String GMCMapLastConnectionDateTime = ""; // last Connection Time GMCMap
long localMillisAtUpdate = 0;
byte ScrollTextTime = 0; 
int16_t waitloop = 0;
byte WiFiConnectLoop = 0;
bool Display = true; // Displaymode - ON
byte DoublePointStatus = 1; // Doublepoint Status - Default = Permanent ON
bool PDP = false;
bool EnhancedFunctions = true; // Additional Functions ON / OFF - Standard = ON (Date, Holidays, Birthdays, etc.)
bool WeatherFunctions = true; // Weather Data Function ON / OFF - Default = ON (OpenWeatherMap API Key and City ID Available)
bool NewsDataFunction = true; // Current News Function ON / OFF - Default = ON (NewsData API Key Available)
bool GMCMapFunction = false; // Current Radioactivity at the selected Location
byte StartStopServers = 0;
byte APStart = 0;
bool ClockStart = true;
int16_t IntInTempValue = -1;
int16_t IntInHumValue = -1;
int16_t humidity = -1; // Humidity of OpenWeatherMAP
byte InTempCounter = 0; // Indoor Temperature Counter
byte InHumCounter = 0; // Humidity
String InTempValue; // Indoor Temperature
String InHumValue; // Humidity
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset;   // Please Refer Captive Portal
String InHumOffset;    // Please Refer Captive Portal
int16_t InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad - in Captive Portal)
int16_t InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1% - in Captive Portal)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "n/a";

/***************************************** ( Setup ) ****************************************/
void setup() {
// Init LED Matrix
initMAX7219();

StringMemoryLimitation(); // Strings Limitation

CheckGlobalVariableRange(); // Check Variables

#if DEBUG // Debugging
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(fmt, ...) ((void)0)
#endif
if (DEBUG == 1) {SerialMonitor = true;} else {SerialMonitor = false;}

if (SerialMonitor) {
InitSerial(115200, 120); DEBUG_PRINTLN("");} // Wait for COM Connection  /  Wait Default= 120

if (SerialMonitor) {
   DEBUG_PRINTLN(F("*** LED-Matrix-Clock ***"));
   DEBUG_PRINTLN("  (Version: "+String(LMCVersion)+")");
   DEBUG_PRINTLN(F(""));}

LoadMatrixAdjustments(); // Load LED Matrix Clock settings....

// NTP-Init (Second Part)
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// #####################################################################################
// für autobrightness (Photoresistor)
// pinMode(A0, INPUT); 
  // Initialize Button
   pinMode(PushButtonPin, INPUT_PULLUP); // (default PushButtonPin = D3)
  // Initialize LEDclock
   if (HWLEDPin != -1) {pinMode(HWLEDPin, OUTPUT); digitalWrite(HWLEDPin, LOW);} // (default HWLEDPin = D5)
   // Connect DHT sensor to GPIO 5 (D1) - Indoor Temperature and Humidity Sensor
   if (DHTSensor != "NoDHT") {
   if (DHTSensor == "DHT11") {// DHT 11 Sensor
   dht.setup(5, DHTesp::DHT11);} else { // DHT 22 Sensor
   dht.setup(5, DHTesp::DHT22);}}
// #####################################################################################

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
    DEBUG_PRINTLN(F(""));
    DEBUG_PRINTLN(F("***** First Start Clock *****")); // First Start Clock
    DEBUG_PRINTLN(F(""));
    }}

  WiFi.hostname("LEDMatrixClock"); // Clock Hostname

  if (ssid != ""){
  if (SerialMonitor) {  
  DEBUG_PRINT(F("Connect with WiFi SSID: "));
  DEBUG_PRINTLN(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // Making wireless Connection
  } else {ClockWiFiConnectFail = true;} // Bypass the WLAN registration when you start the Clock for the first time! - Start the Access Point immediately
  
  if (!ClockWiFiConnectFail) { 
  // Show assigned IP Address on Matrix
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Address:  "+cip+"                "; // Determined IP address (DHCP)

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Display IP Address  MyWaitLoop(10); // short wait

  if (SerialMonitor) {DEBUG_PRINTLN(F("Start UDP-Server  ..."));}
  WiFiUDPStart();  // Start UPD Server

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {DEBUG_PRINTLN(F("Start Arduino OTA/DNS-Server ... - URL: http://ledmatrixclock.local"));}
  ArduinoOTA.setHostname("ledmatrixclock");
  ArduinoOTA.setPassword(OTAPassWD); // Password for OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {DEBUG_PRINTLN("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {DEBUG_PRINTF("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; ESP.restart();}); // Restart on OTA Error
  // Now start Arduino now OTA/DNS
  ArduinoOTA.begin();
  // Start TCP (HTTP) Server
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("OpenWeatherMap has been disabled - No Weather Data"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!NewsDataFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("NewsData has been disabled - No current News"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!GMCMapFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("GMCMap has been disabled - No current Radioactivity Values"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}}
   
    // Get all information of your LittleFS
    if (littleFSInit) {  
    if (SerialMonitor) {  
    DEBUG_PRINTLN(F("Little Filesystem Init - done."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    DEBUG_PRINTLN(F("File sistem info."));
    DEBUG_PRINT(F("Total space:      "));
    DEBUG_PRINT(fs_info.totalBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Total space used: "));
    DEBUG_PRINT(fs_info.usedBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Block size:       "));
    DEBUG_PRINT(fs_info.blockSize);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Page size:        "));
    DEBUG_PRINT(fs_info.totalBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Max open files:   "));
    DEBUG_PRINTLN(fs_info.maxOpenFiles);
    DEBUG_PRINT(F("Max path length:  "));
    DEBUG_PRINTLN(fs_info.maxPathLength);
    DEBUG_PRINTLN(F(""));}
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        if (SerialMonitor) {DEBUG_PRINT(dir.fileName());
        DEBUG_PRINT(F(" - "));}
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            if (SerialMonitor) {DEBUG_PRINTLN(f.size());}
            f.close();
        }else{if (SerialMonitor) {DEBUG_PRINTLN(F("0"));}}}
   if (SerialMonitor) {
   DEBUG_PRINTLN(F("_______________________________"));
   DEBUG_PRINTLN(F(""));}} else {
   if (SerialMonitor) {  
   DEBUG_PRINTLN(F("Little Filesystem Init - Error."));}}
  if (SerialMonitor) {   
  DEBUG_PRINTLN("StartTime: "+FormatedDateTimeEN(false));
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F(""));}

  if (DHTSensor != "NoDHT") {
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Check Whether DHT Sensor is Connected (3 attempts)
  // Check Temperature
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "!") {
   break;}} 
   // Check Humity
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHTSensor == "DHT11") { 
  if (SerialMonitor) {DEBUG_PRINTLN("DHT11 Sensor is Connected. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("DHT22 Sensor is Connected. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("-");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("DHT Sensor is malfunction !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("-"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("No DHT Sensor connected"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}} else {
  InTempValue = F("!"); InHumValue = F("-");
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("No DHT Sensor connected"));
  DEBUG_PRINTLN(F(""));}}

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

  // Activate/Deactivate current Radioactivity Display
  if (AccessGMCMap) {
  server.on("/gmcmapon", GMCMapFunctionOn);
  server.on("/gmcmapoff", GMCMapFunctionOff);
  server.on("/agmcmapd", ActualGMCMapData);}

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
  digitalWrite(HWLEDPin, LOW); // LED off
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix switched off at "+ String(h) + ":" + String(m));}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Buzzer not active"));}} else {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Buzzer is active"));}}

  DateString = ExpandedDateEN(); // Calculate Date and Calendar Week

  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(DateString);
  DEBUG_PRINTLN(F(""));}

  //saveing millis after setup for Runtime Modul - WLAN connection OK - start the clock
  bootTime = millis();} else { 
  // ++++ If WiFi connection failed ++++
  if (ssid != ""){
  StatusWarningLED("HIGH"); // LED ON   
  if (SerialMonitor) {DEBUG_PRINTLN(F("WiFi Connection failed!"));}  
  printStringWithShift("        +++  WiFi Connection failed !  +++           ",ScrollTextTime);
  printStringWithShift("        +++       Please wait ...      +++           ",ScrollTextTime);
  WaitForClockStartWIFI(MaxWiFiConnectLoop); // Try to establish a WiFi Connection
  if (ClockWiFiConnectFail) {
  printStringWithShift("        +++       The WiFi Connection could not be established      +++           ",ScrollTextTime);
  printStringWithShift("        +++       Next attempt - Please wait...      +++           ",ScrollTextTime);} else {
  StatusWarningLED("LOW"); // LED OFF  
  printStringWithShift("        +++  The WiFi connection has been restored  +++           ",ScrollTextTime);
  printStringWithShift("        +++       The clock will Restart ...      +++           ",ScrollTextTime);
  SaveClockStatus("WiFi Connection failed !");} // Save Status of the Clock
  ESP.restart();} // Clock Restart
  else { 
  if (DHTSensor != "NoDHT") {  
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {  
  // Before starting the Access Point - Check Whether DHT Sensor is connected (3 attempts)
    // Check Temperature
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(500);
   if (InTempValue != "!") {
   break;}}
   // Check Humity
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHTSensor == "DHT11") { 
  if (SerialMonitor) {DEBUG_PRINTLN("DHT11 Sensor is Connected. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("DHT22 Sensor is Connected. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("-");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("DHT Sensor is malfunction !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("-"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("No DHT Sensor connected"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}} else {
  InTempValue = F("!"); InHumValue = F("-");
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("No DHT Sensor connected"));
  DEBUG_PRINTLN(F(""));}}

  // No saved Data - Start Access Point (Captive Portal).
  if (SerialMonitor) {  
  DEBUG_PRINTLN(F("Run Access Point ... [LED-Matrix-Clock]")); 
  DEBUG_PRINTLN(F(""));}
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

  //------------------------------------------------
  rtime = runtime(); // Calculate the Clock runtime
  //------------------------------------------------

 // Switch the Display on and off using a Push Button
 if (!await) {MatrixButtonOnOff(false);} 

  // Check whether there is still an Internet Connection
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // Making wireless connection
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Stop server only once
  SaveClockStatus("WiFi Connection: disconnected -/- "+FormatedDateTimeEN(false)); // Save Status of the Clock
  StartStopServers += 1;  
  WebServerStop(); // Stop web Server
  WiFiUDPStop(); // Stop UDP Server
  StatusWarningLED("HIGH");} // LED ON
  if (SerialMonitor) {DEBUG_PRINTLN("WiFi Connection: disconnected - "+FormatedDateTimeEN(false));
  DEBUG_PRINTLN(F("Please wait ..."));} 
  if (Display){printStringWithShift("        +++       WiFi Connection: Disconnected       +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. wait 
  printStringWithShift("        +++       Please wait ...      +++           ",ScrollTextTime);}
  MyWaitLoop(500);}} else { // 0,5 sek. wait
  if (StartStopServers > 0) {
  if (SerialMonitor) {DEBUG_PRINTLN("WiFi Connection Restored - "+FormatedDateTimeEN(false));
  DEBUG_PRINTLN(F("The Clock will Restart..."));}
  if (Display) {sendCmdAll(CMD_SHUTDOWN, 0); // Display short off
  StatusWarningLED("LOW"); // LED OFF
  ClearMatrix();
  printStringWithShift("           +++       WiFi Connection: Restored...       +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift("           +++       The Clock will Restart       +++           ",ScrollTextTime);} 
  ESP.restart();}} // Restart the Clock if there are WiFi problems

// Only when there is a WiFi Connection
if (!ClockWiFiConnectFail) { 
HandleOTAandWebServer();  // OTA and Web Server Query  

if (!await) {
// Start the Clock
if (ClockStart) {
ClockStart = false; // Setup end 
await = true;
MyWaitLoop(50); // short wait
// ****  A Hello at Start-up to the Main Owner or Indication of the WiFi Signal Strength **********************************************************************
String SHT = WiFiSignalStrength(true); // WiFi Signal Strength Reading
if (SHT == "bad" || SHT == "very bad") {
if (SHT == "bad") { 
SHT = cleanText("       +++     Attention: The WiFi Signal Strength is bad !    ---    The Clock will still Start normally...       +++                    ");}   
if (SHT == "very bad") {  
SHT = cleanText("       +++     Attention: The WiFi Signal Strength is very bad !    ---    The Clock starts without any further Online Functions...       +++                    "); 
// Disable all Functions
if (GMCMapFunction) {GMCMapFunction = false;}
if (WeatherFunctions) {WeatherFunctions = false;}
if (NewsDataFunction) {NewsDataFunction = false;}}
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
if (WatchOwner1FirstName != "") {
SHT= "       +++   Hello  "+cleanText(WatchOwner1FirstName)+"    ---    The Clock is started...       +++                    ";} else {
SHT= F("       +++     The Clock is started...     +++                    ");}  
printStringWithShift(SHT.c_str(), ScrollTextTime);} 
// *************************************************************************************************************************************************************
MEP=true; Greetings_and_Holiday(true); MEP=false; // First, greetings and Holidays if available 
if (DoublePointStatus == 0) {dots = 0;} else {dots = 1;}  // Doublepoint Status
StatusWarningLED("CHECK"); // Check LED
// First regular Measurement when DHT Sensor ready 
CheckInHumidityStatus(); // Humidity Measurement

// Measure Radioactivity
if (GMCMapFunction && AccessGMCMap) {
if (GMCMapMidnightAccessDelay(GMCMMAD)) {  
if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // Only if enough RAM is available
printStringWithShift("             ", ScrollTextTime);
printStringWithShift("GMCMap",ScrollTextTime);   // Wait for GMCMap Result
MEP=true; getGMCMapData(); MEP=false; // Get current Radioactivity Values
printStringWithShift("             ", ScrollTextTime); // Run GMCMap from the Display
GMCMapMessage(5,2);}}} // GMCMap Evaluate and display Data
ClockScrollIn(); // Scroll the time in the Display   
await = false;} // Start of the clock - End

  // Midnight Action
  if (MidnightGhost == "YES" || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "3" && Display) {PDP = true;} // DoublePoint on    
  if (String(h) == "0" && String(m) == "0" && String(s) == "4") {
  await = true;
  MyWaitLoop(1000); // short wait
  if (MidnightGhost == "YES" && String(d) + "." + String(mo) != "1.1") {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Show Ghost"));}  
  ShowGhost();} // Ghost at Midnight
  getTimeLocal(); 
  if (EnhancedFunctions) {MEP=true; Greetings_and_Holiday(false); MEP=false; // Determine Greetings and Holidays for the new Day  
  DateString = ExpandedDateEN();}
  if (WeatherFunctions && AccessOpenWeatherMap) {
  if (IsEnoughRAM(MinRAMMem,"Weather") && IsWiFiSignalOK()) {MEP=true; getWeatherData(); MEP=false;}} // Determine new Sunrise and Sunset Times
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ****** Switch off Matrix automatically *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Leave the display on on New Year's Eve
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":5") {PDP = true;} // DoublePoint on 
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6") {     
  await = true;
  Display = false; 
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800); MyWaitLoop(1000);} // Matrix switch off Beep
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix switch off
  digitalWrite(HWLEDPin, LOW); // LED off
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix switched off at "+ String(h) + ":" + String(m));} 
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ***** Switch on Matrix automatically ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") {   
  await = true; 
  ClearMatrix();
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800);} // Matrix turn on Beep
  MyWaitLoop(1500); // short wait
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  CheckInHumidityStatus(); // Humidity Measurement

  // Measure Radioactivity
  if (GMCMapFunction && AccessGMCMap) {
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {   
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // Only if enough RAM is available  
  printStringWithShift("        ", ScrollTextTime); // Run GMCMap from the Display  
  printStringWithShift("GMCMap",ScrollTextTime);  // Wait for GMCMap Result
  MEP=true; getGMCMapData(); MEP=false; // Get current Radioactivity Values
  printStringWithShift("             ", ScrollTextTime); // Run GMCMap from the Display
  GMCMapMessage(5,2);}}} // Evaluate and display GMCMap data or defragment RAM
  ClockScrollIn(); // Scroll the time in the Display
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix switched on at "+ String(h) + ":" + String(m));} 
  Display = true;
  await = false;}}

  // ******************** Begin actual Program *******************************
  if (Display) {
  // Sound at full Hour when activated 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // full Hour Beep
  await = false;}}

  MatrixAction = GoMatrixAction();
  if (MatrixAction) { // No more Actions 10 Minutes before the Matrix switches off automatically!
                          // (for Safe switching off of the Matrix - no possible overlap with an Action)
  // Evaluation of Air Humidity and Radioactivity (optional) on the Hour
  if ((InHumValue != "-") || GMCMapFunction) {
  if  (String(m)+":"+String(s) == "0:7") {PDP = true;} // Colon on
  if  (String(m)+":"+String(s) == "0:8") { 
  await = true;  
  CheckInHumidityStatus(); // Humidity Measurement
  // Download Radioactivity Value 
  if (GMCMapFunction && AccessGMCMap) {
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {  
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // Only if enough RAM is available     
  SIC = true; // ScrollIn Clock  
  printStringWithShift("             ", ScrollTextTime); // Let the Time run out
  printStringWithShift("GMCMap",ScrollTextTime);  // Wait for GMCMap Result
  MEP=true; getGMCMapData(); MEP=false; // Get Current Radioactivity Value
  printStringWithShift("             ", ScrollTextTime); // Run "GMCMap" from the Display
  GMCMapMessage(10,2);}}}  // Evaluate and display GMCMap data or defragment RAM
  if (SIC) {ClockScrollIn(); SIC = false;} // Let the Radioactivity Value run from the Display 
  PDP = false; // Colon permanently off again when Flashing Mode
  await = false;}}

  // ###### Display Times for "OpenWeatherMAP", "NewsData" and "GMCMap" as well as Indoor/Outdoor Temperature and Humidity (DHT Sensor) ####
  // ********** Minute 0 is Reserved for the Evaluation of Humidity and Radioactivity (if Activated) - do not use! ***********
  // Actions every X Minutes
  byte OnlyTempViewTimeCount = 6; // Number of Times
  String OnlyTempViewTime[OnlyTempViewTimeCount] = {F("3"),F("12"),F("21"),F("36"),F("48"),F("57")}; // Only Show Indoor and Outdoor Temperature (Minute)
  byte OnlyHumViewTimeCount  = 5; // Number of Times
  String OnlyHumViewTime[OnlyHumViewTimeCount]   = {F("6"),F("24"),F("33"),F("39"),F("51")}; // Current Date, Greetings and News (Minute) 
  byte DateNewsViewTimeCount = 5; // Number of Times
  String DateNewsViewTime[DateNewsViewTimeCount] = {F("9"),F("18"),F("27"),F("42"),F("54")}; // aktuelles Datum, Greetings und News (Minute)
  byte FullWeatherViewTimeCount = 2; // Number of Times
  String FullWeatherViewTime[FullWeatherViewTimeCount] = {F("15"),F("45")}; // Full Weather (Minute)
  byte GMCMapViewTimeCount = 1; // Number of Times
  String GMCMapViewTime[GMCMapViewTimeCount] = {F("30")}; // Radioactivity at selected Location (or full Weather if Radioactivity is disabled) (Minute)
  // *********************************************************************************************************

  // // only Indoor and Outdoor Temperature --- 3x per Hour (default)
  if ((InTempValue != "!") || WeatherFunctions) {
  for (byte i=0; i < OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Doublepoint on
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
     // *** Indoor Temperature ***
    if (InTempValue != "!") { // DHT active
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();}  // Temperature measurement
    MyWaitLoop(100); // short wait
    String InTempString = "";
    if (InTempValue != "!") {
    IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10);
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
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("DHT Sensor disabled or broken"));}
    StatusWarningLED("LOW"); // LED turn off
    InTempValue = F("-");}} else {StatusWarningLED("LOW");} // LED turn off

    // *** Outdoor Temperature ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(MinRAMMem,"Weather") && IsWiFiSignalOK()) {  // Only if enough RAM is available   
    MEP=true; getWeatherData(); MEP=false; // Get current weather data (download)
    SIC = true; // ScrollIn Clock
    if (InTempValue == "!") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Show Outdoor Temperature
    if (StatusOpenWeatherMap == "OK") {MyWaitLoop(10000);} // short wait (10 sek. - default) 
    printStringWithShift("             ", ScrollTextTime); // Let the Outdoor Temperature run from the display
    }} // or defragment RAM
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Only Indoor and Outdoor Air Humidity --- 3x per Hour (default)
  if ((InHumValue != "-") || WeatherFunctions) {
  for (byte i=0; i < OnlyHumViewTimeCount; i++){
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
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("DHT Sensor disabled or broken"));}}

    // *** Outdoor Air Humidity ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(MinRAMMem,"Weather") && IsWiFiSignalOK()) {  // Only if enough RAM is available  
    MEP=true; getWeatherData(); MEP=false;  // Get current Weather Data (download)
    SIC = true; // ScrollIn Clock
    if (InHumidityStatus == "FAIL") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Display Outdoor Humidity
    if (StatusOpenWeatherMap == "OK") {MyWaitLoop(10000);} // short wait (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Let the outside air humidity run from the display
    }}  // or defragment RAM
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Additional Functions (Date, Holidays, Birthdays) and/or News Displays --- every 10 Minutes
  if (NewsDataFunction || EnhancedFunctions) {
  for (byte i=0; i < DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Doublepoint on      
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
    await = true;
    InHumidityStatus = CheckInHumidity(); // Measuring Indoor Air Humidity
    if (NewsDataFunction && AccessNewsData) {
    if (IsEnoughRAM(MinRAMMem,"News") && IsWiFiSignalOK()) {MEP=true; getNews(); MEP=false;} else {APINews.clear();}} // get actual News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    DateString = ExpandedDateEN(); // Calculate date with calendar week
    SIC = true; // ScrollIn Clock
    printStringWithShift("        ", ScrollTextTime);
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Output date with calendar week
    if (GNCounter == 0) {MEP=true; Greetings_and_Holiday(false); MEP=false;} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Alternately display Greetings and Holidays as well as News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsData Messages 
    // Show Message on Matrix Display if no Greetings
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    InHumidityStatus = CheckInHumidity(); // Measuring Indoor Air Humidity
    if (APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift("        ", ScrollTextTime);
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Display News or defragment RAM
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock 
    waitloop = 0; // Variable reset
    PDP = false; //Doublepoint permanently off again when flashing mode
    await = false;}}}

  // --- Detailed Weather ---
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (byte i=0; i < FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Colon on     
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  InHumidityStatus = CheckInHumidity(); // Measuring Indoor Air Humidity	
  if (IsEnoughRAM(MinRAMMem,"Weather") && IsWiFiSignalOK()) {  // Only if enough RAM is available	
  SIC = true;									   
  MEP=true; getWeatherData(); MEP=false; // Get Current Weather Data
  printStringWithShift("        ", ScrollTextTime);
  printStringWithShift(weatherString.c_str(), ScrollTextTime);} // Scroll into the display to see the time or defragment the RAM.
  if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock 
  PDP = false; // Colon permanently off again when Flashing Mode
  await = false;}}}

  // GMCMap - Radioactivity Display
  if (GMCMapFunction && AccessGMCMap) {
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {   
  for (byte i=0; i < GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Colon on     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true; 
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // Only if enough RAM is available	
  SIC = true;
  printStringWithShift("             ", ScrollTextTime); // Let the time run out
  printStringWithShift("GMCMap",ScrollTextTime);  // Wait for GMCMap Result
  MEP=true; getGMCMapData(); MEP=false; // Get Current Radioactivity Value
  printStringWithShift("             ", ScrollTextTime); // Run "GMCMap" from the Display
  GMCMapMessage(10,2);} // Evaluate and display GMCMap data or defragment RAM
  if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock 
  PDP = false; // Colon permanently off again when Flashing Mode
  await = false;}}}} else { // -------------------------------------------------------------------
  if (WeatherFunctions && AccessOpenWeatherMap) { // Detailed Weather if GMCMap is Disabled
  for (byte i=0; i < GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Colon on     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true;
  if (IsEnoughRAM(MinRAMMem,"Weather") && IsWiFiSignalOK()) {  // Only if enough RAM is available
  SIC = true;	
  MEP=true; getWeatherData(); MEP=false; // Get Current Weather Data
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  printStringWithShift("               ", ScrollTextTime); // Let the time run out
  StatusWarningLED("LOW");}
  if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock 
  PDP = false; // Colon permanently off again when Flashing Mode
  await = false;}}}}
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
}}}
} // Loop - Ende