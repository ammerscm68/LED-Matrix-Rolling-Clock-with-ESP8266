void DHTHTML() { 
  String HTMLString; HTMLString.reserve(180);
  String RefreshTime = F("30"); // Refresh Website after X seconds
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Retrieve temperature value
  if (SerialMonitor) {DEBUG_PRINTLN("Indoor Temperature: "+InTempValue+" °C");}
  MyWaitLoop(500); // short wait
  if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Retrieve Air Humidity Value
  if (SerialMonitor) {DEBUG_PRINTLN("Indoor Air Humidity: "+InHumValue+" %");}
  String TempValueColor, HumValueColor, RefreshButton;
  if (InTempValue != "!" && InHumValue != "-") {
  IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10);
  IntInHumValue = (int16_t)strtol(InHumValue.c_str(), NULL, 10);
  RefreshButton = "href =\"tempvalue\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Refresh</button>";} else {
  RefreshButton = "href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Refresh</button>";}
  // Evaluation Temperature
  if (IntInTempValue < 50) {
  TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp;&nbsp;</td><td id='Bvalue'><span>"+InTempValue+"</span>°C</td></tr>";  
  } else {
  if (IntInTempValue >= 50 && IntInTempValue <= 83) {TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp&nbsp</td><td id='Gvalue'><span>"+InTempValue+"</span>°C</td></tr>";} else {  
  TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+InTempValue+"</span>°C</td></tr>";}
  }
  // Evaluation of Air Humidity
  if (IntInHumValue < MinComfortHum || IntInHumValue > MaxComfortHum) {
  StatusWarningLED("HIGH"); // LED on  
  HumValueColor = "style=\"color: #FC1E17; font-size: 18px; margin-left: 430px;\">at an Air Humidity of:&nbsp;"+InHumValue+"";} else { 
  StatusWarningLED("LOW"); // LED off   
  HumValueColor = "style=\"color: lime; font-size: 18px; margin-left: 430px;\">at an Air Humidity of:&nbsp;"+InHumValue+"";}
  
  HTMLString = F("<!DOCTYPE html>");
  HTMLString += F("<html lang='en-IE'>");
  HTMLString += F("<head>");
  HTMLString += F("<title>LED-Matrix-Clock Temperature Display</title>");
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
  HTMLString += F("<h1>LED-Matrix-Clock</h1>");
  HTMLString += F("<h3>Temperature Display</h3>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<h4>Last Measurement: &nbsp"+LastInTempMessureDateTime+"</h4>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<table>"+TempValueColor+"</table>";
  HTMLString += "<a "+HumValueColor+"%</a>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>");
  HTMLString += "<a "+RefreshButton+"</a>";
  HTMLString += F("<a href =\"/dhtcalibrate\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px; color: red; font-size: 16px;\">Calibrate Sensor</button></a>");
  HTMLString += F("</body>");
  HTMLString += F("</html>");
  SendHTMLString(HTMLString); // Send HTMLString 
  MyWaitLoop(100);}

// =====================================================================================================================================

String InTempMessure() {
String ITempValue;  
int16_t IntITempValue = -1;  
if (DHTSensor != "NoDHT") { 
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("Indoor Temperature is measured ..."));}
  float TempValue = dht.getTemperature();
  ITempValue = String(TempValue,0);
  ITempValue.replace("-0", "0");
  ITempValue.replace("+0", "0");
  ITempValue.replace("+", "");
  ITempValue.trim(); // Remove Spaces
  if (isnan(TempValue) || ITempValue == "" || ITempValue == "nan" || (!IsNumeric(ITempValue))) {ITempValue = F("!");} 
  if (ITempValue == "!") {
  if (SerialMonitor) {DEBUG_PRINTLN(F("No valid Value received from the Sensor !"));}
  LastInTempMessureDateTime = F("n/a / n/a");}
  else {
  IntITempValue = (int16_t)strtol(ITempValue.c_str(), NULL, 10);
  IntITempValue = IntITempValue + InTempOffsetInt; 
  ITempValue = String(IntITempValue);   
  if (SerialMonitor) {DEBUG_PRINTLN("Indoor Temperature: "+ITempValue+" °C");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeEN(true);}
  return ITempValue;} else {ITempValue = F("!"); return ITempValue;}}

// ====================================================================================================================================

String InHumMessure() {
  String IHumValue;  
  if (DHTSensor != "NoDHT") { 
  int16_t IntIHumValue = -1; 
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("Indoor Air Humidity is measured ..."));}
  float LfWert = dht.getHumidity();
  IHumValue = String(LfWert,0); 
  IHumValue.replace("-0", "0");
  IHumValue.replace("+0", "0");
  IHumValue.replace("+", "");
  IHumValue.trim(); // Remove Spaces
  if (isnan(LfWert) || IHumValue == "" || IHumValue == "nan" || (!IsNumeric(IHumValue))) {IHumValue = F("-");} 
  if (IHumValue == "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F("No valid Value received from the Sensor !"));}
  LastInTempMessureDateTime = F("n/a / n/a");}
  else {
  IntIHumValue = (int16_t)strtol(IHumValue.c_str(), NULL, 10);
  IntIHumValue = IntIHumValue + InHumOffsetInt; 
  IHumValue = String(IntIHumValue); 
  if (SerialMonitor) {DEBUG_PRINTLN("Indoor Air Humidity: "+IHumValue+" %");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeEN(true);}} else {IHumValue = F("-");} 
  return IHumValue;}

// ====================================================================================================================================

String CheckInHumidity() {
// Indoor Humidity - Message if too Low or too High
String RLF;
if (InHumValue != "!") {
if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Retrieve Humidity Value 
MyWaitLoop(100); // Short Wait
  if (InHumValue != "-") {
  if (InTempOffset != "999") {    
  IntInHumValue = (int16_t)strtol(InHumValue.c_str(), NULL, 10);
  if (IntInHumValue < MinComfortHum) {RLF = F("MIN");     
  // Humidity too Low
  StatusWarningLED("HIGH");  // LED  ON    
  InHumMessage = F("Attention: Humidity too Low !");}
  if (IntInHumValue > MaxComfortHum) {RLF = F("MAX");
  // Humidity too High    
  StatusWarningLED("HIGH");  // LED  ON 
  if (WeatherFunctions and Display){
  if (humidity > MaxComfortHum || humidity == -1) {InHumMessage = F("Attention: Humidity too High !");} else 
  {InHumMessage = F("Attention: Humidity too High ! - Please Ventilate");}
  } else {InHumMessage = F("Attention: Humidity too High !");}}
  if (RLF != "MIN" && RLF != "MAX") {RLF = F("NORMAL"); StatusWarningLED("LOW"); // LED OFF
  InHumMessage.clear();}
  } else {RLF = F("NORMAL"); InHumMessage = F("DHT Sensor deactivated !"); StatusWarningLED("LOW");} // LED OFF
  } else {StatusWarningLED("LOW"); // LED OFF
  InHumValue = F("!");
  RLF = F("FAIL");
  InHumMessage.clear();}} else {StatusWarningLED("LOW"); RLF = F("FAIL");} // LED OFF and Fail
  return RLF;  
}

// ====================================================================================================================================

void DHTSensorCalibrate() {
String HTMLString; HTMLString.reserve(180);
bool MDTime;
if (!await) {
// When saved Button was pressed 
if (server.method() == HTTP_POST) {
    // DHT Temperature Sensor Offset
    InTempOffset = server.arg("DHTTo");
    InTempOffset.replace("+", "");
    InTempOffset.trim();
    if (InTempOffset != "") {
    if (!IsNumeric(InTempOffset)) {InTempOffset = F("!");} else {
    InTempOffsetInt = (int16_t)strtol(InTempOffset.c_str(), NULL, 10);}
    } else {InTempOffset = F("0");}
    // DHT Humidity Sensor Offset
    InHumOffset = server.arg("DHTHo");
    InHumOffset.replace("+", "");
    InHumOffset.trim(); 
    if (InHumOffset != "") {
    if (!IsNumeric(InHumOffset)) {InHumOffset = F("!");} else {
    InHumOffsetInt = (int16_t)strtol(InHumOffset.c_str(), NULL, 10);} 
    } else {InHumOffset = F("0");}

    // If Input Error
    if (InTempOffset == "!" || InHumOffset == "!") {
    HTMLString = F("<!doctype html><html lang='en-IE'>");
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
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    if (DHTSensor == "DHT11") {
    HTMLString += F("<h5>(DHT11 Sensor Calibration)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Calibration)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Invalid entry(s) - Please correct!</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/dhtcalibrate\"><button>Back</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    } else {
    if (littleFSInit){
        // *** Save entries in the File System ***
    InTempOffset.trim(); // DHT-Sensor Offset (Temperatur)
    InHumOffset.trim(); // DHT-Sensor Offset (Humidity)
    if (InTempOffset != ""){
    SetupSave(F("dht-intempoffset.txt"), InTempOffset);} else {
    if (LittleFS.exists(F("/dht-intempoffset.txt"))){LittleFS.remove(F("/dht-intempoffset.txt"));}}
    if (InHumOffset != ""){
    SetupSave(F("dht-inhumoffset.txt"), InHumOffset);} else {
    if (LittleFS.exists(F("/dht-inhumoffset.txt"))){LittleFS.remove(F("/dht-inhumoffset.txt"));}}
    MyWaitLoop(500); // 0,5 sek. wait
    if (SerialMonitor) {DEBUG_PRINTLN(F("DHT Sensor offset Data saved... "));}
    // Website anzeigen  
    HTMLString = F("<!doctype html><html lang='en-IE'>");
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
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    if (DHTSensor == "DHT11") {
    HTMLString += F("<h5>(DHT11-Sensor Calibration)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Calibration)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>The Settings have been saved</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>OK</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    CheckInHumidity(); // Indoor Humidity measurement 
    MyWaitLoop(100);
    SendHTMLString(HTMLString);} // Send HTMLString 
    else { 
    // DHT Sensor Offset Settings were not saved
     if (SerialMonitor) {DEBUG_PRINTLN(F("DHT Sensor Offset Settings were not saved !"));}
    HTMLString = F("<!doctype html><html lang='en-IE'>");
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
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    if (DHTSensor == "DHT11") {
    HTMLString += F("<h5>(DHT11-Sensor Calibration)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Calibration)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Error saving Settings</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Back</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100);
    StatusWarningLED("LOW"); // LED switch off
    }} // End Inputterror
  } else {  
    // Settings
    HTMLString =  F("<!doctype html><html lang='en-IE'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>LED-Matrix-Clock</title>");
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
    HTMLString += F("<h1>LED-Matrix-Clock</h1>");
    if (DHTSensor == "DHT11") {
    HTMLString += F("<h5>(DHT11-Sensor Calibration)</h5>");} else{
    HTMLString += F("<h5>(DHT22-Sensor Calibration)</h5>");}
    // ******************************************* Parameter ****************************************************************************************************************************************************************************
    if (DHTSensor == "DHT11") { 
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Temperature Offset* (Example: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Humidity Offset (Example: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";} else {
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Temperature Offset* (Example: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Humidity Offset (Example: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";}
    // ******************************************* Parameter ****************************************************************************************************************************************************************************
    HTMLString += F("<h6>*) - 999 at Temperature deactivates the DHT-Sensor (no Display in the LED Matrix)</h6>");
    HTMLString += F("<button type='submit'>Save</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Cancel</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);
    MyWaitLoop(100);}} else {PleaseWait();}} // Send HTMLString or wait

// ====================================================================================================================================  