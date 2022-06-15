// Webserver Startseite
void handleRoot() 
{ // &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
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
  
  yield();
  MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
  String HTMLString,DisplayAutomaticOnOff;
  if (DisplayOnTime != "" && DisplayOffTime != ""){
  DisplayAutomaticOnOff = "<br /></a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\">Display automatisch EIN um "+
  DisplayOnTime+" Uhr</a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\">Display automatisch AUS um "+DisplayOffTime+" Uhr";} else
  {
  if (DisplayOnTime != ""){
  DisplayAutomaticOnOff = "<br /></a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\">Display automatisch EIN um "+
  DisplayOnTime+" Uhr</a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\">";} else {
  if (DisplayOffTime != ""){
  DisplayAutomaticOnOff = "<br /></a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\"></a>\<a><br /></a>\<a style=\"font-family:arial;color:green;font-size: 12px\">Display automatisch AUS um "+DisplayOffTime+" Uhr";}
  }
  }
  
  if (AccessOpenWeatherMap == true)
  { // mit OpenWeatherMap
  if (EnhancedFunctions == false) {StatusOpenWeatherMap = "deaktiviert";}
  HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
    <a>Status - OpenWeatherMap:&nbsp&nbsp"+StatusOpenWeatherMap+" - "+FormatedDateTime()+"</a>\
    <a><br /></a>\
    <a><br /></a>\
    <h2>Auswahl der Helligkeitsstufen:</h2>\
    <a href =\"/moff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display AUS</button></a>\
    <a href =\"/mon\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display EIN</button></a>\
    <a href =\"/m1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB1+"</button></a>\
    <a href =\"/m2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB2+"</button></a>\
    <a href =\"/m3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB3+"</button></a>\
    <a href =\"/m4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB4+"</button></a>\
    <a href =\"/m5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB5+"</button></a>\
    <a>"+DisplayAutomaticOnOff+"</a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Status Doppelpunkt:</h2>\
    <a href =\"/sdflash\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP1+"</button></a>\
    <a href =\"/sdon\"><button class=\"button\"style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP2+"</button></a>\
    <a href =\"/sdoff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP3+"</button></a>\ 
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
    <h2>Speichern der Einstellungen:</h2>\
    <a href =\"/esave\"><button class=\"button\" style=\"margin-right: 0px;height: 26px; width: 90px;\">Speichern</button></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px;height: 28px; width: 160px;color: red;float: right;\">Zurücksetzen! (Reset)</button></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a style=\"font-family:arial;color:black;font-size: 12px\">*) - aktuelle Auswahl</a>\
    </body>\
    </html>";
  }
   else
     { // ohne OpenWeatherMap
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
    <p></p>\
    <a></a>\
    <h2>Auswahl der Helligkeitsstufen:</h2>\
    <a href =\"/moff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display AUS</button></a>\
    <a href =\"/mon\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">Display EIN</button></a>\
    <a href =\"/m1\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB1+"</button></a>\
    <a href =\"/m2\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB2+"</button></a>\
    <a href =\"/m3\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB3+"</button></a>\
    <a href =\"/m4\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB4+"</button></a>\
    <a href =\"/m5\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+MB5+"</button></a>\
    <a>"+DisplayAutomaticOnOff+"</a>\
    <p></p>\
    <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
    <h2>Auswahl - Status Doppelpunkt:</h2>\
    <a href =\"/sdflash\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP1+"</button></a>\
    <a href =\"/sdon\"><button class=\"button\"style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP2+"</button></a>\
    <a href =\"/sdoff\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 110px;\">"+SDP3+"</button></a>\
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
    <h2>Speichern der Einstellungen:</h2>\
    <a href =\"/esave\"><button class=\"button\" style=\"margin-right: 0px;height: 26px; width: 90px;\">Speichern</button></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px;height: 28px; width: 160px;color: red;float: right;\">Zurücksetzen! (Reset)</button></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a style=\"font-family:arial;color:black;font-size: 12px\">*) - aktuelle Auswahl</a>\
    </body>\
    </html>";   
     }
  // Daten senden
  // char WebMessage[1428];
  // snprintf(WebMessage, 1428,HTMLString.c_str());
  server.send(200, "text/html",HTMLString);
  yield();
  MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
}

// Matrix einschalten
void MatrixOn()
{
Display = 1;
initMAX7219();
sendCmdAll(CMD_INTENSITY, 0);
sendCmdAll(CMD_SHUTDOWN, 1);
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">LED Matrix eingeschaltet</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString); 
}

// Matrix ausschalten
void MatrixOff()
{
Display = 0;  
sendCmdAll(CMD_SHUTDOWN,0);
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">LED Matrix ausgeschaltet</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DoublePointFlash()
{
DoublePointStatus = 2; // Doppelpunkt der Uhr blinken lassen 
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Doppelpunkt der Uhr blinkt.</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString); 
Serial.println("Doppelpunkt der Uhr blinkt.");
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DoublePointOn()
{
DoublePointStatus = 1; // Doppelpunkt der Uhr permanant AN
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Doppelpunkt der Uhr permanant an.</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString); 
Serial.println("Doppelpunkt der Uhr permanant an.");
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DoublePointOff()
{
DoublePointStatus = 0; // Doppelpunkt der Uhr permanant aus
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Doppelpunkt der Uhr permanant aus.</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Doppelpunkt der Uhr permanant aus.");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Helligkeitsstufen der LED's des Max7219 per Webserver einstellen
void BrightnessLevel1()
{
// Serial.println("Matrix: Helligkeitsstufe 1 aktiviert)");
sendCmdAll(CMD_INTENSITY, 0);
Brightness = "L1";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Helligkeitsstufe 1 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Helligkeitsstufe 1 aktiviert");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void BrightnessLevel2()
{
// Serial.println("Matrix: Helligkeitsstufe 2 aktiviert");
sendCmdAll(CMD_INTENSITY, 1);
Brightness = "L2";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Helligkeitsstufe 2 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Helligkeitsstufe 2 aktiviert");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void BrightnessLevel3()
{
// Serial.println("Matrix: Helligkeitsstufe 3 aktiviert");
sendCmdAll(CMD_INTENSITY, 2);
Brightness = "L3";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Helligkeitsstufe 3 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Helligkeitsstufe 3 aktiviert");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void BrightnessLevel4()
{
// Serial.println("Matrix: Helligkeitsstufe 4 aktiviert");
sendCmdAll(CMD_INTENSITY, 3);
Brightness = "L4";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Helligkeitsstufe 4 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Helligkeitsstufe 4 aktiviert");
}
void BrightnessLevel5()
{
 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Serial.println("Matrix: Helligkeitsstufe 5 aktiviert");
sendCmdAll(CMD_INTENSITY, 4);
Brightness = "L5";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Helligkeitsstufe 5 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Helligkeitsstufe 5 aktiviert");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Geschwindigkeit der Laufschrift einstellen
void ScrollTextTimeLevel1()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe1");
ScrollTextTime   = 60;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 1 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe1 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel2()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe2");
ScrollTextTime   = 55;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 2 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 2 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel3()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe3");
ScrollTextTime   = 50;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 3 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 3 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel4()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe4");
ScrollTextTime   = 45;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 4 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 4 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel5()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe5");
ScrollTextTime   = 40;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 5 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 5 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel6()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe6");
ScrollTextTime   = 35;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 6 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 6 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel7()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe7");
ScrollTextTime   = 30;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 7 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 7 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ScrollTextTimeLevel8()
{
// Serial.println("Matrix: Laufschriftgeschwindigkeit Stufe8");
ScrollTextTime   = 25;
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Laufschriftgeschwindigkeit Stufe 8 aktiviert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Laufschriftgeschwindigkeit Stufe 8 aktiviert");
printStringWithShift("        +++  Das ist ein Laufschrift Geschwindigkeitstest  +++           ",ScrollTextTime); 
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void EnhancedFunctionsEin()
{
// Serial.println("Zusatzfunktionen eingeschaltet");
if (EnhancedFunctions == false || StatusOpenWeatherMap != "OK") {
EnhancedFunctions = true;
StatusOpenWeatherMap = "***Bitte warten***";
Serial.println("");   
Serial.println("Lade Wetterdaten - Bitte warten ... ");   
printStringWithShift("     +++    Lade Wetterdaten - Bitte warten ...   +++           ", ScrollTextTime); // Wetterdaten holen 
ProgramSetup = true;  
getWeatherData(); // Wetterdaten von OpenWeatherMap holen
ProgramSetup = false; 
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Zusatzfunktionen eingeschaltet - Wetterdaten aktualisiert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Zusatzfunktionen eingeschaltet - Wetterdaten aktualisiert");
}
 else
   {
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Zusatzfunktionen eingeschaltet</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Zusatzfunktionen eingeschaltet");    
   }
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void EnhancedFunctionsAus()
{
// Serial.println("Zusatzfunktionen ausgeschaltet");
EnhancedFunctions = false;
StatusOpenWeatherMap = "deaktiviert";
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Zusatzfunktionen ausgeschaltet (Es wird nur noch die Uhrzeit angezeigt)</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("Zusatzfunktionen ausgeschaltet (Es wird nur noch die Uhrzeit angezeigt)");
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SaveWebSettings()
{
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
MyWaitLoop(100);
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Die Einstellungen wurden gespeichert</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetClockSettings()
{
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Alle Einstellungen auf Standard zurücksetzen ! (WLAN-Zugangsdaten werden gelöscht)</a>\
     <a><br /></a>\
     <a><br /></a>\
     <a style=\"font-family:arial;color:black;font-size: 16px\ style=\"margin-left: 80px\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Jetzt alle Einstellungen zurücksetzen ?</a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 70px;height: 28px; width: 90px\">JA</button></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 28px; width: 90px\">NEIN</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);    
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetClockYes()
{
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ LED-Matrix-Uhr +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 16px\">Alle Einstellungen werden auf Standard zurückgesetzt. (WLAN-Zugangsdaten werden gelöscht)</a>\
     <a><br /></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a style=\"font-family:arial;color:black;font-size: 16px\ style=\"margin-left: 10px\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Der Access Point wird gestartet.</a>\
    <a><br /></a>\
    <a><br /></a>\
    <h1>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Bitte warten ...</h1>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString); 
if (littleFSInit == true) {
ResetClock();} // Alle Einstellungen auf Standard - WLAN Zugangsdaten werden gelöscht     
}
