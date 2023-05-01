/* ***************** Captive Portal *********************/
IPAddress cpIP(192, 168, 4, 1);  // Captive Portal IP-Adresse - outside of the router's own DHCP range

String ProgramName = "LED-Matrix-Clock";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// Start Captive Portal (Access Point)
WiFi.mode(WIFI_AP);
WiFi.softAPConfig(cpIP, cpIP, IPAddress(255, 255, 255, 0));   //Captive Portal IP-Adress
WiFi.softAP(ProgramName, "");
// Webserver - Ausgabe wenn Link nicht gefunden wurde
server.onNotFound(WebSiteNotFound);
server.on("/", handlePortal);
server.begin();  
}

// *****************************************************************************************************************************************************

void handlePortal() {
String HTMLString; 
String ssidsearch;
int loops = 0; // for Sort SSID's
if (ProgramName == "") {ProgramName = "Hostname";} 
// Wenn gespeichert Button betätigt wurde 
if (server.method() == HTTP_POST) {
    // Einstellungen speichern
    TempSSID = server.arg("ssid"); // Wifi SSID
    password = server.arg("password"); // Wifi SSID Passwort
    weatherKey = server.arg("apikey"); // OpenWeatherMap - API-Key
    cityID = server.arg("cityid"); // OpenWeatherMap - City ID
    newsKey = server.arg("napi"); // NewsAPI-Key
    DisplayOnTime = server.arg("DOnT"); // Display On-Time
    DisplayOffTime = server.arg("DOffT"); // Display Off-Time
    InTempOffset = server.arg("DHTTo"); // DHT Temperature Sensor Offset
    InHumOffset = server.arg("DHTHo"); // DHT Humidity Sensor Offset

    // Trim Strings
    TempSSID.trim(); password.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); DisplayOnTime.trim(); DisplayOffTime.trim();
    InTempOffset.trim(); InHumOffset.trim();

    if (TempSSID != "" && password != "") { 
    Serial.println("SSID: "+TempSSID); // Wifi SSID
    Serial.println("Passwort: "+password); // Wifi SSID Passwort

    if (littleFSInit == true){
    HTMLString = "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>WiFi-Manager</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>WiFi-Manager</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Einstellungen wurden gespeichert<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    server.send(200, "text/html", HTMLString); // Captive Portal 
    SaveMatrixAdjustments(); // Eingaben im FileSystem speichern
    MyWaitLoop(500); // 0,5 sek. warten 
    Serial.println("Zugangsdaten gespeichert... "); 
    printStringWithShift("        +++       Zugangsdaten gespeichert...      +++           ",ScrollTextTime); 
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
    HTMLString += "<title>WiFi-Manager</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";    
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>WiFi-Manager</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";
    server.send(200, "text/html", HTMLString); // Captive Portal 
    printStringWithShift("        +++       Zugangsdaten wurden nicht gespeichert !     +++           ",ScrollTextTime);
    // Reset auslösen 
    printStringWithShift("        +++       Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();  
    } 
    } else
       {
    // unvollständige Einstellungen    
    HTMLString =  "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>WiFi-Manager</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"; 
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<h1>WiFi-Manager</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
    HTMLString += "<h6>Die Einstellungen sind unvollständig !<br />Einstellungen wurden nicht gespeichert</h6>";
    HTMLString += "<br/>";
    HTMLString += "<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>";
    HTMLString += "</main>";
    HTMLString += "</body>";
    HTMLString += "</html>";}
    server.send(200, "text/html", HTMLString);  // Speichern   
  } else {
    // Captive Portal Einstellungen
    int n = WiFi.scanNetworks(false, false); //WiFi.scanNetworks(async, show_hidden)
    HTMLString =  "<!doctype html><html lang='de'>";
    HTMLString += "<head><meta charset='utf-8'>";
    HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    HTMLString += "<title>WiFi-Manager</title>";
    HTMLString += "<style>";
    HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
    HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
    HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
    HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
    HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
    HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
    HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
    HTMLString += "h6 { font-size: 18px; margin-left: 110px; margin-top: 15px; margin-bottom: 5px; color: #0245b0;}";
    HTMLString += "h7 { font-size: 20px; font-weight: bold; margin-left: 110px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}";
    HTMLString += "</style>";
    HTMLString += "</head>";
    HTMLString += "<body><main class='form-signin'>";
    HTMLString += "<form action='/' method='post'>";
    HTMLString += "<h1>WiFi-Manager</h1>";
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += "<br/>";
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
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - API-Key (Optional)</label><input type='text' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - City-ID</label><input type='text' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsAPI - API-Key (Optional)</label><input type='text' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display EIN-Zeit (Beispiel: 7:30)   -Optional-</label><input type='text' class='form-control' name='DOnT' id='DOnT' value='"+DisplayOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display AUS-Zeit (Beispiel: 23:30)   -Optional-</label><input type='text' class='form-control' name='DOffT' id='DOffT' value='"+DisplayOffTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT-Sensor Temperatur Offset (Beispiel: -1 or +1)</label><input type='text' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>DHT-Sensor Luftfeuchte Offset (Beispiel: -1 or +1)</label><input type='text' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
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
    server.send(200, "text/html", HTMLString); // Captive Portal 
  }
}

// *****************************************************************************************************************************************************

void WebSiteNotFound() {
String HTMLString;   
HTMLString =  "<!doctype html><html lang='de'>";
HTMLString += "<head><meta charset='utf-8'>";
HTMLString += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
HTMLString += "<title>WiFi-Manager</title>";
HTMLString += "<style>";
HTMLString += "*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:";
HTMLString += "'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;";
HTMLString += "font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:";
HTMLString += "block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid";
HTMLString += "transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;";
HTMLString += "line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}";
HTMLString += "h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
HTMLString += "h6 { font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"; 
HTMLString += "</style>";
HTMLString += "</head>";
HTMLString += "<body><main class='form-signin'>";
HTMLString += "<h1>WiFi-Manager</h1>";
HTMLString += "<h5>("+ProgramName+")</h5>";
HTMLString += "<br/>";
HTMLString += "<h6>Die Webseite wurde nicht gefunden !</h6>";
HTMLString += "<br/><br/>";
HTMLString += "<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>";
HTMLString += "</main>";
HTMLString += "</body>";
HTMLString += "</html>";    
server.send(200, "text/html", HTMLString);   
} 

// *****************************************************************************************************************************************************
// *****************************************************************************************************************************************************
