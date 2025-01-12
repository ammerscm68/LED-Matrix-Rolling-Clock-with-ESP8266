void DHTHTML() { 
  String HTMLString = F("");
  String RefreshTime = F("30"); // Aktualisierung der Website nach X Sekunden
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Temperatur Wert abrufen
  if (SerialMonitor) {Serial.println("Innentemperatur: "+InTempValue+" °C");}
  MyWaitLoop(500); // kurz warten
  if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Luftfeuchte Wert abrufen
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
  StatusWarningLED("HIGH"); // LED Einschalten  
  HumValueColor = "style=\"color: #FC1E17; font-size: 18px; margin-left: 377px;\">bei einer Luftfeuchte von:&nbsp;"+InHumValue+"";} else { 
  StatusWarningLED("LOW"); // LED Ausschalten   
  HumValueColor = "style=\"color: lime; font-size: 18px; margin-left: 377px;\">bei einer Luftfeuchte von:&nbsp;"+InHumValue+"";}
  
  HTMLString = F("<!DOCTYPE html>");
  HTMLString += F("<html lang='de'>");
  HTMLString += F("<head>");
  HTMLString += F("<title>LED-Matrix-Uhr Temperaturanzeige</title>");
  HTMLString += F("<meta http-equiv='content-type' content='text/html'; charset='utf-8'>");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
  HTMLString += F("h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
  HTMLString += F("h2 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h3 { font-size: 32px; margin-top: 0px; margin-bottom: 10px; }"); 
  HTMLString += F("h4 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }"); 
  HTMLString += F("#Rvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FC1E17; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("#Gvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: lime; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("#Bvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #2ECCFA; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("</style>"); 
  HTMLString += F("</head>"); 
  HTMLString += F("<body>");
  HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
  HTMLString += F("<h3>Temperaturanzeige</h3>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<h4>Letzte Messung: &nbsp"+LastInTempMessureDateTime+"</h4>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<table>"+TempValueColor+"</table>";
  HTMLString += "<a "+HumValueColor+"%</a>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">zurück zur Startseite</button></a>");
  HTMLString += "<a "+RefreshButton+"</a>";
  HTMLString += F("<a href =\"/dhtcalibrate\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px; color: red; font-size: 16px;\">Sensor Kalibrieren</button></a>");
  HTMLString += F("</body>");
  HTMLString += F("</html>");
  SendHTMLString(HTMLString); // Send HTMLString 
  MyWaitLoop(100);}

// =====================================================================================================================================

String InTempMessure() {
String ITempValue = F("");  
int IntITempValue = -1;  
if (InTempValue != "!") {  
  if (SerialMonitor) {Serial.println(F(""));
  Serial.println(F("Innentemperatur wird gemessen ..."));}
  float TempWert = dht.getTemperature();
  ITempValue = String(TempWert,0);
  ITempValue.replace("-0", "0");
  ITempValue.replace("+0", "0");
  ITempValue.replace("+", "");
  ITempValue.trim(); // Leerzeichen entfernen
  if (isnan(TempWert) || ITempValue == "" || ITempValue == "nan" || (!IsNumeric(ITempValue))) {ITempValue = F("-");} 
  if (ITempValue == "-") {
  if (SerialMonitor) {Serial.println(F("keinen gültigen Wert vom Sensor empfangen !"));}
  LastInTempMessureDateTime = F("k.A. / k.A.");}
  else {
  IntITempValue = ITempValue.toInt();
  IntITempValue = IntITempValue + InTempOffsetInt; 
  ITempValue = String(IntITempValue);   
  if (SerialMonitor) {Serial.println("Innentemperatur: "+ITempValue+" °C");} // nur im Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeDE(true);}
  return ITempValue;} else {ITempValue = "!"; return ITempValue;}
  }

// ====================================================================================================================================

String InHumMessure() {
  int IntIHumValue = -1;  
  if (SerialMonitor) {Serial.println(F(""));
  Serial.println(F("Luftfeuchte wird gemessen ..."));}
  float LfWert = dht.getHumidity();
  String IHumValue = String(LfWert,0); 
  IHumValue.replace("-0", "0");
  IHumValue.replace("+0", "0");
  IHumValue.replace("+", "");
  IHumValue.trim(); // Leerzeichen entfernenen
  if (isnan(LfWert) || IHumValue == "" || IHumValue == "nan" || (!IsNumeric(IHumValue))) {IHumValue = F("-");} 
  if (IHumValue == "-") {
  if (SerialMonitor) {Serial.println(F("keinen gültigen Wert vom Sensor empfangen !"));}
  LastInTempMessureDateTime = F("k.A. / k.A.");}
  else {
  IntIHumValue = IHumValue.toInt();
  IntIHumValue = IntIHumValue + InHumOffsetInt; 
  IHumValue = String(IntIHumValue); 
  if (SerialMonitor) {Serial.println("Innenluftfeuchte: "+IHumValue+" %");} // nur im Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeDE(true);} 
  return IHumValue;}

// ====================================================================================================================================  

String CheckInHumidity() {
// Innenfeuchtigkeit - Meldung wenn zu niedrig oder zu hoch
String RLF = F("");
if (InHumValue != "!") {
if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Luftfeuchte Wert abrufen  
MyWaitLoop(100); // kurz warten
  if (InHumValue != "-") {
  if (InTempOffset != "999") {    
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {RLF = F("MIN");     
  // Luftfeuchte zu niedrig 
  StatusWarningLED("HIGH");  // LED  Einschalten    
  InHumMessage = F("Achtung: Luftfeuchte zu niedrig !");}
  if (IntInHumValue >= MaxComfortHum) {RLF = F("MAX");
  // Luftfeuchte zu hoch     
  StatusWarningLED("HIGH");  // LED  Einschalten 
  if (WeatherFunctions and Display){
  if (humidity >= MaxComfortHum || humidity == -1) {InHumMessage = F("Achtung: Luftfeuchte zu hoch !");} else 
  {InHumMessage = F("Achtung: Luftfeuchte zu hoch ! - Bitte Lüften");}
  } else {InHumMessage = F("Achtung: Luftfeuchte zu hoch !");}}
  if (RLF != "MIN" && RLF != "MAX") {RLF = F("NORMAL"); StatusWarningLED("LOW"); // LED Ausschalten
  InHumMessage = F("");}
  } else {RLF = F("NORMAL"); InHumMessage = F("DHT-Sensor deaktiviert !"); StatusWarningLED("LOW");} // LED Ausschalten
  } else {StatusWarningLED("LOW"); // LED Ausschalten
  InHumValue = F("!");
  RLF = F("FAIL");
  InHumMessage = F("");}} else {StatusWarningLED("LOW"); RLF = F("FAIL");} // LED Ausschalten und Fail
  return RLF;  
}

// ====================================================================================================================================  

void DHTSensorCalibrate() {
String HTMLString;
bool MDTime;
if (!await) {
// Wenn gespeichert Button betätigt wurde 
if (server.method() == HTTP_POST) {
    // DHT Temperatur Sensor Offset  
    InTempOffset = server.arg("DHTTo");
    InTempOffset.replace("+", "");
    InTempOffset.trim();
    if (InTempOffset != "") {
    if (!IsNumeric(InTempOffset)) {InTempOffset = F("!");} else {
    InTempOffsetInt = InTempOffset.toInt();}
    } else {InTempOffset = F("0");}
    // DHT Luftfeuchte Sensor Offset
    InHumOffset = server.arg("DHTHo");
    InHumOffset.replace("+", "");
    InHumOffset.trim(); 
    if (InHumOffset != "") {
    if (!IsNumeric(InHumOffset)) {InHumOffset = F("!");} else {
    InHumOffsetInt = InHumOffset.toInt();} 
    } else {InHumOffset = F("0");}

    // Wenn Eingabefehler
    if (InTempOffset == "!" || InHumOffset == "!") {
    HTMLString = F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Uhr</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(DHT11-Sensor Kalibrierung)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Kalibrierung)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Ungültige Eingabe(n) - Bitte korrigieren!</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/dhtcalibrate\"><button>Zurück</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    } else {
    if (littleFSInit){
        // *** Eingaben im FileSystem speichern ***
    InTempOffset.trim(); // DHT-Sensor Offset (Temperatur)
    InHumOffset.trim(); // DHT-Sensor Offset (Luftfeuchte)
    if (InTempOffset != ""){
    SetupSave("dht-intempoffset.txt", InTempOffset);} else {
    if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
    if (InHumOffset != ""){
    SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
    if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {Serial.println(F("DHT-Sensor Offset Daten gespeichert... "));}
    // Website anzeigen  
    HTMLString = F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Uhr</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(DHT11-Sensor Kalibrierung)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Kalibrierung)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Die Einstellungen wurden gespeichert</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>OK</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    CheckInHumidity();} // Innenluftfeuchte Messen 
    else { 
    // DHT-Sensor Offset Einstellungen wurden nicht gespeichert
     if (SerialMonitor) {Serial.println(F("DHT-Sensor Offset Einstellungen wurden nicht gespeichert !"));}
    HTMLString = F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Uhr</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");   
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(DHT11-Sensor Kalibrierung)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Kalibrierung)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Fehler beim Speichern der Einstellungen</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Zurück</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Send HTMLString
	MyWaitLoop(100);
    StatusWarningLED("LOW"); // LED Ausschalten 
    }} // Ende Eingabefehler
  } else {  
    // Einstellungen
    HTMLString =  F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Uhr</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:430px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/dhtcalibrate' method='post'>");
    HTMLString += F("<h1>LED-Matrix-Uhr</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(DHT11-Sensor Kalibrierung)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Kalibrierung)</h5>");}
    // ******************************************* Parameter ********************************************************************************************************************************************************************************
    if (DHT11Sensor) { 
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Temperatur Offset* (Beispiel: -1 oder +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Luftfeuchte Offset (Beispiel: -1 oder +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";} else {
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Temperatur Offset* (Beispiel: -1 oder +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Luftfeuchte Offset (Beispiel: -1 oder +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";}
    // ******************************************* Parameter ********************************************************************************************************************************************************************************
    HTMLString += F("<h6>*) - 999 bei Temperatur deaktiviert den DHT-Sensor (keine Anzeige im LED-Matrix Display)</h6>");
    HTMLString += F("<button type='submit'>Speichern</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Abbrechen</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);
    MyWaitLoop(100);}} else {PleaseWait();}} // Send HTMLString oder Warten

// ====================================================================================================================================  
// ====================================================================================================================================  