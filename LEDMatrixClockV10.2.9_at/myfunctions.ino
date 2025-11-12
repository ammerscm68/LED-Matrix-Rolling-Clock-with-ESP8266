// **************************** Meine Funktionen ***************************************************
// *************************************************************************************************

void Greetings_and_Holiday(bool GHStart) {   // uae = ä Zeichen 
getTimeLocal(); // aktuelle Zeit holen       // uoe = ö Zeichen 
GreetingsToday = false;                      // uue = ü Zeichen                                         
GT = F(""); HT = F("");                      // deg = ° Grad Zeichen  - example : HollyDay 1.Mai
String GreetingText = F("");                 // WatchOwner Only
int HDay = 0;                                // uss = ß Zeichen  
int EasterSunday;
WatchOwner1FirstName.trim(); WatchOwner1LastName.trim(); Gender1.trim(); Birthday1.trim();
WatchOwner2FirstName.trim(); WatchOwner2LastName.trim(); Gender2.trim(); Birthday2.trim();
WatchOwner3FirstName.trim(); WatchOwner3LastName.trim(); Gender3.trim(); Birthday3.trim();
// ***************************** "Geburtstage oder andere Grüße" ********************************* 
// 1. WatchOwner 
if (WatchOwner1FirstName != "" && Gender1 != "" && Birthday1 != "") { // nur Geburtstag der Besitzer dieser Uhr
if  ((String(d) + "." + String(mo)) == Birthday1) {  
if (Gender1 == "male") {  // Geburtstagsgruß an Hauptbesitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner1FirstName;} 
if (Gender1 == "female") { // Geburtstagsgruß an Hauptbesitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner1FirstName;}
if (Gender1 == "male" || Gender1 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Grüße: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}
// 2. WatchOwner (Optional)
if (WatchOwner2FirstName != "" && Gender2 != "" && Birthday2 != "") {
if  ((String(d) + "." + String(mo)) == Birthday2) {  
if (Gender2 == "male") { // Geburtstagsgruß an 2. Besitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner2FirstName;} 
if (Gender2 == "female") { // Geburtstagsgruß an 2. Besitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner2FirstName;}
if (Gender2 == "male" || Gender2 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}
// 3. WatchOwner (Optional)
if (WatchOwner3FirstName != "" && Gender3 != "" && Birthday3 != "") {
if  ((String(d) + "." + String(mo)) == Birthday3) {  
if (Gender3 == "male") { // Geburtstagsgruß an 3. Besitzer männlich
GreetingText = "Alles gute zum Geburtstag lieber "+WatchOwner3FirstName;} 
if (Gender3 == "female") { // Geburtstagsgruß an 3. Besitzer weiblich
GreetingText = "Alles gute zum Geburtstag liebe "+WatchOwner3FirstName;}
if (Gender3 == "male" || Gender3 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}} // Output GreetingText

// ***************************** "Feiertage" *********************************************************
if (IsFederalStateOK(FederalState)) {
String HolidayDate = String(d) + "." + String(mo);
String HoliDayText = "";
// Zuerst die festen Feiertage
if (HolidayDate == "1.1") {HoliDayText = F("Neujahr");}   // Neujahr
if (HolidayDate == "6.1") {HoliDayText = F("Heilige Drei Könige");}   // Heilige Drei Könige
if (HolidayDate == "14.2") {HoliDayText = F("Valentinstag");} // Valentinstag (kein Feiertag - besonderer Außnahmefall)
if (HolidayDate == "19.3" && (FederalState == "K" || FederalState == "ST" || FederalState == "T" || FederalState == "V") ) {HoliDayText = F("Josefstag");}   // Josef (kein bundesweiter Feiertag)
if (HolidayDate == "1.5") {HoliDayText = F("Staatsfeiertag");}   // Staatsfeiertag
if (HolidayDate == "4.5" && FederalState == "O") {HoliDayText = F("Florianstag");}   // Florian
if (HolidayDate == "15.8") {HoliDayText = "Mari+"+String(uae)+" Himmelfahrt";}   // Mariä Himmelfahrt
if (HolidayDate == "24.9" && FederalState == "S") {HoliDayText = F("Rupertstag");}   // Rupert (kein bundesweiter Feiertag)
if (HolidayDate == "10.10" && FederalState == "K") {HoliDayText = F("Tag der Volksabstimmung");}  // Tag der Volksabstimmung (kein bundesweiter Feiertag)
if (HolidayDate == "26.10") {HoliDayText = F("Nationalfeiertag");}   // Nationalfeiertag
if (HolidayDate == "1.11") {HoliDayText = F("Allerheiligen");}   // Allerheiligen
if (HolidayDate == "11.11" && FederalState == "B") {HoliDayText = F("Martinstag");}   // Martin (kein bundesweiter Feiertag)
if (HolidayDate == "15.11" && (FederalState == "N" || FederalState == "W")) {HoliDayText = F("Leopoldstag");}   // Leopold (kein bundesweiter Feiertag)
if (HolidayDate == "8.12") {HoliDayText = "Mari+"+String(uae)+" Empf"+String(uae)+"ngnis";}   // Mariä Empfängnis
if (HolidayDate == "24.12") {HoliDayText = F("Heiligabend");} // Heiligabend (eigentlich kein Feiertag)
if (HolidayDate == "25.12") {HoliDayText = F("1. Weihnachtsfeiertag");} // Erster Weihnachtsfeirtag"
if (HolidayDate == "26.12") {HoliDayText = F("Stefanitag");} // Zweiter Weihnachsfeiertag
if (HolidayDate == "31.12") {HoliDayText = F("Silvester");}   // Silvester (kein Feiertag - Außnahmefall)
 
// Danke an "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916)
if (mo>2 && mo<7) {
if (mo==3) {HDay=d;}  // Wenn März, aktuellen Tag ermitteln
if (mo==4) {HDay=d+31;} 
if (mo==5) {HDay=d+31+30;} 
if (mo==6) {HDay=d+31+30+31;} 
EasterSunday = Calculate_Eastern();
if (SerialMonitor) {
Serial.println(F("")); 
Serial.println("Ostersonntag: "+String(EasterSunday)); // Ostersonntag anzeigen
Serial.println(F("")); } 
if (EasterSunday==HDay)  {HoliDayText = F("Ostersonntag");}     // Ostersonntag
if (EasterSunday-2==HDay)  {HoliDayText = F("Karfreitag");}   // Karfreitag
if (EasterSunday+1==HDay)  {HoliDayText = F("Ostermontag");}   // Ostermontag
if (EasterSunday+39==HDay)  {HoliDayText = F("Christi Himmelfahrt");}  // Christi Himmelfahrt
if (EasterSunday+49==HDay)  {HoliDayText = F("Pfingstsonntag");}  // Pfingstsonntag
if (EasterSunday+50==HDay)  {HoliDayText = F("Pfingstmontag");} // Pfingstmontag
// Fronleichnam
if (EasterSunday+60==HDay) {HoliDayText = F("Fronleichnam");}}   // Fronleichnam

if (HoliDayText != "") {
GreetingsToday = true;
if (HolidayDate != "31.12") { 
HT= "          +++     Heute:   "+cleanText(HoliDayText)+"     +++                ";
HT1= "+++ "+HoliDayText+" +++";  
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Feiertag: "+HT1); // Zeige Holiday Text
Serial.println(F(""));}} else { // Silvester
HT= F("       +++     Guten Rutsch ins neue Jahr     +++                ");
HT1= F("+++ Guten Rutsch ins neue Jahr +++");
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Heute: Silvester"); // Zeige Silvester Text
Serial.println(F(""));}}
if (Display && !GHStart) { // Feiertag Textausgabe wenn Display an
if (HolidayDate != "1.1") {  
printStringWithShift(HT.c_str(), ScrollTextTime);
if (HolidayDate == "14.2") {ValentinesDay();}} else {  // Valentinstag Anzeige von Herzchen 
printStringWithShift("           ", ScrollTextTime); 
for (int i=0; i < 3; i++){ // 3x Neujahresgruß anzeigen    
printStringWithShift("          +++     Alles gute im neuen Jahr     +++                ", ScrollTextTime);}
ClockScrollIn(); SIC = false;}}} else {
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println(F("Feiertag: kein Feiertag"));
Serial.println(F(""));}}} else {
if (SerialMonitor) {
Serial.println(F("")); 
Serial.println(F("Funktion Feiertage deaktiviert!"));
Serial.println(F("")); }}} // Ende der Funktion "void Greetings_and_Holiday()"

// *************************************************************************************************

void InitSerial(int Baud, int Wait) { 
Serial.begin(Baud);  
while (!Serial) {Serial.print(F("*"));}
printStringWithShift("COM !  ",38); // COM INIT
for (int i = 0; i < Wait; i++) {
Serial.print(F("*")); MyWaitLoop(100);}
printStringWithShift("           ",38); 
/*for(int i=0; i<10; i++) { 
Serial.println(F("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"));}*/}

// *************************************************************************************************

void ClockScrollIn() {
String TimeString; 
sendCmdAll(CMD_SHUTDOWN,0); // Display aus
for (int i=0; i <= 50; i++){ 
MyWaitLoop(10);     
HandleOTAandWebServer();  // OTA und Webserver Abfrage 
getAnimClock(false);
if (s == 59 && m < 59) {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m+1);} else {TimeString = "  "+String(h)+String(m+1);}
} else {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m);} else {TimeString = "  "+String(h)+String(m);}}}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display an 
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);}

// *************************************************************************************************

void ShowGhost() {
String GShow = F(""); 
// Sonderzeichen und Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Zeichen
if (SerialMonitor) {Serial.println(F("Geisterstunde ....."));} 
if (Display) {
sendCmdAll(CMD_SHUTDOWN,0); // Matrix ausschalten 
printStringWithShift3("         ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
GShow = String(ghost)+"  "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(5000); // kurz warten
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime+10);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // kurz warten
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
printStringWithShift3("      ",ScrollTextTime+5);
MyWaitLoop(100); // kurz warten
ClockScrollIn();
} else {
printStringWithShift3("      ",ScrollTextTime-5);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
GShow = String(ghost)+"  "+String(ghost);
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // kurz warten
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime+5);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // kurz warten 
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,0);}} // Matrix ausschalten

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Einstellungen laden .... ***
swait = true; // während Load kein Websitehandle
bool MDTime; // Hilfsvariable
// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit) {
if (SerialMonitor) {Serial.println(F("Lade Einstellungen der Uhr ..."));}
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt").c_str();
  } else {ssid = F("");}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = F("");}

  // Zeitzone laden
  if (LittleFS.exists("/clocktimezone.txt")){
  MCTZ = SetupLoad("clocktimezone.txt").c_str();}

  // NTP-Server laden
  if (LittleFS.exists("/ntpservername.txt")){
  NTPSN = SetupLoad("ntpservername.txt").c_str();}
  if (NTPSN != "") {ntpServerName = NTPSN.c_str();} else {ntpServerName = "at.pool.ntp.org";} // Default

  // Besitzer der Uhr laden
 if (LittleFS.exists("/owner1-firstname.txt")){
 WatchOwner1FirstName = SetupLoad("owner1-firstname.txt").c_str();}
 if (LittleFS.exists("/owner1-lastname.txt")){
 WatchOwner1LastName = SetupLoad("owner1-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner1-gender.txt")){
 Gender1 = SetupLoad("owner1-gender.txt").c_str();} 
 if (LittleFS.exists("/owner1-birthday.txt")){
 Birthday1 = SetupLoad("owner1-birthday.txt").c_str();} 
 if (LittleFS.exists("/owner2-firstname.txt")){
 WatchOwner2FirstName = SetupLoad("owner2-firstname.txt").c_str();} 
 if (LittleFS.exists("/owner2-lastname.txt")){
 WatchOwner2LastName = SetupLoad("owner2-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner2-gender.txt")){
 Gender2 = SetupLoad("owner2-gender.txt").c_str();} 
 if (LittleFS.exists("/owner2-birthday.txt")){
 Birthday2 = SetupLoad("owner2-birthday.txt").c_str();} 
 if (LittleFS.exists("/owner3-firstname.txt")){
 WatchOwner3FirstName = SetupLoad("owner3-firstname.txt").c_str();} 
 if (LittleFS.exists("/owner3-lastname.txt")){
 WatchOwner3LastName = SetupLoad("owner3-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner3-gender.txt")){
 Gender3 = SetupLoad("owner3-gender.txt").c_str();} 
 if (LittleFS.exists("/owner3-birthday.txt")){
 Birthday3 = SetupLoad("owner3-birthday.txt").c_str();} 

 if (LittleFS.exists("/midnightghost.txt")){
 MidnightGhost = SetupLoad("midnightghost.txt").c_str();} 
 if (SerialMonitor) {
 Serial.println(F(""));  
 Serial.println(F("Mitternacht Geister anzeigen ?: [YES / NO] ")+ MidnightGhost);
 Serial.println(F(""));}

 // Auswahl DHT-Sensor [DHT11 oder DHT22]
 if (LittleFS.exists("/dhtsensor.txt")){
 DHTSensor = SetupLoad("dhtsensor.txt").c_str();
 if (DHTSensor == "DHT11") {DHT11Sensor = true;}
 if (DHTSensor == "DHT22") {DHT11Sensor = false;}} else {
 if (DHT11Sensor) {DHTSensor = "DHT11";} else {DHTSensor = "DHT22";}}  
 if (SerialMonitor) {
 Serial.println(F(""));  
 Serial.println(F("Auswahl DHT-Sensor : [DHT11 / DHT22] ")+ DHTSensor);
 Serial.println(F(""));}

  // *** Zugangsdaten OpenWeatherMap laden ***
  // OpenWeatherMap API-Key als String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt").c_str();
  } else {weatherKey = F("");}
  // OpenWeatherMap City-ID als String
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt").c_str();
  } else {cityID = F("");} 
  // NewsData-Key
  if (LittleFS.exists("/newsdata-apikey.txt")){
   newsKey = SetupLoad("newsdata-apikey.txt");
   newsKey.trim();} else {newsKey = F("");} // https://NewsData.io
  // GMCMap-History ID / GMT Offset
  if (LittleFS.exists("/gmcmaphistoryid.txt")){
  GMCMapHistoryID = SetupLoad("gmcmaphistoryid.txt");
  GMCMapHistoryID.trim();} else {GMCMapHistoryID = F("");} // www.gmcmap.com 
  if (LittleFS.exists("/gmcmapgmtoffset.txt")){
  GMCMapGMTOffset = SetupLoad("gmcmapgmtoffset.txt");
  GMCMapGMTOffset.trim();} else {GMCMapGMTOffset = F("");} // www.gmcmap.com GMT Offset

   // OpenWeatherMAP deaktivieren wenn kein(e) API-Key oder City-ID vorhanden
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // NewsData deaktivieren wenn kein API-Key vorhanden
   if (newsKey != "") {AccessNewsData = true;} else {
   AccessNewsData = false;}
   //GMCMap deaktivieren wenn keine HistoryID vorhanden
   if (GMCMapHistoryID != "" && GMCMapGMTOffset != "") {AccessGMCMap = true;} else {
   AccessGMCMap = false;} 

  // Wetterdatenfunktion EIN/AUS 
  if (AccessOpenWeatherMap) {
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = F("aktiviert");}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = F("deaktiviert");}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = F("aktiviert");}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = F("deaktiviert");}

  // NewsData Funktion EIN/AUS
  if (AccessNewsData) { 
  if (LittleFS.exists("/status_NewsDatafunctions.txt")){
  String NAF = SetupLoad("status_NewsDatafunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsDataFunction = true; StatusNewsData = F("aktiviert");}
  if (NAF == "Off") {NewsDataFunction = false; StatusNewsData = F("deaktiviert");}} else {
  NewsDataFunction = true; StatusNewsData = F("aktiviert");}} else {
  NewsDataFunction = false; StatusNewsData = F("deaktiviert");}

  // GMCMap Funktion
  if (AccessGMCMap) { 
  if (LittleFS.exists("/status_GMCMapFunction.txt")){
  String GMCF = SetupLoad("status_GMCMapFunction.txt");
  GMCF.trim();
  if (GMCF == "" || GMCF == "On") {GMCMapFunction = true; StatusGMCMap = F("aktiviert");}
  if (GMCF == "Off") {GMCMapFunction = false; StatusGMCMap = F("deaktiviert");}} else {
  GMCMapFunction = true; StatusGMCMap = F("aktiviert");}} else {
  GMCMapFunction = false; StatusGMCMap = F("deaktiviert");} 

   // Auswahl News oder GMC (beides geht leider nicht weil der Speicher nicht groß genug ist)
  if (AccessGMCMap && AccessNewsData) {
  if (GMCMapFunction) {if (NewsDataFunction) {NewsDataFunction = false;}}   
  if (NewsDataFunction) {if (GMCMapFunction) {GMCMapFunction = false;}}} 

  // Pushover Daten laden
  if (LittleFS.exists("/po-secureapptoken.txt")){
  SecureAppToken = SetupLoad("po-secureapptoken.txt");} else {SecureAppToken = F("");}
  SecureAppToken.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-usertoken.txt")){
  UserToken = SetupLoad("po-usertoken.txt");} else {UserToken = F("");}
  UserToken.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device1.txt")){
  PushoverDevice1 = SetupLoad("po-device1.txt");} else {PushoverDevice1 = F("");}
  PushoverDevice1.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device2.txt")){
  PushoverDevice2 = SetupLoad("po-device2.txt");} else {PushoverDevice2 = F("");}
  PushoverDevice2.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-device3.txt")){
  PushoverDevice3 = SetupLoad("po-device3.txt");} else {PushoverDevice3 = F("");}
  PushoverDevice3.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/po-messagesound.txt")){
  PushoverMessageSound = SetupLoad("po-messagesound.txt");} else {PushoverMessageSound= F("none");}
  if (SecureAppToken != "" && UserToken != "" && PushoverDevice1 != "") {
  IsPushover = true;} else {IsPushover = false;}

  // Display Einschaltzeit als String
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOnTime = F("");}
  // Display Ausschaltzeit als String
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOffTime = F("");} 

  if (SerialMonitor) {
  Serial.println("");  
  Serial.println("DisplayOnTime: "+DisplayOnTime);
  Serial.println("DisplayOffTime: "+DisplayOffTime);
  Serial.println("");}

  // DHT-Sensor Offset
  if (LittleFS.exists("/dht-intempoffset.txt")){
  InTempOffset = SetupLoad("dht-intempoffset.txt");
  InTempOffset.trim(); // Leerzeichen vorne und Hinten entfernen
  InTempOffsetInt = InTempOffset.toInt(); 
  } else {InTempOffset = F("0");
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Leerzeichen vorne und Hinten entfernen
  InHumOffsetInt = InHumOffset.toInt(); 
  } else {InHumOffset = F("0");
          InHumOffsetInt = 0;}  // Humidity

// Bundesland im Speicher löschen (z.Bsp. bei Umzug nach Österreich)
  if (FederalState == "?") {
  if (LittleFS.exists("/federalstate.txt")){LittleFS.remove("/federalstate.txt");}}

  // Bundesland laden (nur wenn vordefinierte Variable gültiges Bundesland)
  if (IsFederalStateOK(FederalState)) {
  if (LittleFS.exists("/federalstate.txt")){
  FederalState = SetupLoad("federalstate.txt");}}

  if (FederalState != "" && FederalState != "-") {
  FederalState.trim(); FederalState.toUpperCase();} 
  if (SerialMonitor) {
  Serial.println(F(" "));  
  Serial.println("Bundesland: "+LongFederalState(FederalState));
  Serial.println(F(" "));}

  // Ist ein Passiv Buzzer aktiviert ?
  if (LittleFS.exists("/buzzer-install.txt")){
  if (SetupLoad("buzzer-install.txt") == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Status Buzzermodi
  if (IsBuzzer) {
  if (LittleFS.exists("/status_startsound.txt")){
  StartSound = SetupLoad("status_startsound.txt");} else {StartSound = F("Off");}
  if (LittleFS.exists("/status_hoursound.txt")){
  HourSound = SetupLoad("status_hoursound.txt");} else {HourSound = F("Off");}}

  // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
  CheckLEDMatrixDisplayOnOff();

  // Helligkeit der LED-Matrix
  if (LittleFS.exists("/matrix_brightness.txt")){
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // bei leerer Datei oder Fehler Stufe 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Stufe 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Stufe 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Stufe 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // Stufe 5 - Stufe 1
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

  LoadClockStatus(); // letzten Status der Uhr laden
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    ssid = F("");
    password = F(""); 
    Brightness = F("L1");
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    DisplayOnTime = F("6:30");
    DisplayOffTime = F("23:15");
    EnhancedFunctions = true;
    WeatherFunctions = false;
    weatherKey = F(""); // API-Key von OpenWeatherMap - https://openweathermap.org
    cityID = F(""); // City ID von OpenWeatherMap
    newsKey = F(""); // API-Key von NewsData - https://NewsData.io
    GMCMapHistoryID = F(""); // www.GMCMap.com - History ID
    GMCMapGMTOffset = F(""); // www.GMCMap.com - GMTOffset
    InTempOffsetInt = -1; // Temperature Offset
    InHumOffsetInt = -1; // Humidity  Offset
    StartSound = F("Off"); // kein Buzzer Sound
    HourSound = F("Off"); // kein Buzzer Sound
    DHTSensor = F("DHT11"); // Default DHT-Sensor
    MCTZ = "Berlin, Frankfurt, Wien, Paris, Madrid"; // Default Zeitzone 
    ntpServerName = "at.pool.ntp.org";  // Finde lokale Server unter http://www.pool.ntp.org/zone
    }
 swait = false;} // während Load kein Websitehandle

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Originale gespeicherte Uhrzeit
  FDOffTime = DisplayOffTime; // Originale gespeicherte Uhrzeit

   // Display On/Off Time auf Plausibilität prüfen
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
  if (DisplayOnTime != "") {
  if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = F("");};
  if (DisplayOnTime != "") {
  if (DisplayOnTime.substring(0,1) == "0") {DisplayOnTime.remove(0, 1);}
  int DP = DisplayOnTime.indexOf(':')+1;
  if (DisplayOnTime.substring(DP,DP+1) == "0") {DisplayOnTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
if (DisplayOnTime != "") {
MDTime = false;
int DP = DisplayOnTime.indexOf(':'); 
String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = F("");} else {   
MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = F("");}}}}}

  if (DisplayOffTime != "") {
  if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = F("");};
  if (DisplayOffTime != "") {
  if (DisplayOffTime.substring(0,1) == "0") {DisplayOffTime.remove(0, 1);}
  int DP = DisplayOffTime.indexOf(':')+1;
  if (DisplayOffTime.substring(DP,DP+1) == "0") {DisplayOffTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
if (DisplayOffTime != "") {
MDTime = false;
int DP = DisplayOffTime.indexOf(':'); 
String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
String CheckMDMinOff=  DisplayOffTime.substring(DP+1,6);
CheckMDMinOff.trim(); 

// Eingaben für Matrix On/Off aufbereiten
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");} else {MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");}}}}}}} // SSID - Ende  

// *************************************************************************************************

void SaveMatrixAdjustments(){
  swait = true;  // während Save kein Websitehandle
  if (littleFSInit == true) {
  if (SerialMonitor) {Serial.println(F("Save Clock Adjustments ..."));}    
  // Trim
  TempSSID.trim(); // SSID WLAN
  newsKey.trim(); // NewsData API-Key
  DisplayOnTime.trim(); // Matrix Display Einschaltzeit
  DisplayOffTime.trim(); // Matrix Display Ausschaltzeit
  InTempOffset.trim(); // DHT-Sensor Offset (Temperatur)
  InHumOffset.trim(); // DHT-Sensor Offset (Luftfeuchte)
  FederalState.trim(); // Bundesland
  FederalState.toUpperCase(); // Bundesland
  WatchOwner1FirstName.trim(); // 1. Besitzer der Uhr - Vorname
  WatchOwner1LastName.trim(); // 1. Besitzer der Uhr - Nachname
  Gender1.trim(); // 1. Besitzer der Uhr - Geschlecht
  Birthday1.trim(); // 1. Besitzer der Uhr - Geburtsdatum
  WatchOwner2FirstName.trim(); // 2. Besitzer der Uhr - Vorname
  WatchOwner2LastName.trim(); // 2. Besitzer der Uhr - Nachname
  Gender2.trim(); // 2. Besitzer der Uhr - Geschlecht
  Birthday2.trim(); // 2. Besitzer der Uhr - Geburtsdatum
  WatchOwner3FirstName.trim(); // 3. Besitzer der Uhr - Vorname
  WatchOwner3LastName.trim(); // 3. Besitzer der Uhr - Nachname
  Gender3.trim(); // 3. Besitzer der Uhr - Geschlecht
  Birthday3.trim(); // 3. Besitzer der Uhr - Geburtsdatum
  
  // SSID speichern
  SetupSave("ssid.txt", TempSSID); // SSID WLAN
  SetupSave("ssid-passwd.txt", password); // Password WLAN

  // Zeitzone speichern
  SetupSave("clocktimezone.txt", MCTZ);
  // NTP-Server speichern
  SetupSave("ntpservername.txt", NTPSN);

  // Besitzer der Uhr speichern
  if (WatchOwner1FirstName != "") {
  SetupSave("owner1-firstname.txt", WatchOwner1FirstName);} else {
  if (LittleFS.exists("/owner1-firstname.txt")){LittleFS.remove("/owner1-firstname.txt");}}  
  if (WatchOwner1LastName != "") {
  SetupSave("owner1-lastname.txt", WatchOwner1LastName);} else {
  if (LittleFS.exists("/owner1-lastname.txt")){LittleFS.remove("/owner1-lastname.txt");}}  
  if (Gender1 != "") {
  SetupSave("owner1-gender.txt", Gender1);} else {
  if (LittleFS.exists("/owner1-gender.txt")){LittleFS.remove("/owner1-gender.txt");}}  
  if (Birthday1 != "") {
  SetupSave("owner1-birthday.txt", Birthday1);} else {
  if (LittleFS.exists("/owner1-birthday.txt")){LittleFS.remove("/owner1-birthday.txt");}}  
  if (WatchOwner2FirstName != "") {
  SetupSave("owner2-firstname.txt", WatchOwner2FirstName);} else {
  if (LittleFS.exists("/owner2-firstname.txt")){LittleFS.remove("/owner2-firstname.txt");}}  
  if (WatchOwner2LastName != "") {
  SetupSave("owner2-lastname.txt", WatchOwner2LastName);} else {
  if (LittleFS.exists("/owner2-lastname.txt")){LittleFS.remove("/owner2-lastname.txt");}}  
  if (Gender2 != "") {
  SetupSave("owner2-gender.txt", Gender2);} else {
  if (LittleFS.exists("/owner2-gender.txt")){LittleFS.remove("/owner2-gender.txt");}}  
  if (Birthday2 != "") {
  SetupSave("owner2-birthday.txt", Birthday2);} else {
  if (LittleFS.exists("/owner2-birthday.txt")){LittleFS.remove("/owner2-birthday.txt");}}  
  if (WatchOwner3FirstName != "") {
  SetupSave("owner3-firstname.txt", WatchOwner3FirstName);} else {
  if (LittleFS.exists("/owner3-firstname.txt")){LittleFS.remove("/owner3-firstname.txt");}}  
  if (WatchOwner3LastName != "") {
  SetupSave("owner3-lastname.txt", WatchOwner3LastName);} else {
  if (LittleFS.exists("/owner3-lastname.txt")){LittleFS.remove("/owner3-lastname.txt");}}  
  if (Gender3 != "") {
  SetupSave("owner3-gender.txt", Gender1);} else {
  if (LittleFS.exists("/owner3-gender.txt")){LittleFS.remove("/owner3-gender.txt");}}  
  if (Birthday3 != "") {
  SetupSave("owner3-birthday.txt", Birthday3);} else {
  if (LittleFS.exists("/owner3-birthday.txt")){LittleFS.remove("/owner3-birthday.txt");}}

  // Anzeige Mitternachtsgeist ? (JA oder NEIN)
  SetupSave("midnightghost.txt", MidnightGhost);
  
  // Auswahl des DHT-Sensors speichern [DHT11 oder DHT22]
  SetupSave("dhtsensor.txt", DHTSensor);
  
  // OpenWeatherMap API-Key und OpenWeatherMap City-ID
  if (weatherKey != "" && cityID != "") {
  SetupSave("owm-apikey.txt", weatherKey);
  SetupSave("owm-cityid.txt", cityID);} else {
  if (LittleFS.exists("/owm-apikey.txt")){LittleFS.remove("/owm-apikey.txt");}
  if (LittleFS.exists("/owm-cityid.txt")){LittleFS.remove("/owm-cityid.txt");}} 
  // NewsData API-Key
  if (newsKey != "") {
  SetupSave("newsdata-apikey.txt", newsKey);} else {
  if (LittleFS.exists("/newsdata-apikey.txt")){LittleFS.remove("/newsdata-apikey.txt");}}
  // GMCMap Data 
  if (GMCMapHistoryID != "") {
  SetupSave("gmcmaphistoryid.txt", GMCMapHistoryID);} else {
  if (LittleFS.exists("/gmcmaphistoryid.txt")){LittleFS.remove("/gmcmaphistoryid.txt");}}
  if (GMCMapGMTOffset != "") {
  SetupSave("gmcmapgmtoffset.txt", GMCMapGMTOffset);} else {
  if (LittleFS.exists("/gmcmapgmtoffset.txt")){LittleFS.remove("/gmcmapgmtoffset.txt");}}
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

  // Bundesland speichern
  if (IsFederalStateOK(FederalState)) {
  SetupSave("federalstate.txt", FederalState);} else {
  if (LittleFS.exists("/federalstate.txt")){LittleFS.remove("/federalstate.txt");}}

  // Pushover SecureAppToken
  if (SecureAppToken == "") {
  if (LittleFS.exists("/po-secureapptoken.txt")){LittleFS.remove("/po-secureapptoken.txt");}  
  } else {SetupSave("po-secureapptoken.txt", SecureAppToken);} 
  // Pushover UserToken
  if (UserToken == "") {
  if (LittleFS.exists("/po-usertoken.txt")){LittleFS.remove("/po-usertoken.txt");} 
  } else {SetupSave("po-usertoken.txt", UserToken);}
  // Pushover Gerätename 1
  if (PushoverDevice1 == "") {
  if (LittleFS.exists("/po-device1.txt")){LittleFS.remove("/po-device1.txt");} 
  } else {SetupSave("po-device1.txt", PushoverDevice1);}
  // Pushover Gerätename 2
  if (PushoverDevice2 == "") {
  if (LittleFS.exists("/po-device2.txt")){LittleFS.remove("/po-device2.txt");} 
  } else {SetupSave("po-device2.txt", PushoverDevice2);}
  // Pushover Gerätename 3
  if (PushoverDevice3 == "") {
  if (LittleFS.exists("/po-device3.txt")){LittleFS.remove("/po-device3.txt");} 
  } else {SetupSave("po-device3.txt", PushoverDevice3);}
  // Pushover Nachrichten Sound
  if (PushoverMessageSound == "none" || PushoverMessageSound == "") {
  if (LittleFS.exists("/po-messagesound.txt")){LittleFS.remove("/po-messagesound.txt");} 
  } else {SetupSave("po-messagesound.txt", PushoverMessageSound);}

  // Check DHT Offset
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = "0";} // Temperature
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = "0";} // Humidity
  
  // DHT-Sensor Offset
  if (InTempOffset != ""){
  SetupSave("dht-intempoffset.txt", InTempOffset);} else {
  if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
  if (InHumOffset != ""){
  SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
  if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}}
 swait = false;}  // während Save kein Websitehandle 

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
swait = true;  
String result = F(""); // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();}
  this_file.close();
  result.trim();
  MyWaitLoop(10); //wait
  swait = false;
  return result;}

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
  return true;}

// *************************************************************************************************

// Display per Taster Ein- und Ausschalten
void MatrixButtonOnOff(bool Start){
if(digitalRead(PushButtonPin) == LOW){
if (Start) {APStart += 1; // inc   
if (SerialMonitor) {Serial.println("Reset Counter: "+ String(APStart));}} else {
Display = !Display;
if (!Display){ 
MyWaitLoop(500); // wait     
sendCmdAll(CMD_SHUTDOWN,0); // Display ausschalten
if (SerialMonitor) {Serial.println("Matrix per Taster um "+ FormatedDateTimeDE(false) + " ausgeschaltet.");}
APStart += 1; // inc   
} else { 
    MyWaitLoop(500); // wait     
    getTimeLocal(); // aktuelle Uhrzeit holen 
    sendCmdAll(CMD_SHUTDOWN, 1); // Display einschalten
    sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
    // ShortBuzzerBeep();
    if (SerialMonitor) {Serial.println("Matrix per Taster um "+ FormatedDateTimeDE(false) + " eingeschaltet.");}
    APStart += 1;}} // inc 
    if (APStart >= 10 && littleFSInit) {ResetClock();} // Uhr Zurücksetzen 
    delay(250);} else {if (APStart > 0) {APStart = 0;}}} // wait

// *************************************************************************************************

String FormatedDateTimeDE(bool FDTHTML) {
String DateTimeString = F("");
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (FDTHTML) {DateTimeString += " Uhr  &nbsp;-/-&nbsp;  ";} else {DateTimeString += " Uhr  -/-  ";}  // Uhrzeit
// Datum
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += ".0"+String(mo)+"."+String(ye);} else {DateTimeString += "."+String(mo)+"."+String(ye);}
return DateTimeString;}

// *************************************************************************************************

// Arduino OTA-Abfrage nur alle 0,8 sek. und Webserverabfrage nur alle 0,8 sek.
void HandleOTAandWebServer(){
if (PushOverTest) {WSR = 20000;} else {WSR = 800;}   // Webserver Refreshrate | default = 800
if (millis() - hwsTime > WSR) { 
hwsTime = millis(); 
if (!swait) {
server.handleClient();}} else {
ArduinoOTA.handle();}} // OTA Abfrage

// *************************************************************************************************

void ClearMatrix() {
for (int i=0; i <= 50; i++){ 
MyWaitLoop(10);     
HandleOTAandWebServer();  // OTA und Webserver Abfrage 
getAnimClock(false);} // Uhrzeit anzeigen / berechnen 
printStringWithShift("           ",1);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix einschalten
}

// *************************************************************************************************

void WaitForClockStartWIFI(int WaitLoop) {
for (int i = 0; i < WaitLoop; i++) { 
MatrixButtonOnOff(true);  
FlashNoWLANDots();}
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { 
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}}
if (WiFi.status() != WL_CONNECTED) {ClockWiFiConnectFail = true;} else {ClockWiFiConnectFail = false;}
WiFi.disconnect();
WiFi.mode(WIFI_OFF);}

// *************************************************************************************************

// WLAN-Verbindung (wieder) herstellen 
void wificonnect(bool Start) {
WiFiConnectLoop = 0; // Variable zurücksetzen  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { 
if (Start) {MyWaitLoop(1000);} else {MyWaitLoop(250);}
if (SerialMonitor) {Serial.print(".");} 
if (!Start && StartStopServers > 0) {
if (WiFiConnectLoop == 0) {await = true;} MatrixButtonOnOff(true); FlashNoWLANDots();} 
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}}
if (WiFi.status() != WL_CONNECTED) {ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
if (Start) {if (SerialMonitor) {Serial.println(F("WLAN-Verbindung fehlgeschlagen ..."));}} else {
if (SerialMonitor) {Serial.println(F("WLAN-Verbindung getrennt ..."));}}
} else {
ClockWiFiConnectFail = false;
APStart = 0; // "Access Point" Button Counter zurücksetzen
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop));} // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {Serial.println(F(""));
Serial.print(F("Verbunden mit IP-Adresse: ")); 
Serial.println(WiFi.localIP());}}}

// *************************************************************************************************

void WiFiUDPStart() { 
Udp.begin(localPort);
if (SerialMonitor) {
Serial.print(F("lokaler Port: "));
Serial.println(Udp.localPort());
Serial.println(F("NTP-Time - Warten auf die Synchronisation"));}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
if (SerialMonitor) {Serial.println("UDP Server gestartet - "+FormatedDateTimeDE(false));}}

// *************************************************************************************************

void WiFiUDPStop() {
Udp.stop();
if (SerialMonitor) {Serial.println("UDP Server gestoppt - "+FormatedDateTimeDE(false));}}

// *************************************************************************************************

void WebServerStart() {
server.begin();
if (SerialMonitor) {Serial.println("Webserver gestartet - "+FormatedDateTimeDE(false));}}

// *************************************************************************************************

void WebServerStop() {
server.stop();
if (SerialMonitor) {Serial.println("Webserver gestoppt - "+FormatedDateTimeDE(false));}}

// *************************************************************************************************

void MyWaitLoop(int wlt){// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) { // Pause in Millisekunden{ 
waitloop += 1; // inc waitloop 1
delay(1);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
yield();}} // Equivalent zu  Application.ProcessMessages; 

// *************************************************************************************************

// Überprüfung einer Zeichenfolge, um festzustellen, ob sie numerisch ist, und akzeptieren vom Dezimalpunkt und den positiven / negativen Wert
boolean IsNumeric(String str) {
if(str.length()<1){return false;}
bool bPoint=false;
if ((str.charAt(0) == '-' || str.charAt(0) == '+') && (str.length()>1)) {  
for(unsigned char i = 1; i < str.length(); i++) {
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};}} else {
for(unsigned char i = 0; i < str.length(); i++) {
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};}}
return true;}


// ********************** Routine by freddy64 ******************************************************

String runtime() {
  static uint8_t rolloverCounter = 0;
  static uint32_t lastMillis = 0;
  uint32_t currentMillis = millis() - bootTime;
  if (currentMillis < lastMillis) {       // check millis overflow
    rolloverCounter++;}
  lastMillis = currentMillis;
  uint32_t secs = (0xFFFFFFFF / 1000 ) * rolloverCounter + (currentMillis / 1000);
  char buf[20];
  sprintf(buf, "%ld %s %02ld:%02ld:%02ld", secs / 86400, (secs < 86400 || secs > 172800) ? "Tage" : "Tag", secs / 3600 % 24, secs / 60 % 60, secs % 60);
  return buf;}

// ************************************************************************************************

void ResetClock() {
APStart = 0; // zurücksetzen
swait = true; // kein Website Handling mehr
bool IsFileExist = false; // zurücksetzen
StatusWarningLED("LOW"); // LED Ausschalten  
if (LittleFS.exists("/ssid.txt")){  
LittleFS.remove("/ssid.txt");}
if (LittleFS.exists("/ssid-passwd.txt")){  
LittleFS.remove("/ssid-passwd.txt");}
MyWaitLoop(500); // 0,5 sek. warten 
if (SerialMonitor) {Serial.println(F("Reset - Die Uhr wird neu gestartet"));}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus
printStringWithShift("        +++     Reset - Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
ESP.restart();}

// ************************************************************************************************

int FormatedDisplayOffTime() {
String fTime;
fTime = FDOffTime;
if (fTime != "") {
int DP = fTime.indexOf(':'); 
fTime.remove(DP, 1);
} else {fTime = F("0");} 
return fTime.toInt();}

// *************************************************************************************************

bool GoMatrixAction() { 
bool LMOTH, DOffMinIntOffset;  
String TempDOffHour, TempDOffMin;
String TempActualTimeHour, TempActualTimeMin;              
if (DisplayOffTime != "") { 
int DP = DisplayOffTime.indexOf(':'); 
String DOffHour = DisplayOffTime.substring(0,DP); 
String DOffMin=  DisplayOffTime.substring(DP+1,6);
DOffHour.trim(); 
DOffMin.trim(); 
int DOffHourInt = DOffHour.toInt();
int DOffMinInt = DOffMin.toInt(); 
DOffMinInt -= 10; // Minus 10 Minuten
if (DOffMinInt <= 0) {
DOffMinInt = 50 + DOffMin.toInt(); 
DOffMinIntOffset = true;
DOffHourInt -= 1;
if (DOffHourInt < 0) {DOffHourInt = 23;}} else {DOffMinIntOffset = false;} 

// Display Off Time -  minus 10 Minuten
if (DOffHourInt < 10) {TempDOffHour = "0"+String(DOffHourInt);} else {TempDOffHour = String(DOffHourInt);}
if (DOffMinInt < 10)  {TempDOffMin = "0"+String(DOffMinInt);} else {TempDOffMin = String(DOffMinInt);}
String ActionTime = TempDOffHour+TempDOffMin;  
int ActionTimeInt = ActionTime.toInt();

int ActualHour = h; // aktuelle Stunde
int ActualMin = m; // aktuelle Minute
// Aktuelle Zeit als String
if (ActualHour < 10) {TempActualTimeHour = "0"+String(ActualHour);} else 
                     {TempActualTimeHour = String(ActualHour);}
if (ActualMin < 10) {TempActualTimeMin = "0"+String(ActualMin);} else 
                    {TempActualTimeMin = String(ActualMin);}
String ActualTime = TempActualTimeHour + TempActualTimeMin;
int ActualTimeInt = ActualTime.toInt();

int TimeDifference = ActualTimeInt - ActionTimeInt;

if (DOffMinIntOffset) {
if (TimeDifference >= 0 && TimeDifference <= 50) {LMOTH = false;} else {LMOTH = true;}} else {
if (TimeDifference >= 0 && TimeDifference <= 10) {LMOTH = false;} else {LMOTH = true;}}
} else {LMOTH = true;}
return LMOTH;}

// *************************************************************************************************

void SendHTMLString(String SHTMLString) {
server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
server.sendHeader("Pragma", "no-cache");
server.sendHeader("Expires", "-1");  
server.send(200, "text/html", SHTMLString);
SHTMLString = F(""); // String leeren
freeRam();} // Defragment Heap 

// *************************************************************************************************

String Politeness(bool WO1F) {
String PReturn = F(""); 
if (WO1F) {
if (h >= 0 && h < 12)   {PReturn = "        +++  Guten Morgen "+WatchOwner1FirstName+"   Schönen Tag noch  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Schönen Tag noch  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = "        +++  Guten Abend "+WatchOwner1FirstName+"   bis bald  +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Gute Nacht "+WatchOwner1FirstName+" +++           ";}} else {
if (h >= 0 && h < 12)   {PReturn = F("        +++  Guten Morgen ...   Schönen Tag noch  +++           ");} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Schönen Tag noch  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = F("        +++  Guten Abend ...   bis bald  +++           ");} 
if (h >= 21 && h <= 23) {PReturn = F("        +++  Gute Nacht  +++           ");}} 
return cleanText(PReturn);}

// *************************************************************************************************

bool IsFederalStateOK(String sfs) {
bool IFSO = true; 
if (sfs != "B" && sfs != "K" && sfs != "ST" && sfs != "N" && sfs != "O" &&
    sfs != "S" && sfs != "T" && sfs != "V" && sfs != "W" && sfs != "-" && sfs != "") IFSO = false; 
return IFSO;    
}

// *************************************************************************************************

String LongFederalState(String sfs) {
String lfs = F("k.A.");  
if (sfs == "-" || sfs == "") lfs = F("kein Bundesland");
if (sfs == "B") lfs = F("Burgenland"); 
if (sfs == "K") lfs = F("Kärnten");  
if (sfs == "ST") lfs = F("Steiermark");  
if (sfs == "N") lfs = F("Niederösterreich");  
if (sfs == "O") lfs = F("Oberösterreich");  
if (sfs == "S") lfs = F("Salzburg");  
if (sfs == "T") lfs = F("Tirol");  
if (sfs == "V") lfs = F("Vorarlberg");  
if (sfs == "W") lfs = F("Wien");  
if (sfs == "k.A.") lfs = F("keine Angabe");  
return lfs;
}

// *************************************************************************************************

int freeRam() {
 extern int __heap_start, *__brkval;
 int v;
 return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// ********************************************************************************************************************

void StatusWarningLED(String LEDStatus) {
if (HWLEDPin != -1) {
if (LEDStatus == "CHECK") {digitalWrite(HWLEDPin, HIGH); MyWaitLoop(150); digitalWrite(HWLEDPin, LOW);} else {
if (LEDStatus == "HIGH") {if (Display) {digitalWrite(HWLEDPin, HIGH);}}
if (LEDStatus == "LOW") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX" && GMCMapValue != "NoGMCMapValue" && GMCMapToHigh == "") {  
digitalWrite(HWLEDPin, LOW);}}}}}

// ********************************************************************************************************************

String WiFiSignalStrength(bool MStart) {
long WifiRSSI = WiFi.RSSI(); // WiFi Signalstärke in dBm
String WiFiSignalQuality = F(""); 
if (SerialMonitor) {Serial.println("WiFi Signalstärke: "+ String(WifiRSSI) +" dBm");}
if (WifiRSSI >= -55) {
if (MStart) {WiFiSignalQuality = F("sehr gut");} else {  
WiFiSignalQuality = "<h7>WiFi Signalstärke: &nbsp;&nbsp; sehr gut ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -55 && WifiRSSI >= -65) {
if (MStart) {WiFiSignalQuality = F("gut");} else {   
WiFiSignalQuality = "<h7>WiFi Signalstärke: &nbsp;&nbsp; gut ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -65 && WifiRSSI >= -80) {
if (MStart) {WiFiSignalQuality = F("ausreichend");} else {   
WiFiSignalQuality = "<h7>WiFi Signalstärke: &nbsp;&nbsp; ausreichend ("+String(WifiRSSI)+" dBm)</h7>";}}
if (WifiRSSI < -80 && WifiRSSI >= -85) {
if (MStart) {WiFiSignalQuality = F("schlecht");} else {
WiFiSignalQuality = "<h10>WiFi Signalstärke: &nbsp;&nbsp; schlecht ("+String(WifiRSSI)+" dBm)</h10>";}}  
if (WifiRSSI < -85) {
 if (MStart) {WiFiSignalQuality = F("sehr schlecht");} else {
WiFiSignalQuality = "<h10>WiFi Signalstärke: &nbsp;&nbsp; sehr schlecht ("+String(WifiRSSI)+" dBm)</h10>";}} 
return WiFiSignalQuality;}

// *************************************************************************************************

void LoadClockStatus() {   
if (LittleFS.exists("/clockstatus.txt")){
ClockStatus = SetupLoad("clockstatus.txt").c_str();
SetupSave("clockstatus.txt", "OK");
} else {ClockStatus = "OK"; SetupSave("clockstatus.txt", ClockStatus);}}

// *************************************************************************************************

void SaveClockStatus(String StatusText) {
SetupSave("clockstatus.txt", StatusText);}

// *************************************************************************************************

void FlashNoWLANDots() {  
if (APStart == 0) {  
clr();
refreshAll();
printCharWithShiftVO('.',500);
MyWaitLoop(500);
clr();
refreshAll();} else {printCharWithShift('.',5); MyWaitLoop(500);}}

// *************************************************************************************************

void ValentinesDay() {
String VDHart = F("");
for (int i=0; i <= 3; i++){ 
VDHart += String(Hart2);   
printStringWithShift(VDHart.c_str(), ScrollTextTime+5);
MyWaitLoop(2000);
printStringWithShift("             ", ScrollTextTime+5);}}

// *************************************************************************************************

String HTMLStringPart1() {
String HTMLString;  
HTMLString = F("<!doctype html><html lang='de-AT'>");
HTMLString += F("<head><meta charset='utf-8'>");
HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<style>");
HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
return HTMLString;}

// *************************************************************************************************