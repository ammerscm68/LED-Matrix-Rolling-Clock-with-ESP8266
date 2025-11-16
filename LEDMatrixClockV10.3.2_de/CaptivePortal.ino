/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Captive Portal IP-Adresse - outside of the router's own DHCP range - Default = 192.168.4.1

const char* ProgramName = "LED-Matrix-Uhr";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s= F("");
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// API-Key Update (per Remote)
// RemoteAPIKeyUpdate("N","pub_5323534636453645745781d3cdd5dfcf5e4",""); // N= News-Modul  /  W= Wetter-Modul  / Leer = kein Update  - APIKey
// RemoteAPIKeyUpdate("W","94364564354365574586552ab76f6a89","2555670"); // N= News-Modul  /  W= Wetter-Modul  / Leer = kein Update  - APIKey,CityID

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
    FederalState = server.arg("WBl"); // Bundesland in Deutschland
    weatherKey = server.arg("apikey"); // OpenWeatherMap - API-Key
    cityID = server.arg("cityid"); // OpenWeatherMap - City ID
    newsKey = server.arg("napi"); // NewsData-Key
    GMCMapHistoryID = server.arg("gmcmhid"); // GMCMap History ID
    GMCMapGMTOffset = server.arg("gmcmgmto"); // GMCMap GMT Offset 
    SecureAppToken = server.arg("posat"); // Pushover SecureAppToken
    UserToken = server.arg("pout"); // Pushover UserToken
    PushoverDevice1 = server.arg("podev1"); // Pushover Gerätename 1
    PushoverDevice2 = server.arg("podev2"); // Pushover Gerätename 2
    PushoverDevice3 = server.arg("podev3"); // Pushover Gerätename 3
    PushoverMessageSound = server.arg("poms"); // Pushover Nachrichten Sound
    WatchOwner1FirstName = server.arg("1bv"); // 1. Besitzer - Vorname
    WatchOwner1LastName = server.arg("1bn"); // 1. Besitzer - Nachname
    Gender1 = server.arg("GS1"); // 1. Besitzer - Geschlecht
    Birthday1 = server.arg("1bgb"); // 1. Besitzer - Geburtsdatum
    WatchOwner2FirstName = server.arg("2bv"); // 2. Besitzer - Vorname
    WatchOwner2LastName = server.arg("2bn"); // 2. Besitzer - Nachname
    Gender2 = server.arg("GS2"); // 2. Besitzer - Geschlecht
    Birthday2 = server.arg("2bgb"); // 2. Besitzer - Geburtsdatum
    WatchOwner3FirstName = server.arg("3bv"); // 3. Besitzer - Vorname
    WatchOwner3LastName = server.arg("3bn"); // 3. Besitzer - Nachname
    Gender3 = server.arg("GS3"); // 3. Besitzer - Geschlecht
    Birthday3 = server.arg("3bgb"); // 3. Besitzer - Geburtsdatum
    MidnightGhost = server.arg("MG1"); // Mitternachsgeist anzeigen JA oder NEIN
    DHTSensor = server.arg("DHT"); // Auswahl DHT-Sensor [DHT11 oder DHT22]
    MCTZ = server.arg("TZ1"); // Gewählte Zeitzone
    NTPSN = server.arg("NTPS"); NTPSN.trim(); // NTPServer-Name
    if (NTPSN != "") {ntpServerName = NTPSN.c_str();} else {ntpServerName = "de.pool.ntp.org";}

    CheckPushOver(); // Check PushOver Eingaben

    // Buzzer installiert ?
    if (server.arg("IsBuZZer") == "True") 
    {IsBuzzer = true;} else {IsBuzzer = false;} 
    
    // Trim Strings
    TempSSID.trim(); password.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); WatchOwner1FirstName.trim(); WatchOwner1LastName.trim();
    Gender1.trim(); Birthday1.trim(); WatchOwner2FirstName.trim(); 
    WatchOwner2LastName.trim(); Gender2.trim(); Birthday2.trim();
    WatchOwner3FirstName.trim(); WatchOwner3LastName.trim();
    Gender3.trim(); Birthday3.trim();

    if (TempSSID != "" && password != "") { 
    DEBUG_PRINTLN("SSID: "+TempSSID); // Wifi SSID
    DEBUG_PRINTLN("Passwort: "+String(password)); // Wifi SSID Passwort

    if (littleFSInit){
    HTMLString = CP_HTMLStringPart1();
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
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
    MyWaitLoop(100);
    SaveMatrixAdjustments(); // Eingaben im FileSystem speichern
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {DEBUG_PRINTLN(F("Zugangsdaten gespeichert... "));} 
    printStringWithShift("        +++       Zugangsdaten und Einstellungen gespeichert...      +++           ",ScrollTextTime); 
    if (SerialMonitor) {DEBUG_PRINTLN(F("Die Uhr wird neu gestartet"));} 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    // Reset auslösen
    ESP.restart();} else
     { // Zugangsdaten wurden nicht gespeichert
     if (SerialMonitor) {DEBUG_PRINTLN(F("Zugangsdaten wurden nicht gespeichert !"));}
    HTMLString = CP_HTMLStringPart1(); 
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Fehler beim Speichern der Einstellungen<br />Die Uhr wird neu gestartet</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString 
    MyWaitLoop(100);
    printStringWithShift("        +++       Zugangsdaten und Einstellungen wurden nicht gespeichert !     +++           ",ScrollTextTime);
    // Reset auslösen 
    printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();}} else {
    // unvollständige Einstellungen    
    HTMLString = CP_HTMLStringPart1();
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Die Einstellungen sind unvollständig oder ungültig !<br />Einstellungen wurden nicht gespeichert</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString 
    MyWaitLoop(100);}
  } else {
    // Captive Portal Einstellungen
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
    SendHTMLString(""); // Send HTMLString 
    MyWaitLoop(100);
    server.sendContent(CPRootHTMLString_1()); // 1. Teil
    server.sendContent(CPRootHTMLString_2()); // 2. Teil
    server.sendContent(CPRootHTMLString_3()); // 3. Teil
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
    HTMLString += F("h6 { font-size: 18px; margin-left: 100px; margin-top: 15px; margin-bottom: 5px; color: #0245b0;}");
    HTMLString += F("h7 { font-size: 20px; font-weight: bold; margin-left: 100px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/' method='post'>");
    HTMLString += F("<h1>Captive-Portal</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    return HTMLString;} 

// *****************************************************************************************************************************************************

String CPRootHTMLString_2(){
String HTMLString, ssidsearch; 
int loops = 0; // for Sort SSID's 
int n = WiFi.scanNetworks(false, true); //WiFi.scanNetworks(async, show_hidden)
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
    HTMLString += F("<h9>kein WLAN gefunden !</h9>");
    HTMLString += F("<br/>");}
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password' id='password' value=''></div>");
    HTMLString += F("<br/>");
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><label>NTP-Serverpool (z.Bsp. de.pool.ntp.org)</label><input type='text' class='form-control' name='NTPS' id='NTPS' value='"+NTPSN+"'></div>";
    HTMLString += F("<br/>"); 
    // *********************************** Zeitzone wählen **************************************************************************************************************
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier Ihre Zeitzone aus:</label>");   
    HTMLString += F("<select name='TZ1' id='TZ1' class='form-control'>");
    if (MCTZ == "Berlin, Frankfurt, Wien, Paris, Madrid") {HTMLString += "<option selected value='Berlin, Frankfurt, Wien, Paris, Madrid'>Berlin, Frankfurt, Wien, Paris, Madrid</option>";} else {HTMLString += "<option value='Berlin, Frankfurt, Wien, Paris, Madrid'>Berlin, Frankfurt, Wien, Paris, Madrid</option>";}
    if (MCTZ == "London, Belfast, Dublin") {HTMLString += "<option selected value='London, Belfast, Dublin'>London, Belfast, Dublin</option>";} else {HTMLString += "<option value='London, Belfast, Dublin'>London, Belfast, Dublin</option>";}
    if (MCTZ == "New York, Detroit, Miami, Lima") {HTMLString += "<option selected value='New York, Detroit, Miami, Lima'>New York, Detroit, Miami, Lima</option>";} else {HTMLString += "<option value='New York, Detroit, Miami, Lima'>New York, Detroit, Miami, Lima</option>";}
    if (MCTZ == "Chicago, Houston, Mexico City") {HTMLString += "<option selected value='Chicago, Houston, Mexico City'>Chicago, Houston, Mexico City</option>";} else {HTMLString += "<option value='Chicago, Houston, Mexico City'>Chicago, Houston, Mexico City</option>";}
    if (MCTZ == "Denver, Salt Lake City, Calgary") {HTMLString += "<option selected value='Denver, Salt Lake City, Calgary'>Denver, Salt Lake City, Calgary</option>";} else {HTMLString += "<option value='Denver, Salt Lake City, Calgary'>Denver, Salt Lake City, Calgary</option>";}
    if (MCTZ == "Arizona") {HTMLString += "<option selected value='Arizona'>Arizona</option>";} else {HTMLString += "<option value='Arizona'>Arizona</option>";}
    if (MCTZ == "Las Vegas, Los Angeles, Seattle") {HTMLString += "<option selected value='Las Vegas, Los Angeles, Seattle'>Las Vegas, Los Angeles, Seattle</option>";} else {HTMLString += "<option value='Las Vegas, Los Angeles, Seattle'>Las Vegas, Los Angeles, Seattle</option>";}
    if (MCTZ == "Sydney, Melbourne") {HTMLString += "<option selected value='Sydney, Melbourne'>Sydney, Melbourne</option>";} else {HTMLString += "<option value='Sydney, Melbourne'>Sydney, Melbourne</option>";}
    if (MCTZ == "Moskau") {HTMLString += "<option selected value='Moskau'>Moskau</option>";} else {HTMLString += "<option value='Moskau'>Moskau</option>";}
    if (MCTZ == "Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary") {HTMLString += "<option selected value='Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary'>Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary</option>";} else {HTMLString += "<option value='Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary'>Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary</option>";}
    if (MCTZ == "Vancouver") {HTMLString += "<option selected value='Vancouver'>Vancouver</option>";} else {HTMLString += "<option value='Vancouver'>Vancouver</option>";}
    if (MCTZ == "Winnipeg, Baker Lake, Kenora, Regina, Creighton") {HTMLString += "<option selected value='Winnipeg, Baker Lake, Kenora, Regina, Creighton'>Winnipeg, Baker Lake, Kenora, Regina, Creighton</option>";} else {HTMLString += "<option value='Winnipeg, Baker Lake, Kenora, Regina, Creighton'>Winnipeg, Baker Lake, Kenora, Regina, Creighton</option>";}
    if (MCTZ == "Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules") {HTMLString += "<option selected value='Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules'>Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules</option>";} else {HTMLString += "<option value='Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules'>Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules</option>";}
    if (MCTZ == "Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa") {HTMLString += "<option selected value='Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa'>Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa</option>";} else {HTMLString += "<option value='Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa'>Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa</option>";}
    if (MCTZ == "Whitehorse") {HTMLString += "<option selected value='Whitehorse'>Whitehorse</option>";} else {HTMLString += "<option value='Whitehorse'>Whitehorse</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += F("<br/>"); 
    //                                  +++++++++++ Besitzer der Uhr +++++++++++++
    // Hauptbesitzer
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\"> ++++++++ Besitzer der Uhr (Optional) ++++++++</label></div>");
    HTMLString += "<div class='form-floating'><br/><label>1. Besitzer - Nachname</label><input type='text' maxlength='64' class='form-control' name='1bn' id='1bn' value='"+WatchOwner1LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>1. Besitzer - Vorname</label><input type='text' maxlength='64' class='form-control' name='1bv' id='1bv' value='"+WatchOwner1FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier das Geschlecht aus:</label>");   
    HTMLString += F("<select name='GS1' id='GS1' class='form-control'>");
    if (Gender1 == "") {HTMLString += "<option selected value=''>Geschlecht</option>";} else {HTMLString += "<option value=''>Geschlecht</option>";}
    if (Gender1 == "female") {HTMLString += "<option selected value='female'>weiblich</option>";} else {HTMLString += "<option value='female'>weiblich</option>";}
    if (Gender1 == "male") {HTMLString += "<option selected value='male'>männlich</option>";} else {HTMLString += "<option value='male'>männlich</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>1. Besitzer - Geburtsdatum*</label><input type='text' maxlength='5' class='form-control' name='1bgb' id='1bgd' value='"+Birthday1+"'></div>";
    HTMLString += F("<br/>"); 
    // 2. Besitzer
    HTMLString += "<div class='form-floating'><br/><label>2. Besitzer - Nachname</label><input type='text' maxlength='64' class='form-control' name='2bn' id='2bn' value='"+WatchOwner2LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>2. Besitzer - Vorname</label><input type='text' maxlength='64' class='form-control' name='2bv' id='2bv' value='"+WatchOwner2FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier das Geschlecht aus:</label>");   
    HTMLString += F("<select name='GS2' id='GS2' class='form-control'>");
    if (Gender2 == "") {HTMLString += "<option selected value=''>Geschlecht</option>";} else {HTMLString += "<option value=''>Geschlecht</option>";}
    if (Gender2 == "female") {HTMLString += "<option selected value='female'>weiblich</option>";} else {HTMLString += "<option value='female'>weiblich</option>";}
    if (Gender2 == "male") {HTMLString += "<option selected value='male'>männlich</option>";} else {HTMLString += "<option value='male'>männlich</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>2. Besitzer - Geburtsdatum*</label><input type='text' maxlength='5' class='form-control' name='2bgb' id='2bgd' value='"+Birthday2+"'></div>";
    HTMLString += F("<br/>"); 
    // 3. Besitzer
    HTMLString += "<div class='form-floating'><br/><label>3. Besitzer - Nachname</label><input type='text' maxlength='64' class='form-control' name='3bn' id='3bn' value='"+WatchOwner3LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>3. Besitzer - Vorname</label><input type='text' maxlength='64' class='form-control' name='3bv' id='3bv' value='"+WatchOwner3FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier das Geschlecht aus:</label>");   
    HTMLString += F("<select name='GS3' id='GS3' class='form-control'>");
    if (Gender3 == "") {HTMLString += "<option selected value=''>Geschlecht</option>";} else {HTMLString += "<option value=''>Geschlecht</option>";}
    if (Gender3 == "female") {HTMLString += "<option selected value='female'>weiblich</option>";} else {HTMLString += "<option value='female'>weiblich</option>";}
    if (Gender3 == "male") {HTMLString += "<option selected value='male'>männlich</option>";} else {HTMLString += "<option value='male'>männlich</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>3. Besitzer - Geburtsdatum*</label><input type='text' maxlength='5' class='form-control' name='3bgb' id='3bgd' value='"+Birthday3+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 6px;\">*) Geburtsdatum ohne führende Null und Jahr</label></div>");
    HTMLString += F("<div class='form-floating'><label style=\"margin-left: 24px;\">(z.Bsp. 17.3 oder 2.4 oder 5.10 oder 23.11)</label></div>");
    HTMLString += F("<br/><br/>"); 
    // ********* Bundesländer einlesen *********
    if (IsFederalStateOK(FederalState)) {
    HTMLString += F("<div class='form-floating'><br/><label>Wählen Sie hier Ihr Bundesland in Deutschland aus:</label>");   
    HTMLString += F("<select name='WBl' id='WBl' class='form-control'>");
    if (FederalState == "" || FederalState == "-") {HTMLString += "<option selected value='-'>"+LongFederalState("-")+"</option>";} else {HTMLString += "<option value='-'>"+LongFederalState("-")+"</option>";}
    if (FederalState == "BW") {HTMLString += "<option selected value='BW'>"+LongFederalState("BW")+"</option>";} else {HTMLString += "<option value='BW'>"+LongFederalState("BW")+"</option>";}
    if (FederalState == "BY") {HTMLString += "<option selected value='BY'>"+LongFederalState("BY")+"</option>";} else {HTMLString += "<option value='BY'>"+LongFederalState("BY")+"</option>";}
    if (FederalState == "BE") {HTMLString += "<option selected value='BE'>"+LongFederalState("BE")+"</option>";} else {HTMLString += "<option value='BE'>"+LongFederalState("BE")+"</option>";}
    if (FederalState == "BB") {HTMLString += "<option selected value='BB'>"+LongFederalState("BB")+"</option>";} else {HTMLString += "<option value='BB'>"+LongFederalState("BB")+"</option>";}
    if (FederalState == "HB") {HTMLString += "<option selected value='HB'>"+LongFederalState("HB")+"</option>";} else {HTMLString += "<option value='HB'>"+LongFederalState("HB")+"</option>";}
    if (FederalState == "HH") {HTMLString += "<option selected value='HH'>"+LongFederalState("HH")+"</option>";} else {HTMLString += "<option value='HH'>"+LongFederalState("HH")+"</option>";}
    if (FederalState == "HE") {HTMLString += "<option selected value='HE'>"+LongFederalState("HE")+"</option>";} else {HTMLString += "<option value='HE'>"+LongFederalState("HE")+"</option>";}
    if (FederalState == "MV") {HTMLString += "<option selected value='MV'>"+LongFederalState("MV")+"</option>";} else {HTMLString += "<option value='MV'>"+LongFederalState("MV")+"</option>";}
    if (FederalState == "NI") {HTMLString += "<option selected value='NI'>"+LongFederalState("NI")+"</option>";} else {HTMLString += "<option value='NI'>"+LongFederalState("NI")+"</option>";}
    if (FederalState == "NRW") {HTMLString += "<option selected value='NRW'>"+LongFederalState("NRW")+"</option>";} else {HTMLString += "<option value='NRW'>"+LongFederalState("NRW")+"</option>";}
    if (FederalState == "RP") {HTMLString += "<option selected value='RP'>"+LongFederalState("RP")+"</option>";} else {HTMLString += "<option value='RP'>"+LongFederalState("RP")+"</option>";}
    if (FederalState == "SL") {HTMLString += "<option selected value='SL'>"+LongFederalState("SL")+"</option>";} else {HTMLString += "<option value='SL'>"+LongFederalState("SL")+"</option>";}
    if (FederalState == "SN") {HTMLString += "<option selected value='SN'>"+LongFederalState("SN")+"</option>";} else {HTMLString += "<option value='SN'>"+LongFederalState("SN")+"</option>";}
    if (FederalState == "SA") {HTMLString += "<option selected value='SA'>"+LongFederalState("SA")+"</option>";} else {HTMLString += "<option value='SA'>"+LongFederalState("SA")+"</option>";}
    if (FederalState == "SH") {HTMLString += "<option selected value='SH'>"+LongFederalState("SH")+"</option>";} else {HTMLString += "<option value='SH'>"+LongFederalState("SH")+"</option>";}
    if (FederalState == "TH") {HTMLString += "<option selected value='TH'>"+LongFederalState("TH")+"</option>";} else {HTMLString += "<option value='TH'>"+LongFederalState("TH")+"</option>";}
    HTMLString += F("</select>");}
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\">+++ OpenWeatherMap und NewsData (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap.org - API-Key</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap.org - City-ID</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsData.io - API-Key</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 36px; font-weight: bold;\">+++ Radioaktivitätsmessung (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - History-ID</label><input type='text' maxlength='15' class='form-control' name='gmcmhid' id='gmcmhid' value='"+GMCMapHistoryID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - GMT-Offset</label><input type='text' maxlength='3' class='form-control' name='gmcmgmto' id='gmcmgmto' value='"+GMCMapGMTOffset+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 16px; font-weight: bold;\">+++ Pushover Nachrichtendienst (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - SecureAppToken</label><input type='text' maxlength='64' class='form-control' name='posat' id='posat' value='"+SecureAppToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - UserToken</label><input type='text' maxlength='64' class='form-control' name='pout' id='pout' value='"+UserToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 1</label><input type='text' maxlength='64' class='form-control' name='podev1' id='podev1' value='"+PushoverDevice1+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 2 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev2' id='podev2' value='"+PushoverDevice2+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Gerätename 3 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev3' id='podev3' value='"+PushoverDevice3+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nachrichten Sound (https://pushover.net/api#sounds)</label><input type='text' maxlength='32' class='form-control' name='poms' id='poms' value='"+PushoverMessageSound+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><br/><label>Geister um Mitternacht anzeigen ? [JA / NEIN]</label>");   
    HTMLString += F("<select name='MG1' id='MG1' class='form-control'>");
    if (MidnightGhost == "YES") {HTMLString += F("<option selected value='YES'>JA</option>");} else {HTMLString += F("<option value='YES'>JA</option>");}
    if (MidnightGhost == "NO") {HTMLString += F("<option selected value='NO'>NEIN</option>");} else {HTMLString += F("<option value='NO'>NEIN</option>");}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><br/><label>Temperatur- und Luftfeuchtesensor ?</label>"); 
    HTMLString += F("<select name='DHT' id='DHT' class='form-control'>");
    if (DHTSensor == "DHT11") {HTMLString += F("<option selected value='DHT11'>DHT11</option>");} else {HTMLString += F("<option value='DHT11'>DHT11</option>");}
    if (DHTSensor == "DHT22") {HTMLString += F("<option selected value='DHT22'>DHT22</option>");} else {HTMLString += F("<option value='DHT22'>DHT22</option>");}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
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
HTMLString = CP_HTMLStringPart1();
HTMLString += F("<body><main class='form-signin'>");
HTMLString += F("<h1>Captive-Portal</h1>");
HTMLString += "<h5>("+String(ProgramName)+")</h5>";
HTMLString += F("<br/>");
HTMLString += F("<h8>&nbsp;&nbsp;&nbsp;Die Webseite wurde nicht gefunden !</h8>");
HTMLString += F("<br/><br/>");
HTMLString += F("<a href =\"/\"><button class=\"button\">zurück zur Startseite</button></a>");
HTMLString += F("</main>");
HTMLString += F("</body>");
HTMLString += F("</html>");    
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);}

// *****************************************************************************************************************************************************

void RemoteAPIKeyUpdate(const char* APIModul,const char* NewKey,const char* OWMCID) {
if (APIModul != "") {  
if (littleFSInit) { 
MyWaitLoop(1000); // kurz Warten
if (SerialMonitor) {DEBUG_PRINTLN("");
DEBUG_PRINTLN("****** Update API-Keys *******");}      
if (APIModul == "N") { // News-Modul 
if (NewKey != "") {
if (LittleFS.exists("/newsapi-apikey.txt")){  
LittleFS.remove("/newsapi-apikey.txt");}
if (LittleFS.exists("/newsdata-apikey.txt")){  
LittleFS.remove("/newsdata-apikey.txt");}
SetupSave("newsdata-apikey.txt", NewKey);
newsKey = NewKey;
if (SerialMonitor) {DEBUG_PRINTLN("News API-Key: "+newsKey);}}}
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
if (SerialMonitor) {DEBUG_PRINTLN("OpenWeatherMap API-Key: "+weatherKey);
DEBUG_PRINTLN("OpenWeatherMap City-ID: "+cityID);}}}}}}

// *****************************************************************************************************************************************************

String CP_HTMLStringPart1() {
String HTMLString;
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
HTMLString += F("h7 { font-size: 20px; font-weight: bold; margin-left: 100px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}");
HTMLString += F("h8 { font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
return HTMLString;}

// ************************************************************************************************************************************************