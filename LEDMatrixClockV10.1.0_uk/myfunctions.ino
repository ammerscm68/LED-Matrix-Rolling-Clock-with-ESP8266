// **************************** My Functions ***************************************************
// *************************************************************************************************

void Greetings_and_Holiday(bool GHStart) {   // uae = ä Sign 
getTimeLocal(); // get current time          // uoe = ö Sign 
GreetingsToday = false;                      // uue = ü Sign                                         
GT = F(""); HT = F("");                      // deg = ° Grad Sign
String GreetingText = F("");                 // WatchOwner Only
int HDay = 0;                                // uss = ß Sign  
int EasterSunday;
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
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
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
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
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
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}} // Output GreetingText

// ***************************** "Public Holidays" *********************************************************
String HolidayDate = String(d) + "." + String(mo);
String HoliDayText = F("");
// Holiday's
if (HolidayDate == "1.1") {HoliDayText = F("New Year's Day");}   // New Year
if (HolidayDate == First_Monday(5)) {HoliDayText = F("Early May Bank Holiday");}   // May Bank Holiday
if (HolidayDate == Last_Monday(5)) {HoliDayText = F("Spring Bank Holiday");}   // Spring Bank Holiday
if (HolidayDate == "25.12") {HoliDayText = F("Christmas Day");} // 1st Christmas Holiday
if (HolidayDate == "26.12") {HoliDayText = F("Boxing Day");} // Second Christmas Day
if (HolidayDate == "31.12") {HoliDayText = F("New Year's Eve");}   // New Year's Eve (Not a Holiday - Exceptional Case)
 
// Determine the non-fixed holidays (thanks to "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916)
if (mo>2 && mo<7) {
if (mo==3) {HDay=d;}  // If March, determine current Day
if (mo==4) {HDay=d+31;} 
if (mo==5) {HDay=d+31+30;} 
if (mo==6) {HDay=d+31+30+31;} 
EasterSunday = Calculate_Eastern();
if (SerialMonitor) {
Serial.println(F("")); 
Serial.println("EasterSunday: "+String(EasterSunday)); // View EasterSunday Value
Serial.println(F("")); } 
if (EasterSunday==HDay)  {HoliDayText = F("Easter Sunday");}     // Easter Sunday
if (EasterSunday-2==HDay)  {HoliDayText = F("Good Friday");}   // Good Friday
if (EasterSunday+1==HDay)  {HoliDayText = F("Easter Monday");}   // Easter Monday
if (EasterSunday+39==HDay)  {HoliDayText = F("Ascension of Christ");}  // Ascension of Christ
if (EasterSunday+49==HDay)  {HoliDayText = F("Pentecost Sunday");}  // Pentecost Sunday
if (EasterSunday+50==HDay)  {HoliDayText = F("Pentecost Monday");}}  // Pentecost Monday

if (HoliDayText != "") {
GreetingsToday = true;
if (HolidayDate != "31.12") { 
HT= "          +++     ToDay:   "+cleanText(HoliDayText)+"     +++                ";  
HT1= "+++ "+HoliDayText+" +++";  
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Holiday: "+HT1); // View Holiday Text
Serial.println(F(""));}} else { // New Year's Eve
HT= F("       +++     We wish you a good Start into the new Year     +++                ");
HT1= F("+++ We wish you a good Start into the new Year +++");
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("ToDay: New Year's Eve"); // View New Year's Eve Text
Serial.println(F(""));}}
if (Display && !GHStart) { // Output HoliDayText when Display on
if (HolidayDate != "1.1") {   
printStringWithShift(HT.c_str(), ScrollTextTime);} else { 
printStringWithShift("           ", ScrollTextTime); 
for (int i=0; i < 3; i++){ // 3x New Year's Greetings    
printStringWithShift("          +++     Happy New Year     +++                ", ScrollTextTime);}
ClockScrollIn(); SIC = false;}}} else {
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println(F("Holiday: No Holiday"));
Serial.println(F(""));}}} // End of the Function "void Greetings_and_Holiday()"

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
sendCmdAll(CMD_SHUTDOWN,0); // Display off
for (int i=0; i <= 50; i++){ 
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
String GShow = F(""); 
// Signs and Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Sign
if (SerialMonitor) {Serial.println(F("Witching Hour ....."));} 
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
if (SerialMonitor) {Serial.println(F("Load Clock Adjustments ..."));}
  // Load WiFi Access Data
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt").c_str();
  } else {ssid = F("");}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = F("");}

  // Load the Owner of the Watch
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
 Serial.println(F("Show Midnight Ghosts ?: [YES / NO] ")+ MidnightGhost);
 Serial.println(F(""));}

  // *** Load OpenWeatherMap Access Data ***
  // OpenWeatherMap API-Key as String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt").c_str();
  } else {weatherKey = F("");}
  // OpenWeatherMap City-ID as String
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
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}

  // NewsData Function ON/OFF
  if (AccessNewsData) { 
  if (LittleFS.exists("/status_NewsDatafunctions.txt")){
  String NAF = SetupLoad("status_NewsDatafunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsDataFunction = true; StatusNewsData = F("enabled");}
  if (NAF == "Off") {NewsDataFunction = false; StatusNewsData = F("disabled");}} else {
  NewsDataFunction = true; StatusNewsData = F("enabled");}} else {
  NewsDataFunction = false; StatusNewsData = F("disabled");}  

  // GMCMap function
  if (AccessGMCMap) { 
  if (LittleFS.exists("/status_GMCMapFunction.txt")){
  String GMCF = SetupLoad("status_GMCMapFunction.txt");
  GMCF.trim();
  if (GMCF == "" || GMCF == "On") {GMCMapFunction = true; StatusGMCMap = F("enabled");}
  if (GMCF == "Off") {GMCMapFunction = false; StatusGMCMap = F("disabled");}} else {
  GMCMapFunction = true; StatusGMCMap = F("enabled");}} else {
  GMCMapFunction = false; StatusGMCMap = F("disabled");}  

  // Load Pushover Data
  if (LittleFS.exists("/po-secureapptoken.txt")){
  SecureAppToken = SetupLoad("po-secureapptoken.txt");} else {SecureAppToken = F("");}
  SecureAppToken.trim(); // Remove spaces at the front and back
  if (LittleFS.exists("/po-usertoken.txt")){
  UserToken = SetupLoad("po-usertoken.txt");} else {UserToken = F("");}
  UserToken.trim(); // Remove spaces at the front and back
  if (LittleFS.exists("/po-device1.txt")){
  PushoverDevice1 = SetupLoad("po-device1.txt");} else {PushoverDevice1 = F("");}
  PushoverDevice1.trim(); // Remove spaces at the front and back
  if (LittleFS.exists("/po-device2.txt")){
  PushoverDevice2 = SetupLoad("po-device2.txt");} else {PushoverDevice2 = F("");}
  PushoverDevice2.trim(); // Remove spaces at the front and back
  if (LittleFS.exists("/po-device3.txt")){
  PushoverDevice3 = SetupLoad("po-device3.txt");} else {PushoverDevice3 = F("");}
  PushoverDevice3.trim(); // Remove spaces at the front and back
  if (LittleFS.exists("/po-messagesound.txt")){
  PushoverMessageSound = SetupLoad("po-messagesound.txt");} else {PushoverMessageSound= F("none");}
  if (SecureAppToken != "" && UserToken != "" && PushoverDevice1 != "") {
  IsPushover = true;} else {IsPushover = false;}

  // Display switch-on time as a string
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Remove spaces at the front and back
  } else {DisplayOnTime = F("");}
  // Display switch-off time as a string
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Remove spaces at the front and back
  } else {DisplayOffTime = F("");} 

  if (SerialMonitor) {
  Serial.println("");  
  Serial.println("DisplayOnTime: "+DisplayOnTime);
  Serial.println("DisplayOffTime: "+DisplayOffTime);
  Serial.println("");}

  // DHT-Sensor Offset
  if (LittleFS.exists("/dht-intempoffset.txt")){
  InTempOffset = SetupLoad("dht-intempoffset.txt");
  InTempOffset.trim(); // Remove spaces at the front and back
  InTempOffsetInt = InTempOffset.toInt(); // IntegerCheck was carried out in the Captive Portal
  } else {InTempOffset = F("0");
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Remove spaces at the front and back
  InHumOffsetInt = InHumOffset.toInt(); // IntegerCheck was carried out in the Captive Portal
  } else {InHumOffset = F("0");
          InHumOffsetInt = 0;}  // Humidity

  // Is a passive buzzer activated ?
  if (LittleFS.exists("/buzzer-install.txt")){
  if (SetupLoad("buzzer-install.txt") == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Status Buzzermodi
  if (IsBuzzer) {
  if (LittleFS.exists("/status_startsound.txt")){
  StartSound = SetupLoad("status_startsound.txt");} else {StartSound = F("Off");}
  if (LittleFS.exists("/status_hoursound.txt")){
  HourSound = SetupLoad("status_hoursound.txt");} else {HourSound = F("Off");}}

  // Check the plausibility of the information for Matrix Display Automatic On/Off
  CheckLEDMatrixDisplayOnOff();

  // Brightness of the LED Matrix
  if (LittleFS.exists("/matrix_brightness.txt")){
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // if the File is empty or error --> Level 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Level 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Level 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Level 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // Level 5 - Level 1
  // Double Point status
  if (LittleFS.exists("/status_doublepoint.txt")){
  String DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // if the File is empty or error
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}} else {DoublePointStatus = 1;}
  // ScrollText Speed
  if (LittleFS.exists("/status_scrolltexttime.txt")){
  String STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "25") {ScrollTextTime = 25;} // Level 8
  if (STT == "30") {ScrollTextTime = 30;} // Level 7
  if (STT == "35") {ScrollTextTime = 35;} // Level 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Default = Level 5
  if (STT == "45") {ScrollTextTime = 45;} // Level 4
  if (STT == "50") {ScrollTextTime = 50;} // Level 3
  if (STT == "55") {ScrollTextTime = 55;} // Level 2
  if (STT == "60") {ScrollTextTime = 60;}} else {ScrollTextTime = 40;} // Level 1 - Level 5 
  
  // Additional functions ON/OFF
  if (LittleFS.exists("/status_enhancedfunctions.txt")){
  String EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}} else {EnhancedFunctions = true;}

  LoadClockStatus(); // Load the last Status of the Clock
} else
    {
     // Set default values ​​for "Little FS" error
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
    weatherKey = F(""); // API-Key from OpenWeatherMap - https://openweathermap.org
    cityID = F(""); // City ID from OpenWeatherMap
    newsKey = F(""); // API-Key from NewsData - https://NewsData.io
    GMCMapHistoryID = F(""); // www.GMCMap.com - History ID
    GMCMapGMTOffset = F(""); // www.GMCMap.com - GMTOffset
    InTempOffsetInt = -1; // Temperature Offset
    InHumOffsetInt = -1; // Humidity  Offset
    StartSound = F("Off"); // no Buzzer Sound
    HourSound = F("Off"); // no Buzzer Sound
    SaveClockStatus("File System faulty !"); // Save Status of the Clock
    LoadClockStatus(); // Load the last Status of the Clock
    }
 swait = false;} // during Load no Website Handle - off

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Original saved time
  FDOffTime = DisplayOffTime; // Original saved time

   // Check display on/off time for plausibility
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

// Process inputs for Matrix On/Off
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");} else {MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");}}}}}}} // SSID - Ende  

// *************************************************************************************************

void SaveMatrixAdjustments(){
  swait = true;  // while Save no Website Handle - on
  if (littleFSInit == true) {
  if (SerialMonitor) {Serial.println(F("Save Clock Adjustments ..."));}    
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
  SetupSave("ssid.txt", TempSSID); // SSID WLAN
  SetupSave("ssid-passwd.txt", password); // Password WLAN

  // Save Owner of the Watch
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

  SetupSave("midnightghost.txt", MidnightGhost);

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
  // Matrix display switch-on time
  if (DisplayOnTime != ""){
  SetupSave("display-on-time.txt", DisplayOnTime);} else {
  if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
  // Matrix display switch-off time
  if (DisplayOffTime != ""){
  SetupSave("display-off-time.txt", DisplayOffTime);} else {
  if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
  // Is a passive Buzzer activated ?
  if (IsBuzzer){
  SetupSave("buzzer-install.txt", "True");} else {
  if (LittleFS.exists("/buzzer-install.txt")){LittleFS.remove("/buzzer-install.txt");}}

  // *** Save Pushover Data ***
  // Pushover SecureAppToken
  if (SecureAppToken == "") {
  if (LittleFS.exists("/po-secureapptoken.txt")){LittleFS.remove("/po-secureapptoken.txt");}  
  } else {SetupSave("po-secureapptoken.txt", SecureAppToken);} 
  // Pushover UserToken
  if (UserToken == "") {
  if (LittleFS.exists("/po-usertoken.txt")){LittleFS.remove("/po-usertoken.txt");} 
  } else {SetupSave("po-usertoken.txt", UserToken);}
  // Pushover Devicename 1
  if (PushoverDevice1 == "") {
  if (LittleFS.exists("/po-device1.txt")){LittleFS.remove("/po-device1.txt");} 
  } else {SetupSave("po-device1.txt", PushoverDevice1);}
  // Pushover Devicename 2
  if (PushoverDevice2 == "") {
  if (LittleFS.exists("/po-device2.txt")){LittleFS.remove("/po-device2.txt");} 
  } else {SetupSave("po-device2.txt", PushoverDevice2);}
  // Pushover Devicename 3
  if (PushoverDevice3 == "") {
  if (LittleFS.exists("/po-device3.txt")){LittleFS.remove("/po-device3.txt");} 
  } else {SetupSave("po-device3.txt", PushoverDevice3);}
  // Pushover Message Sound
  if (PushoverMessageSound == "none" || PushoverMessageSound == "") {
  if (LittleFS.exists("/po-messagesound.txt")){LittleFS.remove("/po-messagesound.txt");} 
  } else {SetupSave("po-messagesound.txt", PushoverMessageSound);}

  // Check DHT Offset
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = F("0");} // Temperature
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = F("0");} // Air Humidity
  
  // DHT-Sensor Offset
  if (InTempOffset != ""){
  SetupSave("dht-intempoffset.txt", InTempOffset);} else {
  if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
  if (InHumOffset != ""){
  SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
  if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}}
 swait = false;}  // while Save no Website Handle - off

// *************************************************************************************************

// Load and save setup settings
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

// Switch Display on and off using a Button
void MatrixButtonOnOff(){
if(digitalRead(PushButtonPin) == LOW){
MyWaitLoop(500); //wait
Display = !Display;
if (!Display && ClockWiFiConnectFail == false){  
sendCmdAll(CMD_SHUTDOWN,0); // Display turn off
if (SerialMonitor) {Serial.println("Matrix switched off via Button at "+ FormatedDateTimeEN(false));} 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // get current time
    sendCmdAll(CMD_SHUTDOWN, 1); // Display turn on
    sendCmdAll(CMD_INTENSITY, 0); // LED Matrix brightness --> 0= Minimum
    // ShortBuzzerBeep();
    if (SerialMonitor) {Serial.println("Matrix switched on via Button at "+ FormatedDateTimeEN(false));}} else
    {
    sendCmdAll(CMD_SHUTDOWN, 1); // only Display on
    printCharWithShift('.',5);
    }
    // Activate the “Access Point” after switching it off 10 times
    APStart += 1; // inc  
    if (APStart == 10 && littleFSInit == true) {
    ResetClock();} // Clock reset 
    }
    delay(250);}} //wait

// *************************************************************************************************

String FormatedDateTimeEN(bool FDTHTML) {
String DateTimeString = F("");
getTimeLocal(); // get current DateTime
// Time
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
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

void wificonnect(bool Start) {
WiFiConnectLoop = 0; // Variable reset  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
if (SerialMonitor) {Serial.print(".");} 
if (!ClockStart && Display) {printCharWithShift2('.',1);}
MatrixButtonOnOff(); // Query Button
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED)
{ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
APStart = 0; // "Access Point" Button Counter reset
if (SerialMonitor) {Serial.println(F("WiFi Connection disconnected ..."));} 
} else
{
ClockWiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop));} // Time until connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {
Serial.println(F(""));
Serial.print(F("Connected to IP-Address: ")); 
Serial.println(WiFi.localIP());}}}

// *************************************************************************************************

void WiFiUDPStart() { 
Udp.begin(localPort);
if (SerialMonitor) {
Serial.print(F("local Port: "));
Serial.println(Udp.localPort());
Serial.println(F("NTP-Time - Waiting for synchronization"));}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Number of seconds between resynchronizations. 86400 = 1 day
// setSyncInterval(60); // Test
if (SerialMonitor) {Serial.println("UDP Server started - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WiFiUDPStop() {
Udp.stop();
if (SerialMonitor) {Serial.println("UDP Server stopped - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WebServerStart() {
server.begin();
if (SerialMonitor) {Serial.println("Web Server started - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void WebServerStop() {
server.stop();
if (SerialMonitor) {Serial.println("Web Server stopped - "+FormatedDateTimeEN(false));}}

// *************************************************************************************************

void MyWaitLoop(int wlt){// Pause Loop
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

// *************************************************************************************************

// Checking a string to see if it is of type Float and accepting from the decimal point
boolean IsFloat(String str) {  
bool bPoint=false;  
if(str.length()<1){return false;} else {
for(int i = 1; i < str.length(); i++) {
if(str.charAt(i) == '.') {bPoint=true; break;}}
return bPoint;}}

// *************************************************************************************************
void ResetAPCounter() {
String ResetValue[] = {"0","2", "4", "6", "8", "10", "12", "14" ,"16", "18", 
                       "20", "22", "24", "26", "28", "30", "32", "34", "36",
                       "38", "40", "42", "44", "46", "48", "50", "52", "54",
                       "56", "58"}; 
for (int i=0; i <= 30; i++){                      
if  (String(m)+":"+String(s) == ResetValue[i]+":2") { 
APStart = 0;}}} // "Reset Access Point" Button counter every 2 minutes

// *************************************************************************************************

void Download() {
if (ClockStart) { 
char dl = char('~' + 24); //  // Arrow down for Download   
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);}}

// *************************************************************************************************

void DownloadComplet(){
if (ClockStart) { 
// Advertisement clear    
clr(); 
refreshAll();}}

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
if (SerialMonitor) {Serial.println(F("Reset - The Clock will Restart"));}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
printStringWithShift("        +++     Reset - The Clock will Restart      +++           ",ScrollTextTime);
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
DOffMinInt -= 10; // Minus 10 Minutes
if (DOffMinInt <= 0) {
DOffMinInt = 50 + DOffMin.toInt(); 
DOffMinIntOffset = true;
DOffHourInt -= 1;
if (DOffHourInt < 0) {DOffHourInt = 23;}} else {DOffMinIntOffset = false;} 

// Display Off Time -  Minus 10 Minutes
if (DOffHourInt < 10) {TempDOffHour = "0"+String(DOffHourInt);} else {TempDOffHour = String(DOffHourInt);}
if (DOffMinInt < 10)  {TempDOffMin = "0"+String(DOffMinInt);} else {TempDOffMin = String(DOffMinInt);}
String ActionTime = TempDOffHour+TempDOffMin;  
int ActionTimeInt = ActionTime.toInt();

int ActualHour = h; // Current Hour
int ActualMin = m;  // Current Minutes
// Current Time as a String
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
SHTMLString = F(""); // String clear
freeRam();} // Defragment Heap

// *************************************************************************************************

void StringBytes(String myString) { // Check how much memory a string occupies.
unsigned int len = myString.length() + 1;
byte buff[len];
String GesBuff;
int gesBuff = 0;
myString.getBytes(buff, len);
if (SerialMonitor) {Serial.print(F("String-Size in Byte: "));}
for (int i = 0; i < len; i++){
if (SerialMonitor) {  
GesBuff = Serial.println(buff[i]);}
gesBuff += GesBuff.toInt();}
if (SerialMonitor) {
Serial.println(F("+++++++"));
Serial.println(gesBuff);
Serial.println(F("+++++++"));}}

// *************************************************************************************************

String Politeness(bool WO1F) {
String PReturn = F("");  
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

// *************************************************************************************************

void StatusWarningLED(String LEDStatus) {
if (HWLEDPin != -1) {
if (LEDStatus == "CHECK") {digitalWrite(HWLEDPin, HIGH); MyWaitLoop(150); digitalWrite(HWLEDPin, LOW);} else {
if (LEDStatus == "HIGH") {if (Display) {digitalWrite(HWLEDPin, HIGH);}}
if (LEDStatus == "LOW") {digitalWrite(HWLEDPin, LOW);}}}}

// *************************************************************************************************

String WiFiSignalStrength(bool MStart) {
long WifiRSSI = WiFi.RSSI(); // WiFi Signal Strength in dBm
String WiFiSignalQuality = F(""); 
if (SerialMonitor) {Serial.println("WiFi Signal Strength: "+ String(WifiRSSI) +" dBm");}
if (WifiRSSI >= -40) {
if (MStart) {WiFiSignalQuality = F("very good");} else {  
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; very good ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -40 && WifiRSSI >= -60) {
if (MStart) {WiFiSignalQuality = F("good");} else {   
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; good ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -60 && WifiRSSI >= -70) {
if (MStart) {WiFiSignalQuality = F("sufficient");} else {   
WiFiSignalQuality = "<h7>WiFi Signal Strength: &nbsp;&nbsp; sufficient ("+String(WifiRSSI)+" dBm)</h7>";}}
if (WifiRSSI < -70 && WifiRSSI >= -80) {
if (MStart) {WiFiSignalQuality = F("bad");} else {
WiFiSignalQuality = "<h10>WiFi Signal Strength: &nbsp;&nbsp; bad ("+String(WifiRSSI)+" dBm)</h10>";}}  
if (WifiRSSI < -80) {
 if (MStart) {WiFiSignalQuality = F("very bad");} else {
WiFiSignalQuality = "<h10>WiFi Signal Strength: &nbsp;&nbsp; very bad ("+String(WifiRSSI)+" dBm)</h10>";}} 
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
