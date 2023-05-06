// ****************************Hilfsfunktionen******************************************************

// passiv buzzer melody: (Big Ben)
int melody[] = {
NOTE_E3, NOTE_GS3, NOTE_FS3, NOTE_B2, 0,
NOTE_E3, NOTE_FS3, NOTE_GS3, NOTE_E3, 0,
NOTE_GS3, NOTE_E3, NOTE_FS3, NOTE_B2, 0,
NOTE_B2, NOTE_FS3, NOTE_GS3, NOTE_E3
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
4,4,4,2.5,4,
4,4,4,2.5,4,
4,4,4,2.5,4,
4,4,4,2.5};

// *************************************************************************************************

void greatings()                   // uae = ä Zeichen
{                                  // uoe = ö Zeichen 
GreetingsToday = false;            // uue = ü Zeichen
bool dFound = false;               // uss = ß Zeichen 
String GT, HT;                     // deg = ° Grad Zeichen  - example : HollyDay 1.Mai
// ***************************** "Feiertage" ********************************************************* 
int GreetingsCount = 1;                                 
String GreetingsDate[GreetingsCount] {"0.0"};
String GreetingsText[GreetingsCount] {"---No Greetings---"}; // No Greetings
// ---------------------------------------------------------------------------------------------------
// String GreetingsDate[GreetingsCount] {"17.3", "1.4", "18.4", "10.7"};
// String GreetingsText[GreetingsCount] {"Alles gute zum Geburtstag",
//                                       "Alles gute zum Geburtstag lieber Rolf",
//                                       "Alles gute zum Geburtstag liebe Rosi",
//                                       "Alles gute zum Geburtstag lieber Klaus"}; 
// ---------------------------------------------------------------------------------------------------                                        
// String GreetingsDate[GreetingsCount] {"18.4", "10.7"};
// String GreetingsText[GreetingsCount] {"Alles gute zum Geburtstag liebe Rosi",
//                                       "Alles gute zum Geburtstag lieber Klaus"};  
// ---------------------------------------------------------------------------------------------------
// String GreetingsDate[GreetingsCount] {"17.3"};
// String GreetingsText[GreetingsCount] {"Alles gute zum Geburtstag"};
// ---------------------------------------------------------------------------------------------------  
// String GreetingsDate[GreetingsCount] {"1.4"};
// String GreetingsText[GreetingsCount] {"Alles gute zum Geburtstag lieber Rolf"}; 
// ---------------------------------------------------------------------------------------------------                                                                                                                                                                                                                                                                              
for (int i = 0; i < GreetingsCount; i++){
if  ((String(d) + "." + String(mo)) == GreetingsDate[i]) {
GT= "       +++     "+GreetingsText[i]+"     +++                "; 
dFound = true;} // Grüße
}
if (dFound == true){
GreetingsToday = true;
dFound = false;
Serial.println("Greetings: "+GT);  
printStringWithShift(GT.c_str(), ScrollTextTime);
}

// ***************************** "Feiertage" *********************************************************
int HollyDayCount = 6; 
String HollyDayDate[HollyDayCount] {"1.1", "1.5", "24.12", "25.12", "26.12", "31.12"};
String HollyDayText[HollyDayCount] {"Alles gute im neuen Jahr",
                                    "Sch"+uoe+"nen 1. Mai",
                                    "Heiligabend",
                                    "Frohe Weihnachten (1)",
                                    "Frohe Weihnachten (2)",
                                    "Guten Rutsch ins neue Jahr"};
for (int i = 0; i < HollyDayCount; i++){
if  ((String(d) + "." + String(mo)) == HollyDayDate[i]) {
HT= "       +++     "+HollyDayText[i]+"     +++                "; 
dFound = true;} // Feiertage
}
if (dFound == true){
GreetingsToday = true;
dFound = false; 
Serial.println("Holliday: "+HT); 
printStringWithShift(HT.c_str(), ScrollTextTime);
}
}

// *************************************************************************************************

void clearscreen() { 
for(int i=0; i<10; i++) {
Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
}

// *************************************************************************************************

void ClockScrollIn() {
getTimeLocal(); // aktuelle Uhrzeit holen
String TimeString;
// Uhrzeit
if (m < 10) {TimeString = String(h)+"0"+String(m);} else {TimeString = String(h)+String(m);}
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);
}

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Einstellungen laden .... ***
bool MDTime; // Hilfsvariable
// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit == true) {
  Serial.println("Load Clock Adjustments ...");
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt");
  ssid.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {ssid = "";}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt");
  password.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {password = "";}
  // *** Zugangsdaten OpenWeatherMap laden ***
  // OpenWeatherMap API-Key als String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt");
  weatherKey.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {weatherKey = "";}
  // OpenWeatherMap City-ID als String
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt");
  cityID.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {cityID = "";} 
  if (LittleFS.exists("/newsapi-apikey.txt")){
   newsKey = SetupLoad("newsapi-apikey.txt");
   newsKey.trim();} else {newsKey = "";} // https://newsapi.org
   
  // Display Einschaltzeit als String
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOnTime = "";}
  // Display Ausschaltzeit als String
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOffTime = "";} 

  // DHT-Sensor Offset
  if (LittleFS.exists("/dht-intempoffset.txt")){
  InTempOffset = SetupLoad("dht-intempoffset.txt");
  InTempOffset.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {InTempOffset = "0";}  // Temperature
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {InHumOffset = "0";}  // Humidity

  /*Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Loaded-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Loaded-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Loaded-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Loaded-DHT] InTempOffset: " + InTempOffset);  // Only for Debuging
  Serial.println("[Loaded-DHT] InHumOffset: " + InHumOffset);  // Only for Debuging*/

  // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
  CheckLEDMatrixDisplayOnOff();

/*Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println(""); // for Debug Mode*/

// Check DHT Offset
if (!IsNumeric(InTempOffset)) {InTempOffsetInt = 0;} else {InTempOffsetInt = InTempOffset.toInt();} // Temperature
if (!IsNumeric(InHumOffset)) {InHumOffsetInt = 0;} else {InHumOffsetInt = InHumOffset.toInt();} // Humidity
if (InTempOffset != "") {InTempOffset = (String)InTempOffsetInt;} else {InTempOffsetInt = 0;} 
if (InHumOffset != "") {InHumOffset = (String)InHumOffsetInt;} else {InHumOffsetInt = 0;} 

  // Helligkeit der LED-Matrix
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // bei leerer Datei oder Fehler Stufe 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Stufe 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Stufe 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Stufe 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);} // Stufe 5
  // Doppelpunktstatus
  String DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // bei leerer Datei oder Fehler
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}
  // ScrollText Geschwindigkeit
  String STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "25") {ScrollTextTime = 25;} // Stufe 8
  if (STT == "30") {ScrollTextTime = 30;} // Stufe 7
  if (STT == "35") {ScrollTextTime = 35;} // Stufe 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Stufe 5
  if (STT == "45") {ScrollTextTime = 45;} // Stufe 4
  if (STT == "50") {ScrollTextTime = 50;} // Stufe 3
  if (STT == "55") {ScrollTextTime = 55;} // Stufe 2
  if (STT == "60") {ScrollTextTime = 60;} // Stufe 1
  
  // Zusatzfunktionen EIN/AUS
  String EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}

  // Wetterdatenfunktion EIN/AUS
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true;}
  if (OWMF == "Off") {WeatherFunctions = false;}} else {WeatherFunctions = true;}

  // NewsAPI Funktion EIN/AUS 
  if (LittleFS.exists("/status_newsapifunctions.txt")){
  String NAF = SetupLoad("status_newsapifunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsAPIFunction = true;}
  if (NAF == "Off") {NewsAPIFunction = false;}} else {NewsAPIFunction = true;}

 /*weatherKey = ""; // Only for Debuging 
   cityID = ""; // Only for Debuging 
   newsKey = ""; // Only for Debuging*/ 

  /*Serial.println("[Loaded-WIFI] SSID: " + ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + password); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Loaded-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Loaded-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Loaded-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Loaded-DHT] InTempOffset: " + (String)InTempOffsetInt);  // Only for Debuging
  Serial.println("[Loaded-DHT] InHumOffset: " + (String)InHumOffsetInt);  // Only for Debuging*/ 
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    ssid = "";
    password = ""; 
    Brightness = "L1";
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    DisplayOnTime = "6:30";
    DisplayOffTime = "23:15";
    EnhancedFunctions = true;
    WeatherFunctions = false;
    weatherKey = "1234567890"; // API-Key von OpenWeatherMap - https://openweathermap.org
    cityID = "09876"; // City ID von OpenWeatherMap
    newsKey = "0987654321"; // API-Key von NewsAPI - https://newsapi.org
    InTempOffsetInt = -1; // Temperature Offset
    InHumOffsetInt = -1; // Humidity  Offset
    }  
}

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Originale gespeicherte Uhrzeit
  FDOffTime = DisplayOffTime; // Originale gespeicherte Uhrzeit

   // Display On/Off Time auf Plausibilität prüfen
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
  if (DisplayOnTime != "") {
  if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = "";};
  if (DisplayOnTime != "") {
  if (DisplayOnTime.substring(0,1) == "0") {DisplayOnTime.remove(0, 1);}
  int DP = DisplayOnTime.indexOf(':')+1;
  if (DisplayOnTime.substring(DP,DP+1) == "0") {DisplayOnTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
if (DisplayOnTime != "") {
MDTime = false;
int DP = DisplayOnTime.indexOf(':'); 
String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = "";} else {   
MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = "";}}}}}

  if (DisplayOffTime != "") {
  if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = "";};
  if (DisplayOffTime != "") {
  if (DisplayOffTime.substring(0,1) == "0") {DisplayOffTime.remove(0, 1);}
  int DP = DisplayOffTime.indexOf(':')+1;
  if (DisplayOffTime.substring(DP,DP+1) == "0") {DisplayOffTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
if (DisplayOffTime != "") {
MDTime = false;
int DP = DisplayOffTime.indexOf(':'); 
String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
String CheckMDMinOff=  DisplayOffTime.substring(DP+1,6);
CheckMDMinOff.trim(); 

// Eingaben für Matrix On/Off aufbereiten
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = "---";} else {MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = "---";}}}}}  
  } // SSID - Ende  
}

// *************************************************************************************************

void SaveMatrixAdjustments(){
  if (littleFSInit == true) {
  Serial.println("Save Clock Adjustments ...");    
  // Trim
  TempSSID.trim(); // SSID WLAN
  password.trim(); // Password WLAN
  weatherKey.trim(); // OpenWeatherMap API-Key
  cityID.trim();  // OpenWeatherMap City-ID
  newsKey.trim(); // NewsAPI API-Key
  DisplayOnTime.trim(); // Matrix Display Einschaltzeit
  DisplayOffTime.trim(); // Matrix Display Ausschaltzeit
  InTempOffset.trim(); // DHT-Sensor Offset (Temperatur)
  InHumOffset.trim(); // DHT-Sensor Offset (Luftfeuchte)
  
  // SSID speichern
  SetupSave("ssid.txt", TempSSID); // SSID WLAN
  SetupSave("ssid-passwd.txt", password); // Password WLAN
  // OpenWeatherMap API-Key und OpenWeatherMap City-ID
  if (weatherKey != "" && cityID != "") {
  SetupSave("owm-apikey.txt", weatherKey);
  SetupSave("owm-cityid.txt", cityID);} else {
  if (LittleFS.exists("/owm-apikey.txt")){LittleFS.remove("/owm-apikey.txt");}
  if (LittleFS.exists("/owm-cityid.txt")){LittleFS.remove("/owm-cityid.txt");}} 
  // NewsAPI API-Key
  if (newsKey != "") {
  SetupSave("newsapi-apikey.txt", newsKey);} else {
  if (LittleFS.exists("/newsapi-apikey.txt")){LittleFS.remove("/newsapi-apikey.txt");}}
  // Matrix Display Einschaltzeit
  if (DisplayOnTime != ""){
  SetupSave("display-on-time.txt", DisplayOnTime);} else {
  if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
  // Matrix Display Ausschaltzeit
  if (DisplayOffTime != ""){
  SetupSave("display-off-time.txt", DisplayOffTime);} else {
  if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}

  // Check DHT Offset
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = "0";} // Temperature
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = "0";} // Humidity
  
  // DHT-Sensor Offset
  if (InTempOffset != ""){
  SetupSave("dht-intempoffset.txt", InTempOffset);} else {
  if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
  if (InHumOffset != ""){
  SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
  if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}

  /*Serial.println("[Save-WIFI] SSID: " + TempSSID); // Only for Debuging 
  Serial.println("[Save-WIFI] PASS: " + password); // Only for Debuging 
  Serial.println("[Save-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Save-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Save-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Save-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Save-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Save-DHT] InTempOffset: " + InTempOffset);  // Only for Debuging
  Serial.println("[Save-DHT] InHumOffset: " + InHumOffset);  // Only for Debuging*/ 
  }
}

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
String result = ""; // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return result;
}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // failed to open the file, return false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return true;
}

// *************************************************************************************************

// Display per Taster Ein- und Ausschalten
void MatrixButtonOnOff(){
if(digitalRead(TasterPin) == LOW){
MyWaitLoop(500); //wait
Display=!Display;
if (Display == 0 && ClockWiFiConnectFail == false){
sendCmdAll(CMD_SHUTDOWN,0); // Display ausschalten
Serial.println("Matrix per Taster um "+ FormatedDateTime() + " ausgeschaltet."); 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // aktuelle Uhrzeit holen 
    sendCmdAll(CMD_SHUTDOWN, 1); // Display einschalten
    sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
    // ShortBuzzerBeep();
    Serial.println("Matrix per Taster um "+ FormatedDateTime() + " eingeschaltet.");} else
    {
    sendCmdAll(CMD_SHUTDOWN, 1); // nur Display einschalten
    printCharWithShift('.',5);
    }
    // Nach 10x Ausschalten den "Access Point" aktivieren
    APStart += 1; // inc  
    // Serial.println("Taster Count: "+String(APStart));
    if (APStart == 10 && littleFSInit == true) {
    ResetClock();} // Uhr Zurücksetzen 
    }
    delay(250); //wait
   }
}

// *************************************************************************************************

String FormatedDateTime()
{
String DateTimeString;
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (s < 10){DateTimeString += ":0"+String(s)+" Uhr / ";} else{DateTimeString += ":"+String(s)+" Uhr / ";}
// Datum
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += ".0"+String(mo)+"."+String(ye);} else {DateTimeString += "."+String(mo)+"."+String(ye);}
return DateTimeString;    
}

// *************************************************************************************************

// Arduino OTA-Abfrage nur alle 0,5 sek.  und Webserverabfrage nur alle 1,5 sek.
void HandleOTAandWebServer(){ 
if (millis() - hwsTime > 1500) { 
 hwsTime = millis();
if (swait == false) {  
server.handleClient();}} else { // Webserver Abfrage
if (millis() - hwsTime1 > 500) { 
 hwsTime1 = millis();
 ArduinoOTA.handle();} // OTA Abfrage
}
}

// *************************************************************************************************

void wificonnect(bool Start)
{
WiFiConnectLoop = 0; // Variable zurücksetzen  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
Serial.print("."); 
if (ProgramSetup == false && Display == 1) {printCharWithShift2('.',1);}
MatrixButtonOnOff(); // Taster abfragen
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED)
{ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
APStart = 0; // "Access Point" Button Counter zurücksetzen
if (!Start) {Serial.println("WLAN-Verbindung getrennt ...");} else {StartFail = true;} 
} else
{
ClockWiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("");
Serial.print("Verbunden mit IP-Adresse: "); Serial.println(WiFi.localIP());
}
}

// *************************************************************************************************

void WiFiUDPStart()
{ 
Udp.begin(localPort);
Serial.print("lokaler Port: ");
Serial.println(Udp.localPort());
Serial.println("NTP-Time - Warten auf die Synchronisation");
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
Serial.println("UDP Server gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WiFiUDPStop()
{
Udp.stop();
Serial.println("UDP Server gestoppt - "+FormatedDateTime());   
}

// *************************************************************************************************

void WebServerStart()
{
server.begin();
Serial.println("Webserver gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WebServerStop()
{
server.stop();
Serial.println("Webserver gestoppt - "+FormatedDateTime()); 
}

// *************************************************************************************************

void MyWaitLoop(int wlt)
{// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) // Pause in Millisekunden
{ 
waitloop += 1; // inc waitloop 1
HandleOTAandWebServer();  // OTA und Webserver Abfrage
delay(1);
yield(); // Equivalent zu  Application.ProcessMessages; 
// Serial.println(String(waitloop));
}
}

// *************************************************************************************************

// check a string to see if it is numeric and accept Decimal point and positiv negativ Value
boolean IsNumeric(String str)
{
if(str.length()<1){return false;}
bool bPoint=false;
if ((str.charAt(0) == '-' || str.charAt(0) == '+') && (str.length()>1)) {  
for(unsigned char i = 1; i < str.length(); i++)
{
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};
}
} else
{
for(unsigned char i = 0; i < str.length(); i++)
{
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};
}  
  }
return true;
}

// *************************************************************************************************

void ResetAPCounter()
{
String ResetValue[] = {"0","2", "4", "6", "8", "10", "12", "14" ,"16", "18", 
                       "20", "22", "24", "26", "28", "30", "32", "34", "36",
                       "38", "40", "42", "44", "46", "48", "50", "52", "54",
                       "56", "58"}; 
for (int i=0; i <= 30; i++){                      
if  (String(m)+":"+String(s) == ResetValue[i]+":2")
{ 
// Serial.println("Access Point Button Reset");
APStart = 0; // "Access Point" Button Counter alle 2 Minuten zurücksetzen
} 
}    
}

// *************************************************************************************************

void Download()
{
if (ProgramSetup == true) {  
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
} 
}

// *************************************************************************************************

void DownloadComplet()
{
if (ProgramSetup == true) { 
// Anzeige löschen    
clr(); 
refreshAll();
}
}

// ********************** Routine by freddy64 ******************************************************

String runtime() {
  static uint8_t rolloverCounter = 0;
  static uint32_t lastMillis = 0;
  uint32_t currentMillis = millis() - bootTime;
  if (currentMillis < lastMillis) {       // check millis overflow
    rolloverCounter++;
  }
  lastMillis = currentMillis;
  uint32_t secs = (0xFFFFFFFF / 1000 ) * rolloverCounter + (currentMillis / 1000);
  char buf[20];
  sprintf(buf, "%ld %s %02ld:%02ld:%02ld", secs / 86400, (secs < 86400 || secs > 172800) ? "Tage" : "Tag", secs / 3600 % 24, secs / 60 % 60, secs % 60);
  return buf;
}

// ************************************************************************************************

void ResetClock()
{
APStart = 0; // zurücksetzen
swait = true; // kein Website Handling mehr
bool IsFileExist = false; // zurücksetzen
// Little-Filesystem formatieren
LittleFS.format();
// Wenn OpenWeatherMap aktiv - die Zugangsdaten wieder speichern
if (weatherKey != "" && cityID != "") {
SetupSave("owm-apikey.txt", weatherKey); 
SetupSave("owm-cityid.txt", cityID);}
if (newsKey != "") {
SetupSave("newsapi-apikey.txt", newsKey);}
if (DisplayOnTime != "---" && DisplayOnTime != "") {
// Matrix Display Einschaltzeit 
SetupSave("display-on-time.txt", FDOnTime);} 
if (DisplayOffTime != "---" && DisplayOffTime != "") {
// Matrix Display Ausschaltzeit
SetupSave("display-off-time.txt", FDOffTime);} 
if (InTempOffset == "") {InTempOffset ="0";}  
// DHT-Sensor Temperature Offset
SetupSave("dht-intempoffset.txt", InTempOffset); 
if (InHumOffset == "") {InHumOffset ="0";}  
// DHT-Sensor Humidity Offset
SetupSave("dht-inhumoffset.txt", InHumOffset);
MyWaitLoop(500); // 0,5 sek. warten 
Serial.println("Reset - Uhr wird neu gestartet");
printStringWithShift("        +++     Reset - Uhr wird neu gestartet      +++           ",ScrollTextTime);
ESP.restart();   
}

// *************************************************************************************************

void beep(int note, int duration){
tone(buzzerPin, note, duration);
MyWaitLoop(duration);}

// *************************************************************************************************

void BuzzerBeep()
{
beep(NOTE_A4, 500);
beep(NOTE_A4, 500);    
beep(NOTE_A4, 500);
beep(NOTE_F4, 350);
beep(NOTE_C5, 150);  
beep(NOTE_A4, 500);
beep(NOTE_F4, 350);
beep(NOTE_C5, 150);
beep(NOTE_A4, 650);
} 

// *************************************************************************************************

void ShortBuzzerBeep()
{
// iterate over the notes of the melody:
for (int thisNote = 0; thisNote < 4; thisNote++) {
// to calculate the note duration, take one second divided by the note type.
//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
int noteDuration = 1000 / noteDurations[thisNote];
tone(buzzerPin, melody[thisNote], noteDuration);
// to distinguish the notes, set a minimum time between them.
// the note's duration + 30% seems to work well:
int pauseBetweenNotes = noteDuration * 1.30;
MyWaitLoop(pauseBetweenNotes);
// stop the tone playing:
noTone(buzzerPin);
}  
}

// *************************************************************************************************

void BigBenBuzzerBeep()
{
// iterate over the notes of the melody:
for (int thisNote = 0; thisNote < 19; thisNote++) {
// to calculate the note duration, take one second divided by the note type.
//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
int noteDuration = 1000 / noteDurations[thisNote];
tone(buzzerPin, melody[thisNote], noteDuration);
// to distinguish the notes, set a minimum time between them.
// the note's duration + 30% seems to work well:
int pauseBetweenNotes = noteDuration * 1.30;
MyWaitLoop(pauseBetweenNotes);
// stop the tone playing:
noTone(buzzerPin);
}  
}

// *************************************************************************************************

void StarWarsBuzzerBeep()
{
beep(NOTE_A4, 500);
 beep(NOTE_A4, 500);    
  beep(NOTE_A4, 500);
   beep(NOTE_F4, 350);
    beep(NOTE_C5, 150);  
     beep(NOTE_A4, 500);
      beep(NOTE_F4, 350);
       beep(NOTE_C5, 150);
        beep(NOTE_A4, 650);
 
MyWaitLoop(500);
 
beep(NOTE_E5, 500);
 beep(NOTE_E5, 500);
  beep(NOTE_E5, 500);  
   beep(NOTE_F5, 350);
    beep(NOTE_C5, 150);
     beep(NOTE_GS4, 500);
      beep(NOTE_F4, 350);
       beep(NOTE_C5, 150);
        beep(NOTE_A4, 650);
        
MyWaitLoop(500);

beep(NOTE_A5, 500);
 beep(NOTE_A4, 300);
  beep(NOTE_A4, 150);
   beep(NOTE_A5, 500);
    beep(NOTE_GS5, 325);
     beep(NOTE_G5, 175);
      beep(NOTE_FS5, 125);
       beep(NOTE_F5, 125);    
        beep(NOTE_FS5, 250);
        
MyWaitLoop(325);

beep(NOTE_AS4, 250);
 beep(NOTE_DS5, 500);
  beep(NOTE_D5, 325);  
   beep(NOTE_CS5, 175);  
    beep(NOTE_C5, 125);  
     beep(NOTE_AS4, 125);  
      beep(NOTE_C5, 250);  
 
MyWaitLoop(350);
}

// *************************************************************************************************

int FormatedDisplayOffTime()
{
String fTime;
fTime = FDOffTime;
if (fTime != "") {
int DP = fTime.indexOf(':'); 
fTime.remove(DP, 1);
} else {fTime = "0";} 
return fTime.toInt();
}

// *************************************************************************************************

/*void TelegramMessages(){
if (BOTtoken != "" && BOTname != "" && BOTusername != "") { 
bot.getUpdates(bot.message[0][1]);   
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++){
// bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
Serial.println(bot.message[i][4]+ "   "+bot.message[i][5]);
  }
// bot.message[0][0] = "";   // All messages have been replied - reset new messages  
}
}*/

// *************************************************************************************************
// *************************************************************************************************
