// **************************** Meine Funktionen ***************************************************
// *************************************************************************************************

void greatings() {                       // uae = ä Zeichen 
getTimeLocal(); // aktuelle Zeit holen   // uoe = ö Zeichen 
GreetingsToday = false;                  // uue = ü Zeichen
bool dFound = false;                     // uss = ß Zeichen  
GT = ""; HT = "";                        // deg = ° Grad Zeichen  - example : HollyDay 1.Mai
String GreetingText = "";                // WatchOwner Only
WatchOwner1.trim(); Gender1.trim(); Birthday1.trim();
WatchOwner2.trim(); Gender2.trim(); Birthday2.trim();
WatchOwner3.trim(); Gender3.trim(); Birthday3.trim();
// ***************************** "Geburtstage oder andere Grüße" ********************************* 
// 1. WatchOwner 
if (WatchOwner1 != "" && Gender1 != "" && Birthday1 != "") { // nur Geburtstag Besitzer der Uhr
// **** Ein Hallo beim Start an den Hauptbesitzer ***
if (ClockStart == true) {
GreetingText = "Hallo "+WatchOwner1; 
GT= "       +++     "+GreetingText+"     +++                  "; 
SIC == true; // for ScrollIn Clock 
printStringWithShift(GT.c_str(), ScrollTextTime);}
// *************************************************
if  ((String(d) + "." + String(mo)) == Birthday1) {  
if (Gender1 == "male") {  // Geburtstagsgruß an Hauptbesitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner1;} 
if (Gender1 == "female") { // Geburtstagsgruß an Hauptbesitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner1;}
if (Gender1 == "male" || Gender1 == "female") {
if (ClockStart == true) {
GT= "       +++     "+GreetingText+"     +++                  ";} else {  
GT= "       +++     "+GreetingText+"     +++           ";} 
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
printStringWithShift(GT.c_str(), ScrollTextTime);}}
// 2. WatchOwner (Optional)
if (WatchOwner2 != "" && Gender2 != "" && Birthday2 != "") {
if  ((String(d) + "." + String(mo)) == Birthday2) {  
if (Gender2 == "male") { // Geburtstagsgruß an 2. Besitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner2;} 
if (Gender2 == "female") { // Geburtstagsgruß an 2. Besitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner2;}
if (Gender2 == "male" || Gender2 == "female") {
if (ClockStart == true) {
GT= "       +++     "+GreetingText+"     +++                  ";} else {  
GT= "       +++     "+GreetingText+"     +++           ";} 
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
printStringWithShift(GT.c_str(), ScrollTextTime);}}}
// 3. WatchOwner (Optional)
if (WatchOwner3 != "" && Gender3 != "" && Birthday3 != "") {
if  ((String(d) + "." + String(mo)) == Birthday3) {  
if (Gender3 == "male") { // Geburtstagsgruß an 3. Besitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner3;} 
if (Gender3 == "female") { // Geburtstagsgruß an 3. Besitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner3;}
if (Gender3 == "male" || Gender3 == "female") {
if (ClockStart == true) {
GT= "       +++     "+GreetingText+"     +++                  ";} else {  
GT= "       +++     "+GreetingText+"     +++           ";} 
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
printStringWithShift(GT.c_str(), ScrollTextTime);}}
}
} else {  // nur andere Grüße 
int GreetingsCount = 1; // je nachdem wieviele Grüße oder Meldungen                                
String GreetingsDate[GreetingsCount] {"0.0"}; // GreetingsCount = 1
String GreetingsText[GreetingsCount] {"---No Greetings---"}; // No Greetings
// ---------------------------------------------------------------------------------------------------
// String GreetingsDate[GreetingsCount] {"17.3", "1.4", "18.4", "10.7"}; // GreetingsCount = 4
// String GreetingsText[GreetingsCount] {"Hier ein Gruß oder anderer Text für den 17.3",
//                                       "Hier ein Gruß oder anderer Text für den 1.4",
//                                       "Hier ein Gruß oder anderer Text für den 18.4",
//                                       "Hier ein Gruß oder anderer Text für den 10.7"}; 
// ---------------------------------------------------------------------------------------------------                                        
// String GreetingsDate[GreetingsCount] {"18.4", "10.7"}; // GreetingsCount = 2
// String GreetingsText[GreetingsCount] {"Hier ein Gruß oder anderer Text für den 18.4",
//                                       "Hier ein Gruß oder anderer Text für den 10.7"};  
// ---------------------------------------------------------------------------------------------------                                                                                                                                                                                                                                                                            
for (int i = 0; i < GreetingsCount; i++){
if  ((String(d) + "." + String(mo)) == GreetingsDate[i]) {
if (ClockStart == true) {
GT= "       +++     "+GreetingsText[i]+"     +++                  ";} else {  
GT= "       +++     "+GreetingsText[i]+"     +++           ";} 
dFound = true;}} // Grüße
if (dFound == true){
GreetingsToday = true;
dFound = false;
if (SerialMonitor) {Serial.println("Greetings: "+GT);}  
SIC == true; // for ScrollIn Clock 
printStringWithShift(GT.c_str(), ScrollTextTime);
}  
}

// ***************************** "Feiertage" *********************************************************
int HollyDayCount = 6; 
String HollyDayDate[HollyDayCount] {"1.1", "1.5", "24.12", "25.12", "26.12", "31.12"};
String HollyDayText[HollyDayCount] {"Alles gute im neuen Jahr",
                                    "Sch"+String(uoe)+"nen 1. Mai",
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
if (SerialMonitor) {Serial.println("Holliday: "+HT);}
SIC == true; // for ScrollIn Clock  
printStringWithShift(HT.c_str(), ScrollTextTime);
}
} // End function greatings

// *************************************************************************************************

void clearscreen() { 
for(int i=0; i<10; i++) {
if (SerialMonitor) {Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");}
}
}

// *************************************************************************************************

void ClockScrollIn() {
String TimeString;
// Uhrzeit
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m);} else {TimeString = "  "+String(h)+String(m);}
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);
}

// *************************************************************************************************

void ShowGhost() {
String GShow = ""; 
if (SerialMonitor) {Serial.println("Geisterstunde .....");} 
if (Display) {
sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten 
printStringWithShift3("         ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
GShow = String(ghost)+"  "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // kurz warten
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // kurz warten
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
getTimeLocal(); // aktuelle Zeit holen 
sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten 
for (int i=0; i <= 20; i++){
HandleOTAandWebServer();  // OTA und Webserver Abfrage  
getAnimClock(false);} // Uhrzeit anzeigen / berechnen
printStringWithShift3("      ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
MyWaitLoop(100); // kurz warten
ClockScrollIn();
} else {
printStringWithShift3("      ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
GShow = String(ghost)+"  "+String(ghost);
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // kurz warten
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // kurz warten 
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten
}   
}

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Einstellungen laden .... ***
swait = true; // während Load kein Websitehandle
bool MDTime; // Hilfsvariable
// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit == true) {
if (SerialMonitor) {Serial.println("Load Clock Adjustments ...");}
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt").c_str();
  } else {ssid = "";}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = "";}
  // *** Zugangsdaten OpenWeatherMap laden ***
  // OpenWeatherMap API-Key als String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt").c_str();
  } else {weatherKey = "";}
  // OpenWeatherMap City-ID als String
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt").c_str();
  } else {cityID = "";} 
  // NewsAPI-Key
  if (LittleFS.exists("/newsapi-apikey.txt")){
   newsKey = SetupLoad("newsapi-apikey.txt");
   newsKey.trim();} else {newsKey = "";} // https://newsapi.org

   // OpenWeatherMAP deaktivieren wenn kein(e) API-Key oder City-ID vorhanden
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // NewsAPI deaktivieren wenn kein API-Key vorhanden
   if (newsKey != "") {AccessNewsAPI = true;} else {
   AccessNewsAPI = false;}   

  // Wetterdatenfunktion EIN/AUS 
  if (AccessOpenWeatherMap) {
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = "aktiviert";}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = "deaktiviert";}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = "aktiviert";}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = "deaktiviert";}

  // NewsAPI Funktion EIN/AUS
  if (AccessNewsAPI) { 
  if (LittleFS.exists("/status_newsapifunctions.txt")){
  String NAF = SetupLoad("status_newsapifunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsAPIFunction = true; StatusNewsAPI = "aktiviert";}
  if (NAF == "Off") {NewsAPIFunction = false; StatusNewsAPI = "deaktiviert";}} else {
  NewsAPIFunction = true; StatusNewsAPI = "aktiviert";}} else {
  NewsAPIFunction = false; StatusNewsAPI = "deaktiviert";}  

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
  InTempOffsetInt = InTempOffset.toInt(); // IntegerCheck erfolgte im Captive Portal
  } else {InTempOffset = "0";
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Leerzeichen vorne und Hinten entfernen
  InHumOffsetInt = InHumOffset.toInt(); // IntegerCheck erfolgte im Captive Portal
  } else {InHumOffset = "0";
          InHumOffsetInt = 0;}  // Humidity

  // Ist ein Passiv Buzzer aktiviert ?
  if (LittleFS.exists("/buzzer-install.txt")){
  if (SetupLoad("buzzer-install.txt") == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Status Buzzermodi
  if (IsBuzzer) {
  if (LittleFS.exists("/status_startsound.txt")){
  StartSound = SetupLoad("status_startsound.txt");} else {StartSound = "Off";}
  if (LittleFS.exists("/status_hoursound.txt")){
  HourSound = SetupLoad("status_hoursound.txt");} else {HourSound = "Off";}}

  /*
  if (SerialMonitor) {
  Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Loaded-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Loaded-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Loaded-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Loaded-DHT] InTempOffset: " + InTempOffset);  // Only for Debuging
  Serial.println("[Loaded-DHT] InHumOffset: " + InHumOffset);}  // Only for Debuging*/

  // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
  CheckLEDMatrixDisplayOnOff();

  /*if (SerialMonitor) {
    Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println("");} // for Debug Mode*/

  // Helligkeit der LED-Matrix
  if (LittleFS.exists("/matrix_brightness.txt")){
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // bei leerer Datei oder Fehler Stufe 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Stufe 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Stufe 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Stufe 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // Stufe 5 - Stufe 1
  // Doppelpunktstatus
  if (LittleFS.exists("/status_doublepoint.txt")){
  String DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // bei leerer Datei oder Fehler
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}} else {DoublePointStatus = 1;}
  // ScrollText Geschwindigkeit
  if (LittleFS.exists("/status_scrolltexttime.txt")){
  String STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "25") {ScrollTextTime = 25;} // Stufe 8
  if (STT == "30") {ScrollTextTime = 30;} // Stufe 7
  if (STT == "35") {ScrollTextTime = 35;} // Stufe 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Default = Stufe 5
  if (STT == "45") {ScrollTextTime = 45;} // Stufe 4
  if (STT == "50") {ScrollTextTime = 50;} // Stufe 3
  if (STT == "55") {ScrollTextTime = 55;} // Stufe 2
  if (STT == "60") {ScrollTextTime = 60;}} else {ScrollTextTime = 40;} // Stufe 1 - Stufe 5 
  
  // Zusatzfunktionen EIN/AUS
  if (LittleFS.exists("/status_enhancedfunctions.txt")){
  String EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}} else {EnhancedFunctions = true;}

 /*weatherKey = ""; // Only for Debuging 
   cityID = ""; // Only for Debuging 
   newsKey = ""; // Only for Debuging*/ 

  /*if (SerialMonitor) {
  Serial.println("[Loaded-WIFI] SSID: " + ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + password); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Loaded-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Loaded-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Loaded-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Loaded-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Loaded-DHT] InTempOffset: " + (String)InTempOffsetInt);  // Only for Debuging
  Serial.println("[Loaded-DHT] InHumOffset: " + (String)InHumOffsetInt);}  // Only for Debuging*/ 
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
    weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
    cityID = ""; // City ID von OpenWeatherMap
    newsKey = ""; // API-Key von NewsAPI - https://newsapi.org
    InTempOffsetInt = -1; // Temperature Offset
    InHumOffsetInt = -1; // Humidity  Offset
    StartSound = "Off"; // kein Buzzer Sound
    HourSound = "Off"; // kein Buzzer Sound
    }
 swait = false; // während Load kein Websitehandle     
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
  swait = true;  // während Save kein Websitehandle
  if (littleFSInit == true) {
  if (SerialMonitor) {Serial.println("Save Clock Adjustments ...");}    
  // Trim
  TempSSID.trim(); // SSID WLAN
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
  // Ist ein Passiv Buzzer aktiviert ?
  if (IsBuzzer){
  SetupSave("buzzer-install.txt", "True");} else {
  if (LittleFS.exists("/buzzer-install.txt")){LittleFS.remove("/buzzer-install.txt");}}

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

  /*if (SerialMonitor) {
  Serial.println("[Save-WIFI] SSID: " + TempSSID); // Only for Debuging 
  Serial.println("[Save-WIFI] PASS: " + password); // Only for Debuging 
  Serial.println("[Save-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  Serial.println("[Save-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  Serial.println("[Save-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  Serial.println("[Save-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  Serial.println("[Save-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging
  Serial.println("[Save-DHT] InTempOffset: " + InTempOffset);  // Only for Debuging
  Serial.println("[Save-DHT] InHumOffset: " + InHumOffset);}  // Only for Debuging*/ 
  }
 swait = false;  // während Save kein Websitehandle 
}

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
swait = true;  
String result = ""; // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  result.trim();
  MyWaitLoop(10); //wait
  swait = false;
  return result;
}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {
  swait = true;  
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
  swait = false;
  return true;
}

// *************************************************************************************************

// Display per Taster Ein- und Ausschalten
void MatrixButtonOnOff(){
if(digitalRead(PushButtonPin) == LOW){
MyWaitLoop(500); //wait
Display = !Display;
if (!Display && ClockWiFiConnectFail == false){  
sendCmdAll(CMD_SHUTDOWN,0); // Display ausschalten
if (SerialMonitor) {Serial.println("Matrix per Taster um "+ FormatedDateTime() + " ausgeschaltet.");} 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // aktuelle Uhrzeit holen 
    sendCmdAll(CMD_SHUTDOWN, 1); // Display einschalten
    sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
    // ShortBuzzerBeep();
    if (SerialMonitor) {Serial.println("Matrix per Taster um "+ FormatedDateTime() + " eingeschaltet.");}} else
    {
    sendCmdAll(CMD_SHUTDOWN, 1); // nur Display einschalten
    printCharWithShift('.',5);
    }
    // Nach 10x Ausschalten den "Access Point" aktivieren
    APStart += 1; // inc  
    // if (SerialMonitor) {Serial.println("Taster Count: "+String(APStart));}
    if (APStart == 10 && littleFSInit == true) {
    ResetClock();} // Uhr Zurücksetzen 
    }
    delay(250); //wait
   }
}

// *************************************************************************************************

String FormatedDateTime() {
String DateTimeString = "";
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
DateTimeString += " Uhr / "; // Uhrzeit
// Datum
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += ".0"+String(mo)+"."+String(ye);} else {DateTimeString += "."+String(mo)+"."+String(ye);}
return DateTimeString;    
}

// *************************************************************************************************

// Arduino OTA-Abfrage nur alle 0,5 sek. und Webserverabfrage nur alle 0,5 sek.
void HandleOTAandWebServer(){ 
if (millis() - hwsTime > 800) { 
hwsTime = millis(); 
if (!swait) {
server.handleClient();}} else {
ArduinoOTA.handle();} // OTA Abfrage
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
if (SerialMonitor) {Serial.print(".");} 
if (!ClockStart && Display) {printCharWithShift2('.',1);}
MatrixButtonOnOff(); // Taster abfragen
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED)
{ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
APStart = 0; // "Access Point" Button Counter zurücksetzen
if (!Start) {if (SerialMonitor) {Serial.println("WLAN-Verbindung getrennt ...");}} else {StartFail = true;} 
} else
{
ClockWiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop));} // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {
Serial.println("");
Serial.print("Verbunden mit IP-Adresse: "); 
Serial.println(WiFi.localIP());}
}
}

// *************************************************************************************************

void WiFiUDPStart()
{ 
Udp.begin(localPort);
if (SerialMonitor) {
Serial.print("lokaler Port: ");
Serial.println(Udp.localPort());
Serial.println("NTP-Time - Warten auf die Synchronisation");}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
if (SerialMonitor) {Serial.println("UDP Server gestartet - "+FormatedDateTime());}  
}

// *************************************************************************************************

void WiFiUDPStop()
{
Udp.stop();
if (SerialMonitor) {Serial.println("UDP Server gestoppt - "+FormatedDateTime());}   
}

// *************************************************************************************************

void WebServerStart()
{
server.begin();
if (SerialMonitor) {Serial.println("Webserver gestartet - "+FormatedDateTime());}  
}

// *************************************************************************************************

void WebServerStop()
{
server.stop();
if (SerialMonitor) {Serial.println("Webserver gestoppt - "+FormatedDateTime());} 
}

// *************************************************************************************************

void MyWaitLoop(int wlt)
{// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) // Pause in Millisekunden
{ 
waitloop += 1; // inc waitloop 1
delay(1);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
yield(); // Equivalent zu  Application.ProcessMessages; 
// if (SerialMonitor) {Serial.println(String(waitloop));}
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
// if (SerialMonitor) {Serial.println("Access Point Button Reset");}
APStart = 0; // "Access Point" Button Counter alle 2 Minuten zurücksetzen
} 
}    
}

// *************************************************************************************************

void Download()
{
if (ClockStart) {  
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
if (ClockStart) { 
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
// Passive Buzzer
if (IsBuzzer){
SetupSave("buzzer-install.txt", "True");}
MyWaitLoop(500); // 0,5 sek. warten 
if (SerialMonitor) {Serial.println("Reset - Uhr wird neu gestartet");}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus
printStringWithShift("        +++     Reset - Uhr wird neu gestartet      +++           ",ScrollTextTime);
ESP.restart();   
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

void SendHTMLString(String SHTMLString) {
server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
server.sendHeader("Pragma", "no-cache");
server.sendHeader("Expires", "-1");  
server.send(200, "text/html", SHTMLString);
SHTMLString = ""; // String leeren 
}

// *************************************************************************************************

void StringBytes(String myString) { // Check wieviel Arbeitsspeicher ein String belegt.
unsigned int len = myString.length() + 1;
byte buff[len];
String GesBuff;
int gesBuff = 0;
myString.getBytes(buff, len);
if (SerialMonitor) {Serial.print("String-Size in Byte: ");}
for (int i = 0; i < len; i++){
if (SerialMonitor) {  
GesBuff = Serial.println(buff[i]);}
gesBuff += GesBuff.toInt();}
if (SerialMonitor) {
Serial.println("+++++++");
Serial.println(gesBuff);
Serial.println("+++++++");}
}

// *************************************************************************************************

/*void TelegramMessages(){
if (BOTtoken != "" && BOTname != "" && BOTusername != "") { 
bot.getUpdates(bot.message[0][1]);   
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++){
// bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
if (SerialMonitor) {Serial.println(bot.message[i][4]+ "   "+bot.message[i][5]);}
  }
// bot.message[0][0] = "";   // All messages have been replied - reset new messages  
}
}*/

// *************************************************************************************************
// *************************************************************************************************
