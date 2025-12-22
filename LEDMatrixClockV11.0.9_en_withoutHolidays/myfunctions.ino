// **************************** My Functions ***************************************************
// *************************************************************************************************
void Greetings(bool GHStart) {               // uae = ä Sign 
getTimeLocal(); // get current time          // uoe = ö Sign 
GreetingsToday = false;                      // uue = ü Sign                                         
GT.clear();                                  // deg = ° Grad Sign
String GreetingText;                         // WatchOwner Only
                                             // uss = ß Sign 
WatchOwner1FirstName.trim(); WatchOwner1LastName.trim(); Gender1.trim(); Birthday1.trim();
WatchOwner2FirstName.trim(); WatchOwner2LastName.trim(); Gender2.trim(); Birthday2.trim();
WatchOwner3FirstName.trim(); WatchOwner3LastName.trim(); Gender3.trim(); Birthday3.trim();
// ***************************** "Birthdays or other Greetings" ********************************* 
// 1. WatchOwner 
if (WatchOwner1FirstName != "" && Gender1 != "" && Birthday1 != "") { // Only Birthdays of the Owners of this Watch
if  ((String(d) + "." + String(mo)) == Birthday1) {  
if (Gender1 == "male") {  // Birthday Greeting to main Owner male
GreetingText = "Happy Birthday dear "+WatchOwner1FirstName;} 
if (Gender1 == "female") { // Birthday Greeting to main Owner female
GreetingText = "Happy Birthday, darling "+WatchOwner1FirstName;}
if (Gender1 == "male" || Gender1 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {DEBUG_PRINTLN("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}
// 2. WatchOwner (Optional)
if (WatchOwner2FirstName != "" && Gender2 != "" && Birthday2 != "") {
if  ((String(d) + "." + String(mo)) == Birthday2) {  
if (Gender2 == "male") { // Birthday greeting to the 2nd male Owner
GreetingText = "Happy Birthday dear "+WatchOwner2FirstName;} 
if (Gender2 == "female") { // Birthday greeting to the 2nd female Owner
GreetingText = "Happy Birthday, darling "+WatchOwner2FirstName;}
if (Gender2 == "male" || Gender2 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {DEBUG_PRINTLN("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}
// 3. WatchOwner (Optional)
if (WatchOwner3FirstName != "" && Gender3 != "" && Birthday3 != "") {
if  ((String(d) + "." + String(mo)) == Birthday3) {  
if (Gender3 == "male") { // Birthday greeting to the 3nd male Owner
GreetingText = "Happy Birthday dear "+WatchOwner3FirstName;} 
if (Gender3 == "female") { // Birthday greeting to the 3nd female Owner
GreetingText = "Happy Birthday, darling "+WatchOwner3FirstName;}
if (Gender3 == "male" || Gender3 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {DEBUG_PRINTLN("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}} // Output GreetingText
} // End function "void Greetings()"

// *************************************************************************************************

void InitSerial(int Baud, byte Wait) { 
Serial.begin(Baud);  
while (!Serial) {DEBUG_PRINT(F("*"));}
printStringWithShift("COM !  ",38); // COM INIT
for (byte i = 0; i < Wait; i++) {
DEBUG_PRINT(F("*")); MyWaitLoop(100);}
printStringWithShift("           ",38); 
/*for(byte i=0; i<10; i++) { 
DEBUG_PRINTLN(F("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"));}*/}

// *************************************************************************************************

void ClockScrollIn() {
String TimeString; 
sendCmdAll(CMD_SHUTDOWN,0); // Display off
for (byte i=0; i <= 50; i++){ 
MyWaitLoop(10);     
HandleOTAandWebServer();  // OTA and Web Server Query  
getAnimClock(false);
if (s == 59 && m < 59) {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m+1);} else {TimeString = "  "+String(h)+String(m+1);}
} else {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m);} else {TimeString = "  "+String(h)+String(m);}}}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display on 
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);}

// *************************************************************************************************

void ShowGhost() {
String GShow; 
// Signs and Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Sign
if (SerialMonitor) {DEBUG_PRINTLN(F("Witching Hour ....."));} 
if (Display) {
sendCmdAll(CMD_SHUTDOWN,0); // Matrix switch off
printStringWithShift3("         ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix switch on
GShow = String(ghost)+"  "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(5000); // Short wait
HandleOTAandWebServer();  // OTA and Web Server Query 
printStringWithShift3("      ",ScrollTextTime+10);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Short wait
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
printStringWithShift3("      ",ScrollTextTime+5);
MyWaitLoop(100); // Short wait
ClockScrollIn();
} else {
printStringWithShift3("      ",ScrollTextTime-5);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix switch on
GShow = String(ghost)+"  "+String(ghost);
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Short wait
HandleOTAandWebServer();  // OTA and Web Server Query 
printStringWithShift3("      ",ScrollTextTime+5);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Short wait
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
HandleOTAandWebServer();  // OTA and Web Server Query 
printStringWithShift3("      ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,0);}} // Matrix switch off

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Load settings .... ***
swait = true; // during Load no Website Handle - on
bool MDTime; // Auxiliary variable
// Activate WEMOS D1 Little file system
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit == true) {
if (SerialMonitor) {DEBUG_PRINTLN(F("Load Clock Adjustments ..."));}
  // Load WiFi Access Data
  if (LittleFS.exists(F("/ssid.txt"))){
  ssid = SetupLoad(F("ssid.txt")).c_str();
  } else {ssid.clear();}
  if (LittleFS.exists(F("/ssid-passwd.txt"))){
  password = SetupLoad(F("ssid-passwd.txt")).c_str();
  } else {password.clear();}

  // Load Time Zone
  if (LittleFS.exists(F("/clocktimezone.txt"))){
  MCTZ = SetupLoad(F("clocktimezone.txt")).c_str();}

  // Load NTP-Server
  if (LittleFS.exists(F("/ntpservername.txt"))){
  NTPSN = SetupLoad(F("ntpservername.txt")).c_str();}
  if (NTPSN != "") {ntpServerName = NTPSN.c_str();} else {ntpServerName = "uk.pool.ntp.org";} // Default

  // Load the Owner of the Watch
 if (LittleFS.exists(F("/owner1-firstname.txt"))){
 WatchOwner1FirstName = SetupLoad(F("owner1-firstname.txt")).c_str();}
 if (LittleFS.exists(F("/owner1-lastname.txt"))){
 WatchOwner1LastName = SetupLoad(F("owner1-lastname.txt")).c_str();} 
 if (LittleFS.exists(F("/owner1-gender.txt"))){
 Gender1 = SetupLoad(F("owner1-gender.txt")).c_str();} 
 if (LittleFS.exists(F("/owner1-birthday.txt"))){
 Birthday1 = SetupLoad(F("owner1-birthday.txt")).c_str();} 
 if (LittleFS.exists(F("/owner2-firstname.txt"))){
 WatchOwner2FirstName = SetupLoad(F("owner2-firstname.txt")).c_str();} 
 if (LittleFS.exists(F("/owner2-lastname.txt"))){
 WatchOwner2LastName = SetupLoad(F("owner2-lastname.txt")).c_str();} 
 if (LittleFS.exists(F("/owner2-gender.txt"))){
 Gender2 = SetupLoad(F("owner2-gender.txt")).c_str();} 
 if (LittleFS.exists(F("/owner2-birthday.txt"))){
 Birthday2 = SetupLoad(F("owner2-birthday.txt")).c_str();} 
 if (LittleFS.exists(F("/owner3-firstname.txt"))){
 WatchOwner3FirstName = SetupLoad(F("owner3-firstname.txt")).c_str();} 
 if (LittleFS.exists(F("/owner3-lastname.txt"))){
 WatchOwner3LastName = SetupLoad(F("owner3-lastname.txt")).c_str();} 
 if (LittleFS.exists(F("/owner3-gender.txt"))){
 Gender3 = SetupLoad(F("owner3-gender.txt")).c_str();} 
 if (LittleFS.exists(F("/owner3-birthday.txt"))){
 Birthday3 = SetupLoad(F("owner3-birthday.txt")).c_str();} 

 if (LittleFS.exists(F("/midnightghost.txt"))){
 MidnightGhost = SetupLoad(F("midnightghost.txt")).c_str();}
 if (MidnightGhost != "YES" && MidnightGhost != "NO") {MidnightGhost = "YES";} 
 if (SerialMonitor) {
 DEBUG_PRINTLN(F(""));  
 DEBUG_PRINTLN(F("Show Midnight Ghosts ?: [YES / NO] ")+ MidnightGhost);
 DEBUG_PRINTLN(F(""));}

 // Selection DHT-Sensor [DHT11, DHT22 or NO]
 if (LittleFS.exists(F("/dhtsensor.txt"))){
 DHTSensor = SetupLoad(F("dhtsensor.txt")).c_str();} 
 if (DHTSensor != "DHT11" && DHTSensor != "DHT22" && DHTSensor != "NoDHT") {DHTSensor = "NoDHT";}
 if (SerialMonitor) {
 DEBUG_PRINTLN(F(""));  
 DEBUG_PRINTLN(F("DHT-Sensor Selection : [DHT11 / DHT22 / NO] ")+ DHTSensor);
 DEBUG_PRINTLN(F(""));}

  // *** Load OpenWeatherMap Access Data ***
  // OpenWeatherMap API-Key as String
  if (LittleFS.exists(F("/owm-apikey.txt"))){
  weatherKey = SetupLoad(F("owm-apikey.txt")).c_str();
  } else {weatherKey.clear();}
  // OpenWeatherMap City-ID as String
  if (LittleFS.exists(F("/owm-cityid.txt"))){
  cityID = SetupLoad(F("owm-cityid.txt")).c_str();
  } else {cityID.clear();} 
  // NewsData-Key
  if (LittleFS.exists(F("/newsdata-apikey.txt"))){
   newsKey = SetupLoad(F("newsdata-apikey.txt"));
   newsKey.trim();} else {newsKey.clear();} // https://NewsData.io
   // GMCMap-History ID / GMT Offset
  if (LittleFS.exists(F("/gmcmaphistoryid.txt"))){
  GMCMapHistoryID = SetupLoad(F("gmcmaphistoryid.txt"));
  GMCMapHistoryID.trim();} else {GMCMapHistoryID.clear();} // www.gmcmap.com 
  if (LittleFS.exists(F("/gmcmapgmtoffset.txt"))){
  GMCMapGMTOffset = SetupLoad(F("gmcmapgmtoffset.txt"));
  GMCMapGMTOffset.trim();} else {GMCMapGMTOffset.clear();} // www.gmcmap.com GMT Offset

   // Deactivate OpenWeatherMAP if no API Key or City ID is available
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // Deactivate NewsData if no API Key is available
   if (newsKey != "") {AccessNewsData = true;} else {
   AccessNewsData = false;}  
   //GMCMap Deactivate if no HistoryID is available
   if (GMCMapHistoryID != "" && GMCMapGMTOffset != "") {AccessGMCMap = true;} else {
   AccessGMCMap = false;}  

  // Weather Data function ON/OFF
  if (AccessOpenWeatherMap) {
  if (LittleFS.exists(F("/status_weatherfunctions.txt"))){
  String OWMF = SetupLoad(F("status_weatherfunctions.txt"));
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}

  // NewsData Function ON/OFF
  if (AccessNewsData) { 
  if (LittleFS.exists(F("/status_NewsDatafunctions.txt"))){
  String NAF = SetupLoad(F("status_NewsDatafunctions.txt"));
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsDataFunction = true; StatusNewsData = F("enabled");}
  if (NAF == "Off") {NewsDataFunction = false; StatusNewsData = F("disabled");}} else {
  NewsDataFunction = true; StatusNewsData = F("enabled");}} else {
  NewsDataFunction = false; StatusNewsData = F("disabled");}  

  // GMCMap function
  if (AccessGMCMap) { 
  if (LittleFS.exists(F("/status_GMCMapFunction.txt"))){
  String GMCF = SetupLoad(F("status_GMCMapFunction.txt"));
  GMCF.trim();
  if (GMCF == "" || GMCF == "On") {GMCMapFunction = true; StatusGMCMap = F("enabled");}
  if (GMCF == "Off") {GMCMapFunction = false; StatusGMCMap = F("disabled");}} else {
  GMCMapFunction = true; StatusGMCMap = F("enabled");}} else {
  GMCMapFunction = false; StatusGMCMap = F("disabled");} 
  
  // Load Pushover Data
  if (LittleFS.exists(F("/po-secureapptoken.txt"))){
  SecureAppToken = SetupLoad(F("po-secureapptoken.txt"));} else {SecureAppToken.clear();}
  SecureAppToken.trim(); // Remove spaces at the front and back
  if (LittleFS.exists(F("/po-usertoken.txt"))){
  UserToken = SetupLoad(F("po-usertoken.txt"));} else {UserToken.clear();}
  UserToken.trim(); // Remove spaces at the front and back
  if (LittleFS.exists(F("/po-device1.txt"))){
  PushoverDevice1 = SetupLoad(F("po-device1.txt"));} else {PushoverDevice1.clear();}
  PushoverDevice1.trim(); // Remove spaces at the front and back
  if (LittleFS.exists(F("/po-device2.txt"))){
  PushoverDevice2 = SetupLoad(F("po-device2.txt"));} else {PushoverDevice2.clear();}
  PushoverDevice2.trim(); // Remove spaces at the front and back
  if (LittleFS.exists(F("/po-device3.txt"))){
  PushoverDevice3 = SetupLoad(F("po-device3.txt"));} else {PushoverDevice3.clear();}
  PushoverDevice3.trim(); // Remove spaces at the front and back
  if (LittleFS.exists(F("/po-messagesound.txt"))){
  PushoverMessageSound = SetupLoad(F("po-messagesound.txt"));} else {PushoverMessageSound= F("none");}
  if (SecureAppToken != "" && UserToken != "" && PushoverDevice1 != "") {
  IsPushover = true;} else {IsPushover = false;}

  // Display switch-on time as a string
  if (LittleFS.exists(F("/display-on-time.txt"))){
  DisplayOnTime = SetupLoad(F("display-on-time.txt"));
  DisplayOnTime.trim(); // Remove spaces at the front and back
  } else {DisplayOnTime.clear();}
  // Display switch-off time as a string
  if (LittleFS.exists(F("/display-off-time.txt"))){
  DisplayOffTime = SetupLoad(F("display-off-time.txt"));
  DisplayOffTime.trim(); // Remove spaces at the front and back
  } else {DisplayOffTime.clear();} 

  if (SerialMonitor) {
  DEBUG_PRINTLN("");  
  DEBUG_PRINTLN("DisplayOnTime: "+DisplayOnTime);
  DEBUG_PRINTLN("DisplayOffTime: "+DisplayOffTime);
  DEBUG_PRINTLN("");}

  // DHT-Sensor Offset
  if (LittleFS.exists(F("/dht-intempoffset.txt"))){
  InTempOffset = SetupLoad(F("dht-intempoffset.txt"));
  InTempOffset.trim(); // Remove spaces at the front and back
  InTempOffsetInt = (int16_t)strtol(InTempOffset.c_str(), NULL, 10); // IntegerCheck was carried out in the Captive Portal
  } else {InTempOffset = F("0");
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists(F("/dht-inhumoffset.txt"))){
  InHumOffset = SetupLoad(F("dht-inhumoffset.txt"));
  InHumOffset.trim(); // Remove spaces at the front and back
  InHumOffsetInt = (int16_t)strtol(InHumOffset.c_str(), NULL, 10); // IntegerCheck was carried out in the Captive Portal
  } else {InHumOffset = F("0");
          InHumOffsetInt = 0;}  // Humidity

  // Is a passive buzzer activated ?
  if (LittleFS.exists(F("/buzzer-install.txt"))){
  if (SetupLoad(F("buzzer-install.txt")) == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Status Buzzermodi
  if (IsBuzzer) {
  if (LittleFS.exists(F("/status_startsound.txt"))){
  StartSound = SetupLoad(F("status_startsound.txt"));} else {StartSound = F("Off");}
  if (LittleFS.exists(F("/status_hoursound.txt"))){
  HourSound = SetupLoad(F("status_hoursound.txt"));} else {HourSound = F("Off");}}

  // Check the plausibility of the information for Matrix Display Automatic On/Off
  CheckLEDMatrixDisplayOnOff();

  // Brightness of the LED Matrix
  if (LittleFS.exists(F("/matrix_brightness.txt"))){
  Brightness = SetupLoad(F("matrix_brightness.txt"));
  if (Brightness == "" || Brightness == "L1") {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // if the File is empty or error --> Level 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Level 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Level 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Level 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // Level 5 - Level 1
  // Double Point status
  if (LittleFS.exists(F("/status_doublepoint.txt"))){
  String DPS = SetupLoad(F("status_doublepoint.txt"));
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // if the File is empty or error
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}} else {DoublePointStatus = 2;}
  // ScrollText Speed
  if (LittleFS.exists(F("/status_scrolltexttime.txt"))){
  String STT = SetupLoad(F("status_scrolltexttime.txt"));
  if (STT == "25") {ScrollTextTime = 25;} // Level 8
  if (STT == "30") {ScrollTextTime = 30;} // Level 7
  if (STT == "35") {ScrollTextTime = 35;} // Level 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Default = Level 5
  if (STT == "45") {ScrollTextTime = 45;} // Level 4
  if (STT == "50") {ScrollTextTime = 50;} // Level 3
  if (STT == "55") {ScrollTextTime = 55;} // Level 2
  if (STT == "60") {ScrollTextTime = 60;}} else {ScrollTextTime = 40;} // Level 1 - Level 5 
  
  // Additional functions ON/OFF
  if (LittleFS.exists(F("/status_enhancedfunctions.txt"))){
  String EF = SetupLoad(F("status_enhancedfunctions.txt"));
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}} else {EnhancedFunctions = true;}

  LoadClockStatus(); // Load the last Status of the Clock
} else
    {
     // Set default values ​​for "Little FS" error
    ssid.clear();
    password.clear(); 
    Brightness = F("L1");
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    DisplayOnTime = F("6:30");
    DisplayOffTime = F("23:15");
    EnhancedFunctions = true;
    WeatherFunctions = false;
    weatherKey.clear(); // API-Key from OpenWeatherMap - https://openweathermap.org
    cityID.clear(); // City ID from OpenWeatherMap
    newsKey.clear(); // API-Key from NewsData - https://NewsData.io
    GMCMapHistoryID.clear(); // www.GMCMap.com - History ID
    GMCMapGMTOffset.clear(); // www.GMCMap.com - GMTOffset
    InTempOffsetInt = -1; // Temperature Offset
    InHumOffsetInt = -1; // Humidity  Offset
    StartSound = F("Off"); // no Buzzer Sound
    HourSound = F("Off"); // no Buzzer Sound
    DHTSensor = F("DHT11"); // Default DHT-Sensor
    MCTZ = F("London, Belfast, Dublin"); // Default Time Zone
    ntpServerName = "uk.pool.ntp.org";  // Find local Server under http://www.pool.ntp.org/zone
    }
 swait = false;} // during Load no Website Handle - off

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Original saved time
  FDOffTime = DisplayOffTime; // Original saved time

   // Check display on/off time for plausibility
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime.clear();}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime.clear();}
  if (DisplayOnTime != "") {
  if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime.clear();};
  if (DisplayOnTime != "") {
  if (DisplayOnTime.substring(0,1) == "0") {DisplayOnTime.remove(0, 1);}
  int16_t DP = DisplayOnTime.indexOf(':')+1;
  if (DisplayOnTime.substring(DP,DP+1) == "0") {DisplayOnTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime.clear();}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime.clear();}
if (DisplayOnTime != "") {
MDTime = false;
int16_t DP = DisplayOnTime.indexOf(':'); 
String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (byte i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime.clear();} else {   
MDTime = false;    
for (byte i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime.clear();}}}}}

  if (DisplayOffTime != "") {
  if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime.clear();};
  if (DisplayOffTime != "") {
  if (DisplayOffTime.substring(0,1) == "0") {DisplayOffTime.remove(0, 1);}
  int16_t DP = DisplayOffTime.indexOf(':')+1;
  if (DisplayOffTime.substring(DP,DP+1) == "0") {DisplayOffTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime.clear();}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime.clear();}
if (DisplayOffTime != "") {
MDTime = false;
int16_t DP = DisplayOffTime.indexOf(':'); 
String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
String CheckMDMinOff=  DisplayOffTime.substring(DP+1,6);
CheckMDMinOff.trim(); 

// Process inputs for Matrix On/Off
for (byte i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");} else {MDTime = false;    
for (byte i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");}}}}}}} // SSID - Ende  

// *************************************************************************************************

void SaveMatrixAdjustments(){
  swait = true;  // while Save no Website Handle - on
  if (littleFSInit == true) {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Save Clock Adjustments ..."));}    
  // Trim
  TempSSID.trim(); // SSID WLAN
  newsKey.trim(); // NewsData API-Key
  DisplayOnTime.trim(); // Matrix display switch-on time
  DisplayOffTime.trim(); // Matrix display switch-off time
  InTempOffset.trim(); // DHT-Sensor Offset (Temperature)
  InHumOffset.trim(); // DHT-Sensor Offset (Air Humidity)
  WatchOwner1FirstName.trim(); // 1st Owner - First Name
  WatchOwner1LastName.trim(); // 1st Owner - Last Name
  Gender1.trim(); // 1st Owner - Gender
  Birthday1.trim(); // 1st Owner - Date of Birth
  WatchOwner2FirstName.trim(); //  2nd Owner - First Name
  WatchOwner2LastName.trim(); // 2nd Owner - Last Name
  Gender2.trim(); // 2nd Owner - Gender
  Birthday2.trim(); // 2nd Owner - Date of Birth
  WatchOwner3FirstName.trim(); // 3rd Owner - First Name
  WatchOwner3LastName.trim(); // 3rd Owner - Last Name
  Gender3.trim(); // // 3rd Owner - Gender
  Birthday3.trim(); // 3rd Owner - Date of Birth
  
  // SSID Save
  SetupSave(F("ssid.txt"), TempSSID); // SSID WLAN
  SetupSave(F("ssid-passwd.txt"), password); // Password WLAN

  // Timezone save
  SetupSave(F("clocktimezone.txt"), MCTZ);
   // NTP-Server save
  SetupSave(F("ntpservername.txt"), NTPSN);

  // Save Owner of the Watch
  if (WatchOwner1FirstName != "") {
  SetupSave(F("owner1-firstname.txt"), WatchOwner1FirstName);} else {
  if (LittleFS.exists(F("/owner1-firstname.txt"))){LittleFS.remove(F("/owner1-firstname.txt"));}}  
  if (WatchOwner1LastName != "") {
  SetupSave(F("owner1-lastname.txt"), WatchOwner1LastName);} else {
  if (LittleFS.exists(F("/owner1-lastname.txt"))){LittleFS.remove(F("/owner1-lastname.txt"));}}  
  if (Gender1 != "") {
  SetupSave(F("owner1-gender.txt"), Gender1);} else {
  if (LittleFS.exists(F("/owner1-gender.txt"))){LittleFS.remove(F("/owner1-gender.txt"));}}  
  if (Birthday1 != "") {
  SetupSave(F("owner1-birthday.txt"), Birthday1);} else {
  if (LittleFS.exists(F("/owner1-birthday.txt"))){LittleFS.remove(F("/owner1-birthday.txt"));}}  
  if (WatchOwner2FirstName != "") {
  SetupSave(F("owner2-firstname.txt"), WatchOwner2FirstName);} else {
  if (LittleFS.exists(F("/owner2-firstname.txt"))){LittleFS.remove(F("/owner2-firstname.txt"));}}  
  if (WatchOwner2LastName != "") {
  SetupSave(F("owner2-lastname.txt"), WatchOwner2LastName);} else {
  if (LittleFS.exists(F("/owner2-lastname.txt"))){LittleFS.remove(F("/owner2-lastname.txt"));}}  
  if (Gender2 != "") {
  SetupSave(F("owner2-gender.txt"), Gender2);} else {
  if (LittleFS.exists(F("/owner2-gender.txt"))){LittleFS.remove(F("/owner2-gender.txt"));}}  
  if (Birthday2 != "") {
  SetupSave(F("owner2-birthday.txt"), Birthday2);} else {
  if (LittleFS.exists(F("/owner2-birthday.txt"))){LittleFS.remove(F("/owner2-birthday.txt"));}}  
  if (WatchOwner3FirstName != "") {
  SetupSave(F("owner3-firstname.txt"), WatchOwner3FirstName);} else {
  if (LittleFS.exists(F("/owner3-firstname.txt"))){LittleFS.remove(F("/owner3-firstname.txt"));}}  
  if (WatchOwner3LastName != "") {
  SetupSave(F("owner3-lastname.txt"), WatchOwner3LastName);} else {
  if (LittleFS.exists(F("/owner3-lastname.txt"))){LittleFS.remove(F("/owner3-lastname.txt"));}}  
  if (Gender3 != "") {
  SetupSave(F("owner3-gender.txt"), Gender1);} else {
  if (LittleFS.exists(F("/owner3-gender.txt"))){LittleFS.remove(F("/owner3-gender.txt"));}}  
  if (Birthday3 != "") {
  SetupSave(F("owner3-birthday.txt"), Birthday3);} else {
  if (LittleFS.exists(F("/owner3-birthday.txt"))){LittleFS.remove(F("/owner3-birthday.txt"));}}

  // View Midnight Ghost ? (Yes or No)
  SetupSave(F("midnightghost.txt"), MidnightGhost);
  
  // Save DHT Sensor Selection [DHT11 or DHT22]
  SetupSave(F("dhtsensor.txt"), DHTSensor); 
  
  // OpenWeatherMap API-Key und OpenWeatherMap City-ID
  if (weatherKey != "" && cityID != "") {
  SetupSave(F("owm-apikey.txt"), weatherKey);
  SetupSave(F("owm-cityid.txt"), cityID);} else {
  if (LittleFS.exists(F("/owm-apikey.txt"))){LittleFS.remove(F("/owm-apikey.txt"));}
  if (LittleFS.exists(F("/owm-cityid.txt"))){LittleFS.remove(F("/owm-cityid.txt"));}} 
  // NewsData API-Key
  if (newsKey != "") {
  SetupSave(F("newsdata-apikey.txt"), newsKey);} else {
  if (LittleFS.exists(F("/newsdata-apikey.txt"))){LittleFS.remove(F("/newsdata-apikey.txt"));}}
  // GMCMap Data 
  if (GMCMapHistoryID != "") {
  SetupSave(F("gmcmaphistoryid.txt"), GMCMapHistoryID);} else {
  if (LittleFS.exists(F("/gmcmaphistoryid.txt"))){LittleFS.remove(F("/gmcmaphistoryid.txt"));}}
  if (GMCMapGMTOffset != "") {
  SetupSave(F("gmcmapgmtoffset.txt"), GMCMapGMTOffset);} else {
  if (LittleFS.exists(F("/gmcmapgmtoffset.txt"))){LittleFS.remove(F("/gmcmapgmtoffset.txt"));}}
  // Matrix display switch-on time
  if (DisplayOnTime != ""){
  SetupSave(F("display-on-time.txt"), DisplayOnTime);} else {
  if (LittleFS.exists(F("/display-on-time.txt"))){LittleFS.remove(F("/display-on-time.txt"));}}
  // Matrix display switch-off time
  if (DisplayOffTime != ""){
  SetupSave(F("display-off-time.txt"), DisplayOffTime);} else {
  if (LittleFS.exists(F("/display-off-time.txt"))){LittleFS.remove(F("/display-off-time.txt"));}}
  // Is a passive Buzzer activated ?
  if (IsBuzzer){
  SetupSave(F("buzzer-install.txt"), "True");} else {
  if (LittleFS.exists(F("/buzzer-install.txt"))){LittleFS.remove(F("/buzzer-install.txt"));}}

  // *** Save Pushover Data ***
  // Pushover SecureAppToken
  if (SecureAppToken == "") {
  if (LittleFS.exists(F("/po-secureapptoken.txt"))){LittleFS.remove(F("/po-secureapptoken.txt"));}  
  } else {SetupSave(F("po-secureapptoken.txt"), SecureAppToken);} 
  // Pushover UserToken
  if (UserToken == "") {
  if (LittleFS.exists(F("/po-usertoken.txt"))){LittleFS.remove(F("/po-usertoken.txt"));} 
  } else {SetupSave(F("po-usertoken.txt"), UserToken);}
  // Pushover Devicename 1
  if (PushoverDevice1 == "") {
  if (LittleFS.exists(F("/po-device1.txt"))){LittleFS.remove(F("/po-device1.txt"));} 
  } else {SetupSave(F("po-device1.txt"), PushoverDevice1);}
  // Pushover Devicename 2
  if (PushoverDevice2 == "") {
  if (LittleFS.exists(F("/po-device2.txt"))){LittleFS.remove(F("/po-device2.txt"));} 
  } else {SetupSave(F("po-device2.txt"), PushoverDevice2);}
  // Pushover Devicename 3
  if (PushoverDevice3 == "") {
  if (LittleFS.exists(F("/po-device3.txt"))){LittleFS.remove(F("/po-device3.txt"));} 
  } else {SetupSave(F("po-device3.txt"), PushoverDevice3);}
  // Pushover Message Sound
  if (PushoverMessageSound == "none" || PushoverMessageSound == "") {
  if (LittleFS.exists(F("/po-messagesound.txt"))){LittleFS.remove(F("/po-messagesound.txt"));} 
  } else {SetupSave(F("po-messagesound.txt"), PushoverMessageSound);}

  // Check DHT Offset
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = F("0");} // Temperature
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = F("0");} // Air Humidity
  
  // DHT-Sensor Offset
  if (InTempOffset != ""){
  SetupSave(F("dht-intempoffset.txt"), InTempOffset);} else {
  if (LittleFS.exists(F("/dht-intempoffset.txt"))){LittleFS.remove(F("/dht-intempoffset.txt"));}}
  if (InHumOffset != ""){
  SetupSave(F("dht-inhumoffset.txt"), InHumOffset);} else {
  if (LittleFS.exists(F("/dht-inhumoffset.txt"))){LittleFS.remove(F("/dht-inhumoffset.txt"));}}}
 swait = false;}  // while Save no Website Handle - off

// *************************************************************************************************

// Load and save setup settings
String SetupLoad(String file_name) {
swait = true;  
String result; // init
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
  int16_t bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
  this_file.close();
  MyWaitLoop(10); //wait
  swait = false;
  return true;}

// *************************************************************************************************

// Switch Display on and off using a Button
void MatrixButtonOnOff(bool Start){
if(digitalRead(PushButtonPin) == LOW){
if (Start) {APStart += 1; // inc   
if (SerialMonitor) {DEBUG_PRINTLN("Reset Counter: "+ String(APStart));}} else {
Display = !Display;
if (!Display){ 
MyWaitLoop(500); // wait     
sendCmdAll(CMD_SHUTDOWN,0); // Display Off
if (SerialMonitor) {DEBUG_PRINTLN("Matrix switched off via Button at "+ FormatedDateTimeEN(false));}
APStart += 1; // inc   
} else { 
    MyWaitLoop(500); // wait     
    getTimeLocal(); // Get current Time 
    sendCmdAll(CMD_SHUTDOWN, 1); // Display On
    if (IsBuzzer) {ButtonDisplayOnBeep();}
    if (SerialMonitor) {DEBUG_PRINTLN("Matrix switched on via Button at "+ FormatedDateTimeEN(false));}
    APStart += 1;}} // inc 
    if (APStart >= 10 && littleFSInit) {ResetClock();} // Clock Reset 
    delay(250);} else {if (APStart > 0) {APStart = 0;}}} // wait

// *************************************************************************************************

String FormatedDateTimeEN(bool FDTHTML) {
String DateTimeString;
getTimeLocal(); // get current DateTime
// Time
if (h < 10){DateTimeString = "0"+String(h);} else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (FDTHTML) {DateTimeString += " o'clock  &nbsp;-/-&nbsp;  ";} else {DateTimeString += " o'clock -/- ";} // Time
// Date
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += "/0"+String(mo)+"/"+String(ye);} else {DateTimeString += "/"+String(mo)+"/"+String(ye);}
return DateTimeString;}

// *************************************************************************************************

// Arduino OTA query only every 0.8 seconds and web server query only every 0.8 seconds.
void HandleOTAandWebServer(){ 
if (PushOverTest) {WSR = 20000;} else {WSR = 800;}   // Webserver Refreshrate | default = 800
if (millis() - hwsTime > WSR) { 
hwsTime = millis(); 
if (!swait) {
server.handleClient();}} else {
ArduinoOTA.handle();}} // OTA Query

// *************************************************************************************************

void ClearMatrix() {
for (byte i=0; i <= 50; i++){ 
MyWaitLoop(10);     
HandleOTAandWebServer();  // OTA und Webserver Request 
getAnimClock(false);} // Show / Calculate Time
printStringWithShift("           ",1);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix ON
}

// *************************************************************************************************

void WaitForClockStartWIFI(byte WaitLoop) {
for (byte i = 0; i < WaitLoop; i++) { 
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

// Establish a WiFi connection
void wificonnect(bool Start) {
WiFiConnectLoop = 0; // Variable Reset   
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { 
if (Start) {MyWaitLoop(1000);} else {MyWaitLoop(250);}
if (SerialMonitor) {DEBUG_PRINT(".");} 
if (!Start && StartStopServers > 0) {
if (WiFiConnectLoop == 0) {await = true;} MatrixButtonOnOff(true); FlashNoWLANDots();} 
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}}
if (WiFi.status() != WL_CONNECTED) {ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
if (Start) {if (SerialMonitor) {DEBUG_PRINTLN(F("WLAN-Connection failed ..."));}} else {
if (SerialMonitor) {DEBUG_PRINTLN(F("WLAN-Connection disconnected ..."));}}
} else {
ClockWiFiConnectFail = false;
APStart = 0; // "Access Point" Button Counter Reset
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {DEBUG_PRINT("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop));} // Time until connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINT(F("Connected to IP-Address: ")); 
DEBUG_PRINTLN(WiFi.localIP());}}}

// *************************************************************************************************

void WiFiUDPStart() { 
Udp.begin(localPort);
if (SerialMonitor) {
DEBUG_PRINT(F("local Port: "));
DEBUG_PRINTLN(Udp.localPort());
DEBUG_PRINTLN(F("NTP-Time - Waiting for synchronization"));}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Number of seconds between resynchronizations. 86400 = 1 day
// setSyncInterval(60); // Test
if (SerialMonitor) {DEBUG_PRINTLN("UDP Server started - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WiFiUDPStop() {
Udp.stop();
if (SerialMonitor) {DEBUG_PRINTLN("UDP Server stopped - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WebServerStart() {
server.begin();
if (SerialMonitor) {DEBUG_PRINTLN("Web Server started - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WebServerStop() {
server.stop();
if (SerialMonitor) {DEBUG_PRINTLN("Web Server stopped - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void MyWaitLoop(int16_t wlt){// Pause Loop
waitloop = 0; // Variable reset
while (waitloop < wlt) { // Pause in milliseconds{ 
waitloop += 1; // inc waitloop 1
delay(1);
HandleOTAandWebServer();  // OTA and Web Server query
yield();}} 

// *************************************************************************************************

// check a string to see if it is numeric and accept Decimal point and positiv negativ Value
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
  sprintf(buf, "%ld %s %02ld:%02ld:%02ld", secs / 86400, (secs < 86400 || secs > 172800) ? "Days" : "Day", secs / 3600 % 24, secs / 60 % 60, secs % 60);
  return buf;}

// ************************************************************************************************

void ResetClock() {
APStart = 0; // Reset
swait = true; // no more Website Handling
bool IsFileExist = false; // Reset
StatusWarningLED("LOW"); // LED off  
if (LittleFS.exists("/ssid.txt")){  
LittleFS.remove("/ssid.txt");}
if (LittleFS.exists("/ssid-passwd.txt")){  
LittleFS.remove("/ssid-passwd.txt");}
MyWaitLoop(500); // 0,5 sek. wait 
if (SerialMonitor) {DEBUG_PRINTLN(F("Reset - The Clock will Restart"));}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
printStringWithShift("        +++     Reset - The Clock will Restart      +++           ",ScrollTextTime);
ESP.restart();}

// ************************************************************************************************

int16_t FormatedDisplayOffTime() {
String fTime;
fTime = FDOffTime;
if (fTime != "") {
int16_t DP = fTime.indexOf(':'); 
fTime.remove(DP, 1);
} else {fTime = F("0");} 
return (int16_t)strtol(fTime.c_str(), NULL, 10);}

// *************************************************************************************************

bool GoMatrixAction() { 
bool LMOTH; 
if (NewsDataFunction || EnhancedFunctions || WeatherFunctions || GMCMapFunction) {
bool DOffMinIntOffset;  
String TempDOffHour, TempDOffMin;
String TempActualTimeHour, TempActualTimeMin;              
if (DisplayOffTime != "") { 
int16_t DP = DisplayOffTime.indexOf(':'); 
String DOffHour = DisplayOffTime.substring(0,DP); 
String DOffMin=  DisplayOffTime.substring(DP+1,6);
DOffHour.trim(); 
DOffMin.trim(); 
int16_t DOffHourInt = (int16_t)strtol(DOffHour.c_str(), NULL, 10);
int16_t DOffMinInt = (int16_t)strtol(DOffMin.c_str(), NULL, 10);
DOffMinInt -= 10; // Minus 10 Minutes
if (DOffMinInt <= 0) {
DOffMinInt = 50 + (int16_t)strtol(DOffMin.c_str(), NULL, 10);
DOffMinIntOffset = true;
DOffHourInt -= 1;
if (DOffHourInt < 0) {DOffHourInt = 23;}} else {DOffMinIntOffset = false;} 

// Display Off Time -  Minus 10 Minutes
if (DOffHourInt < 10) {TempDOffHour = "0"+String(DOffHourInt);} else {TempDOffHour = String(DOffHourInt);}
if (DOffMinInt < 10)  {TempDOffMin = "0"+String(DOffMinInt);} else {TempDOffMin = String(DOffMinInt);}
String ActionTime = TempDOffHour+TempDOffMin;  
int16_t ActionTimeInt = (int16_t)strtol(ActionTime.c_str(), NULL, 10);

byte ActualHour = h; // Current Hour
byte ActualMin = m;  // Current Minutes
// Current Time as a String
if (ActualHour < 10) {TempActualTimeHour = "0"+String(ActualHour);} else 
                     {TempActualTimeHour = String(ActualHour);}
if (ActualMin < 10) {TempActualTimeMin = "0"+String(ActualMin);} else 
                    {TempActualTimeMin = String(ActualMin);}
String ActualTime = TempActualTimeHour + TempActualTimeMin;
int16_t ActualTimeInt = (int16_t)strtol(ActualTime.c_str(), NULL, 10);

int16_t TimeDifference = ActualTimeInt - ActionTimeInt;

if (DOffMinIntOffset) {
if (TimeDifference >= 0 && TimeDifference <= 50) {LMOTH = false;} else {LMOTH = true;}} else {
if (TimeDifference >= 0 && TimeDifference <= 10) {LMOTH = false;} else {LMOTH = true;}}
} else {LMOTH = true;}} else {LMOTH = true;}
if (SerialMonitor) {
if (LMOTH) {DEBUG_PRINTLN("GoMatrixAction: True");} else {DEBUG_PRINTLN("GoMatrixAction: False");}}
return LMOTH;}

// *************************************************************************************************

void SendHTMLString(String SHTMLString) { 
server.sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
server.sendHeader(F("Pragma"), F("no-cache"));
server.sendHeader(F("Expires"), F("-1"));  
server.sendHeader(F("Connection"), F("close")); 
server.send(200, F("text/html"), SHTMLString);
SHTMLString.clear();} // String clear

// *************************************************************************************************

String Politeness(bool WO1F) {
String PReturn;  
if (WO1F) {
if (h >= 0 && h < 12)   {PReturn = "        +++  Good morning "+WatchOwner1FirstName+"   Have a nice day  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Have a nice day  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = "        +++  Good evening "+WatchOwner1FirstName+"   see you soon  +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Good night "+WatchOwner1FirstName+" +++           ";}} else {
if (h >= 0 && h < 12)   {PReturn = F("        +++  Good morning... Have a nice day  +++           ");} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Have a nice day  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = F("        +++  Good evening ...   see you soon  +++           ");} 
if (h >= 21 && h <= 23) {PReturn = F("        +++  Good night  +++           ");}} 
return PReturn;}

// *************************************************************************************************

int freeRam() {
 extern int __heap_start, *__brkval;
 int v;
 return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// *******************************************************************************************************************

void StatusWarningLED(String LEDStatus) {
if (HWLEDPin != -1) {
if (LEDStatus == "CHECK") {digitalWrite(HWLEDPin, HIGH); MyWaitLoop(150); digitalWrite(HWLEDPin, LOW);} else {
if (LEDStatus == "HIGH") {if (Display) {digitalWrite(HWLEDPin, HIGH);}}
if (LEDStatus == "LOW") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX" && GMCMapValue != "NoGMCMapValue" && GMCMapToHigh == "") {  
digitalWrite(HWLEDPin, LOW);}}}}}

// *******************************************************************************************************************

String WiFiSignalStrength(bool MStart) {
int8_t WifiRSSI = WiFi.RSSI(); // WiFi Signal Strength in dBm
String WiFiSignalQuality; 
if (SerialMonitor) {DEBUG_PRINTLN("WiFi Signal Strength: "+ String(WifiRSSI) +" dBm");}
if (WifiRSSI >= -55) {
if (MStart) {WiFiSignalQuality = F("very good");} else {  
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; very good ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -55 && WifiRSSI >= -65) {
if (MStart) {WiFiSignalQuality = F("good");} else {   
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; good ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -65 && WifiRSSI >= -80) {
if (MStart) {WiFiSignalQuality = F("sufficient");} else {   
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; sufficient ("+String(WifiRSSI)+" dBm)</h7>";}}
if (WifiRSSI < -80 && WifiRSSI >= -85) {
if (MStart) {WiFiSignalQuality = F("bad");} else {
WiFiSignalQuality = "<h10>WiFi Signal Strength: &nbsp;&nbsp; bad ("+String(WifiRSSI)+" dBm)</h10>";}}  
if (WifiRSSI < -85) {
if (MStart) {WiFiSignalQuality = F("very bad");} else {
WiFiSignalQuality = "<h10>WiFi Signal Strength: &nbsp;&nbsp; very bad ("+String(WifiRSSI)+" dBm)</h10>";}} 
return WiFiSignalQuality;}

// *************************************************************************************************

bool IsWiFiSignalOK() {  
String SHT = WiFiSignalStrength(true); // Read WiFi signal strength
if (SerialMonitor) {DEBUG_PRINTLN(F("")); DEBUG_PRINTLN("WiFi Signal Strength: ("+SHT+")");}
if (SHT != "bad" && SHT != "very bad") {return true;} else {return false;}}

// *************************************************************************************************

void LoadClockStatus() {   
if (LittleFS.exists("/clockstatus.txt")){
ClockStatus = SetupLoad("clockstatus.txt").c_str();
SetupSave(F("clockstatus.txt"), "OK");
} else {ClockStatus = "OK"; SetupSave(F("clockstatus.txt"), ClockStatus);}}

// *************************************************************************************************

void SaveClockStatus(String StatusText) {
SetupSave(F("clockstatus.txt"), StatusText);}

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
String VDHart;
for (byte i=0; i <= 3; i++){ 
VDHart += String(Hart2);   
printStringWithShift(VDHart.c_str(), ScrollTextTime+5);
MyWaitLoop(2000);
printStringWithShift("             ", ScrollTextTime+5);}}

// *************************************************************************************************

String SunBow(uint64_t Sun) {
time_t t = GetTimeZone(static_cast<time_t>(Sun));
tm *timeinfo = localtime(&t); // Local-Time incl. Timezone Offset
// tm *timeinfo = gmtime(&t); // Greenwich Mean Time
char buf[32];
/*snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);*/
snprintf(buf, sizeof(buf), "%02d:%02d",timeinfo->tm_hour, timeinfo->tm_min); // Format DateTime-String
return String(buf);}

// *************************************************************************************************

void StringMemoryLimitation() {
// String MemoryLimitation
weatherKey.reserve(35); // API-Key for OpenWeatherMap - https://openweathermap.org
cityID.reserve(10);
weatherKey = ""; 
cityID = ""; 
newsKey.reserve(45); // NewsData API-Key; - https://newsdata.io/api-key
newsKey = "";
DisplayOnTime.reserve(10);
DisplayOffTime.reserve(10);
DisplayOnTime = "";
DisplayOffTime = "";
FDOnTime.reserve(10);
FDOffTime.reserve(10);
FDOnTime = "";
FDOffTime = "";
StatusOpenWeatherMap.reserve(15);
StatusOpenWeatherMap = "";
StatusNewsData.reserve(15);
StatusNewsData = "";
InTempValue.reserve(10);
InHumValue.reserve(10);
InTempValue = "-"; // Indoor Temperature
InHumValue = ""; // Humidity
InTempOffset.reserve(10);
InHumOffset.reserve(10);
InTempOffset = "";   // for Captive Portal
InHumOffset = "";    // for Captive Portal 
InHumidityStatus.reserve(5);
InHumidityStatus = "";
Brightness.reserve(5);
Brightness = "L1";
StartSound.reserve(5);
HourSound.reserve(5);
StartSound = "";
HourSound = "";}

// *************************************************************************************************

  void CheckInHumidityStatus() {
  InHumidityStatus = CheckInHumidity(); // Measuring Indoor Air Humidity
  if (SerialMonitor) {DEBUG_PRINTLN("");
  DEBUG_PRINTLN("InHumidityStatus: "+InHumidityStatus); DEBUG_PRINTLN("");}
  if (InHumidityStatus != "FAIL") {
  if (IsPushover && (InHumidityStatus == "MIN" || InHumidityStatus == "MAX")) {MEP=true; PushOverAlert(InHumidityStatus); MEP=false;}  
  // Humidity too Low
  if (InHumidityStatus == "MIN") {SIC = true; // ScrollIn Clock    
  for (byte i=0; i <= 1; i++) { // Repeat Message twice
  printStringWithShift("        +++       Attention: The Humidity at the Location of the Clock is too low... !      +++           ",ScrollTextTime);
  MyWaitLoop(1500);}}
  if (InHumidityStatus == "MAX") {SIC = true; // ScrollIn Clock   
  // Humidity too high
  for (byte i=0; i <= 1; i++){ // Repeat Message twice
  printStringWithShift("        +++       Attention: The Humidity at the Location of the Clock is too high... !      +++           ",ScrollTextTime);
  MyWaitLoop(1500);}}
  if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") {SIC = true; // ScrollIn Clock  
  printStringWithShift("        +++       The Humidity at the Location of the Clock is good...      +++           ",ScrollTextTime);} 
  }}

// *************************************************************************************************

bool IsEnoughRAM(int16_t MinRAM, const char* Modul) {
ESPRAM = ESP.getFreeHeap(); // Check RAM
bool ByteRAM = false;
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN("ESPRAM in Byte: ("+String(Modul)+") "+String(ESPRAM)+" Byte");
if (ESPRAM > MinRAM) {DEBUG_PRINTLN("Sufficient RAM");} else {DEBUG_PRINTLN("Insufficient RAM");}}
if (ESPRAM > MinRAM) {ByteRAM = true;}
return ByteRAM;}

// *************************************************************************************************

void CheckGlobalVariableRange() {
int8_t IntGMCMMAD;  
// AccessPoint IP  
if (APIPA1 < 1 || APIPA1 > 254) {APIPA1 = 192;}
if (APIPA2 < 1 || APIPA2 > 254) {APIPA2 = 168;}
if (APIPA3 < 1 || APIPA3 > 254) {APIPA3 = 4;}
if (APIPA4 < 1 || APIPA4 > 254) {APIPA4 = 1;}
// ScrollTime
if (ScrollClockTime < 10 || ScrollClockTime > 254) {ScrollClockTime = 80;}
// Luftfeuchte / Humidity / Humidité
if (MinComfortHum < 0 || MinComfortHum > 100) {MinComfortHum = 32;}
if (MaxComfortHum < 0 || MaxComfortHum > 100) {MinComfortHum = 68;}
// Minimum RAM
if (MinRAMMem < 12288 || MinRAMMem > 19456) {MinRAMMem = 15784;}
// Check Minimum / Maximum "MaxWiFiConnectLoop"
if (MaxWiFiConnectLoop < 15) {MaxWiFiConnectLoop = 15;} 
if (MaxWiFiConnectLoop > 254) {MaxWiFiConnectLoop = 254;}
// GMCMapMidnightAccessDelay
if (!IsNumeric(GMCMMAD)) {GMCMMAD = "30";}
IntGMCMMAD = (int8_t)strtol(GMCMMAD, NULL, 10);
if (IntGMCMMAD < 5 || IntGMCMMAD > 59) {GMCMMAD = "30";}}

// *************************************************************************************************

String HTMLStringPart1() {
String HTMLString; HTMLString.reserve(180);  
HTMLString = F("<!doctype html><html lang='en-GB'>");
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