// Web Server Homepage
void handleRoot() { 
// send Data
if (!MEP){
server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
SendHTMLString(""); // Send HTMLString                    
server.sendContent(RootHTMLString_1());  // 1. Part
server.sendContent(RootHTMLString_2());  // 2. Part
server.sendContent(RootHTMLString_3());  // 3. Part
server.sendContent(RootHTMLString_4());  // 4. Part
server.sendContent(RootHTMLString_5());  // 5. Part
server.sendContent(RootHTMLString_6());  // 6. Part
if (AccessOpenWeatherMap) {server.sendContent(RootHTMLString_7());}  // 7. Part
if (AccessNewsData) {server.sendContent(RootHTMLString_8());}  // 8. Part
if (AccessGMCMap) {server.sendContent(RootHTMLString_9());} // 9. Part
if (IsBuzzer) {server.sendContent(RootHTMLString_10());} // 10. Part only for Passiv Piezo Summer (Buzzer)
server.sendContent(RootHTMLString_11()); // 11. Part
server.sendContent(RootHTMLString_12()); // 12. Part
server.client().flush();
MyWaitLoop(100); // short pause so that the memory can be emptied
server.client().stop();
if (RebootNow) {
if (!Display) {
printStringWithShift("           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Display on  
} // Switch on Display when off  
printStringWithShift("        +++       The Clock will Restart      +++           ",ScrollTextTime); 
StatusWarningLED("LOW"); // LED off 
ESP.restart();} // Restart Clock
if (littleFSInit && ResetNow) {ResetClock();}}} // Settings to Standard - WLAN Access Data will be deleted

/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Space | <meta http-equiv=\"refresh\" content=\"60\">\ = Refresh all 60 sec*/ 
String RefreshTime = F("60"); // Refresh Website after X seconds
  String HTMLString = F("");
   String InTempOnOff1 = F("");
    String InTempOnOff2 = F("");

    CheckInHumidity(); // Indoor Humidity measurement

  // Display Indoor Temperature YES/NO (DHT11 or DHT22)
  if (InTempCounter < 5) {
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Temperature measurement 
  if (InTempValue != "-" && InTempValue != "!") {
  IntInTempValue = InTempValue.toInt();  
  if (IntInTempValue < 10) {
  InTempOnOff1 = F("id= 'Bvalue'>Temperature on the Clock:");  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {InTempOnOff1 = F("id= 'Gvalue'>Temperature on the Clock:");} else {  
  InTempOnOff1 =  F("id= 'Rvalue'>Temperature on the Clock:");}}
  if (InHumMessage == "") { 
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Displays Indoor Temperature</button></a>");} else {
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Displays Indoor Temperature</button></a>"); 
  InTempOnOff2 += "<h9>"+InHumMessage+"</h9>";}
  InTempOnOff2 += F("<p></p>");
  InTempOnOff2 += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  } else {InTempOnOff1 = F("");
          InTempOnOff1 = F("");}
  } else {InTempOnOff1 = F("");
          InTempOnOff2 = F("");
          InTempValue = F("!");}

  // Check HTML Substring
// 1. Part of the HTML string
  HTMLString = F("<!DOCTYPE html><html lang='en-GB'>"); 
  HTMLString += F("<head>");
  HTMLString += F("<title>LED-Matrix-Clock</title>");
  HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body { background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
  HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
  HTMLString += F("h2 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h3 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h4 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h5 { color: white;font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h6 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h7 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffffff;}");
  HTMLString += F("h8 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #f74f4f; font-weight: bold;}");
  HTMLString += F("h9 { font-size: 18px; margin-left: 80px; margin-top: 0px; margin-bottom: 0px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("h10 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}");
  HTMLString += F("#Rvalue {color: #FC3A30;}");
  HTMLString += F("#Gvalue {color: lime;}");
  HTMLString += F("#Bvalue {color: #2ECCFA;}");
  HTMLString += F("</style>");
  HTMLString += F("</head>");
  HTMLString += F("<body>");
  HTMLString += F("<h1>LED-Matrix-Clock</h1>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += InTempOnOff2;
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){ 
// Selection of brightness of the Matrix and Runtime of the Clock
  String HTMLString = F("");
  // 2. Part of the HTML string
  HTMLString = F("<h2>Selection - Display ON / OFF / TEST / Brightness Levels:</h2>");
  if (!Display) { 
  HTMLString += CreateButton("Display ON",false,"mon","","110","10",""); 
  HTMLString += CreateButton("Display OFF",true,"moff","","110","20","");
  HTMLString += CreateButton("Display TEST",false,"","","110","20","disabled");} else {
  HTMLString += CreateButton("Display ON",true,"mon","","110","10","");  
  HTMLString += CreateButton("Display OFF",false,"moff","","110","20","");
  HTMLString += CreateButton("Display TEST",false,"mdt","","110","20","");}

  if (!Display) {  
  if (Brightness == "L1") {HTMLString += CreateButton("Level",true,""," 1","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 1","80","10","disabled");}
  if (Brightness == "L2") {HTMLString += CreateButton("Level",true,""," 2","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 2","80","10","disabled");}
  if (Brightness == "L3") {HTMLString += CreateButton("Level",true,""," 3","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 3","80","10","disabled");}
  if (Brightness == "L4") {HTMLString += CreateButton("Level",true,""," 4","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 4","80","10","disabled");}
  if (Brightness == "L5") {HTMLString += CreateButton("Level",true,""," 5","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 5","80","10","disabled");}} 
  else {
  if (Brightness == "L1") {HTMLString += CreateButton("Level",true,"mb1"," 1","80","10","");} else {HTMLString += CreateButton("Level",false,"mb1"," 1","80","10","");}
  if (Brightness == "L2") {HTMLString += CreateButton("Level",true,"mb2"," 2","80","10","");} else {HTMLString += CreateButton("Level",false,"mb2"," 2","80","10","");}
  if (Brightness == "L3") {HTMLString += CreateButton("Level",true,"mb3"," 3","80","10","");} else {HTMLString += CreateButton("Level",false,"mb3"," 3","80","10","");}
  if (Brightness == "L4") {HTMLString += CreateButton("Level",true,"mb4"," 4","80","10","");} else {HTMLString += CreateButton("Level",false,"mb4"," 4","80","10","");}
  if (Brightness == "L5") {HTMLString += CreateButton("Level",true,"mb5"," 5","80","10","");} else {HTMLString += CreateButton("Level",false,"mb5"," 5","80","10","");}}  
  HTMLString += F("<p></p>");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_3(){
String HTMLString = F("");
String DisplayAutomaticOnOff = F("");  
// Display automatically ON/OFF if specified in Access Point
  if (DisplayOnTime != "" && DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatic ON at &nbsp;"+FDOnTime+"&nbsp;&nbsp;o'clock<br />Display automatic OFF at &nbsp;"+FDOffTime+"&nbsp;&nbsp;o'clock";} else {
  if (DisplayOnTime == "" && DisplayOffTime == ""){
  DisplayAutomaticOnOff = F("Display automatic ON / OFF is disabled");} else {  
  if (DisplayOnTime != ""){
  DisplayAutomaticOnOff = "Display automatic ON at &nbsp;"+FDOnTime+"&nbsp;&nbsp;o'clock";} else {
  if (DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatic OFF at &nbsp;"+FDOffTime+"&nbsp;&nbsp;o'clock";}}}}
// 3. Part of the HTML string
HTMLString = "<h4>"+DisplayAutomaticOnOff+"</h4>";
HTMLString += F("<a href =\"/cdof\"><button class=\"button\" style=\"margin-right: 10px; height: 24px; font-size: 14px; font-weight: bold; width: 80px;\">Change</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}  

/********************************************************************************************************************************************************/

String RootHTMLString_4(){ // Status Doublepoint
  String HTMLString = F("");
  // 4. Part of the HTML string
  HTMLString = F("<h2>Selection - Status Doublepoint of the Time:</h2>");
  if (!Display) {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Flash",true,"","","80","10","disabled");} else {
  HTMLString += CreateButton("Flash",false,"","","80","10","disabled");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Permanent ON",true,"","","130","10","disabled");} else {
  HTMLString += CreateButton("Permanent ON",false,"","","130","10","disabled");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Permanent OFF",true,"","","130","10","disabled");} else {
  HTMLString += CreateButton("Permanent OFF",false,"","","130","10","disabled");}  
  } else {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Flash",true,"sdflash","","80","10","");} else {
  HTMLString += CreateButton("Flash",false,"sdflash","","80","10","");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Permanent ON",true,"sdon","","130","10","");} else {
  HTMLString += CreateButton("Permanent ON",false,"sdon","","130","10","");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Permanent OFF",true,"sdoff","","130","10","");} else {
  HTMLString += CreateButton("Permanent OFF",false,"sdoff","","130","10","");}}
  HTMLString += F("<p></p>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_5(){ // ScrollText Speedlevel
String HTMLString = F("");
  // 5. Part of the HTML string
  HTMLString = F("<h2>Selection - Scrolling Text Speed:</h2>"); 
  if (!Display) { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Level",true,""," 1","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 1","80","10","disabled");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Level",true,""," 2","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 2","80","10","disabled");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Level",true,""," 3","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 3","80","10","disabled");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Level",true,""," 4","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 4","80","10","disabled");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Level",true,""," 5","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 5","80","10","disabled");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Level",true,""," 6","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 6","80","10","disabled");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Level",true,""," 7","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 7","80","10","disabled");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Level",true,""," 8","80","10","disabled");} else {HTMLString += CreateButton("Level",false,""," 8","80","10","disabled");} 
  } else { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Level",true,"vlt1"," 1","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt1"," 1","80","10","");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Level",true,"vlt2"," 2","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt2"," 2","80","10","");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Level",true,"vlt3"," 3","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt3"," 3","80","10","");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Level",true,"vlt4"," 4","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt4"," 4","80","10","");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Level",true,"vlt5"," 5","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt5"," 5","80","10","");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Level",true,"vlt6"," 6","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt6"," 6","80","10","");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Level",true,"vlt7"," 7","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt7"," 7","80","10","");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Level",true,"vlt8"," 8","80","10","");} else {HTMLString += CreateButton("Level",false,"vlt8"," 8","80","10","");}}
  HTMLString += F("<p></p>"); 
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_6(){ // Additional functions
String HTMLString = F("");
String EF = F("");
if (EnhancedFunctions) {
EF =  CreateButton("ON",true,"zfon","","80","10","");
EF += CreateButton("OFF",false,"zfoff","","80","10","");} else {
EF =  CreateButton("ON",false,"zfon","","80","10","");
EF += CreateButton("OFF",true,"zfoff","","80","10","");}
// 6. Part of the HTML string
HTMLString = "<h2>Selection - Additional Functions &nbsp;ON / OFF: &nbsp;(Date, Holidays, Birthdays)</h2>";
HTMLString += EF; // Additional functions (date, holidays, birthdays) - ON/OFF
if (EnhancedFunctions && GreetingsToday) {
if (GT1 != "" && HT1 != "") { // GT1 = Birthdays and other Greetings HT1 = Holidays
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">ToDay: &nbsp;&nbsp;"+HT1+"&nbsp; / &nbsp;"+GT1+"</a>";} else {  
if (GT1 != "") {    
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">ToDay: &nbsp;&nbsp;"+GT1+"</a>";}
if (HT1 != "") {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">ToDay: &nbsp;&nbsp;"+HT1+"</a>";}}} else { // Additional features
if (EnhancedFunctions) {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">&nbsp;&nbsp;"+DateString+"</a>";} else {
HTMLString += F("<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\"></a>");}}
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_7() { // Display Weather Data from OpenWeatherMap
// Enable/disable Weather Data
String HTMLString = F("");
String OWMA = F("");
// Show OpenWeatherMap Status or not
if (AccessOpenWeatherMap) {
if (WeatherFunctions) {
OWMA =  CreateButton("ON",true,"owmfon","","80","10","");
OWMA += CreateButton("OFF",false,"owmfoff","","80","10","");} else {
OWMA =  CreateButton("ON",false,"owmfon","","80","10","");  
OWMA += CreateButton("OFF",true,"owmfoff","","80","10","");}

if (WeatherFunctions && StatusOpenWeatherMap != "enabled" && Display) {
OWMA += CreateButton("Weather",false,"aowmn","","80","10","");} else {
OWMA += CreateButton("Weather",false,"aowmn","","80","10","disabled");}

if (!WeatherFunctions) {StatusOpenWeatherMap = F("disable");} else {
if (OWMAdjustChange) {StatusOpenWeatherMap = F("enabled"); OWMAdjustChange = false;}}
if (WeatherFunctions && StatusOpenWeatherMap == "enabled" && Display) {
OWMA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;enabled</a>");} 
if (WeatherFunctions && StatusOpenWeatherMap == "OK" && Display) {
String WSTemp = String(temp, 0); WSTemp.trim();
if (IsNumeric(WSTemp) && IsNumeric(String(humidity))) { // Check Temperatur und Luftfeuchte 
OWMA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+OWMLastConnectionDateTime+"";
OWMA += "&nbsp;&nbsp;&nbsp;&nbsp;(OT=&nbsp;"+WSTemp+"°C &nbsp;/&nbsp; OH=&nbsp;"+String(humidity)+"%)</a>";} else {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Error - "+OWMLastConnectionDateTime+"</a>";}} 
if (WeatherFunctions && StatusOpenWeatherMap == "Error" && Display) {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Error - "+OWMLastConnectionDateTime+"</a>";}
if (WeatherFunctions && !Display) {
OWMA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ No downloading of Weather Data when Display OFF +++</a>");}}
// 7. Part of the HTML string  
HTMLString =  F("<h2>Selection - Display Current Weather Data from ---OpenWeatherMAP.org--- &nbsp; ON / OFF / WEATHER / STATUS:</h2>");
HTMLString += OWMA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_8() { // Display NewsData.io News
String HTMLString = F("");
String ANA = F("");
  if (AccessNewsData) {
// NewsData ON / OFF
if (NewsDataFunction) {
ANA =  CreateButton("ON",true,"nafon","","80","10","");
ANA += CreateButton("OFF",false,"nafoff","","80","10","");} else {
ANA =  CreateButton("ON",false,"nafon","","80","10","");
ANA += CreateButton("OFF",true,"nafoff","","80","10","");} 

if (NewsDataFunction && StatusNewsData != "enabled" && Display) {
ANA += CreateButton("News",false,"aapin","","80","10","");} else {
ANA += CreateButton("News",false,"aapin","","80","10","disabled");}

if (!NewsDataFunction) {StatusNewsData = F("disabled");} else {
if (NAAdjustChange) {StatusNewsData = F("enabled"); NAAdjustChange = false;}} 

if (NewsDataFunction && StatusNewsData == "enabled" && Display) {
ANA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;enabled</a>");} 
if (NewsDataFunction && StatusNewsData == "OK" && Display) {
if (NewsOutOfCredits) {  
ANA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+NewsDataLastConnectionDateTime+"</a>";} else
{ANA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+NewsDataLastConnectionDateTime+"</a>";}}  
if (NewsDataFunction && StatusNewsData == "Error" && Display) {
ANA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Error - "+NewsDataLastConnectionDateTime+"</a>";}
if (NewsDataFunction && !Display) {
ANA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ No downloading of Messages when Display OFF +++</a>");}} 
// 8. Part of the HTML string
HTMLString = F("<h2>Selection - Show Current News from ---NewsData.io--- &nbsp; ON / OFF / NEWS / STATUS:</h2>");
HTMLString += ANA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/ 

String RootHTMLString_9() { // Display www.gmcmap.com
String HTMLString = F("");
String ARA = F("");
  if (AccessGMCMap) {
// GMCMap ON / OFF
if (GMCMapFunction) {
ARA =  CreateButton("ON",true,"gmcmapon","","90","10","");
ARA += CreateButton("OFF",false,"gmcmapoff","","90","10","");} else {
ARA =  CreateButton("ON",false,"gmcmapon","","90","10","");
ARA += CreateButton("OFF",true,"gmcmapoff","","90","10","");} 

if (GMCMapFunction && StatusGMCMap != "enabled" && Display) {
ARA += CreateButton("current Value",false,"agmcmapd","","100","10","");} else {
ARA += CreateButton("current Value",false,"agmcmapd","","100","10","disabled");}

if (!GMCMapFunction) {StatusGMCMap = F("disabled");} else {
if (GMCMapAdjustChange) {StatusGMCMap = F("enabled"); GMCMapAdjustChange = false;}} 

if (GMCMapFunction && StatusGMCMap == "enabled" && Display) {
ARA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;enabled</a>");} 
if (GMCMapFunction && StatusGMCMap == "OK" && Display) {
if (GMCMapToHigh != "") {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+GMCMapLastConnectionDateTime+"</a>";
ARA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Attention: &nbsp;Danger to life !!!</a>";} else {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+GMCMapLastConnectionDateTime+"</a>";}} 
if (GMCMapFunction && StatusGMCMap == "Error" && Display) {
ARA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Error - "+GMCMapLastConnectionDateTime+"</a>";}
if (GMCMapFunction && !Display) {
ARA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ No downloading of GMCMap-Values when Display OFF +++</a>");}} 
// 9. Part of the HTML string 
HTMLString = F("<h2>Selection - Current Radioactivity Values of ---www.gmcmap.com--- &nbsp; ON / OFF / CURRENT VALUE / STATUS:</h2>");
HTMLString += ARA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_10() { // Settings for passive piezo Buzzer
String HTMLString = F("");
String BSS = F("");
String BHS = F("");
// Startsound ON / OFF
  if (StartSound == "On") { 
  BSS =  CreateButton("Startsound ON",true,"sspbson","","140","10",""); 
  BSS += CreateButton("Startsound OFF",false,"sspbsoff","","140","10","");} else {
  BSS =  CreateButton("Startsound ON",false,"sspbson","","140","10",""); 
  BSS += CreateButton("Startsound OFF",true,"sspbsoff","","140","10","");} 
  // Hoursound ON / OFF
  if (HourSound == "On") {
  BHS =  CreateButton("Hoursound ON",true,"hspbson","","140","10","");
  BHS += CreateButton("Hoursound OFF",false,"hspbsoff","","140","10","");} else {
  BHS =  CreateButton("Hoursound ON",false,"hspbson","","140","10","");
  BHS += CreateButton("Hoursound OFF",true,"hspbsoff","","140","10","");} 

// 10. Part of the HTML string   
HTMLString = F("<h2>Selection - Sound Settings:</h2>");
HTMLString += ""+BSS+BHS+"";
HTMLString += F("<p></p>"); 
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_11() { // Settings from Captiv Portal
String HTMLString = F("");
// 11. Part of the HTML string  
HTMLString = F("<h2>Display - Settings from the Access Point (Captive-Portal):</h2>");
HTMLString += F("<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 110px;\">Settings</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_12() { // Softwareversion
String HTMLString = F("");
String rtime = F("");
rtime = runtime(); // Clock Runtime
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// int ESPRAM = ESP.getMaxFreeBlockSize() / 1024;  // Check RAM
int ESPRAM = ESP.getFreeHeap() / 1024; // Check RAM
String ESPCore = ESP.getCoreVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Core Version
// String ESPCore = ESP.getFullVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Full Version
uint8_t CPUClock = ESP.getCpuFreqMHz(); // CPU-Clock
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 12. Part of the HTML string    
HTMLString = "<h7>Software-Version: &nbsp;"+LMCVersion+"</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Hardware-Version: &nbsp;"+ESPCore+"</h7>";
HTMLString += F("<a><br /></a>");
if (ESPRAM > 5) {
HTMLString += "<h7>Free Memory: &nbsp;&nbsp;"+String(ESPRAM)+" kB / 64 kB</h7>";} else {
HTMLString += "<h8>Free Memory: &nbsp;&nbsp;"+String(ESPRAM)+" kB / 64 kB (Danger ! - too little)</h8>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>CPU Clock Frequency: &nbsp;&nbsp;"+String(CPUClock)+"&nbsp; MHz</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += WiFiSignalStrength(false); // WiFi Signal Strength in dBm
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Runtime of the Clock: &nbsp;&nbsp;"+rtime+"</h7>";
HTMLString += F("<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px; height: 48px; width: 120px;color: red;float: right; font-size: 20px;\">Reset !</button></a>");
HTMLString += F("<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px; height: 48px; width: 120px;color: blue;float: right; font-size: 18px;\">Restart</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h6>*) - current Selection</h6>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
return HTMLString;}

/********************************************************************************************************************************************************/ 

void HTMLAnswer(String WebFunction) {
String HTMLString = F("");
String HTMLAnswerString = F("");  
if (WebFunction == "MatrixOn") {HTMLAnswerString = F("The LED Matrix Display was switched ON");} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = F("The LED Matrix Display is switched OFF");} 
if (WebFunction == "MatrixIsOn") {HTMLAnswerString = F("The LED Matrix Display is already switched ON");} 
if (WebFunction == "MatrixIsOff") {HTMLAnswerString = F("The LED Matrix Display is already switched OFF");}
if (WebFunction == "RebootClock") {HTMLAnswerString = F("When you Return to the Homepage<br />the LED Matrix Clock will restart");}
if (WebFunction == "DoublePointFlash") {HTMLAnswerString = F("The Doublepoint of the Clock will now flash.");} 
if (WebFunction == "DoublePointOn") {HTMLAnswerString = F("The Clock's Doublepoint is now permanently ON.");} 
if (WebFunction == "DoublePointOff") {HTMLAnswerString = F("The Clock's Doublepoint is now permanently OFF.");} 
if (WebFunction == "BrightnessLevel1") {HTMLAnswerString = F("The LED Matrix Brightness Level 1 was activated");} 
if (WebFunction == "BrightnessLevel2") {HTMLAnswerString = F("The LED Matrix Brightness Level 2 was activated");} 
if (WebFunction == "BrightnessLevel3") {HTMLAnswerString = F("The LED Matrix Brightness Level 3 was activated");} 
if (WebFunction == "BrightnessLevel4") {HTMLAnswerString = F("The LED Matrix Brightness Level 4 was activated");} 
if (WebFunction == "BrightnessLevel5") {HTMLAnswerString = F("The LED Matrix Brightness Level 5 was activated");} 
if (WebFunction == "ScrollTextTimeLevel1") {HTMLAnswerString = F("The Scrolling Text Speed Level 1 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel2") {HTMLAnswerString = F("The Scrolling Text Speed Level 2 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel3") {HTMLAnswerString = F("The Scrolling Text Speed Level 3 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel4") {HTMLAnswerString = F("The Scrolling Text Speed Level 4 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel5") {HTMLAnswerString = F("The Scrolling Text Speed Level 5 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel6") {HTMLAnswerString = F("The Scrolling Text Speed Level 6 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel7") {HTMLAnswerString = F("The Scrolling Text Speed Level 7 has been activated");} 
if (WebFunction == "ScrollTextTimeLevel8") {HTMLAnswerString = F("The Scrolling Text Speed Level 8 has been activated");} 
if (WebFunction == "WeatherFunctionsON1") {HTMLAnswerString = F("The OpenWeatherMap Weather Data function has been switched ON");}
if (WebFunction == "WeatherFunctionsON2") {HTMLAnswerString = F("The OpenWeatherMap Weather Data function is already switched ON");}
if (WebFunction == "WeatherFunctionsOFF")  {HTMLAnswerString = F("The OpenWeatherMap Weather Data function has been switched OFF");}
if (WebFunction == "NewsDataFunctionON1")  {HTMLAnswerString = F("The NewsData - Current News has been switched ON");}
if (WebFunction == "NewsDataFunctionON2")  {HTMLAnswerString = F("The NewsData - Current News is already switched ON");}
if (WebFunction == "NewsDataFunctionOFF")   {HTMLAnswerString = F("The NewsData - Current News has been switched OFF");}
if (WebFunction == "GMCMapFunctionON1")  {HTMLAnswerString = F("GMCMap.com - Current Radioactivity Levels were Switched ON");}
if (WebFunction == "GMCMapFunctionON2")  {HTMLAnswerString = F("GMCMap.com - Current Radioactivity Levels are already Switched ON");}
if (WebFunction == "GMCMapFunctionOFF")   {HTMLAnswerString = F("GMCMap.com - Current Radioactivity Levels were Switched OFF");}
if (WebFunction == "EnhancedFunctionsON1") {HTMLAnswerString = F("The additional Functions have been activated<br />(Date, Birthdays)");}
if (WebFunction == "EnhancedFunctionsON2") {HTMLAnswerString = F("The additional Functions are already switched ON<br />(Date, Birthdays)");}
if (WebFunction == "EnhancedFunctionsOFF") {HTMLAnswerString = F("The additional Functions have been switched OFF<br />(Date, Birthdays)");}
if (WebFunction == "StartSoundOnM") {HTMLAnswerString = F("The Clock Startup Sound has been turned ON.");}
if (WebFunction == "StartSoundOnMIs") {HTMLAnswerString = F("The Clock Startup Sound is already turned ON.");}
if (WebFunction == "StartSoundOffM") {HTMLAnswerString = F("The Clock Startup Sound has been turned OFF.");}
if (WebFunction == "StartSoundOffMIs") {HTMLAnswerString = F("The Clock Startup Sound is already turned OFF.");}
if (WebFunction == "HourSoundOnM") {HTMLAnswerString = F("The Clock Hour Sound has been turned ON.");}
if (WebFunction == "HourSoundOnMIs") {HTMLAnswerString = F("The Clock Hour Sound is already turned ON.");}
if (WebFunction == "HourSoundOffM") {HTMLAnswerString = F("The Clock Hour Sound has been turned OFF.");}
if (WebFunction == "HourSoundOffMIs") {HTMLAnswerString = F("The Clock Hour Sound is already turned OFF.");}
if (WebFunction == "DisplayCheck") {HTMLAnswerString = F("LED Matrix Display Test is running. Please wait ...");}
if (WebFunction == "NoDisplayCheck") {HTMLAnswerString = F("********* Display Test not currently possible. *********<br />Please wait until the Time is displayed again.");}
if (WebFunction == "ResetClockYes") {HTMLAnswerString = F("When you Return to the Homepage,<br />the Access Point (Captive Portal) is started<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--- The Settings are retained. ---");}
if (WebFunction == "POD1") {HTMLAnswerString = "The Pushover Test Message was sent to: "+PushoverDevice1;} 
if (WebFunction == "POD2") {HTMLAnswerString = "The Pushover Test Message was sent to: "+PushoverDevice2;} 
if (WebFunction == "POD2") {HTMLAnswerString = "The Pushover Test Message was sent to: "+PushoverDevice3;} 
if (WebFunction == "FPO") {HTMLAnswerString = F("The Pushover Test Message could not be sent!");} 
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>");
HTMLString +=  F("<head>");
HTMLString +=  F("<title>LED-Matrix-Clock</title>");
HTMLString +=  F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString +=  F("<style>");
HTMLString +=   F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString +=   F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString +=   F("h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString +=  F("</style>");
HTMLString +=  F("</head>");
HTMLString +=    F("<body>");
HTMLString +=    F("<h1>LED-Matrix-Clock</h1>");
HTMLString +=    F("<a><br /></a>");
HTMLString +=    "<h2>"+HTMLAnswerString+"</h2>";
HTMLString +=    F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
HTMLString +=  F("</body>");
HTMLString +=    F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");}

/********************************************************************************************************************************************************/

void ActualWeather() {
String HTMLString = F(""); 
if (!await) { 
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Weather Data from: "+OWMLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPweatherString+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualAPINews() {
String HTMLString = F("");  
if (!await) {
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>News from: "+NewsDataLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPAPINews+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualGMCMapData() {
String HTMLString = F(""); 
if (!await){   
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 30px; }");
HTMLString += F("h4 { font-size: 16px; margin-top: 0px; color: #ffcc00; margin-bottom: 30px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Last Value of: "+GMCMapLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPGMCMapValue+"</h3>";
HTMLString += "<h4>Attention: Danger at Values greater than 90 CPM!</h4>";
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Matrix On
void MatrixOn() {
if (!MEP) {  
if (!await){  
if (!Display) {
await = true;
MEP = true;    
if (SerialMonitor) {Serial.println(F("The LED Matrix was switched on."));}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // // Display on
ClockScrollIn(); // Clock scroll in
MyWaitLoop(100); // short wait
HTMLAnswer("MatrixOn");
Display = true; MEP = false; await = false;} else {HTMLAnswer("MatrixIsOn");}
CheckInHumidity(); // Indoor Humidity measurement
} else {PleaseWait();}}}

/********************************************************************************************************************************************************/

// Matrix Off
void MatrixOff() { 
if (!await){  
if (Display) { 
await = true;   
dots = 1; // Doublepoint on  
Display = false;
if (WeatherFunctions) {StatusOpenWeatherMap = F("enabled");}  
if (NewsDataFunction) {StatusNewsData = F("enabled");}   
HTMLAnswer("MatrixOff");
if (SerialMonitor) {Serial.println(F("The LED Matrix is turned off."));} 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime);
printStringWithShift("           ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,0); await = false;} else {HTMLAnswer("MatrixIsOff");} // Display off
CheckInHumidity(); // Indoor Humidity measurement
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointFlash() {
if (!await){      
DoublePointStatus = 2; // Make the Doublepoint of the clock flash
// Status Doublepoint
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointFlash");
if (SerialMonitor) {Serial.println(F("The Doublepoint of the Clock flashes."));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointOn() {
if (!await){  
DoublePointStatus = 1; // The Doublepoint of the clock is permanently ON
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOn");
if (SerialMonitor) {Serial.println(F("The Doublepoint of the Clock is permanently ON"));}
} else {PleaseWait();}}


/********************************************************************************************************************************************************/

void DoublePointOff() {
if (!await){  
DoublePointStatus = 0; // The Doublepoint of the clock is permanently OFF
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOff");
if (SerialMonitor) {Serial.println(F("The Doublepoint of the Clock is permanently OFF"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Set the brightness Levels (BrightnessLevel) of the LEDs of the Max7219 via the Web Server
void BrightnessLevel1() {SetBrightnessLevel("L1", 1);}

/********************************************************************************************************************************************************/

void BrightnessLevel2() {SetBrightnessLevel("L2", 2);}

/********************************************************************************************************************************************************/

void BrightnessLevel3() {SetBrightnessLevel("L3", 3);}

/********************************************************************************************************************************************************/

void BrightnessLevel4() {SetBrightnessLevel("L4", 4);}

/********************************************************************************************************************************************************/

void BrightnessLevel5() {SetBrightnessLevel("L5", 5);}

/********************************************************************************************************************************************************/

void SetBrightnessLevel(String BLText, int BLInt) {
if (!await){
sendCmdAll(CMD_INTENSITY, BLInt-1);
Brightness = BLText;
if (littleFSInit) {
SetupSave("matrix_brightness.txt", Brightness);}
HTMLAnswer("BrightnessLevel"+String(BLInt));
if (SerialMonitor) {Serial.println("Brightness Level "+String(BLInt)+" enabled");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Adjust the Speed of the Scrolling Text
void ScrollTextTimeLevel1() {
SetScrollTimeAndLevel(60,1);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2() { 
SetScrollTimeAndLevel(55,2); }

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel3() { 
SetScrollTimeAndLevel(50,3);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel4() {
SetScrollTimeAndLevel(45,4);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel5() {  
SetScrollTimeAndLevel(40,5);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel6() { 
SetScrollTimeAndLevel(35,6);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel7() {  
SetScrollTimeAndLevel(30,7);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel8() {
SetScrollTimeAndLevel(25,8);}

/********************************************************************************************************************************************************/

void SetScrollTimeAndLevel(int STTime, int STLevel){
if (!await) {
await = true;   
ScrollTextTime = STTime; // Scrolling Text Speed  
if (littleFSInit) {
SetupSave("status_scrolltexttime.txt", String(STTime));}
HTMLAnswer("ScrollTextTimeLevel"+String(STLevel)+"");
if (SerialMonitor) {Serial.println("Scrolling Text Speed Level "+String(STLevel)+" activated");}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);
printStringWithShift("        +++  This is a Scrolling Text Speed Test  +++           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN, 0);} else {
printStringWithShift("        +++  This is a Scrolling Text Speed Test  +++           ",ScrollTextTime);
ClockScrollIn();} // Scroll the Time in the Display 
await = false;} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOn() {
if (!await) {  
if (!WeatherFunctions) {
WeatherFunctions = true;
OWMAdjustChange = true;
StatusOpenWeatherMap = F("enabled");
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "On");} 
HTMLAnswer("WeatherFunctionsON1");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Weather Data switched on"));}}
else {
HTMLAnswer("WeatherFunctionsON2");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Weather Data is already turned on"));}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOff() {
if (!await) {   
WeatherFunctions = false;
StatusOpenWeatherMap = F("disabled");
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "Off");} 
HTMLAnswer("WeatherFunctionsOFF");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Weather Data switched off"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOn() {
if (!await) {   
if (!NewsDataFunction) {
NewsDataFunction = true;
NAAdjustChange = true;
StatusNewsData = F("enabled");
if (littleFSInit) {
SetupSave("status_NewsDatafunctions.txt", "On");}  
HTMLAnswer("NewsDataFunctionON1");
if (SerialMonitor) {Serial.println(F("NewsData - Current News switched on"));}}
else {
HTMLAnswer("NewsDataFunctionON2");
if (SerialMonitor) {Serial.println(F("NewsData - Current News is already switched on"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOff() {
if (!await) {   
NewsDataFunction = false;
StatusNewsData = F("disabled");
if (littleFSInit) {
SetupSave("status_NewsDatafunctions.txt", "Off");}  
HTMLAnswer("NewsDataFunctionOFF");
if (SerialMonitor) {Serial.println(F("NewsData - Current News is already switched off"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOn() {
if (!await) {   
if (!GMCMapFunction) {
GMCMapFunction = true;
GMCMapAdjustChange = true;
StatusGMCMap = F("enabled");
if (littleFSInit) {
SetupSave("status_GMCMapFunction.txt", "On");}  
HTMLAnswer("GMCMapFunctionON1");
if (SerialMonitor) {Serial.println(F("GMCMap - Current Display of Radioactivity Switched on"));}}
else {
HTMLAnswer("GMCMapFunctionON2");
if (SerialMonitor) {Serial.println(F("GMCMap - Current Radioactivity Display is already Switched on"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOff() {
if (!await) {   
GMCMapFunction = false;
StatusGMCMap = F("disabled");
StatusWarningLED("LOW");
if (littleFSInit) {
SetupSave("status_GMCMapFunction.txt", "Off");}  
HTMLAnswer("GMCMapFunctionOFF");
if (SerialMonitor) {Serial.println(F("GMCMap - Current Display of Radioactivity Switched off"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOn() {
if (!await) {   
if (!EnhancedFunctions) {
EnhancedFunctions = true;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "On");} 
HTMLAnswer("EnhancedFunctionsON1");} else {
HTMLAnswer("EnhancedFunctionsON2");}  
if (SerialMonitor) {Serial.println(F("Additional Functions switched on - (Date, Birthdays)"));} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOff() {
if (!await) {   
EnhancedFunctions = false;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "Off");} 
HTMLAnswer("EnhancedFunctionsOFF");
if (SerialMonitor) {Serial.println(F("Additional Functions switched off - (Date, Birthdays)"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GetAdjustments() {
String OWMAPI = F("");
String OWMCID = F("");
String NewsDataKey = F("");
String CheckIP = F(""); 
String HTMLString = F("");
if (!await) {
// Check IP-Address
if (cip.length() > 15 || cip.length() < 7) {CheckIP = F("n/a");} else {CheckIP = cip;} // Check IP-Adresse
if (AccessOpenWeatherMap){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = F("n/a");OWMCID = F("n/a");}
if (newsKey != ""){NewsDataKey = newsKey;} else {NewsDataKey = F("n/a");}
HTMLString = F("<!DOCTYPE html><html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("h3 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("h4 { font-size: 14px; margin-top: 0px; margin-bottom: 1px; color: #ffcc00; float: right;}");
HTMLString += F("h5 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "") {
HTMLString += "<h2>( Owner of the Watch:&nbsp;&nbsp;"+WatchOwner1FirstName+" "+WatchOwner1LastName+" )</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += F("<h3>Settings from the Captive Portal</h3>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;"+String(ssid)+"</h2>";
HTMLString += "<h2>IP-Address:&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += F("<a><br /></a>");
// Owner of the Watch in Detail
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "" && Gender1 != "" && Birthday1 != "") { 
HTMLString += F("<h2>1st Owner of the Watch:</h2>");   
HTMLString += "<h2>Last Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner1LastName+"</h2>";
HTMLString += "<h2>First Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner1FirstName+"</h2>";
if (Gender1 == "male") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;male";}
if (Gender1 == "female") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;female";}
HTMLString += "<h2>Date of Birth: &nbsp;"+Birthday1+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner2FirstName != "" && WatchOwner2LastName != "" && Gender2 != "" && Birthday2 != "") { 
HTMLString += F("<h2>2nd Owner of the Watch:</h2>");   
HTMLString += "<h2>Last Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner2LastName+"</h2>";
HTMLString += "<h2>First Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner2FirstName+"</h2>";
if (Gender2 == "male") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;male";}
if (Gender2 == "female") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;female";}
HTMLString += "<h2>Date of Birth: &nbsp;"+Birthday2+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner3FirstName != "" && WatchOwner3LastName != "" && Gender3 != "" && Birthday3 != "") { 
HTMLString += F("<h2>3rd Owner of the Watch:</h2>");   
HTMLString += "<h2>Last Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner3LastName+"</h2>";
HTMLString += "<h2>First Name: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner3FirstName+"</h2>";
if (Gender3 == "male") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;male";}
if (Gender3 == "female") {HTMLString += "<h2>Gender: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;female";}
HTMLString += "<h2>Date of Birth: &nbsp;"+Birthday3+"</h2>";
HTMLString += F("<a><br /></a>");}
HTMLString += "<h2>OpenWeatherMap API-Key:&nbsp;&nbsp;"+OWMAPI+"</h2>";
if (WeatherFunctions && String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
HTMLString += "<h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+" ( "+String(WeatherLocation)+" )</h2>";} else {  
HTMLString += "<h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>NewsData API-Key:&nbsp;&nbsp;"+NewsDataKey+"</h2>";
HTMLString += F("<a><br /></a>");
if (AccessGMCMap) {
HTMLString += "<h2>GMCMap.com --- History-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+GMCMapHistoryID+"</h2>";
HTMLString += "<h2>GMCMap.com --- GMT-Offset:&nbsp;&nbsp;"+GMCMapGMTOffset+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsPushover) {
HTMLString += "<h2>Pushover-SecureAppToken:  &nbsp;&nbsp;&nbsp;&nbsp;"+SecureAppToken+"</h2>";
HTMLString += "<h2>Pushover-UserToken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+UserToken+"</h2>";
HTMLString += "<h2>Pushover-Devicename1:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice1+"</h2>";
if (PushoverDevice2 != "") { 
HTMLString += "<h2>Pushover-Devicename2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice2+"</h2>";} else {
HTMLString += "<h2>Pushover-Devicename2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>";}  
if (PushoverDevice3 != "") {
HTMLString += "<h2>Pushover-Devicename3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice3+"</h2>";} else {
HTMLString += "<h2>Pushover-Devicename3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>";}  
HTMLString += "<h2>Pushover-Messagesound:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverMessageSound+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsBuzzer) {
HTMLString += F("<h2>Passive Buzzer installed</h2>");} else {
HTMLString += F("<h2>Passive Buzzer not installed or disabled</h2>");}
HTMLString += F("<a><br /></a>");
if (IsPushover) {
HTMLString += F("<h2><br /></h2>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
HTMLString += F("<h5>Selection - Send Pushover Test Message to Device:</h5>");
if (PushoverDevice1 != "" && Display) {
HTMLString += CreateButton("Device 1",false,"potmdevice1","","90","10","");} else {
HTMLString += CreateButton("Device 1",false,"potmdevice1","","90","10","disabled");} 
if (PushoverDevice2 != "" && Display) { 
HTMLString += CreateButton("Device 2",false,"potmdevice2","","90","10","");} else {
HTMLString += CreateButton("Device 2",false,"potmdevice2","","90","10","disabled");}  
if (PushoverDevice3 != "" && Display) { 
HTMLString += CreateButton("Device 3",false,"potmdevice3","","90","10","");} else {
HTMLString += CreateButton("Device 3",false,"potmdevice3","","90","10","disabled");}  
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
HTMLString += F("<a><br /></a>");}
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 1px; height: 48px; width: 160px\">back to Homepage</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
SendHTMLString(HTMLString); // Send HTMLString
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");
if (SerialMonitor) {Serial.println(F("Settings from the Access Point (Captive Portal) are called up"));}}
else {PleaseWait();}}

/********************************************************************************************************************************************************/

String CreateButton(String BMark, bool Bselect, String BHRef, String Blevel, String Bwidth, String BRmargin, String Bdisable) {
String HTMLString = F("");
if (Bselect) {
HTMLString  = "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; color: blue; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"*</button></a>";} else {
HTMLString += "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"</button></a>";}
return HTMLString;}

/********************************************************************************************************************************************************/

void ResetClockSettings() {
if (!await) {   
String HTMLString = F("");
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>\
  <head>\
  <title>LED-Matrix-Clock</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Clock</h1>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;LED-Matrix-Clock Reset !</h2>\
     <h2>(The WLAN Access Data will be deleted)</h2>\
     <a><br /></a>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Now Reset the LED Matrix Clock ?</h2>\
     <a><br /></a>\
     <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 70px;height: 48px; width: 110px; font-size: 22px; color: red\">Yes</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">No</button></a>\
  </body>\
    </html>");  
SendHTMLString(HTMLString); // Send HTMLString
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ResetClockYes() { 
/*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
if (!await) { 
HTMLAnswer("ResetClockYes");
ResetNow = true; // Reset - Default Settings 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void RebootClock()
{
if (!await) {   
HTMLAnswer("RebootClock");
RebootNow = true;
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void SiteNotFound(){
String HTMLString = F("");
HTMLString = F("<!DOCTYPE html> <html lang='en-GB'>\
  <head>\
  <title>LED-Matrix-Clock</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Clock</h1>\
    <a><br /></a>\
    <h2>This Page was not found !</h2>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>\
  </body>\
    </html>"); 
SendHTMLString(HTMLString); // Send HTMLString
CheckInHumidity(); // Indoor Humidity measurement
HTMLString = F("");}

/********************************************************************************************************************************************************/

void ChangeDisplayAutoOnOff() {
String HTMLString;
bool MDTime;
if (!await) {
if (server.method() == HTTP_POST) {
    // Einstellungen speichern
    DisplayOnTime = server.arg("DOnT"); // Display On-Time
    DisplayOffTime = server.arg("DOffT"); // Display Off-Time
    DisplayOnTime.trim();
    DisplayOffTime.trim();

    // Check the plausibility of the information for Matrix Display Automatic On/Off
    if (DisplayOnTime != "") {
    MDTime = false;
    if (DisplayOnTime.length() != 5) {
    if (DisplayOnTime.length() == 4) {
    if (DisplayOnTime.indexOf(':') == 1) {
    DisplayOnTime = "0"+DisplayOnTime;
    } else {DisplayOnTime = F("!");}
    } else {DisplayOnTime = F("!");}} else {
    if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = F("!");}}
    if (DisplayOnTime != "!") {
    int DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = CheckMDHourOn.toInt();} else {DisplayOnTime = F("!");}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = CheckMDMinOn.toInt();} else {DisplayOnTime = F("!");}
    if (DisplayOnTime != "" && DisplayOnTime != "!" ) {
    for (int i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");}}}}}

    if (DisplayOffTime != "") {
    MDTime = false;
    if (DisplayOffTime.length() != 5) {
    if (DisplayOffTime.length() == 4) {
    if (DisplayOffTime.indexOf(':') == 1) {
    DisplayOffTime = "0"+DisplayOffTime;
    } else {DisplayOffTime = F("!");}
    } else {DisplayOffTime = F("!");}} else {
    if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = F("!");}}
    if (DisplayOffTime != "!") {
    int DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = CheckMDHourOff.toInt();} else {DisplayOffTime = F("!");}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = CheckMDMinOff.toInt();} else {DisplayOffTime = F("!");}
    if (DisplayOffTime != "" && DisplayOffTime != "!") {
    for (int i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");}}}}}

    if (DisplayOnTime == "" && DisplayOffTime == "") {
    DisplayOnTime = F("NullOn");
    DisplayOffTime = F("NullOff");}

    FDOnTime =  DisplayOnTime; // for Display only
    FDOffTime = DisplayOffTime; // for Display only

    // If Input Error
    if (DisplayOnTime == "!" || DisplayOffTime == "!" || DisplayOnTime == DisplayOffTime) {
    if (DisplayOnTime == "!") {DisplayOnTime = F("");} 
    if (DisplayOffTime == "!") {DisplayOffTime = F("");}  
    HTMLString = F("<!doctype html><html lang='en-GB'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically ON/OFF)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Invalid Entry(s) - Please correct!</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/cdof\"><button>back to Homepage</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    } else {
    if (DisplayOnTime == "NullOn") {DisplayOnTime = F("");}
    if (DisplayOffTime == "NullOff") {DisplayOffTime = F("");}    
    if (littleFSInit){
    // *** Save entries in the file system ***
    // Matrix Display ON time
    if (DisplayOnTime != ""){
    SetupSave("display-on-time.txt", DisplayOnTime);} else {
    if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
    // Matrix Display OFF time
    if (DisplayOffTime != ""){
    SetupSave("display-off-time.txt", DisplayOffTime);} else {
    if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
    SetupSave("display-change-time.txt", "True"); // Display On/Off Daten wurden geändert
    MyWaitLoop(500); // 0,5 sek. wait 
    if (SerialMonitor) {Serial.println(F("Display automatically ON/OFF Data saved... "));} 
      
    HTMLString = F("<!doctype html><html lang='en-GB'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically ON/OFF)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>The Settings have been saved<br /><br />The Clock will restart when you return to the Homepage</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>back to Homepage</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    SendHTMLString(HTMLString); // Send HTMLString
    // Trigger reboot when back to homepage
    RebootNow = true;
    }
     else
     { // Access data was not saved
     if (SerialMonitor) {Serial.println(F("Access Data and Settings were not saved !"));}
    HTMLString =  F("<!doctype html><html lang='en-GB'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");   
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically ON/OFF)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Error saving Access Data and settings<br />The Clock is restarted</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>back to Homepage</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Send HTMLString
    // Restart trigger
    if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
    printStringWithShift("        +++       The Clock will restart      +++           ",ScrollTextTime);
    StatusWarningLED("LOW"); // LED off
    ESP.restart();  
    } 
    SendHTMLString(HTMLString); // Send HTMLString 
    CheckInHumidity(); // Indoor Humidity measurement
    } // Ende Inputerror
  } else {  
    // Settings
    HTMLString =  F("<!doctype html><html lang='en-GB'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/cdof' method='post'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically ON/OFF)</h5>");
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display ON-Time (Example: 07:00)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOnT' id='DOnT' value='"+FDOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display OFF-Time (Example: 23:30)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOffT' id='DOffT' value='"+FDOffTime+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += F("<h6>*) - No Information deactivates the Function</h6>");
    HTMLString += F("<button type='submit'>Save</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/cdofc\"><button>Cancel</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);}} else {PleaseWait();}}  // Send HTMLString or wait

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DisplayCheck() {
if (!await) {
await = true; 
char  MDC   = '9' + 2; // Full Display ON
String MDCShow = "      "+String(MDC)+String(MDC)+String(MDC)+String(MDC);
HTMLAnswer("DisplayCheck");
printStringWithShift3(MDCShow.c_str(),ScrollTextTime);
HandleOTAandWebServer();  // OTA and Web Server query
StatusWarningLED("HIGH"); // Check LED
MyWaitLoop(8000); // Short wait (8 seconds)
HandleOTAandWebServer();  // OTA and Web Server query
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // OTA and Web Server query
InHumidityStatus = CheckInHumidity(); // Indoor Humidity measurement
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") {
StatusWarningLED("LOW");}} else {StatusWarningLED("LOW");}
ClockScrollIn(); // Scroll the time in the Display 
await = false; 
} else {HTMLAnswer("NoDisplayCheck");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ChangeDisplayAutoOnOffCancel() {
String HTMLString = F("");
  // Display switch-on time as a string
  if (littleFSInit) {
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Remove spaces at the front and back
  } else {DisplayOnTime = F("");}} else
         {DisplayOnTime = F("");}
  // Display Ausschaltzeit als String
  if (littleFSInit) {
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Remove spaces at the front and back
  } else {DisplayOffTime = F("");}} else
         {DisplayOffTime = F("");}

  // Check the plausibility of the information for Matrix Display Automatic On/Off
  CheckLEDMatrixDisplayOnOff();

    HTMLString = F("<!doctype html><html lang='en-GB'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically ON/OFF)</h5>");
    HTMLString += F("<br/>");
    if (littleFSInit) {
    HTMLString += F("<h6>Cancellation! - The old Setting has been loaded again ...</h6>");} else {
    HTMLString += F("<h6>Cancellation! - The old Setting could not be loaded !!! ...</h6>");}
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>back to Homepage</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    CheckInHumidity(); // Indoor Humidity measurement
 HTMLString = F("");}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOn() {
if (StartSound != "On") { 
if (!await) {  
await = true;
StartBuzzerBeep();    
if (littleFSInit) {
StartSound = F("On");   
SetupSave("status_startsound.txt", StartSound);} else {StartSound = F("Off");}
HTMLAnswer("StartSoundOnM");
if (SerialMonitor) {Serial.println(F("Buzzer Startsound switched on"));}
await = false;} else {PleaseWait();}} else {HTMLAnswer("StartSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOff() {
if (StartSound != "Off") {  
StartSound = F("Off");     
if (littleFSInit) {  
SetupSave("status_startsound.txt", StartSound);} 
HTMLAnswer("StartSoundOffM");
if (SerialMonitor) {Serial.println(F("Buzzer Startsound switched off"));}
} else {HTMLAnswer("StartSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOn() {
if (HourSound != "On") { 
if (!await) {  
await = true;
HourBuzzerBeep();   
if (littleFSInit) {
HourSound = F("On");   
SetupSave("status_hoursound.txt", HourSound);} else {HourSound = F("Off");}
HTMLAnswer("HourSoundOnM");
await = false;
if (SerialMonitor) {Serial.println(F("Buzzer Hoursound switched on"));}} else {PleaseWait();}
} else {HTMLAnswer("HourSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOff() {
if (HourSound != "Off") {
HourSound = F("Off");  
if (littleFSInit) {
SetupSave("status_hoursound.txt", HourSound);} 
HTMLAnswer("HourSoundOffM");
if (SerialMonitor) {Serial.println(F("Buzzer Hoursound switched off"));}
} else {HTMLAnswer("HourSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PleaseWait() {
// Please wait, show page
String RefreshTime = F("5"); // Refresh the website after X seconds
String HTMLString;
HTMLString = F("<!DOCTYPE html><html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
// HTMLString += F("<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 { font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 { font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 { font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Please wait ...</h2>");
if (Display) {
HTMLString += F("<h3>(Until the time is shown again in the Matrix Display)</h3>");}
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString); 
CheckInHumidity(); // Indoor Humidity measurement 
Serial.println(F("Please wait ..."));}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GoToWebSiteHomePage() { // Automatic Redirection to the HomePage
String HTMLString;
HTMLString = F("<!DOCTYPE html><html lang='en-GB'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Clock</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 { font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 { font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 { font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("<script>");
// Wait Time (ms) until you Return to the Homepage | default = 1000
HTMLString += F("window.onload = function() {setTimeout(function() {window.location.href = \"/\";}, 2000);}"); 
HTMLString += F("</script>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Clock</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Please wait ...</h2>");
if (Display) {
HTMLString += F("<h3>(Until the time is shown again in the Matrix Display)</h3>");}
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString);}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HTTPCodeText(int httpcode) {
String TempHTTPCodeText = F("k.A.");  
if (httpcode == -1) {TempHTTPCodeText = F("ERROR_CONNECTION_REFUSED");}  
if (httpcode == -2) {TempHTTPCodeText = F("ERROR_SEND_HEADER_FAILED");} 
if (httpcode == -3) {TempHTTPCodeText = F("ERROR_SEND_PAYLOAD_FAILED");} 
if (httpcode == -4) {TempHTTPCodeText = F("ERROR_NOT_CONNECTED");} 
if (httpcode == -5) {TempHTTPCodeText = F("ERROR_CONNECTION_LOST");} 
if (httpcode == -6) {TempHTTPCodeText = F("ERROR_NO_STREAM");} 
if (httpcode == -7) {TempHTTPCodeText = F("ERROR_NO_HTTP_SERVER");} 
if (httpcode == -8) {TempHTTPCodeText = F("ERROR_TOO_LESS_RAM");} 
if (httpcode == -9) {TempHTTPCodeText = F("ERROR_ENCODING");} 
if (httpcode == -10) {TempHTTPCodeText = F("ERROR_STREAM_WRITE");} 
if (httpcode == -11) {TempHTTPCodeText = F("ERROR_READ_TIMEOUT");}

if (httpcode == 100) {TempHTTPCodeText = F("Continue");}
if (httpcode == 101) {TempHTTPCodeText = F("Switching Protocols");}
if (httpcode == 102) {TempHTTPCodeText = F("Processing (WebDAV)");}
if (httpcode == 200) {TempHTTPCodeText = F("OK");}
if (httpcode == 201) {TempHTTPCodeText = F("Created");}
if (httpcode == 202) {TempHTTPCodeText = F("Accepted");}
if (httpcode == 203) {TempHTTPCodeText = F("Non-Authoritative Information");}
if (httpcode == 204) {TempHTTPCodeText = F("No Content");}
if (httpcode == 205) {TempHTTPCodeText = F("Reset Content");}
if (httpcode == 206) {TempHTTPCodeText = F("Partial Content");}
if (httpcode == 207) {TempHTTPCodeText = F("Multi-Status (WebDAV)");}
if (httpcode == 208) {TempHTTPCodeText = F("Already Reported (WebDAV)");}
if (httpcode == 226) {TempHTTPCodeText = F("IM Used");}

if (httpcode == 300) {TempHTTPCodeText = F("Multiple Choices");}
if (httpcode == 301) {TempHTTPCodeText = F("Moved Permanently");}
if (httpcode == 302) {TempHTTPCodeText = F("Found");}
if (httpcode == 303) {TempHTTPCodeText = F("See Other");}
if (httpcode == 304) {TempHTTPCodeText = F("Not Modified");}
if (httpcode == 305) {TempHTTPCodeText = F("Use Proxy");}
if (httpcode == 306) {TempHTTPCodeText = F("(Unused)");}
if (httpcode == 307) {TempHTTPCodeText = F("Temporary Redirect");}
if (httpcode == 308) {TempHTTPCodeText = F("Permanent Redirect (experimental)");}

if (httpcode == 400) {TempHTTPCodeText = F("Bad Request");}
if (httpcode == 401) {TempHTTPCodeText = F("Unauthorized");}
if (httpcode == 402) {TempHTTPCodeText = F("Payment Required");}
if (httpcode == 403) {TempHTTPCodeText = F("Forbidden");}
if (httpcode == 404) {TempHTTPCodeText = F("Not Found");}
if (httpcode == 405) {TempHTTPCodeText = F("Method Not Allowed");}
if (httpcode == 406) {TempHTTPCodeText = F("Not Acceptable");}
if (httpcode == 407) {TempHTTPCodeText = F("Proxy Authentication Required");}
if (httpcode == 408) {TempHTTPCodeText = F("Request Timeout");}
if (httpcode == 409) {TempHTTPCodeText = F("Conflict");}
if (httpcode == 410) {TempHTTPCodeText = F("Gone");}
if (httpcode == 411) {TempHTTPCodeText = F("Length Required");}
if (httpcode == 412) {TempHTTPCodeText = F("Precondition Failed");}
if (httpcode == 413) {TempHTTPCodeText = F("Request Entity Too Large");}
if (httpcode == 414) {TempHTTPCodeText = F("Request-URI Too Long");}
if (httpcode == 415) {TempHTTPCodeText = F("Unsupported Media Type");}
if (httpcode == 416) {TempHTTPCodeText = F("Requested Range Not Satisfiable");}
if (httpcode == 417) {TempHTTPCodeText = F("Expectation Failed");}
if (httpcode == 418) {TempHTTPCodeText = F("I'm a teapot (RFC 2324)");}
if (httpcode == 420) {TempHTTPCodeText = F("Enhance Your Calm (Twitter)");}
if (httpcode == 422) {TempHTTPCodeText = F("Unprocessable Entity (WebDAV)");}
if (httpcode == 423) {TempHTTPCodeText = F("Locked (WebDAV)");}
if (httpcode == 424) {TempHTTPCodeText = F("Failed Dependency (WebDAV)");}
if (httpcode == 425) {TempHTTPCodeText = F("Reserved for WebDAV");}
if (httpcode == 426) {TempHTTPCodeText = F("Upgrade Required");}
if (httpcode == 428) {TempHTTPCodeText = F("Precondition Required");}
if (httpcode == 429) {TempHTTPCodeText = F("Too Many Requests");}
if (httpcode == 431) {TempHTTPCodeText = F("Request Header Fields Too Large");}
if (httpcode == 444) {TempHTTPCodeText = F("No Response (Nginx)");}
if (httpcode == 449) {TempHTTPCodeText = F("Retry With (Microsoft)");}
if (httpcode == 450) {TempHTTPCodeText = F("Blocked by Windows Parental Controls (Microsoft)");}
if (httpcode == 451) {TempHTTPCodeText = F("Unavailable For Legal Reasons");}
if (httpcode == 499) {TempHTTPCodeText = F("Client Closed Request (Nginx)");}

if (httpcode == 500) {TempHTTPCodeText = F("Internal Server Error");}
if (httpcode == 501) {TempHTTPCodeText = F("Not Implemented");}
if (httpcode == 502) {TempHTTPCodeText = F("Bad Gateway");}
if (httpcode == 503) {TempHTTPCodeText = F("Service Unavailable");}
if (httpcode == 504) {TempHTTPCodeText = F("Gateway Timeout");}
if (httpcode == 505) {TempHTTPCodeText = F("HTTP Version Not Supported");}
if (httpcode == 506) {TempHTTPCodeText = F("Variant Also Negotiates (Experimental)");}
if (httpcode == 507) {TempHTTPCodeText = F("Insufficient Storage (WebDAV)");}
if (httpcode == 508) {TempHTTPCodeText = F("Loop Detected (WebDAV)");}
if (httpcode == 509) {TempHTTPCodeText = F("Bandwidth Limit Exceeded (Apache)");}
if (httpcode == 510) {TempHTTPCodeText = F("Not Extended");}
if (httpcode == 511) {TempHTTPCodeText = F("Network Authentication Required");}
if (httpcode == 598) {TempHTTPCodeText = F("Network read timeout error");}
if (httpcode == 599) {TempHTTPCodeText = F("Network connect timeout error");}
return String(httpcode) + " - " + TempHTTPCodeText;}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
