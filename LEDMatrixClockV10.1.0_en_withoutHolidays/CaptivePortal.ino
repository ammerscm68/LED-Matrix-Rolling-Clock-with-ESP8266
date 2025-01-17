/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Captive Portal IP-Adresse - Outdoor of the router's own DHCP range - Default = 192.168.4.1

String ProgramName = "LED-Matrix-Clock";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s= F("");
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// API-Key Update (per Remote)
// RemoteAPIKeyUpdate("N","pub_55344564354hgh455hwmb2391d3cdd5dfcf5e4","");  // N= News-Modul  /  W= Weather-Modul  / Empty = No Update  - APIKey
// RemoteAPIKeyUpdate("W","974432gse457zwrhujmbf590552ab76f6a89","2929670"); // N= News-Modul  /  W= Weather-Modul  / Empty = No Update  - APIKey,CityID

// Start Captive Portal (Access Point)
WiFi.mode(WIFI_AP);
WiFi.softAPConfig(cpIP, cpIP, IPAddress(255, 255, 255, 0));   //Captive Portal IP-Address
WiFi.softAP(ProgramName, "");
// Web Server - Output if link was not found
server.onNotFound(WebSiteNotFound);
server.on("/", handlePortal);
server.begin();}

// *****************************************************************************************************************************************************

void handlePortal() {
String HTMLString;
if (ProgramName == "") {ProgramName = F("Hostname");} 
// When saved button was pressed 
if (server.method() == HTTP_POST) {
    // Save settings
    TempSSID = server.arg("ssid"); // Wifi SSID
    password = server.arg("password"); // Wifi SSID Passwort
    weatherKey = server.arg("apikey"); // OpenWeatherMap - API-Key
    cityID = server.arg("cityid"); // OpenWeatherMap - City ID
    newsKey = server.arg("napi"); // NewsData-Key
    GMCMapHistoryID = server.arg("gmcmhid"); // GMCMap History ID
    GMCMapGMTOffset = server.arg("gmcmgmto"); // GMCMap GMT Offset 
    SecureAppToken = server.arg("posat"); // Pushover SecureAppToken
    UserToken = server.arg("pout"); // Pushover UserToken
    PushoverDevice1 = server.arg("podev1"); // Pushover Devicename 1
    PushoverDevice2 = server.arg("podev2"); // Pushover Devicename 2
    PushoverDevice3 = server.arg("podev3"); // Pushover Devicename 3
    PushoverMessageSound = server.arg("poms"); // Pushover Message Sound
    WatchOwner1FirstName = server.arg("1bv"); // 1st Owner - First Name
    WatchOwner1LastName = server.arg("1bn"); // 1st Owner - Last Name
    Gender1 = server.arg("GS1"); // 1st Owner - Gender
    Birthday1 = server.arg("1bgb"); // 1st Owner - Date of Birth
    WatchOwner2FirstName = server.arg("2bv"); // 2nd Owner - First Name
    WatchOwner2LastName = server.arg("2bn"); // 2nd Owner - Last Name
    Gender2 = server.arg("GS2"); // 2nd Owner - Gender
    Birthday2 = server.arg("2bgb"); // 2nd Owner - Date of Birth
    WatchOwner3FirstName = server.arg("3bv"); // 3rd Owner - First Name
    WatchOwner3LastName = server.arg("3bn"); // 3rd Owner - Last Name
    Gender3 = server.arg("GS3"); // 3rd Owner - Gender
    Birthday3 = server.arg("3bgb"); // 3rd Owner - Date of Birth
    MidnightGhost = server.arg("MG1"); // Show Midnight Ghost  YES or NO
    
    CheckPushOver(); // Check PushOver Input

    // Buzzer installed ?
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
    Serial.println("SSID: "+TempSSID); // Wifi SSID
    Serial.println("Passwort: "+String(password)); // Wifi SSID Passwort

    if (littleFSInit){
    HTMLString = F("<!doctype html><html lang='en-GB'>");
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
    HTMLString += F("<h6>Access Data and Settings have been Saved<br />The Clock will Restart</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString 
    MyWaitLoop(100);
    SaveMatrixAdjustments(); // Save entries in the file system
    MyWaitLoop(500); // 0,5 sek. warten 
    if (SerialMonitor) {Serial.println(F("Access Data Saved... "));} 
    printStringWithShift("        +++       Access Data and Settings saved...      +++           ",ScrollTextTime); 
    if (SerialMonitor) {Serial.println(F("The Clock will Restart"));} 
    printStringWithShift("        +++       The Clock will Restart      +++           ",ScrollTextTime);
    // Reset
    ESP.restart();} else
     { // Access data was not saved
     if (SerialMonitor) {Serial.println(F("Access Data was not saved !"));}
     HTMLString = F("<!doctype html><html lang='en-GB'>");
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
    HTMLString += F("<h6>Error saving Settings<br />The Clock will Restart</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Send HTMLString
    MyWaitLoop(100); 
    printStringWithShift("        +++       Access Data and Settings were not saved !     +++           ",ScrollTextTime);
    // Reset
    printStringWithShift("        +++       The Clock will Restart      +++           ",ScrollTextTime);
    ESP.restart();}} else {
    // Incomplete settings   
    HTMLString =  F("<!doctype html><html lang='en-GB'>");
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
    HTMLString += F("<h6>The Settings are incomplete or invalid !<br />Settings were not Saved</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button class=\"button\">back to Homepage</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString);} // Send HTMLString 
    MyWaitLoop(100);
  } else {
    // Captive Portal Settings
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
    SendHTMLString(""); // Send HTMLString 
    MyWaitLoop(100);
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
    HTMLString =  F("<!DOCTYPE html><html lang='en-GB'>");
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
    HTMLString += "<h5>("+ProgramName+")</h5>";
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
    HTMLString += F("<h6>No WiFi found !</h6>");
    HTMLString += F("<br/>");}
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password' id='password' value=''></div>");
    HTMLString += F("<br/>"); 
    //                                  +++++++++++ Owner of the Watch +++++++++++++
    // 1st Owner
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 10px; font-weight: bold;\"> ++++++++ Owner of the Watch (Optional) ++++++++</label></div>");
    HTMLString += "<div class='form-floating'><br/><label>1st Owner - Last Name</label><input type='text' maxlength='64' class='form-control' name='1bn' id='1bn' value='"+WatchOwner1LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>1st Owner - First Name</label><input type='text' maxlength='64' class='form-control' name='1bv' id='1bv' value='"+WatchOwner1FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Select the Gender here:</label>");   
    HTMLString += F("<select name='GS1' id='GS1' class='form-control'>");
    if (Gender1 == "") {HTMLString += "<option selected value=''>Gender</option>";} else {HTMLString += "<option value=''>Gender</option>";}
    if (Gender1 == "female") {HTMLString += "<option selected value='female'>female</option>";} else {HTMLString += "<option value='female'>female</option>";}
    if (Gender1 == "male") {HTMLString += "<option selected value='male'>male</option>";} else {HTMLString += "<option value='male'>male</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>1st Owner - Date of Birth*</label><input type='text' maxlength='5' class='form-control' name='1bgb' id='1bgd' value='"+Birthday1+"'></div>";
    HTMLString += F("<br/>"); 
    // 2nd Owner
    HTMLString += "<div class='form-floating'><br/><label>2nd Owner - Last Name</label><input type='text' maxlength='64' class='form-control' name='2bn' id='2bn' value='"+WatchOwner2LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>2nd Owner - First Name</label><input type='text' maxlength='64' class='form-control' name='2bv' id='2bv' value='"+WatchOwner2FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Select the Gender here:</label>");   
    HTMLString += F("<select name='GS2' id='GS2' class='form-control'>");
    if (Gender2 == "") {HTMLString += "<option selected value=''>Gender</option>";} else {HTMLString += "<option value=''>Gender</option>";}
    if (Gender2 == "female") {HTMLString += "<option selected value='female'>female</option>";} else {HTMLString += "<option value='female'>female</option>";}
    if (Gender2 == "male") {HTMLString += "<option selected value='male'>male</option>";} else {HTMLString += "<option value='male'>male</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>2nd Owner - Date of Birth*</label><input type='text' maxlength='5' class='form-control' name='2bgb' id='2bgd' value='"+Birthday2+"'></div>";
    HTMLString += F("<br/>"); 
    // 3rd Owner
    HTMLString += "<div class='form-floating'><br/><label>3rd Owner - Last Name</label><input type='text' maxlength='64' class='form-control' name='3bn' id='3bn' value='"+WatchOwner3LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>3rd Owner - First Name</label><input type='text' maxlength='64' class='form-control' name='3bv' id='3bv' value='"+WatchOwner3FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Select the Gender here:</label>");   
    HTMLString += F("<select name='GS3' id='GS3' class='form-control'>");
    if (Gender3 == "") {HTMLString += "<option selected value=''>Gender</option>";} else {HTMLString += "<option value=''>Gender</option>";}
    if (Gender3 == "female") {HTMLString += "<option selected value='female'>female</option>";} else {HTMLString += "<option value='female'>female</option>";}
    if (Gender3 == "male") {HTMLString += "<option selected value='male'>male</option>";} else {HTMLString += "<option value='male'>male</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>3rd Owner - Date of Birth*</label><input type='text' maxlength='5' class='form-control' name='3bgb' id='3bgd' value='"+Birthday3+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 6px;\">*) Date of Birth without leading Zero and Year</label></div>");
    HTMLString += F("<div class='form-floating'><label style=\"margin-left: 18px;\">(Example: 17.3 oder 2.4 oder 5.10 oder 23.11)</label></div>");
    HTMLString += F("<br/><br/>"); 
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\">+++ OpenWeatherMap and NewsData (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - API-Key</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - City-ID</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsData - API-Key</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 30px; font-weight: bold;\">+++ Radioactivity Measurement (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - History-ID</label><input type='text' maxlength='15' class='form-control' name='gmcmhid' id='gmcmhid' value='"+GMCMapHistoryID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - GMT-Offset</label><input type='text' maxlength='3' class='form-control' name='gmcmgmto' id='gmcmgmto' value='"+GMCMapGMTOffset+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 15px; font-weight: bold;\">+++ Pushover Messaging Service (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - SecureAppToken</label><input type='text' maxlength='64' class='form-control' name='posat' id='posat' value='"+SecureAppToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - UserToken</label><input type='text' maxlength='64' class='form-control' name='pout' id='pout' value='"+UserToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Devicename 1</label><input type='text' maxlength='64' class='form-control' name='podev1' id='podev1' value='"+PushoverDevice1+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Devicename 2 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev2' id='podev2' value='"+PushoverDevice2+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Devicename 3 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev3' id='podev3' value='"+PushoverDevice3+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Message Sound (https://pushover.net/api#sounds)</label><input type='text' maxlength='32' class='form-control' name='poms' id='poms' value='"+PushoverMessageSound+"'></div>";
    HTMLString += F("<br/>"); 
    HTMLString += F("<div class='form-floating'><br/><label>Show Ghosts at Midnight ?  [YES / NO]</label>");   
    HTMLString += F("<select name='MG1' id='MG1' class='form-control'>");
    if (MidnightGhost == "YES") {HTMLString += F("<option selected value='YES'>YES</option>");} else {HTMLString += F("<option value='YES'>YES</option>");}
    if (MidnightGhost == "NO") {HTMLString += F("<option selected value='NO'>NO</option>");} else {HTMLString += F("<option value='NO'>NO</option>");}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    if (IsBuzzer) {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Use a passive piezo Buzzer&nbsp;");
    HTMLString += F("(Example: KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True' checked></div>");} else {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Use a passive piezo Buzzer&nbsp;");
    HTMLString += F("(Example: KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True'></div>");}
    // ******************************************* Parameter ************************************************************************************************************************************  
    return HTMLString;}  

// *****************************************************************************************************************************************************

String CPRootHTMLString_3(){
    String HTMLString;
    HTMLString += F("<br/><br/>");
    HTMLString += F("<button type='submit'>Save</button>");
    HTMLString += F("</form>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("<form action='/' method='get'>");
    HTMLString += F("<button type='submit'>Refresh</button>");
    HTMLString += F("</form>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    return HTMLString;}

// *****************************************************************************************************************************************************

void WebSiteNotFound() {
String HTMLString;   
HTMLString =  F("<!doctype html><html lang='en-GB'>");
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
HTMLString += F("<h6>The Site was not found !</h6>");
HTMLString += F("<br/><br/>");
HTMLString += F("<a href =\"/\"><button class=\"button\">back to Homepage</button></a>");
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
