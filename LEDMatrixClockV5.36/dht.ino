void DHTHTML() {  
  InTempValue = InTempMessure(); // Werte abrufen
  // Serial.println("Innentemperatur: "+TempValue+" °C");
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Temperaturanzeige</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <style>\
  body { background-color: #585858; font-size: 50px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 40px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  #temp { width: 230px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>Temperaturanzeige</h1>\
  <a><br /></a>\
  <a>Letzte Messung:</a>\
  <a><br /></a>\
  <a>"+LastInTempMessureDateTime+"</a>\
  <a><br /></a>\
  <a><br /></a>\
  <table><tr><td style=\"height: 10px;\">Aktuelle Innentemperatur:&nbsp;</td>\
  <td id='temp'><span>"+InTempValue+"</span>°C</td></tr></table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 160px\">zurück zur Startseite</button></a>\
  <a href =\"tempvalue\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 160px\">Aktualisieren</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  </body>\
  </html>\
  ");
}

// =====================================================================================================================================

String InTempMessure() {
  Serial.println("");
  Serial.println("Innentemperatur wird gemessen ...");
  float TempWert = dht.getTemperature();
  String ITempValue = String(TempWert-1,0);    // -1 = offset --> -1 Grad
  // Serial.println("Innentemperatur: "+InTempValue); // nur im Debugmodus
  if (isnan(TempWert) || ITempValue == "" || ITempValue == "nan") {ITempValue = "-";} 
  if (ITempValue == "-") 
  {Serial.println("kein Wert vom Sensor empfangen !");
  LastInTempMessureDateTime = "k.A. / k.A.";}
  else {LastInTempMessureDateTime = FormatedDateTime();}
  return ITempValue;
}
// ====================================================================================================================================
