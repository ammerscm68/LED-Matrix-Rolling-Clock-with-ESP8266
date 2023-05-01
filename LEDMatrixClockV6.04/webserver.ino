// Webserver Startseite
void handleRoot() 
{ // Daten senden
  WebServerWait = true;
  server.setContentLength(RootHTMLString_1().length()+RootHTMLString_2().length());   // (or hardcode the length)
  server.send(200, "text/html",RootHTMLString_1());
  server.sendContent(RootHTMLString_2());
  yield();
  MyWaitLoop(10); // kurze Pause sonst Absturz bei Browser refresh
  WebServerWait = false;
  if (RebootNow == true) {ESP.restart();} // Neustart der Uhr
  if (littleFSInit == true && ResetNow == true) {
  ResetClock();} // Alle Einstellungen auf Standard - WLAN Zugangsdaten werden gelöscht    
}

/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden*/ 
String RefreshTime = "60"; // Aktualisierung der Website nach X Sekunden

 // Auswahl Helligkeit der Matrix und Runtime der Uhr
  String MB1, MB2, MB3, MB4, MB5;
  if (Brightness == "L1") {MB1 = "Stufe 1*";} else {MB1 = "Stufe 1";}
  if (Brightness == "L2") {MB2 = "Stufe 2*";} else {MB2 = "Stufe 2";}
  if (Brightness == "L3") {MB3 = "Stufe 3*";} else {MB3 = "Stufe 3";}
  if (Brightness == "L4") {MB4 = "Stufe 4*";} else {MB4 = "Stufe 4";}
  if (Brightness == "L5") {MB5 = "Stufe 5*";} else {MB5 = "Stufe 5";}
  // Status Doppelpunkt
  String SDP1, SDP2, SDP3;
  if (DoublePointStatus == 2) {SDP1 = "Blinken*";} else {SDP1 = "Blinken";}
  if (DoublePointStatus == 1) {SDP2 = "Permanent Ein*";} else {SDP2 = "Permanent Ein";}
  if (DoublePointStatus == 0) {SDP3 = "Permanent Aus*";} else {SDP3 = "Permanent Aus";}
  // ScrollText Geschwindigkeitslevel
  String STL1, STL2, STL3, STL4, STL5, STL6, STL7, STL8;
  if (ScrollTextTime == 60) {STL1 = "Stufe 1*";} else {STL1 = "Stufe 1";}
  if (ScrollTextTime == 55) {STL2 = "Stufe 2*";} else {STL2 = "Stufe 2";}
  if (ScrollTextTime == 50) {STL3 = "Stufe 3*";} else {STL3 = "Stufe 3";}
  if (ScrollTextTime == 45) {STL4 = "Stufe 4*";} else {STL4 = "Stufe 4";}
  if (ScrollTextTime == 40) {STL5 = "Stufe 5*";} else {STL5 = "Stufe 5";}
  if (ScrollTextTime == 35) {STL6 = "Stufe 6*";} else {STL6 = "Stufe 6";}
  if (ScrollTextTime == 30) {STL7 = "Stufe 7*";} else {STL7 = "Stufe 7";}
  if (ScrollTextTime == 25) {STL8 = "Stufe 8*";} else {STL8 = "Stufe 8";}
  // Zusatzfunktionen
  String EF1, EF2;
  if (EnhancedFunctions == true) {EF1 = "EIN*";} else {EF1 = "EIN";} 
  if (EnhancedFunctions == false) {EF2 = "AUS*";} else {EF2 = "AUS";} 
  
  String HTMLString,DisplayAutomaticOnOff, InTempOnOff1,InTempOnOff2;
  
  // Display automatisch EIN/AUS wenn in Access Point angegeben
  if (DisplayOnTime != "" && DisplayOffTime != "" && DisplayOnTime != "---" && DisplayOffTime != "---"){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr<br />Display automatisch AUS um "+FDOffTime+" Uhr";} else
  {
  if ((DisplayOnTime == "" && DisplayOffTime == "") || (DisplayOnTime == "---" && DisplayOffTime == "---") ||
      (DisplayOnTime == "" && DisplayOffTime == "---") || (DisplayOnTime == "---" && DisplayOffTime == "")){
  DisplayAutomaticOnOff = "Display automatisch EIN / AUS wurde deaktiviert";} else {  
  if (DisplayOnTime != "" && DisplayOnTime != "---"){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr";} else {
  if (DisplayOffTime != "" && DisplayOffTime != "---"){
  DisplayAutomaticOnOff = "Display automatisch AUS um "+FDOffTime+" Uhr";}}}}

  // Innentemperatur anzeigen JA/NEIN (DHT11)
  if (InTempCounter < 5) {
  InTempValue = InTempMessure(); 
  if (InTempValue != "-") {
  int IntInTempValue = InTempValue.toInt();  
  if (IntInTempValue < 10) {
  InTempOnOff1 = "id= 'Bvalue'>Temperatur an der Uhr:";  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 24) {InTempOnOff1 = "id= 'Gvalue'>Temperatur an der Uhr:";} else {  
  InTempOnOff1 = "id= 'Rvalue'>Temperatur an der Uhr:";}}
  InTempOnOff2 = "href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 42px; width: 150px; font-size: 14px; color: blue\">Innentemperatur Anzeigen</button>";
  } else {InTempOnOff1 = "";
          InTempOnOff1 = "";}
  } else {InTempOnOff1 = "";
          InTempOnOff2 = "";
          InTempValue = "-";}

  // Check HTML Teilstring
 //  Serial.println("<a"+InTempOnOff2+"</a>\"); // Only for Debuging 

// 1. Teil des HTML-Strings
 HTMLString = "<!DOCTYPE html> <html>"; 
  HTMLString += "<head>";
  HTMLString += "<title>LED-Matrix-Uhr</title>";
  HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += "<style>";
  HTMLString += "body { background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
  HTMLString += "h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
  HTMLString += "h2 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; }";
  HTMLString += "h3 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; }";
  HTMLString += "h4 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}";
  HTMLString += "h5 { font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}";
  HTMLString += "h6 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}";
  HTMLString += "h7 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}";
  HTMLString += "h8 { font-size: 16px; font-weight: bold;}";
  HTMLString += "#Rvalue {color: #FC3A30;}";
  HTMLString += "#Gvalue {color: lime;}";
  HTMLString += "#Bvalue {color: #2ECCFA;}";
  HTMLString += "</style>";
  HTMLString += "</head>";
  HTMLString += "<body>";
  HTMLString += "<h1>LED-Matrix-Uhr</h1>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += "<a "+InTempOnOff2+"</a>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - Display EIN / AUS / Helligkeitsstufen:</h2>";
  HTMLString += "<a href =\"/moff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display AUS</button></a>";
  HTMLString += "<a href =\"/mon\"><button class=\"button\" style=\"margin-right: 20px;height: 26px; width: 90px;\">Display EIN</button></a>";
  HTMLString += "<a href =\"/m1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB1+"</button></a>";
  HTMLString += "<a href =\"/m2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB2+"</button></a>"; 
  HTMLString += "<a href =\"/m3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB3+"</button></a>";
  HTMLString += "<a href =\"/m4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB4+"</button></a>";
  HTMLString += "<a href =\"/m5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB5+"</button></a>";
  HTMLString += "<p></p>";
  HTMLString +="<h4>"+DisplayAutomaticOnOff+"</h4>";
  HTMLString += "<a href =\"/cdof\"><button class=\"button\" style=\"margin-right: 10px;height: 22px; font-size: 12px; font-weight: bold; width: 70px;\">Ändern</button></a>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - Status Doppelpunkt:</h2>";
  HTMLString += "<a href =\"/sdflash\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP1+"</button></a>";
  HTMLString += "<a href =\"/sdon\"><button class=\"button\"style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP2+"</button></a>";
  HTMLString += "<a href =\"/sdoff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP3+"</button></a>";
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - Geschwindigkeit Lauftext:</h2>";
  HTMLString += "<a href =\"/vlt1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL1+"</button></a>";
  HTMLString += "<a href =\"/vlt2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL2+"</button></a>";
  HTMLString += "<a href =\"/vlt3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL3+"</button></a>";
  HTMLString += "<a href =\"/vlt4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL4+"</button></a>";
  HTMLString += "<a href =\"/vlt5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL5+"</button></a>";
  HTMLString += "<a href =\"/vlt6\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL6+"</button></a>";
  HTMLString += "<a href =\"/vlt7\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL7+"</button></a>";
  HTMLString += "<a href =\"/vlt8\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL8+"</button></a>";
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - Zusatzfunktionen EIN / AUS:</h2>";
  HTMLString += "<a href =\"/zfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+EF1+"</button></a>";
  HTMLString += "<a href =\"/zfaus\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+EF2+"</button></a>";
  HTMLString += "<p></p>";
  return HTMLString;
}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){
  String HTMLString, rtime, OWMA, ANA;
  rtime = runtime(); // Laufzeit der Uhr (Clock Runtime)

// Wetterdaten aktivieren / deaktivieren
  String OWMF1, OWMF2, NAF1, NAF2;
  if (WeatherFunctions == true) {OWMF1 = "EIN*";} else {OWMF1 = "EIN";} 
  if (WeatherFunctions == false) {OWMF2 = "AUS*";} else {OWMF2 = "AUS";} 
  if (NewsAPIFunction == true) {NAF1 = "EIN*";} else {NAF1 = "EIN";} 
  if (NewsAPIFunction == false) {NAF2 = "AUS*";} else {NAF2 = "AUS";} 

// OpenWeatherMap Status anzeigen oder nicht
  if (AccessOpenWeatherMap == true) {
  if (!WeatherFunctions) {StatusOpenWeatherMap = "deaktiviert";} else {
  if (OWMAdjustChange == true) {StatusOpenWeatherMap = "aktiviert";
      OWMAdjustChange = false;}
  }
  OWMA = "href =\"/owmfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+OWMF1+"</button></a>";
  OWMA += "<a href =\"/owmfaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\">"+OWMF2+"</button></a>";
  if (StatusOpenWeatherMap == "OK") {
  OWMA += "<a href =\"/cwd\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#0c6103; margin-left: 110px; height: 28px; width: 160px;\">aktuelle Wetterdaten</button>";} else {
  if (StatusOpenWeatherMap == "Fehler") {
  OWMA += "<a href =\"/cwd\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#f7073f; margin-left: 110px; height: 28px; width: 160px;\">aktuelle Wetterdaten</button>";} else {  
  OWMA += "<a href =\"/cwd\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#969091; margin-left: 110px; height: 28px; width: 160px;\">aktuelle Wetterdaten</button>";}}  
  } else {
  OWMA = "href =\"/owmfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>"+OWMF1+"</button></a>";
  OWMA += "<a href =\"/owmfaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\"disabled>"+OWMF2+"</button></a>";
  OWMA += "<a style=\"font-family:arial;color:#ff525a;font-size: 14px; margin-left: 40px;\">OpenWeatherMap ist nicht aktiv !";}

  // NewsAPI Status anzeigen oder nicht
  if (AccessNewsAPI == true) {
  if (!NewsAPIFunction) {StatusNewsAPI = "deaktiviert";} else {
    if (NAAdjustChange == true) {StatusNewsAPI = "aktiviert";
        NAAdjustChange = false;} 
    }   
  ANA = "href =\"/nafein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+NAF1+"</button></a>";
  ANA += "<a href =\"/nafaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\">"+NAF2+"</button></a>";
  if (StatusNewsAPI == "OK") {
  ANA += "<a href =\"/cn\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#0c6103; margin-left: 110px; height: 26px; width: 160px;\">aktuelle Nachricht</button>";} else {
  if (StatusNewsAPI == "Fehler") {
  ANA += "<a href =\"/cn\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#f7073f; margin-left: 110px; height: 26px; width: 160px;\">aktuelle Nachricht</button>";} else {
  ANA += "<a href =\"/cn\"><button class=\"button\" style=\"font-family:arial; font-size: 14px; margin-right: 10px; color:#969091; margin-left: 110px; height: 26px; width: 160px;\">aktuelle Nachricht</button>";}} 
  } else {
  ANA = "href =\"/nafein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>"+NAF1+"</button></a>";
  ANA += "<a href =\"/nafaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\"disabled>"+NAF2+"</button></a>";
  ANA += "<a style=\"font-family:arial;color:#ff525a;font-size: 14px; margin-left: 40px;\">NewsAPI ist nicht aktiv !";}

  // 2. Teil des HTML-Strings
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - Wetterdaten anzeigen &nbsp; EIN / AUS / Wetterdaten:</h2>";
  HTMLString += "<a "+OWMA+"</a>";
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Auswahl - aktuelle Nachrichten anzeigen &nbsp; EIN / AUS / Nachricht:</h2>";
  HTMLString += "<a "+ANA+"</a>";
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Anzeige - Einstellungen aus dem Access Point (Captive Portal):</h2>";
  HTMLString += "<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">Einstellungen</button></a>";
  HTMLString += "<p></p>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h2>Speichern der Einstellungen:</h2>";
  HTMLString += "<a href =\"/esave\"><button class=\"button\" style=\"margin-right: 0px;height: 26px; width: 110px;\">Speichern</button></a>";
  HTMLString += "<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">";
  HTMLString += "<h7>Software-Version: "+LMCVersion+"</h7>";
  HTMLString += "<h7><br /></h7>";
  HTMLString += "<h7>Laufzeit der Uhr: &nbsp;&nbsp;"+rtime+"</h7>";
  HTMLString += "<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px;height: 48px; width: 180px;color: red;float: right; font-size: 16px;\">Zurücksetzen! (Reset)</button></a>";
  HTMLString += "<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px;height: 48px; width: 120px;color: blue;float: right; font-size: 18px;\">Neustart</button></a>";
  HTMLString += "<a><br /></a>";
  HTMLString += "<a><br /></a>";
  HTMLString += "<h6>*) - aktuelle Auswahl</h6>";
  HTMLString += "</body>";
  HTMLString += "</html>";
  return HTMLString;
}

/********************************************************************************************************************************************************/

void HTMLAnswer(String WebFunction) {
String HTMLString, HTMLAnswerString;  
if (WebFunction == "MatrixOn") {HTMLAnswerString = "Die LED Matrix wurde eingeschaltet";} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = "Die LED Matrix wurde ausgeschaltet";} 
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
if (WebFunction == "EnhancedFunctionsEin1") {HTMLAnswerString = "Die Zusatzfunktionen wurden eingeschaltet<br />(Datum, Feiertage, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = "Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Feiertage, Geburtstage)";}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = "Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Feiertage, Geburtstage)";}
if (WebFunction == "SaveWebSettings") {HTMLAnswerString = "Die Einstellungen wurden gespeichert";}
if (WebFunction == "ResetClockYes") {HTMLAnswerString = "Wenn Sie auf die Startseite zurückkehren wird<br />der Access Point (Captive Portal) gestartet<br />und alle Einstellungen auf Standard gesetzt";}
HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
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
server.setContentLength(HTMLString.length());   // (or hardcode the length)    
server.send(200, "text/html", HTMLString);  
}

/********************************************************************************************************************************************************/

// Matrix einschalten
void MatrixOn()
{
Display = 1;
initMAX7219();
sendCmdAll(CMD_INTENSITY, 0);
sendCmdAll(CMD_SHUTDOWN, 1);
HTMLAnswer("MatrixOn");
Serial.println("Die LED-Matrix wurde eingeschaltet.");
}

/********************************************************************************************************************************************************/

// Matrix ausschalten
void MatrixOff()
{
Display = 0;  
sendCmdAll(CMD_SHUTDOWN,0);
HTMLAnswer("MatrixOff");
Serial.println("Die LED-Matrix wurde ausgeschaltet.");
}

/********************************************************************************************************************************************************/

void DoublePointFlash()
{
DoublePointStatus = 2; // Doppelpunkt der Uhr blinken lassen 
HTMLAnswer("DoublePointFlash");
Serial.println("Doppelpunkt der Uhr blinkt.");
}

/********************************************************************************************************************************************************/

void DoublePointOn()
{
DoublePointStatus = 1; // Doppelpunkt der Uhr permanant AN
HTMLAnswer("DoublePointOn");
Serial.println("Doppelpunkt der Uhr permanant an.");
}


/********************************************************************************************************************************************************/

void DoublePointOff()
{
DoublePointStatus = 0; // Doppelpunkt der Uhr permanant aus
HTMLAnswer("DoublePointOff");
Serial.println("Doppelpunkt der Uhr permanant aus.");
}

/********************************************************************************************************************************************************/

// Helligkeitsstufen der LED's des Max7219 per Webserver einstellen
void BrightnessLevel1()
{
// Serial.println("Matrix: Helligkeitsstufe 1 aktiviert)");
sendCmdAll(CMD_INTENSITY, 0);
Brightness = "L1";
HTMLAnswer("BrightnessLevel1");
Serial.println("Helligkeitsstufe 1 aktiviert");
}

/********************************************************************************************************************************************************/

void BrightnessLevel2()
{
// Serial.println("Matrix: Helligkeitsstufe 2 aktiviert");
sendCmdAll(CMD_INTENSITY, 1);
Brightness = "L2";
HTMLAnswer("BrightnessLevel2");
Serial.println("Helligkeitsstufe 2 aktiviert");
}

/********************************************************************************************************************************************************/

void BrightnessLevel3()
{
// Serial.println("Matrix: Helligkeitsstufe 3 aktiviert");
sendCmdAll(CMD_INTENSITY, 2);
Brightness = "L3";
HTMLAnswer("BrightnessLevel3");
Serial.println("Helligkeitsstufe 3 aktiviert");
}

/********************************************************************************************************************************************************/

void BrightnessLevel4()
{
// Serial.println("Matrix: Helligkeitsstufe 4 aktiviert");
sendCmdAll(CMD_INTENSITY, 3);
Brightness = "L4";
HTMLAnswer("BrightnessLevel4");
Serial.println("Helligkeitsstufe 4 aktiviert");
}

/********************************************************************************************************************************************************/

void BrightnessLevel5()
{
// Serial.println("Matrix: Helligkeitsstufe 5 aktiviert");
sendCmdAll(CMD_INTENSITY, 4);
Brightness = "L5";
HTMLAnswer("BrightnessLevel5");
Serial.println("Helligkeitsstufe 5 aktiviert");
}

/********************************************************************************************************************************************************/

// Geschwindigkeit der Laufschrift einstellen
void ScrollTextTimeLevel1()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe1");
ScrollTextTime   = 60;
HTMLAnswer("ScrollTextTimeLevel1");
Serial.println("Laufschriftgeschwindigkeit Stufe1 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe2");
ScrollTextTime   = 55;
HTMLAnswer("ScrollTextTimeLevel2");
Serial.println("Laufschriftgeschwindigkeit Stufe 2 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel3()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe3");
ScrollTextTime   = 50;
HTMLAnswer("ScrollTextTimeLevel3");
Serial.println("Laufschriftgeschwindigkeit Stufe 3 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel4()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe4");
ScrollTextTime   = 45;
HTMLAnswer("ScrollTextTimeLevel4");
Serial.println("Laufschriftgeschwindigkeit Stufe 4 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel5()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe5");
ScrollTextTime   = 40;
HTMLAnswer("ScrollTextTimeLevel5");
Serial.println("Laufschriftgeschwindigkeit Stufe 5 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel6()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe6");
ScrollTextTime   = 35;
HTMLAnswer("ScrollTextTimeLevel6");
Serial.println("Laufschriftgeschwindigkeit Stufe 6 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel7()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe7");
ScrollTextTime   = 30;
HTMLAnswer("ScrollTextTimeLevel7");
Serial.println("Laufschriftgeschwindigkeit Stufe 7 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel8()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe8");
ScrollTextTime   = 25;
HTMLAnswer("ScrollTextTimeLevel8");
Serial.println("Laufschriftgeschwindigkeit Stufe 8 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
ClockScrollIn(); // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void WeatherFunctionsEin()
{
// Serial.println("Zusatzfunktionen eingeschaltet");
if (WeatherFunctions == false) {
WeatherFunctions = true;
OWMAdjustChange = true;
HTMLAnswer("WeatherFunctionsEin1");
Serial.println("OpenWeatherMap - Wetterdaten eingeschaltet");}
else {
HTMLAnswer("WeatherFunctionsEin2");
Serial.println("OpenWeatherMap - Wetterdaten sind bereits eingeschaltet");}    
}

/********************************************************************************************************************************************************/

void WeatherFunctionsAus()
{
// Serial.println("Zusatzfunktionen ausgeschaltet");
WeatherFunctions = false;
StatusOpenWeatherMap = "deaktiviert";
HTMLAnswer("WeatherFunctionsAus");
Serial.println("OpenWeatherMap - Wetterdaten ausgeschaltet");
}

/********************************************************************************************************************************************************/

void NewsAPIFunctionEin()
{
// Serial.println("Zusatzfunktionen eingeschaltet");
if (NewsAPIFunction == false) {
NewsAPIFunction = true;
NAAdjustChange = true;
HTMLAnswer("NewsAPIFunctionEin1");
Serial.println("NewsAPI - aktuelle Nachrichten eingeschaltet");}
else {
HTMLAnswer("NewsAPIFunctionEin2");
Serial.println("NewsAPI - aktuelle Nachrichten sind bereits eingeschaltet");}    
}

/********************************************************************************************************************************************************/

void NewsAPIFunctionAus()
{
// Serial.println("Zusatzfunktionen ausgeschaltet");
NewsAPIFunction = false;
StatusNewsAPI = "deaktiviert";
HTMLAnswer("NewsAPIFunctionAus");
Serial.println("NewsAPI - aktuelle Nachrichten ausgeschaltet");
}

/********************************************************************************************************************************************************/

void EnhancedFunctionsEin()
{
// Serial.println("Zusatzfunktionen eingeschaltet");
if (!EnhancedFunctions) {
EnhancedFunctions = true;
HTMLAnswer("EnhancedFunctionsEin1");} else {
HTMLAnswer("EnhancedFunctionsEin2");}  
Serial.println("Zusatzfunktionen eingeschaltet - (Datum, Feiertage, Geburtstage)"); 
}

/********************************************************************************************************************************************************/

void EnhancedFunctionsAus()
{
// Serial.println("Zusatzfunktionen ausgeschaltet");
EnhancedFunctions = false;
HTMLAnswer("EnhancedFunctionsAus");
Serial.println("Zusatzfunktionen ausgeschaltet (Datum, Feiertage, Geburtstage)");
}

/********************************************************************************************************************************************************/

void GetAdjustments()
{
String OWMAPI, OWMCID, NewsAPIKey, DHTTempOffset, DHTHumOffset, InTempValue, DAOn, DAOff, CheckIP; 
String HTMLString;
InTempValue = InTempMessure(); // Messung
// Check IP-Adresse
if (cip.length() > 15 || cip.length() < 7) {CheckIP = "k.A.";} else {CheckIP = cip;} // Check IP-Adresse
if (AccessOpenWeatherMap == true){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = "k.A.";OWMCID = "k.A.";}
if (newsKey != ""){NewsAPIKey = newsKey;} else {NewsAPIKey = "k.A.";}
if (DisplayOnTime != "" && DisplayOffTime != "" && DisplayOnTime != "---" && DisplayOffTime != "---"){
  DAOn  = "Display automatisch EIN um "+FDOnTime+" Uhr";
  DAOff = "Display automatisch AUS um "+FDOffTime+" Uhr";} else
  {
  if (DisplayOnTime != "" && DisplayOnTime != "---"){
  DAOn = "Display automatisch EIN um "+FDOnTime+" Uhr";} else {
  DAOn = "Display automatisch EIN um: k.A.";}  
  if (DisplayOffTime != "" && DisplayOffTime != "---"){
  DAOff = "Display automatisch AUS um "+FDOffTime+" Uhr";} else {
  DAOff = "Display automatisch AUS um: k.A.";}
  }
if (InTempValue != "-") {
if (InTempOffsetInt > 0) {DHTTempOffset = "DHT-Sensor Temperatur Offset:&nbsp;&nbsp;+"+InTempOffset+"°C";} else { 
DHTTempOffset = "DHT-Sensor Temperatur Offset:&nbsp;&nbsp;"+InTempOffset+"°C";}
if (InHumOffsetInt > 0) {DHTHumOffset = "DHT-Sensor Luftfeuchte Offset:&nbsp;&nbsp;+"+InHumOffset+"%";} else {
DHTHumOffset = "DHT-Sensor Luftfeuchte Offset:&nbsp;&nbsp;"+InHumOffset+"%";}  
} else {
DHTTempOffset = "";
DHTHumOffset = "";    
}
HTMLString = "<!DOCTYPE html> <html>";
HTMLString += "<head>";
HTMLString += "<title>LED-Matrix-Uhr</title>";
HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
HTMLString += "<style>";
HTMLString += "body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
HTMLString += "h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
HTMLString += "h2 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }";
HTMLString += "h3 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }";
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body>";
HTMLString += "<h1>LED-Matrix-Uhr</h1>";
HTMLString += "<a><br /></a>";
HTMLString += "<h3>Einstellungen aus dem Captive Portal</h3>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;"+ssid+"</h2>";
HTMLString += "<h2>IP-Adresse:&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>OpenWeatherMap API-Key:&nbsp;&nbsp;"+OWMAPI+"</h2>";
HTMLString += "<h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>";
HTMLString += "<a><br /></a>";
HTMLString += "<h2>NewsAPI API-Key:&nbsp;&nbsp;"+NewsAPIKey+"</h2>";
HTMLString += "<a><br /></a>";
if (DisplayTimeChange == "False") {
HTMLString += "<h2>"+DAOn+"</h2>";
HTMLString += "<h2>"+DAOff+"</h2>";
HTMLString += "<a><br /></a>";}
HTMLString += "<h2>"+DHTTempOffset+"</h2>";
HTMLString += "<h2>"+DHTHumOffset+"</h2>";
HTMLString += "<a><br /></a>";
HTMLString += "<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 160px\">zurück zur Startseite</button></a>";
HTMLString += "</body>";
HTMLString += "</html>";
server.setContentLength(HTMLString.length());   // (or hardcode the length)
server.send(200, "text/html", HTMLString);  
Serial.println("Einstellungen aus dem Access Point (Captive Portal) aufgerufen");  
}

/********************************************************************************************************************************************************/

void SaveWebSettings()
{
if (littleFSInit == true) {
// Matrix Helligkeit speichern  
SetupSave("matrix_brightness.txt", Brightness);
MyWaitLoop(100);  
// Status Doppelpunkt
SetupSave("status_doublepoint.txt", String(DoublePointStatus));
MyWaitLoop(100);
// ScrollText Geschwindigkeit
SetupSave("status_scrolltexttime.txt", String(ScrollTextTime));
// Status Zusatzfunktionen
if (EnhancedFunctions == true && AccessOpenWeatherMap == true){
SetupSave("status_enhancedfunctions.txt", "On");  
} else {SetupSave("status_enhancedfunctions.txt", "Off");}
// Status OpenWeatherMap
if (WeatherFunctions == true){
SetupSave("status_weatherfunctions.txt", "On");  
} else {SetupSave("status_weatherfunctions.txt", "Off");}
// Status NewsAPI
if (NewsAPIFunction == true){
SetupSave("status_newsapifunctions.txt", "On");  
} else {SetupSave("status_newsapifunctions.txt", "Off");}
HTMLAnswer("SaveWebSettings");
MyWaitLoop(100);} else
{  
Serial.println("Die Daten konnten nicht gespeichert werden !");
printStringWithShift("     +++    Die Daten konnten nicht gespeichert werden !!!   +++           ", ScrollTextTime);
ClockScrollIn();} // Uhrzeit in Display hineinscrollen 
}

/********************************************************************************************************************************************************/

void ResetClockSettings()
{
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
   h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
     <h2>Alle Einstellungen auf Standard zurücksetzen !</h2>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(WLAN-Zugangsdaten werden gelöscht)</h2>\
     <a><br /></a>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Jetzt alle Einstellungen zurücksetzen ?</h2>\
     <a><br /></a>\
     <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 70px;height: 48px; width: 110px; font-size: 22px; color: red\">JA</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">NEIN</button></a>\
  </body>\
    </html>";
server.setContentLength(HTMLString.length());   // (or hardcode the length)    
server.send(200, "text/html", HTMLString);    
}

/********************************************************************************************************************************************************/

void CurrentWeatherData()
{
String HTMLString;
if (StatusOpenWeatherMap == "Fehler") {weatherString1 = "Fehler beim Herunterladen der Wetterdaten !";}
if (StatusOpenWeatherMap == "aktiviert" || StatusOpenWeatherMap == "k.A.") {weatherString1 = "Es liegen noch keine aktuellen Wetterdaten vor.";}
if (StatusOpenWeatherMap == "deaktiviert") {weatherString1 = "Das Herunterladen von Wetterdaten wurde deaktiviert";}
HTMLString = "<!DOCTYPE html> <html>";
HTMLString += "<head>";
HTMLString += "<title>LED-Matrix-Uhr</title>";
HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
HTMLString += "<style>";
HTMLString += "body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
HTMLString += "h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
HTMLString += "h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "h3 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "h4 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body>";
HTMLString += "<h1>LED-Matrix-Uhr</h1>";
HTMLString += "<h2>Die aktuellen Wetterdaten von OpenWeatherMap</h2>";
HTMLString += "<a><br /></a>";
if (StatusOpenWeatherMap == "aktiviert" || StatusOpenWeatherMap == "deaktiviert")  {
HTMLString += "<h4>Status: "+StatusOpenWeatherMap+"</h4>";} else {
HTMLString += "<h4>Status: "+StatusOpenWeatherMap+" - "+OWMLastConnectionDateTime+"</h4>";}
HTMLString += "<a><br /></a>";
HTMLString += "<h3>"+weatherString1+"</h3>";
HTMLString += "<a><br /></a>";
HTMLString += "<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 160px\">zurück zur Startseite</button></a>";
HTMLString += "</body>";
HTMLString += "</html>";
server.setContentLength(HTMLString.length());   // (or hardcode the length)    
server.send(200, "text/html", HTMLString);    
}

/********************************************************************************************************************************************************/

void CurrentNews()
{
String HTMLString;
if (StatusNewsAPI == "Fehler") {APINews = "Fehler beim Herunterladen der Nachrichten !";}
if (StatusNewsAPI == "aktiviert" || StatusNewsAPI == "k.A.") {APINews = "Es liegen noch keine aktuellen Nachrichten vor.";}
if (StatusNewsAPI == "deaktiviert") {APINews = "Das Herunterladen von Nachrichten wurde deaktiviert";}
HTMLString =  "<!DOCTYPE html> <html>";
HTMLString += "<head>";
HTMLString += "<title>LED-Matrix-Uhr</title>";
HTMLString += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
HTMLString += "<style>";
HTMLString += "body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }";
HTMLString += "h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
HTMLString += "h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "h3 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "h4 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}";
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body>";
HTMLString += "<h1>LED-Matrix-Uhr</h1>";
HTMLString += "<h2>Die aktuelle Nachricht von APINews</h2>";
HTMLString += "<a><br /></a>";
if (StatusNewsAPI == "aktiviert" || StatusNewsAPI == "deaktiviert") {
HTMLString += "<h4>Status: "+StatusNewsAPI+"</h4>";} else {
HTMLString += "<h4>Status: "+StatusNewsAPI+" - "+NewsAPILastConnectionDateTime+"</h4>";}
HTMLString += "<a><br /></a>";
HTMLString += "<h3>"+APINews+"</h3>";
HTMLString += "<a><br /></a>";
HTMLString += "<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 160px\">zurück zur Startseite</button></a>";
HTMLString += "</body>";
HTMLString += "</html>";
server.setContentLength(HTMLString.length());   // (or hardcode the length)    
server.send(200, "text/html", HTMLString);    
}

/********************************************************************************************************************************************************/

void ResetClockYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
HTMLAnswer("ResetClockYes");
ResetNow = true; // Reset - Werkseinstellungen    
}

/********************************************************************************************************************************************************/

void RebootClock()
{
HTMLAnswer("RebootClock");
RebootNow = true;
}

/********************************************************************************************************************************************************/

void ChangeDisplayOnOff()
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
    if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "---";}
    if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "---";}
    if (DisplayOnTime != "" && DisplayOnTime != "---") {
    if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = "---";};}
    if (DisplayOffTime != "" && DisplayOffTime != "---") {
    if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = "---";};}

    /*Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println(""); // for Debug Mode*/

    if (DisplayOnTime != "" && DisplayOnTime != "---") {
    MDTime = false;
    int DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = CheckMDHourOn.toInt();} else {DisplayOnTime = "---";}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = CheckMDMinOn.toInt();} else {DisplayOnTime = "---";}
    if (DisplayOnTime != "" && DisplayOnTime != "---") {
    for (int i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = "---";} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = "---";}}}}

    if (DisplayOffTime != "" && DisplayOffTime != "---") {
    MDTime = false;
    int DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = CheckMDHourOff.toInt();} else {DisplayOffTime = "---";}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = CheckMDMinOff.toInt();} else {DisplayOffTime = "---";}
    if (DisplayOffTime != "" && DisplayOffTime != "---") {
    for (int i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = "---";} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = "---";}}}}

    FDOnTime =  DisplayOnTime; 
    FDOffTime = DisplayOffTime; 

    /*Serial.println("*** Display On Time ***: "+DisplayOnTime+" Uhr"); // for Debug Mode
    Serial.println("*** Display Off Time ***: "+DisplayOffTime+" Uhr"); // for Debug Mode
    Serial.println(""); // for Debug Mode
    Serial.println(""); // for Debug Mode*/

    if (littleFSInit == true){
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
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Einstellungen wurden gespeichert<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    server.send(200, "text/html", HTMLString); // Captive Portal 
    // *** Eingaben im FileSystem speichern ***
    // Matrix Display Einschaltzeit
    if (DisplayOnTime != "" && DisplayOnTime != "---"){
    SetupSave("display-on-time.txt", DisplayOnTime);} else {
    if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
    // Matrix Display Ausschaltzeit
    if (DisplayOffTime != "" && DisplayOffTime != "---"){
    SetupSave("display-off-time.txt", DisplayOffTime);} else {
    if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
    SetupSave("display-change-time.txt", "True"); // Display On/Off Daten wurden geändert
    MyWaitLoop(500); // 0,5 sek. warten 
    Serial.println("Display automatisch EIN/AUS  Daten gespeichert... "); 
    Serial.println("Uhr wird neu gestartet"); 
    printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
    // Reset auslösen
    ESP.restart();  
    }
     else
     { // Zugangsdaten wurden nicht gespeichert
     Serial.println("Zugangsdaten wurden nicht gespeichert !");
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
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}"; 
    HTMLString += "h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }";   
    HTMLString += "h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"; 
    HTMLString += "h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>LED-Matrix-Uhr</h1>";
    HTMLString += "<h5>(LED-Matrix-Display automatisch EIN / AUS)</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    server.send(200, "text/html", HTMLString); // Captive Portal 
    // Reset auslösen 
    printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();  
    } 
    server.send(200, "text/html", HTMLString);  // Speichern   
  } else {  
// Captive Portal Einstellungen
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
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
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
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display EIN-Zeit (Beispiel: 7:30)   -Optional*-</label><input type='text' class='form-control' name='DOnT' id='DOnT' value='"+FDOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display AUS-Zeit (Beispiel: 23:30)   -Optional*-</label><input type='text' class='form-control' name='DOffT' id='DOffT' value='"+FDOffTime+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<h6>*) - Leer oder drei Striche (---) deaktiviert die Funktion</h6>";
    HTMLString += "<button type='submit'>Speichern</button>";
    HTMLString += "<br/><br/>";
    HTMLString += "</form>";
    HTMLString += "<a href =\"/\"><button>zurück zur Startseite</button></a>";
    HTMLString += "<br/><br/>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    server.send(200, "text/html", HTMLString); // Captive Portal
  }  
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HTTPCodeText(int httpcode)
{
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
return String(httpcode) + " - " + TempHTTPCodeText;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
