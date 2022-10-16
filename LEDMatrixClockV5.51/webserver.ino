// Webserver Startseite
void handleRoot() 
{ /* &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden*/ 
  String RefreshTime = "60"; // Aktualisierung der Website nach X Sekunden
  // Auswahl Helligkeit der Matrix
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
  // Wetterdaten aktivieren / deaktivieren
  String OWMF1, OWMF2, NAF1, NAF2;
  if (WeatherFunctions == true) {OWMF1 = "EIN*";} else {OWMF1 = "EIN";} 
  if (WeatherFunctions == false) {OWMF2 = "AUS*";} else {OWMF2 = "AUS";} 
  if (NewsAPIFunction == true) {NAF1 = "EIN*";} else {NAF1 = "EIN";} 
  if (NewsAPIFunction == false) {NAF2 = "AUS*";} else {NAF2 = "AUS";} 
  
  yield();
  MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
  String HTMLString,DisplayAutomaticOnOff, InTempOnOff1,InTempOnOff2, OWMSS, OWMA, ANS, ANA;
  if (FDOnTime != "" && FDOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr<br />Display automatisch AUS um "+FDOffTime+" Uhr";} else
  {
  if (FDOnTime != ""){
  DisplayAutomaticOnOff = "Display automatisch EIN um "+FDOnTime+" Uhr";} else {
  if (FDOffTime != ""){
  DisplayAutomaticOnOff = "Display automatisch AUS um "+FDOffTime+" Uhr";}
  }
  }

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

  // OpenWeatherMap Status anzeigen oder nicht
  if (AccessOpenWeatherMap == true) {
  if (!WeatherFunctions) {StatusOpenWeatherMap = "deaktiviert";}
  OWMSS = "Status - OpenWeatherMap:&nbsp&nbsp"+StatusOpenWeatherMap+" - "+OWMLastConnectionDateTime;
  OWMA = "href =\"/owmfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+OWMF1+"</button></a>\<a href =\"/owmfaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\">"+OWMF2+"</button>";
  } else {
  OWMSS = "";
  OWMA = "href =\"/owmfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>"+OWMF1+"</button></a>\<a href =\"/owmfaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\" disabled>"+OWMF2+"</button></a>\<a style=\"font-family:arial;color:#ff525a;font-size: 14px; margin-left: 20px;\">OpenWeatherMap ist nicht aktiv !";
  }

  // NewsAPI Status anzeigen oder nicht
  if (AccessNewsAPI == true) {
  if (!NewsAPIFunction) {StatusNewsAPI = "deaktiviert";}  
  ANS = "Status - NewsAPI:&nbsp&nbsp"+StatusNewsAPI+" - "+NewsAPILastConnectionDateTime; 
  ANA = "href =\"/nafein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+NAF1+"</button></a>\<a href =\"/nafaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\">"+NAF2+"</button>"; 
  } else {
  ANS = "";
  ANA = "href =\"/nafein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\" disabled>"+NAF1+"</button></a>\<a href =\"/nafaus\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 10px; height: 26px; width: 90px;\" disabled>"+NAF2+"</button></a>\<a style=\"font-family:arial;color:#ff525a;font-size: 14px; margin-left: 20px;\">NewsAPI ist nicht aktiv !";
  }

  // Check HTML Teilstring
 //  Serial.println("<a"+InTempOnOff2+"</a>\"); // Only for Debuging 

  // HTML String zusammenstellen 
  HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
   body { background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
   h2 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; }\
   h3 { font-size: 16px; margin-top: 0px; margin-bottom: 10px; }\
   h4 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
   h5 { font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}\
   h6 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
   h7 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}\
   #Rvalue {color: #FC3A30;}\
   #Gvalue {color: lime;}\
   #Bvalue {color: #2ECCFA;}\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h3>"+OWMSS+"</h3>\
    <h3>"+ANS+"</h3>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h5 "+InTempOnOff1+"</h5>\
    <a "+InTempOnOff2+"</a>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl der Helligkeitsstufen:</h2>\
    <a href =\"/moff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display AUS</button></a>\
    <a href =\"/mon\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display EIN</button></a>\
    <a href =\"/m1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB1+"</button></a>\
    <a href =\"/m2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB2+"</button></a>\
    <a href =\"/m3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB3+"</button></a>\
    <a href =\"/m4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB4+"</button></a>\
    <a href =\"/m5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB5+"</button></a>\
    <p></p>\
    <h4>"+DisplayAutomaticOnOff+"</h4>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Status Doppelpunkt:</h2>\
    <a href =\"/sdflash\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP1+"</button></a>\
    <a href =\"/sdon\"><button class=\"button\"style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP2+"</button></a>\
    <a href =\"/sdoff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">"+SDP3+"</button></a>\ 
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Geschwindigkeit Lauftext:</h2>\
    <a href =\"/vlt1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL1+"</button></a>\
    <a href =\"/vlt2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL2+"</button></a>\
    <a href =\"/vlt3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL3+"</button></a>\
    <a href =\"/vlt4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL4+"</button></a>\
    <a href =\"/vlt5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL5+"</button></a>\
    <a href =\"/vlt6\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL6+"</button></a>\
    <a href =\"/vlt7\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL7+"</button></a>\
    <a href =\"/vlt8\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+STL8+"</button></a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Zusatzfunktionen EIN / AUS:</h2>\
    <a href =\"/zfein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+EF1+"</button></a>\
    <a href =\"/zfaus\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+EF2+"</button></a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Wetterdaten anzeigen &nbsp; EIN / AUS:</h2>\
    <a "+OWMA+"</a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - aktuelle Nachrichten anzeigen &nbsp; EIN / AUS:</h2>\
    <a "+ANA+"</a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Anzeige - Einstellungen aus dem Access Point (Captive Portal):</h2>\
    <a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 130px;\">Einstellungen</button></a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Speichern der Einstellungen:</h2>\
    <a href =\"/esave\"><button class=\"button\" style=\"margin-right: 0px;height: 26px; width: 110px;\">Speichern</button></a>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h7>Software-Version: "+LMCVersion+"</h7>\
    <h7><br /></h7>\
    <h7>Laufzeit der Uhr: &nbsp;&nbsp;"+runtime()+"</h7>\
    <a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px;height: 48px; width: 180px;color: red;float: right; font-size: 16px;\">Zurücksetzen! (Reset)</button></a>\
    <a><br /></a>\
    <a><br /></a>\
    <h6>*) - aktuelle Auswahl</h6>\
    </body>\
    </html>";  
  // Daten senden
  server.send(200, "text/html",HTMLString);
  yield();
  MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
  if (littleFSInit == true && ResetNow == true) {
  ResetClock();} // Alle Einstellungen auf Standard - WLAN Zugangsdaten werden gelöscht    
}

/********************************************************************************************************************************************************/

void HTMLAnswer(String WebFunction) {
String HTMLString, HTMLAnswerString;  
if (WebFunction == "MatrixOn") {HTMLAnswerString = "Die LED Matrix wurde eingeschaltet";} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = "Die LED Matrix wurde ausgeschaltet";} 
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
if (WebFunction == "EnhancedFunctionsEin2") {HTMLAnswerString = "Die Zusatzfunktionen sind bereits eingeschaltet<br />(Datum, Feiertage, Geburtstage";}
if (WebFunction == "EnhancedFunctionsAus") {HTMLAnswerString = "Die Zusatzfunktionen wurden ausgeschaltet<br />(Datum, Feiertage, Geburtstage, News)";}
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
String OWMAPI, OWMCID, NewsAPIKey, DHTTempOffset, DHTHumOffset, InTempValue; 
String HTMLString;
InTempValue = InTempMessure(); // Messung
if (AccessOpenWeatherMap == true){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = "k.A.";OWMCID = "k.A.";}
if (newsKey != ""){NewsAPIKey = newsKey;} else {NewsAPIKey = "k.A.";}
if (InTempValue != "-") {
if (InTempOffsetInt > 0) {DHTTempOffset = "DHT-Sensor Temperatur Offset:&nbsp;&nbsp;+"+InTempOffset+"°C";} else { 
DHTTempOffset = "DHT-Sensor Temperatur Offset:&nbsp;&nbsp;"+InTempOffset+"°C";}
if (InHumOffsetInt > 0) {DHTHumOffset = "DHT-Sensor Luftfeuchte Offset:&nbsp;&nbsp;+"+InHumOffset+"%";} else {
DHTHumOffset = "DHT-Sensor Luftfeuchte Offset:&nbsp;&nbsp;"+InHumOffset+"%";}  
} else {
DHTTempOffset = "";
DHTHumOffset = "";    
}
HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
   h2 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
   h3 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }\
  </style>\
  </head>\
    <body>\
    <h1>LED-Matrix-Uhr</h1>\
    <a><br /></a>\
    <h3>Einstellungen aus dem Captive Portal</h3>\
    <a><br /></a>\
    <h2>OpenWeatherMap API-Key:&nbsp;&nbsp;"+OWMAPI+"</h2>\
    <h2>OpenWeatherMap City-ID:&nbsp;&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>\
    <a><br /></a>\
    <h2>NewsAPI API-Key:&nbsp;&nbsp;"+NewsAPIKey+"</h2>\
    <a><br /></a>\
    <h2>"+DHTTempOffset+"</h2>\
    <h2>"+DHTHumOffset+"</h2>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
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
server.send(200, "text/html", HTMLString);    
}

/********************************************************************************************************************************************************/

void ResetClockYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
HTMLAnswer("ResetClockYes");
ResetNow = true; // Reset - Werkseinstellungen    
}

/********************************************************************************************************************************************************/

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
