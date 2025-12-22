/* ***************** Captive Portal *********************/
IPAddress cpIP(APIPA1, APIPA2, APIPA3, APIPA4);  // Adresse IP du Portail Captif - En dehors de la plage DHCP du Routeur - Par défaut = 192.168.4.1

const char* ProgramName = "Horloge-Matricielle-LED";

// *****************************************************************************************************************************************************

String ipToString(IPAddress ip){
  String s;
  for (byte i=0; i<4; i++)
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
String HTMLString; HTMLString.reserve(220);
if (ProgramName == "") {ProgramName = "Hostname";} 
// Lorsque le bouton enregistré a été enfoncé 
if (server.method() == HTTP_POST) {
    // Enregistrer les paramètres
    TempSSID = server.arg("ssid"); // SSID Wi-Fi
    password = server.arg("password"); // Mot de passe Wi-Fi SSID
    String checkpasswd =  server.arg(F("checkpasswd")); // Wifi SSID Confirmation du mot de passe
    weatherKey = server.arg("apikey"); // OpenWeatherMap - Clé API
    cityID = server.arg("cityid"); // OpenWeatherMap - ID de la ville
    newsKey = server.arg("napi"); // NewsData-Key
    GMCMapHistoryID = server.arg("gmcmhid"); // GMCMap Identifiant de l'Historique
    GMCMapGMTOffset = server.arg("gmcmgmto"); // GMCMap Décalage GMT
    SecureAppToken = server.arg("posat"); // Pushover Jeton d'application sécurisé
    UserToken = server.arg("pout"); // Pushover Jeton utilisateur
    PushoverDevice1 = server.arg("podev1"); // Pushover Nom de l'appareil 1
    PushoverDevice2 = server.arg("podev2"); // Pushover Nom de l'appareil 2
    PushoverDevice3 = server.arg("podev3"); // Pushover Nom de l'appareil 3
    PushoverMessageSound = server.arg("poms"); // Pushover Son des messages
    WatchOwner1FirstName = server.arg("1bv"); // 1er propriétaire - Prénom
    WatchOwner1LastName = server.arg("1bn"); // 1er propriétaire - Nom de famille
    Gender1 = server.arg("GS1"); // 1er propriétaire - Sexe
    Birthday1 = server.arg("1bgb"); // 1er propriétaire - Date de naissance
    WatchOwner2FirstName = server.arg("2bv"); // 2ème propriétaire - Prénom
    WatchOwner2LastName = server.arg("2bn"); // 2e propriétaire - Nom de famille
    Gender2 = server.arg("GS2"); // 2ème propriétaire - Sexe
    Birthday2 = server.arg("2bgb"); // 2ème propriétaire - Date de naissance
    WatchOwner3FirstName = server.arg("3bv"); // 3ème propriétaire - Prénom
    WatchOwner3LastName = server.arg("3bn"); // 3ème propriétaire - Nom de famille
    Gender3 = server.arg("GS3"); // 3ème propriétaire - Sexe
    Birthday3 = server.arg("3bgb"); // 3ème propriétaire - Date de naissance
    MidnightGhost = server.arg("MG1"); // Afficher le Fantôme de Minuit -  OUI ou NON
    DHTSensor = server.arg("DHT"); // Sélection du Capteur DHT [DHT11, DHT22 ou NON]
    MCTZ = server.arg("TZ1"); // Fuseau Horaire sélectionné
    NTPSN = server.arg("NTPS"); NTPSN.trim(); // Nom du Serveur NTP
    if (NTPSN != "") {ntpServerName = NTPSN.c_str();} else {ntpServerName = "fr.pool.ntp.org";}
    
    CheckPushOver(); // Vérifier l'entrée PushOver

    // Le Buzzer est-il installé ?
    if (server.arg("IsBuZZer") == "True") 
    {IsBuzzer = true;} else {IsBuzzer = false;} 
    
    // Couper les cordes
    TempSSID.trim(); password.trim(); checkpasswd.trim(); weatherKey.trim(); cityID.trim();
    newsKey.trim(); WatchOwner1FirstName.trim(); WatchOwner1LastName.trim();
    Gender1.trim(); Birthday1.trim(); WatchOwner2FirstName.trim(); 
    WatchOwner2LastName.trim(); Gender2.trim(); Birthday2.trim();
    WatchOwner3FirstName.trim(); WatchOwner3LastName.trim();
    Gender3.trim(); Birthday3.trim();

    if (TempSSID != "" && password != "" && checkpasswd != "" && password.equals(checkpasswd)) { 
    DEBUG_PRINTLN("SSID: "+TempSSID); // SSID Wi-Fi
    DEBUG_PRINTLN("Passwort: "+String(password)); // Mot de passe Wi-Fi SSID

    if (littleFSInit){
    HTMLString = CP_HTMLStringPart1();
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les données d'accès et les Paramètres ont été Enregistrés<br />L'Horloge va Redémarrer</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    MyWaitLoop(100);
    SaveMatrixAdjustments(); // Enregistrer les entrées dans le système de fichiers
    MyWaitLoop(500); // 0,5 secondes. attendez
    if (SerialMonitor) {DEBUG_PRINTLN(F("Accéder aux données Enregistrées... "));} 
    printStringWithShift(cleanText("        +++       Accéder aux données et aux Paramètres Enregistrés...      +++           ").c_str(),ScrollTextTime); 
    if (SerialMonitor) {DEBUG_PRINTLN(F("L'Horloge va Redémarrer"));} 
    printStringWithShift(cleanText("        +++       L'Horloge va Redémarrer      +++           ").c_str(),ScrollTextTime);
    // Réinitialiser
    ESP.restart();} else
     { // Les données d'accès n'ont pas été enregistrées
     if (SerialMonitor) {DEBUG_PRINTLN(F("Les données d'accès n'ont pas été Enregistrées !"));}
    HTMLString = CP_HTMLStringPart1();
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Erreur lors de l'enregistrement des Paramètres<br />L'Horloge va Redémarrer</h6>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    MyWaitLoop(100);
    printStringWithShift(cleanText("        +++       Les données d'accès et les Paramètres n'ont pas été Enregistrés !     +++           ").c_str(),ScrollTextTime);
    // Réinitialiser
    printStringWithShift(cleanText("        +++       L'Horloge va Redémarrer      +++           ").c_str(),ScrollTextTime);
    ESP.restart();}} else {
    // Paramètres incomplets   
    HTMLString = CP_HTMLStringPart1();
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les Paramètres sont Incomplets ou Invalides !<br />Settings were not Saved</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button class=\"button\">Retour à la Page d'accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);}
  } else {
    // Paramètres du Portail Captif
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
    SendHTMLString(""); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    server.sendContent(CPRootHTMLString_1()); // 1. Partie
    server.sendContent(CPRootHTMLString_2()); // 2. Partie
    server.sendContent(CPRootHTMLString_3()); // 3. Partie
    MyWaitLoop(100);
    server.client().stop();}}

// *****************************************************************************************************************************************************

String CPRootHTMLString_1(){
String HTMLString; HTMLString.reserve(220);  
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
    HTMLString += F("h5 {font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("h6 {font-size: 18px; margin-left: 100px; margin-top: 15px; margin-bottom: 5px; color: #0245b0;}");
    HTMLString += F("h7 {font-size: 20px; font-weight: bold; margin-left: 100px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/' method='post'>");
    HTMLString += F("<h1>Portail Captif</h1>");
    HTMLString += "<h5>("+String(ProgramName)+")</h5>";
    HTMLString += F("<br/>");
    return HTMLString;} 

// *****************************************************************************************************************************************************

String CPRootHTMLString_2(){
String HTMLString, ssidsearch; 
HTMLString.reserve(220);
int16_t loops = 0; // pour trier les SSID 
int16_t n = WiFi.scanNetworks(false, true); //WiFi.scanNetworks(async, show_hidden)
    if (n > 0) {
     // Tri des WLAN par RSSI
    int16_t indices[n];
    int16_t skip[n];
    int16_t o = n;
    for (int16_t i = 0; i < n; i++) {
    indices[i] = i;}
    for (int16_t i = 0; i < n; i++) {
        for (int16_t j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            loops++;
            //int16_t temp = indices[j];
            //indices[j] = indices[i];
            //indices[i] = temp;
            std::swap(indices[i], indices[j]);
            std::swap(skip[i], skip[j]);}}}
      for (int16_t i = 0; i < n; i++) {
        if(indices[i] == -1){
          --o;
          continue;}
        ssidsearch = WiFi.SSID(indices[i]);
        for (int16_t j = i + 1; j < n; j++) {
          loops++;
          if (ssidsearch == WiFi.SSID(indices[j])) {
            indices[j] = -1;}}}
    for (int16_t i = 0; i < n; ++i){
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
    HTMLString += F("<h6>Aucun WiFi trouvé !</h6>");
    HTMLString += F("<br/>");}
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid' id='ssid' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Mot de Passe</label><input type='password' class='form-control' name='password' id='password' value=''></div>");
    HTMLString += F("<div class='form-floating'><br/><label>Confirmez le Mot de Passe</label><input type='password' class='form-control' name='checkpasswd' id='checkpasswd' value=''></div>");
    HTMLString += F("<br/>"); 
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><label>Pool de Serveurs NTP (Exemple: fr.pool.ntp.org)</label><input type='text' class='form-control' name='NTPS' id='NTPS' value='"+NTPSN+"'></div>";
    HTMLString += F("<br/>"); 
    // *********************************** Select Time Zone **************************************************************************************************************
    HTMLString += F("<div class='form-floating'><br/><label>Sélectionnez votre fuseau Horaire ici :</label>");   
    HTMLString += F("<select name='TZ1' id='TZ1' class='form-control'>");
    if (MCTZ == "Berlin, Frankfurt, Vienna, Paris, Madrid") {HTMLString += "<option selected value='Berlin, Frankfurt, Vienna, Paris, Madrid'>Berlin, Frankfurt, Vienna, Paris, Madrid</option>";} else {HTMLString += "<option value='Berlin, Frankfurt, Vienna, Paris, Madrid'>Berlin, Frankfurt, Vienna, Paris, Madrid</option>";}
    if (MCTZ == "London, Belfast, Dublin") {HTMLString += "<option selected value='London, Belfast, Dublin'>London, Belfast, Dublin</option>";} else {HTMLString += "<option value='London, Belfast, Dublin'>London, Belfast, Dublin</option>";}
    if (MCTZ == "New York, Detroit, Miami, Lima") {HTMLString += "<option selected value='New York, Detroit, Miami, Lima'>New York, Detroit, Miami, Lima</option>";} else {HTMLString += "<option value='New York, Detroit, Miami, Lima'>New York, Detroit, Miami, Lima</option>";}
    if (MCTZ == "Chicago, Houston, Mexico City") {HTMLString += "<option selected value='Chicago, Houston, Mexico City'>Chicago, Houston, Mexico City</option>";} else {HTMLString += "<option value='Chicago, Houston, Mexico City'>Chicago, Houston, Mexico City</option>";}
    if (MCTZ == "Denver, Salt Lake City, Calgary") {HTMLString += "<option selected value='Denver, Salt Lake City, Calgary'>Denver, Salt Lake City, Calgary</option>";} else {HTMLString += "<option value='Denver, Salt Lake City, Calgary'>Denver, Salt Lake City, Calgary</option>";}
    if (MCTZ == "Arizona") {HTMLString += "<option selected value='Arizona'>Arizona</option>";} else {HTMLString += "<option value='Arizona'>Arizona</option>";}
    if (MCTZ == "Las Vegas, Los Angeles, Seattle") {HTMLString += "<option selected value='Las Vegas, Los Angeles, Seattle'>Las Vegas, Los Angeles, Seattle</option>";} else {HTMLString += "<option value='Las Vegas, Los Angeles, Seattle'>Las Vegas, Los Angeles, Seattle</option>";}
    if (MCTZ == "Sydney, Melbourne") {HTMLString += "<option selected value='Sydney, Melbourne'>Sydney, Melbourne</option>";} else {HTMLString += "<option value='Sydney, Melbourne'>Sydney, Melbourne</option>";}
    if (MCTZ == "Moscow") {HTMLString += "<option selected value='Moscow'>Moscow</option>";} else {HTMLString += "<option value='Moscow'>Moscow</option>";}
    if (MCTZ == "Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary") {HTMLString += "<option selected value='Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary'>Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary</option>";} else {HTMLString += "<option value='Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary'>Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary</option>";}
    if (MCTZ == "Vancouver") {HTMLString += "<option selected value='Vancouver'>Vancouver</option>";} else {HTMLString += "<option value='Vancouver'>Vancouver</option>";}
    if (MCTZ == "Winnipeg, Baker Lake, Kenora, Regina, Creighton") {HTMLString += "<option selected value='Winnipeg, Baker Lake, Kenora, Regina, Creighton'>Winnipeg, Baker Lake, Kenora, Regina, Creighton</option>";} else {HTMLString += "<option value='Winnipeg, Baker Lake, Kenora, Regina, Creighton'>Winnipeg, Baker Lake, Kenora, Regina, Creighton</option>";}
    if (MCTZ == "Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules") {HTMLString += "<option selected value='Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules'>Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules</option>";} else {HTMLString += "<option value='Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules'>Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules</option>";}
    if (MCTZ == "Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa") {HTMLString += "<option selected value='Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa'>Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa</option>";} else {HTMLString += "<option value='Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa'>Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa</option>";}
    if (MCTZ == "Whitehorse") {HTMLString += "<option selected value='Whitehorse'>Whitehorse</option>";} else {HTMLString += "<option value='Whitehorse'>Whitehorse</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    //                                  +++++++++++ Propriétaire de l'Horloge +++++++++++++
    // 1er Propriétaire
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 10px; font-weight: bold;\"> +++ Propriétaire de l'Horloge (Facultatif) +++</label></div>");
    HTMLString += "<div class='form-floating'><br/><label>1er Propriétaire - Nom de Famille</label><input type='text' maxlength='64' class='form-control' name='1bn' id='1bn' value='"+WatchOwner1LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>1er Propriétaire - Prénom</label><input type='text' maxlength='64' class='form-control' name='1bv' id='1bv' value='"+WatchOwner1FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Sélectionnez le Sexe ici:</label>");   
    HTMLString += F("<select name='GS1' id='GS1' class='form-control'>");
    if (Gender1 == "") {HTMLString += "<option selected value=''>Genre</option>";} else {HTMLString += "<option value=''>Genre</option>";}
    if (Gender1 == "female") {HTMLString += "<option selected value='female'>femelle</option>";} else {HTMLString += "<option value='female'>femelle</option>";}
    if (Gender1 == "male") {HTMLString += "<option selected value='male'>mâle</option>";} else {HTMLString += "<option value='male'>mâle</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>1er Propriétaire - Date de Naissance*</label><input type='text' maxlength='5' class='form-control' name='1bgb' id='1bgd' value='"+Birthday1+"'></div>";
    HTMLString += F("<br/>"); 
    // 2ème Propriétaire
    HTMLString += "<div class='form-floating'><br/><label>2e Propriétaire - Nom de Famille</label><input type='text' maxlength='64' class='form-control' name='2bn' id='2bn' value='"+WatchOwner2LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>2ème Propriétaire - Prénom</label><input type='text' maxlength='64' class='form-control' name='2bv' id='2bv' value='"+WatchOwner2FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Sélectionnez le Sexe ici:</label>");   
    HTMLString += F("<select name='GS2' id='GS2' class='form-control'>");
    if (Gender2 == "") {HTMLString += "<option selected value=''>Genre</option>";} else {HTMLString += "<option value=''>Genre</option>";}
    if (Gender2 == "female") {HTMLString += "<option selected value='female'>femelle</option>";} else {HTMLString += "<option value='female'>femelle</option>";}
    if (Gender2 == "male") {HTMLString += "<option selected value='male'>mâle</option>";} else {HTMLString += "<option value='male'>mâle</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>2ème Propriétaire - Date de Naissance*</label><input type='text' maxlength='5' class='form-control' name='2bgb' id='2bgd' value='"+Birthday2+"'></div>";
    HTMLString += F("<br/>"); 
    // 3ème Propriétaire
    HTMLString += "<div class='form-floating'><br/><label>3ème Propriétaire - Nom de Famille</label><input type='text' maxlength='64' class='form-control' name='3bn' id='3bn' value='"+WatchOwner3LastName+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>3ème Propriétaire - Prénom</label><input type='text' maxlength='64' class='form-control' name='3bv' id='3bv' value='"+WatchOwner3FirstName+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label>Sélectionnez le Sexe ici:</label>");   
    HTMLString += F("<select name='GS3' id='GS3' class='form-control'>");
    if (Gender3 == "") {HTMLString += "<option selected value=''>Genre</option>";} else {HTMLString += "<option value=''>Genre</option>";}
    if (Gender3 == "female") {HTMLString += "<option selected value='female'>femelle</option>";} else {HTMLString += "<option value='female'>femelle</option>";}
    if (Gender3 == "male") {HTMLString += "<option selected value='male'>mâle</option>";} else {HTMLString += "<option value='male'>mâle</option>";}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += "<div class='form-floating'><br/><label>3ème Propriétaire - Date de Naissance*</label><input type='text' maxlength='5' class='form-control' name='3bgb' id='3bgd' value='"+Birthday3+"'></div>";
    HTMLString += F("<div class='form-floating'><br/><label style=\"margin-left: 1px;\">*) Date de Naissance sans Zéro non Significatif et Année</label></div>");
    HTMLString += F("<div class='form-floating'><label style=\"margin-left: 18px;\">(Exemple: 17.3 oder 2.4 oder 5.10 oder 23.11)</label></div>");
    HTMLString += F("<br/><br/>"); 
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 6px; font-weight: bold;\">+++ OpenWeatherMap et NewsData (Facultatif) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - Clé API</label><input type='text' maxlength='64' class='form-control' name='apikey' id='apikey' value='"+weatherKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>OpenWeatherMap - ID de la Ville</label><input type='text' maxlength='10' class='form-control' name='cityid' id='cityid' value='"+cityID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>NewsData - Clé API</label><input type='text' maxlength='64' class='form-control' name='napi' id='napi' value='"+newsKey+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 30px; font-weight: bold;\">+++ Mesure de la Radioactivité (Facultatif) +++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - ID d'Historique</label><input type='text' maxlength='15' class='form-control' name='gmcmhid' id='gmcmhid' value='"+GMCMapHistoryID+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>GMCMap.com - Décalage-GMT</label><input type='text' maxlength='3' class='form-control' name='gmcmgmto' id='gmcmgmto' value='"+GMCMapGMTOffset+"'></div>";
    HTMLString += F("<br/>");
    HTMLString += "<div class='form-floating'><br/><label style=\"margin-left: 15px; font-weight: bold;\">++ Pushover Service de Messagerie (Facultatif) ++</label></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Jeton d'Application Sécurisé</label><input type='text' maxlength='64' class='form-control' name='posat' id='posat' value='"+SecureAppToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Jeton Utilisateur</label><input type='text' maxlength='64' class='form-control' name='pout' id='pout' value='"+UserToken+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 1</label><input type='text' maxlength='64' class='form-control' name='podev1' id='podev1' value='"+PushoverDevice1+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 2 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev2' id='podev2' value='"+PushoverDevice2+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Nom de l'Appareil 3 (Optional)</label><input type='text' maxlength='64' class='form-control' name='podev3' id='podev3' value='"+PushoverDevice3+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Pushover - Son des Messages (https://pushover.net/api#sounds)</label><input type='text' maxlength='32' class='form-control' name='poms' id='poms' value='"+PushoverMessageSound+"'></div>";
    HTMLString += F("<br/>"); 
    HTMLString += F("<div class='form-floating'><br/><label>Afficher les Fantômes à Minuit ? [OUI / NON]</label>");   
    HTMLString += F("<select name='MG1' id='MG1' class='form-control'>");
    if (MidnightGhost == "OUI") {HTMLString += F("<option selected value='OUI'>OUI</option>");} else {HTMLString += F("<option value='OUI'>OUI</option>");}
    if (MidnightGhost == "NON") {HTMLString += F("<option selected value='NON'>NON</option>");} else {HTMLString += F("<option value='NON'>NON</option>");}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
    HTMLString += F("<br/>");
    HTMLString += F("<div class='form-floating'><br/><label>Capteur de Température et d'Humidité ?</label>"); 
    HTMLString += F("<select name='DHT' id='DHT' class='form-control'>");
    if (DHTSensor == "NoDHT") {HTMLString += F("<option selected value='NoDHT'>NON</option>");} else {HTMLString += F("<option value='NoDHT'>NON</option>");}
    if (DHTSensor == "DHT11") {HTMLString += F("<option selected value='DHT11'>DHT11</option>");} else {HTMLString += F("<option value='DHT11'>DHT11</option>");}
    if (DHTSensor == "DHT22") {HTMLString += F("<option selected value='DHT22'>DHT22</option>");} else {HTMLString += F("<option value='DHT22'>DHT22</option>");}
    HTMLString += F("</select>");
    HTMLString += F("</div>");
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
    String HTMLString; HTMLString.reserve(220);
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
String HTMLString; HTMLString.reserve(220);   
HTMLString = CP_HTMLStringPart1();
HTMLString += F("<body><main class='form-signin'>");
HTMLString += F("<h1>Portail Captif</h1>");
HTMLString += "<h5>("+String(ProgramName)+")</h5>";
HTMLString += F("<br/>");
HTMLString += F("<h8>&nbsp;&nbsp;Le site n'a pas été trouvé !</h8>");
HTMLString += F("<br/><br/>");
HTMLString += F("<a href =\"/\"><button class=\"button\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</main>");
HTMLString += F("</body>");
HTMLString += F("</html>");    
SendHTMLString(HTMLString);
MyWaitLoop(100);} // Envoyer une chaîne HTML  

// *****************************************************************************************************************************************************

void RemoteAPIKeyUpdate(const char* APIModul,const char* NewKey,const char* OWMCID) {
if (APIModul != "") {  
if (littleFSInit) { 
MyWaitLoop(1000); // courte attente 
if (SerialMonitor) {DEBUG_PRINTLN("");
DEBUG_PRINTLN("****** Mettre à jour les clés API *******");}      
if (APIModul == "N") { // Module Actualités 
if (NewKey != "") {
if (LittleFS.exists("/newsapi-apikey.txt")){  
LittleFS.remove("/newsapi-apikey.txt");}
if (LittleFS.exists("/newsdata-apikey.txt")){  
LittleFS.remove("/newsdata-apikey.txt");}
SetupSave(F("newsdata-apikey.txt"), NewKey);
newsKey = NewKey;
if (SerialMonitor) {DEBUG_PRINTLN("Clé API Actualités : "+newsKey);}}}
if (APIModul == "W") { // Module Météo
if (NewKey != "") {
if (LittleFS.exists(F("/owm-apikey.txt"))){  
LittleFS.remove(F("/owm-apikey.txt"));}
SetupSave(F("owm-apikey.txt"), NewKey);
if (LittleFS.exists(F("/owm-cityid.txt"))){  
LittleFS.remove(F("/owm-cityid.txt"));}
SetupSave(F("owm-cityid.txt"), OWMCID);
weatherKey = NewKey;
cityID = OWMCID;
if (SerialMonitor) {DEBUG_PRINTLN("Clé API OpenWeatherMap : "+weatherKey);
DEBUG_PRINTLN("ID de ville OpenWeatherMap : "+cityID);}}}}}}

// *****************************************************************************************************************************************************

String CP_HTMLStringPart1() {
String HTMLString; HTMLString.reserve(220);
HTMLString = F("<!doctype html><html lang='fr'>");
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
HTMLString += F("h3 {font-size: 16px; color: red; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
HTMLString += F("h4 {font-size: 18px; color: red; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
HTMLString += F("h5 {font-size: 24px; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
HTMLString += F("h6 {font-size: 18px; text-align:center; margin-top: 0px; margin-bottom: 15px;}");
HTMLString += F("h7 {font-size: 20px; font-weight: bold; margin-left: 100px; margin-top: 0px; margin-bottom: 5px; color: #06942c;}");
HTMLString += F("h8 {font-size: 20px; text-align:center; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
return HTMLString;}

// ************************************************************************************************************************************************