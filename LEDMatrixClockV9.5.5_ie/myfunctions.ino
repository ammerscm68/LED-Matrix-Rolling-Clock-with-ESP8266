// **************************** My Functions ***************************************************
// *************************************************************************************************

void Greetings_and_Holiday(bool GHStart) {   // uae = ä Sign 
getTimeLocal(); // get current time          // uoe = ö Sign 
GreetingsToday = false;                      // uue = ü Sign                                         
GT = ""; HT = "";                            // deg = ° Grad Sign
String GreetingText = "";                    // WatchOwner Only
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
if (ClockStart == true) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";} 
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
if (ClockStart == true) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= GreetingText;} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= GreetingText;}  
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
if (ClockStart == true) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= GreetingText;} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= GreetingText;} 
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Greeting: "+GT);}  
SIC == true; // for ScrollIn Clock 
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}} // Output GreetingText

// ***************************** "Public Holidays" *********************************************************
String HolidayDate = String(d) + "." + String(mo);
String HoliDayText = "";
// Holiday's
if (HolidayDate == "1.1") {HoliDayText = "New Year's Day";}   // New Year
if (HolidayDate == First_Monday(2)) {HoliDayText = "Saint Brigid's Day / Imbolc";}   // Saint Brigid's Day / Imbolc
if (HolidayDate == "17.3") {HoliDayText = "Saint Patrick's Day";}   //Saint Patrick's Day
if (HolidayDate == First_Monday(5)) {HoliDayText = "May Day";}   // May Day
if (HolidayDate == First_Monday(6)) {HoliDayText = "June Holiday";}   // June Holiday
if (HolidayDate == First_Monday(8)) {HoliDayText = "August Holiday";}   // August Holiday
if (HolidayDate == Last_Monday(10)) {HoliDayText = "October Holiday";}   // October Holiday
if (HolidayDate == "25.12") {HoliDayText = "Christmas Day";} // 1st Christmas Holiday
if (HolidayDate == "26.12") {HoliDayText = "Saint Stephen's Day";} // Saint Stephen's Day
 
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
if (EasterSunday==HDay)  {HoliDayText = "Easter Sunday";}     // Easter Sunday (No Holiday)
if (EasterSunday+1==HDay)  {HoliDayText = "Easter Monday";}}   // Easter Monday

if (HoliDayText != "" || HolidayDate == "31.12") {
GreetingsToday = true;
if (HolidayDate != "31.12") {
HT= "          +++     ToDay:   "+cleanText(HoliDayText)+"     +++                ";  
HT1= HoliDayText;  
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Holiday: "+HT1); // View Holiday Text
Serial.println(F(""));}} else { // New Year's Eve
HT= "       +++     We wish you a good Start into the new Year     +++                ";
HT1= "We wish you a good Start into the new Year";
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("New Year's Eve: "+HT1); // View New Year's Eve Text
Serial.println(F(""));}}
SIC == true; // for ScrollIn Clock  
if (Display && !GHStart) {printStringWithShift(HT.c_str(), ScrollTextTime);}} else { // Output HoliDayText
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println(F("Holiday: No Holiday"));
Serial.println(F(""));}}} // End function "void Greetings_and_Holiday()"

// *************************************************************************************************

void clearscreen() { 
for(int i=0; i<10; i++) {
if (SerialMonitor) {Serial.println(F("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"));}}}

// *************************************************************************************************

void ClockScrollIn() {
String TimeString;
// Uhrzeit
getTimeLocal(); // get current time
if (s == 59 && m < 59) {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m+1);} else {TimeString = "  "+String(h)+String(m+1);}
} else {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m);} else {TimeString = "  "+String(h)+String(m);}  
}
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);}

// *************************************************************************************************

void ShowGhost() {
String GShow = ""; 
// Sonderzeichen und Umlaute (Font: clockfont[])
char  ghost = '9' + 1; // Ghost Sign
if (SerialMonitor) {Serial.println(F("Witching Hour ....."));} 
if (Display) {
sendCmdAll(CMD_SHUTDOWN,0); // Matrix off 
printStringWithShift3("         ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix on
GShow = String(ghost)+"  "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // short wait
HandleOTAandWebServer();  // OTA and Web Server Query
printStringWithShift3("      ",ScrollTextTime);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // short wait
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // OTA and Web Server Query
getTimeLocal(); // get current time 
sendCmdAll(CMD_SHUTDOWN,0); // Matrix off 
for (int i=0; i <= 20; i++){
HandleOTAandWebServer();  // OTA and Web Server Query 
getAnimClock(false);} // Show / Calculate Time
printStringWithShift3("      ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Matrix on
MyWaitLoop(100); // short wait
ClockScrollIn();
} else {
printStringWithShift3("      ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,1); // Matrix on
GShow = String(ghost)+"  "+String(ghost);
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // short wait
HandleOTAandWebServer();  // OTA and Web Server Query 
printStringWithShift3("      ",ScrollTextTime);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime);
MyWaitLoop(3000); // short wait
HandleOTAandWebServer();  // OTA and Web Server Query 
printStringWithShift3("      ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,0);}} // Matrix off

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
  } else {ssid = "";}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = "";}
  // *** Load OpenWeatherMap Access Data ***
  // OpenWeatherMap API-Key as String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt").c_str();
  } else {weatherKey = "";}
  // OpenWeatherMap City-ID as String
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt").c_str();
  } else {cityID = "";} 
  // NewsAPI-Key
  if (LittleFS.exists("/newsapi-apikey.txt")){
   newsKey = SetupLoad("newsapi-apikey.txt");
   newsKey.trim();} else {newsKey = "";} // https://newsapi.org

   // Deactivate OpenWeatherMAP if no API Key or City ID is available
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // Deactivate NewsAPI if no API Key is available
   if (newsKey != "") {AccessNewsAPI = true;} else {
   AccessNewsAPI = false;}   

  // Weather Data function ON/OFF
  if (AccessOpenWeatherMap) {
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = "enabled";}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = "disabled";}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = "enabled";}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = "disabled";}

  // NewsAPI Function ON/OFF
  if (AccessNewsAPI) { 
  if (LittleFS.exists("/status_newsapifunctions.txt")){
  String NAF = SetupLoad("status_newsapifunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsAPIFunction = true; StatusNewsAPI = "enabled";}
  if (NAF == "Off") {NewsAPIFunction = false; StatusNewsAPI = "disabled";}} else {
  NewsAPIFunction = true; StatusNewsAPI = "enabled";}} else {
  NewsAPIFunction = false; StatusNewsAPI = "disabled";}  

  // Display switch-on time as a string
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Remove spaces at the front and back
  } else {DisplayOnTime = "";}
  // Display switch-off time as a string
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Remove spaces at the front and back
  } else {DisplayOffTime = "";} 

  // DHT-Sensor Offset
  if (LittleFS.exists("/dht-intempoffset.txt")){
  InTempOffset = SetupLoad("dht-intempoffset.txt");
  InTempOffset.trim(); // Remove spaces at the front and back
  InTempOffsetInt = InTempOffset.toInt(); // IntegerCheck was carried out in the Captive Portal
  } else {InTempOffset = "0";
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Remove spaces at the front and back
  InHumOffsetInt = InHumOffset.toInt(); // IntegerCheck was carried out in the Captive Portal
  } else {InHumOffset = "0";
          InHumOffsetInt = 0;}  // Humidity

  // Is a passive buzzer activated ?
  if (LittleFS.exists("/buzzer-install.txt")){
  if (SetupLoad("buzzer-install.txt") == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Status Buzzermodi
  if (IsBuzzer) {
  if (LittleFS.exists("/status_startsound.txt")){
  StartSound = SetupLoad("status_startsound.txt");} else {StartSound = "Off";}
  if (LittleFS.exists("/status_hoursound.txt")){
  HourSound = SetupLoad("status_hoursound.txt");} else {HourSound = "Off";}}

  // Check the plausibility of the information for Matrix Display Automatic On/Off
  CheckLEDMatrixDisplayOnOff();

  // Brightness of the LED Matrix
  if (LittleFS.exists("/matrix_brightness.txt")){
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // if the File is empty or error --> Level 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Level 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Level 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Level 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // Level 5 - Level 1
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
} else
    {
     // Set default values ​​for "Little FS" error
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
    StartSound = "Off"; // no Buzzer Sound
    HourSound = "Off"; // no Buzzer Sound
    }
 swait = false;} // during Load no Website Handle - off

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Original saved time
  FDOffTime = DisplayOffTime; // Original saved time

   // Check display on/off time for plausibility
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

// Process inputs for Matrix On/Off
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = "---";} else {MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = "---";}}}}}}} // SSID - Ende  

// *************************************************************************************************

void SaveMatrixAdjustments(){
  swait = true;  // while Save no Website Handle - on
  if (littleFSInit == true) {
  if (SerialMonitor) {Serial.println(F("Save Clock Adjustments ..."));}    
  // Trim
  TempSSID.trim(); // SSID WLAN
  newsKey.trim(); // NewsAPI API-Key
  DisplayOnTime.trim(); // Matrix display switch-on time
  DisplayOffTime.trim(); // Matrix display switch-off time
  InTempOffset.trim(); // DHT-Sensor Offset (Temperature)
  InHumOffset.trim(); // DHT-Sensor Offset (Air Humidity)
  
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

  // Check DHT Offset
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = "0";} // Temperature
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = "0";} // Air Humidity
  
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
if (SerialMonitor) {Serial.println("Matrix switched off via Button at "+ FormatedDateTime());} 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // get current time
    sendCmdAll(CMD_SHUTDOWN, 1); // Display turn on
    sendCmdAll(CMD_INTENSITY, 0); // LED Matrix brightness --> 0= Minimum
    // ShortBuzzerBeep();
    if (SerialMonitor) {Serial.println("Matrix switched on via Button at "+ FormatedDateTime());}} else
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

String FormatedDateTime() {
String DateTimeString = "";
getTimeLocal(); // get current time
// clock
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
DateTimeString += " / "; // Time
// Date
DateTimeString += String(ye); 
if (mo < 10){DateTimeString += "-0"+String(mo);} else {DateTimeString += "-"+String(mo);}
if (d < 10){DateTimeString += "-0"+String(d);} else {DateTimeString += "-"+String(d);}
return DateTimeString;}

// *************************************************************************************************

// Arduino OTA query only every 0.8 seconds and web server query only every 0.8 seconds.
void HandleOTAandWebServer(){ 
if (millis() - hwsTime > 800) { 
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
if (!Start) {if (SerialMonitor) {Serial.println(F("WiFi Connection disconnected ..."));}} else {StartFail = true;} 
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
Serial.print(F("lokaler Port: "));
Serial.println(Udp.localPort());
Serial.println(F("NTP-Time - Waiting for synchronization"));}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Number of seconds between resynchronizations. 86400 = 1 day
// setSyncInterval(60); // Test
if (SerialMonitor) {Serial.println("UDP Server started - "+FormatedDateTime());}}

// *************************************************************************************************

void WiFiUDPStop() {
Udp.stop();
if (SerialMonitor) {Serial.println("UDP Server stopped - "+FormatedDateTime());}}

// *************************************************************************************************

void WebServerStart() {
server.begin();
if (SerialMonitor) {Serial.println("Web Server started - "+FormatedDateTime());}}

// *************************************************************************************************

void WebServerStop() {
server.stop();
if (SerialMonitor) {Serial.println("Web Server stopped - "+FormatedDateTime());}}

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

void DownloadComplet() {
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
digitalWrite(HWLEDPin, LOW); // LED off  
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
} else {fTime = "0";} 
return fTime.toInt();}

// *************************************************************************************************

void SendHTMLString(String SHTMLString) {
server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
server.sendHeader("Pragma", "no-cache");
server.sendHeader("Expires", "-1");  
server.send(200, "text/html", SHTMLString);
SHTMLString = "";} // String leeren

// *************************************************************************************************

void CheckHumidity() {
  // Indoor Humidity - Message if too low or too high
InHumValue = InHumMessure();
  if (InHumValue != "-") {
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {
   // Air Humidity too low    
  if (Display) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // LED On or Off 
  InHumMessage = F("Attention: Air Humidity too low !");}
  if (IntInHumValue >= MaxComfortHum) {
  // Air Humidity too high     
  if (Display) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}  // LED On or Off  
  if (WeatherFunctions and Display){
  if (humidity >= MaxComfortHum || humidity == -1) {InHumMessage = F("Attention: Air Humidity too high !");} else 
  {InHumMessage = F("Attention: Air Humidity too high! - Please ventilate");}
  } else {InHumMessage = F("Attention: Air Humidity too high !");}}
  if (IntInHumValue >= MinComfortHum && IntInHumValue <= MaxComfortHum) {
  digitalWrite(HWLEDPin, LOW); // LED Off
  InHumMessage = F("");}} else {digitalWrite(HWLEDPin, LOW); // LED Off
  InHumMessage = F("");}  
}

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
String PReturn = "";  
if (WO1F) {
if (h >= 0 && h < 12)   {PReturn = "        +++  Good morning "+WatchOwner1FirstName+"   Have a nice day  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = "        +++  Have a nice day  +++           ";} 
if (h >= 18 && h < 21)  {PReturn = "        +++  Good evening "+WatchOwner1FirstName+"   see you soon +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Good night "+WatchOwner1FirstName+" +++           ";}} else {
if (h >= 0 && h < 12)   {PReturn = "        +++  Good morning... Have a nice day  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = "        +++  Have a nice day  +++           ";} 
if (h >= 18 && h < 21)  {PReturn = "        +++  Good evening ...   see you soon +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Good night  +++           ";}} 
return PReturn; 
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
