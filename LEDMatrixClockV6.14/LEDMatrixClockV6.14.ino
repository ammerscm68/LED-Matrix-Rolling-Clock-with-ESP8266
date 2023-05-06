/*******************************************************************************************
 ** Autor: Mario Ammerschuber                                                             **
 ** Datum: Mai 2023                                                                       **
 ** Version: 6.1.4                                                                        ** 
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "6.1.4";
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
#include <DHTesp.h>                         // DHT Temperatursensor - über Bibliothek installieren
#include <pitches.h>                        // for passiv Buzzer

#define ROTATE  90 // Displayausrichtung (0,90 or 270)

ESP8266WebServer server(80); // Webserver auf Port 80     border-radius: 50\%

// Variable für den HTTP Request
String header;

// Taster-Pin (D3 = GPIO 0)
// *************************************************
int TasterPin = D3;  // -1 = Kein Button / no Button
// *************************************************

// passiv Buzzer Init
// *****************************************************************************************************
// const int buzzerPin = D1; //for ESP8266 Microcontroller (D4 dont work with ESP8266 Microcontroller)
const int buzzerPin = -1; // no Buzzer
// *****************************************************************************************************

DHTesp dht; // DHT Sensor für Tepmeraturmessung innen (D1)

String ssid = "";     // SSID of local network 
String TempSSID = "";     // Temp SSID of local network 
String password = "";   // Password on network 
String weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
String cityID = ""; // OpenWeatherMap City-ID 
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

// Sonderzeichen und Umlaute
String deg = String(char('~' + 25)); // ° Grad Zeichen
String uue = String(char('~' + 26)); // ü Zeichen
String uoe = String(char('~' + 27)); // ö Zeichen
String uae = String(char('~' + 28)); // ä Zeichen
String uss = String(char('~' + 29)); // ß Zeichen

String WeatherLocation = "";
String StatusOpenWeatherMap = "k.A.";
String wresult = ""; // OpenWeatherMap

// GreetingsToday
bool GreetingsToday = false;

// For Clock Reboot and Reset
bool ResetNow = false; // For Clock Reset
bool RebootNow = false; // For Clock Reboot

uint16_t bootTime; // Startzeit LED-Matrix-Clock

String newsKey = ""; // NewsAPI Key;
String APINews = "";
String StatusNewsAPI = "k.A.";

String DisplayOnTime, DisplayOffTime, FDOnTime, FDOffTime;
String Brightness = "L1";
bool WeatherFail = false; 
bool NewsFail = false; 
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool ClockWiFiConnectFail = false; // false standard
String date;

String WTemp, Datum, Zeit, SText;
String weatherString, weatherString1, ATemp, DateString;
String cip, cip2;

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
String OWMLastConnectionDateTime = "k.A."; // last Connection Time OpenWeatherMap
String NewsAPILastConnectionDateTime = "k.A."; // last Connection Time NewsAPI
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
int Display = 1; // Displaymode - EINGESCHALTET
int DoublePointStatus = 1; // Status desd Doppelpunktes  - Standard = Permanent EIN
bool EnhancedFunctions = true; // Zusatzfunktionen EIN / AUS  -  Standard = EIN  (Datum, Feiertage, Geburtstage u.s.w)
bool WeatherFunctions = true; // Wetterdaten Funktion EIN / AUS  -  Standard = EIN (OpenWeatherMap API-Key und City-ID vorhanden)
bool NewsAPIFunction = true; // aktuelle Nachrichten Funktion EIN / AUS  -  Standard = EIN (NewsAPI API-Key vorhanden)
int StartStopServers = 0;
int APStart = 0;
bool ProgramSetup = true;
char dl = char('~' + 24); //  // arrow down for Download
int InTempCounter = 0; // Innentemperatur
int InHumCounter = 0; // Luftfeuchte
String InTempValue = ""; // Innentemperatur
String InHumValue = ""; // Luftfeuchte
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset = "";   // siehe Captive Portal
String InHumOffset = "";    // siehe Captive Portal
int InTempOffsetInt = -1;  // Offset Indoor Temperature  (Example : -1 = offset --> -1 Grad   or   +1 = offset --> +1 Grad)
int InHumOffsetInt = -1;  // Offset Indoor Huminity  (Example : -1 = offset --> -1%   or   +1 = offset --> +1%)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String LastInTempMessureDateTime = "k.A.";

// Anzeigezeiten
String OnlyTempViewTime[] = {"5", "20", "35", "50"}; // nur Außentemperatur anzeigen (Minute)
String DateViewTime[] = {"10", "25", "40", "55"}; // aktuelles Datum und eventuell News (Minute)
String FullWeatherViewTime[] = {"0", "15", "30", "45"}; // Vollständiges Wetter (Minute)
String DoublePointPermanantTime[] = {"0", "5", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55"}; // Vor Aktion Doppelpunkt Permanent anzeigen

/*****************************************( Setup )****************************************/
void setup()
{
  // system_update_cpu_freq(SYS_CPU_160MHZ); // CPU Takt auf 160MHz Einstellen
  Serial.begin(115200); // Serielle Verbindung mit 115200 Baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen
  
  // pinMode(A0, INPUT); // für autobrightness

  // Init passiv Buzzer
  if (buzzerPin != -1) {
  pinMode(buzzerPin, OUTPUT);
  noTone(buzzerPin);} // kein Ton
  
  // Taster initialisieren wenn vorhanden
  if (TasterPin != -1) {
  pinMode(TasterPin, INPUT_PULLUP);} 

  // DHT 11
  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 5 (D1) - Temperatursensor innen

  // LEDControl.h
  for(int index=0;index<lc.getDeviceCount();index++) {
  lc.shutdown(index,false);} 

   Serial.println("*** LED-Matrix-Clock ***");
   Serial.println("");
   Serial.println("Anzahl Max7219 Module: "+String(lc.getDeviceCount())); // LEDControl.h
   Serial.println("");

   // ***First Start***
     // Little-Filesystem formatieren
     // LittleFS.format(); // alle Dateien löschen -->   danch wieder deaktivieren und nochmal hochladen

   LoadMatrixAdjustments(); // LED-Matrix-Clock Einstellungen laden ....

  // Wetterdaten deaktivieren wenn kein API-Key oder City-ID von OpenWeatherMap vorhanden
  if (weatherKey != "" && cityID != "") {
     AccessOpenWeatherMap = true;}
     else {
     WeatherFunctions = false; 
     AccessOpenWeatherMap = false;
     }

  // NewsAPI deaktivieren wenn kein API-Key vorhanden
  if (newsKey != "") {
     AccessNewsAPI = true;}
     else {
     NewsAPIFunction = false; 
     AccessNewsAPI = false;
     }   
     
  // Martrix initialisieren
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
  MyWaitLoop(1000);
  // *******************************************************
  ScrollClockTime  = 80; // Scrollverzögerung der Uhrzeit    80 = Default
  // *******************************************************

  /*for (int i=1; i <= 29; i++){  
  String UEString = String(char('~' + i)); //
  String TestUmlaute = String(i)+"=   "+UEString+"    ";
  printStringWithShift(TestUmlaute.c_str(), 90); // Test Umlaute und Sonderzeichen
  }*/

  if (ssid == "") {
    Serial.println("");
    Serial.println("***** First Start Clock *****"); // erster Start der Uhr
    Serial.println("");
    }

  WiFi.hostname("LEDMatrixClock"); // Hostname der Uhr  

  if (ssid != ""){
  Serial.print("Verbinde mit WiFi SSID: ");
  Serial.println(ssid);
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {ClockWiFiConnectFail = true;} // beim ersten Start der Uhr die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (ClockWiFiConnectFail == false)
  { 
  // vergebene IP-Adresse auf Matrix anzeigen 
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "        IP-Adresse:  "+cip+"                  "; // Ermittelte IP-Adresse

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Anzeige IP-Adresse
  MyWaitLoop(10); // kurze Pause

  // Webserver - bei falschem Link zurück zur Startseite
  server.onNotFound(handleRoot);

  // Webserver - Startseite und Helligkeitsmodi
  server.on("/", handleRoot);
  server.on("/moff", MatrixOff);
  server.on("/mon",MatrixOn);
  server.on("/mb1", BrightnessLevel1);
  server.on("/mb2", BrightnessLevel2);
  server.on("/mb3", BrightnessLevel3);
  server.on("/mb4", BrightnessLevel4);
  server.on("/mb5", BrightnessLevel5);
  server.on("/cdof", ChangeDisplayOnOff);
  server.on("/cdofc", ChangeDisplayOnOffCancel);

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
  server.on("/owmfein", WeatherFunctionsEin);
  server.on("/owmfaus", WeatherFunctionsAus);
  server.on("/cwd", CurrentWeatherData);

  // aktuelle Nachrichten aktivieren / deaktivieren
  server.on("/nafein", NewsAPIFunctionEin);
  server.on("/nafaus", NewsAPIFunctionAus);
  server.on("/cn", CurrentNews);

  // Einstellungen löschen - Reset
  server.on("/reset", ResetClockSettings); 
  server.on("/goreset", ResetClockYes); 

  // Neustart der Uhr
  server.on("/reboot", RebootClock); 

  // Innentemperaturmessung
  server.on("/tempvalue", DHTHTML); 

  // API-Keys anzeigen
  server.on("/cpadjust", GetAdjustments); 

  Serial.println("UDP-Server starten ...");
  WiFiUDPStart();  // UPD Server starten

  // #######################################################################################
  // Start TCP (HTTP) server
  WebServerStart();
  // Arduino OTA/DNS
  Serial.println("Arduino OTA/DNS-Server starten ... - URL: http://ledmatrixclock.local");
  ArduinoOTA.setHostname("ledmatrixclock");
  ArduinoOTA.setPassword("74656"); // default Passwort for OTA
  ArduinoOTA.onEnd([]() {Serial.println("\nEnd");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));});
  ArduinoOTA.onError([](ota_error_t error) {(void)error;ESP.restart();}); // Restart bei OTA Fehler
  // Arduino jetzt OTA/DNS starten
  ArduinoOTA.begin();
  // #####################################################################################

  if (!WeatherFunctions || !AccessOpenWeatherMap) {
  Serial.println("");
  Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}

  if (!NewsAPIFunction || !AccessNewsAPI) {
  Serial.println("");
  Serial.println("NewsAPI wurde deaktiviert - keine aktuellen Nachrichten");
  Serial.println(""); 
   } else {Serial.println("");Serial.println("");}
   
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
  Serial.println("");
  Serial.println("");

  greatings(); // Als erstes Greetings 

  // Display ausschalten wenn "automatisch AUS" Zeit überschritten
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime()) {
  Display = 0; 
  if (h >= 0 && h < 12)   {SText = "        +++  Guten Morgen ...   Sch"+uoe+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18)  {SText = "        +++  Sch"+uoe+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21)  {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0);
  Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet.");}}
  }
  else
  { 
  if (ssid != ""){ 
  Serial.println("WLAN-Verbindung fehlgeschlagen!");  
  printStringWithShift("        +++  WLAN-Verbindung fehlgeschlagen!  +++           ",ScrollTextTime);
  MyWaitLoop(100);} // kurze Pause
  else {
  Serial.println("Starte Access Point ... [LED-Matrix-Clock]"); 
  Serial.println("");
  printStringWithShift("        +++       Access Point wird gestartet      +++       IP-Adresse: 192.168.4.1      +++           ",ScrollTextTime);  
  printCharWithShift('.',ScrollTextTime);
    printCharWithShift('.',ScrollTextTime);
      printCharWithShift('.',ScrollTextTime);
      // ########################################################
       CaptivePortal(); // Start Captive Portal (WiFi-Manager)
      // ######################################################## 
       }
  } 
  // BuzzerBeep(); // Setup abgeschlossen
  bootTime = millis(); //saveing millis after setup for Runtime Modul
  ProgramSetup = false; // Setup beendet 
}

/*************************************(Hauptprogramm)**************************************/

void loop()
{
if (ssid == "") {if (swait == false) {server.handleClient();}} else {
// contrast(); //AutoBrightness

 if (await == false && TasterPin != -1) { 
  MatrixButtonOnOff();} // Display per Taster Ein- und Ausschalten
 
  // Check ob Internetverbindung noch besteht
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) 
  {
  dots = 1; // Doppelpunkt Dauerleuchten
  wificonnect(false); // WLAN-Verbindung herstellen
  if (ClockWiFiConnectFail == true)
  {
  if (StartStopServers == 0)
  { // Server nur einmal stoppen
  StartStopServers += 1;  
  WebServerStop(); // Webserver stoppen
  WiFiUDPStop(); // UDP Server stoppen
  }
  if (Display == 1){
  printStringWithShift("        +++  WLAN-Verbindung: getrennt  +++           ",ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. warten 
  Serial.println("Bitte warten ... "+FormatedDateTime());
  printStringWithShift("        +++       Bitte warten ...      +++           ",ScrollTextTime);
  }
  MyWaitLoop(1000);} // 1 sek. warten
  }
  else
  {
  ClockWiFiConnectFail = false;  // WLAN-Verbindung OK
  if (StartStopServers == 1)
  { // Server nur einmal neu starten
  StartStopServers -= 1;
  Serial.println("WLAN-Verbindung: wieder hergestellt - "+FormatedDateTime());
  WiFiUDPStart(); // UDP Server starten
  WebServerStart();}} // Webserver starten

  // nur wenn WLAN-Verbindung besteht
  if (ClockWiFiConnectFail == false)
  { 
  if (StartFail) {Serial.println("Uhr wird neu gestartet..."); ESP.restart();} // Uhr neu starten bei WLAN Problemen      
  HandleOTAandWebServer();  // OTA und Webserver Abfrage
  getTimeLocal(); // aktuelle Uhrzeit holen
  // ***** Matrix einschalten ******
  if (Display == 0 && DisplayOnTime != "") {
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":0")
  {
  if (await == false) 
  { 
  await = true;   
  ESP.restart();}}} // Uhr neu starten

  // ****** Matrix abschalten *******
  if (Display == 1 && DisplayOffTime != "") {
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":0")
  {
  if (await == false) 
  {  
  await = true;  
  Display = 0;  
  if (h >= 0 && h < 12)  {SText = "        +++  Guten Morgen ...   Sch"+uoe+"nen Tag noch  +++           ";} 
  if (h >= 12 && h < 18) {SText = "        +++  Sch"+uoe+"nen Tag noch  +++           ";} 
  if (h >= 18 && h < 21) {SText = "        +++  Guten Abend ...   bis bald +++           ";} 
  if (h >= 21 && h <= 23) {SText = "        +++  Gute Nacht  +++           ";}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0);
  Serial.println("Matrix um "+ String(h) + ":" + String(m) + " ausgeschaltet."); 
  await = false;}}}

  // ******************** Begin eigentliches Programm *******************************
  if (Display == 1) 
  {
  // nur Innen- und Außentemperatur --- alle 10 Minuten
  if (await == false) { 
  for (int i=0; i <= 4; i++){  
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":5")
  { 
    await = true;
    if (WeatherFunctions == true && AccessOpenWeatherMap == true){getWeatherData();} // aktuelle Wetterdaten holen wenn aktiviert
     
    // *** Innentemperatur ***
    if (InTempCounter < 5) {
    InTempValue = InTempMessure(); 
    if (InTempValue != "-") {
    if (InTempCounter > 0) {InTempCounter = 0;} // Variable Reset
    String InTempString = "              Innentemperatur:        "+ InTempValue + deg + "C  "; 
    // Serial.println(InTempString); // Only for Debuging 
    printStringWithShift(InTempString.c_str(), ScrollTextTime); // Innentemperatur anzeigen
    MyWaitLoop(4000); // kurz warten
    printStringWithShift("             ", ScrollTextTime); // Innen-Temperatur aus dem Display laufen lassen
    } else {InTempCounter += 1;} // inc
    } else {Serial.println("DHT-Innentemperatur Sensor deaktiviert");
            InTempValue = "-";}
    
    // *** Außentemperatur ***
    if (WeatherFunctions == true && AccessOpenWeatherMap == true){
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+ATemp);
    if (String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
    // Serial.println(ATemp); // Only for Debuging 
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Außentemperatur anzeigen
    MyWaitLoop(4000); // kurz warten
    printStringWithShift("             ", ScrollTextTime); // Außen-Temperatur aus dem Display laufen lassen
    }
    if (InTempValue != "-" || StatusOpenWeatherMap == "OK") {ClockScrollIn();}} // Uhrzeit in Display hineinscrollen
    await = false;}}}
    
  // nur Datum und Zusatzfunktionen Anzeigen --- alle 10 Minuten
  if (await == false && EnhancedFunctions == true){ 
  for (int i=0; i <= 4; i++){  
  if  (String(m)+":"+String(s) == DateViewTime[i]+":5"){ 
    await = true;
    if (NewsAPIFunction == true && AccessNewsAPI == true) {getNews();}  // aktuelle News
    if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
    if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
    String dayName[] = {"Err", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
    DateString = "          +++         Heute ist " + dayName[w] + " der  " + FormatedDay + "." + FormatedMonth + "." + String(ye)+ "        +++               ";
    printStringWithShift(DateString.c_str(), ScrollTextTime); 
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- "+DateString);
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    greatings(); // Grüße und Feiertage
    if (!GreetingsToday && AccessNewsAPI == true && NewsAPIFunction == true && APINews != "Fehler" && APINews != "") {
    // Anzeige News
    printStringWithShift(APINews.c_str(), ScrollTextTime);} 
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
    waitloop = 0; // Variable zurücksetzen
    await = false;}}}
    
  // Ausführliches Wetter mit Datum  --- alle 15 min
  if (await == false && WeatherFunctions == true && AccessOpenWeatherMap == true) 
  { 
  for (int i=0; i <= 4; i++){  
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":5")
  { 
    await = true;
    if (String(m) == "0" && buzzerPin != -1) {ShortBuzzerBeep();} // Volle Stunde Beep
    // Serial.println("Verbinde mit OpenweatherMap ..."); // nur wenn Display on
    getWeatherData(); // aktuelle Wetterdaten holen
    if (String(WeatherLocation) == "" && StatusOpenWeatherMap == "OK") {
    WeatherFunctions = false;
    Serial.println("OpenWeatherMap wurde deaktiviert - keine Wetterdaten"); 
    printStringWithShift("     +++    Es konnten keine Wetterdaten geladen werden!   +++           ", ScrollTextTime); // Wetterdaten holen  
    ClockScrollIn(); // Uhrzeit in Display hineinscrollen  
    } 
     else {
    // Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- " + weatherString);
    if (StatusOpenWeatherMap == "OK"){
    printStringWithShift(weatherString.c_str(), ScrollTextTime);
    ClockScrollIn();}} // Uhrzeit in Display hineinscrollen   
    await = false;}}}

if (DoublePointStatus == 2) 
{  // Doppelpunkt bei diesen Zeiten Dauerleuchten 
for (int i=0; i <= 12; i++){  
if  (String(m)+":"+String(s) == DoublePointPermanantTime[i]+":4")
{
if (WeatherFunctions == true && AccessOpenWeatherMap == true)
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
 }}
}
else
{ // Doppelpunkt blinken lassen
 if (millis() - dotTime > 500) 
 { 
 dotTime = millis();
 dots = !dots;
 delay(1); // kurze Pause
 // Serial.println(String(h)+":"+String(m)+":" + String(s)); // Anzeige Zeit zum Testen
 }}}} 

if (DoublePointStatus == 0) 
{
dots = 0; // Doppelpunkt aus
}

if (DoublePointStatus == 1) 
{
dots = 1; // Doppelpunkt Dauerleuchten
}
  }  
  if (Display == 1 && ClockWiFiConnectFail == false) {
  showAnimClock(); // Uhrzeit anzeigen
  }
  ResetAPCounter(); // Access Point Button Reset alle 2 Minuten
  }
  }
} // Loop - Ende
