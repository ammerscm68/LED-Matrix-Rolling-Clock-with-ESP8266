/*******************************************************************************************
 ** Autor: Mario Ammerschuber                                                             **
 ** Datum: Juni 2023                                                                      **
 ** Version: 7.1.2                                                                        ** 
 ** Board: LOLIN (WEMOS) D1 R2 und mini                                                   **  
 ** Sensor: DHT11 oder DHT22                                                              **
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "7.1.2";
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

#define ROTATE  90 // Displayausrichtung (0,90 or 270)

ESP8266WebServer server(80); // Webserver auf Port 80     border-radius: 50\%

// ####################################################################################################
bool DHT11Sensor = false; // true = DHT11  /  false = DHT22   Sensor
// ####################################################################################################

// ####################################################################################################
bool MidnightGhost = true; // true = show a ghost at midnight / false= not show a ghost at midnight
// ####################################################################################################

// ####################################################################################################
bool SerialMonitor = false; // Serial-Monitor   true = Eingeschaltet(On) / false = ausgeschaltet (Off)
// ####################################################################################################

// Variable für den HTTP Request
//String header;

// #####################################################################################################
// PushButton-Pin (D3 = GPIO 0)
const int PushButtonPin = D3; 
// passiv Buzzer Init
const int buzzerPin = D2; //for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
// #####################################################################################################

DHTesp dht; // DHT Sensor für Tepmeraturmessung innen (default = D1)

String ssid = "";       // SSID of local network 
String TempSSID = "";   // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = "";     // OpenWeatherMap City-ID 
// cityID.reserve()
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

// - Timezone. - //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
Timezone CE(CEST, CET);

/********************( Definieren der Globale Variablen und Konstanten)********************/

// Sonderzeichen und Umlaute (Font: font[])
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
String DateString = "";
String cip = "";
String cip2 = "";
String date = "";

#define NUM_MAX 4
#define LINE_WIDTH 16

// for ESP-01 module
//#define DIN_PIN 2 // D4
//#define CS_PIN  3 // D9/RX
//#define CLK_PIN 0 // D3

// for NodeMCU 1.0 and WEMOS D1 mini
#define DIN_PIN 15  // D8
#define CS_PIN  13  // D7
#define CLK_PIN 12  // D6

#include "max7219.h" // MAX7219 LED-Matrix Display
#include "fonts.h"

// for ESP-01 module
// LedControl lc=LedControl(2,0,3,4);

// for NodeMCU 1.0
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

String GT = ""; // Greetings
String HT = ""; // Holliday

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
int ScrollClockTime = 0;
int ScrollTextTime = 0; 
int waitloop = 0;
int WiFiConnectLoop = 0;
int MaxWiFiConnectLoop = 30;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
bool StartFail = false;
bool Display = true; // Displaymode - EINGESCHALTET
int DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN  (Datum, Feiertage, Geburtstage u.s.w)
bool WeatherFunctions = true; // Wetterdaten Funktion EIN / AUS  -  Standard = EIN (OpenWeatherMap API-Key und City-ID vorhanden)
bool NewsAPIFunction = true; // aktuelle Nachrichten Funktion EIN / AUS  -  Standard = EIN (NewsAPI API-Key vorhanden)
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
char dl = char('~' + 24); //  // arrow down for Download
int InTempCounter = 0; // Innentemperatur
int InHumCounter = 0; // Luftfeuchte
String InTempValue = "-"; // Innentemperatur
String InHumValue = ""; // Luftfeuchte
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset = "";   // siehe Captive Portal
String InHumOffset = "";    // siehe Captive Portal
int InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad)
int InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1%)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String LastInTempMessureDateTime = "k.A.";

// ------------------- Anzeigezeiten -------------------------------------------------------------------------------
// ****** Minute 0 ist reserviert für die Auswertung der Luftfeuchtigkeit - nicht verwenden ! *******
int OnlyTempViewTimeCount = 4;
String OnlyTempViewTime[] = {"5", "20", "35", "50"}; // nur Innen- und  Außentemperatur anzeigen (Minute)
int DateNewsViewTimeCount = 4;
String DateNewsViewTime[] = {"10", "25", "40", "55"}; // aktuelles Datum, Greetings und News (Minute)
int FullWeatherViewTimeCount = 3;
String FullWeatherViewTime[] = {"15", "30", "45"}; // Vollständiges Wetter (Minute)
int DoublePointPermanantTimeCount = 11;
String DoublePointPermanantTime[] = {"5", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55"}; // Vor Aktion Doppelpunkt Permanent anzeigen
//------------------------------------------------------------------------------------------------------------------

/*****************************************( Setup )****************************************/
void setup() {
  // system_update_cpu_freq(SYS_CPU_160MHZ); // CPU Takt auf 160MHz Einstellen
  if (SerialMonitor) {Serial.begin(115200);} // Serielle Verbindung mit 115200 Baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen

// #################################################################################################################
// für autobrightness (Photoresistor)
// pinMode(A0, INPUT); 
  // Taster initialisieren
   pinMode(PushButtonPin, INPUT_PULLUP); // (default PushButtonPin = D3)
   // Connect DHT sensor to GPIO 5 (D1) - Temperatur- und Luftfeuchtesensor für Innen
   if (DHT11Sensor) {// DHT 11 Sensor
   dht.setup(5, DHTesp::DHT11);} else { // DHT 22 Sensor
   dht.setup(5, DHTesp::DHT22);}
// #################################################################################################################  

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

    // Check ob DHT Sensor angeschlossen ist
   for (int i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(500);
   if (InTempValue != "-") {
   break;}}

  if (SerialMonitor) {
  if (InTempValue != "-") {
  Serial.println("");  
  Serial.println("Temperatursensor ist angeschlossen. ("+InTempValue+"°C)");
  Serial.println("");} else {
  Serial.println("");  
  Serial.println("kein Temperatursensor angeschlossen oder Sensor defekt !");
  Serial.println("");}}  

    // Init passiv Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (default: buzzerPin = D2)
    noTone(buzzerPin);} // kein Ton
     
  // Martrix initialisieren
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
  MyWaitLoop(1000);
  // **********************************************************************
  ScrollClockTime  = 80; // Scrollverzögerung der Uhrzeit    80 = Default
  // **********************************************************************

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
  cip2 = "             IP-Adresse:  "+cip+"           "; // Ermittelte IP-Adresse

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Anzeige IP-Adresse  MyWaitLoop(10); // kurze Pause

  // Webserver - bei falschem Link zurück zur Startseite
  server.onNotFound(SiteNotFound);

  // Webserver - Startseite und Helligkeitsmodi
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
  server.on("/zfein", EnhancedFunctionsEin);
  server.on("/zfaus", EnhancedFunctionsAus);

  // Wetterdaten aktivieren / deaktivieren
  if (AccessOpenWeatherMap) {
  server.on("/owmfein", WeatherFunctionsEin);
  server.on("/owmfaus", WeatherFunctionsAus);}

  // aktuelle Nachrichten aktivieren / deaktivieren
  if (AccessNewsAPI) {
  server.on("/nafein", NewsAPIFunctionEin);
  server.on("/nafaus", NewsAPIFunctionAus);}

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
  ArduinoOTA.onEnd([]() {Serial.println("\nEnd");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error;ESP.restart();}); // Restart bei OTA Fehler
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
   Serial.println("Little Filesystem Init - Fehler.");}}
  if (SerialMonitor) {   
  Serial.println("Startzeit: "+FormatedDateTime());
  Serial.println("");
  Serial.println("");}

  greatings(); // Als erstes Greetings wenn vorhanden 

  // Display ausschalten wenn "automatisch AUS" Zeit überschritten
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime()) {
  Display = false; 
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen ...   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Display aus
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println("Summer (Buzzer) nicht aktiv");}}

  bootTime = millis();} //saveing millis after setup for Runtime Modul
  else { // Wenn WLAN Verbindung fehlgeschlagen
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println("WLAN-Verbindung fehlgeschlagen!");}  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // kurze Pause
  else {
  if (SerialMonitor) {  
  Serial.println("Starte Access Point ... [LED-Matrix-Uhr]"); 
  Serial.println("");}
  printStringWithShift("        +++       Access Point wird gestartet      +++       IP-Adresse: 192.168.4.1      +++           ",ScrollTextTime);  
  printCharWithShift('.',ScrollTextTime);
    printCharWithShift('.',ScrollTextTime);
      printCharWithShift('.',ScrollTextTime);
      // ########################################################
       CaptivePortal(); // Start Captive Portal (WiFi-Manager)
      // ######################################################## 
       }
  } 
}

/*************************************(Hauptprogramm)**************************************/

void loop() {  
getTimeLocal(); // aktuelle Zeit holen 
if (ssid == "") {server.handleClient();} else {

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
  if (StartFail) {if (SerialMonitor) {Serial.println("Uhr wird neu gestartet...");}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus 
  printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
  ESP.restart();} // Uhr neu starten bei WLAN Problemen

if (!await) {
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// if (String(s) == "5")  {getNews();} // News Modul DeBug
// if (String(s) == "5") {getWeatherData();}// Weather Modul Debug
// if (String(s) == "5") {HourBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {StartBuzzerBeep();}// Sound DeBug/Config
// if (String(s) == "5") {ShowGhost();} // Show Ghost Debug
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Start der Uhr
if (ClockStart) {
ClockStart = false; // Setup beendet 
await = true; 
sendCmdAll(CMD_SHUTDOWN,0); // Display aus
if (DoublePointStatus == 0)
{dots = 0;} else {dots = 1;}  // Doppelpunkt Status
for (int i=0; i <= 15; i++){
HandleOTAandWebServer();  // OTA und Webserver Abfrage  
getAnimClock(false);} // Uhrzeit anzeigen / berechnen
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display an
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
if (IsBuzzer && StartSound == "On") {
for (int i=0; i <= 15; i++){  
HandleOTAandWebServer();  // OTA und Webserver Abfrage  
getAnimClock(false);}  
StartBuzzerBeep();} // Sound beim Start   
await = false;
} // Start der Uhr - Ende

  // ***** Matrix automatisch einschalten ******
  if (!Display && DisplayOnTime != "") {
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") { 
  await = true; 
  dots = 1; // Doppelpunkt an 
  getAnimClock(false); // Uhrzeit anzeigen / berechnen   
  sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
  MyWaitLoop(1500); // kurz warten
  if (h >= 0 && h < 12)  {SText = "        +++  Guten Morgen ...   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18) {SText = "        +++  Guten Tag  +++           ";} 
  if (h >= 18 && h <= 23) {SText = "        +++  Guten Abend  +++           ";} 
  printStringWithShift(SText.c_str(),ScrollTextTime);
  ClockScrollIn(); // Uhrzeit in Display hineinscrollen
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " eingeschaltet.");}
  Display = true;
  await = false;}}

  // ****** Matrix automatisch abschalten *******
  if (Display && DisplayOffTime != "") {
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6"){ 
  await = true;
  dots = 1; // Doppelpunkt an  
  getAnimClock(true); // Uhrzeit anzeigen / berechnen 
  Display = false;  
  if (h >= 0 && h < 12)  {SText = "        +++  Guten Morgen ...   Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18) {SText = "        +++  Sch"+String(uoe)+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 22) {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 22 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten
  if (SerialMonitor) {Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");} 
  await = false;}}

  // Ghost at Midnight
  if (MidnightGhost) {  
  if (String(h) == "23" && String(m) == "59" && String(s) == "58") {
  await = true; ShowGhost(); await = false;}}

  // ******************** Begin eigentliches Programm *******************************
  if (Display) {
  // Sound zu voller Stunde wenn aktiviert 
  if (IsBuzzer) { 
  if (String(m) == "0" && String(s) == "5" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // Volle Stunde Beep
  await = false;}} 

  // Auswertung der Luftfeuchtigkeit wenn Sensor installiert ist
  if  (String(m)+":"+String(s) == "0:9") {
  await = true;  
  InHumValue = InHumMessure();
  if (InHumValue != "-") {
  if (IsNumeric(InHumValue)){  
  int IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= 32) {
  MyWaitLoop(1500); 
  // Luftfeuchte zu niedrig
  printStringWithShift("        +++       Die Luftfeuchtigkeit in diesem Raum ist zu niedrig !      +++           ",ScrollTextTime);
  ClockScrollIn();}
  if (IntInHumValue >= 68) {
  dots = 1; // Doppelpunkt an
  getAnimClock(true); // Uhrzeit anzeigen / berechnen 
  MyWaitLoop(1500);  
  // Luftfeuchte zu hoch
  String HumMessageString = "        +++       Die Luftfeuchtigkeit in diesem Raum ist zu hoch ! - Bitte L"+String(uue)+"ften...      +++           ";
  printStringWithShift(HumMessageString.c_str(),ScrollTextTime);
  ClockScrollIn();}}}
  await = false;}

  // nur Innen- und Außentemperatur --- alle 10 Minuten
  for (int i=0; i <= OnlyTempViewTimeCount; i++){  
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":8") { 
    await = true;
    // *** Innentemperatur ***
    if (InTempValue != "-") { // DHT aktiv
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); 
    if (InTempValue != "-") {
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    String InTempString = "              Innentemperatur:        "+ InTempValue + deg + "C  "; 
    // Serial.println(InTempString); // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(InTempString.c_str(), ScrollTextTime); // Innentemperatur anzeigen
    MyWaitLoop(4000); // kurz warten
    printStringWithShift("             ", ScrollTextTime); // Innen-Temperatur aus dem Display laufen lassen
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println("DHT-Innentemperatur Sensor deaktiviert");}
            InTempValue = "-";}}
    
    // *** Außentemperatur ***
    if (WeatherFunctions){
    getWeatherData();// aktuelle Wetterdaten holen 
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // Serial.println(ATemp); // Only for Debuging 
    SIC = true; // ScrollIn Clock
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    MyWaitLoop(4000); // kurz warten
    printStringWithShift("             ", ScrollTextTime);}} // Außen-Temperatur aus dem Display laufen lassen
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen
    await = false;}}
    
  // Zusatzfunktionen (Datum, Feiertage, Geburtstage) und/oder News Anzeigen --- alle 10 Minuten
  for (int i=0; i <= DateNewsViewTimeCount; i++){  
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":8"){ 
    await = true;
    if (NewsAPIFunction) {getNews();} // aktuelle News

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
    if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
    String dayName[] = {"Err", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
    DateString = "          +++         Heute ist " + dayName[w] + " der  " + FormatedDay + "." + FormatedMonth + "." + String(ye)+ "        +++               ";
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); 
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+DateString);
    greatings();} // Grüße und Feiertage
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NewsAPI Nachtichten 
    // Nachricht auf Matrix-Display anzeigen wenn keine greetings (Auf Website wird die Nachricht aber angezeigt trotz greetings)
    if (!GreetingsToday && NewsAPIFunction) {
    if (APINews != "Fehler" && APINews != "") {
    SIC = true; // ScrollIn Clock  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Anzeige News
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Uhrzeit in Display hineinscrollen 
    waitloop = 0; // Variable zurücksetzen
    await = false;}}

  // Ausführliches Wetter  --- alle 15 min
  if (WeatherFunctions) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++){  
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":8") { 
  await = true;
    // Serial.println("Verbinde mit OpenweatherMap ..."); // nur wenn Display on
    getWeatherData(); // aktuelle Wetterdaten holen
    if (String(WeatherLocation) == "" && StatusOpenWeatherMap == "OK") {
    WeatherFunctions = false;
    Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"); 
    printStringWithShift("     +++    Es konnten keine Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
    // Uhrzeit in Display hineinscrollen  
    ClockScrollIn();} else { 
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- " + weatherString);
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(weatherString.c_str(), ScrollTextTime);
    ClockScrollIn();}} // Uhrzeit in Display hineinscrollen   
    await = false;}}}

// Doppelpunkt bei diesen Zeiten Dauerleuchten 
if (DoublePointStatus == 2) {  
for (int i=0; i <= DoublePointPermanantTimeCount; i++){  
if  (String(m)+":"+String(s) == DoublePointPermanantTime[i]+":7") {
if (InTempValue != "-" || InHumValue != "-" || WeatherFunctions || NewsAPIFunction) { 
// Doppelpunkt Dauerleuchten für eine Sek. bevor Laufschrift kommt     
dots = 1;} else {
// Doppelpunkt blinken lassen
 if (millis() - dotTime > 500) { 
 dotTime = millis();
 dots = !dots;
 // kurze Pause
 delay(1);}}} else { 
 // Doppelpunkt blinken lassen  
 if (millis() - dotTime > 500){ 
 dotTime = millis();
 dots = !dots;
 delay(1); // kurze Pause
 // Serial.println(String(h)+":"+String(m)+":" + String(s)); // Anzeige Zeit zum Testen
 }}}}
if (DoublePointStatus == 0){
dots = 0;} // Doppelpunkt aus
if (DoublePointStatus == 1){
dots = 1;} // Doppelpunkt Dauerleuchten
} // Display = true 
getAnimClock(true); // Uhrzeit anzeigen / berechnen 
ResetAPCounter(); // Access Point Button Reset alle 2 Minuten
}}}
} // Loop - Ende
