void DHTHTML() { 
  String RefreshTime = "30"; // Aktualisierung der Website nach X Sekunden
  InTempValue = InTempMessure(); // Temperatur Wert abrufen
  if (SerialMonitor) {Serial.println("Innentemperatur: "+InTempValue+" °C");}
  MyWaitLoop(500); // kurz warten
  InHumValue = InHumMessure(); // Luftfeuchte Wert abrufen
  if (SerialMonitor) {Serial.println("Innenluftfeuchte: "+InHumValue+" %");}
  String TempValueColor, HumValueColor, RefreshButton;
  if (InTempValue != "-" && InHumValue != "-") {
  IntInTempValue = InTempValue.toInt(); 
  IntInHumValue = InHumValue.toInt(); 
  RefreshButton = "href =\"tempvalue\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Aktualisieren</button>";} else {
  RefreshButton = "href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Aktualisieren</button>";}
  // Auswertung Temperatur
  if (IntInTempValue < 10) {
  TempValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Innentemperatur:&nbsp;&nbsp;</td><td id='Bvalue'><span>"+InTempValue+"</span>°C</td></tr>";  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {TempValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Innentemperatur:&nbsp&nbsp</td><td id='Gvalue'><span>"+InTempValue+"</span>°C</td></tr>";} else {  
  TempValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Innentemperatur:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+InTempValue+"</span>°C</td></tr>";}
  }
  // Auswertung Luftfeuchte
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED Einschalten  
  HumValueColor = "style=\"color: #FC1E17; font-size: 18px; margin-left: 377px;\">bei einer Luftfeuchte von:&nbsp;"+InHumValue+"";} else { 
  digitalWrite(HWLEDPin, LOW); // LED Ausschalten   
  HumValueColor = "style=\"color: lime; font-size: 18px; margin-left: 377px;\">bei einer Luftfeuchte von:&nbsp;"+InHumValue+"";}
  
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>LED-Matrix-Uhr Temperaturanzeige</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
   h2 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }\
   h3 { font-size: 32px; margin-top: 0px; margin-bottom: 10px; }\
   h4 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }\
   #Rvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FC1E17; background-color: #000000; font-size: 60px; }\
   #Gvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: lime; background-color: #000000; font-size: 60px; }\
   #Bvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #2ECCFA; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>LED-Matrix-Uhr</h1>\
  <h3>Temperaturanzeige</h3>\
  <h2><br /></h2>\
  <h4>Letzte Messung: &nbsp"+LastInTempMessureDateTime+"</h4>\
  <h2><br /></h2>\
  <table>"+TempValueColor+"</table>\
  <a "+HumValueColor+"%</a>\
  <h2><br /></h2>\
  <h2><br /></h2>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>\
  <a "+RefreshButton+"</a>\
  </body>\
  </html>\
  ");
}

// =====================================================================================================================================

String InTempMessure() {
int IntITempValue = -1;  
  if (SerialMonitor) {Serial.println("");
  Serial.println("Innentemperatur wird gemessen ...");}
  float TempWert = dht.getTemperature();
  String ITempValue = String(TempWert,0);
  if (isnan(TempWert) || ITempValue == "" || ITempValue == "nan" || (not IsNumeric(ITempValue))) {ITempValue = "-";} 
  if (ITempValue == "-") {
  if (SerialMonitor) {Serial.println("keinen gültigen Wert vom Sensor empfangen !");}
  LastInTempMessureDateTime = "k.A. / k.A.";}
  else {
  IntITempValue = ITempValue.toInt();
  IntITempValue = IntITempValue + InTempOffsetInt; 
  ITempValue = String(IntITempValue);   
  if (SerialMonitor) {Serial.println("Innentemperatur: "+ITempValue+" °C");} // nur im Debugmodus
  LastInTempMessureDateTime = FormatedDateTime();}
  return ITempValue;
}

// ====================================================================================================================================

String InHumMessure() {
int IntIHumValue = -1;  
  if (SerialMonitor) {Serial.println("");
  Serial.println("Luftfeuchte wird gemessen ...");}
  float LfWert = dht.getHumidity();
  String IHumValue = String(LfWert,0); 
  if (isnan(LfWert) || IHumValue == "" || IHumValue == "nan" || (not IsNumeric(IHumValue))) {IHumValue = "-";} 
  if (IHumValue == "-") {
  if (SerialMonitor) {Serial.println("keinen gültigen Wert vom Sensor empfangen !");}
  LastInTempMessureDateTime = "k.A. / k.A.";}
  else {
  IntIHumValue = IHumValue.toInt();
  IntIHumValue = IntIHumValue + InHumOffsetInt; 
  IHumValue = String(IntIHumValue); 
  if (SerialMonitor) {Serial.println("Luftfeuchte: "+IHumValue+" %");} // nur im Debugmodus
  LastInTempMessureDateTime = FormatedDateTime();} 
  return IHumValue;
}

// ====================================================================================================================================
