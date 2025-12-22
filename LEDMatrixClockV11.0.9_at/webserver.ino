// Webserver Startseite
void handleRoot() { 
// Daten senden
if (!MEP){
IsEnoughRAM(MinRAMMem,"Website"); // Check RAM
if (!OneTimeCall) {if (ESPRAM < MinRAMMem-640) {BadRAM();}}
server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
server.sendContent(RootHTMLString_1());  // 1. Teil
server.sendContent(RootHTMLString_2());  // 2. Teil
server.sendContent(RootHTMLString_3());  // 3. Teil
server.sendContent(RootHTMLString_4());  // 4. Teil
server.sendContent(RootHTMLString_5());  // 5. Teil
server.sendContent(RootHTMLString_6());  // 6. Teil
if (AccessOpenWeatherMap) {server.sendContent(RootHTMLString_7());}  // 7. Teil
if (AccessNewsData) {server.sendContent(RootHTMLString_8());}  // 8. Teil
if (AccessGMCMap) {server.sendContent(RootHTMLString_9());} // 9. Teil
if (IsBuzzer) {server.sendContent(RootHTMLString_10());} // 10. Teil nur für Passiv Piezo Summer (Buzzer)
server.sendContent(RootHTMLString_11()); // 11. Teil
server.sendContent(RootHTMLString_12()); // 12. Teil
server.client().stop(); 
freeRam();
if (RebootNow) {
if (!Display) {
printStringWithShift("           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Display an   
} // Display einschalten wenn Aus  
printStringWithShift("                  +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime); 
StatusWarningLED("LOW"); // LED Ausschalten 
ESP.restart();} // Neustart der Uhr
if (littleFSInit && ResetNow) {ResetClock();}} else {PleaseWait();}} // Einstellungen auf Standard - WLAN Zugangsdaten werden gelöscht
 
/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"60\">\ = Refresh alle 60 sekunden*/ 
String RefreshTime = F("60"); // Aktualisierung der Website nach X Sekunden
  String HTMLString; HTMLString.reserve(180);
   String InTempOnOff1;
    String InTempOnOff2;

    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen

  // Innentemperatur anzeigen JA/NEIN (DHT11 oder DHT22)
  if (InTempCounter < 5) {
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Temperatur Wert abrufen
  if (InTempValue !="!") {
  IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10);
  if (IntInTempValue < 10) {
  InTempOnOff1 = F("id= 'Bvalue'>Temperatur an der Uhr:");  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {InTempOnOff1 = F("id= 'Gvalue'>Temperatur an der Uhr:");} else {  
  InTempOnOff1 =  F("id= 'Rvalue'>Temperatur an der Uhr:");}}
  if (InHumMessage == "") { 
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Innentemperatur Anzeigen</button></a>");} else {
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Innentemperatur Anzeigen</button></a>"); 
  InTempOnOff2 += "<h9>"+InHumMessage+"</h9>";}
  InTempOnOff2 += F("<p></p>");
  InTempOnOff2 += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  } else {InTempOnOff1.clear();
          InTempOnOff1.clear();}
  } else {InTempOnOff1.clear();
          InTempOnOff2.clear();
          InTempValue = F("!");}

  // Check HTML Teilstring
 //  if (SerialMonitor) {DEBUG_PRINTLN("<a"+InTempOnOff2+"</a>\");} // Only for Debuging 
// 1. Teil des HTML-Strings
  HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>"); 
  HTMLString += F("<head>");
  HTMLString += F("<title>LED-Matrix-Uhr</title>");
  HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body {background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
  HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}");
  HTMLString += F("h2 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h3 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h4 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h5 {color: white;font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h6 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h7 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffffff;}");
  HTMLString += F("h8 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #f74f4f; font-weight: bold;}");
  HTMLString += F("h9 {font-size: 18px; margin-left: 80px; margin-top: 0px; margin-bottom: 0px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("h10 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}");
  HTMLString += F("h11 {font-size: 14px; margin-left: 10px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("#Rvalue {color: #FC3A30;}");
  HTMLString += F("#Gvalue {color: lime;}");
  HTMLString += F("#Bvalue {color: #2ECCFA;}");
  HTMLString += F("</style>");
  HTMLString += F("</head>");
  HTMLString += F("<body>");
  HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += InTempOnOff2;
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){ 
// Auswahl Helligkeit der Matrix und Runtime der Uhr
  String HTMLString; HTMLString.reserve(180);
  // 2. Teil des HTML-Strings
  HTMLString = F("<h2>Auswahl - Display EIN / AUS / TEST / Helligkeitsstufen:</h2>");
  if (!Display) { 
  HTMLString += CreateButton("Display EIN",false,"mon","","110","10",""); 
  HTMLString += CreateButton("Display AUS",true,"moff","","110","20","");
  HTMLString += CreateButton("Display TEST",false,"","","110","20","disabled");} else {
  HTMLString += CreateButton("Display EIN",true,"mon","","110","10","");  
  HTMLString += CreateButton("Display AUS",false,"moff","","110","20","");
  HTMLString += CreateButton("Display TEST",false,"mdt","","110","20","");}

  if (!Display) {  
  if (Brightness == "L1") {HTMLString += CreateButton("Stufe",true,""," 1","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 1","90","10","disabled");}
  if (Brightness == "L2") {HTMLString += CreateButton("Stufe",true,""," 2","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 2","90","10","disabled");}
  if (Brightness == "L3") {HTMLString += CreateButton("Stufe",true,""," 3","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 3","90","10","disabled");}
  if (Brightness == "L4") {HTMLString += CreateButton("Stufe",true,""," 4","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 4","90","10","disabled");}
  if (Brightness == "L5") {HTMLString += CreateButton("Stufe",true,""," 5","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 5","90","10","disabled");}} 
  else {
  if (Brightness == "L1") {HTMLString += CreateButton("Stufe",true,"mb1"," 1","90","10","");} else {HTMLString += CreateButton("Stufe",false,"mb1"," 1","90","10","");}
  if (Brightness == "L2") {HTMLString += CreateButton("Stufe",true,"mb2"," 2","90","10","");} else {HTMLString += CreateButton("Stufe",false,"mb2"," 2","90","10","");}
  if (Brightness == "L3") {HTMLString += CreateButton("Stufe",true,"mb3"," 3","90","10","");} else {HTMLString += CreateButton("Stufe",false,"mb3"," 3","90","10","");}
  if (Brightness == "L4") {HTMLString += CreateButton("Stufe",true,"mb4"," 4","90","10","");} else {HTMLString += CreateButton("Stufe",false,"mb4"," 4","90","10","");}
  if (Brightness == "L5") {HTMLString += CreateButton("Stufe",true,"mb5"," 5","90","10","");} else {HTMLString += CreateButton("Stufe",false,"mb5"," 5","90","10","");}}  
  HTMLString += F("<p></p>");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_3(){
String HTMLString; HTMLString.reserve(180);
String DisplayAutomaticOnOff;  
// Display automatisch EIN/AUS wenn in Access Point angegeben
  if (DisplayOnTime != "" && DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr<br />Display automatisch AUS um "+FDOffTime+" Uhr";} else {
  if (DisplayOnTime == "" && DisplayOffTime == ""){
  DisplayAutomaticOnOff = F("Display automatisch EIN / AUS ist deaktiviert");} else {  
  if (DisplayOnTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr";} else {
  if (DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch AUS um "+FDOffTime+" Uhr";}}}}
// 3. Teil des HTML-Strings
HTMLString ="<h4>"+DisplayAutomaticOnOff+"</h4>";
HTMLString += F("<a href =\"/cdof\"><button class=\"button\" style=\"margin-right: 10px; height: 24px; font-size: 14px; font-weight: bold; width: 80px;\">Ändern</button></a>");
if (!MatrixAction && Display) {HTMLString += F("<h11>[ Bis zum automatischen ausschalten des Display's wird nur noch die Uhrzeit angezeigt ! ]</h11>");}
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}  

/********************************************************************************************************************************************************/

String RootHTMLString_4(){ // Status Doppelpunkt
  String HTMLString; HTMLString.reserve(180);
  // 4. Teil des HTML-Strings
  HTMLString = F("<h2>Auswahl - Status Doppelpunkt der Uhrzeit:</h2>");
  if (!Display) {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Blinken",true,"","","90","10","disabled");} else {
  HTMLString += CreateButton("Blinken",false,"","","90","10","disabled");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Permanent EIN",true,"","","130","10","disabled");} else {
  HTMLString += CreateButton("Permanent EIN",false,"","","130","10","disabled");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Permanent AUS",true,"","","130","10","disabled");} else {
  HTMLString += CreateButton("Permanent AUS",false,"","","130","10","disabled");}  
  } else {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Blinken",true,"sdflash","","90","10","");} else {
  HTMLString += CreateButton("Blinken",false,"sdflash","","90","10","");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Permanent EIN",true,"sdon","","130","10","");} else {
  HTMLString += CreateButton("Permanent EIN",false,"sdon","","130","10","");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Permanent AUS",true,"sdoff","","130","10","");} else {
  HTMLString += CreateButton("Permanent AUS",false,"sdoff","","130","10","");}}
  HTMLString += F("<p></p>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_5(){ // ScrollText Geschwindigkeitslevel
String HTMLString; HTMLString.reserve(180);
  // 5. Teil des HTML-Strings  
  HTMLString = F("<h2>Auswahl - Lauftextgeschwindigkeit:</h2>"); 
  if (!Display) { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Stufe",true,""," 1","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 1","90","10","disabled");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Stufe",true,""," 2","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 2","90","10","disabled");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Stufe",true,""," 3","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 3","90","10","disabled");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Stufe",true,""," 4","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 4","90","10","disabled");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Stufe",true,""," 5","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 5","90","10","disabled");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Stufe",true,""," 6","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 6","90","10","disabled");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Stufe",true,""," 7","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 7","90","10","disabled");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Stufe",true,""," 8","90","10","disabled");} else {HTMLString += CreateButton("Stufe",false,""," 8","90","10","disabled");} 
  } else { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Stufe",true,"vlt1"," 1","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt1"," 1","90","10","");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Stufe",true,"vlt2"," 2","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt2"," 2","90","10","");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Stufe",true,"vlt3"," 3","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt3"," 3","90","10","");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Stufe",true,"vlt4"," 4","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt4"," 4","90","10","");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Stufe",true,"vlt5"," 5","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt5"," 5","90","10","");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Stufe",true,"vlt6"," 6","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt6"," 6","90","10","");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Stufe",true,"vlt7"," 7","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt7"," 7","90","10","");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Stufe",true,"vlt8"," 8","90","10","");} else {HTMLString += CreateButton("Stufe",false,"vlt8"," 8","90","10","");}}
  HTMLString += F("<p></p>"); 
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_6(){ // Zusatzfunktionen
String HTMLString; HTMLString.reserve(180);
String EF;
if (EnhancedFunctions) {
EF =  CreateButton("EIN",true,"zfon","","90","10","");
EF += CreateButton("AUS",false,"zfoff","","90","10","");} else {
EF =  CreateButton("EIN",false,"zfon","","90","10","");
EF += CreateButton("AUS",true,"zfoff","","90","10","");}
// 6. Teil des HTML-Strings
if (IsFederalStateOK(FederalState)) { 
HTMLString = F("<h2>Auswahl - Zusatzfunktionen &nbsp;EIN / AUS: &nbsp;(Datum, Feiertage, Geburtstage)</h2>");} else {
HTMLString = F("<h2>Auswahl - Zusatzfunktionen &nbsp;EIN / AUS: &nbsp;(Datum, Geburtstage)</h2>");}  
HTMLString += EF; // Zusatzfunktionen (Datum, Feiertage, Geburtstage) - EIN/AUS
if (EnhancedFunctions && GreetingsToday) {
if (GT1 != "" && HT1 != "") {  // GT1 = Geburtstage und andere Grüße   HT1 = Feiertage
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Heute: &nbsp;&nbsp;"+HT1+"&nbsp; / &nbsp;"+GT1+"</a>";} else {  
if (GT1 != "") {    
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Heute: &nbsp;&nbsp;"+GT1+"</a>";}
if (HT1 != "") {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Heute: &nbsp;&nbsp;"+HT1+"</a>";}}} else { // Zusatzfunktionen
if (EnhancedFunctions) {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">&nbsp;&nbsp;"+DateString+"</a>";} else {
HTMLString += F("<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\"></a>");}}
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_7() { // Anzeige Wetterdaten von OpenWeatherMap
// Wetterdaten aktivieren / deaktivieren
String HTMLString; HTMLString.reserve(180);
String OWMA;
// OpenWeatherMap Status anzeigen oder nicht
if (AccessOpenWeatherMap) {
if (WeatherFunctions) {
OWMA =  CreateButton("EIN",true,"owmfon","","90","10","");
OWMA += CreateButton("AUS",false,"owmfoff","","90","10","");} else {
OWMA =  CreateButton("EIN",false,"owmfon","","90","10","");  
OWMA += CreateButton("AUS",true,"owmfoff","","90","10","");}

if (WeatherFunctions && StatusOpenWeatherMap != "aktiviert" && Display) {
OWMA += CreateButton("Wetter",false,"aowmn","","100","10","");} else {
OWMA += CreateButton("Wetter",false,"aowmn","","100","10","disabled");}

if (!WeatherFunctions) {StatusOpenWeatherMap = F("deaktiviert");} else {
if (OWMAdjustChange) {StatusOpenWeatherMap = F("aktiviert"); OWMAdjustChange = false;}}
if (WeatherFunctions && StatusOpenWeatherMap == "aktiviert" && Display) {
OWMA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;aktiviert</a>");} 
if (WeatherFunctions && StatusOpenWeatherMap == "OK" && Display) {
String WSTemp = String(temp, 0); WSTemp.trim();
if (IsNumeric(WSTemp) && IsNumeric(String(humidity))) { // Check Temperatur und Luftfeuchte 
OWMA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+OWMLastConnectionDateTime+"";
OWMA += "&nbsp;&nbsp;&nbsp;&nbsp;(AT=&nbsp;"+WSTemp+"°C &nbsp;/&nbsp; AL=&nbsp;"+String(humidity)+"%)</a>";} else {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+OWMLastConnectionDateTime+"</a>";}} 
if (WeatherFunctions && StatusOpenWeatherMap == "Fehler" && Display) {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+OWMLastConnectionDateTime+"</a>";}
if (WeatherFunctions && !Display) {
OWMA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ kein Herunterladen von Wetterdaten wenn Display AUS +++</a>");}}
// 7. Teil des HTML-Strings  
HTMLString =  F("<h2>Auswahl - aktuelle Wetterdaten von ---OpenWeatherMAP.org--- anzeigen &nbsp; EIN / AUS / WETTER / STATUS:</h2>");
HTMLString += OWMA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_8() { // Anzeige NewsData.io Nachrichten
String HTMLString; HTMLString.reserve(180);
String ANA;
if (AccessNewsData) {
// NewsData Einschalten / Ausschalten
if (NewsDataFunction) {
ANA =  CreateButton("EIN",true,"nafon","","90","10","");
ANA += CreateButton("AUS",false,"nafoff","","90","10","");} else {
ANA =  CreateButton("EIN",false,"nafon","","90","10","");
ANA += CreateButton("AUS",true,"nafoff","","90","10","");} 

if (NewsDataFunction && StatusNewsData != "aktiviert" && Display) {
ANA += CreateButton("Nachricht",false,"aapin","","100","10","");} else {
ANA += CreateButton("Nachricht",false,"aapin","","100","10","disabled");}

if (!NewsDataFunction) {StatusNewsData = F("deaktiviert");} else {
if (NAAdjustChange) {StatusNewsData = F("aktiviert"); NAAdjustChange = false;}} 

if (NewsDataFunction && StatusNewsData == "aktiviert" && Display) {
ANA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;aktiviert</a>");} 
if (NewsDataFunction && StatusNewsData == "OK" && Display) {
if (NewsOutOfCredits) {  
ANA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+NewsDataLastConnectionDateTime+"</a>";} else
{ANA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+NewsDataLastConnectionDateTime+"</a>";}} 
if (NewsDataFunction && StatusNewsData == "Fehler" && Display) {
ANA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+NewsDataLastConnectionDateTime+"</a>";}
if (NewsDataFunction && !Display) {
ANA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ kein Herunterladen von Nachrichten wenn Display AUS +++</a>");}}
// 8. Teil des HTML-Strings 
HTMLString = F("<h2>Auswahl - aktuelle Nachrichten von ---NewsData.io--- anzeigen &nbsp; EIN / AUS / NACHRICHT / STATUS:</h2>");
HTMLString += ANA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_9() { // Anzeige www.gmcmap.com
String HTMLString; HTMLString.reserve(180);
String ARA;
  if (AccessGMCMap) {
// GMCMap Einschalten / Ausschalten
if (GMCMapFunction) {
ARA =  CreateButton("EIN",true,"gmcmapon","","90","10","");
ARA += CreateButton("AUS",false,"gmcmapoff","","90","10","");} else {
ARA =  CreateButton("EIN",false,"gmcmapon","","90","10","");
ARA += CreateButton("AUS",true,"gmcmapoff","","90","10","");} 

if (GMCMapFunction && StatusGMCMap != "aktiviert" && Display) {
ARA += CreateButton("aktueller Wert",false,"agmcmapd","","100","10","");} else {
ARA += CreateButton("aktueller Wert",false,"agmcmapd","","100","10","disabled");}

if (!GMCMapFunction) {StatusGMCMap = F("deaktiviert");} else {
if (GMCMapAdjustChange) {StatusGMCMap = F("aktiviert"); GMCMapAdjustChange = false;}} 

if (GMCMapFunction && StatusGMCMap == "aktiviert" && Display) {
ARA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;aktiviert</a>");} 
if (GMCMapFunction && StatusGMCMap == "OK" && Display) {
if (GMCMapToHigh != "") {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+GMCMapLastConnectionDateTime+"</a>";
ARA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Achtung: &nbsp;Gefahr !!!</a>";} else {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+GMCMapLastConnectionDateTime+"</a>";}} 
if (GMCMapFunction && StatusGMCMap == "Fehler" && Display) {
ARA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+GMCMapLastConnectionDateTime+"</a>";}
if (GMCMapFunction && !Display) {
ARA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ kein Herunterladen von GMCMap-Daten wenn Display AUS +++</a>");}} 
// 9. Teil des HTML-Strings 
HTMLString = F("<h2>Auswahl - aktuelle Radioaktivität von ---www.gmcmap.com--- anzeigen &nbsp; EIN / AUS / AKTUELLER WERT / STATUS:</h2>");
HTMLString += ARA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_10() { // Einstellungen für passiv Piezo Buzzer 
String HTMLString; HTMLString.reserve(180);
String BSS;
String BHS;
// Startsound Einschalten / Ausschalten
  if (StartSound == "On") { 
  BSS =  CreateButton("Startsound EIN",true,"sspbson","","140","10",""); 
  BSS += CreateButton("Startsound AUS",false,"sspbsoff","","140","10","");} else {
  BSS =  CreateButton("Startsound EIN",false,"sspbson","","140","10",""); 
  BSS += CreateButton("Startsound AUS",true,"sspbsoff","","140","10","");} 
  // Startsound Einschalten / Ausschalten
  if (HourSound == "On") {
  BHS =  CreateButton("Stundensound EIN",true,"hspbson","","150","10","");
  BHS += CreateButton("Stundensound AUS",false,"hspbsoff","","150","10","");} else {
  BHS =  CreateButton("Stundensound EIN",false,"hspbson","","150","10","");
  BHS += CreateButton("Stundensound AUS",true,"hspbsoff","","150","10","");} 

// 10. Teil des HTML-Strings   
HTMLString = F("<h2>Auswahl - Sound Einstellungen:</h2>");
HTMLString += ""+BSS+BHS+"";
HTMLString += F("<p></p>"); 
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_11() { // Einstellungen aus Captiv-Portal
String HTMLString; HTMLString.reserve(180);
// 11. Teil des HTML-Strings  
HTMLString = F("<h2>Anzeige - Einstellungen aus dem Access Point (Captive-Portal):</h2>");
HTMLString += F("<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 130px;\">Einstellungen</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_12() { // Softwareversion
String HTMLString; HTMLString.reserve(180);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// int16_t ESPRAM = ESP.getMaxFreeBlockSize() / 1024;  // Check RAM
String ESPCore = ESP.getCoreVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Core Version
// String ESPCore = ESP.getFullVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Vollversion
uint8_t CPUTakt = ESP.getCpuFreqMHz(); // CPU-Takt
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 12. Teil des HTML-Strings  
HTMLString = "<h7>Software-Version: &nbsp;"+String(LMCVersion)+"</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Hardware-Version: &nbsp;"+ESPCore+"</h7>";
HTMLString += F("<a><br /></a>");
if (ESPRAM > MinRAMMem-512) {
HTMLString += "<h7>Freier Arbeitsspeicher: &nbsp;&nbsp;"+String(ESPRAM)+" Byte / 49152 Byte</h7>";} else {
HTMLString += "<h8>Freier Arbeitsspeicher: &nbsp;&nbsp;"+String(ESPRAM)+" Byte / 49152 Byte (Achtung ! - zu wenig)</h8>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>CPU-Taktfrequenz: &nbsp;&nbsp;"+String(CPUTakt)+"&nbsp; MHz</h7>";
HTMLString += F("<a><br /></a>");
if (ClockStatus == "OK") {
HTMLString += F("<h7>Status letzter Start: &nbsp;&nbsp;OK</h7>");} else {
HTMLString += "<h10>Status letzter Start: &nbsp;&nbsp;"+ClockStatus+"</h10>";}
HTMLString += F("<a><br /></a>");
HTMLString += WiFiSignalStrength(false); // WiFi Signalstärke in dBm
HTMLString += F("<a><br /></a>");
if (!MEP && !await) {
HTMLString += "<h7>Laufzeit der Uhr: &nbsp;&nbsp;"+rtime+"</h7>";} else {
HTMLString += F("<h7>Laufzeit der Uhr: &nbsp;&nbsp; Bitte warten ...</h7>");}
HTMLString += F("<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px; height: 48px; width: 180px;color: red;float: right; font-size: 16px;\">Zurücksetzen! (Reset)</button></a>");
HTMLString += F("<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px; height: 48px; width: 120px;color: blue;float: right; font-size: 18px;\">Neustart</button></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-right: 60px; height: 48px; width: 180px;color: navy;float: right; font-size: 18px;\">Aktualisieren</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h6>*) - aktuelle Auswahl</h6>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
return HTMLString;}

/********************************************************************************************************************************************************/ 

void HTMLAnswer(String WebFunction) {
String HTMLString; HTMLString.reserve(180);
String HTMLAnswerString;  
if (WebFunction == "MatrixOn") {HTMLAnswerString = F("Das LED-Matrix Display wurde eingeschaltet");} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = F("Das LED-Matrix Display wird ausgeschaltet");} 
if (WebFunction == "MatrixIsOn") {HTMLAnswerString = F("Das LED-Matrix Display ist bereits eingeschaltet");} 
if (WebFunction == "MatrixIsOff") {HTMLAnswerString = F("Das LED-Matrix Display ist bereits ausgeschaltet");}
if (WebFunction == "RebootClock") {HTMLAnswerString = F("Wenn Sie auf die Startseite zurückkehren<br />wird die LED Matrix Uhr neu gestartet");}
if (WebFunction == "DoublePointFlash") {HTMLAnswerString = F("Der Doppelpunkt der Uhr blinkt ab jetzt.");} 
if (WebFunction == "DoublePointOn") {HTMLAnswerString = F("Der Doppelpunkt der Uhr ist jetzt permanent an.");} 
if (WebFunction == "DoublePointOff") {HTMLAnswerString = F("Der Doppelpunkt der Uhr ist jetzt permanent aus.");} 
if (WebFunction == "BrightnessLevel1") {HTMLAnswerString = F("LED-Matrix Helligkeitsstufe 1 wurde aktiviert");} 
if (WebFunction == "BrightnessLevel2") {HTMLAnswerString = F("LED-Matrix Helligkeitsstufe 2 wurde aktiviert");} 
if (WebFunction == "BrightnessLevel3") {HTMLAnswerString = F("LED-Matrix Helligkeitsstufe 3 wurde aktiviert");} 
if (WebFunction == "BrightnessLevel4") {HTMLAnswerString = F("LED-Matrix Helligkeitsstufe 4 wurde aktiviert");} 
if (WebFunction == "BrightnessLevel5") {HTMLAnswerString = F("LED-Matrix Helligkeitsstufe 5 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel1") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 1 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel2") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 2 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel3") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 3 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel4") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 4 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel5") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 5 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel6") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 6 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel7") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 7 wurde aktiviert");} 
if (WebFunction == "ScrollTextTimeLevel8") {HTMLAnswerString = F("Die Laufschriftgeschwindigkeit Stufe 8 wurde aktiviert");} 
if (WebFunction == "WeatherFunctionsEin1") {HTMLAnswerString = F("OpenWeatherMap.org - Wetterdaten wurden eingeschaltet");}
if (WebFunction == "WeatherFunctionsEin2") {HTMLAnswerString = F("OpenWeatherMap.org - Wetterdaten sind bereits eingeschaltet");}
if (WebFunction == "WeatherFunctionsAus") {HTMLAnswerString = F("OpenWeatherMap.org - Wetterdaten wurden ausgeschaltet");}
if (WebFunction == "NewsDataFunctionEin1") {HTMLAnswerString = F("NewsData.io - aktuelle Nachrichten wurden eingeschaltet");}
if (WebFunction == "NewsDataFunctionEin2") {HTMLAnswerString = F("NewsData.io - aktuelle Nachrichten sind bereits eingeschaltet");}
if (WebFunction == "NewsDataFunctionAus") {HTMLAnswerString = F("NewsData.io - aktuelle Nachrichten wurden ausgeschaltet");}
if (WebFunction == "GMCMapFunctionEin1") {HTMLAnswerString = F("GMCMap.com - aktuelle Radioaktivitätswerte wurden eingeschaltet");}
if (WebFunction == "GMCMapFunctionEin2") {HTMLAnswerString = F("GMCMap.com - aktuelle Radioaktivitätswerte sind bereits eingeschaltet");}
if (WebFunction == "GMCMapFunctionAus") {HTMLAnswerString = F("GMCMap.com - aktuelle Radioaktivitätswerte wurden ausgeschaltet");}
if (IsFederalStateOK(FederalState)) { 
if (WebFunction == "EnhancedFunctionsEin1") {HTMLAnswerString = F("Die Zusatzfunktionen wurden eingeschaltet<br />(Datum, Feiertage, Geburtstage)");}
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = F("Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Feiertage, Geburtstage)");}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = F("Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Feiertage, Geburtstage)");}} else {
if (WebFunction == "EnhancedFunctionsEin1") {HTMLAnswerString = F("Die Zusatzfunktionen wurden eingeschaltet<br />(Datum, Geburtstage)");}
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = F("Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Geburtstage)");}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = F("Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Geburtstage)");}}
if (WebFunction == "StartSoundOnM") {HTMLAnswerString = F("Der Uhr-Startsound wurde eingeschaltet.");}
if (WebFunction == "StartSoundOnMIs") {HTMLAnswerString = F("Der Uhr-Startsound ist bereits eingeschaltet.");}
if (WebFunction == "StartSoundOffM") {HTMLAnswerString = F("Der Uhr-Startsound wurde ausgeschaltet.");}
if (WebFunction == "StartSoundOffMIs") {HTMLAnswerString = F("Der Uhr-Startsound ist bereits ausgeschaltet.");}
if (WebFunction == "HourSoundOnM") {HTMLAnswerString = F("Der Uhr-Stundensound wurde eingeschaltet.");}
if (WebFunction == "HourSoundOnMIs") {HTMLAnswerString = F("Der Uhr-Stundensound ist bereits eingeschaltet.");}
if (WebFunction == "HourSoundOffM") {HTMLAnswerString = F("Der Uhr-Stundensound wurde ausgeschaltet.");}
if (WebFunction == "HourSoundOffMIs") {HTMLAnswerString = F("Der Uhr-Stundensound ist bereits ausgeschaltet.");}
if (WebFunction == "DisplayCheck") {HTMLAnswerString = F("LED-Matrix Display Test läuft. Bitte warten ...");}
if (WebFunction == "NoDisplayCheck") {HTMLAnswerString = F("********* Display-Test derzeit nicht möglich. *********<br />Bitte warten Sie bis die Uhrzeit wieder angezeigt wird.");}
if (WebFunction == "ResetClockYes") {HTMLAnswerString = F("Wenn Sie auf die Startseite zurückkehren wird<br />der Access Point (Captive-Portal) gestartet.<br />&nbsp;&nbsp;&nbsp;&nbsp;--- Die Einstellungen bleiben erhalten. ---");}
if (WebFunction == "POD1") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice1+" gesendet ...";} 
if (WebFunction == "POD2") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice2+" gesendet ...";} 
if (WebFunction == "POD2") {HTMLAnswerString = "Die Pushover Testnachricht wurde an "+PushoverDevice3+" gesendet ...";} 
if (WebFunction == "FPO") {HTMLAnswerString = F("Die Pushover Testnachricht konnte nicht gesendet werden !");} 
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; text-align: center;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;  margin-bottom: 0px;}");
HTMLString += F("h2 {font-size: 26px; margin-top: 0px;  margin-bottom: 50px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>"+HTMLAnswerString+"</h2>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();}

/********************************************************************************************************************************************************/

void ActualWeather() {
String HTMLString; HTMLString.reserve(180); 
if (!await){
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 50px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Wetterdaten von: "+OWMLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPweatherString+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualAPINews() {
String HTMLString; HTMLString.reserve(180); 
if (!await){
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 50px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Letze Nachricht von: "+NewsDataLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPAPINews+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualGMCMapData() {
String HTMLString; HTMLString.reserve(180); 
if (!await){   
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 30px;}");
HTMLString += F("h4 {font-size: 16px; color: #ffcc00; margin-bottom: 30px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Letzter Wert von: "+GMCMapLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPGMCMapValue+"</h3>";
HTMLString += "<h4>Achtung: Gefahr bei Werten größer 90 CPM !</h4>";
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Send HTMLString 
MyWaitLoop(100);
HTMLString.clear();} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Matrix einschalten
void MatrixOn() {
if (!MEP) {  
if (!await){  
if (!Display) {
await = true;
HTMLAnswer("MatrixOn");
MEP = true;
if (SerialMonitor) {DEBUG_PRINTLN(F("Das LED-Matrix Display wurde eingeschaltet"));}   
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display an
printStringWithShift("           ",ScrollTextTime); 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime); 
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus == "MIN" || InHumidityStatus == "MAX") {
digitalWrite(HWLEDPin, HIGH);}}  // LED  Einschalten
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
MyWaitLoop(100); // kurz warten
Display = true; MEP = false; await = false;} else {HTMLAnswer("MatrixIsOn");}
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
} else {PleaseWait();}}}

/********************************************************************************************************************************************************/

// Matrix ausschalten
void MatrixOff() { 
if (!await){  
if (Display) { 
await = true;   
dots = 1; // Doppelpunkt an  
Display = false;     
HTMLAnswer("MatrixOff"); 
if (SerialMonitor) {DEBUG_PRINTLN(F("Das LED-Matrix Display wurde ausgeschaltet"));} 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime);
printStringWithShift("           ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,0);
digitalWrite(HWLEDPin, LOW); // LED Ausschalten
await = false;} else {HTMLAnswer("MatrixIsOff");} 
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointFlash() {
if (!await){      
DoublePointStatus = 2; // Doppelpunkt der Uhr blinken lassen 
// Status Doppelpunkt
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointFlash");
if (SerialMonitor) {DEBUG_PRINTLN(F("Doppelpunkt der Uhr blinkt."));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointOn() {
if (!await){  
DoublePointStatus = 1; // Doppelpunkt der Uhr permanant AN
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointOn");
if (SerialMonitor) {DEBUG_PRINTLN(F("Doppelpunkt der Uhr permanant an."));}
} else {PleaseWait();}}


/********************************************************************************************************************************************************/

void DoublePointOff() {
if (!await){  
DoublePointStatus = 0; // Doppelpunkt der Uhr permanant aus
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointOff");
if (SerialMonitor) {DEBUG_PRINTLN(F("Doppelpunkt der Uhr permanant aus."));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Helligkeitsstufen (BrightnessLevel) der LED's des Max7219 per Webserver einstellen
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

void SetBrightnessLevel(String BLText, byte BLInt) {
if (!await){
sendCmdAll(CMD_INTENSITY, BLInt-1);
Brightness = BLText;
if (littleFSInit) {
SetupSave(F("matrix_brightness.txt"), Brightness);}
HTMLAnswer("BrightnessLevel"+String(BLInt));
if (SerialMonitor) {DEBUG_PRINTLN("Helligkeitsstufe "+String(BLInt)+" aktiviert");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Geschwindigkeit der Laufschrift einstellen
void ScrollTextTimeLevel1() {
SetScrollTimeAndLevel(60,1);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2() { 
SetScrollTimeAndLevel(55,2);}

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

void SetScrollTimeAndLevel(byte STTime, byte STLevel){
if (!await) {
await = true;   
ScrollTextTime = STTime; // Lauftextgeschwindigkeit   
if (littleFSInit) {
SetupSave(F("status_scrolltexttime.txt"), String(STTime));}
HTMLAnswer("ScrollTextTimeLevel"+String(STLevel)+"");
if (SerialMonitor) {DEBUG_PRINTLN("Laufschriftgeschwindigkeit Stufe"+String(STLevel)+" aktiviert");}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN, 0);} else {
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime);
ClockScrollIn();} // Uhrzeit in Display hineinscrollen 
await = false;} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOn() {
if (!await) {  
if (!WeatherFunctions) {
WeatherFunctions = true;
OWMAdjustChange = true;
StatusOpenWeatherMap = F("aktiviert");
if (littleFSInit) {
SetupSave(F("status_weatherfunctions.txt"), "On");} 
HTMLAnswer("WeatherFunctionsEin1");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Wetterdaten eingeschaltet"));}}
else {
HTMLAnswer("WeatherFunctionsEin2");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Wetterdaten sind bereits eingeschaltet"));}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOff() {
if (!await) {   
// DEBUG_PRINTLN(F("Zusatzfunktionen ausgeschaltet"));
WeatherFunctions = false;
StatusOpenWeatherMap = F("deaktiviert");
if (littleFSInit) {
SetupSave(F("status_weatherfunctions.txt"), "Off");} 
HTMLAnswer("WeatherFunctionsAus");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Wetterdaten ausgeschaltet"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOn() {
if (!await) { 
if (!NewsDataFunction) {  
NewsDataFunction = true;
NAAdjustChange = true;
StatusNewsData = F("aktiviert");
if (littleFSInit) {
SetupSave(F("status_NewsDatafunctions.txt"), "On");} 
if (SerialMonitor) {
DEBUG_PRINTLN(F("NewsData - aktuelle Nachrichten eingeschaltet"));
if (AccessGMCMap && AccessNewsData) {DEBUG_PRINTLN(F("+++   Die Uhr wird neu gestartet   +++"));}}
HTMLAnswer("NewsDataFunctionEin1");} else {
HTMLAnswer("NewsDataFunctionEin2");
if (SerialMonitor) {DEBUG_PRINTLN(F("NewsData - aktuelle Nachrichten sind bereits eingeschaltet"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOff() {
if (!await) {   
NewsDataFunction = false;
StatusNewsData = F("deaktiviert");
if (littleFSInit) {
SetupSave(F("status_NewsDatafunctions.txt"), "Off");}  
HTMLAnswer("NewsDataFunctionAus");
if (SerialMonitor) {DEBUG_PRINTLN(F("NewsData - aktuelle Nachrichten ausgeschaltet"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOn() {
if (!await) {
if (!GMCMapFunction) {
GMCMapFunction = true;
GMCMapAdjustChange = true;
StatusGMCMap = F("aktiviert");
if (littleFSInit) {
SetupSave(F("status_GMCMapFunction.txt"), "On");}  
if (SerialMonitor) {
DEBUG_PRINTLN(F("GMCMap - Anzeige der aktuellen Radioaktivität eingeschaltet"));
if (AccessGMCMap && AccessNewsData) {DEBUG_PRINTLN(F("+++   Die Uhr wird neu gestartet   +++"));}}
HTMLAnswer("GMCMapFunctionEin1");} else {
HTMLAnswer("GMCMapFunctionEin2");
if (SerialMonitor) {DEBUG_PRINTLN(F("GMCMap - Anzeige der aktuellen Radioaktivität ist bereits eingeschaltet"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOff() {
if (!await) { 
GMCMapValue = "";
GMCMapToHigh == "";     
GMCMapFunction = false;
StatusGMCMap = F("deaktiviert");
StatusWarningLED("LOW");
if (littleFSInit) {
SetupSave(F("status_GMCMapFunction.txt"), "Off");}  
HTMLAnswer("GMCMapFunctionAus");
if (SerialMonitor) {DEBUG_PRINTLN(F("GMCMap - aktuelle Anzeige der Radioaktivität ausgeschaltet"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOn() {
if (!await) {   
if (!EnhancedFunctions) {
EnhancedFunctions = true;
if (littleFSInit) {
SetupSave(F("status_enhancedfunctions.txt"), "On");} 
HTMLAnswer("EnhancedFunctionsEin1");} else {
HTMLAnswer("EnhancedFunctionsEin2");} 
if (IsFederalStateOK(FederalState)) {  
if (SerialMonitor) {DEBUG_PRINTLN(F("Zusatzfunktionen eingeschaltet - (Datum, Feiertage, Geburtstage)"));}} else {
if (SerialMonitor) {DEBUG_PRINTLN(F("Zusatzfunktionen eingeschaltet - (Datum, Geburtstage)"));}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOff() {
if (!await) {   
EnhancedFunctions = false;
if (littleFSInit) {
SetupSave(F("status_enhancedfunctions.txt"), "Off");} 
HTMLAnswer("EnhancedFunctionsAus");
if (IsFederalStateOK(FederalState)) {  
if (SerialMonitor) {DEBUG_PRINTLN(F("Zusatzfunktionen ausgeschaltet - (Datum, Feiertage, Geburtstage)"));}} else {
if (SerialMonitor) {DEBUG_PRINTLN(F("Zusatzfunktionen ausgeschaltet - (Datum, Geburtstage)"));}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GetAdjustments() {
String OWMAPI, OWMCID, NewsDataKey, CheckIP, HTMLString;
HTMLString.reserve(180);
if (!await){ 
// Check IP-Adresse
if (cip.length() > 15 || cip.length() < 7) {CheckIP = F("k.A.");} else {CheckIP = cip;} // Check IP-Adresse
if (AccessOpenWeatherMap){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = F("k.A.");OWMCID = F("k.A.");}
if (newsKey != ""){NewsDataKey = newsKey;} else {NewsDataKey = F("k.A.");}
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}");
HTMLString += F("h2 {font-size: 18px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("h3 {font-size: 24px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("h4 {font-size: 14px; margin-top: 0px; margin-bottom: 1px; color: #ffcc00; float: right;}");
HTMLString += F("h5 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "") {
HTMLString += "<h2>( Besitzer der Uhr:&nbsp;&nbsp;"+WatchOwner1FirstName+" "+WatchOwner1LastName+" )</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += F("<h3>Einstellungen aus dem Captive Portal</h3>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;"+String(ssid)+"</h2>";
HTMLString += "<h2>IP-Adresse:&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += "<h2>NTP-Server:&nbsp;&nbsp;&nbsp;"+NTPSN+"</h2>";
HTMLString += "<h2>Zeitzone:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+MCTZ+"</h2>";
HTMLString += F("<a><br /></a>");
// Besitzer der Uhr im Detail
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "" && Gender1 != "" && Birthday1 != "") { 
HTMLString += F("<h2>1. Besitzer der Uhr:</h2>");   
HTMLString += "<h2>Nachname: &nbsp;&nbsp;"+WatchOwner1LastName+"</h2>";
HTMLString += "<h2>Vorname: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner1FirstName+"</h2>";
if (Gender1 == "male") {HTMLString += "<h2>Geschlecht: &nbsp;männlich";}
if (Gender1 == "female") {HTMLString += "<h2>Geschlecht: &nbsp;weiblich";}
HTMLString += "<h2>Geburtstag: &nbsp;"+Birthday1+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner2FirstName != "" && WatchOwner2LastName != "" && Gender2 != "" && Birthday2 != "") { 
HTMLString += F("<h2>2. Besitzer der Uhr:</h2>");   
HTMLString += "<h2>Nachname: &nbsp;&nbsp;"+WatchOwner2LastName+"</h2>";
HTMLString += "<h2>Vorname: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner2FirstName+"</h2>";
if (Gender2 == "male") {HTMLString += "<h2>Geschlecht: &nbsp;männlich";}
if (Gender2 == "female") {HTMLString += "<h2>Geschlecht: &nbsp;weiblich";}
HTMLString += "<h2>Geburtstag: &nbsp;"+Birthday2+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner3FirstName != "" && WatchOwner3LastName != "" && Gender3 != "" && Birthday3 != "") { 
HTMLString += F("<h2>3. Besitzer der Uhr:</h2>");   
HTMLString += "<h2>Nachname: &nbsp;&nbsp;"+WatchOwner3LastName+"</h2>";
HTMLString += "<h2>Vorname: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner3FirstName+"</h2>";
if (Gender3 == "male") {HTMLString += "<h2>Geschlecht: &nbsp;männlich";}
if (Gender3 == "female") {HTMLString += "<h2>Geschlecht: &nbsp;weiblich";}
HTMLString += "<h2>Geburtstag: &nbsp;"+Birthday3+"</h2>";
HTMLString += F("<a><br /></a>");}
// Bundesland
if (IsFederalStateOK(FederalState)) {
HTMLString += "<h2>Österreichisches Bundesland:&nbsp;&nbsp;"+LongFederalState(FederalState)+"</h2>";
HTMLString += F("<a><br /></a>");}
if (AccessOpenWeatherMap) {
HTMLString += "<h2>OpenWeatherMap.org --- API-Key:&nbsp;&nbsp;"+OWMAPI+"</h2>";
if (WeatherFunctions && String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
HTMLString += "<h2>OpenWeatherMap.org --- City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+" ( "+String(WeatherLocation)+" )</h2>";} else {  
HTMLString += "<h2>OpenWeatherMap.org --- City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>";}
HTMLString += F("<a><br /></a>");}
if (AccessNewsData) {
HTMLString += "<h2>NewsData.io --- API-Key:&nbsp;&nbsp;"+NewsDataKey+"</h2>";
HTMLString += F("<a><br /></a>");}
if (AccessGMCMap) {
HTMLString += "<h2>GMCMap.com --- History-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+GMCMapHistoryID+"</h2>";
HTMLString += "<h2>GMCMap.com --- GMT-Offset:&nbsp;&nbsp;"+GMCMapGMTOffset+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsPushover) {
HTMLString += "<h2>Pushover-SecureAppToken:  &nbsp;&nbsp;&nbsp;&nbsp;"+SecureAppToken+"</h2>";
HTMLString += "<h2>Pushover-UserToken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+UserToken+"</h2>";
HTMLString += "<h2>Pushover-Gerätename1:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice1+"</h2>";
if (PushoverDevice2 != "") { 
HTMLString += "<h2>Pushover-Gerätename2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice2+"</h2>";} else {
HTMLString += F("<h2>Pushover-Gerätename2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;k.A</h2>");}  
if (PushoverDevice3 != "") {
HTMLString += "<h2>Pushover-Gerätename3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice3+"</h2>";} else {
HTMLString += F("<h2>Pushover-Gerätename3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;k.A.</h2>");}  
HTMLString += "<h2>Pushover-Nachrichtensound:  &nbsp;"+PushoverMessageSound+"</h2>";
HTMLString += F("<a><br /></a>");}
if (MidnightGhost == "JA") {HTMLString += F("<h2>Geister um Mitternacht anzeigen ?:  &nbspJA</h2>");} else {HTMLString += F("<h2>Geister um Mitternacht anzeigen ?:  &nbspNEIN</h2>");}
HTMLString += F("<a><br /></a>");
if (InTempOffset == "999") {HTMLString += F("<h2>Temperatur- und Luftfeuchtesensor ?:  &nbspdeaktiviert</h2>");} else {
if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {HTMLString += F("<h2>Temperatur- und Luftfeuchtesensor ?:  &nbspKEIN</h2>");} else {
if (DHTSensor == "DHT11") {HTMLString += F("<h2>Temperatur- und Luftfeuchtesensor ?:  &nbspDHT11</h2>");} else {HTMLString += F("<h2>Temperatur- und Luftfeuchtesensor ?:  &nbspDHT22</h2>");}}}
HTMLString += F("<a><br /></a>");
if (IsBuzzer) {
HTMLString += F("<h2>Passiv Buzzer installiert</h2>");} else {
HTMLString += F("<h2>Passiv Buzzer nicht installiert oder deaktiviert</h2>");}
HTMLString += F("<a><br /></a>");
if (IsPushover) {
HTMLString += F("<h2><br /></h2>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
HTMLString += F("<h5>Auswahl - Pushover Testnachricht an Gerät senden:</h5>");
if (PushoverDevice1 != "" && Display) {
HTMLString += CreateButton("Gerät 1",false,"potmdevice1","","90","10","");} else {
HTMLString += CreateButton("Gerät 1",false,"potmdevice1","","90","10","disabled");} 
if (PushoverDevice2 != "" && Display) { 
HTMLString += CreateButton("Gerät 2",false,"potmdevice2","","90","10","");} else {
HTMLString += CreateButton("Gerät 2",false,"potmdevice2","","90","10","disabled");}  
if (PushoverDevice3 != "" && Display) { 
HTMLString += CreateButton("Gerät 3",false,"potmdevice3","","90","10","");} else {
HTMLString += CreateButton("Gerät 3",false,"potmdevice3","","90","10","disabled");}  
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
HTMLString += F("<a><br /></a>");}
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 1px; height: 48px; width: 160px\">zurück zur Startseite</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();
if (SerialMonitor) {DEBUG_PRINTLN(F("Einstellungen aus dem Access Point (Captive Portal) aufgerufen"));}}
else {PleaseWait();}}

/********************************************************************************************************************************************************/

String CreateButton(String BMark, bool Bselect, String BHRef, String Blevel, String Bwidth, String BRmargin, String Bdisable) {
String HTMLString; HTMLString.reserve(180);
if (Bselect) {
HTMLString  = "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; color: blue; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"*</button></a>";} else {
HTMLString += "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"</button></a>";}
return HTMLString;}

/********************************************************************************************************************************************************/

void ResetClockSettings() {
if (!await) {   
String HTMLString; HTMLString.reserve(180);
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align:center;}\
   h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}\
   h2 {font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
   h3 {font-size: 28px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <a><br /></a>\
     <h3>LED-Matrix-Uhr zurücksetzen !</h3>\
     <h2>(Die WLAN-Zugangsdaten werden gelöscht)</h2>\
     <a><br /></a>\
     <h2>Jetzt LED-Matrix-Uhr zurücksetzen ?</h2>\
     <a><br /></a>\
     <a href =\"/goreset\"><button class=\"button\" style=\"height: 48px; width: 110px; font-size: 22px; color: red\">JA</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">NEIN</button></a>\
  </body>\
    </html>");  
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void BadRAM() { 
String HTMLString; HTMLString.reserve(180);
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align:center;}\
   h1 {color: #2ECCFA; margin-top: 30px;}\
   h2 {font-size: 22px; margin-bottom: 10px; margin-left: 10px;}\
   h3 {font-size: 28px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
     <a><br /></a>\
     <h3>Zu wenig Arbeitsspeicher !</h3>\
     <h2>(Ein Neustart der Uhr behebt das Problem)</h2>\
     <a><br /></a>\
     <h2>Jetzt LED-Matrix-Uhr neu starten ?</h2>\
     <a><br /></a>\
     <a href =\"/reboot\"><button class=\"button\" style=\"height: 48px; width: 110px; font-size: 22px; color: red\">JA</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">NEIN</button></a>\
  </body>\
    </html>"); 
OneTimeCall = true;        
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);
HTMLString.clear();}

/********************************************************************************************************************************************************/

void ResetClockYes() { 
/*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
if (!await) { 
HTMLAnswer("ResetClockYes");
ResetNow = true; // Reset - Werkseinstellungen 
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
String HTMLString; HTMLString.reserve(180);
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; text-align: center;}\
   h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}\
   h2 {font-size: 26px; margin-top: 0px; margin-bottom: 10px;}\
   h3 {font-size: 14px; margin-top: 0px; margin-bottom: 50px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <a><br /></a>\
    <h2>Diese Seite wurde nicht gefunden !</h2>\
    <h3>(Ein Neustart der Uhr könnte das Problem eventuell beheben)</h3>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>\
  </body>\
    </html>"); 
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
HTMLString.clear();}

/********************************************************************************************************************************************************/

void ChangeDisplayAutoOnOff() {
String HTMLString; HTMLString.reserve(180);
bool MDTime;
if (!await) {
// Wenn gespeichert Button betätigt wurde 
if (server.method() == HTTP_POST) {
    // Einstellungen speichern
    DisplayOnTime = server.arg("DOnT"); // Display On-Time
    DisplayOffTime = server.arg("DOffT"); // Display Off-Time
    DisplayOnTime.trim();
    DisplayOffTime.trim();

    // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
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
    int16_t DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int16_t CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = (int16_t)strtol(CheckMDHourOn.c_str(), NULL, 10);} else {DisplayOnTime = F("!");}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = (int16_t)strtol(CheckMDMinOn.c_str(), NULL, 10);} else {DisplayOnTime = F("!");}
    if (DisplayOnTime != "" && DisplayOnTime != "!" ) {
    for (byte i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");} else {   
    MDTime = false;    
    for (byte i=0; i <= 59; i++){
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
    int16_t DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int16_t CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = (int16_t)strtol(CheckMDHourOff.c_str(), NULL, 10);} else {DisplayOffTime = F("!");}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = (int16_t)strtol(CheckMDMinOff.c_str(), NULL, 10);} else {DisplayOffTime = F("!");}
    if (DisplayOffTime != "" && DisplayOffTime != "!") {
    for (byte i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");} else {   
    MDTime = false;    
    for (byte i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");}}}}}

    if (DisplayOnTime == "" && DisplayOffTime == "") {
    DisplayOnTime = F("NullOn");
    DisplayOffTime = F("NullOff");}

    // Wenn MidnightGhost dann bei 00:00 Uhr Ausschaltzeit eine Minute weiter 
    if (MidnightGhost == "YES" && DisplayOffTime == "00:00") {
    DisplayOffTime = "00:01";}
    FDOnTime =  DisplayOnTime; // nur für Anzeige
    FDOffTime = DisplayOffTime; // nur für Anzeige

    // Wenn Eingabefehler
    if (DisplayOnTime == "!" || DisplayOffTime == "!" || DisplayOnTime == DisplayOffTime) {
    if (DisplayOnTime == "!") {DisplayOnTime = F("");} 
    if (DisplayOffTime == "!") {DisplayOffTime = F("");}  
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    HTMLString += F("<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Ungültige Eingabe(n) - Bitte korrigieren!</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/cdof\"><button>Zurück</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    } else {
    if (DisplayOnTime == "NullOn") {DisplayOnTime = F("");}
    if (DisplayOffTime == "NullOff") {DisplayOffTime = F("");}   
    if (littleFSInit){
    // *** Eingaben im FileSystem speichern ***
    // Matrix Display Einschaltzeit
    if (DisplayOnTime != ""){
    SetupSave(F("display-on-time.txt"), DisplayOnTime);} else {
    if (LittleFS.exists(F("/display-on-time.txt"))){LittleFS.remove(F("/display-on-time.txt"));}}
    // Matrix Display Ausschaltzeit
    if (DisplayOffTime != ""){
    SetupSave(F("display-off-time.txt"), DisplayOffTime);} else {
    if (LittleFS.exists(F("/display-off-time.txt"))){LittleFS.remove(F("/display-off-time.txt"));}}
    SetupSave(F("display-change-time.txt"), "True"); // Display On/Off Daten wurden geändert
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {DEBUG_PRINTLN(F("Display automatisch EIN/AUS  Daten gespeichert... "));} 

    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    HTMLString += F("<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Die Einstellungen wurden gespeichert<br /><br />Die Uhr wird neu gestartet wenn Sie auf die Startseite zurückkehren</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>zurück zur Startseite</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    // Reboot auslösen wenn auf Startseite zurück
    RebootNow = true;
    }
     else
     { // Zugangsdaten wurden nicht gespeichert
     if (SerialMonitor) {DEBUG_PRINTLN(F("Zugangsdaten und Einstellungen wurden nicht gespeichert !"));}
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");   
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    HTMLString += F("<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>zurück zur Startseite</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    // Restart auslösen
    if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    StatusWarningLED("LOW"); // LED Ausschalten
    ESP.restart();  
    } 
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen 
    } // Ende Eingabefehler
  } else {  
    // Einstellungen
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/cdof' method='post'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    HTMLString += F("<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>");
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display EIN-Zeit (Beispiel: 07:00)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOnT' id='DOnT' value='"+FDOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display AUS-Zeit (Beispiel: 23:30)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOffT' id='DOffT' value='"+FDOffTime+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += F("<h6>*) - keine Angabe deaktiviert die Funktion</h6>");
    HTMLString += F("<button type='submit'>Speichern</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/cdofc\"><button>Abbrechen</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);
    MyWaitLoop(100);}} else {PleaseWait();}}  // Send HTMLString oder Warten

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DisplayCheck() {
if (!await) {
await = true; 
char  MDC   = '9' + 2; // Full Display ON
String MDCShow = "      "+String(MDC)+String(MDC)+String(MDC)+String(MDC);
HTMLAnswer("DisplayCheck");
printStringWithShift3(MDCShow.c_str(),ScrollTextTime);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
StatusWarningLED("HIGH"); // Check LED
MyWaitLoop(8000); // kurz warten (8 Sekunden)
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") {
StatusWarningLED("LOW");}} else {StatusWarningLED("LOW");}
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
await = false; 
} else {HTMLAnswer("NoDisplayCheck");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ChangeDisplayAutoOnOffCancel() {
String HTMLString; HTMLString.reserve(180);
  // Display Einschaltzeit als String
  if (littleFSInit) {
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOnTime.clear();}} else
         {DisplayOnTime.clear();}
  // Display Ausschaltzeit als String
  if (littleFSInit) {
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOffTime.clear();}} else
         {DisplayOffTime.clear();}

  // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
  CheckLEDMatrixDisplayOnOff();

    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    HTMLString += F("<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>");
    HTMLString += F("<br/>");
    if (littleFSInit) {
    HTMLString += F("<h6>Abbruch! - Die alte Einstellung wurde wieder geladen ...</h6>");} else {
    HTMLString += F("<h6>Abbruch! - Die alte Einstellung konnte nicht geladen werden !!! ...</h6>");}
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>zurück zur Startseite</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
 HTMLString.clear();}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOn() {
if (StartSound != "On") { 
if (!await) {  
await = true;
StartBuzzerBeep();    
if (littleFSInit) {
StartSound = F("On");   
SetupSave(F("status_startsound.txt"), StartSound);} else {StartSound = F("Off");}
HTMLAnswer("StartSoundOnM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) Startsound eingeschaltet"));}
await = false;} else {PleaseWait();}} else {HTMLAnswer("StartSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOff() {
if (StartSound != "Off") {  
StartSound = F("Off");     
if (littleFSInit) {  
SetupSave(F("status_startsound.txt"), StartSound);} 
HTMLAnswer("StartSoundOffM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) Startsound ausgeschaltet"));}
} else {HTMLAnswer("StartSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOn() {
if (HourSound != "On") { 
if (!await) {  
await = true;
HourBuzzerBeep();   
if (littleFSInit) {
HourSound = F("On");   
SetupSave(F("status_hoursound.txt"), HourSound);} else {HourSound = F("Off");}
HTMLAnswer("HourSoundOnM");
await = false;
if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) Stundensound eingeschaltet"));}} else {PleaseWait();}
} else {HTMLAnswer("HourSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOff() {
if (HourSound != "Off") {
HourSound = F("Off");  
if (littleFSInit) {
SetupSave(F("status_hoursound.txt"), HourSound);} 
HTMLAnswer("HourSoundOffM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Summer (Buzzer) Stundensound ausgeschaltet"));}
} else {HTMLAnswer("HourSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PleaseWait() {
// Bitte warten Seite einlenden
String RefreshTime = F("5"); // Aktualisierung der Website nach X Sekunden
String HTMLString; HTMLString.reserve(180);
OneTimeCall = false;
HTMLString = F("<!DOCTYPE html> <html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
// HTMLString += F("<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 {font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 {font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 {font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Bitte warten ...</h2>");
if (Display) {
HTMLString += F("<h3>(Bis die Uhrzeit wieder im Matrix-Display angezeigt wird)</h3>");}
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString);  
InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
DEBUG_PRINTLN(F("Bitte warten ..."));}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GoToWebSiteHomePage() { // Automatische Weiterleitung zur HomePage
String HTMLString; HTMLString.reserve(180);
HTMLString = F("<!DOCTYPE html><html lang='de-AT'>");
HTMLString += F("<head>");
HTMLString += F("<title>LED-Matrix-Uhr</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 {font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 {font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 {font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("<script>");
// Wartezeit (ms), bis Sie zur Homepage zurückkehren | Standard = 1000
HTMLString += F("window.onload = function() {setTimeout(function() {window.location.href = \"/\";}, 2000);}"); 
HTMLString += F("</script>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Bitte warten ...</h2>");
if (Display) {
HTMLString += F("<h3>(Bis die Uhrzeit wieder im Matrix-Display angezeigt wird)</h3>");}
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString);}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HTTPCodeText(int16_t httpcode) {
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
