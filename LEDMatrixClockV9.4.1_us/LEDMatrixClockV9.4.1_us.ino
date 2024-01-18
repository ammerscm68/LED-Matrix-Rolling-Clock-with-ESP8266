/*******************************************************************************************
 ** Author:  Mario Ammerschuber                                                           **
 ** Date:    January 2024                                                                 **
 ** Version: 9.4.1 (English-US)                                                           ** 
 ** Board:   LOLIN (WEMOS) D1 R2 und mini                                                 **  
 ** Sensor:  DHT11 or DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Yellow                                                                   **  
 ** Website: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          **
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "9.4.1 us";
/************************( Importing the libraries )************************/

#include <ESP8266WiFi.h>                    // Install via library
#include <ArduinoOTA.h>                     // Install via library
#include <ArduinoJson.h>                    // Install via library Version: 7
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include <ESP8266WebServer.h>               // Webserver - Install via library
#include <WiFiClient.h>                     // WLAN Client - Install via library
#include <ESP8266HTTPClient.h>              // HTTPClient - Install via library
#include "LittleFS.h"                       // Little Filesystem - Install via library
#include <WiFiUdp.h>
#include <string.h>
#include "LedControl.h"                     // Install via library
#include <DHTesp.h>                         // DHT-11 / DHT-22 Temperature Sensor - Install via library
#include <pitches.h>                        // for passiv Buzzer

#define ROTATE  90 // Display orientation (0,90,180 or 270)

ESP8266WebServer server(80); // Webserver on Port 80     border-radius: 50\%

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
bool DHT11Sensor = true; // true = DHT11  /  false = DHT22   Sensor             (Output the Temperature in Fahrenheit)
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

// #############################################################################################################################################
const int PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const int buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const int HWLEDPin = D5; // LED Pin for to High Humidty Warning (D5 = GPIO 14)
// #############################################################################################################################################

// ###### Display Times for "OpenWeatherMAP" and "NewsAPI" as well as Indoor/Outdoor Temperature and Humidity (DHT-Sensor) ####
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
bool AccessNewsAPI = false;

// NTP Server Pool for US-United States:
static const char ntpServerName[] = "us.pool.ntp.org";    //Find local Server under http://www.pool.ntp.org/zone/us
const int timeZone = 0;                     // 0 when working with <Timezone.h>!
WiFiUDP Udp;
unsigned int localPort = 1701; // local port for listening to UDP packets
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

// -------------------------------- Timezone. ----------------------------------- //
// Edit these Lines According to your Time Zone and Daylight saving Time.
// TimeZone Settings Details https://github.com/JChristensen/Timezone
// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

/*
// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
Timezone tzMSK(msk);

// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);

// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);
*/

/********************( Defining Global Variables and Constants )********************/

// Special Characters  (Font: font[])
char IL  = '~' + 1; // Sign for Indoor Humidity
char IT  = '~' + 2; // Indoor Temperature Sign
char AL  = '~' + 3; // Sign for Outdoor Humidity
char AT  = '~' + 4; // Sign for Outdoor Temperature
char IL2 = '~' + 5; // Sign for Indoor Humidity for > 99
char IT2 = '~' + 6; // Sign for Indoor Temperature for Sub-Zero Temperatures
char AT2 = '~' + 7; // Sign for Outdoor Temperature for Sub-Zero Temperatures
char SM  = '~' + 8; // Sign - Short Minus --> for Sub-Zero Temperatures at Indoor and Outdoor Temperatures
char Face = '~' + 21; // A Face Sign
char Hart = '~' + 22; // A Hart Sign
char   AU = '~' + 23; // Up Arrow Sign
char   AD = '~' + 24; // Down Arrow Sign
char  deg = '~' + 25; // ° Degree Sign
char  uue = '~' + 26; // ü Sign
char  uoe = '~' + 27; // ö Sign
char  uae = '~' + 28; // ä Sign
char  uss = '~' + 29; // ß Sign
char  uAe = '~' + 30; // Ä Sign
char  uOe = '~' + 31; // Ö Sign
char  uUe = '~' + 32; // Ü Sign

// Sonderzeichen und Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Sign
char  MDC   = '9' + 2; // Full Display ON

String WeatherLocation = "";
String StatusOpenWeatherMap = "";
String wresult = ""; // OpenWeatherMap
bool FirstWeatherData = true;

// GreetingsToday
bool GreetingsToday = false;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Start Time LED Matrix Clock

String newsKey = ""; // NewsAPI Key;
String APINews = "";
String StatusNewsAPI = "";

String DisplayOnTime = "";
String DisplayOffTime = "";
String FDOnTime = "";
String FDOffTime = "";
String Brightness = "L1";
bool WeatherFail = false; 
bool NewsFail = false; 
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool ClockWiFiConnectFail = false; // false standard
bool SIC = false; // Scroll In Clock 

String WTemp = "";
String Datum = "";
String Zeit = "";
String SText = "";
String weatherString = "";
String ATemp = "";
String AHum = "";
String DateString = "";
String cip = "";
String cip2 = "";
String date = "";

#define NUM_MAX 4
#define LINE_WIDTH 16

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

// for ESP-01 module
// LedControl lc=LedControl(2,0,3,4);

// for NodeMCU 1.0 or WEMOS D1 mini
LedControl lc=LedControl(15,12,13,4);

int  WaitForNTPIP = 0;
bool bNTPStarted = false;
bool gbIsConnecting = false;
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
String NewsAPILastConnectionDateTime = ""; // last Connection Time NewsAPI
String FormatedDay = "";
String FormatedMonth = "";
const char* wochenstring;
float utcOffset = 2;
long localEpoc = 0;
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
bool NewsAPIFunction = true; // Current News Function ON / OFF - Default = ON (NewsAPI API Key Available)
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
char dl = char('~' + 24); //  // Arrow down for Download
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

// int FT = 0; // Only for BugFix 

/***************************************** ( Setup ) ****************************************/
void setup() {
  // system_update_cpu_freq(SYS_CPU_160MHZ); // Set CPU clock to 160MHz
  if (SerialMonitor) {Serial.begin(115200);} // Initialize Serial Connection with 115200 baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen

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

  // LEDControl.h
  for(int index=0;index<lc.getDeviceCount();index++) {
  lc.shutdown(index,false);} 

   if (SerialMonitor) {
   Serial.println("*** LED-Matrix-Clock ***");
   Serial.println("   (Version: "+LMCVersion+")");
   Serial.println("");
   Serial.println("Number Max7219 Module: "+String(lc.getDeviceCount())); // LEDControl.h
   Serial.println("");}

   // ***First Start***
     // Little-Filesystem format
     // LittleFS.format(); // Delete all Files --> then deactivate them again and upload them again

   LoadMatrixAdjustments(); // Load LED Matrix Clock settings....

    // Init passiv Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (default: buzzerPin = D2)
    noTone(buzzerPin);} // no Sound
     
  // Initialize Matrix
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED Matrix brightness --> 0= Minimum
  MyWaitLoop(1000);

  if (SerialMonitor) {
  if (ssid == "") {
    Serial.println("");
    Serial.println("***** First Start Clock *****"); // First Start Clock
    Serial.println("");
    }}

  WiFi.hostname("LEDMatrixClock"); // Clock Hostname

  if (ssid != ""){
  if (SerialMonitor) {  
  Serial.print("Connect with WiFi SSID: ");
  Serial.println(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // Making wireless Connection
  } else {ClockWiFiConnectFail = true;} // Bypass the WLAN registration when you start the Clock for the first time! - Start the Access Point immediately
  
  if (!ClockWiFiConnectFail) { 
  // Show assigned IP Address on Matrix
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Address:  "+cip+"                "; // Determined IP address (DHCP)

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Display IP Address  MyWaitLoop(10); // short wait

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
  server.on("/owmfoff", WeatherFunctionsOff);}

  // Activate/Deactivate current News
  if (AccessNewsAPI) {
  server.on("/nafon", NewsAPIFunctionOn);
  server.on("/nafoff", NewsAPIFunctionOff);}

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
  server.on("/tempvalue", DHTHTML); 

  // View API Keys
  server.on("/cpadjust", GetAdjustments); 

  if (SerialMonitor) {Serial.println("UDP-Server starten ...");}
  WiFiUDPStart();  // Start UPD Server

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {Serial.println("Start Arduino OTA/DNS-Server ... - URL: http://ledmatrixclock.local");}
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
  Serial.println("");
  Serial.println("OpenWeatherMap has been disabled - No Weather Data");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}

  if (!NewsAPIFunction) {
  Serial.println("");
  Serial.println("NewsAPI has been disabled - No current News");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}}
   
    // Get all information of your LittleFS
    if (littleFSInit) {  
    if (SerialMonitor) {  
    Serial.println(F("Little Filesystem Init - done."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.println("File sistem info.");
    Serial.print("Total space:      ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Total space used: ");
    Serial.print(fs_info.usedBytes);
    Serial.println("byte");
    Serial.print("Block size:       ");
    Serial.print(fs_info.blockSize);
    Serial.println("byte");
    Serial.print("Page size:        ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Max open files:   ");
    Serial.println(fs_info.maxOpenFiles);
    Serial.print("Max path length:  ");
    Serial.println(fs_info.maxPathLength);
    Serial.println();}
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        if (SerialMonitor) {Serial.print(dir.fileName());
        Serial.print(" - ");}
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            if (SerialMonitor) {Serial.println(f.size());}
            f.close();
        }else{if (SerialMonitor) {Serial.println("0");}}}
   if (SerialMonitor) {
   Serial.println("_______________________________");
   Serial.println("");}} else {
   if (SerialMonitor) {  
   Serial.println("Little Filesystem Init - Error.");}}
  if (SerialMonitor) {   
  Serial.println("StartTime: "+FormatedDateTime());
  Serial.println("");
  Serial.println("");}

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

  if (SerialMonitor) {
  if (InTempValue != "-" && InHumValue != "-") {
  Serial.println(""); 
  if (DHT11Sensor) { 
  Serial.println("DHT11 Sensor is Connected. ("+InTempValue+"°F)");} else {
  Serial.println("DHT22 Sensor is Connected. ("+InTempValue+"°F)");}  
  Serial.println("");} else {
  if ((InTempValue != "-" && InHumValue == "-") || (InTempValue == "-" && InHumValue != "-")) {  
  Serial.println("");  
  Serial.println("DHT Sensor is malfunction !");} else {Serial.println("No DHT Sensor connected");}
  Serial.println("");}}  

  // Switch off Display if "Automatic OFF" time is exceeded
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime()) {
  Display = false; 
  ClockStart = false;
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Good morning "+WatchOwner1FirstName+"   Have a nice day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening "+WatchOwner1FirstName+"   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good night "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Good morning... Have a nice day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening ...   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good night  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Display off
  if (SerialMonitor) {Serial.println("Matrix switched off at "+ String(h) + ":" + String(m));}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println("Buzzer not active");}} else {
  if (SerialMonitor) {Serial.println("Buzzer is active");}}

  //saveing millis after setup for Runtime Modul - WLAN connection OK - start the clock
  bootTime = millis();} else { 
  // ++++ If WiFi connection failed ++++
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println("WiFi Connection failed!");}  
  printStringWithShift("        +++  WiFi Connection failed !  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // short Pause
  else { 
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

  if (SerialMonitor) {
  if (InTempValue != "-" && InHumValue != "-") {
  Serial.println(""); 
  if (DHT11Sensor) { 
  Serial.println("DHT11-Sensor is connected. ("+InTempValue+"°F)");} else {
  Serial.println("DHT22-Sensor is connected. ("+InTempValue+"°F)");}  
  Serial.println("");} else {
  if ((InTempValue != "-" && InHumValue == "-") || (InTempValue == "-" && InHumValue != "-")) {  
  Serial.println("");  
  Serial.println("DHT-Sensor is malfunction !");} else {Serial.println("No DHT Sensor connected");}
  Serial.println("");}}  

  // No saved Data - Start Access Point (Captive Portal).
  if (SerialMonitor) {  
  Serial.println("Run Access Point ... [LED-Matrix-Clock]"); 
  Serial.println("");}
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
  if (SerialMonitor) {Serial.println("Please wait ... "+FormatedDateTime());}
  printStringWithShift("        +++       Please wait ...      +++           ",ScrollTextTime);}
  MyWaitLoop(1000);}} else { // 1 sek. wait
  ClockWiFiConnectFail = false;  // // WLAN-Connection OK
  if (StartStopServers == 1)
  { // Restart server only once
  StartStopServers -= 1;
  if (SerialMonitor) {Serial.println("WiFi Connection: restored - "+FormatedDateTime());}
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
  if (StartFail) {if (SerialMonitor) {Serial.println("The Clock will restart...");}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
  printStringWithShift("        +++       The Clock will restart      +++           ",ScrollTextTime); 
  digitalWrite(HWLEDPin, LOW); // LED turn off
  ESP.restart();} // Restart the Clock if there are WiFi problems

if (!await) {
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// if (String(s) == "5")  {MyWaitLoop(1000);getNews();} // News Modul DeBug
// if (String(s) == "5") {MyWaitLoop(1000);getWeatherData();}// Weather Modul Debug
// if (String(s) == "5") {MyWaitLoop(1000);HourBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {MyWaitLoop(1000);StartBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {MyWaitLoop(1000);ShowGhost();} // Show Ghost Debug
// if (String(s) == "5") {MyWaitLoop(1000);Greetings_and_Holiday();} // DeBug Greetings Function
/*if (String(s) == "5" || String(s) == "30" || String(s) == "45") { // FONT Test
await = true; 
FT++;
char  test = '~' + FT; // Zeichen
String TestString = "     "+String(test)+"     ";
if (FT > 7) {FT = 0;} 
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(8500); // short wait
await = false;
} */
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
Greetings_and_Holiday(true); // First, greetings if available 
sendCmdAll(CMD_SHUTDOWN,0); // Display off
if (DoublePointStatus == 0)
{dots = 0;} else {dots = 1;}  // Doublepoint Status
for (int i=0; i <= 20; i++){
getTimeLocal(); // get current local time    
HandleOTAandWebServer();  // OTA and web server query  
getAnimClock(false);} // Show / Calculate Time
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display on
  // First regular measurement when sensor ready 
  if (InTempValue != "-" && InHumValue != "-") { 
  InTempValue = InTempMessure(); // Temperature measurement
  MyWaitLoop(100); // short wait
  InHumValue = InHumMessure(); // Humidity measurement
  if (InTempValue != "-" && InHumValue != "-") {
  IntInTempValue = InTempValue.toInt(); // Measured value as an integer
  IntInHumValue  = InHumValue.toInt(); // Measured value as an integer
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH);} else {
  // Show whether LED is OK
  digitalWrite(HWLEDPin, HIGH); // LED switch on 
  MyWaitLoop(100); 
  digitalWrite(HWLEDPin, LOW);}}} // LED switch off  
ClockScrollIn(); // Scroll the time in the Display 
if (IsBuzzer && StartSound == "On") {
for (int i=0; i <= 20; i++){   
HandleOTAandWebServer();  // OTA and web server query  
getAnimClock(false);}  
StartBuzzerBeep();} // Sound at startup  
await = false;} // Start of the clock - End

  // Midnight Action
  if (MidnightGhost || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "34" && Display) {PDP = true;} // DoublePoint on    
  if (String(h) == "0" && String(m) == "0" && String(s) == "35") {
  await = true;
  MyWaitLoop(1000); // short wait
  if (MidnightGhost && String(d) + "." + String(mo) != "1.1") {
  if (SerialMonitor) {Serial.println("Show Ghost");}  
  ShowGhost();} // Ghost at Midnight
  getTimeLocal(); 
  if (EnhancedFunctions) {Greetings_and_Holiday(false);} // Determine Greetings and Holidays for the new Day 
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ***** Switch on Matrix automatically ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") { 
  await = true; 
  getAnimClock(false); // Show / Calculate Time   
  sendCmdAll(CMD_SHUTDOWN,1); // Matrix turn on
  MyWaitLoop(1500); // short wait
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Good Morning "+WatchOwner1FirstName+"   Have a nice Day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening "+WatchOwner1FirstName+"   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good Night "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Good Morning... Have a nice day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice Day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening ...   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good Night  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  ClockScrollIn(); // Scroll the Time in the Display
  if (SerialMonitor) {Serial.println("Matrix switched on at "+ String(h) + ":" + String(m));}
  Display = true;
  await = false;}}

  // ****** Switch off Matrix automatically *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Leave the display on on New Year's Eve
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":5") {PDP = true;} // Doublepoint on     
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6") {   
  await = true;
  Display = false;  
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Good Morning "+WatchOwner1FirstName+"   Have a nice Day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice Day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening "+WatchOwner1FirstName+"   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good Night "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Good Morning... Have a nice day  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Have a nice Day  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Good evening ...   see you soon +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Good Night  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix turn off
  if (SerialMonitor) {Serial.println("Matrix switched off at "+ String(h) + ":" + String(m));} 
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // ******************** Begin actual Program *******************************
  if (Display) {
  // Sound at full Hour when activated 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // full Hour Beep
  await = false;}} 

  // Evaluation of the Humidity on the Hour if the Sensor is installed
  if (InHumValue != "-") {  
  if  (String(m)+":"+String(s) == "0:4") { 
  PDP = true; // Doublepoint on    
  await = true; 
  InHumValue = InHumMessure();
  if (InHumValue != "-") {
  if (IsNumeric(InHumValue)){  
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED turn on  
  MyWaitLoop(1500); 
  // Humidity too low
  for (int i=0; i <= 2; i++){ // Repeat message 3x
  printStringWithShift("        +++       Attention:  The Humidity at the location of the Clock is too low ... !      +++           ",ScrollTextTime);
  MyWaitLoop(2500);}
  ClockScrollIn();} else {digitalWrite(HWLEDPin, LOW);} // LED turn off 
  if (IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED turn on   
  MyWaitLoop(1500);  
  // Humidity too high
  String HumMessageString = "        +++       Attention: The Humidity at the location of the Clock is too high...!      +++           ";
  for (int i=0; i <= 2; i++){ // Repeat message 3x
  printStringWithShift(HumMessageString.c_str(),ScrollTextTime);
  MyWaitLoop(2500);}
  ClockScrollIn();} else {digitalWrite(HWLEDPin, LOW);}}} else {digitalWrite(HWLEDPin, LOW);}
  PDP = false; // Doublepoint permanently off again when flashing mode
  await = false;}}

  // only Indoor and Outdoor Temperature --- 3x per Hour (default)
  if (InTempValue != "-" || WeatherFunctions) {
  for (int i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":7") {PDP = true;} // Doublepoint on
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":8") {    
    await = true;
    // *** Indoor Temperature ***
    if (InTempValue != "-") { // DHT aktiv
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); // Temperature measurement 
    // InTempValue = 115; // only for Debug
    MyWaitLoop(100); // kurz warten
    InHumValue = InHumMessure(); // Humidity measurement
    String InTempString = "";
    if (InTempValue != "-" && InHumValue != "-") {
    IntInTempValue = InTempValue.toInt(); 
    IntInHumValue  = InHumValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // Turn LED on or off
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Replace Minus with "short" Minus
    if (IntInTempValue > -10) {
    InTempString = "           Indoor Temperature:          "+String(IT) + " " + InTempValue + deg + "F ";} else {
    InTempString = "           Indoor Temperature:          "+String(IT2) + " " + InTempValue + deg + "F";}} else {
    if (IntInTempValue > 9) {
    if (IntInTempValue < 100) {
    InTempString = "           Indoor Temperature:          "+String(IT) + " " + InTempValue + deg + "F ";} else {      
    InTempString = "           Indoor Temperature:          "+String(IT) + " " + InTempValue + deg + "F";}} else {
    InTempString = "           Indoor Temperature:          "+String(IT) + "  " + InTempValue + deg + "F ";}} 
    // if (SerialMonitor) {Serial.println(InTempString);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(InTempString.c_str(), ScrollTextTime); // Show Indoor temperature
    MyWaitLoop(10000); // short wait (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Let the inside Temperature run from the display
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println("DHT-Sensor deaktiviert oder defekt");}
    digitalWrite(HWLEDPin, LOW); // LED turn off
    InTempValue = "-";}} else {digitalWrite(HWLEDPin, LOW);} // LED turn off
    
    // *** Outdoor Temperature ***
    if (WeatherFunctions){
    getWeatherData();// Get current weather data (download)
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);}
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // if (SerialMonitor) {Serial.println(ATemp);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Show Outdoor Temperature
    MyWaitLoop(10000); // short wait (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);}} // Let the Outdoor Temperature run from the display
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Only Indoor and Outdoor Air Humidity --- 3x per Hour (default)
  if (InHumValue != "-"|| WeatherFunctions) {
  for (int i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":7") {PDP = true;} // Doublepoint on     
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":8") {     
    await = true;
    // *** Indoor Air Humidity ***
    if (InTempValue != "-") { // DHT aktiv
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); // Temperature measurement 
    MyWaitLoop(100); // kurz warten 
    InHumValue = InHumMessure(); // Humidity measurement
    String InHumString = "";
    if (InTempValue != "-" && InHumValue != "-") {
    IntInTempValue = InTempValue.toInt();  
    IntInHumValue = InHumValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // Turn LED on or off
    if (IntInHumValue > 99) {
    InHumString = "           Indoor Air Humidity:          "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "           Indoor Air Humidity:          "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "           Indoor Air Humidity:          "+ String(IL) + " " + InHumValue + " % ";}}
    // if (SerialMonitor) {Serial.println(InTempString);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(InHumString.c_str(), ScrollTextTime); // Show indoor temperature
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Let the inside Humidity run from the display
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println("DHT Sensor deactivated or malfunction");}
    digitalWrite(HWLEDPin, LOW); // LED turn off
    InTempValue = "-";}} else {digitalWrite(HWLEDPin, LOW);} // LED turn off
    
    // *** Outdoor Air Humidity ***
    if (WeatherFunctions){
    getWeatherData();// Get current Weather Data (download)
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);}
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // if (SerialMonitor) {Serial.println(ATemp);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Show Humidity Outdoor
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);}} // Let the Outdoor Air Humidity run from the Display
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock
    PDP = false; // Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Additional Functions (Date, Holidays, Birthdays) and/or News Displays --- every 10 Minutes
  if (NewsAPIFunction || EnhancedFunctions) {
  for (int i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":7") {PDP = true;} // Doublepoint on    
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":8") { 
    await = true;
    TriggerHumityLED(); // Humidity LED Switch on or off if Sensor is Present and OK
    if (NewsAPIFunction) {getNews();} // actual News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
    if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
    String dayName[] = {"Err", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    DateString = "          +++         Today is " + dayName[w] + "   " + String(ye) + "-" + FormatedMonth + "-" + FormatedDay + "        +++               ";
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+DateString);}
    if (GNCounter == 0) {Greetings_and_Holiday(false);} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Alternately display Greetings and Holidays as well as News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsAPI Messages 
    // Show Message on Matrix Display if no Greetings
    if (!GreetingsToday && NewsAPIFunction) {
    TriggerHumityLED(); // Humidity LED Switch on or off if Sensor is Present and OK
    if (APINews != "Error" && APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Show News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // ScrollIn Clock   
    waitloop = 0; // Variable reset
    PDP = false; //Doublepoint permanently off again when flashing mode
    await = false;}}}

  // Detailed Weather --- 2x per Hour (default)
  if (WeatherFunctions) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":7") {PDP = true;} // Doublepoint on   
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":8") { 
  await = true;
  TriggerHumityLED(); // Humidity LED Switch on or off if Sensor is Present and OK
    // if (SerialMonitor) {Serial.println("Verbinde mit OpenweatherMap ...");} // only when Display on
    getWeatherData(); // Get current Weather Data (download)
    if (String(WeatherLocation) == "" && StatusOpenWeatherMap == "OK") {
    WeatherFunctions = false;
    if (SerialMonitor) {Serial.println("OpenWeatherMap has been disabled - no Weather Data");} 
    printStringWithShift("     +++    No Weather Data could be loaded !   +++           ", ScrollTextTime); // No Weather Data could be loaded
    // ScrollIn Clock  
    ClockScrollIn();} else { 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- " + weatherString);}
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(weatherString.c_str(), ScrollTextTime);
    ClockScrollIn();}} // ScrollIn Clock 
    PDP = false; //Doublepoint permanently off again when flashing mode
    await = false;}}}

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