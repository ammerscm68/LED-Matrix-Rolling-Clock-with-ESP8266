/*******************************************************************************************
 ** Autor:    Mario Ammerschuber                                                           **
 ** Datum:    Dezember 2025                                                                **
 ** Version:  11.0.5 (Deutsch-at)                                                          ** 
 ** Board:    LOLIN (WEMOS) D1 R2 und mini                                                 **  
 ** Sensor:   DHT11 oder DHT22                                                             **
 ** Buzzer:   Passiv Piezo Buzzer KY-006                                                   **
 ** LED:      3mm Gelb                                                                     **
 ** Webseite: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          ** 
 ** Lizenz:   Creative Commons Zero v1.0 Universal                                         **
 ********************************************************************************************
*/
const char* LMCVersion = "11.0.5 at";
/************************( Importieren der Bibliotheken )************************/

#include <ESP8266WiFi.h>                    // über Bibliothek installieren
#include <WiFiClientSecure.h>               // WLAN Client - über Bibliothek installieren
#include <ESP8266HTTPClient.h>              // HTTPClient - über Bibliothek installieren
#include <ESP8266WebServer.h>               // Webserver - über Bibliothek installieren
#include <ArduinoOTA.h>                     // über Bibliothek installieren
#include <ArduinoJson.h>                    // über Bibliothek installieren Version: 7
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include "LittleFS.h"                       // Little Filesystem - über Bibliothek installieren
#include <DHTesp.h>                         // DHT-11 / DHT-22 Temperatursensor - über Bibliothek installieren
#include <pitches.h>                        // for passiv Buzzer

ESP8266WebServer server(80); // Webserver auf Port 80     border-radius: 50\%

/********************( Definieren der Globale Variablen und Konstanten)********************/
const char* BoardName = "Wemos/Lolin D1 mini"; // Boardname des ESP8266

/********************( Arduino OTA/DNS Passwort)********************/
const char* OTAPassWD = "74656"; // Default = 74656

// ####################### Besitzer der Uhr #### kann ab Version 10.0.1 im Captive Portal geändert werden ###########
// ************************ Hauptbesitzer ************************************  [for void Greetings_and_Holiday()]
String WatchOwner1FirstName = ""; // Vorname des Hauptbesitzers - Example: Mario
String WatchOwner1LastName  = ""; // Nachname des Hauptbesitzers - Example: Müller
String Gender1 = "";  // male=männlich | female=weiblich
String Birthday1 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 2. Besitzer (Optional) **************************************************
String WatchOwner2FirstName = ""; // // Vorname des 2. Besitzers - Example: Rosi
String WatchOwner2LastName  = ""; // Nachname des 2. Besitzers (Optional) - wird noch nicht verwendet
String Gender2 = "";  // male=männlich | female=weiblich
String Birthday2 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 3. Besitzer (Optional) **************************************************
String WatchOwner3FirstName = ""; // Vorname des 3. Besitzers - Example: Klaus
String WatchOwner3LastName  = ""; // Nachname des 3. Besitzers (Optional) - wird noch nicht verwendet
String Gender3 = "";  // male=männlich | female=weiblich
String Birthday3 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ##############################################################################################################################################################

// ##### Angabe des österreichischen Bundeslandes für die Feiertagsberechnung (siehe ganz unten)  - kann ab Version 9.4.0 im Captive Portal geändert werden #####
String FederalState = ""; // Bundesländer Feiertage (siehe unten) 
// ######### z.Bsp.: B  = Burgenland / Leer oder "-" = nur bundesweite Feiertage / ? = Bundesland im Speicher löschen (zBsp. bei Umzug nach Deutschland) ########

// ########################## Access Point (Captive Portal) IP-Adresse ##########################################################################################
byte APIPA1 = 192; byte APIPA2 = 168; byte APIPA3 = 4; byte APIPA4 = 1; // Default = 192.168.4.1
// ##########################################################################################################################################################

// ##########################################################################################################################################################
byte ScrollClockTime  = 80; // Scrollverzögerung bei Animation der Uhrzeit    80 = Default  Maximum = 255
// ##########################################################################################################################################################

// #################### Temperatur- und Luftfeuchtesensor (DHT 11 oder DHT 22) ##############################################################################
bool DHT11Sensor = true; // true = DHT11  /  false = DHT22   Sensor    (Ausgabe der Temperatur in Celsius)
// ##########################################################################################################################################################

// ########################## Mitternacht einen Geist anzeigen ###################### kann ab Version 10.1.0 im Captive Portal geändert werden ##############
String MidnightGhost = "YES"; // YES = Geist anzeigen / NO = Geist nicht anzeigen
// ##########################################################################################################################################################

// ##########################################################################################################################################################
#define DEBUG 0   // Debugging (Fehlersuche) - 1=aktivieren, 0=deaktivieren
// ##########################################################################################################################################################

// ########################################################################################################################################################## 
int16_t MinComfortHum = 32; // Minimale komfort Luftfeuchte (default = 32)  - Werte in %
int16_t MaxComfortHum = 68; // Maximale komfort Luftfeuchte (default = 68)  - Werte in %
// ##########################################################################################################################################################

// ##########################################################################################################################################################
int16_t WGPMinRAMMem = 12288; // Minimum RAM in Byte  Default = 12288  (Für Wetter, GMC, Pushover)
int16_t NMinRAMMem   = 14024; // Minimum RAM in Byte  Default = 14024  (Für News)
// ##########################################################################################################################################################

// ########################## GMCMapMidnightAccessDelay >> Default = 30. Minute | Minimum = 0 | Maximum = 59 ################################################
const char* GMCMMAD = "30";  // Beispiel: 30 --> Also bis 00:30 kein Download der aktuellen GMCMap-Daten 
// ##########################################################################################################################################################

// ##########################################################################################################################################################
String MCTZ = "Berlin, Frankfurt, Wien, Paris, Madrid"; // Voreinstallung der Zeitzone in welcher sich die Uhr befindet (Änderungen im Captiv Portal möglich)
// ##########################################################################################################################################################

// ##########################################################################################################################################################
String NTPSN = "at.pool.ntp.org";
const char* ntpServerName = "at.pool.ntp.org";  // Finde lokale Server unter http://www.pool.ntp.org/zone
// ##########################################################################################################################################################

// ########################### !!!!!!!!!!! ##################################################################################################################
bool FormatFileSystem = false; // true = löscht alle gespeicherten Dateien im FileSystem !!!  /  false = Normalzustand
// ##########################################################################################################################################################

// ##########################################################################################################################################################
byte MaxWiFiConnectLoop = 20;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN  -  Minimum = 15  Maximum = 255
// ##########################################################################################################################################################

// ##########################################################################################################################################################
const byte PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const byte buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const byte HWLEDPin = D5; // LED Pin für Warnung vor zu hoher oder zu niedriger Luftfeuchte oder zu hoher Radioaktivität  (D5 = GPIO 14) -1 = keine LED
// ##########################################################################################################################################################

DHTesp dht; // DHT Sensor für Tepmeraturmessung innen (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID;     // OpenWeatherMap City-ID 

String rtime = "Bitte warten ..."; // Laufzeit der Uhr
String ClockStatus = ""; // Status der Uhr

bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;
bool AccessGMCMap = false;

String WeatherLocation = "";
String StatusOpenWeatherMap;
bool FirstWeatherData = true;

String SecureAppToken = ""; // Pushover SecureAppToken
String UserToken = ""; // Pushover UserToken
String PushoverDevice1 = ""; // Pushover Gerät (z.Bsp. Smartphone)
String PushoverDevice2 = ""; // Pushover Gerät
String PushoverDevice3 = ""; // Pushover Gerät
String PushoverMessageSound = "none"; // Pushover Nachrichten Sound
bool IsPushover = false; // Pushover default deaktiviert
bool FailPushOver = false; // Bei PushOver Fehler
bool PushOverTest = false; // Test Puschover Geräte Empfänger
int16_t  WSR = 800; // Webserver Refreshrate | default = 800

// NTP-Init (1. Teil)
const byte timeZone = 0; // 0 wenn mit <Timezone.h> gearbeitet wird !
WiFiUDP Udp;
unsigned int localPort = 1701; // lokaler Port zum Abhören von UDP-Paketen

// GreetingsToday
bool GreetingsToday = false;

// DHT-Sensor Select for CaptivPortal
String DHTSensor;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Startzeit LED-Matrix-Clock

String newsKey; // NewsData API-Key;
String APINews = "";
String WPAPINews = "";
String StatusNewsData;
bool NewsOutOfCredits = false;

String DisplayOnTime;
String DisplayOffTime;
String FDOnTime;
String FDOffTime;
String Brightness;
bool MEP = false;  
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool GMCMapAdjustChange = false;
bool ClockWiFiConnectFail = false; // false standard
bool SIC = false; // Scroll In Clock 
bool SerialMonitor = false; // Für Debuging - Default = deaktiviert

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

int16_t WSMinRAMMem;

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

byte WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound;
String HourSound;

String GT =  ""; // Grüße
String GT1 = ""; // Grüße
String HT =  ""; // Feiertage
String HT1 = ""; // Feiertage
byte GNCounter = 1; // Zähler für Geburtstagsgrüße und News

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
byte dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
unsigned long hwsTime1 = 0;
int16_t dx = 0; // muss "int16_t" sein
int16_t dy = 0; // muss "int16_t" sein
int16_t del = 0; // muss "int16_t" sein
int16_t h, m, s, w, mo, ye, d;
String OWMLastConnectionDateTime = ""; // last Connection Time OpenWeatherMap
String NewsDataLastConnectionDateTime = ""; // last Connection Time NewsData
String GMCMapLastConnectionDateTime = ""; // last Connection Time GMCMap
long localMillisAtUpdate = 0;
byte ScrollTextTime = 0; 
int16_t waitloop = 0;
byte WiFiConnectLoop = 0;
bool Display = true; // Displaymode - EINGESCHALTET
byte DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool PDP = false;
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN  (Datum, Feiertage, Geburtstage u.s.w)
bool WeatherFunctions = true; // Wetterdaten Funktion EIN / AUS  -  Standard = EIN (OpenWeatherMap API-Key und City-ID vorhanden)
bool NewsDataFunction = true; // aktuelle Nachrichten Funktion EIN / AUS  -  Standard = EIN (NewsData API-Key vorhanden)
bool GMCMapFunction = false; // aktuelle Radioaktivität am gewählten Standort
byte StartStopServers = 0;
byte APStart = 0;
bool ClockStart = true;
int16_t IntInTempValue = -1;
int16_t IntInHumValue = -1;
int16_t humidity = -1; // Luftfeuchte von OpenWeatherMAP
byte InTempCounter = 0; // Innentemperatur Counter
byte InHumCounter = 0; // Luftfeuchte
String InTempValue; // Innentemperatur
String InHumValue; // Luftfeuchte
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset;   // siehe Captive Portal
String InHumOffset;    // siehe Captive Portal
int16_t InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad - in Captive Portal)
int16_t InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1% - in Captive Portal)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "k.A.";

/***************************************** ( Setup ) ****************************************/
void setup() {

// Matrix initialisieren
initMAX7219();

StringMemoryLimitation(); // Strings begrenzen

// Check Minimum / Maximum "MaxWiFiConnectLoop"
if (MaxWiFiConnectLoop < 15) {MaxWiFiConnectLoop = 15;} 
if (MaxWiFiConnectLoop > 255) {MaxWiFiConnectLoop = 254;}

// Für Anzeige zu wenig RAM auf Website
WSMinRAMMem = min(WGPMinRAMMem, NMinRAMMem); 

#if DEBUG // Debugging (Fehlersuche)
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
InitSerial(115200, 120); DEBUG_PRINTLN("");} //Warten auf COM Verbindung  /  Warten Default= 120 (Maximum: 255)

if (SerialMonitor) {
   DEBUG_PRINTLN(F("*** LED-Matrix-Uhr ***"));
   DEBUG_PRINTLN("  (Version: "+String(LMCVersion)+")");
   DEBUG_PRINTLN(F(""));}

LoadMatrixAdjustments(); // LED-Matrix-Clock Einstellungen laden ....

// NTP-Init (2. Teil)
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
  
// #####################################################################################
// für autobrightness (Photoresistor)
// pinMode(A0, INPUT); 
  // Taster initialisieren
   pinMode(PushButtonPin, INPUT_PULLUP); // (default PushButtonPin = D3)
  // LED Initialisieren 
   if (HWLEDPin != -1) {pinMode(HWLEDPin, OUTPUT); digitalWrite(HWLEDPin, LOW);} // (default HWLEDPin = D5)
   // Connect DHT sensor to GPIO 5 (D1) - Temperatur- und Luftfeuchtesensor für Innen
   if (DHT11Sensor) {// DHT 11 Sensor
   dht.setup(5, DHTesp::DHT11);} else { // DHT 22 Sensor
   dht.setup(5, DHTesp::DHT22);}
// #####################################################################################

    // Init passiv Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (default: buzzerPin = D2)
    noTone(buzzerPin);} // kein Ton
     
  // Little-Filesystem formatieren
  if (FormatFileSystem) {
  if (littleFSInit) {
  LittleFS.format(); 
  printStringWithShift("     +++       Das Filesystem wurde formatiert !  \
  --- Setzen Sie die Variable -FormatFileSystem- wieder auf -false- und \
  laden Sie den Sketch erneut hoch.....",ScrollTextTime);
  printStringWithShift("                              +++\
         Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
  MyWaitLoop(5000); // kurze Pause
  ESP.restart(); // Neustart der Uhr
  }}

  if (SerialMonitor) {
  if (ssid == "") {
    DEBUG_PRINTLN(F(""));
    DEBUG_PRINTLN(F("***** Erster Start der Uhr *****")); // erster Start der Uhr
    DEBUG_PRINTLN(F(""));
    }}

  WiFi.hostname("LEDMatrixUhr"); // Hostname der Uhr  

  if (ssid != ""){
  if (SerialMonitor) {  
  DEBUG_PRINT(F("Verbinde mit WiFi SSID: "));
  DEBUG_PRINTLN(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {ClockWiFiConnectFail = true;} // beim ersten Start der Uhr die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (!ClockWiFiConnectFail) { 
  // vergebene IP-Adresse auf Matrix anzeigen 
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Adresse:  "+cip+"                "; // Ermittelte IP-Adresse

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Anzeige IP-Adresse  MyWaitLoop(10); // kurze Pause  

  if (SerialMonitor) {DEBUG_PRINTLN(F("UDP-Server starten ..."));}
  WiFiUDPStart();  // UPD Server starten

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {DEBUG_PRINTLN(F("Arduino OTA/DNS-Server starten ... - URL: http://ledmatrixuhr.local"));}
  ArduinoOTA.setHostname("ledmatrixuhr");
  ArduinoOTA.setPassword(OTAPassWD); // Passwort for OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {DEBUG_PRINTLN("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {DEBUG_PRINTF("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; ESP.restart();}); // Restart bei OTA Fehler
  // Arduino jetzt OTA/DNS starten
  ArduinoOTA.begin();
  // Start TCP (HTTP) server
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!NewsDataFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("NewsData wurde deaktiviert - keine aktuellen Nachrichten"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!GMCMapFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("GMCMap wurde deaktiviert - keine aktuellen Radioaktivitätswerte"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}}
   
    // Alle Information vom LittleFS (Festspeicher)
    if (littleFSInit) {  
    if (SerialMonitor) {  
    DEBUG_PRINTLN(F("Little Filesystem Init - OK."));
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
   DEBUG_PRINTLN(F("Little Filesystem Init - Fehler."));}}
  if (SerialMonitor) {   
  DEBUG_PRINTLN("Startzeit: "+FormatedDateTimeDE(false));
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F(""));} 

  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Check ob DHT Sensor angeschlossen ist (3 Versuche)
  // Check Temperatur
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "-") {
   break;}} 
   // Check Humity
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}}

  if (InTempValue == "-") {InTempValue = F("!");}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {DEBUG_PRINTLN("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("DHT-Sensor ist defekt !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("kein DHT-Sensor angeschlossen"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}

  // Webserver - bei falschem Link zurück zur Startseite
  server.onNotFound(SiteNotFound);

  // Webserver - Startseite, Helligkeitsmodi und Displaytest
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

  // Status Doppelpunkt
  server.on("/sdflash", DoublePointFlash);
  server.on("/sdon", DoublePointOn);
  server.on("/sdoff", DoublePointOff);

  // Laufschriftgeschwindigkeit
  server.on("/vlt1", ScrollTextTimeLevel1);
  server.on("/vlt2", ScrollTextTimeLevel2);
  server.on("/vlt3", ScrollTextTimeLevel3);
  server.on("/vlt4", ScrollTextTimeLevel4);
  server.on("/vlt5", ScrollTextTimeLevel5);
  server.on("/vlt6", ScrollTextTimeLevel6);
  server.on("/vlt7", ScrollTextTimeLevel7);
  server.on("/vlt8", ScrollTextTimeLevel8);

  // Zusatzfunktionen aktivieren / deaktivieren
  server.on("/zfon", EnhancedFunctionsOn);
  server.on("/zfoff", EnhancedFunctionsOff);

  // Wetterdaten aktivieren / deaktivieren
  if (AccessOpenWeatherMap) {
  server.on("/owmfon", WeatherFunctionsOn);
  server.on("/owmfoff", WeatherFunctionsOff);
  server.on("/aowmn", ActualWeather);}

  // aktuelle Nachrichten aktivieren / deaktivieren
  if (AccessNewsData) {
  server.on("/nafon", NewsDataFunctionOn);
  server.on("/nafoff", NewsDataFunctionOff);
  server.on("/aapin", ActualAPINews);}

  // aktuelle Radioaktivitätsanzeige aktivieren / deaktivieren
  if (AccessGMCMap) {
  server.on("/gmcmapon", GMCMapFunctionOn);
  server.on("/gmcmapoff", GMCMapFunctionOff);
  server.on("/agmcmapd", ActualGMCMapData);}

  // Pushover Testnachricht
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

  // Einstellungen löschen - Reset
  server.on("/reset", ResetClockSettings); 
  server.on("/goreset", ResetClockYes); 

  // Neustart der Uhr
  server.on("/reboot", RebootClock); 

  // Innentemperaturmessung
  if (InTempValue != "!" && InHumValue != "-") {
  server.on("/tempvalue", DHTHTML); 
  server.on("/dhtcalibrate", DHTSensorCalibrate);} 

  // Einstellungen anzeigen
  server.on("/cpadjust", GetAdjustments); 

  // Display ausschalten wenn "automatisch AUS" Zeit überschritten
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime() && String(d) + "." + String(mo) != "31.12") { // Silvester das Display eingeschaltet lassen
  Display = false; 
  ClockStart = false;
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Display aus
  digitalWrite(HWLEDPin, LOW); // LED Ausschalten
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) nicht aktiv"));}} else {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) ist aktiv"));}}

  DateString = ExpandedDateDE(); // Datum mit Kalenderwoche berechnen

  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(DateString);
  DEBUG_PRINTLN(F(""));
  if (IsPushover) { 
  DEBUG_PRINTLN("PushOverStatus: aktiviert");} else {
  DEBUG_PRINTLN("PushOverStatus: deaktiviert");}}

  // Saveing millis after Setup for Runtime Modul - WLAN Verbindung OK - Start der Uhr
  bootTime = millis();} else { 
  // ++++ Wenn WLAN Verbindung fehlgeschlagen ++++
  if (ssid != ""){
  StatusWarningLED("HIGH"); // LED Einschalten   
  if (SerialMonitor) {DEBUG_PRINTLN(F("WLAN-Verbindung fehlgeschlagen!"));}  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime);
  WaitForClockStartWIFI(MaxWiFiConnectLoop); // Versuch WLAN Verbindung herzustellen
  if (ClockWiFiConnectFail) {
  printStringWithShift("        +++       Die WLAN-Verbindung konnte nicht hergestellt werden      +++           ",ScrollTextTime);
  String TA = cleanText("        +++       Nächster Versuch - Bitte warten...      +++           ");
  printStringWithShift(TA.c_str(),ScrollTextTime);} else {
  StatusWarningLED("LOW"); // LED Ausschalten  
  printStringWithShift("        +++  Die WLAN-Verbindung wurde wieder hergestellt  +++           ",ScrollTextTime);
  printStringWithShift("        +++       Die Uhr wird neu gestartet ...      +++           ",ScrollTextTime);
  SaveClockStatus("WLAN-Verbindung fehlgeschlagen!");} // Status der Uhr speichern 
  ESP.restart();} // Neustart der Uhr
  else { 
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {  
  // Vor Start des Access Point - Check ob DHT Sensor angeschlossen ist (3 Versuche)
    // Check Temperatur
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(500);
   if (InTempValue != "-") {
   break;}}
   // Check Humity
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

   if (InTempValue == "-") {InTempValue = "!";}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {DEBUG_PRINTLN("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("DHT-Sensor ist defekt !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("kein DHT-Sensor angeschlossen"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}

  // keine gespeicherten Daten - Access Point (Captive Portal) starten
  if (SerialMonitor) {  
  DEBUG_PRINTLN(F("Starte Access Point ... [LED-Matrix-Uhr]")); 
  DEBUG_PRINTLN(F(""));}
  String APIPAdresse = "        +++       Access Point wird gestartet      +++       IP-Adresse: "+String(APIPA1)+"."+String(APIPA2)+"."+String(APIPA3)+"."+String(APIPA4)+"      +++           ";
  printStringWithShift(APIPAdresse.c_str(),ScrollTextTime);  
  printCharWithShift('.',ScrollTextTime);
    printCharWithShift('.',ScrollTextTime);
      printCharWithShift('.',ScrollTextTime);
      // ########################################################
       CaptivePortal();}}} // Start Captive Portal (WiFi-Manager)
      // ######################################################## 

/*************************************(Hauptprogramm)**************************************/

void loop() {  
  if (ssid == "") {getTimeLocal(); server.handleClient();} else {

  // contrast(); //AutoBrightness

  //---------------------------------------------------------------
  rtime = runtime(); // Laufzeit der Uhr berechnen (Clock Runtime)
  //---------------------------------------------------------------

  // Display per Taster (PushButton) Ein- und Ausschalten
  if (!await) {MatrixButtonOnOff(false);} 
  
  // Check ob Internetverbindung noch besteht
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // WLAN-Verbindung herstellen
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Server stoppen
  StartStopServers += 1;  
  SaveClockStatus("WLAN-Verbindung getrennt -/- "+FormatedDateTimeDE(false)); // Status der Uhr speichern 
  WebServerStop(); // Webserver stoppen
  WiFiUDPStop(); // UDP Server stoppen
  StatusWarningLED("HIGH");} // LED Einschalten
  if (SerialMonitor) {DEBUG_PRINTLN("WLAN-Verbindung: getrennt - "+FormatedDateTimeDE(false));
  DEBUG_PRINTLN(F("Bitte warten ..."));} 
  if (Display){printStringWithShift("        +++       WLAN-Verbindung: getrennt       +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. warten 
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime);}
  MyWaitLoop(500);}} else { // 0,5 sek. warten
  if (StartStopServers > 0) {
  if (SerialMonitor) {DEBUG_PRINTLN("WLAN-Verbindung: wieder hergestellt - "+FormatedDateTimeDE(false));
  DEBUG_PRINTLN(F("Die Uhr wird neu gestartet..."));}
  if (Display) {sendCmdAll(CMD_SHUTDOWN, 0); // Display kurz aus
  StatusWarningLED("LOW"); // LED Ausschalten
  ClearMatrix();
  printStringWithShift("           +++       WLAN-Verbindung: wieder hergestellt...       +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift("           +++       Die Uhr wird neu gestartet       +++           ",ScrollTextTime);} 
  ESP.restart();}}

  // nur wenn WLAN-Verbindung besteht
  if (!ClockWiFiConnectFail) {HandleOTAandWebServer();  // OTA und Webserver Abfrage  

if (!await) {
// +++++++++++++++++++++ Debug ++++++++++++++++++++++++++++++++++ 
/*if (String(s) == "3") {
await = true;
// MEP=true; getGMCMapData(); MEP=false;
// printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime);
// ClockScrollIn(); // Uhrzeit in Display hineinscrollen
MEP=true; getNews(); MEP=false;
printStringWithShift(APINews.c_str(), ScrollTextTime); // Anzeige News
ClockScrollIn(); // Uhrzeit in Display hineinscrollen
// MEP=true; getWeatherData(); MEP=false;
// printStringWithShift(weatherString.c_str(), ScrollTextTime);
// ClockScrollIn(); // Uhrzeit in Display hineinscrollen
MyWaitLoop(1000); // kurz warten 
await = false;} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Start der Uhr
if (ClockStart) {
ClockStart = false; // Setup beendet 
await = true; 
MyWaitLoop(50); // kurz warten
if (IsBuzzer && StartSound == "On") {StartBuzzerBeep();} // Sound beim Start
// ****  Ein Hallo beim Start an den Hauptbesitzer oder Hinweis auf die WiFi Signalstärke **********************************************************************
String SHT = WiFiSignalStrength(true); // WiFi Signalstärke auslesen
if (SHT == "schlecht" || SHT == "sehr schlecht") {
if (SHT == "schlecht") { 
SHT = cleanText("       +++     Achtung: Die WiFi Signalstärke ist schlecht !    ---    Die Uhr wird trotzdem normal gestartet...       +++                    ");}   
if (SHT == "sehr schlecht") {  
SHT = cleanText("       +++     Achtung: Die WiFi Signalstärke ist sehr schlecht !    ---    Die Uhr wird ohne weitere Online Funktionen gestartet...       +++                    "); 
// Alle Funktionen deaktivieren
if (GMCMapFunction) {GMCMapFunction = false;}
if (WeatherFunctions) {WeatherFunctions = false;}
if (NewsDataFunction) {NewsDataFunction = false;}}
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
if (WatchOwner1FirstName != "") {
SHT= "       +++   Hallo  "+cleanText(WatchOwner1FirstName)+"    ---    Die Uhr wird gestartet...       +++                    ";} else {
SHT= F("       +++     Die Uhr wird gestartet...     +++                    ");}  
printStringWithShift(SHT.c_str(), ScrollTextTime);} 
// *************************************************************************************************************************************************************
MEP=true; Greetings_and_Holiday(true); MEP=false; // Als erstes Greetings wenn vorhanden 
if (DoublePointStatus == 0) {dots = 0;} else {dots = 1;}  // Doppelpunkt Status
StatusWarningLED("CHECK"); // LED Prüfen 
// Erste reguläre Messung wenn Sensor bereit 
CheckInHumidityStatus(); // Innenluftfeuchte Messen wenn Sensor angeschlossen und OK

// Radioaktivität messen
if (GMCMapFunction && AccessGMCMap && GMCMapMidnightAccessDelay(GMCMMAD)) {
if (IsEnoughRAM(WGPMinRAMMem,"GMC")) {  // nur wenn genügend RAM zur Verfügung steht  
printStringWithShift("             ", ScrollTextTime);
printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
MEP=true; getGMCMapData(); MEP=false; // aktuelle Radioaktivitätswerte holen
printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
GMCMapMessage(5,2);} else {freeRam();}}  // GMCMap Daten Auswerten und Anzeigen
ClockScrollIn(); // Uhrzeit in Display hineinscrollen   
await = false;} // Start der Uhr - Ende

  // Mitternachts Aktionen
  if (MidnightGhost == "YES" || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "3" && Display) {PDP = true;} // Doppelpunkt an      
  if (String(h) == "0" && String(m) == "0" && String(s) == "4") {
  await = true;
  MyWaitLoop(1000); // kurz warten 
  if (MidnightGhost == "YES" && String(d) + "." + String(mo) != "1.1") { 
  if (SerialMonitor) {DEBUG_PRINTLN(F("Geisterstunde"));}  
  ShowGhost();} // Geist um Mitternacht
  getTimeLocal();
  if (EnhancedFunctions) {MEP=true; Greetings_and_Holiday(false); MEP=false; // Grüße und Feiertage für den neuen Tag ermitteln 
  DateString = ExpandedDateDE();} // Datum mit Kalenderwoche berechnen
  if (WeatherFunctions && AccessOpenWeatherMap && IsEnoughRAM(WGPMinRAMMem,"Wetter")) {MEP=true; getWeatherData(); MEP=false;} // Neuen Sonnenauf- und Sonnenuntergang ermitteln
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ****** Matrix automatisch abschalten *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Silvester das Display eingeschaltet lassen
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":5") {PDP = true;} // Doppelpunkt an     
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6") {     
  await = true;
  Display = false; 
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 650); MyWaitLoop(1000);} // Matrix Abschalten Beep 
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten
  digitalWrite(HWLEDPin, LOW); // LED Ausschalten
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");} 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ***** Matrix automatisch einschalten ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") {   
  await = true; 
  ClearMatrix();
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 650);} // Matrix Einschalten Beep
  MyWaitLoop(1500); // kurz warten
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  CheckInHumidityStatus(); // Innenluftfeuchte Messen wenn Sensor angeschlossen und OK

  // Radioaktivität messen
  if (GMCMapFunction && AccessGMCMap && GMCMapMidnightAccessDelay(GMCMMAD)) { 
  if (IsEnoughRAM(WGPMinRAMMem,"GMC")) {  // nur wenn genügend RAM zur Verfügung steht  
  printStringWithShift("        ", ScrollTextTime); // Uhrzeit aus dem Display laufen lassen  
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  MEP=true; getGMCMapData(); MEP=false; // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(5,2);} else {freeRam();}} // GMCMap Daten Auswerten und Anzeigen oder RAM defragmentieren
  ClockScrollIn();  // Uhrzeit in Display hineinscrollen
  if (SerialMonitor) {DEBUG_PRINTLN("Matrix um "+ String(h) + ":" + String(m) + " eingeschaltet.");}
  Display = true;
  await = false;}}

  // ******************** Begin eigentliches Programm *******************************

  if (Display) {
  // Sound zu voller Stunde wenn aktiviert 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // Volle Stunde Beep
  await = false;}} 

  if (GoMatrixAction()) { // 10 Minuten vor dem automatischen Abschalten der Matrix keine Aktionen mehr ! 
                          // (für sicheres Abschalten der Matrix - keine eventuelle Überlappung mit einer Aktion )
  // Auswertung der Luftfeuchtigkeit und der Radioaktiovität (Optional) zur vollen Stunde
  if ((InHumValue != "-" && InHumValue != "!") || GMCMapFunction) {
  if  (String(m)+":"+String(s) == "0:7") {PDP = true;} // Doppelpunkt an
  if  (String(m)+":"+String(s) == "0:8") { 
  await = true;  
  CheckInHumidityStatus(); // Innenluftfeuchte Messen wenn Sensor angeschlossen und OK
  // Radioaktivität Download
  if (GMCMapFunction && AccessGMCMap && GMCMapMidnightAccessDelay(GMCMMAD)) {
  if (IsEnoughRAM(WGPMinRAMMem,"GMC")) {  // nur wenn genügend RAM zur Verfügung steht  
  SIC = true; // ScrollIn Clock  
  printStringWithShift("             ", ScrollTextTime); // Uhrzeit rauslaufen lassen
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  MEP=true; getGMCMapData(); MEP=false; // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(10,5);} else {freeRam();}} // GMCMap Daten Auswerten und Anzeigen oder RAM defragmentieren
  if (SIC) {ClockScrollIn(); SIC = false;} // Radioaktivitätswert aus dem Display laufen lassen  
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ###### Anzeigezeiten für "OpenWeatherMAP", "NewsData" und "GMCMap" sowie Innen-/Außentemperatur- und Luftfeuchte (DHT-Sensor) ####
  // **** Minute 0 ist reserviert für die Auswertung der Luftfeuchtigkeit und der Radioaktivität (wenn aktiviert) - nicht verwenden ! *****
  // **** Aktionen alle X Minuten ****
  byte OnlyTempViewTimeCount = 6; // Anzahl der Zeiten
  String OnlyTempViewTime[OnlyTempViewTimeCount] = {F("3"),F("12"),F("21"),F("36"),F("48"),F("57")}; // nur Innen- und  Außentemperatur anzeigen (Minute)
  byte OnlyHumViewTimeCount  = 5; // Anzahl der Zeiten
  String OnlyHumViewTime[OnlyHumViewTimeCount]   = {F("6"),F("24"),F("33"),F("39"),F("51")}; // nur Innen- und  Außenluftfeuchte anzeigen (Minute)
  byte DateNewsViewTimeCount = 5; // Anzahl der Zeiten
  String DateNewsViewTime[DateNewsViewTimeCount] = {F("9"),F("18"),F("27"),F("42"),F("54")}; // aktuelles Datum, Greetings, Holidays und News (Minute)
  byte FullWeatherViewTimeCount = 2; // Anzahl der Zeiten
  String FullWeatherViewTime[FullWeatherViewTimeCount] = {F("15"),F("45")}; // Vollständiges Wetter (Minute)
  byte GMCMapViewTimeCount = 1; // Anzahl der Zeiten
  String GMCMapViewTime[GMCMapViewTimeCount] = {F("30")}; // Radioaktivität am gewählten Standort (oder vollständiges Wetter wenn Radioaktivität deaktiviert) (Minute)
  // *****************************************************************************************************************************************************************

  // ---------------- nur Innen- und Außentemperatur ------------------------ 
  if (InTempValue != "-" || WeatherFunctions) {
  for (byte i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Doppelpunkt an  
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
    // *** Innentemperatur ***
    if (InTempValue != "-" && InTempValue != "!") { // DHT aktiv
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Temperatur Wert abrufen
    MyWaitLoop(100); // kurz warten
    String InTempString;
    if (InTempValue != "-") {
    IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10); 
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Minus durch "kurzes" Minus ersetzen
    if (IntInTempValue > -10) {
    InTempString = "              Innentemperatur:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "              Innentemperatur:          "+String(IT2) + " " + InTempValue + deg + "C";}} else {
    if (IntInTempValue > 9) {  
    InTempString = "              Innentemperatur:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "              Innentemperatur:          "+String(IT) + "  " + InTempValue + deg + "C ";}} 
    SIC = true; // ScrollIn Clock
    if (InTempOffset != "999") {printStringWithShift(InTempString.c_str(), ScrollTextTime); // Innentemperatur anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Innen-Temperatur aus dem Display laufen lassen
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("DHT-Sensor deaktiviert oder defekt"));}
    StatusWarningLED("LOW"); // LED Ausschalten
    InTempValue = F("-");}} else {StatusWarningLED("LOW");} // LED Ausschalten

    // *** Außentemperatur ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(WGPMinRAMMem,"Wetter")) {  // nur wenn genügend RAM zur Verfügung steht  
    MEP=true; getWeatherData(); MEP=false; // aktuelle Wetterdaten holen 
    SIC = true; // ScrollIn Clock
    if (InTempValue == "-" || InTempValue == "!") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    if (StatusOpenWeatherMap == "OK") {MyWaitLoop(10000);} // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Außen-Temperatur aus dem Display laufen lassen
    } else {freeRam();}} // oder RAM defragmentieren
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

// ----------- nur Innen- und Außenluftfeuchte  ---------------------- 
  if ((InHumValue != "-" && InHumValue != "!") || WeatherFunctions) {
  for (byte i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":10") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":11") { 
    await = true;
    // *** Innenluftfeuchte ***
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
    if (InHumidityStatus != "FAIL") {   
    String InHumString;
    if (IntInHumValue > 99) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + " " + InHumValue + " % ";}}
    SIC = true; // ScrollIn Clock
    if (InTempOffset != "999") {printStringWithShift(InHumString.c_str(), ScrollTextTime); // Innenluftfeuchte anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Innen-Luftfeuchte aus dem Display laufen lassen
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("DHT-Sensor deaktiviert oder defekt"));}}

    // *** Außenluftfeuchte ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(WGPMinRAMMem,"Wetter")) {  // nur wenn genügend RAM zur Verfügung steht  
    MEP=true; getWeatherData(); MEP=false; // aktuelle Wetterdaten holen 
    SIC = true; // ScrollIn Clock
    if (InHumidityStatus == "FAIL") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Außenluftfeuchte anzeigen
    if (StatusOpenWeatherMap == "OK") {MyWaitLoop(10000);} // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Außen-Luftfeuchte aus dem Display laufen lassen
    } else {freeRam();}}  // oder RAM defragmentieren
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // Zusatzfunktionen (Datum, Feiertage, Geburtstage) und/oder News Anzeigen 
  if (NewsDataFunction || EnhancedFunctions) {
  for (byte i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
  await = true;
  InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
  if (NewsDataFunction && AccessNewsData) {MEP=true; getNews(); MEP=false;} // aktuelle News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    DateString = ExpandedDateDE(); // Datum mit Kalenderwoche berechnen
    SIC = true; // ScrollIn Clock
    printStringWithShift("        ", ScrollTextTime);
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Datum mit Kalenderwoche ausgeben
    if (GNCounter == 0) {MEP=true; Greetings_and_Holiday(false); MEP=false;} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Abwechselnd Grüße und Feiertage sowie News anzeigen
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsData Nachtichten 
    // Nachricht auf Matrix-Display anzeigen wenn keine Greetings
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    if (IsEnoughRAM(NMinRAMMem,"News")) {  // nur wenn genügend RAM zur Verfügung steht
    if (APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift("          ", ScrollTextTime);
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} else {freeRam();}} // Anzeige News oder RAM defragmentieren
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
    waitloop = 0; // Variable zurücksetzen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // ---------- Ausführliches Wetter ----------------
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (byte i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  if (IsEnoughRAM(WGPMinRAMMem,"Wetter")) {  // nur wenn genügend RAM zur Verfügung steht
  SIC = true; // ScrollIn Clock
  MEP=true; getWeatherData(); MEP=false; // aktuelle Wetterdaten holen
  printStringWithShift("          ", ScrollTextTime);
  printStringWithShift(weatherString.c_str(), ScrollTextTime);} else {freeRam();} // Uhrzeit in Display hineinscrollen oder RAM defragmentieren
  if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}}

  // GMCMap - Radioaktivitätsanzeige
  if (GMCMapFunction && AccessGMCMap && GMCMapMidnightAccessDelay(GMCMMAD)) { 
  for (byte i=0; i <= GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true; 
  if (IsEnoughRAM(WGPMinRAMMem,"GMC")) {  // nur wenn genügend RAM zur Verfügung steht
  SIC = true; // ScrollIn Clock
  printStringWithShift("             ", ScrollTextTime); // Uhrzeit rauslaufen lassen
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  MEP=true; getGMCMapData(); MEP=false; // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(10,5);} else {freeRam();} // GMCMap Daten Auswerten und Anzeigen oder RAM defragmentieren
  if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}} else { // -------------------------------------------------------------------
  if (WeatherFunctions && AccessOpenWeatherMap) { // ausführliches Wetter wenn GMCMap deaktiviert
  for (byte i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true;
  if (IsEnoughRAM(WGPMinRAMMem,"Wetter")) {  // nur wenn genügend RAM zur Verfügung steht
  SIC = true; // ScrollIn Clock
  MEP=true; getWeatherData(); MEP=false; // aktuelle Wetterdaten holen
  printStringWithShift(weatherString.c_str(), ScrollTextTime); 
  printStringWithShift("               ", ScrollTextTime); // Uhrzeit aus dem Display laufen lassen
  StatusWarningLED("LOW");} else {freeRam();}
  if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}}}
  }  // GoMatrixAction() = True

// Doppelpunkt blinken lassen
if (DoublePointStatus == 2) { 
 if (millis() - dotTime > 500) { 
 dotTime = millis();
 if (!PDP) {dots = !dots;} else {dots = 1;}
 // kurze Pause
 delay(1);}}
// Doppelpunkt aus 
if (DoublePointStatus == 0) {dots = 0;} 
// Doppelpunkt Dauerleuchten
if (DoublePointStatus == 1) {dots = 1;} 
} // Display = true 
getAnimClock(true); // Uhrzeit anzeigen / berechnen 
}}}
} // Loop - Ende

/*  +++ Bundesländer Österreich +++ (für Funktion void Greetings_and_Holiday())
 --- bei keiner Angabe (Leerzeichen) werden nur die bundesweiten Feiertage angezeigt. ---
           B  = Burgenland
           K  = Kärnten 
           ST = Steiermark 
           N  = Niederösterreich
           O  = Oberösterreich 
           S  = Salzburg
           T  = Tirol 
           V  = Vorarlberg
           W  = Wien 
***************************************************************************************
Keine Feiertage anzeigen = Irgend eine Buchstabenkombination außer der oben z.Bsp. KF
***************************************************************************************
*/