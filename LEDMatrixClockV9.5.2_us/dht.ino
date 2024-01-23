void DHTHTML() { 
  String RefreshTime = "30"; // Refresh Website after X seconds
  InTempValue = InTempMessure(); // Retrieve temperature value
  if (SerialMonitor) {Serial.println("Indoor Temperature: "+InTempValue+" °F");}
  MyWaitLoop(500); // short wait
  InHumValue = InHumMessure(); // Retrieve Air Humidity Value
  if (SerialMonitor) {Serial.println("Indoor Air Humidity: "+InHumValue+" %");}
  String TempValueColor, HumValueColor, RefreshButton;
  if (InTempValue != "-" && InHumValue != "-") {
  IntInTempValue = InTempValue.toInt(); 
  IntInHumValue = InHumValue.toInt(); 
  RefreshButton = "href =\"tempvalue\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Refresh</button>";} else {
  RefreshButton = "href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Refresh</button>";}
  // Evaluation Temperature
  if (IntInTempValue < 50) {
  TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp;&nbsp;</td><td id='Bvalue'><span>"+InTempValue+"</span>°F</td></tr>";  
  } else {
  if (IntInTempValue >= 50 && IntInTempValue <= 82) {TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp&nbsp</td><td id='Gvalue'><span>"+InTempValue+"</span>°F</td></tr>";} else {  
  TempValueColor = "<tr><td style=\"height: 10px;\">Current Indoor Temperature:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+InTempValue+"</span>°F</td></tr>";}
  }
  // Evaluation of Air Humidity
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED on  
  HumValueColor = "style=\"color: #FC1E17; font-size: 18px; margin-left: 430px;\">at an Air Humidity of:&nbsp;"+InHumValue+"";} else { 
  digitalWrite(HWLEDPin, LOW); // LED off   
  HumValueColor = "style=\"color: lime; font-size: 18px; margin-left: 430px;\">at an Air Humidity of:&nbsp;"+InHumValue+"";}
  
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html lang='en-US'>\
  <head>\
  <title>LED-Matrix-Clock Temperature Display</title>\
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
  <h1>LED-Matrix-Clock</h1>\
  <h3>Temperature Display</h3>\
  <h2><br /></h2>\
  <h4>Last Measurement: &nbsp"+LastInTempMessureDateTime+"</h4>\
  <h2><br /></h2>\
  <table>"+TempValueColor+"</table>\
  <a "+HumValueColor+"%</a>\
  <h2><br /></h2>\
  <h2><br /></h2>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">back to Homepage</button></a>\
  <a "+RefreshButton+"</a>\
  </body>\
  </html>\
  ");}

// =====================================================================================================================================

String InTempMessure() {
int IntITempValue = -1;  
  if (SerialMonitor) {Serial.println("");
  Serial.println("Indoor Temperature is measured ...");}
  float TempValue = dht.getTemperature();
  String ITempValue = String(dht.toFahrenheit(TempValue),0); // Output in Fahrenheit
  ITempValue.replace("-0", "0");
  ITempValue.replace("+0", "0");
  ITempValue.replace("+", "");
  ITempValue.trim(); // Remove Spaces
  if (isnan(TempValue) || ITempValue == "" || ITempValue == "nan" || (!IsNumeric(ITempValue))) {ITempValue = "-";} 
  if (ITempValue == "-") {
  if (SerialMonitor) {Serial.println("No valid Value received from the Sensor !");}
  LastInTempMessureDateTime = "n/a / n/a";}
  else {
  IntITempValue = ITempValue.toInt();
  IntITempValue = IntITempValue + InTempOffsetInt; 
  ITempValue = String(IntITempValue);   
  if (SerialMonitor) {Serial.println("Indoor Temperature: "+ITempValue+" °F");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTime();}
  return ITempValue;}

// ====================================================================================================================================

String InHumMessure() {
int IntIHumValue = -1;  
  if (SerialMonitor) {Serial.println("");
  Serial.println("Indoor Air Humidity is measured ...");}
  float LfWert = dht.getHumidity();
  String IHumValue = String(LfWert,0); 
  IHumValue.replace("-0", "0");
  IHumValue.replace("+0", "0");
  IHumValue.replace("+", "");
  IHumValue.trim(); // Remove Spaces
  if (isnan(LfWert) || IHumValue == "" || IHumValue == "nan" || (!IsNumeric(IHumValue))) {IHumValue = "-";} 
  if (IHumValue == "-") {
  if (SerialMonitor) {Serial.println("No valid Value received from the Sensor !");}
  LastInTempMessureDateTime = "n/a / n/a";}
  else {
  IntIHumValue = IHumValue.toInt();
  IntIHumValue = IntIHumValue + InHumOffsetInt; 
  IHumValue = String(IntIHumValue); 
  if (SerialMonitor) {Serial.println("Indoor Air Humidity: "+IHumValue+" %");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTime();} 
  return IHumValue;}

// ====================================================================================================================================  

  void TriggerHumityLED() {
  if (InHumValue != "-") {  
  InHumValue = InHumMessure(); // Humidity measurement
  if (InHumValue != "-") {
  if (IsNumeric(InHumValue)){  
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {digitalWrite(HWLEDPin, HIGH);} else {digitalWrite(HWLEDPin, LOW);}
  }}}}

// ====================================================================================================================================
