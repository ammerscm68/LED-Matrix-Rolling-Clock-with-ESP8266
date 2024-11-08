/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Captive Portal IP-Adresse - outside of the router's own DHCP range - Default = 192.168.4.1

String ProgramName = "LED-Matrix-Uhr";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s= F("");
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// API-Key Update (per Remote)
// RemoteAPIKeyUpdate("N","pub_55347ze546784z7hw457791d3cdd5dfcf5e4",""); // N= News-Modul  /  W= Wetter-Modul  / Leer = kein Update  - APIKey
// RemoteAPIKeyUpdate("W","wuwt553dtzui64567hgerds2ab76f6a89","2929670"); // N= News-Modul  /  W= Wetter-Modul  / Leer = kein Update  - APIKey,CityID

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
if (ProgramName == "") {ProgramName = F("Hostname");} 
// Wenn gespeichert Button betätigt wurde 
if (server.method() == HTTP_POST) {
    // Einstellungen speichern
    TempSSID = server.arg("ssid"); // Wifi SSID
    password = server.arg("password"); // Wifi SSID Passwort
    FederalState = server.arg("WBl"); // Bundesland in Östereich
    weatherKey = server.arg("apikey"); // OpenWeatherMap - API-Key
    cityID = server.arg("cityid"); // OpenWeatherMap - City ID
    newsKey = server.arg("napi"); // NewsData-Key
    SecureAppToken = server.arg("posat"); // Pushover SecureAppToken
    UserToken = server.arg("pout"); // Pushover UserToken
    PushoverDevice1 = server.arg("podev1"); // Pushover Gerätename 1
    PushoverDevice2 = server.arg("podev2"); // Pushover Gerätename 2
    PushoverDevice3 = server.arg("podev3"); // Pushover Gerätename 3
    PushoverMessageSound = server.arg("poms"); // Pushover Nachrichten Sound
    
    CheckPushOver(); // Check PushOver Eingaben

    // Buzzer installiert ?
    if (server.arg("IsBuZZer") == "True") 
    {IsBuzzer = true;} else {IsBuzzer = false;} 
    
    // Trim Strings
    TempSSID.trim(); password.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); 

    if (TempSSID != "" && password != "") { 
    Serial.println("SSID: "+TempSSID); // Wifi SSID
    Serial.println("Passwort: "+String(password)); // Wifi SSID Passwort

    if (littleFSInit){
    HTMLString = F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Captive-Portal</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h3 { font-size: 16px; color: red; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
    HTMLString += F("h4 { font-size: 18px; color: red; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
    HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Die Zugangsdaten und Einstellungen wurden gespeichert<br />Die Uhr wird neu gestartet</h6>");
    if (FederalState == "" || FederalState == "-") {
    HTMLString += F("<br/>");
    HTMLString += F("<br/>");
    HTMLString += F("<br/>");
    HTMLString += F("<h4>Achtung:</h4>");
    HTMLString += F("<h3>Es werden nur die bundesweiten Feiertage angezeigt.</h3>");}
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString 
    SaveMatrixAdjustments(); // Eingaben im FileSystem speichern
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {Serial.println(F("Zugangsdaten gespeichert... "));} 
    printStringWithShift("        +++       Zugangsdaten und Einstellungen gespeichert...      +++           ",ScrollTextTime); 
    if (SerialMonitor) {Serial.println(F("Die Uhr wird neu gestartet"));} 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    // Reset auslösen
    ESP.restart();} else
     { // Zugangsdaten wurden nicht gespeichert
     if (SerialMonitor) {Serial.println(F("Zugangsdaten wurden nicht gespeichert !"));}
     HTMLString = F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Captive-Portal</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");    
    HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString 
    printStringWithShift("        +++       Zugangsdaten und Einstellungen wurden nicht gespeichert !     +++           ",ScrollTextTime);
    // Reset auslösen 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();}} else {
    // unvollständige Einstellungen    
    HTMLString =  F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Captive-Portal</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:450px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("h6 { font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Die Einstellungen sind unvollständig oder ungültig !<br />Einstellungen wurden nicht gespeichert</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
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
    HTMLString =  F("<!doctype html><html lang='de'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Captive-Portal</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("h6 { font-size: 18px; margin-left: 110px; margin-top: 15px; margin-bottom: 5px; color: #0245b0;}");
    HTMLString += F("h7 { font-size: 20px; font-weight: bold; margin-left: 110px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/' method='post'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
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
    HTMLString += F("<script>");
    HTMLString += "function SetSSID"+(String)i+"() {document.getElementById('ssid').value = document.getElementById('fssid"+(String)i+"').innerHTML;}";
    HTMLString += F("</script>");}
    } else {
    HTMLString += F("<br/>");  
    HTMLString += F("<br/>"); 
    HTMLString += F("<h6>kein WLAN gefunden !</h6>");
    HTMLString += F("<br/>");}
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password' id='password' value=''></div>");
    HTMLString += F("<br/>"); 
    // ********* Bundesländer einlesen *********
    if (IsFederalStateOK(FederalState)) {
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier Ihr Bundesland in Österreich aus:</label>");   
    HTMLString += F("<select name='WBl' id='WBl' class='form-control'>");
    if (FederalState == "" || FederalState == "-") {HTMLString += "<option selected value='-'>"+LongFederalState("-")+"</option>";} else {HTMLString += "<option value='-'>"+LongFederalState("-")+"</option>";}
    if (FederalState == "B") {HTMLString += "<option selected value='B'>"+LongFederalState("B")+"</option>";} else {HTMLString += "<option value='B'>"+LongFederalState("B")+"</option>";}
    if (FederalState == "K") {HTMLString += "<option selected value='K'>"+LongFederalState("K")+"</option>";} else {HTMLString += "<option value='K'>"+LongFederalState("K")+"</option>";}
    if (FederalState == "ST") {HTMLString += "<option selected value='ST'>"+LongFederalState("ST")+"</option>";} else {HTMLString += "<option value='ST'>"+LongFederalState("ST")+"</option>";}
    if (FederalState == "N") {HTMLString += "<option selected value='N'>"+LongFederalState("N")+"</option>";} else {HTMLString += "<option value='N'>"+LongFederalState("N")+"</option>";}
    if (FederalState == "O") {HTMLString += "<option selected value='O'>"+LongFederalState("O")+"</option>";} else {HTMLString += "<option value='O'>"+LongFederalState("O")+"</option>";}
    if (FederalState == "S") {HTMLString += "<option selected value='S'>"+LongFederalState("S")+"</option>";} else {HTMLString += "<option value='S'>"+LongFederalState("S")+"</option>";}
    if (FederalState == "T") {HTMLString += "<option selected value='T'>"+LongFederalState("T")+"</option>";} else {HTMLString += "<option value='T'>"+LongFederalState("T")+"</option>";}
    if (FederalState == "V") {HTMLString += "<option selected value='V'>"+LongFederalState("V")+"</option>";} else {HTMLString += "<option value='V'>"+LongFederalState("V")+"</option>";}
    if (FederalState == "W") {HTMLString += "<option selected value='W'>"+LongFederalState("W")+"</option>";} else {HTMLString += "<option value='W'>"+LongFederalState("W")+"</option>";}
    HTMLString += F("</select>");}
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\">+++ OpenWeatherMap und NewsData (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - API-Key</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - City-ID</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsData - API-Key</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 16px; font-weight: bold;\">+++ Pushover Nachrichtendienst (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - SecureAppToken</label><input type='text' maxlength='64' class='form-control' name='posat' id='posat' value='"+SecureAppToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - UserToken</label><input type='text' maxlength='64' class='form-control' name='pout' id='pout' value='"+UserToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 1</label><input type='text' maxlength='64' class='form-control' name='podev1' id='podev1' value='"+PushoverDevice1+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 2 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev2' id='podev2' value='"+PushoverDevice2+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 3 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev3' id='podev3' value='"+PushoverDevice3+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nachrichten Sound (https://pushover.net/api#sounds)</label><input type='text' maxlength='32' class='form-control' name='poms' id='poms' value='"+PushoverMessageSound+"'></div>";
    HTMLString += F("<br/>");
    if (IsBuzzer) {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;Ein Passiv Piezo Summer verwenden&nbsp;");
    HTMLString += F("(z.B. KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True' checked></div>");} else {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;Ein Passiv Piezo Summer verwenden&nbsp;");
    HTMLString += F("(z.B. KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True'></div>");}
    // ******************************************* Parameter ************************************************************************************************************************************  
    return HTMLString;}  

// *****************************************************************************************************************************************************

String CPRootHTMLString_3(){
    String HTMLString;
    HTMLString += F("<br/><br/>");
    HTMLString += F("<button type='submit'>Speichern</button>");
    HTMLString += F("</form>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("<form action='/' method='get'>");
    HTMLString += F("<button type='submit'>Aktualisieren</button>");
    HTMLString += F("</form>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    return HTMLString;}

// *****************************************************************************************************************************************************

void WebSiteNotFound() {
String HTMLString;   
HTMLString =  F("<!doctype html><html lang='de'>");
HTMLString += F("<head><meta charset='utf-8'>");
HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
HTMLString += F("<title>Captive-Portal</title>");
HTMLString += F("<style>");
HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:410px;padding:15px;margin:auto;}h1,p{text-align: center}");
HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
HTMLString += F("h6 { font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body><main class='form-signin'>");
HTMLString += F("<h1>Captive-Portal</h1>");
HTMLString += "<h5>("+ProgramName+")</h5>";
HTMLString += F("<br/>");
HTMLString += F("<h6>Die Webseite wurde nicht gefunden !</h6>");
HTMLString += F("<br/><br/>");
HTMLString += F("<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>");
HTMLString += F("</main>");
HTMLString += F("</body>");
HTMLString += F("</html>");    
SendHTMLString(HTMLString);} // Send HTMLString   

// *****************************************************************************************************************************************************

void RemoteAPIKeyUpdate(const char* APIModul,const char* NewKey,const char* OWMCID) {
if (APIModul != "") {  
if (littleFSInit) { 
MyWaitLoop(1000); // kurz Warten   
if (SerialMonitor) {Serial.println("");
Serial.println("****** Update API-Keys *******");}      
if (APIModul == "N") { // News-Modul 
if (NewKey != "") {
if (LittleFS.exists("/newsapi-apikey.txt")){  
LittleFS.remove("/newsapi-apikey.txt");}
if (LittleFS.exists("/newsdata-apikey.txt")){  
LittleFS.remove("/newsdata-apikey.txt");}
SetupSave("newsdata-apikey.txt", NewKey);
newsKey = NewKey;
if (SerialMonitor) {Serial.println("News API-Key: "+newsKey);}}}
if (APIModul == "W") { // Weather Modul
if (NewKey != "") {
if (LittleFS.exists("/owm-apikey.txt")){  
LittleFS.remove("/owm-apikey.txt");}
SetupSave("owm-apikey.txt", NewKey);
if (LittleFS.exists("/owm-cityid.txt")){  
LittleFS.remove("/owm-cityid.txt");}
SetupSave("owm-cityid.txt", OWMCID);
weatherKey = NewKey;
cityID = OWMCID;
if (SerialMonitor) {Serial.println("OpenWeatherMap API-Key: "+weatherKey);
Serial.println("OpenWeatherMap City-ID: "+cityID);}}}}}}

// *****************************************************************************************************************************************************
// *****************************************************************************************************************************************************
