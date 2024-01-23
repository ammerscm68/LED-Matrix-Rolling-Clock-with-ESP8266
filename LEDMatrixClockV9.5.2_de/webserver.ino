// Webserver Startseite
void handleRoot() { 
// Daten senden
server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
SendHTMLString(""); // Send HTMLString                    
server.sendContent(RootHTMLString_1());  // 1. Teil
server.sendContent(RootHTMLString_2());  // 2. Teil
server.sendContent(RootHTMLString_3());  // 3. Teil
server.sendContent(RootHTMLString_4());  // 4. Teil
server.sendContent(RootHTMLString_5());  // 5. Teil
server.sendContent(RootHTMLString_6());  // 6. Teil
server.sendContent(RootHTMLString_7());  // 7. Teil
server.sendContent(RootHTMLString_8());  // 8. Teil
if (IsBuzzer) {server.sendContent(RootHTMLString_9());} // 9. Teil nur für Passiv Piezo Summer (Buzzer)
server.sendContent(RootHTMLString_10()); // 10. Teil
server.sendContent(RootHTMLString_11()); // 11. Teil
server.client().flush();
MyWaitLoop(100); // kurze Pause damit der Speicher geleert werden kann
server.client().stop();
if (RebootNow) {
if (!Display) {
printStringWithShift("           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Display an   
} // Display einschalten wenn Aus  
printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime); 
digitalWrite(HWLEDPin, LOW); // LED Ausschalten 
ESP.restart();} // Neustart der Uhr
if (littleFSInit && ResetNow) {ResetClock();}} // Einstellungen auf Standard - WLAN Zugangsdaten werden gelöscht

/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"60\">\ = Refresh alle 60 sekunden*/ 
String RefreshTime = "60"; // Aktualisierung der Website nach X Sekunden
  String HTMLString = "";
   String InTempOnOff1 = "";
    String InTempOnOff2 = "";
     String InHumMessage = "";

// Innenfeuchtigkeit - Meldung wenn zu niedrig oder zu hoch
  InHumValue = InHumMessure();
  if (InHumValue != "-") {
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {
  digitalWrite(HWLEDPin, HIGH);  // LED Einschalten
  // Luftfeuchte zu niedrig
  InHumMessage = "Achtung: Luftfeuchte zu niedrig !";} else {digitalWrite(HWLEDPin, LOW);}  // LED Ausschalten
  if (IntInHumValue >= MaxComfortHum) {
  // Luftfeuchte zu hoch     
  digitalWrite(HWLEDPin, HIGH);  // LED Einschalten  
  if (WeatherFunctions and Display){
  if (humidity >= MaxComfortHum || humidity == -1) {InHumMessage = "Achtung: Luftfeuchte zu hoch !";} else 
  {InHumMessage = "Achtung: Luftfeuchte zu hoch ! - Bitte Lüften";}
  } else {InHumMessage = "Achtung: Luftfeuchte zu hoch !";}} else {digitalWrite(HWLEDPin, LOW);}} else {
  digitalWrite(HWLEDPin, LOW);}  // LED Ausschalten

  // Innentemperatur anzeigen JA/NEIN (DHT11 oder DHT22)
  if (InTempCounter < 5) {
  InTempValue = InTempMessure(); 
  if (InTempValue != "-") {
  IntInTempValue = InTempValue.toInt();  
  if (IntInTempValue < 10) {
  InTempOnOff1 = "id= 'Bvalue'>Temperatur an der Uhr:";  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {InTempOnOff1 = "id= 'Gvalue'>Temperatur an der Uhr:";} else {  
  InTempOnOff1 =  "id= 'Rvalue'>Temperatur an der Uhr:";}}
  if (InHumMessage == "") { 
  InTempOnOff2 =  "<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Innentemperatur Anzeigen</button></a>";} else {
  InTempOnOff2 =  "<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Innentemperatur Anzeigen</button></a>"; 
  InTempOnOff2 += "<h9>"+InHumMessage+"</h9>";}
  InTempOnOff2 += "<p></p>";
  InTempOnOff2 += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  } else {InTempOnOff1 = "";
          InTempOnOff1 = "";}
  } else {InTempOnOff1 = "";
          InTempOnOff2 = "";
          InTempValue = "-";}

  // Check HTML Teilstring
 //  if (SerialMonitor) {Serial.println("<a"+InTempOnOff2+"</a>\");} // Only for Debuging 
// 1. Teil des HTML-Strings
  HTMLString = "<!DOCTYPE html> <html lang='de'>"; 
  HTMLString += "<head>";
  HTMLString += "<title>LED-Matrix-Uhr</title>";
  HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += "<style>";
  HTMLString += "body { background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
  HTMLString += "h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }";
  HTMLString += "h2 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }";
  HTMLString += "h3 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }";
  HTMLString += "h4 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}";
  HTMLString += "h5 { color: white;font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}";
  HTMLString += "h6 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}";
  HTMLString += "h7 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}";
  HTMLString += "h8 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #f74f4f; font-weight: bold;}";
  HTMLString += "h9 { font-size: 18px; margin-left: 80px; margin-top: 0px; margin-bottom: 0px; color: #f79d36; font-weight: bold;}";
  HTMLString += "#Rvalue {color: #FC3A30;}";
  HTMLString += "#Gvalue {color: lime;}";
  HTMLString += "#Bvalue {color: #2ECCFA;}";
  HTMLString += "</style>";
  HTMLString += "</head>";
  HTMLString += "<body>";
  HTMLString += "<h1>LED-Matrix-Uhr</h1>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += InTempOnOff2;
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){ 
// Auswahl Helligkeit der Matrix und Runtime der Uhr
  String HTMLString = "";
  // 2. Teil des HTML-Strings
  HTMLString = "<h2>Auswahl - Display EIN / AUS / Test / Helligkeitsstufen:</h2>";
  if (!Display) { 
  HTMLString += OnOffAndLevelButton("Display EIN",false,"mon","","110","10",""); 
  HTMLString += OnOffAndLevelButton("Display AUS",true,"moff","","110","20","");
  HTMLString += OnOffAndLevelButton("Display Test",false,"","","110","20","disabled");} else {
  HTMLString += OnOffAndLevelButton("Display EIN",true,"mon","","110","10","");  
  HTMLString += OnOffAndLevelButton("Display AUS",false,"moff","","110","20","");
  HTMLString += OnOffAndLevelButton("Display Test",false,"mdt","","110","20","");}

  if (!Display) {  
  if (Brightness == "L1") {HTMLString += OnOffAndLevelButton("Stufe",true,""," 1","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 1","90","10","disabled");}
  if (Brightness == "L2") {HTMLString += OnOffAndLevelButton("Stufe",true,""," 2","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 2","90","10","disabled");}
  if (Brightness == "L3") {HTMLString += OnOffAndLevelButton("Stufe",true,""," 3","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 3","90","10","disabled");}
  if (Brightness == "L4") {HTMLString += OnOffAndLevelButton("Stufe",true,""," 4","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 4","90","10","disabled");}
  if (Brightness == "L5") {HTMLString += OnOffAndLevelButton("Stufe",true,""," 5","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 5","90","10","disabled");}} 
  else {
  if (Brightness == "L1") {HTMLString += OnOffAndLevelButton("Stufe",true,"mb1"," 1","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"mb1"," 1","90","10","");}
  if (Brightness == "L2") {HTMLString += OnOffAndLevelButton("Stufe",true,"mb2"," 2","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"mb2"," 2","90","10","");}
  if (Brightness == "L3") {HTMLString += OnOffAndLevelButton("Stufe",true,"mb3"," 3","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"mb3"," 3","90","10","");}
  if (Brightness == "L4") {HTMLString += OnOffAndLevelButton("Stufe",true,"mb4"," 4","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"mb4"," 4","90","10","");}
  if (Brightness == "L5") {HTMLString += OnOffAndLevelButton("Stufe",true,"mb5"," 5","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"mb5"," 5","90","10","");}}  
  HTMLString += "<p></p>";
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_3(){
String HTMLString = "";
String DisplayAutomaticOnOff = "";  
// Display automatisch EIN/AUS wenn in Access Point angegeben
  if (DisplayOnTime != "" && DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr<br />Display automatisch AUS um "+FDOffTime+" Uhr";} else {
  if (DisplayOnTime == "" && DisplayOffTime == ""){
  DisplayAutomaticOnOff = "Display automatisch EIN / AUS ist deaktiviert";} else {  
  if (DisplayOnTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr";} else {
  if (DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch AUS um "+FDOffTime+" Uhr";}}}}
// 3. Teil des HTML-Strings
HTMLString ="<h4>"+DisplayAutomaticOnOff+"</h4>";
HTMLString += "<a href =\"/cdof\"><button class=\"button\" style=\"margin-right: 10px; height: 24px; font-size: 14px; font-weight: bold; width: 80px;\">Ändern</button></a>";
HTMLString += "<p></p>";
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}  

/********************************************************************************************************************************************************/

String RootHTMLString_4(){ // Status Doppelpunkt
  String HTMLString = "";
  // 4. Teil des HTML-Strings
  HTMLString = "<h2>Auswahl - Status Doppelpunkt der Uhrzeit:</h2>";
  if (!Display) {
  if (DoublePointStatus == 2){
  HTMLString += OnOffAndLevelButton("Blinken",true,"","","90","10","disabled");} else {
  HTMLString += OnOffAndLevelButton("Blinken",false,"","","90","10","disabled");}
  if (DoublePointStatus == 1){
  HTMLString += OnOffAndLevelButton("Permanent EIN",true,"","","130","10","disabled");} else {
  HTMLString += OnOffAndLevelButton("Permanent EIN",false,"","","130","10","disabled");}
  if (DoublePointStatus == 0){
  HTMLString += OnOffAndLevelButton("Permanent AUS",true,"","","130","10","disabled");} else {
  HTMLString += OnOffAndLevelButton("Permanent AUS",false,"","","130","10","disabled");}  
  } else {
  if (DoublePointStatus == 2){
  HTMLString += OnOffAndLevelButton("Blinken",true,"sdflash","","90","10","");} else {
  HTMLString += OnOffAndLevelButton("Blinken",false,"sdflash","","90","10","");}
  if (DoublePointStatus == 1){
  HTMLString += OnOffAndLevelButton("Permanent EIN",true,"sdon","","130","10","");} else {
  HTMLString += OnOffAndLevelButton("Permanent EIN",false,"sdon","","130","10","");}
  if (DoublePointStatus == 0){
  HTMLString += OnOffAndLevelButton("Permanent AUS",true,"sdoff","","130","10","");} else {
  HTMLString += OnOffAndLevelButton("Permanent AUS",false,"sdoff","","130","10","");}}
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_5(){ // ScrollText Geschwindigkeitslevel
String HTMLString = "";
  // 5. Teil des HTML-Strings  
  HTMLString = "<h2>Auswahl - Lauftextgeschwindigkeit:</h2>"; 
  if (!Display) { 
  if (ScrollTextTime == 60) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 1","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 1","90","10","disabled");}
  if (ScrollTextTime == 55) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 2","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 2","90","10","disabled");}
  if (ScrollTextTime == 50) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 3","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 3","90","10","disabled");}
  if (ScrollTextTime == 45) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 4","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 4","90","10","disabled");}
  if (ScrollTextTime == 40) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 5","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 5","90","10","disabled");}
  if (ScrollTextTime == 35) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 6","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 6","90","10","disabled");}
  if (ScrollTextTime == 30) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 7","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 7","90","10","disabled");}
  if (ScrollTextTime == 25) {HTMLString += OnOffAndLevelButton("Stufe",true,""," 8","90","10","disabled");} else {HTMLString += OnOffAndLevelButton("Stufe",false,""," 8","90","10","disabled");} 
  } else { 
  if (ScrollTextTime == 60) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt1"," 1","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt1"," 1","90","10","");}
  if (ScrollTextTime == 55) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt2"," 2","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt2"," 2","90","10","");}
  if (ScrollTextTime == 50) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt3"," 3","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt3"," 3","90","10","");}
  if (ScrollTextTime == 45) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt4"," 4","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt4"," 4","90","10","");}
  if (ScrollTextTime == 40) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt5"," 5","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt5"," 5","90","10","");}
  if (ScrollTextTime == 35) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt6"," 6","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt6"," 6","90","10","");}
  if (ScrollTextTime == 30) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt7"," 7","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt7"," 7","90","10","");}
  if (ScrollTextTime == 25) {HTMLString += OnOffAndLevelButton("Stufe",true,"vlt8"," 8","90","10","");} else {HTMLString += OnOffAndLevelButton("Stufe",false,"vlt8"," 8","90","10","");}}
  HTMLString += "<p></p>"; 
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_6(){ // Zusatzfunktionen
String HTMLString = "";
String EF = "";
if (EnhancedFunctions) {
EF =  OnOffAndLevelButton("EIN",true,"zfon","","90","10","");
EF += OnOffAndLevelButton("AUS",false,"zfoff","","90","10","");} else {
EF =  OnOffAndLevelButton("EIN",false,"zfon","","90","10","");
EF += OnOffAndLevelButton("AUS",true,"zfoff","","90","10","");}
// 6. Teil des HTML-Strings
if (IsFederalStateOK(FederalState)) { 
HTMLString = "<h2>Auswahl - Zusatzfunktionen &nbsp;EIN / AUS: &nbsp;(Datum, Feiertage, Geburtstage)</h2>";} else {
HTMLString = "<h2>Auswahl - Zusatzfunktionen &nbsp;EIN / AUS: &nbsp;(Datum, Geburtstage)</h2>";}  
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
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\"></a>";}}
HTMLString += "<p></p>";
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_7() { // Anzeige Wetterdaten von OpenWeatherMap
// Wetterdaten aktivieren / deaktivieren
String HTMLString = "";
String OWMA = "";
// OpenWeatherMap Status anzeigen oder nicht
if (AccessOpenWeatherMap) {
if (WeatherFunctions) {
OWMA =  OnOffAndLevelButton("EIN",true,"owmfon","","90","10","");
OWMA += OnOffAndLevelButton("AUS",false,"owmfoff","","90","10","");} else {
OWMA =  OnOffAndLevelButton("EIN",false,"owmfon","","90","10","");  
OWMA += OnOffAndLevelButton("AUS",true,"owmfoff","","90","10","");}
if (!WeatherFunctions) {StatusOpenWeatherMap = "deaktiviert";} else {
if (OWMAdjustChange) {StatusOpenWeatherMap = "aktiviert"; OWMAdjustChange = false;}}
if (WeatherFunctions && StatusOpenWeatherMap == "aktiviert" && Display) {
OWMA += "<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;aktiviert</a>";} 
if (WeatherFunctions && StatusOpenWeatherMap == "OK" && Display) {
String WSTemp = String(temp, 0); WSTemp.trim();
if (IsNumeric(WSTemp) && IsNumeric(String(humidity))) { // Check Temperatur und Luftfeuchte 
OWMA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+OWMLastConnectionDateTime+"";
OWMA += "&nbsp;&nbsp;&nbsp;&nbsp;(AT=&nbsp;"+WSTemp+"°C &nbsp;/&nbsp; AL=&nbsp;"+String(humidity)+"%)</a>";} else {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+OWMLastConnectionDateTime+"</a>";}} 
if (WeatherFunctions && StatusOpenWeatherMap == "Fehler" && Display) {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+OWMLastConnectionDateTime+"</a>";}
if (WeatherFunctions && !Display) {
OWMA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ kein Herunterladen von Wetterdaten wenn Display AUS +++</a>";}
} else {
OWMA = "href =\"/\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 90px;\" disabled>EIN</button></a>";
OWMA += "<a href =\"/\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\"disabled>AUS*</button></a>";
OWMA += "<a style=\"font-family:arial;color:#ff525a;font-size: 16px; font-weight: bold; margin-left: 40px;\">OpenWeatherMap ist nicht aktiv !";}
// 7. Teil des HTML-Strings  
HTMLString =  "<h2>Auswahl - aktuelle Wetterdaten von ---OpenWeatherMAP--- anzeigen &nbsp; EIN / AUS / STATUS:</h2>";
HTMLString += OWMA;
HTMLString += "<p></p>";
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_8() { // Anzeige NewsAPI Nachrichten
String HTMLString = "";
String ANA = "";
  if (AccessNewsAPI) {
// NewsAPI Einschalten / Ausschalten
if (NewsAPIFunction) {
ANA =  OnOffAndLevelButton("EIN",true,"nafon","","90","10","");
ANA += OnOffAndLevelButton("AUS",false,"nafoff","","90","10","");} else {
ANA =  OnOffAndLevelButton("EIN",false,"nafon","","90","10","");
ANA += OnOffAndLevelButton("AUS",true,"nafoff","","90","10","");} 
if (!NewsAPIFunction) {StatusNewsAPI = "deaktiviert";} else {
if (NAAdjustChange) {StatusNewsAPI = "aktiviert"; NAAdjustChange = false;}} 

if (NewsAPIFunction && StatusNewsAPI == "aktiviert" && Display) {
ANA += "<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;aktiviert</a>";} 
if (NewsAPIFunction && StatusNewsAPI == "OK" && Display) {
ANA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;OK - "+NewsAPILastConnectionDateTime+"</a>";} 
if (NewsAPIFunction && StatusNewsAPI == "Fehler" && Display) {
ANA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Status: &nbsp;Fehler - "+NewsAPILastConnectionDateTime+"</a>";}
if (NewsAPIFunction && !Display) {
ANA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ kein Herunterladen von Nachrichten wenn Display AUS +++</a>";}
} else {
ANA = "href =\"/\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 90px;\" disabled>EIN</button></a>";
ANA += "<a href =\"/\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 90px;\"disabled>AUS*</button></a>";
ANA += "<a style=\"font-family:arial;color:#ff525a;font-size: 16px; font-weight: bold; margin-left: 40px;\">NewsAPI ist nicht aktiv !";}

// 8. Teil des HTML-Strings 
HTMLString = "<h2>Auswahl - aktuelle Nachrichten von ---NewsAPI--- anzeigen &nbsp; EIN / AUS / STATUS:</h2>";
HTMLString += ANA;
HTMLString += "<p></p>";
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_9() { // Einstellungen für passiv Piezo Buzzer 
String HTMLString = "";
String BSS = "";
String BHS = "";
// Startsound Einschalten / Ausschalten
  if (StartSound == "On") { 
  BSS =  OnOffAndLevelButton("Startsound EIN",true,"sspbson","","140","10",""); 
  BSS += OnOffAndLevelButton("Startsound AUS",false,"sspbsoff","","140","10","");} else {
  BSS =  OnOffAndLevelButton("Startsound EIN",false,"sspbson","","140","10",""); 
  BSS += OnOffAndLevelButton("Startsound AUS",true,"sspbsoff","","140","10","");} 
  // Startsound Einschalten / Ausschalten
  if (HourSound == "On") {
  BHS =  OnOffAndLevelButton("Stundensound EIN",true,"hspbson","","150","10","");
  BHS += OnOffAndLevelButton("Stundensound AUS",false,"hspbsoff","","150","10","");} else {
  BHS =  OnOffAndLevelButton("Stundensound EIN",false,"hspbson","","150","10","");
  BHS += OnOffAndLevelButton("Stundensound AUS",true,"hspbsoff","","150","10","");} 

// 9. Teil des HTML-Strings   
HTMLString = "<h2>Auswahl - Sound Einstellungen:</h2>";
HTMLString += ""+BSS+BHS+"";
HTMLString += "<p></p>"; 
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_10() { // Einstellungen aus Captiv-Portal
String HTMLString = "";
// 10. Teil des HTML-Strings  
HTMLString = "<h2>Anzeige - Einstellungen aus dem Access Point (Captive-Portal):</h2>";
HTMLString += "<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 130px;\">Einstellungen</button></a>";
HTMLString += "<p></p>";
HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_11() { // Softwareversion
String HTMLString = "";
String rtime = "";
rtime = runtime(); // Laufzeit der Uhr (Clock Runtime)
int ESPRAM = ESP.getMaxFreeBlockSize() / 1024;  // Check RAM
// 11. Teil des HTML-Strings  
HTMLString = "<h7>Software-Version: &nbsp;"+LMCVersion+"</h7>";
HTMLString += "<a><br /></a>";
if (ESPRAM > 2) {
HTMLString += "<h7>Freier Arbeitsspeicher: &nbsp;&nbsp;"+String(ESPRAM)+" kB</h7>";} else {
HTMLString += "<h8>Freier Arbeitsspeicher: &nbsp;&nbsp;"+String(ESPRAM)+" kB (Achtung ! - zu wenig)</h8>";}
HTMLString += "<a><br /></a>";
HTMLString += "<h7>Laufzeit der Uhr: &nbsp;&nbsp;"+rtime+"</h7>";
HTMLString += "<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px; height: 48px; width: 180px;color: red;float: right; font-size: 16px;\">Zurücksetzen! (Reset)</button></a>";
HTMLString += "<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px; height: 48px; width: 120px;color: blue;float: right; font-size: 18px;\">Neustart</button></a>";
HTMLString += "<a><br /></a>";
HTMLString += "<a><br /></a>";
HTMLString += "<h6>*) - aktuelle Auswahl</h6>";
HTMLString += "</body>";
HTMLString += "</html>";
return HTMLString;}

/********************************************************************************************************************************************************/ 

void HTMLAnswer(String WebFunction) {
String HTMLString = "";
String HTMLAnswerString = "";  
if (WebFunction == "MatrixOn") {HTMLAnswerString = "Das LED-Matrix Display wurde eingeschaltet";} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = "Das LED-Matrix Display wurde ausgeschaltet";} 
if (WebFunction == "MatrixIsOn") {HTMLAnswerString = "Das LED-Matrix Display ist bereits eingeschaltet";} 
if (WebFunction == "MatrixIsOff") {HTMLAnswerString = "Das LED-Matrix Display ist bereits ausgeschaltet";}
if (WebFunction == "RebootClock") {HTMLAnswerString = "Wenn Sie auf die Startseite zurückkehren<br />wird die LED Matrix Uhr neu gestartet";}
if (WebFunction == "DoublePointFlash") {HTMLAnswerString = "Der Doppelpunkt der Uhr blinkt ab jetzt.";} 
if (WebFunction == "DoublePointOn") {HTMLAnswerString = "Der Doppelpunkt der Uhr ist jetzt permanent an.";} 
if (WebFunction == "DoublePointOff") {HTMLAnswerString = "Der Doppelpunkt der Uhr ist jetzt permanent aus.";} 
if (WebFunction == "BrightnessLevel1") {HTMLAnswerString = "LED-Matrix Helligkeitsstufe 1 wurde aktiviert";} 
if (WebFunction == "BrightnessLevel2") {HTMLAnswerString = "LED-Matrix Helligkeitsstufe 2 wurde aktiviert";} 
if (WebFunction == "BrightnessLevel3") {HTMLAnswerString = "LED-Matrix Helligkeitsstufe 3 wurde aktiviert";} 
if (WebFunction == "BrightnessLevel4") {HTMLAnswerString = "LED-Matrix Helligkeitsstufe 4 wurde aktiviert";} 
if (WebFunction == "BrightnessLevel5") {HTMLAnswerString = "LED-Matrix Helligkeitsstufe 5 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel1") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 1 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel2") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 2 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel3") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 3 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel4") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 4 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel5") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 5 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel6") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 6 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel7") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 7 wurde aktiviert";} 
if (WebFunction == "ScrollTextTimeLevel8") {HTMLAnswerString = "Die Laufschriftgeschwindigkeit Stufe 8 wurde aktiviert";} 
if (WebFunction == "WeatherFunctionsEin1") {HTMLAnswerString = "Die OpenWeatherMap - Wetterdaten wurden eingeschaltet";}
if (WebFunction == "WeatherFunctionsEin2") {HTMLAnswerString = "Die OpenWeatherMap - Wetterdaten sind bereits eingeschaltet";}
if (WebFunction == "WeatherFunctionsAus")  {HTMLAnswerString = "Die OpenWeatherMap - Wetterdaten wurden ausgeschaltet";}
if (WebFunction == "NewsAPIFunctionEin1")  {HTMLAnswerString = "Die NewsAPI - aktuelle Nachrichten wurden eingeschaltet";}
if (WebFunction == "NewsAPIFunctionEin2")  {HTMLAnswerString = "Die NewsAPI - aktuelle Nachrichten sind bereits eingeschaltet";}
if (WebFunction == "NewsAPIFunctionAus")   {HTMLAnswerString = "Die NewsAPI - aktuelle Nachrichten wurden ausgeschaltet";}
if (IsFederalStateOK(FederalState)) { 
if (WebFunction == "EnhancedFunctionsEin1") {HTMLAnswerString = "Die Zusatzfunktionen wurden eingeschaltet<br />(Datum, Feiertage, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = "Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Feiertage, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = "Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Feiertage, Geburtstage)";}} else {
if (WebFunction == "EnhancedFunctionsEin1") {HTMLAnswerString = "Die Zusatzfunktionen wurden eingeschaltet<br />(Datum, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = "Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = "Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Geburtstage)";}}
if (WebFunction == "StartSoundOnM") {HTMLAnswerString = "Der Uhr-Startsound wurde eingeschaltet.";}
if (WebFunction == "StartSoundOnMIs") {HTMLAnswerString = "Der Uhr-Startsound ist bereits eingeschaltet.";}
if (WebFunction == "StartSoundOffM") {HTMLAnswerString = "Der Uhr-Startsound wurde ausgeschaltet.";}
if (WebFunction == "StartSoundOffMIs") {HTMLAnswerString = "Der Uhr-Startsound ist bereits ausgeschaltet.";}
if (WebFunction == "HourSoundOnM") {HTMLAnswerString = "Der Uhr-Stundensound wurde eingeschaltet.";}
if (WebFunction == "HourSoundOnMIs") {HTMLAnswerString = "Der Uhr-Stundensound ist bereits eingeschaltet.";}
if (WebFunction == "HourSoundOffM") {HTMLAnswerString = "Der Uhr-Stundensound wurde ausgeschaltet.";}
if (WebFunction == "HourSoundOffMIs") {HTMLAnswerString = "Der Uhr-Stundensound ist bereits ausgeschaltet.";}
if (WebFunction == "DisplayCheck") {HTMLAnswerString = "LED-Matrix Display Test läuft. Bitte warten ...";}
if (WebFunction == "NoDisplayCheck") {HTMLAnswerString = "********* Display-Test derzeit nicht möglich. *********<br />Bitte warten Sie bis die Uhrzeit wieder angezeigt wird.";}
if (WebFunction == "ResetClockYes") {HTMLAnswerString = "Wenn Sie auf die Startseite zurückkehren wird<br />der Access Point (Captive-Portal) gestartet<br />und die Einstellungen auf Standard gesetzt.";}
HTMLString = "<!DOCTYPE html> <html lang='de'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <a><br /></a>\
    <h2>"+HTMLAnswerString+"</h2>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>\
  </body>\
    </html>"; 
SendHTMLString(HTMLString); // Send HTMLString 
HTMLString = "";}

/********************************************************************************************************************************************************/

// Matrix einschalten
void MatrixOn() {
if (!await){  
if (!Display) {  
HTMLAnswer("MatrixOn");
if (SerialMonitor) {Serial.println("Die LED-Matrix wurde eingeschaltet.");}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display an
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
Display = true;} else {HTMLAnswer("MatrixIsOn");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Matrix ausschalten
void MatrixOff() { 
if (!await){  
if (Display) {  
dots = 1; // Doppelpunkt an  
Display = false;   
HTMLAnswer("MatrixOff");
if (SerialMonitor) {Serial.println("Die LED-Matrix wurde ausgeschaltet.");} 
printStringWithShift("           ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,0);} else {HTMLAnswer("MatrixIsOff");} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointFlash() {
if (!await){      
DoublePointStatus = 2; // Doppelpunkt der Uhr blinken lassen 
// Status Doppelpunkt
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointFlash");
if (SerialMonitor) {Serial.println("Doppelpunkt der Uhr blinkt.");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointOn() {
if (!await){  
DoublePointStatus = 1; // Doppelpunkt der Uhr permanant AN
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOn");
if (SerialMonitor) {Serial.println("Doppelpunkt der Uhr permanant an.");}
} else {PleaseWait();}}


/********************************************************************************************************************************************************/

void DoublePointOff() {
if (!await){  
DoublePointStatus = 0; // Doppelpunkt der Uhr permanant aus
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOff");
if (SerialMonitor) {Serial.println("Doppelpunkt der Uhr permanant aus.");}
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

void SetBrightnessLevel(String BLText, int BLInt) {
if (!await){
// Serial.println("Matrix: Helligkeitsstufe 5 aktiviert");
sendCmdAll(CMD_INTENSITY, BLInt-1);
Brightness = BLText;
if (littleFSInit) {
SetupSave("matrix_brightness.txt", Brightness);}
HTMLAnswer("BrightnessLevel"+String(BLInt));
if (SerialMonitor) {Serial.println("Helligkeitsstufe "+String(BLInt)+" aktiviert");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Geschwindigkeit der Laufschrift einstellen
void ScrollTextTimeLevel1() {
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe1");
SetScrollTimeAndLevel(60,1);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2() { 
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe2");
SetScrollTimeAndLevel(55,2); }

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel3() { 
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe3");
SetScrollTimeAndLevel(50,3);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel4() {
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe4");
SetScrollTimeAndLevel(45,4);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel5() {  
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe5");
SetScrollTimeAndLevel(40,5);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel6() { 
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe6");
SetScrollTimeAndLevel(35,6);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel7() {  
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe7");
SetScrollTimeAndLevel(30,7);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel8() {
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe8");
SetScrollTimeAndLevel(25,8);}

/********************************************************************************************************************************************************/

void SetScrollTimeAndLevel(int STTime, int STLevel){
if (!await) {
await = true;   
ScrollTextTime = STTime; // Lauftextgeschwindigkeit   
if (littleFSInit) {
SetupSave("status_scrolltexttime.txt", String(STTime));}
HTMLAnswer("ScrollTextTimeLevel"+String(STLevel)+"");
if (SerialMonitor) {Serial.println("Laufschriftgeschwindigkeit Stufe"+String(STLevel)+" aktiviert");}
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
StatusOpenWeatherMap = "aktiviert";
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "On");} 
HTMLAnswer("WeatherFunctionsEin1");
if (SerialMonitor) {Serial.println("OpenWeatherMap - Wetterdaten eingeschaltet");}}
else {
HTMLAnswer("WeatherFunctionsEin2");
if (SerialMonitor) {Serial.println("OpenWeatherMap - Wetterdaten sind bereits eingeschaltet");}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOff() {
if (!await) {   
// Serial.println("Zusatzfunktionen ausgeschaltet");
WeatherFunctions = false;
StatusOpenWeatherMap = "deaktiviert";
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "Off");} 
HTMLAnswer("WeatherFunctionsAus");
if (SerialMonitor) {Serial.println("OpenWeatherMap - Wetterdaten ausgeschaltet");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsAPIFunctionOn() {
if (!await) {   
// Serial.println("Zusatzfunktionen eingeschaltet");
if (!NewsAPIFunction) {
NewsAPIFunction = true;
NAAdjustChange = true;
StatusNewsAPI = "aktiviert";
if (littleFSInit) {
SetupSave("status_newsapifunctions.txt", "On");}  
HTMLAnswer("NewsAPIFunctionEin1");
if (SerialMonitor) {Serial.println("NewsAPI - aktuelle Nachrichten eingeschaltet");}}
else {
HTMLAnswer("NewsAPIFunctionEin2");
if (SerialMonitor) {Serial.println("NewsAPI - aktuelle Nachrichten sind bereits eingeschaltet");}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsAPIFunctionOff() {
if (!await) {   
// Serial.println("Zusatzfunktionen ausgeschaltet");
NewsAPIFunction = false;
StatusNewsAPI = "deaktiviert";
if (littleFSInit) {
SetupSave("status_newsapifunctions.txt", "Off");}  
HTMLAnswer("NewsAPIFunctionAus");
if (SerialMonitor) {Serial.println("NewsAPI - aktuelle Nachrichten ausgeschaltet");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOn() {
if (!await) {   
// Serial.println("Zusatzfunktionen eingeschaltet");
if (!EnhancedFunctions) {
EnhancedFunctions = true;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "On");} 
HTMLAnswer("EnhancedFunctionsEin1");} else {
HTMLAnswer("EnhancedFunctionsEin2");} 
if (IsFederalStateOK(FederalState)) {  
if (SerialMonitor) {Serial.println("Zusatzfunktionen eingeschaltet - (Datum, Feiertage, Geburtstage)");}} else {
if (SerialMonitor) {Serial.println("Zusatzfunktionen eingeschaltet - (Datum, Geburtstage)");}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOff() {
if (!await) {   
// Serial.println("Zusatzfunktionen ausgeschaltet");
EnhancedFunctions = false;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "Off");} 
HTMLAnswer("EnhancedFunctionsAus");
if (IsFederalStateOK(FederalState)) {  
if (SerialMonitor) {Serial.println("Zusatzfunktionen ausgeschaltet - (Datum, Feiertage, Geburtstage)");}} else {
if (SerialMonitor) {Serial.println("Zusatzfunktionen ausgeschaltet - (Datum, Geburtstage)");}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GetAdjustments() {
String OWMAPI = "";
 String OWMCID = "";
  String NewsAPIKey = "";
   String DHTTempOffset = "";
    String DHTHumOffset = "";
   String InTempValue = "";
  String CheckIP = ""; 
String HTMLString = "";
InTempValue = InTempMessure(); // Messung
// Check IP-Adresse
if (cip.length() > 15 || cip.length() < 7) {CheckIP = "k.A.";} else {CheckIP = cip;} // Check IP-Adresse
if (AccessOpenWeatherMap){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = "k.A.";OWMCID = "k.A.";}
if (newsKey != ""){NewsAPIKey = newsKey;} else {NewsAPIKey = "k.A.";}
if (InTempValue != "-") {
if (InTempOffsetInt > 0) {
if (DHT11Sensor) {  
DHTTempOffset = "DHT11-Sensor Temperatur Offset:&nbsp;&nbsp;+"+InTempOffset+"°C";} else {
DHTTempOffset = "DHT22-Sensor Temperatur Offset:&nbsp;&nbsp;+"+InTempOffset+"°C";}} else {
if (DHT11Sensor) {   
DHTTempOffset = "DHT11-Sensor Temperatur Offset:&nbsp;&nbsp;"+InTempOffset+"°C";} else {
DHTTempOffset = "DHT22-Sensor Temperatur Offset:&nbsp;&nbsp;"+InTempOffset+"°C";}}
if (InHumOffsetInt > 0) {
if (DHT11Sensor) {    
DHTHumOffset = "DHT11-Sensor Luftfeuchte Offset:&nbsp;&nbsp;+"+InHumOffset+"%";} else {
DHTHumOffset = "DHT22-Sensor Luftfeuchte Offset:&nbsp;&nbsp;+"+InHumOffset+"%";}} else {
if (DHT11Sensor) {  
DHTHumOffset = "DHT11-Sensor Luftfeuchte Offset:&nbsp;&nbsp;"+InHumOffset+"%";} else {
DHTHumOffset = "DHT22-Sensor Luftfeuchte Offset:&nbsp;&nbsp;"+InHumOffset+"%";}} 
} else {
DHTTempOffset = "";
DHTHumOffset = "";    
}
HTMLString = "<!DOCTYPE html> <html lang='de'>";
HTMLString += "<head>";
HTMLString += "<title>LED-Matrix-Uhr</title>";
HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
HTMLString += "<style>";
HTMLString += "body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
HTMLString += "h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }";
HTMLString += "h2 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }";
HTMLString += "h3 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }";
HTMLString += "h4 { font-size: 14px; margin-top: 0px; margin-bottom: 1px; color: #ffcc00; float: right;}";
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body>";
HTMLString += "<h1>LED-Matrix-Uhr</h1>";
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "") {
HTMLString += "<h2>( Besitzer der Uhr:&nbsp;&nbsp;"+WatchOwner1FirstName+" "+WatchOwner1LastName+" )</h2>";}
HTMLString += "<a><br /></a>";
HTMLString += "<h3>Einstellungen aus dem Captive Portal</h3>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;"+String(ssid)+"</h2>";
HTMLString += "<h2>IP-Adresse:&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += "<a><br /></a>";
if (IsFederalStateOK(FederalState)) {
HTMLString += "<h2>Deutsches Bundesland:&nbsp;&nbsp;"+LongFederalState(FederalState)+"</h2>";
HTMLString += "<a><br /></a>";}
HTMLString += "<h2>OpenWeatherMap API-Key:&nbsp;&nbsp;"+OWMAPI+"</h2>";
if (WeatherFunctions && String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
HTMLString += "<h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+" ( "+String(WeatherLocation)+" )</h2>";} else {  
HTMLString += "<h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>";}
HTMLString += "<a><br /></a>";
HTMLString += "<h2>NewsAPI API-Key:&nbsp;&nbsp;"+NewsAPIKey+"</h2>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>"+DHTTempOffset+"</h2>";
HTMLString += "<h2>"+DHTHumOffset+"</h2>";
HTMLString += "<a><br /></a>";
if (IsBuzzer) {
HTMLString += "<h2>Passiv Buzzer installiert</h2>";} else {
HTMLString += "<h2>Passiv Buzzer nicht installiert oder deaktiviert</h2>";}
HTMLString += "<a><br /></a>";
HTMLString += "<a href =\"/\"><button class=\"button\" style=\"margin-left: 1px; height: 48px; width: 160px\">zurück zur Startseite</button></a>";
HTMLString += "<a><br /></a>";
HTMLString += "<a><br /></a>";
HTMLString += "</body>";
HTMLString += "</html>";
SendHTMLString(HTMLString); // Send HTMLString
HTMLString = "";
if (SerialMonitor) {Serial.println("Einstellungen aus dem Access Point (Captive Portal) aufgerufen");}}

/********************************************************************************************************************************************************/

String OnOffAndLevelButton(String BMark, bool Bselect, String BHRef, String Blevel, String Bwidth, String BRmargin, String Bdisable) {
String HTMLString = "";
if (Bselect) {
HTMLString  = "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; color: blue; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"*</button></a>";} else {
HTMLString += "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"</button></a>";}
return HTMLString;}

/********************************************************************************************************************************************************/

void ResetClockSettings() {
if (!await) {   
String HTMLString = "";
HTMLString = "<!DOCTYPE html> <html lang='de'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;LED-Matrix-Uhr zurücksetzen !</h2>\
     <h2>(Die WLAN-Zugangsdaten werden gelöscht)</h2>\
     <a><br /></a>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Jetzt LED-Matrix-Uhr zurücksetzen ?</h2>\
     <a><br /></a>\
     <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 70px;height: 48px; width: 110px; font-size: 22px; color: red\">JA</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">NEIN</button></a>\
  </body>\
    </html>";  
SendHTMLString(HTMLString); // Send HTMLString
HTMLString = "";
} else {PleaseWait();}}

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
String HTMLString = "";
HTMLString = "<!DOCTYPE html> <html lang='de'>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <a><br /></a>\
    <h2>Diese Seite wurde nicht gefunden !</h2>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>\
  </body>\
    </html>"; 
SendHTMLString(HTMLString); // Send HTMLString
HTMLString = "";}

/********************************************************************************************************************************************************/

void ChangeDisplayAutoOnOff()
{
String HTMLString;
bool MDTime;
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
    } else {DisplayOnTime = "!";}
    } else {DisplayOnTime = "!";}} else {
    if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = "!";}}
    if (DisplayOnTime != "!") {
    int DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = CheckMDHourOn.toInt();} else {DisplayOnTime = "!";}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = CheckMDMinOn.toInt();} else {DisplayOnTime = "!";}
    if (DisplayOnTime != "" && DisplayOnTime != "!" ) {
    for (int i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = "!";} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = "!";}}}}}

    if (DisplayOffTime != "") {
    MDTime = false;
    if (DisplayOffTime.length() != 5) {
    if (DisplayOffTime.length() == 4) {
    if (DisplayOffTime.indexOf(':') == 1) {
    DisplayOffTime = "0"+DisplayOffTime;
    } else {DisplayOffTime = "!";}
    } else {DisplayOffTime = "!";}} else {
    if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = "!";}}
    if (DisplayOffTime != "!") {
    int DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = CheckMDHourOff.toInt();} else {DisplayOffTime = "!";}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = CheckMDMinOff.toInt();} else {DisplayOffTime = "!";}
    if (DisplayOffTime != "" && DisplayOffTime != "!") {
    for (int i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = "!";} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = "!";}}}}}

    FDOnTime =  DisplayOnTime; // nur für Anzeige
    FDOffTime = DisplayOffTime; // nur für Anzeige

  /*if (SerialMonitor) {
    Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println("");} // for Debug Mode*/

    // Wenn Eingabefehler
    if (DisplayOnTime == "!" || DisplayOffTime == "!") {
    if (DisplayOnTime == "!") {DisplayOnTime = "";} 
    if (DisplayOffTime == "!") {DisplayOffTime = "";}  
    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>LED-Matrix-Uhr</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Ungültige Eingabe(n) - Bitte korrigieren!</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/cdof\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    SendHTMLString(HTMLString); // Send HTMLString
    } else {
    if (littleFSInit){
    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>LED-Matrix-Uhr</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Einstellungen wurden gespeichert<br /><br />Die Uhr wird neu gestartet wenn Sie auf die Startseite zurückkehren</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";   
    SendHTMLString(HTMLString); // Send HTMLString
    // *** Eingaben im FileSystem speichern ***
    // Matrix Display Einschaltzeit
    if (DisplayOnTime != ""){
    SetupSave("display-on-time.txt", DisplayOnTime);} else {
    if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
    // Matrix Display Ausschaltzeit
    if (DisplayOffTime != ""){
    SetupSave("display-off-time.txt", DisplayOffTime);} else {
    if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
    SetupSave("display-change-time.txt", "True"); // Display On/Off Daten wurden geändert
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {Serial.println("Display automatisch EIN/AUS  Daten gespeichert... ");} 
    // Reboot auslösen wenn auf Startseite zurück
    RebootNow = true;
    }
     else
     { // Zugangsdaten wurden nicht gespeichert
     if (SerialMonitor) {Serial.println("Zugangsdaten und Einstellungen wurden nicht gespeichert !");}
    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>LED-Matrix-Uhr</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"; 
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";   
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Fehler beim Speichern der Zugangsdaten und Einstellungen<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>"; 
    SendHTMLString(HTMLString); // Send HTMLString
    // Restart auslösen
    if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Display einschalten wenn Aus 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    digitalWrite(HWLEDPin, LOW); // LED Ausschalten
    ESP.restart();  
    } 
    SendHTMLString(HTMLString); // Send HTMLString 
    } // Ende Eingabefehler
  } else {  
// Einstellungen
    HTMLString =  "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>LED-Matrix-Uhr</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "label {color: white;}";
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}";
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<form action='/cdof' method='post'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display EIN-Zeit (Beispiel: 07:00)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOnT' id='DOnT' value='"+FDOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display AUS-Zeit (Beispiel: 23:30)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOffT' id='DOffT' value='"+FDOffTime+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<h6>*) - keine Angabe deaktiviert die Funktion</h6>";
    HTMLString += "<button type='submit'>Speichern</button>";
    HTMLString += "<br/><br/>";
    HTMLString += "</form>";
    HTMLString += "<a href =\"/cdofc\"><button>Abbrechen</button></a>";
    HTMLString += "<br/><br/>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";  
    SendHTMLString(HTMLString);}} // Send HTMLString

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DisplayCheck() {
if (!await) {
await = true; 
String MDCShow = "      "+String(MDC)+String(MDC)+String(MDC)+String(MDC);
HTMLAnswer("DisplayCheck");
printStringWithShift3(MDCShow.c_str(),ScrollTextTime);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
MyWaitLoop(10000); // kurz warten (10 Sekunden)
HandleOTAandWebServer();  // OTA und Webserver Abfrage
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // OTA und Webserver Abfrage
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
await = false; 
} else {HTMLAnswer("NoDisplayCheck");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ChangeDisplayAutoOnOffCancel() {
String HTMLString = "";
  // Display Einschaltzeit als String
  if (littleFSInit) {
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOnTime = "";}} else
         {DisplayOnTime = "";}
  // Display Ausschaltzeit als String
  if (littleFSInit) {
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Leerzeichen vorne und Hinten entfernen
  } else {DisplayOffTime = "";}} else
         {DisplayOffTime = "";}

  // Prüfen auf Plausibilität der Angaben für Matrix Display Automatisch Ein / Aus
  CheckLEDMatrixDisplayOnOff();

  /*if (SerialMonitor) {
    Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println("");} // for Debug Mode*/

    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>LED-Matrix-Uhr</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    if (littleFSInit) {
    HTMLString += "<h6>Abbruch! - Die alte Einstellung wurde wieder geladen ...</h6>";} else {
    HTMLString += "<h6>Abbruch! - Die alte Einstellung konnte nicht geladen werden !!! ...</h6>";}
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    SendHTMLString(HTMLString); // Send HTMLString
 HTMLString = "";}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOn() {
if (StartSound != "On") { 
if (!await) {  
await = true;
StartBuzzerBeep();    
if (littleFSInit) {
StartSound = "On";   
SetupSave("status_startsound.txt", StartSound);} else {StartSound = "Off";}
HTMLAnswer("StartSoundOnM");
if (SerialMonitor) {Serial.println("Summer (Buzzer) Startsound eingeschaltet");}
await = false;
} else {PleaseWait();}
} else {HTMLAnswer("StartSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOff() {
if (StartSound != "Off") {  
StartSound = "Off";     
if (littleFSInit) {  
SetupSave("status_startsound.txt", StartSound);} 
HTMLAnswer("StartSoundOffM");
if (SerialMonitor) {Serial.println("Summer (Buzzer) Startsound ausgeschaltet");}
} else {HTMLAnswer("StartSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOn() {
if (HourSound != "On") { 
if (!await) {  
await = true;
HourBuzzerBeep();   
if (littleFSInit) {
HourSound = "On";   
SetupSave("status_hoursound.txt", HourSound);} else {HourSound = "Off";}
HTMLAnswer("HourSoundOnM");
await = false;
if (SerialMonitor) {Serial.println("Summer (Buzzer) Stundensound eingeschaltet");}
} else {PleaseWait();}
} else {HTMLAnswer("HourSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOff() {
if (HourSound != "Off") {
HourSound = "Off";  
if (littleFSInit) {
SetupSave("status_hoursound.txt", HourSound);} 
HTMLAnswer("HourSoundOffM");
if (SerialMonitor) {Serial.println("Summer (Buzzer) Stundensound ausgeschaltet");}
} else {HTMLAnswer("HourSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PleaseWait() {
// Bitte warten Seite einlenden
String RefreshTime = "5"; // Aktualisierung der Website nach X Sekunden
String HTMLString;
HTMLString = "<!DOCTYPE html> <html lang='de'>";
HTMLString += "<head>";
HTMLString += "<title>LED-Matrix-Uhr</title>";
HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
// HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
HTMLString += "<style>";
HTMLString += "body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}";
HTMLString += "h1 { font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}";
HTMLString += "h2 { font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}";
HTMLString += "h3 { font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}";
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body>";
HTMLString += "<h1>LED-Matrix-Uhr</h1>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>Bitte warten ...</h2>";
HTMLString += "<h3>(Bis die Uhrzeit wieder im Matrix-Display angezeigt wird)</h3>";
HTMLString += "<a><br /></a>";
HTMLString += "<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>";
HTMLString += "</body>";
HTMLString += "</html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Bitte warten ...");}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HTTPCodeText(int httpcode) {
String TempHTTPCodeText = "k.A.";  
if (httpcode == -1) {TempHTTPCodeText = "ERROR_CONNECTION_REFUSED";}  
if (httpcode == -2) {TempHTTPCodeText = "ERROR_SEND_HEADER_FAILED";} 
if (httpcode == -3) {TempHTTPCodeText = "ERROR_SEND_PAYLOAD_FAILED";} 
if (httpcode == -4) {TempHTTPCodeText = "ERROR_NOT_CONNECTED";} 
if (httpcode == -5) {TempHTTPCodeText = "ERROR_CONNECTION_LOST";} 
if (httpcode == -6) {TempHTTPCodeText = "ERROR_NO_STREAM";} 
if (httpcode == -7) {TempHTTPCodeText = "ERROR_NO_HTTP_SERVER";} 
if (httpcode == -8) {TempHTTPCodeText = "ERROR_TOO_LESS_RAM";} 
if (httpcode == -9) {TempHTTPCodeText = "ERROR_ENCODING";} 
if (httpcode == -10) {TempHTTPCodeText = "ERROR_STREAM_WRITE";} 
if (httpcode == -11) {TempHTTPCodeText = "ERROR_READ_TIMEOUT";}

if (httpcode == 100) {TempHTTPCodeText = "Continue";}
if (httpcode == 101) {TempHTTPCodeText = "Switching Protocols";}
if (httpcode == 102) {TempHTTPCodeText = "Processing (WebDAV)";}
if (httpcode == 200) {TempHTTPCodeText = "OK";}
if (httpcode == 201) {TempHTTPCodeText = "Created";}
if (httpcode == 202) {TempHTTPCodeText = "Accepted";}
if (httpcode == 203) {TempHTTPCodeText = "Non-Authoritative Information";}
if (httpcode == 204) {TempHTTPCodeText = "No Content";}
if (httpcode == 205) {TempHTTPCodeText = "Reset Content";}
if (httpcode == 206) {TempHTTPCodeText = "Partial Content";}
if (httpcode == 207) {TempHTTPCodeText = "Multi-Status (WebDAV)";}
if (httpcode == 208) {TempHTTPCodeText = "Already Reported (WebDAV)";}
if (httpcode == 226) {TempHTTPCodeText = "IM Used";}

if (httpcode == 300) {TempHTTPCodeText = "Multiple Choices";}
if (httpcode == 301) {TempHTTPCodeText = "Moved Permanently";}
if (httpcode == 302) {TempHTTPCodeText = "Found";}
if (httpcode == 303) {TempHTTPCodeText = "See Other";}
if (httpcode == 304) {TempHTTPCodeText = "Not Modified";}
if (httpcode == 305) {TempHTTPCodeText = "Use Proxy";}
if (httpcode == 306) {TempHTTPCodeText = "(Unused)";}
if (httpcode == 307) {TempHTTPCodeText = "Temporary Redirect";}
if (httpcode == 308) {TempHTTPCodeText = "Permanent Redirect (experimental)";}

if (httpcode == 400) {TempHTTPCodeText = "Bad Request";}
if (httpcode == 401) {TempHTTPCodeText = "Unauthorized";}
if (httpcode == 402) {TempHTTPCodeText = "Payment Required";}
if (httpcode == 403) {TempHTTPCodeText = "Forbidden";}
if (httpcode == 404) {TempHTTPCodeText = "Not Found";}
if (httpcode == 405) {TempHTTPCodeText = "Method Not Allowed";}
if (httpcode == 406) {TempHTTPCodeText = "Not Acceptable";}
if (httpcode == 407) {TempHTTPCodeText = "Proxy Authentication Required";}
if (httpcode == 408) {TempHTTPCodeText = "Request Timeout";}
if (httpcode == 409) {TempHTTPCodeText = "Conflict";}
if (httpcode == 410) {TempHTTPCodeText = "Gone";}
if (httpcode == 411) {TempHTTPCodeText = "Length Required";}
if (httpcode == 412) {TempHTTPCodeText = "Precondition Failed";}
if (httpcode == 413) {TempHTTPCodeText = "Request Entity Too Large";}
if (httpcode == 414) {TempHTTPCodeText = "Request-URI Too Long";}
if (httpcode == 415) {TempHTTPCodeText = "Unsupported Media Type";}
if (httpcode == 416) {TempHTTPCodeText = "Requested Range Not Satisfiable";}
if (httpcode == 417) {TempHTTPCodeText = "Expectation Failed";}
if (httpcode == 418) {TempHTTPCodeText = "I'm a teapot (RFC 2324)";}
if (httpcode == 420) {TempHTTPCodeText = "Enhance Your Calm (Twitter)";}
if (httpcode == 422) {TempHTTPCodeText = "Unprocessable Entity (WebDAV)";}
if (httpcode == 423) {TempHTTPCodeText = "Locked (WebDAV)";}
if (httpcode == 424) {TempHTTPCodeText = "Failed Dependency (WebDAV)";}
if (httpcode == 425) {TempHTTPCodeText = "Reserved for WebDAV";}
if (httpcode == 426) {TempHTTPCodeText = "Upgrade Required";}
if (httpcode == 428) {TempHTTPCodeText = "Precondition Required";}
if (httpcode == 429) {TempHTTPCodeText = "Too Many Requests";}
if (httpcode == 431) {TempHTTPCodeText = "Request Header Fields Too Large";}
if (httpcode == 444) {TempHTTPCodeText = "No Response (Nginx)";}
if (httpcode == 449) {TempHTTPCodeText = "Retry With (Microsoft)";}
if (httpcode == 450) {TempHTTPCodeText = "Blocked by Windows Parental Controls (Microsoft)";}
if (httpcode == 451) {TempHTTPCodeText = "Unavailable For Legal Reasons";}
if (httpcode == 499) {TempHTTPCodeText = "Client Closed Request (Nginx)";}

if (httpcode == 500) {TempHTTPCodeText = "Internal Server Error";}
if (httpcode == 501) {TempHTTPCodeText = "Not Implemented";}
if (httpcode == 502) {TempHTTPCodeText = "Bad Gateway";}
if (httpcode == 503) {TempHTTPCodeText = "Service Unavailable";}
if (httpcode == 504) {TempHTTPCodeText = "Gateway Timeout";}
if (httpcode == 505) {TempHTTPCodeText = "HTTP Version Not Supported";}
if (httpcode == 506) {TempHTTPCodeText = "Variant Also Negotiates (Experimental)";}
if (httpcode == 507) {TempHTTPCodeText = "Insufficient Storage (WebDAV)";}
if (httpcode == 508) {TempHTTPCodeText = "Loop Detected (WebDAV)";}
if (httpcode == 509) {TempHTTPCodeText = "Bandwidth Limit Exceeded (Apache)";}
if (httpcode == 510) {TempHTTPCodeText = "Not Extended";}
if (httpcode == 511) {TempHTTPCodeText = "Network Authentication Required";}
if (httpcode == 598) {TempHTTPCodeText = "Network read timeout error";}
if (httpcode == 599) {TempHTTPCodeText = "Network connect timeout error";}
return String(httpcode) + " - " + TempHTTPCodeText;}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
