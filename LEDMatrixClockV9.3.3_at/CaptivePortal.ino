/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Captive Portal IP-Adresse - outside of the router's own DHCP range - Default = 192.168.4.1

String ProgramName = "LED-Matrix-Uhr";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// Start Captive Portal (Access Point)
WiFi.mode(WIFI_AP);
WiFi.softAPConfig(cpIP, cpIP, IPAddress(255, 255, 255, 0));   //Captive Portal IP-Adress
WiFi.softAP(ProgramName, "");
// Webserver - Ausgabe wenn Link nicht gefunden wurde
server.onNotFound(WebSiteNotFound);
server.on("/", handlePortal);
server.begin();}

// *****************************************************************************************************************************************************

void handlePortal() {
String HTMLString;
if (ProgramName == "") {ProgramName = "Hostname";} 
// Wenn gespeichert Button betätigt wurde 
if (server.method() == HTTP_POST) {
    // Einstellungen speichern
    TempSSID = server.arg("ssid"); // Wifi SSID
    password = server.arg("password"); // Wifi SSID Passwort
    weatherKey = server.arg("apikey"); // OpenWeatherMap - API-Key
    cityID = server.arg("cityid"); // OpenWeatherMap - City ID
    newsKey = server.arg("napi"); // NewsAPI-Key
    if (InTempValue != "-") {
    // DHT Temperature Sensor Offset  
    InTempOffset = server.arg("DHTTo");
    InTempOffset.replace("+", "");
    InTempOffset.trim();
    if (InTempOffset != "") {
    if (!IsNumeric(InTempOffset)) {InTempOffset = "!";}
    } else {InTempOffset = "0";}
    // DHT Humidity Sensor Offset
    InHumOffset = server.arg("DHTHo");
    InHumOffset.replace("+", "");
    InHumOffset.trim(); 
    if (InHumOffset != "") {
    if (!IsNumeric(InHumOffset)) {InHumOffset = "!";}
    } else {InHumOffset = "0";}
    } else {
    InTempOffset = "0"; InHumOffset = "0";}

    // Buzzer installiert ?
    if (server.arg("IsBuZZer") == "True") 
    {IsBuzzer = true;} else {IsBuzzer = false;} 
    
    // Trim Strings
    TempSSID.trim(); password.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); 

    if (TempSSID != "" && password != "" && InTempOffset != "!" && InHumOffset != "!") { 
    Serial.println("SSID: "+TempSSID); // Wifi SSID
    Serial.println("Passwort: "+String(password)); // Wifi SSID Passwort

    if (littleFSInit){
    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>Captive-Portal</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>Captive-Portal</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Zugangsdaten und Einstellungen wurden gespeichert<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    SendHTMLString(HTMLString); // Send HTMLString 
    SaveMatrixAdjustments(); // Eingaben im FileSystem speichern
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {Serial.println("Zugangsdaten gespeichert... ");} 
    printStringWithShift("        +++       Zugangsdaten und Einstellungen gespeichert...      +++           ",ScrollTextTime); 
    if (SerialMonitor) {Serial.println("Die Uhr wird neu gestartet");} 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    // Reset auslösen
    ESP.restart();} else
     { // Zugangsdaten wurden nicht gespeichert
     if (SerialMonitor) {Serial.println("Zugangsdaten wurden nicht gespeichert !");}
     HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>Captive-Portal</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";    
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>Captive-Portal</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    SendHTMLString(HTMLString); // Send HTMLString 
    printStringWithShift("        +++       Zugangsdaten und Einstellungen wurden nicht gespeichert !     +++           ",ScrollTextTime);
    // Reset auslösen 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();}} else {
    // unvollständige Einstellungen    
    HTMLString =  "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>Captive-Portal</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:450px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>Captive-Portal</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Einstellungen sind unvollständig oder ungültig !<br />Einstellungen wurden nicht gespeichert</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    SendHTMLString(HTMLString);} // Send HTMLString 
  } else {
    // Captive Portal Einstellungen
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
    SendHTMLString(""); // Send HTMLString 
    server.sendContent(CPRootHTMLString_1()); // 1. Teil
    server.sendContent(CPRootHTMLString_2()); // 2. Teil
    server.sendContent(CPRootHTMLString_3()); // 3. Teil
    MyWaitLoop(100);
    server.client().flush();
    MyWaitLoop(100);
    server.client().stop();}}

// *****************************************************************************************************************************************************

String CPRootHTMLString_1(){
String HTMLString;  
    HTMLString =  "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>Captive-Portal</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; margin-left: 110px; margin-top: 15px; margin-bottom: 5px; color: #0245b0;}";
    HTMLString += "h7 { font-size: 20px; font-weight: bold; margin-left: 110px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}";
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<form action='/' method='post'>";
    HTMLString += "<h1>Captive-Portal</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    return HTMLString;} 

// *****************************************************************************************************************************************************

String CPRootHTMLString_2(){
String HTMLString, ssidsearch; 
int loops = 0; // for Sort SSID's 
int n = WiFi.scanNetworks(false, false); //WiFi.scanNetworks(async, show_hidden)
    if (n > 0) {
     // WLAN's sort by RSSI
    int indices[n];
    int skip[n];
    int o = n;
    for (int i = 0; i < n; i++) {
    indices[i] = i;}
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            loops++;
            //int temp = indices[j];
            //indices[j] = indices[i];
            //indices[i] = temp;
            std::swap(indices[i], indices[j]);
            std::swap(skip[i], skip[j]);}}}
      for (int i = 0; i < n; i++) {
        if(indices[i] == -1){
          --o;
          continue;}
        ssidsearch = WiFi.SSID(indices[i]);
        for (int j = i + 1; j < n; j++) {
          loops++;
          if (ssidsearch == WiFi.SSID(indices[j])) {
            indices[j] = -1;}}}
    for (int i = 0; i < n; ++i){
    // Print SSID 
    if (i == 0) {
    HTMLString += "<h7 onclick='SetSSID"+(String)i+"()' id='fssid"+(String)i+"'>" + WiFi.SSID(indices[i]) + "</h7>";} else {
    HTMLString += "<h6 onclick='SetSSID"+(String)i+"()' id='fssid"+(String)i+"'>" + WiFi.SSID(indices[i]) + "</h6>";}
    HTMLString += "<script>";
    HTMLString += "function SetSSID"+(String)i+"() {document.getElementById('ssid').value = document.getElementById('fssid"+(String)i+"').innerHTML;}";
    HTMLString += "</script>";}
    } else {
    HTMLString += "<br/>";  
    HTMLString += "<br/>"; 
    HTMLString += "<h6>kein WLAN gefunden !</h6>";
    HTMLString += "<br/>";}
    HTMLString += "<br/>";
    HTMLString += "<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>";
    HTMLString += "<div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password' id='password' value=''></div>";
    HTMLString += "<br/>"; 
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - API-Key (Optional)</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - City-ID</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsAPI - API-Key (Optional)</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    if (InTempValue != "-") {
    if (DHT11Sensor) { 
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Temperatur Offset (Beispiel: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT11-Sensor Luftfeuchte Offset (Beispiel: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";} else {
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Temperatur Offset (Beispiel: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT22-Sensor Luftfeuchte Offset (Beispiel: -1 or +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";}}
    if (IsBuzzer) {
    HTMLString += "<div class='form-floating'><br/><label>&nbsp;Ein Passiv Piezo Summer verwenden&nbsp;";
    HTMLString += "(z.B. KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True' checked></div>";} else {
    HTMLString += "<div class='form-floating'><br/><label>&nbsp;Ein Passiv Piezo Summer verwenden&nbsp;";
    HTMLString += "(z.B. KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True'></div>";}
    // ******************************************* Parameter ************************************************************************************************************************************  
    return HTMLString;}  

// *****************************************************************************************************************************************************

String CPRootHTMLString_3(){
    String HTMLString;
    HTMLString += "<br/><br/>";
    HTMLString += "<button type='submit'>Speichern</button>";
    HTMLString += "</form>";
    HTMLString += "<br/><br/>";
    HTMLString += "<form action='/' method='get'>";
    HTMLString += "<button type='submit'>Aktualisieren</button>";
    HTMLString += "</form>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    return HTMLString;}

// *****************************************************************************************************************************************************

void WebSiteNotFound() {
String HTMLString;   
HTMLString =  "<!doctype html><html lang='de'>";
HTMLString += "<head><meta charset='utf-8'>";
HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
HTMLString += "<title>Captive-Portal</title>";
HTMLString += "<style>";
HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:410px;padding:15px;margin:auto;}h1,p{text-align: center}";
HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
HTMLString += "h6 { font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body><main class='form-signin'>";
HTMLString += "<h1>Captive-Portal</h1>";
HTMLString += "<h5>("+ProgramName+")</h5>";
HTMLString += "<br/>";
HTMLString += "<h6>Die Webseite wurde nicht gefunden !</h6>";
HTMLString += "<br/><br/>";
HTMLString += "<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>";
HTMLString += "</main>";
HTMLString += "</body>";
HTMLString += "</html>";    
SendHTMLString(HTMLString);} // Send HTMLString   

// *****************************************************************************************************************************************************
// *****************************************************************************************************************************************************
