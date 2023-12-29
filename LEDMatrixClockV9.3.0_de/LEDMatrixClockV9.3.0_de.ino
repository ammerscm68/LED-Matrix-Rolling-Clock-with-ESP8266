/*******************************************************************************************
 ** Autor:   Mario Ammerschuber                                                           **
 ** Datum:   Dezember 2023                                                                **
 ** Version: 9.3.0 (Deutsch-de)                                                           ** 
 ** Board:   LOLIN (WEMOS) D1 R2 und mini                                                 **  
 ** Sensor:  DHT11 or DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Gelb                                                                     **  
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "9.3.0";
/************************( Importieren der Bibliotheken )************************/

#include <ESP8266WiFi.h>                    // über Bibliothek installieren
#include <ArduinoOTA.h>                     // über Bibliothek installieren
#include <ArduinoJson.h>                    // über Bibliothek installieren Version: 6
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include <ESP8266WebServer.h>               // Webserver - über Bibliothek installieren
#include <WiFiClient.h>                     // WLAN Client - über Bibliothek installieren
#include <ESP8266HTTPClient.h>              // HTTPClient - über Bibliothek installieren
#include "LittleFS.h"                       // Little Filesystem - über Bibliothek installieren
#include <WiFiUdp.h>
#include <string.h>
#include "LedControl.h"                     // über Bibliothek installieren
#include <DHTesp.h>                         // DHT-11 / DHT-22 Temperatursensor - über Bibliothek installieren
#include <pitches.h>                        // for passiv Buzzer

#define ROTATE  90 // Displayausrichtung (0,90,180 or 270)

ESP8266WebServer server(80); // Webserver auf Port 80     border-radius: 50\%

// ###################################### Besitzer der Uhr #########################################################
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
// ##########################################################################################################################################################

// ################ Angabe des deutschen Bundeslandes für die Feiertagsberechnung (siehe ganz unten) ########################################################
String FederalState = ""; // Bundesländer Feiertage (siehe unten) Beispiel: TH = Thüringen / "" = nur Bundesweite Feiertage / NGH = keine deutschen Feiertage
// ##########################################################################################################################################################

// ########################## Access Point (Captive Portal) IP-Adresse ######################################################################################
int APIPA1 = 192; int APIPA2 = 168; int APIPA3 = 4; int APIPA4 = 1; // Default = 192.168.4.1
// ##########################################################################################################################################################

// ##########################################################################################################################################################
int ScrollClockTime  = 80; // Scrollverzögerung bei Animation der Uhrzeit    80 = Default
// ##########################################################################################################################################################

// #################### Temperatur- und Luftfeuchtesensor (DHT 11 oder DHT 22) ##############################################################################
bool DHT11Sensor = false; // true = DHT11  /  false = DHT22   Sensor
// ##########################################################################################################################################################

// ########################## Mitternacht einen Geist anzeigen ##############################################################################################
bool MidnightGhost = true; // true = Geist anzeigen / false= Geist nicht anzeigen
// ##########################################################################################################################################################

// ##########################################################################################################################################################
bool SerialMonitor = false; // Serial-Monitor   true = Eingeschaltet / false = ausgeschaltet
// ##########################################################################################################################################################

// ########################################################################################################################################################## 
int MinComfortHum = 32; // Minimale komfort Luftfeuchte (default = 32) 
int MaxComfortHum = 68; // Maximale komfort Luftfeuchte (default = 68)
// ##########################################################################################################################################################

// ##########################################################################################################################################################
const int PushButtonPin = D3; // Hardware-PushButton-Pin (D3 = GPIO 0)
const int buzzerPin = D2; // passiv Buzzer Init (D2 = GPIO 4) || for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const int HWLEDPin = D5; // LED Pin für Warnung vor zu hoher oder zu niedriger Luftfeuchte  (D5 = GPIO 14)
// ##########################################################################################################################################################

// ###### Anzeigezeiten für "OpenWeatherMAP" und "NewsAPI" sowie Innen-/Außentemperatur- und Luftfeuchte (DHT-Sensor) ####
// ********** Minute 0 ist reserviert für die Auswertung der Luftfeuchtigkeit - nicht verwenden ! ***********

// **** Aktionen alle 3 Minuten ****
int OnlyTempViewTimeCount = 6; // Anzahl der Zeiten
String OnlyTempViewTime[] = {"3","12","21","36","48","57"}; // nur Innen- und  Außentemperatur anzeigen (Minute)
int OnlyHumViewTimeCount = 5; // Anzahl der Zeiten
String OnlyHumViewTime[] = {"6","24","33","39","51"}; // nur Innen- und  Außenluftfeuchte anzeigen (Minute)
int DateNewsViewTimeCount = 5; // Anzahl der Zeiten
String DateNewsViewTime[] = {"9","18","27","42","54"}; // aktuelles Datum, Greetings und News (Minute) // aktuelles Datum, Greetings und News (Minute)
int FullWeatherViewTimeCount = 3; // Anzahl der Zeiten
String FullWeatherViewTime[] = {"15","30","45"}; // Vollständiges Wetter (Minute)

// Aktionen alle 5 Minuten 
/*int OnlyTempViewTimeCount = 3; // Anzahl der Zeiten
String OnlyTempViewTime[] = {"20", "35", "50"}; // nur Innen- und  Außentemperatur anzeigen (Minute)
int OnlyHumViewTimeCount = 3; // Anzahl der Zeiten
String OnlyHumViewTime[] = {"5", "30", "55"}; // nur Innen- und  Außenluftfeuchte anzeigen (Minute)
int DateNewsViewTimeCount = 3; // Anzahl der Zeiten
String DateNewsViewTime[] = {"10", "25", "40"}; // aktuelles Datum, Greetings und News (Minute)
int FullWeatherViewTimeCount = 2; // Anzahl der Zeiten
String FullWeatherViewTime[] = {"15", "45"}; // Vollständiges Wetter (Minute)*/
//##############################################################################################################################################

DHTesp dht; // DHT Sensor für Tepmeraturmessung innen (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = "";     // OpenWeatherMap City-ID 
bool AccessOpenWeatherMap = false;
bool AccessNewsAPI = false;

// NTP Serverpool für Deutschland:
static const char ntpServerName[] = "de.pool.ntp.org";    //Finde lokale Server unter http://www.pool.ntp.org/zone/de
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird !
WiFiUDP Udp;
unsigned int localPort = 1701; // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

// ---------------------------- Timezone. -------------------------------- //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Standard Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
Timezone CE(CEST, CET);

/*
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
Timezone usMT(usMDT, usMST);*/


/********************( Definieren der Globale Variablen und Konstanten)********************/

// Sonderzeichen und Umlaute (Font: font[])
char IL  = '~' + 1; // Zeichen für InnenLuftfeuchte
char IT  = '~' + 2; // Zeichen für InnenTemperatur
char AL  = '~' + 3; // Zeichen für AußenLuftfeuchte
char AT  = '~' + 4; // Zeichen für AußenTemperatur
char IL2 = '~' + 5; // Zeichen für InnenLuftfeuchte für > 99
char IT2 = '~' + 6; // Zeichen für InnenTemperatur für Minusgrade
char AT2 = '~' + 7; // Zeichen für AußenTemperatur für Minusgrade
char SM  = '~' + 8; // Zeichen - kurzes Minus für Minusgrade bei Innen- und AußenTemperatur
char Face = '~' + 21; // Ein Gesicht Zeichen
char Hart = '~' + 22; // Ein Herz Zeichen
char   AU = '~' + 23; // Pfeil nach oben Zeichen
char   AD = '~' + 24; // Pfeil nach unten Zeichen
char  deg = '~' + 25; // ° Grad Zeichen
char  uue = '~' + 26; // ü Zeichen
char  uoe = '~' + 27; // ö Zeichen
char  uae = '~' + 28; // ä Zeichen
char  uss = '~' + 29; // ß Zeichen
char  uAe = '~' + 30; // Ä Zeichen
char  uOe = '~' + 31; // Ö Zeichen
char  uUe = '~' + 32; // Ü Zeichen

// Sonderzeichen und Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Zeichen
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

uint16_t bootTime; // Startzeit LED-Matrix-Clock

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

bool bNTPStarted = false;
bool gbIsConnecting = false;
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
int MaxWiFiConnectLoop = 30;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
bool StartFail = false;
bool Display = true; // Displaymode - EINGESCHALTET
int DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool PDP = false;
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN  (Datum, Feiertage, Geburtstage u.s.w)
bool WeatherFunctions = true; // Wetterdaten Funktion EIN / AUS  -  Standard = EIN (OpenWeatherMap API-Key und City-ID vorhanden)
bool NewsAPIFunction = true; // aktuelle Nachrichten Funktion EIN / AUS  -  Standard = EIN (NewsAPI API-Key vorhanden)
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
char dl = char('~' + 24); //  // arrow down for Download
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

// int FT = 0; // nur für BugFix 

/***************************************** ( Setup ) ****************************************/
void setup() {
  // system_update_cpu_freq(SYS_CPU_160MHZ); // CPU Takt auf 160MHz Einstellen
  if (SerialMonitor) {Serial.begin(115200);} // Serielle Verbindung mit 115200 Baudinitialisieren
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen

// #####################################################################################
// für autobrightness (Photoresistor)
// pinMode(A0, INPUT); 
  // Taster initialisieren
   pinMode(PushButtonPin, INPUT_PULLUP); // (default PushButtonPin = D3)
  // LED Initialisieren 
   pinMode(HWLEDPin, OUTPUT); // (default HWLEDPin = D5)
   // Connect DHT sensor to GPIO 5 (D1) - Temperatur- und Luftfeuchtesensor für Innen
   if (DHT11Sensor) {// DHT 11 Sensor
   dht.setup(5, DHTesp::DHT11);} else { // DHT 22 Sensor
   dht.setup(5, DHTesp::DHT22);}
// #####################################################################################

  // LEDControl.h
  for(int index=0;index<lc.getDeviceCount();index++) {
  lc.shutdown(index,false);} 

   if (SerialMonitor) {
   Serial.println("*** LED-Matrix-Uhr ***");
   Serial.println("   (Version: "+LMCVersion+")");
   Serial.println("");
   Serial.println("Anzahl Max7219 Module: "+String(lc.getDeviceCount())); // LEDControl.h
   Serial.println("");}

   // ***First Start***
     // Little-Filesystem formatieren
     // LittleFS.format(); // alle Dateien löschen -->   danch wieder deaktivieren und nochmal hochladen

   LoadMatrixAdjustments(); // LED-Matrix-Clock Einstellungen laden ....

    // Init passiv Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (default: buzzerPin = D2)
    noTone(buzzerPin);} // kein Ton
     
  // Martrix initialisieren
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
  MyWaitLoop(1000);

  /*for (int i=1; i <= 29; i++){  
  String UEString = String(char('~' + i)); //
  String TestUmlaute = String(i)+"=   "+UEString+"    ";
  printStringWithShift(TestUmlaute.c_str(), 90); // Test Umlaute und Sonderzeichen
  }*/

  if (SerialMonitor) {
  if (ssid == "") {
    Serial.println("");
    Serial.println("***** First Start Clock *****"); // erster Start der Uhr
    Serial.println("");
    }}

  WiFi.hostname("LEDMatrixUhr"); // Hostname der Uhr  

  if (ssid != ""){
  if (SerialMonitor) {  
  Serial.print("Verbinde mit WiFi SSID: ");
  Serial.println(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {ClockWiFiConnectFail = true;} // beim ersten Start der Uhr die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (!ClockWiFiConnectFail) { 
  // vergebene IP-Adresse auf Matrix anzeigen 
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             IP-Adresse:  "+cip+"                "; // Ermittelte IP-Adresse

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Anzeige IP-Adresse  MyWaitLoop(10); // kurze Pause

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
  server.on("/owmfoff", WeatherFunctionsOff);}

  // aktuelle Nachrichten aktivieren / deaktivieren
  if (AccessNewsAPI) {
  server.on("/nafon", NewsAPIFunctionOn);
  server.on("/nafoff", NewsAPIFunctionOff);}

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
  server.on("/tempvalue", DHTHTML); 

  // API-Keys anzeigen
  server.on("/cpadjust", GetAdjustments); 

  if (SerialMonitor) {Serial.println("UDP-Server starten ...");}
  WiFiUDPStart();  // UPD Server starten

  // #######################################################################################
  // Start TCP (HTTP) server
  WebServerStart();
  // Arduino OTA/DNS
  if (SerialMonitor) {Serial.println("Arduino OTA/DNS-Server starten ... - URL: http://ledmatrixuhr.local");}
  ArduinoOTA.setHostname("ledmatrixuhr");
  ArduinoOTA.setPassword("74656"); // default Passwort for OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {Serial.println("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; 
  digitalWrite(HWLEDPin, LOW); ESP.restart();}); // Restart bei OTA Fehler
  // Arduino jetzt OTA/DNS starten
  ArduinoOTA.begin();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  Serial.println("");
  Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}

  if (!NewsAPIFunction) {
  Serial.println("");
  Serial.println("NewsAPI wurde deaktiviert - keine aktuellen Nachrichten");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}}
   
    // Alle Information vom LittleFS (Festspeicher)
    if (littleFSInit) {  
    if (SerialMonitor) {  
    Serial.println(F("Little Filesystem Init - OK."));
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
   Serial.println("Little Filesystem Init - Fehler.");}}
  if (SerialMonitor) {   
  Serial.println("Startzeit: "+FormatedDateTime());
  Serial.println("");
  Serial.println("");}

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

  if (SerialMonitor) {
  if (InTempValue != "-" && InHumValue != "-") {
  Serial.println(""); 
  if (DHT11Sensor) { 
  Serial.println("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");} else {
  Serial.println("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}  
  Serial.println("");} else {
  if ((InTempValue != "-" && InHumValue == "-") || (InTempValue == "-" && InHumValue != "-")) {  
  Serial.println("");  
  Serial.println("DHT-Sensor ist defekt !");} else {Serial.println("kein DHT-Sensor angeschlossen");}
  Serial.println("");}}  

  // Display ausschalten wenn "automatisch AUS" Zeit überschritten
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime()) {
  Display = false; 
  ClockStart = false;
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen "+WatchOwner1FirstName+"   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend "+WatchOwner1FirstName+"   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen ...   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Display aus
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println("Summer (Buzzer) nicht aktiv");}}

  //saveing millis after setup for Runtime Modul - WLAN Verbindung OK - Start der Uhr
  bootTime = millis();} else { 
  // ++++ Wenn WLAN Verbindung fehlgeschlagen ++++
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println("WLAN-Verbindung fehlgeschlagen!");}  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // kurze Pause
  else { 
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

  if (SerialMonitor) {
  if (InTempValue != "-" && InHumValue != "-") {
  Serial.println(""); 
  if (DHT11Sensor) { 
  Serial.println("DHT11-Sensor ist angeschlossen. ("+InTempValue+"°C)");} else {
  Serial.println("DHT22-Sensor ist angeschlossen. ("+InTempValue+"°C)");}  
  Serial.println("");} else {
  if ((InTempValue != "-" && InHumValue == "-") || (InTempValue == "-" && InHumValue != "-")) {  
  Serial.println("");  
  Serial.println("DHT-Sensor ist defekt !");} else {Serial.println("kein DHT-Sensor angeschlossen");}
  Serial.println("");}}  

  // keine gespeicherten Daten - Access Point (Captive Portal) starten
  if (SerialMonitor) {  
  Serial.println("Starte Access Point ... [LED-Matrix-Uhr]"); 
  Serial.println("");}
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
  dots = 1; // Doppelpunkt Dauerleuchten
  wificonnect(false); // WLAN-Verbindung herstellen
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Server nur einmal stoppen
  StartStopServers += 1;  
  WebServerStop(); // Webserver stoppen
  WiFiUDPStop(); // UDP Server stoppen
  }
  if (Display){
  printStringWithShift("        +++  WLAN-Verbindung: getrennt  +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. warten 
  if (SerialMonitor) {Serial.println("Bitte warten ... "+FormatedDateTime());}
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime);}
  MyWaitLoop(1000);}} else { // 1 sek. warten
  ClockWiFiConnectFail = false;  // WLAN-Verbindung OK
  if (StartStopServers == 1)
  { // Server nur einmal neu starten
  StartStopServers -= 1;
  if (SerialMonitor) {Serial.println("WLAN-Verbindung: wieder hergestellt - "+FormatedDateTime());}
  WiFiUDPStart(); // UDP Server starten
  WebServerStart();}} // Webserver starten

  // nur wenn WLAN-Verbindung besteht
  if (!ClockWiFiConnectFail) { 
  HandleOTAandWebServer();  // OTA und Webserver Abfrage  
  if (StartFail) {if (SerialMonitor) {Serial.println("Die Uhr wird neu gestartet...");}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus 
  printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime); 
  digitalWrite(HWLEDPin, LOW); // LED Ausschalten
  ESP.restart();} // Uhr neu starten bei WLAN Problemen

if (!await) {
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// if (String(s) == "5")  {MyWaitLoop(1000);getNews();} // News Modul DeBug
// if (String(s) == "5") {MyWaitLoop(1000);getWeatherData();}// Weather Modul Debug
// if (String(s) == "5") {MyWaitLoop(1000);HourBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {MyWaitLoop(1000);StartBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {MyWaitLoop(1000);ShowGhost();} // Show Ghost Debug
// if (String(s) == "5") {MyWaitLoop(1000);Greetings_and_Holiday(false);} // DeBug Greetings Function
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

// Start der Uhr
if (ClockStart) {
ClockStart = false; // Setup beendet 
await = true; 
// *********************  Ein Hallo beim Start an den Hauptbesitzer **********************************************************
if (WatchOwner1FirstName != "") {
String SHT= "       +++   Hallo  "+cleanText(WatchOwner1FirstName)+"    ---    Die Uhr wird gestartet...       +++                    "; 
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
printStringWithShift("       +++     Die Uhr wird gestartet...     +++                    ", ScrollTextTime);} 
// ***************************************************************************************************************************
Greetings_and_Holiday(true); // Als erstes Greetings wenn vorhanden 
sendCmdAll(CMD_SHUTDOWN,0); // Display aus
if (DoublePointStatus == 0)
{dots = 0;} else {dots = 1;}  // Doppelpunkt Status
for (int i=0; i <= 20; i++){
getTimeLocal(); // aktuelle Zeit holen    
HandleOTAandWebServer();  // OTA und Webserver Abfrage  
getAnimClock(false);} // Uhrzeit anzeigen / berechnen
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display an
  // Erste reguläre Messung wenn Sensor bereit 
  if (InTempValue != "-" && InHumValue != "-") { 
  InTempValue = InTempMessure(); // Temperaturmessung 
  MyWaitLoop(100); // kurz warten
  InHumValue = InHumMessure(); // Luftfeuchtemessung
  if (InTempValue != "-" && InHumValue != "-") {
  IntInTempValue = InTempValue.toInt(); // Messwert als Integer
  IntInHumValue  = InHumValue.toInt(); // Messwert als Integer  
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH);} else {
  // Anzeigen ob LED OK ist
  digitalWrite(HWLEDPin, HIGH); // LED Einschalten 
  MyWaitLoop(100); 
  digitalWrite(HWLEDPin, LOW);}}} // LED Ausschalten  
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
if (IsBuzzer && StartSound == "On") {
for (int i=0; i <= 20; i++){   
HandleOTAandWebServer();  // OTA und Webserver Abfrage  
getAnimClock(false);}  
StartBuzzerBeep();} // Sound beim Start   
await = false;} // Start der Uhr - Ende

  // Mitternachts Aktionen
  if (MidnightGhost || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "34" && Display) {PDP = true;} // Doppelpunkt an      
  if (String(h) == "0" && String(m) == "0" && String(s) == "35") {
  await = true;
  MyWaitLoop(1000); // kurz warten 
  if (MidnightGhost) {ShowGhost();} // Geist um Mitternacht
  getTimeLocal(); 
  if (EnhancedFunctions) {Greetings_and_Holiday(false);} // Grüße und Feiertage für den neuen Tag ermitteln 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ***** Matrix automatisch einschalten ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") { 
  await = true; 
  getAnimClock(false); // Uhrzeit anzeigen / berechnen   
  sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
  MyWaitLoop(1500); // kurz warten
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen "+WatchOwner1FirstName+"  ---   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend "+WatchOwner1FirstName+"   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen ...   ---   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  ClockScrollIn(); // Uhrzeit in Display hineinscrollen
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " eingeschaltet.");}
  Display = true;
  await = false;}}

  // ****** Matrix automatisch abschalten *******
  if (Display && DisplayOffTime != "") {
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":5") {PDP = true;} // Doppelpunkt an     
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6") {   
  await = true;
  Display = false;  
  if (WatchOwner1FirstName != "") { 
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen "+WatchOwner1FirstName+"   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend "+WatchOwner1FirstName+"   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht "+WatchOwner1FirstName+" +++           ";}} else {
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen ...   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");} 
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // ******************** Begin eigentliches Programm *******************************
  if (Display) {
  // Sound zu voller Stunde wenn aktiviert 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // Volle Stunde Beep
  await = false;}} 

  // Auswertung der Luftfeuchtigkeit zur vollen Stunde wenn Sensor installiert ist
  if (InHumValue != "-") {  
  if  (String(m)+":"+String(s) == "0:4") { 
  PDP = true; // Doppelpunkt an      
  await = true; 
  InHumValue = InHumMessure();
  if (InHumValue != "-") {
  if (IsNumeric(InHumValue)){  
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED Einschalten  
  MyWaitLoop(1500); 
  // Luftfeuchte zu niedrig
  for (int i=0; i <= 2; i++){ // Nachricht 3x wiederholen
  printStringWithShift("        +++       Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu niedrig ... !      +++           ",ScrollTextTime);
  MyWaitLoop(2500);}
  ClockScrollIn();} else {digitalWrite(HWLEDPin, LOW);} // LED Ausschalten 
  if (IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED Einschalten   
  MyWaitLoop(1500);  
  // Luftfeuchte zu hoch
  String HumMessageString = "        +++       Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu hoch ... !      +++           ";
  for (int i=0; i <= 2; i++){ // Nachricht 3x wiederholen
  printStringWithShift(HumMessageString.c_str(),ScrollTextTime);
  MyWaitLoop(2500);}
  ClockScrollIn();} else {digitalWrite(HWLEDPin, LOW);}}} else {digitalWrite(HWLEDPin, LOW);}
  PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
  await = false;}}

  // nur Innen- und Außentemperatur --- 3x pro Stunde (default)
  if (InTempValue != "-" || WeatherFunctions) {
  for (int i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":7") {PDP = true;} // Doppelpunkt an  
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":8") {    
    await = true;
    // *** Innentemperatur ***
    if (InTempValue != "-") { // DHT aktiv
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); // Temperaturmessung 
    MyWaitLoop(100); // kurz warten
    InHumValue = InHumMessure(); // Luftfeuchtemessung
    String InTempString = "";
    if (InTempValue != "-" && InHumValue != "-") {
    IntInTempValue = InTempValue.toInt(); 
    IntInHumValue  = InHumValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // LED Ein- oder Ausschalten
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Minus durch "kurzes" Minus ersetzen
    if (IntInTempValue > -10) {
    InTempString = "              Innentemperatur:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "              Innentemperatur:          "+String(IT2) + " " + InTempValue + deg + "C";}} else {
    if (IntInTempValue > 9) {  
    InTempString = "              Innentemperatur:          "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "              Innentemperatur:          "+String(IT) + "  " + InTempValue + deg + "C ";}} 
    // if (SerialMonitor) {Serial.println(InTempString);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(InTempString.c_str(), ScrollTextTime); // Innentemperatur anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Innen-Temperatur aus dem Display laufen lassen
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println("DHT-Sensor deaktiviert oder defekt");}
    digitalWrite(HWLEDPin, LOW); // LED Ausschalten
    InTempValue = "-";}} else {digitalWrite(HWLEDPin, LOW);} // LED Ausschalten
    
    // *** Außentemperatur ***
    if (WeatherFunctions){
    getWeatherData();// aktuelle Wetterdaten holen 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);}
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // if (SerialMonitor) {Serial.println(ATemp);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);}} // Außen-Temperatur aus dem Display laufen lassen
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

// nur Innen- und Außenluftfeuchte --- 3x pro Stunde (default)
  if (InHumValue != "-"|| WeatherFunctions) {
  for (int i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":7") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":8") {     
    await = true;
    // *** Innenluftfeuchte ***
    if (InTempValue != "-") { // DHT aktiv
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); // Temperaturmessung 
    MyWaitLoop(100); // kurz warten 
    InHumValue = InHumMessure(); // Luftfeuchtemessung
    String InHumString = "";
    if (InTempValue != "-" && InHumValue != "-") {
    IntInTempValue = InTempValue.toInt();  
    IntInHumValue = InHumValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // LED Ein- oder Ausschalten
    if (IntInHumValue > 99) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "              Innenluftfeuchte:          "+ String(IL) + " " + InHumValue + " % ";}}
    // if (SerialMonitor) {Serial.println(InTempString);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(InHumString.c_str(), ScrollTextTime); // Innentemperatur anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime); // Innen-Temperatur aus dem Display laufen lassen
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println("DHT-Sensor deaktiviert oder defekt");}
    digitalWrite(HWLEDPin, LOW); // LED Ausschalten
    InTempValue = "-";}} else {digitalWrite(HWLEDPin, LOW);} // LED Ausschalten
    
    // *** Außenluftfeuchte ***
    if (WeatherFunctions){
    getWeatherData();// aktuelle Wetterdaten holen 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);}
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // if (SerialMonitor) {Serial.println(ATemp);} // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    MyWaitLoop(10000); // kurz warten (10 sek. - default)
    printStringWithShift("             ", ScrollTextTime);}} // Außen-Temperatur aus dem Display laufen lassen
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // Zusatzfunktionen (Datum, Feiertage, Geburtstage) und/oder News Anzeigen 
  if (NewsAPIFunction || EnhancedFunctions) {
  for (int i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":7") {PDP = true;} // Doppelpunkt an      
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":8") { 
    await = true;
    TriggerHumityLED(); // Luftfeuchte LED Ein- oder Ausschalten wenn Sensor vorhanden und OK
    if (NewsAPIFunction) {getNews();} // aktuelle News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
    if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
    String dayName[] = {"Err", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
    DateString = "          +++         Heute ist " + dayName[w] + " der  " + FormatedDay + "." + FormatedMonth + "." + String(ye)+ "        +++               ";
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+DateString);}
    if (GNCounter == 0) {Greetings_and_Holiday(false);} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Abwechselnd Grüße und Feiertage sowie News anzeigen
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsAPI Nachtichten 
    // Nachricht auf Matrix-Display anzeigen wenn keine Greetings
    if (!GreetingsToday && NewsAPIFunction) {
    TriggerHumityLED(); // Luftfeuchte LED Ein- oder Ausschalten wenn Sensor vorhanden und OK
    if (APINews != "Fehler" && APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Anzeige News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
    waitloop = 0; // Variable zurücksetzen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

  // Ausführliches Wetter  --- 2x pro Stunde (default)
  if (WeatherFunctions) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":7") {PDP = true;} // Doppelpunkt an     
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":8") { 
  await = true;
  TriggerHumityLED(); // Luftfeuchte LED Ein- oder Ausschalten wenn Sensor vorhanden und OK
    // if (SerialMonitor) {Serial.println("Verbinde mit OpenweatherMap ...");} // nur wenn Display on
    getWeatherData(); // aktuelle Wetterdaten holen
    if (String(WeatherLocation) == "" && StatusOpenWeatherMap == "OK") {
    WeatherFunctions = false;
    if (SerialMonitor) {Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten");} 
    printStringWithShift("     +++    Es konnten keine Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
    // Uhrzeit in Display hineinscrollen  
    ClockScrollIn();} else { 
    // if (SerialMonitor) {Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- " + weatherString);}
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(weatherString.c_str(), ScrollTextTime);
    ClockScrollIn();}} // Uhrzeit in Display hineinscrollen
    PDP = false; // Doppelpunkt permanet wieder aus wenn Blinken Modus
    await = false;}}}

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

/*  +++ Bundesländer Deutschland +++ (for function void Greetings_and_Holiday())
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
            Keine Feiertage (No German Holiday's) = NGH
*/
