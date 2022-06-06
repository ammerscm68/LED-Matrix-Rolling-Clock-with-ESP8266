
/*******************************************************************************************
 ** Autor: Mario Ammerschuber                                                             **
 ** Datum: Juni 2022                                                                      **
 ** Version: 3.0                                                                          ** 
 ** Lizenz:                                                                               **
 *******************************************************************************************
*/

/************************( Importieren der Bibliotheken )************************/

#include "Arduino.h"
#include <ESP8266WiFi.h>                    
#include <ArduinoJson.h>                   
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include <ESP8266WebServer.h>               // Webserver
#include <WiFiClient.h>                     // WLAN Client
// #include <ESP8266WebServerSecure.h>      // Webserver Secure
#include "LittleFS.h"                       // Little Filesystem
#include <WiFiUdp.h>
#include <string.h>
#include <DNSServer.h>
#include <strings_en.h>
#include <WiFiManager.h>
#include <EEPROM.h>

#define ROTATE  90 // Displayausrichtung

ESP8266WebServer server(80); // Webserver auf Port 80
// Variable für den HTTP Request
String header;

// Taster-Pin (D3 = 0)
int TasterPin = D3;

const char* ssid = "";     // SSID of local network 
String lssid = ""; // SSID of local network 
const char* password = "";   // Password on network 
String lpassword = ""; // SSID of local network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = ""; // OpenWeatherMap City-ID 
bool AccessOpenWeatherMap = false;

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

WiFiClient client; // WLAN Client
WiFiManager wifiManager; // Instanz von WiFiManager
/********************( Definieren der Globale Variablen und Konstanten)********************/
String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String StatusOpenWeatherMap = "Fehler";
char OWMapiKey[40]; 
char OWMCityID[10]; 
String Brightness = "L1";
bool WeatherFail = false; 
bool ClockWiFiConnectFail = false;
String country;
int humidity;
int pressure;
float temp;
float tempMin, tempMax;
int clouds;
float windSpeed;
int windDeg;
String date;

String WTemp, Datum, Zeit;
String weatherString, ATemp, DateString;
String ip;

#define NUM_MAX 4
#define LINE_WIDTH 16

// for ESP-01 module
//#define DIN_PIN 2 // D4
//#define CS_PIN  3 // D9/RX
//#define CLK_PIN 0 // D3

// for NodeMCU 1.0
#define DIN_PIN 15  // D8
#define CS_PIN  13  // D7
#define CLK_PIN 12  // D6

#include "max7219.h"
#include "fonts.h"

bool bNTPStarted = false;
bool gbIsConnecting = false;
bool await = false;
bool littleFSInit = true;

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
String FormatedDay = "";
String FormatedMonth = "";
const char* wochenstring;
float utcOffset = 2;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int ScrollClockTime = 0;
int ScrollTextTime = 0; 
String STT = "";
int waitloop = 0;
int OWwaitloop = 0;
int MaxWaitLoop = 550; // Maximale Anzahl Loops bei Verbindung mit OpenWeaherMap
int WiFiConnectLoop = 0;
int MaxWiFiConnectLoop = 15;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
int Display = 1; // Displaymode - EINGESCHALTET
int DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
String DPS = "1"; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN
String EF = "";
int StartStopServers = 0;
int APStart = 0;

// Matrix On / Off
void MatrixOn();
void MatrixOff();

// Status Doppelpunkt
void DoublePointFlash(); // DoublePointStatus = 2
void DoublePointOn(); // DoublePointStatus = 1
void DoublePointOff(); // DoublePointStatus = 0

// Helligkeitsmodi Max7219
void BrightnessLevel1();
void BrightnessLevel2();
void BrightnessLevel3();
void BrightnessLevel4();
void BrightnessLevel5();

// Geschwindigkeit der Laufschrift
void ScrollTextTimeLevel1();
void ScrollTextTimeLevel2();
void ScrollTextTimeLevel3();

//flag for saving data WiFiManager
bool shouldSaveConfig = false;
 
//callback notifying us of the need to save config WiFiManager
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

/*****************************************( Setup )****************************************/
void setup()
{
  Serial.begin(115200); // Serielle Verbindung mit 115200 Baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen
  
  // pinMode(A0, INPUT); // für autobrightness
  
  // Taster initialisieren
  pinMode(TasterPin, INPUT_PULLUP); 
  
  // WEMOS D1 Little-Filesystem aktivieren
    Serial.println("");
    Serial.println("Inizializing LittleFS...");
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    delay(250); //waits for data

// *** Einstellungen laden .... ***
if (littleFSInit == true)
{
  // Little-Filesystem formatieren
  // LittleFS.format();
  
  // Helligkeit der LED-Matrix
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // bei leerer Datei oder Fehler
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);}
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);}
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);}
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}
  // Doppelpunktstatus
  DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // bei leerer Datei oder Fehler
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}
  // ScrollText Geschwindigkeit
  STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "" || STT == "40") {ScrollTextTime = 40;}
  if (STT == "30") {ScrollTextTime = 30;}
  if (STT == "50") {ScrollTextTime = 50;}
  EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}
  // Zugangsdaten WiFi laden
  lssid = SetupLoad("ssid.txt");
  lpassword = SetupLoad("ssid-passwd.txt");
  ssid = lssid.c_str();
  password = lpassword.c_str();
  // Zugangsdaten OpenWeatherMap laden
  weatherKey = SetupLoad("owm-apikey.txt");
  cityID = SetupLoad("owm-cityid.txt");
  weatherKey.toCharArray(OWMapiKey, 40);  // API-Key
  cityID.toCharArray(OWMCityID, 10);  // City-ID
  
  // Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  // Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OAK] API-Key: " + weatherKey); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OCI] City-ID: " + cityID); // Only for Debuging 
  // weatherKey = ""; // nur zum Test
  // cityID = ""; // nur zum Test
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    Brightness = "L1";
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    EnhancedFunctions = false;
    weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
    cityID = "";
    }

  // Zusatzfunktionen deaktivieren wenn kein API-Key oder City-ID von OpenWeatherMap vorhanden
  if (weatherKey != "" && cityID != "") {
     AccessOpenWeatherMap = true;}
     else {AccessOpenWeatherMap = false;}

  // WiFiManager initialisieren
  Serial.println("WiFiManager (Accees Point) initialisieren ");
  // wifiManager.resetSettings(); // alle Einstellungen zurücksetzen
  // wifiManager.setCountry("DE"); // Ländereinstellung
  wifiManager.setTimeout(600); // Nach 600 sekunden AccessPoint abschalten
  wifiManager.setConnectTimeout(10);
  // wifiManager.setSaveConnect(false); // do not connect, only save
  wifiManager.setConfigPortalTimeout(300); //sets timeout until configuration portal gets turned off - useful to make it all retry or go to sleep in seconds
  wifiManager.setDarkMode(false); // Dunkler Hintergrund
  wifiManager.setScanDispPerc(true);  // show scan RSSI as percentage, instead of signal stength graphic
  wifiManager.setBreakAfterConfig(true); // needed to use saveWifiCallback
  // wifiManager.setParamsPage(true); // move params to seperate page, not wifi, do not combine with setmenu!
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // andere IP als die aktuelle
    
  // Martrix initialisieren
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
  delay(1000);
  // *******************************************************
  ScrollClockTime  = 80; // Scrollverzögerung der Uhrzeit    80 = Default
  // *******************************************************
  Serial.println("Verbinde mit WiFi ");
  WiFi.begin(ssid, password);
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(); // WLAN Verbindung herstellen
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Serial.println("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (ClockWiFiConnectFail == false)
  { 
  Serial.println("");
  Serial.print("Verbunden: "); Serial.println(WiFi.localIP());
  ip = "        IP-Adresse:  "+(String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3]+"                  "; // Ermittelte IP-Adresse
  printStringWithShift(ip.c_str(), ScrollTextTime); // Anzeige IP-Adresse
  delay(10); // kurze Pause
  
  // Webserver - Ausgabe wenn Link nicht gefunden wurde
  server.onNotFound([](){
  server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });
  
  // Webserver - Startseite und Helligkeitsmodi
  server.on("/", handleRoot);
  server.on("/moff", MatrixOff);
  server.on("/mon",MatrixOn);
  server.on("/m1", BrightnessLevel1);
  server.on("/m2", BrightnessLevel2);
  server.on("/m3", BrightnessLevel3);
  server.on("/m4", BrightnessLevel4);
  server.on("/m5", BrightnessLevel5);

  // Status Doppelpunkt
  server.on("/sdflash", DoublePointFlash);
  server.on("/sdon", DoublePointOn);
  server.on("/sdoff", DoublePointOff);

  // Laufschriftgeschwindigkeit
  server.on("/vlt1", ScrollTextTimeLevel1);
  server.on("/vlt2", ScrollTextTimeLevel2);
  server.on("/vlt3", ScrollTextTimeLevel3);

  // Zusatzfunktionen
  server.on("/zfein", EnhancedFunctionsEin);
  server.on("/zfaus", EnhancedFunctionsAus);

  // Einstellungen Speichern
  server.on("/esave", SaveWebSettings);

  WiFiUDPStart();  // UPD Server starten
  WebServerStart(); // Webserver starten
  
  if (EnhancedFunctions = true && AccessOpenWeatherMap == true) { 
  Serial.println("");   
  Serial.println("Lade Wetterdaten - Bitte warten ... ");   
  printStringWithShift("     +++    Lade Wetterdaten - Bitte warten ...   +++           ", ScrollTextTime); // Wetterdaten holen  
  getWeatherData(); // Wetterdaten von OpenWeatherMap holen
  Serial.println("");
  Serial.println("_______________________________");
  Serial.println("");
  // Serial.println(weatherString); // Wetterdaten anzeigen im Debugmode
  // Wenn keine Wetterdaten geladen wurden - Zusatzfunktionen deaktivieren
  if (String(weatherLocation) == "" && StatusOpenWeatherMap == "OK") {
  EnhancedFunctions = false;
  Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"); 
  printStringWithShift("     +++    Es konnten keine Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
  } 
   }
   else
     {
  Serial.println("");    
  if (AccessOpenWeatherMap == false) { 
  Serial.println("OpenWeatherMap ist nicht aktiv - keine Zugangsdaten"); 
  if (EnhancedFunctions = false) { 
  Serial.println("OpenWeatherMap ist nicht aktiviert"); 
     }
     }
  Serial.println("");   
     }
     
    // Get all information of your LittleFS
    if (littleFSInit == true)
    {
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
    Serial.println();
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            Serial.println(f.size());
            f.close();
        }else{
            Serial.println("0");
        }   
    }
   Serial.println("_______________________________");
   Serial.println("");
   }
   else
     {
     Serial.println("Little Filesystem Init - Fehler.");      
     }
     
  Serial.println("Startzeit: "+FormatedDateTime());
  }
  else
  { 
  // Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  // Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OAK] API-Key: " + weatherKey); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OCI] City-ID: " + cityID); // Only for Debuging 
  // Serial.println("[Loaded-OpenWeatherMap] API-Key: " + (String)OWMapiKey); // Only for Debuging 
  // Serial.println("[Loaded-OpenWeatherMap] City-ID: " + (String)OWMCityID); // Only for Debuging 
     
  if ((String)ssid != ""){    
  Serial.println("WLAN-Verbindung fehlgeschlagen!");  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  delay(100);} // kurze Pause
  else {
  Serial.println("Starte Access Point ..."); 
  printStringWithShift("        +++       Access Point wird gestartet      +++           ",ScrollTextTime);  
  WiFiManagerParameter OpenWeatherMapApiKey("OWMapiKey", "OpenWeatherMap API-Key",OWMapiKey, 50);
  WiFiManagerParameter OpenWeatherCityID("OWMCityID", "OpenWeatherMap City-ID",OWMCityID, 10);
  wifiManager.addParameter(&OpenWeatherMapApiKey); // OpenWheaterMap API-Key
  wifiManager.addParameter(&OpenWeatherCityID); // OpenWheaterMap City-ID
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  wifiManager.autoConnect("LED-Matrix-Clock",""); // SSID,  SSID-Passwort , OpenWheaterMap APIKey, OpenWheaterMap CityID
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  strcpy(OWMapiKey,OpenWeatherMapApiKey.getValue());
  strcpy(OWMCityID,OpenWeatherCityID.getValue());
  if (shouldSaveConfig && littleFSInit == true){
    
  // Serial.println("[Save-WIFI] SSID: " + (String)wifiManager.getWiFiSSID());  // Only for Debuging 
  // Serial.println("[Save-WIFI] PASS: " + (String)wifiManager.getWiFiPass());   // Only for Debuging 
  // Serial.println("[Save-OpenWeatherMap] API-Key: " + (String)OWMapiKey);  // Only for Debuging 
  // Serial.println("[Save-OpenWeatherMap] City-ID: " + (String)OWMCityID);  // Only for Debuging  
  
  // SSID speichern
  SetupSave("ssid.txt", (String)wifiManager.getWiFiSSID()); 
  SetupSave("ssid-passwd.txt", (String)wifiManager.getWiFiPass()); 
  SetupSave("owm-apikey.txt", (String)OWMapiKey); 
  SetupSave("owm-cityid.txt", (String)OWMCityID);
  delay(500); // 0,5 sek. warten 
  Serial.println("Zugangsdaten gespeichert... "); 
  printStringWithShift("        +++       Zugangsdaten gespeichert...      +++           ",ScrollTextTime);
  // Reset auslösen
  Serial.println("Uhr wird neu gestartet"); 
  printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
  ESP.restart();  
  }
   else
   { // Zugangsdaten wurden nicht gespeichert
  printStringWithShift("        +++       Zugangsdaten wurden nicht gespeichert !     +++           ",ScrollTextTime);
   // Reset auslösen 
  printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
  ESP.restart();  
    }  
  }
  }
}

/*************************************(Hauptprogramm)**************************************/

void loop()
{
  // contrast(); //AutoBrightness
  
  // Display per Taster Ein- und Ausschalten
  MatrixButtonOnOff();
  
  // Check ob Internetverbindung noch besteht
  if (WiFi.status() != WL_CONNECTED) 
  {
  dots = 1; // Doppelpunkt Dauerleuchten
  showAnimClock();
  wificonnect();
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Serial.println("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (ClockWiFiConnectFail == true)
  {
  Serial.println("WLAN-Verbindung: getrennt - "+FormatedDateTime());
  if (StartStopServers == 0)
  { // Server nur einmal stoppen
  StartStopServers += 1;  
  WebServerStop(); // Webserver stoppen
  WiFiUDPStop(); // UDP Server stoppen
  }
  if (Display == 1) {
  printStringWithShift("        +++  WLAN-Verbindung: getrennt  +++           ",ScrollTextTime); 
  delay(100); // 0,1 sek. warten 
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime); 
  }
  delay(1000); // 1 sek. warten
  }
  }
  else
  {
  ClockWiFiConnectFail = false;  // WLAN-Verbindung OK
  if (StartStopServers == 1)
  { // Server nur einmal neu starten
  StartStopServers -= 1;
  Serial.println("WLAN-Verbindung: wieder hergestellt - "+FormatedDateTime());
  WiFiUDPStart(); // UDP Server starten
  WebServerStart(); // Webserver starten
  }
  }

  // nur wenn WLAN-Verbindung besteht
  if (ClockWiFiConnectFail == false)
  { 
  HandleWebServer();  // Webserver Abfrage
  getTimeLocal(); // aktuelle Uhrzeit holen
  
  // Matrix einschalten
  if (Display == 0) {
  if  (String(h)+":"+String(m)+":"+String(s) == "7:14:55" || String(h)+":"+String(m)+":"+String(s) == "7:14:56")
  {
  if (await == false) 
  { 
  await = true;   
  Display = 1;
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
  // Serial.println("Matrix um "+ String(h) + ":" + String(m) + " eingeschaltet."); 
  printStringWithShift("        +++  Guten Morgen  +++           ",ScrollTextTime);
  if (EnhancedFunctions == true && AccessOpenWeatherMap == true) {
  // Serial.println("Verbinde mit OpenweatherMap ..."); // nur wenn Display on
  getWeatherData(); // aktuelle Wetterdaten abrufen
  if (String(weatherLocation) == "" && StatusOpenWeatherMap == "OK") {
  EnhancedFunctions = false;
  Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"); 
  printStringWithShift("     +++    Es konnten keine aktuellen Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
    } 
  } 
  await = false;  
  }
  }
  }

  // Matrix abschalten
  if (Display == 1) {
  if  (String(h)+":"+String(m)+":"+String(s) == "23:14:55" || String(h)+":"+String(m)+":"+String(s) == "23:14:56")
  {
  if (await == false) 
  {  
  await = true;  
  Display = 0;  
  printStringWithShift("        +++  Gute Nacht  +++           ",ScrollTextTime); 
  sendCmdAll(CMD_SHUTDOWN,0);
  // Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet."); 
  await = false;
  }
  }
  }

  // ******************** Begin eigentliches Programm *******************************
  if (Display == 1) 
  {
  // nur Temperatur und eventuell Grüße --- alle 10 Minuten
  if (await == false && EnhancedFunctions == true && AccessOpenWeatherMap == true) 
  { 
  getTimeLocal(); // aktuelle Uhrzeit holen  
  if  ((String(m)+":"+String(s) == "5:5" || String(m)+":"+String(s) == "5:6" || 
       String(m)+":"+String(s) == "20:5" || String(m)+":"+String(s) == "20:6" || 
       String(m)+":"+String(s) == "35:5" || String(m)+":"+String(s) == "35:6" || 
       String(m)+":"+String(s) == "50:5" || String(m)+":"+String(s) == "50:6"))
  { 
    await = true;
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    MyWaitLoop(35); // kurz warten
    printStringWithShift("             ", ScrollTextTime); // Temperatur aus dem Display laufen lassen
    MyWaitLoop(3); // kurz warten
    }
    await = false;
  }
  }

  // nur Datum Anzeigen --- alle 10 Minuten
  if (await == false && EnhancedFunctions == true)
  { 
  getTimeLocal(); // aktuelle Uhrzeit holen  
  if ((String(m)+":"+String(s) == "10:5" || String(m)+":"+String(s) == "10:6" ||
       String(m)+":"+String(s) == "25:5" || String(m)+":"+String(s) == "25:6" ||
       String(m)+":"+String(s) == "40:5" || String(m)+":"+String(s) == "40:6" ||
       String(m)+":"+String(s) == "55:5" || String(m)+":"+String(s) == "55:6"))
  { 
    await = true;
    APStart = 0; // Access Point Count zurücksetzen
    if (d < 10)
    {
    FormatedDay = "0"+String(d);  
    }
    else 
    {
    FormatedDay = String(d);  
    }
    if (mo < 10)
    {
    FormatedMonth = "0"+String(mo);  
    }
    else
    {
    FormatedMonth = String(mo);    
    }
    String dayName[] = {"Err", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
    DateString = "          +++         Heute ist " + dayName[w] + " der  " + FormatedDay + "." + FormatedMonth + "." + String(ye)+ "        +++               ";
    printStringWithShift(DateString.c_str(), ScrollTextTime); 
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+DateString);
    // ++++++++++++++++++
    greatings(); // Grüße
    // ++++++++++++++++++
    waitloop = 0; // Variable zurücksetzen
    await = false;
  }
  }
  
  // Ausführliches Wetter mit Datum  --- alle 15 min
  if (await == false && EnhancedFunctions == true && AccessOpenWeatherMap == true) 
  { 
  getTimeLocal(); // aktuelle Uhrzeit holen
  if ((String(m)+":"+String(s) == "0:5" || String(m)+":"+String(s) == "0:6" || 
       String(m)+":"+String(s) == "15:5" || String(m)+":"+String(s) == "15:6" ||
       String(m)+":"+String(s) == "30:5" || String(m)+":"+String(s) == "30:6" ||
       String(m)+":"+String(s) == "45:5" || String(m)+":"+String(s) == "45:6"))
  { 
    await = true;
    // Serial.println("Verbinde mit OpenweatherMap ..."); // nur wenn Display on
    getWeatherData(); // aktuelle Wetterdaten holen
    if (String(weatherLocation) == "" && StatusOpenWeatherMap == "OK") {
    EnhancedFunctions = false;
    Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"); 
    printStringWithShift("     +++    Es konnten keine Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
    } 
     else
       {
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- " + weatherString);
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(weatherString.c_str(), ScrollTextTime);
    }
       }
    await = false;
  }
  }

if (DoublePointStatus == 2) 
{  // Doppelpunkt bei diesen Zeiten Dauerleuchten  
if  ((String(m)+":"+String(s) == "0:4" || String(m)+":"+String(s) == "0:5" ||
      String(m)+":"+String(s) == "5:4" || String(m)+":"+String(s) == "5:5" || 
      String(m)+":"+String(s) == "10:4" || String(m)+":"+String(s) == "10:5" ||
      String(m)+":"+String(s) == "15:4" || String(m)+":"+String(s) == "15:5" ||
      String(m)+":"+String(s) == "20:4" || String(m)+":"+String(s) == "20:5" || 
      String(m)+":"+String(s) == "25:4" || String(m)+":"+String(s) == "25:5" ||
      String(m)+":"+String(s) == "30:4" || String(m)+":"+String(s) == "30:5" ||
      String(m)+":"+String(s) == "35:4" || String(m)+":"+String(s) == "35:5" || 
      String(m)+":"+String(s) == "40:4" || String(m)+":"+String(s) == "40:5" ||
      String(m)+":"+String(s) == "45:4" || String(m)+":"+String(s) == "45:5" ||
      String(m)+":"+String(s) == "50:4" || String(m)+":"+String(s) == "50:5" ||
      String(m)+":"+String(s) == "55:4" || String(m)+":"+String(s) == "55:5" ))
{
if (EnhancedFunctions == true)
{   
dots = 1; // Doppelpunkt Dauerleuchten  
} 
else
{
// Doppelpunkt blinken lassen
 if (millis() - dotTime > 500) 
 { 
 dotTime = millis();
 dots = !dots;
 delay(1); // kurze Pause
 // Serial.println(String(h)+":"+String(m)+":" + String(s)); // Anzeige Zeit zum Testen
 }         
}
}
else
{ // Doppelpunkt blinken lassen
 if (millis() - dotTime > 500) 
 { 
 dotTime = millis();
 dots = !dots;
 delay(1); // kurze Pause
 // Serial.println(String(h)+":"+String(m)+":" + String(s)); // Anzeige Zeit zum Testen
 }
}
} 

if (DoublePointStatus == 0) 
{
dots = 0; // Doppelpunkt aus
}

if (DoublePointStatus == 1) 
{
dots = 1; // Doppelpunkt Dauerleuchten
}
  }  
  if (Display == 1) {
  showAnimClock();
  }
  }
}
