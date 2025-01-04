/*******************************************************************************************
 ** Autor:    Mario Ammerschuber                                                           **
 ** Datum:    Januar 2025                                                                  **
 ** Version:  10.0.2 (Deutsch-de)                                                          ** 
 ** Board:    LOLIN (WEMOS) D1 R2 und mini                                                 **  
 ** Sensor:   DHT11 oder DHT22                                                             **
 ** Buzzer:   Passiv Piezo Buzzer KY-006                                                   **
 ** LED:      3mm Gelb                                                                     **
 ** Webseite: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          ** 
 ** Lizenz:   Creative Commons Zero v1.0 Universal                                         **
 ********************************************************************************************
*/
String LMCVersion = "10.0.2";
/************************( Importieren der Bibliotheken )************************/

#include <ESP8266WiFi.h>                    // über Bibliothek installieren
#include <WiFiClientSecure.h>               // WLAN Client - über Bibliothek installieren
#include <WiFiUdp.h>
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
String BoardName = "Wemos/Lolin D1 mini"; // Boardname des ESP8266

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
// ###########################################################################################################################################################

// ##### Angabe des deutschen Bundeslandes für die Feiertagsberechnung (siehe ganz unten)  - kann ab Version 9.4.0 im Captive Portal geändert werden #########
String FederalState = ""; // Bundesländer Feiertage (siehe unten) 
// ######### z.Bsp.: TH = Thüringen / Leer oder "-" = nur bundesweite Feiertage / ? = Bundesland im Speicher löschen (zBsp. bei Umzug nach Österreich) #######

// ########################## Access Point (Captive Portal) IP-Adresse #######################################################################################
int APIPA1 = 192; int APIPA2 = 168; int APIPA3 = 4; int APIPA4 = 1; // Default = 192.168.4.1
// ###########################################################################################################################################################

// ##########################################################################################################################################################
int ScrollClockTime  = 80; // Scrollverzögerung bei Animation der Uhrzeit    80 = Default
// ##########################################################################################################################################################

// #################### Temperatur- und Luftfeuchtesensor (DHT 11 oder DHT 22) ##############################################################################
bool DHT11Sensor = false; // true = DHT11  /  false = DHT22   Sensor    (Ausgabe der Temperatur in Celsius)
// ##########################################################################################################################################################

// ########################## Mitternacht einen Geist anzeigen ##############################################################################################
bool MidnightGhost = true; // true = Geist anzeigen / false= Geist nicht anzeigen
// ##########################################################################################################################################################

// ##########################################################################################################################################################
bool SerialMonitor = false; // Serial-Monitor   true = Eingeschaltet / false = ausgeschaltet
// ##########################################################################################################################################################

// ########################################################################################################################################################## 
int MinComfortHum = 32; // Minimale komfort Luftfeuchte (default = 32)  - Werte in %
int MaxComfortHum = 68; // Maximale komfort Luftfeuchte (default = 68)  - Werte in %
// ##########################################################################################################################################################

// ########################### !!!!!!!!!!! ##################################################################################################################
bool FormatFileSystem = false; // true = löscht alle gespeicherten Dateien im FileSystem !!!  /  false = Normalzustand
// ##########################################################################################################################################################

// ##########################################################################################################################################################
const int PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const int buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const int HWLEDPin = D5; // LED Pin für Warnung vor zu hoher oder zu niedriger Luftfeuchte oder zu hoher Radioaktivität  (D5 = GPIO 14) -1 = keine LED
// ##########################################################################################################################################################

// ###### Anzeigezeiten für "OpenWeatherMAP", "NewsData" und "GMCMap" sowie Innen-/Außentemperatur- und Luftfeuchte (DHT-Sensor) ####
// ********** Minute 0 ist reserviert für die Auswertung der Luftfeuchtigkeit und der Radioaktivität (wenn aktiviert) - nicht verwenden ! ***********

// **** Aktionen alle X Minuten ****
int OnlyTempViewTimeCount = 6; // Anzahl der Zeiten
String OnlyTempViewTime[] = {"3","12","21","36","48","57"}; // nur Innen- und  Außentemperatur anzeigen (Minute)
int OnlyHumViewTimeCount = 5; // Anzahl der Zeiten
String OnlyHumViewTime[] = {"6","24","33","39","51"}; // nur Innen- und  Außenluftfeuchte anzeigen (Minute)
int DateNewsViewTimeCount = 5; // Anzahl der Zeiten
String DateNewsViewTime[] = {"9","18","27","42","54"}; // aktuelles Datum, Greetings und News (Minute) // aktuelles Datum, Greetings und News (Minute)
int FullWeatherViewTimeCount = 2; // Anzahl der Zeiten
String FullWeatherViewTime[] = {"15","45"}; // Vollständiges Wetter (Minute)
int GMCMapViewTimeCount = 1; // Anzahl der Zeiten
String GMCMapViewTime[] = {"30"}; // Radioaktivität am gewählten Standort (oder vollständiges Wetter wenn Radioaktivität deaktiviert) (Minute)

//##############################################################################################################################################

DHTesp dht; // DHT Sensor für Tepmeraturmessung innen (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = "";     // OpenWeatherMap City-ID 

bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;
bool AccessGMCMap = false;

String WeatherLocation = "";
String StatusOpenWeatherMap = "";
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
int  WSR = 800; // Webserver Refreshrate | default = 800 

// NTP Serverpool für Deutschland:
static const char ntpServerName[] = "de.pool.ntp.org";    //Finde lokale Server unter http://www.pool.ntp.org/zone/de
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird !
WiFiUDP Udp;
unsigned int localPort = 1701; // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// GreetingsToday
bool GreetingsToday = false;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Startzeit LED-Matrix-Clock

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
bool GMCMapAdjustChange = false;
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

int  WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound = "";
String HourSound = "";

String GT =  ""; // Grüße
String GT1 = ""; // Grüße
String HT =  ""; // Feiertage
String HT1 = ""; // Feiertage
int GNCounter = 1; // Zähler für Geburtstagsgrüße und News

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
String GMCMapLastConnectionDateTime = ""; // last Connection Time GMCMap
long localMillisAtUpdate = 0;
int ScrollTextTime = 0; 
int waitloop = 0;
int WiFiConnectLoop = 0;
int MaxWiFiConnectLoop = 30;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
bool StartFail = false;
bool Display = true; // Displaymode - EINGESCHALTET
int DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool PDP = false;
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN  (Datum, Feiertage, Geburtstage u.s.w)
bool WeatherFunctions = true; // Wetterdaten Funktion EIN / AUS  -  Standard = EIN (OpenWeatherMap API-Key und City-ID vorhanden)
bool NewsDataFunction = true; // aktuelle Nachrichten Funktion EIN / AUS  -  Standard = EIN (NewsData API-Key vorhanden)
bool GMCMapFunction = false; // aktuelle Radioaktivität am gewählten Standort
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
int IntInTempValue = -1;
int IntInHumValue = -1;
int humidity = -1; // Luftfeuchte von OpenWeatherMAP
int InTempCounter = 0; // Innentemperatur Counter
int InHumCounter = 0; // Luftfeuchte
String InTempValue = "-"; // Innentemperatur
String InHumValue = ""; // Luftfeuchte
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset = "";   // siehe Captive Portal
String InHumOffset = "";    // siehe Captive Portal
int InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad - in Captive Portal)
int InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1% - in Captive Portal)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "k.A.";

/***************************************** ( Setup ) ****************************************/
void setup() {
// Matrix initialisieren
initMAX7219();

if (SerialMonitor) {
InitSerial(115200, 120); Serial.println("");} //Warten auf COM Verbindung  /  Warten Default= 120

if (SerialMonitor) {
   Serial.println(F("*** LED-Matrix-Uhr ***"));
   Serial.println("  (Version: "+LMCVersion+")");
   Serial.println(F(""));}

LoadMatrixAdjustments(); // LED-Matrix-Clock Einstellungen laden ....
  
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
    Serial.println(F(""));
    Serial.println(F("***** Erster Start der Uhr *****")); // erster Start der Uhr
    Serial.println(F(""));
    }}

  WiFi.hostname("LEDMatrixUhr"); // Hostname der Uhr  

  if (ssid != ""){
  if (SerialMonitor) {  
  Serial.print(F("Verbinde mit WiFi SSID: "));
  Serial.println(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {ClockWiFiConnectFail = true;} // beim ersten Start der Uhr die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (!ClockWiFiConnectFail) { 
  // vergebene IP-Adresse auf Matrix anzeigen 
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Adresse:  "+cip+"                "; // Ermittelte IP-Adresse

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Anzeige IP-Adresse  MyWaitLoop(10); // kurze Pause  

  if (SerialMonitor) {Serial.println(F("UDP-Server starten ..."));}
  WiFiUDPStart();  // UPD Server starten

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {Serial.println(F("Arduino OTA/DNS-Server starten ... - URL: http://ledmatrixuhr.local"));}
  ArduinoOTA.setHostname("ledmatrixuhr");
  ArduinoOTA.setPassword(OTAPassWD); // Passwort for OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {Serial.println("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error;ESP.restart();}); // Restart bei OTA Fehler
  // Arduino jetzt OTA/DNS starten
  ArduinoOTA.begin();
  // Start TCP (HTTP) server
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  Serial.println(F(""));
  Serial.println(F("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}

  if (!NewsDataFunction) {
  Serial.println(F(""));
  Serial.println(F("NewsData wurde deaktiviert - keine aktuellen Nachrichten"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}

  if (!GMCMapFunction) {
  Serial.println(F(""));
  Serial.println(F("GMCMap wurde deaktiviert - keine aktuellen Radioaktivitätswerte"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}}
   
    // Alle Information vom LittleFS (Festspeicher)
    if (littleFSInit) {  
    if (SerialMonitor) {  
    Serial.println(F("Little Filesystem Init - OK."));
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
   Serial.println(F("Little Filesystem Init - Fehler."));}}
  if (SerialMonitor) {   
  Serial.println("Startzeit: "+FormatedDateTimeDE(false));
  Serial.println(F(""));
  Serial.println(F(""));} 

  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Check ob DHT Sensor angeschlossen ist (3 Versuche)
  // Check Temperatur
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

  if (InTempValue == "-") {InTempValue = F("!");}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("DHT-Sensor ist defekt !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("kein DHT-Sensor angeschlossen"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

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
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println(F("Summer (Buzzer) nicht aktiv"));}} else {
  if (SerialMonitor) {Serial.println(F("Summer (Buzzer) ist aktiv"));}}

  DateString = ExpandedDateDE(); // Datum mit Kalenderwoche berechnen

  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(DateString);
  Serial.println(F(""));
  if (IsPushover) { 
  Serial.println("PushOverStatus: aktiviert");} else {
  Serial.println("PushOverStatus: deaktiviert");}}

  //saveing millis after setup for Runtime Modul - WLAN Verbindung OK - Start der Uhr
  bootTime = millis();} else { 
  // ++++ Wenn WLAN Verbindung fehlgeschlagen ++++
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println(F("WLAN-Verbindung fehlgeschlagen!"));}  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // kurze Pause
  else { 
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {  
  // Vor Start des Access Point - Check ob DHT Sensor angeschlossen ist (3 Versuche)
    // Check Temperatur
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

   if (InTempValue == "-") {InTempValue = "!";}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("DHT-Sensor ist defekt !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("kein DHT-Sensor angeschlossen"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

  // keine gespeicherten Daten - Access Point (Captive Portal) starten
  if (SerialMonitor) {  
  Serial.println(F("Starte Access Point ... [LED-Matrix-Uhr]")); 
  Serial.println(F(""));}
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

 // Display per Taster (PushButton) Ein- und Ausschalten
 if (!await) {MatrixButtonOnOff();} 
 
  // Check ob Internetverbindung noch besteht
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // WLAN-Verbindung herstellen
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Server nur einmal stoppen
  StartStopServers += 1;  
  WebServerStop(); // Webserver stoppen
  WiFiUDPStop(); // UDP Server stoppen
  }
  if (Display){
  printStringWithShift("        +++       WLAN-Verbindung: getrennt       +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. warten 
  if (SerialMonitor) {Serial.println("Bitte warten ... "+FormatedDateTimeDE(false));}
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime);}
  MyWaitLoop(1000);}} else { // 1 sek. warten
  ClockWiFiConnectFail = false;  // WLAN-Verbindung OK
  if (StartStopServers == 1)
  { // Server nur einmal neu starten
  StartStopServers -= 1;
  if (SerialMonitor) {Serial.println("WLAN-Verbindung: wieder hergestellt - "+FormatedDateTimeDE(false));}
  WiFiUDPStart(); // UDP Server starten
  WebServerStart(); // Webserver wieder starten
  if (Display) {printStringWithShift("        +++       WLAN-Verbindung: wieder hergestellt...       +++           ",ScrollTextTime);}
  MyWaitLoop(1000);
  if (DoublePointStatus == 0) {
  dots = 0;} else {dots = 1;}  // Doppelpunkt Status
  ClockScrollIn();}} // Uhrzeit in Display hineinscrollen 

  // nur wenn WLAN-Verbindung besteht
  if (!ClockWiFiConnectFail) { 
  HandleOTAandWebServer();  // OTA und Webserver Abfrage  
  if (StartFail) {if (SerialMonitor) {Serial.println(F("Die Uhr wird neu gestartet..."));}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus 
  printStringWithShift("        +++       Die Uhr wird neu gestartet       +++           ",ScrollTextTime); 
  StatusWarningLED("LOW"); // LED Ausschalten
  ESP.restart();} // Uhr neu starten bei WLAN Problemen

if (!await) {
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
/*if (String(s) == "15") {
await = true;
// getGMCMapData();
// printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime);
// ClockScrollIn(); // Uhrzeit in Display hineinscrollen
// getNews();
getWeatherData();
MyWaitLoop(1000); // kurz warten 
await = false;} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*if (String(s) == "3" || String(s) == "10" || String(s) == "30") { // FONT Test
await = true; 
String TestString = cleanText("+++  ä   +++");
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(8500); // kurz warten
await = false;
} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*if (String(s) == "3") { 
await = true;   // FONT Test 2
String TestString = "";
for (int i=1; i <= 50; i++){
char  test = '~' + i; // Zeichen
TestString += " "+String(test)+" ";}
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(1500); // kurz warten
await = false;
} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Start der Uhr
if (ClockStart) {
ClockStart = false; // Setup beendet 
await = true; 
if (IsBuzzer && StartSound == "On") {StartBuzzerBeep();} // Sound beim Start
// ****  Ein Hallo beim Start an den Hauptbesitzer oder Hinweis auf die WiFi Signalstärke **********************************************************************
String SHT = WiFiSignalStrength(true); // WiFi Signalstärke auslesen
if (SHT == "schlecht" || SHT == "sehr schlecht") {
if (SHT == "schlecht") { 
SHT = cleanText("       +++     Achtung: Die WiFi Signalstärke ist schlecht !    ---    Die Uhr wird trotzdem gestartet...       +++                    ");}   
if (SHT == "sehr schlecht") {  
SHT = cleanText("       +++     Achtung: Die WiFi Signalstärke ist sehr schlecht !    ---    Die Uhr wird ohne weitere Online Funktionen gestartet...       +++                    "); 
// Alle Funktionen deaktivieren
if (MidnightGhost) {MidnightGhost = false;}
if (EnhancedFunctions) {EnhancedFunctions = false;}
if (GMCMapFunction) {GMCMapFunction = false;}
if (WeatherFunctions) {WeatherFunctions = false;}
if (NewsDataFunction) {NewsDataFunction = false;}}
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
if (WatchOwner1FirstName != "") {
SHT= "       +++   Hallo  "+cleanText(WatchOwner1FirstName)+"    ---    Die Uhr wird gestartet...       +++                    ";} else {
SHT= F("       +++     Die Uhr wird gestartet...     +++                    ");}  
printStringWithShift(SHT.c_str(), ScrollTextTime);} 
// *************************************************************************************************************************************************************
Greetings_and_Holiday(true); // Als erstes Greetings wenn vorhanden 
if (DoublePointStatus == 0) {dots = 0;} else {dots = 1;}  // Doppelpunkt Status
StatusWarningLED("CHECK"); // LED Prüfen 
// Erste reguläre Messung wenn Sensor bereit 
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus == "MIN" || InHumidityStatus == "MAX") {
StatusWarningLED("HIGH"); // LED an
if (IsPushover) {PushOverAlert(InHumidityStatus);}}}

// Radioaktivität messen
if (GMCMapFunction && AccessGMCMap) { 
printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
getGMCMapData(); // aktuelle Radioaktivitätswerte holen
printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
GMCMapMessage(5,2);} // GMCMap Daten Auswerten und Anzeigen
ClockScrollIn(); // Uhrzeit in Display hineinscrollen   
await = false;} // Start der Uhr - Ende

  // Mitternachts Aktionen
  if (MidnightGhost || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "3" && Display) {PDP = true;} // Doppelpunkt an      
  if (String(h) == "0" && String(m) == "0" && String(s) == "4") {
  await = true;
  MyWaitLoop(1000); // kurz warten 
  if (MidnightGhost && String(d) + "." + String(mo) != "1.1") { 
  if (SerialMonitor) {Serial.println(F("Geisterstunde"));}  
  ShowGhost();} // Geist um Mitternacht
  getTimeLocal();
  if (EnhancedFunctions) {Greetings_and_Holiday(false); // Grüße und Feiertage für den neuen Tag ermitteln 
  DateString = ExpandedDateDE();} // Datum mit Kalenderwoche berechnen
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
  StatusWarningLED("LOW"); // LED Aus wenn Eingeschaltet
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");} 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ***** Matrix automatisch einschalten ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") {   
  await = true; 
  for (int i=0; i <= 50; i++){ 
  MyWaitLoop(10);     
  HandleOTAandWebServer();  // OTA und Webserver Abfrage 
  getAnimClock(false);} // Uhrzeit anzeigen / berechnen 
  printStringWithShift("           ",1);  
  sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 650);} // Matrix Einschalten Beep
  MyWaitLoop(1500); // kurz warten
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
  if (InHumidityStatus != "FAIL") {
  if (InHumidityStatus == "MIN" || InHumidityStatus == "MAX") {
  if (IsPushover) {PushOverAlert(InHumidityStatus);}}}
  // Radioaktivität messen
  if (GMCMapFunction && AccessGMCMap) { 
  printStringWithShift("     ", ScrollTextTime); // Uhrzeit aus dem Display laufen lassen  
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  getGMCMapData(); // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(5,2);} // GMCMap Daten Auswerten und Anzeigen
  ClockScrollIn(); // Uhrzeit in Display hineinscrollen
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " eingeschaltet.");}
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
  if  (String(m)+":"+String(s) == "0:7") {PDP = true;} // Doppelpunkt an
  if  (String(m)+":"+String(s) == "0:8") { 
  await = true;  
  InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
  if (InHumidityStatus != "FAIL") {          
  // Luftfeuchte zu niedrig
  if (InHumidityStatus == "MIN") {
  SIC = true; // ScrollIn Clock   
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Nachricht über Pushover senden  
  for (int i=0; i <= 1; i++) { // Nachricht 2x wiederholen
  printStringWithShift("        +++       Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu niedrig ... !      +++           ",ScrollTextTime);
  MyWaitLoop(1500);}}// Uhrzeit in Display hineinscrollen  
  if (InHumidityStatus == "MAX") {
  SIC = true; // ScrollIn Clock   
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Nachricht über Pushover senden    
  // Luftfeuchte zu hoch
  for (int i=0; i <= 1; i++){ // Nachricht 2x wiederholen
  printStringWithShift("        +++       Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu hoch ... !      +++           ",ScrollTextTime);
  MyWaitLoop(1500);}}} 

  // Radioaktivität Download
  if (GMCMapFunction && AccessGMCMap) {
  SIC = true; // ScrollIn Clock  
  printStringWithShift("             ", ScrollTextTime); // Uhrzeit rauslaufen lassen
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  getGMCMapData(); // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(10,5);} // GMCMap Daten Auswerten und Anzeigen
  if (SIC) {ClockScrollIn(); SIC = false;} // Radioaktivitätswert aus dem Display laufen lassen  
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}

  // nur Innen- und Außentemperatur --- 3x pro Stunde (default)
  if (InTempValue != "-" || WeatherFunctions) {
  for (int i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Doppelpunkt an  
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
    // *** Innentemperatur ***
    if (InTempValue != "-" && InTempValue != "!") { // DHT aktiv
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Temperatur Wert abrufen
    MyWaitLoop(100); // kurz warten
    String InTempString = F("");
    if (InTempValue != "-") {
    IntInTempValue = InTempValue.toInt();  
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
    } else {if (SerialMonitor) {Serial.println(F("DHT-Sensor deaktiviert oder defekt"));}
    StatusWarningLED("LOW"); // LED Ausschalten
    InTempValue = F("-");}} else {StatusWarningLED("LOW");} // LED Ausschalten

    // *** Außentemperatur ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData();// aktuelle Wetterdaten holen 
    SIC = true; // ScrollIn Clock
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Außen-Temperatur aus dem Display laufen lassen
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

// nur Innen- und Außenluftfeuchte --- 3x pro Stunde (default)
  if ((InHumValue != "-" && InHumValue != "!") || WeatherFunctions) {
  for (int i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":10") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":11") { 
    await = true;
    // *** Innenluftfeuchte ***
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
    if (InHumidityStatus != "FAIL") {   
    String InHumString = F("");
    if (IntInHumValue > 99) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + " " + InHumValue + " % ";}}
    SIC = true; // ScrollIn Clock
    if (InTempOffset != "999") {printStringWithShift(InHumString.c_str(), ScrollTextTime); // Innenluftfeuchte anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Innen-Luftfeuchte aus dem Display laufen lassen
    } else {if (SerialMonitor) {Serial.println(F("DHT-Sensor deaktiviert oder defekt"));}}

    // *** Außenluftfeuchte ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData();// aktuelle Wetterdaten holen 
    SIC = true; // ScrollIn Clock
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Außenluftfeuchte anzeigen
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);} // Außen-Luftfeuchte aus dem Display laufen lassen
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // Zusatzfunktionen (Datum, Feiertage, Geburtstage) und/oder News Anzeigen 
  if (NewsDataFunction || EnhancedFunctions) {
  for (int i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
  await = true;
  if (NewsDataFunction && AccessNewsData) {getNews();} // aktuelle News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    DateString = ExpandedDateDE(); // Datum mit Kalenderwoche berechnen
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Datum mit Kalenderwoche ausgeben
    if (GNCounter == 0) {Greetings_and_Holiday(false);} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Abwechselnd Grüße und Feiertage sowie News anzeigen
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsData Nachtichten 
    // Nachricht auf Matrix-Display anzeigen wenn keine Greetings
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    if (APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Anzeige News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
    waitloop = 0; // Variable zurücksetzen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // Ausführliches Wetter  --- 2x pro Stunde (default)
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  getWeatherData(); // aktuelle Wetterdaten holen
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  ClockScrollIn(); // Uhrzeit in Display hineinscrollen
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}}

  // GMCMap - Radioaktivitätsanzeige
  if (GMCMapFunction && AccessGMCMap) { 
  for (int i=0; i <= GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true; 
  printStringWithShift("             ", ScrollTextTime); // Uhrzeit rauslaufen lassen
  printStringWithShift("GMCMap",ScrollTextTime);  // Auf GMCMap Ergebnis warten
  getGMCMapData(); // aktuelle Radioaktivitätswerte holen
  printStringWithShift("             ", ScrollTextTime); // GMCMap aus dem Display laufen lassen
  GMCMapMessage(10,5); // GMCMap Daten Auswerten und Anzeigen
  ClockScrollIn();
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}} else { // -------------------------------------------------------------------
  if (WeatherFunctions && AccessOpenWeatherMap) { // ausführliches Wetter wenn GMCMap deaktiviert
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true;
  getWeatherData(); // aktuelle Wetterdaten holen
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  printStringWithShift("               ", ScrollTextTime); // Uhrzeit aus dem Display laufen lassen
  ClockScrollIn(); // Uhrzeit in Display hineinscrollen
  StatusWarningLED("LOW");
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}}}
  }  // OnMatrixAction() = True

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
ResetAPCounter(); // Access Point Button Reset alle 2 Minuten
}}}
} // Loop - Ende

/*  +++ Bundesländer Deutschland +++ (für Funktion void Greetings_and_Holiday())
 --- bei keiner Angabe (Leerzeichen) werden nur die bundesweiten Feiertage angezeigt. ---
            Baden-Württemberg = BW
            Bayern = BY
            Berlin = BE
            Brandenburg = BB
            Bremen = HB
            Hamburg = HH
            Hessen = HE
            Mecklenburg-Vorpommern = MV
            Niedersachsen = NI
            Nordrhein-Westfalen = NRW
            Rheinland-Pfalz = RP
            Saarland = SL
            Sachsen = SN
            Sachsen-Anhalt = SA
            Schleswig-Holstein = SH
            Thüringen = TH
            ***************************************************************************************
            Keine Feiertage anzeigen = Irgend eine Buchstabenkombination außer der oben z.Bsp. KF
            ***************************************************************************************
*/
