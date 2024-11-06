/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Captive Portal IP-Adresse - Outdoor of the router's own DHCP range - Default = 192.168.4.1

String ProgramName = "Horloge-Matricielle-LED";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s= F("");
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;}

// *****************************************************************************************************************************************************

void CaptivePortal(){
// Mise à jour de la clé API (par télécommande)
// RemoteAPIKeyUpdate("N","pub_55344564354hgh455hwmb2391d3cdd5dfcf5e4","");  // N= Module Actualités  /  W= Module Météo  / Vide = Aucune mise à Jour  - APIKey
// RemoteAPIKeyUpdate("W","974432gse457zwrhujmbf590552ab76f6a89","2929670"); // N= Module Actualités  /  W= Module Météo  / Vide = Aucune mise à Jour  - APIKey,CityID

// Démarrer le portail captif (point d'accès)
WiFi.mode(WIFI_AP);
WiFi.softAPConfig(cpIP, cpIP, IPAddress(255, 255, 255, 0));   // Adresse IP du portail captif
WiFi.softAP(ProgramName, "");
// Serveur Web - Sortie si le lien n'a pas été trouvé
server.onNotFound(WebSiteNotFound);
server.on("/", handlePortal);
server.begin();}

// *****************************************************************************************************************************************************

void handlePortal() {
String HTMLString;
if (ProgramName == "") {ProgramName = F("Hostname");} 
// Lorsque le bouton enregistré a été enfoncé 
if (server.method() == HTTP_POST) {
    // Enregistrer les paramètres
    TempSSID = server.arg("ssid"); // SSID Wi-Fi
    password = server.arg("password"); // Mot de passe Wi-Fi SSID
    weatherKey = server.arg("apikey"); // OpenWeatherMap - Clé API
    cityID = server.arg("cityid"); // OpenWeatherMap - ID de la ville
    newsKey = server.arg("napi"); // NewsData-Key
    SecureAppToken = server.arg("posat"); // Pushover Jeton d'application sécurisé
    UserToken = server.arg("pout"); // Pushover Jeton utilisateur
    PushoverDevice1 = server.arg("podev1"); // Pushover Nom de l'appareil 1
    PushoverDevice2 = server.arg("podev2"); // Pushover Nom de l'appareil 2
    PushoverDevice3 = server.arg("podev3"); // Pushover Nom de l'appareil 3
    PushoverMessageSound = server.arg("poms"); // Pushover Son des messages
    
    CheckPushOver(); // Vérifier l'entrée PushOver

    // Le Buzzer est-il installé ?
    if (server.arg("IsBuZZer") == "True") 
    {IsBuzzer = true;} else {IsBuzzer = false;} 
    
    // Couper les cordes
    TempSSID.trim(); password.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); 

    if (TempSSID != "" && password != "") { 
    Serial.println("SSID: "+TempSSID); // SSID Wi-Fi
    Serial.println("Passwort: "+String(password)); // Mot de passe Wi-Fi SSID

    if (littleFSInit){
    HTMLString = F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Portail Captif</title>");
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
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les données d'accès et les Paramètres ont été Enregistrés<br />L'Horloge va Redémarrer</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    SaveMatrixAdjustments(); // Enregistrer les entrées dans le système de fichiers
    MyWaitLoop(500); // 0,5 secondes. attendez
    if (SerialMonitor) {Serial.println(F("Accéder aux données Enregistrées... "));} 
    ct = cleanText("        +++       Accéder aux données et aux Paramètres Enregistrés...      +++           ");
    printStringWithShift(ct.c_str(),ScrollTextTime); 
    if (SerialMonitor) {Serial.println(F("L'Horloge va Redémarrer"));} 
    ct = cleanText("        +++       L'Horloge va Redémarrer      +++           ");
    printStringWithShift(ct.c_str(),ScrollTextTime);
    // Réinitialiser
    ESP.restart();} else
     { // Les données d'accès n'ont pas été enregistrées
     if (SerialMonitor) {Serial.println(F("Les données d'accès n'ont pas été Enregistrées !"));}
     HTMLString = F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Portail Captif</title>");
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
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Erreur lors de l'enregistrement des Paramètres<br />L'Horloge va Redémarrer</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    ct = cleanText("        +++       Les données d'accès et les Paramètres n'ont pas été Enregistrés !     +++           ");
    printStringWithShift(ct.c_str(),ScrollTextTime);
    // Réinitialiser
    ct = cleanText("        +++       L'Horloge va Redémarrer      +++           ");
    printStringWithShift(ct.c_str(),ScrollTextTime);
    ESP.restart();}} else {
    // Paramètres incomplets   
    HTMLString =  F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Portail Captif</title>");
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
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les Paramètres sont Incomplets ou Invalides !<br />Settings were not Saved</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button class=\"button\">Retour à la Page d'accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString);} // Envoyer une chaîne HTML
  } else {
    // Captive Portal Settings
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
    SendHTMLString(""); // Envoyer une chaîne HTML
    server.sendContent(CPRootHTMLString_1()); // 1. Partie
    server.sendContent(CPRootHTMLString_2()); // 2. Partie
    server.sendContent(CPRootHTMLString_3()); // 3. Partie
    MyWaitLoop(100);
    server.client().flush();
    MyWaitLoop(100);
    server.client().stop();}}

// *****************************************************************************************************************************************************

String CPRootHTMLString_1(){
String HTMLString;  
    HTMLString =  F("<!DOCTYPE html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Portail Captif</title>");
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
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+ProgramName+")</h5>";
    HTMLString += F("<br/>");
    return HTMLString;} 

// *****************************************************************************************************************************************************

String CPRootHTMLString_2(){
String HTMLString, ssidsearch; 
int loops = 0; // pour trier les SSID 
int n = WiFi.scanNetworks(false, false); // WiFi.scanNetworks(async, show_hidden)
    if (n > 0) {
     // Tri des WLAN par RSSI
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
    // Imprimer le SSID 
    if (i == 0) {
    HTMLString += "<h7 onclick='SetSSID"+(String)i+"()' id='fssid"+(String)i+"'>" + WiFi.SSID(indices[i]) + "</h7>";} else {
    HTMLString += "<h6 onclick='SetSSID"+(String)i+"()' id='fssid"+(String)i+"'>" + WiFi.SSID(indices[i]) + "</h6>";}
    HTMLString += F("<script>");
    HTMLString += "function SetSSID"+(String)i+"() {document.getElementById('ssid').value = document.getElementById('fssid"+(String)i+"').innerHTML;}";
    HTMLString += F("</script>");}
    } else {
    HTMLString += F("<br/>");  
    HTMLString += F("<br/>"); 
    HTMLString += F("<h6>Aucun WiFi trouvé !</h6>");
    HTMLString += F("<br/>");}
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Mot de Passe</label><input type='password' class='form-control' name='password' id='password' value=''></div>");
    HTMLString += F("<br/>"); 
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\">+++ OpenWeatherMap et NewsData (Optional) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - Clé API</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - ID de la Ville</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsData - Clé API</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 15px; font-weight: bold;\">+++ Pushover Service de Messagerie (Facultatif) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Jeton d'Application Sécurisé</label><input type='text' maxlength='64' class='form-control' name='posat' id='posat' value='"+SecureAppToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Jeton Utilisateur</label><input type='text' maxlength='64' class='form-control' name='pout' id='pout' value='"+UserToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 1</label><input type='text' maxlength='64' class='form-control' name='podev1' id='podev1' value='"+PushoverDevice1+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 2 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev2' id='podev2' value='"+PushoverDevice2+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 3 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev3' id='podev3' value='"+PushoverDevice3+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Son des Messages (https://pushover.net/api#sounds)</label><input type='text' maxlength='32' class='form-control' name='poms' id='poms' value='"+PushoverMessageSound+"'></div>";
    HTMLString += F("<br/>"); 
    if (IsBuzzer) {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Utiliser un Buzzer piézo Passif&nbsp;");
    HTMLString += F("(Exemple: KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True' checked></div>");} else {
    HTMLString += F("<div class='form-floating'><br/><label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Utiliser un Buzzer piézo Passif&nbsp;");
    HTMLString += F("(Exemple: KY-006)</label><input type='checkbox' class='form-control' name='IsBuZZer' id='IsBuZZer' value='True'></div>");}
    // ******************************************* Parameter ************************************************************************************************************************************  
    return HTMLString;}  

// *****************************************************************************************************************************************************

String CPRootHTMLString_3(){
    String HTMLString;
    HTMLString += F("<br/><br/>");
    HTMLString += F("<button type='submit'>Sauvegarder</button>");
    HTMLString += F("</form>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("<form action='/' method='get'>");
    HTMLString += F("<button type='submit'>Rafraîchir</button>");
    HTMLString += F("</form>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    return HTMLString;}

// *****************************************************************************************************************************************************

void WebSiteNotFound() {
String HTMLString;   
HTMLString =  F("<!doctype html><html lang='fr'>");
HTMLString += F("<head><meta charset='utf-8'>");
HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
HTMLString += F("<title>Portail Captif</title>");
HTMLString += F("<style>");
HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:");
HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:510px;padding:15px;margin:auto;}h1,p{text-align: center}");
HTMLString += F("h5 { font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
HTMLString += F("h6 { font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body><main class='form-signin'>");
HTMLString += F("<h1>Portail Captif</h1>");
HTMLString += "<h5>("+ProgramName+")</h5>";
HTMLString += F("<br/>");
HTMLString += F("<h6>Le site n'a pas été trouvé !</h6>");
HTMLString += F("<br/><br/>");
HTMLString += F("<a href =\"/\"><button class=\"button\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</main>");
HTMLString += F("</body>");
HTMLString += F("</html>");    
SendHTMLString(HTMLString);} // Envoyer une chaîne HTML  

// *****************************************************************************************************************************************************

void RemoteAPIKeyUpdate(const char* APIModul,const char* NewKey,const char* OWMCID) {
if (APIModul != "") {  
if (littleFSInit) { 
MyWaitLoop(1000); // courte attente 
if (SerialMonitor) {Serial.println("");
Serial.println("****** Mettre à jour les clés API *******");}      
if (APIModul == "N") { // Module Actualités 
if (NewKey != "") {
if (LittleFS.exists("/newsapi-apikey.txt")){  
LittleFS.remove("/newsapi-apikey.txt");}
if (LittleFS.exists("/newsdata-apikey.txt")){  
LittleFS.remove("/newsdata-apikey.txt");}
SetupSave("newsdata-apikey.txt", NewKey);
newsKey = NewKey;
if (SerialMonitor) {Serial.println("Clé API Actualités : "+newsKey);}}}
if (APIModul == "W") { // Module Météo
if (NewKey != "") {
if (LittleFS.exists("/owm-apikey.txt")){  
LittleFS.remove("/owm-apikey.txt");}
SetupSave("owm-apikey.txt", NewKey);
if (LittleFS.exists("/owm-cityid.txt")){  
LittleFS.remove("/owm-cityid.txt");}
SetupSave("owm-cityid.txt", OWMCID);
weatherKey = NewKey;
cityID = OWMCID;
if (SerialMonitor) {Serial.println("Clé API OpenWeatherMap : "+weatherKey);
Serial.println("ID de ville OpenWeatherMap : "+cityID);}}}}}}

// *****************************************************************************************************************************************************
// *****************************************************************************************************************************************************
