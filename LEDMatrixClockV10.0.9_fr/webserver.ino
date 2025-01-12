// Page d'accueil du Serveur Web
void handleRoot() { 
// envoyer des données
if (!MEP){
server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
SendHTMLString(""); // Send HTMLString
MyWaitLoop(100);
server.sendContent(RootHTMLString_1());  // 1. Partie
server.sendContent(RootHTMLString_2());  // 2. Partie
server.sendContent(RootHTMLString_3());  // 3. Partie
server.sendContent(RootHTMLString_4());  // 4. Partie
server.sendContent(RootHTMLString_5());  // 5. Partie
server.sendContent(RootHTMLString_6());  // 6. Partie
if (AccessOpenWeatherMap) {server.sendContent(RootHTMLString_7());}  // 7. Partie
if (AccessNewsData) {server.sendContent(RootHTMLString_8());}  // 8. Partie
if (AccessGMCMap) {server.sendContent(RootHTMLString_9());} // 9. Partie
if (IsBuzzer) {server.sendContent(RootHTMLString_10());} // 10. Partie uniquement pour Buzzer piézo passif
server.sendContent(RootHTMLString_11()); // 11. Partie
server.sendContent(RootHTMLString_12()); // 12. Partie
server.client().flush();
MyWaitLoop(100); // petite attente pour que la mémoire puisse être vidée
server.client().stop();
if (RebootNow) {
if (!Display) {
printStringWithShift("           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Afficher sur  
} // Allumer l'affichage lorsqu'il est éteint 
ct = cleanText("        +++       L'Horloge va Redémarrer      +++           "); 
printStringWithShift(ct.c_str(),ScrollTextTime); 
StatusWarningLED("LOW"); // LED éteinte
ESP.restart();} // Redémarrer l'Horloge
if (littleFSInit && ResetNow) {ResetClock();}} else {PleaseWait();}} // Paramètres sur Standard - Les données d'accès WLAN seront supprimées

/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Space | <meta http-equiv=\"refresh\" content=\"60\">\ = Refresh all 60 sec*/ 
String RefreshTime = F("60"); // Actualiser le site Web après X secondes
  String HTMLString = F("");
   String InTempOnOff1 = F("");
    String InTempOnOff2 = F("");

    CheckInHumidity(); // Mesure de l'humidité intérieure

  // Afficher la Température intérieure OUI/NON (DHT11 or DHT22)
  if (InTempCounter < 5) {
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Mesure de Température 
  if (InTempValue != "-" && InTempValue != "!") {
  IntInTempValue = InTempValue.toInt();  
  if (IntInTempValue < 10) {
  InTempOnOff1 = F("id= 'Bvalue'>Température sur l'Horloge:");  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {InTempOnOff1 = F("id= 'Gvalue'>Température sur l'Horloge:");} else {  
  InTempOnOff1 =  F("id= 'Rvalue'>Température sur l'Horloge:");}}
  if (InHumMessage == "") { 
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 52px; width: 170px; font-size: 14px; color: blue\">Affiche la Température Intérieure</button></a>");} else {
  InTempOnOff2 =  F("<a href =\"/tempvalue\"><button class=\"button\" style=\"margin-right: 10px; margin-left: 3px; height: 52px; width: 170px; font-size: 14px; color: blue\">Affiche la Température Intérieure</button></a>"); 
  InTempOnOff2 += "<h9>"+InHumMessage+"</h9>";}
  InTempOnOff2 += F("<p></p>");
  InTempOnOff2 += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  } else {InTempOnOff1 = F("");
          InTempOnOff1 = F("");}
  } else {InTempOnOff1 = F("");
          InTempOnOff2 = F("");
          InTempValue = F("!");}

  // Vérifier la sous-chaîne HTML
 //  if (SerialMonitor) {Serial.println("<a"+InTempOnOff2+"</a>\");} // Uniquement pour le débogage
// 1. Une partie de la chaîne HTML
  HTMLString = F("<!DOCTYPE html><html lang='fr'>"); 
  HTMLString += F("<head>");
  HTMLString += F("<title>Horloge-Matricielle-LED</title>");
  HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body { background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
  HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
  HTMLString += F("h2 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h3 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h4 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h5 { color: white;font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h6 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h7 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffffff;}");
  HTMLString += F("h8 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #f74f4f; font-weight: bold;}");
  HTMLString += F("h9 { font-size: 18px; margin-left: 80px; margin-top: 0px; margin-bottom: 0px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("h10 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}");
  HTMLString += F("#Rvalue {color: #FC3A30;}");
  HTMLString += F("#Gvalue {color: lime;}");
  HTMLString += F("#Bvalue {color: #2ECCFA;}");
  HTMLString += F("</style>");
  HTMLString += F("</head>");
  HTMLString += F("<body>");
  HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += InTempOnOff2;
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){ 
// Sélection de la luminosité de la matrice et du temps d'exécution de l'Horloge
  String HTMLString = F("");
  // 2. Une partie de la chaîne HTML
  HTMLString = F("<h2>Sélection - Affichage ALLUMÉ / DÉSACTIVÉ / TEST / Niveaux de Luminosité:</h2>");
  if (!Display) { 
  HTMLString += CreateButton("Affichage Allumé",false,"mon","","140","10",""); 
  HTMLString += CreateButton("Affichage Éteint",true,"moff","","120","20","");
  HTMLString += CreateButton("Afficher Test",false,"","","110","20","disabled");} else {
  HTMLString += CreateButton("Affichage Allumé",true,"mon","","140","10","");  
  HTMLString += CreateButton("Affichage Éteint",false,"moff","","120","20","");
  HTMLString += CreateButton("Afficher Test",false,"mdt","","110","20","");}

  if (!Display) {  
  if (Brightness == "L1") {HTMLString += CreateButton("Niveau",true,""," 1","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 1","80","10","disabled");}
  if (Brightness == "L2") {HTMLString += CreateButton("Niveau",true,""," 2","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 2","80","10","disabled");}
  if (Brightness == "L3") {HTMLString += CreateButton("Niveau",true,""," 3","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 3","80","10","disabled");}
  if (Brightness == "L4") {HTMLString += CreateButton("Niveau",true,""," 4","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 4","80","10","disabled");}
  if (Brightness == "L5") {HTMLString += CreateButton("Niveau",true,""," 5","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 5","80","10","disabled");}} 
  else {
  if (Brightness == "L1") {HTMLString += CreateButton("Niveau",true,"mb1"," 1","80","10","");} else {HTMLString += CreateButton("Niveau",false,"mb1"," 1","80","10","");}
  if (Brightness == "L2") {HTMLString += CreateButton("Niveau",true,"mb2"," 2","80","10","");} else {HTMLString += CreateButton("Niveau",false,"mb2"," 2","80","10","");}
  if (Brightness == "L3") {HTMLString += CreateButton("Niveau",true,"mb3"," 3","80","10","");} else {HTMLString += CreateButton("Niveau",false,"mb3"," 3","80","10","");}
  if (Brightness == "L4") {HTMLString += CreateButton("Niveau",true,"mb4"," 4","80","10","");} else {HTMLString += CreateButton("Niveau",false,"mb4"," 4","80","10","");}
  if (Brightness == "L5") {HTMLString += CreateButton("Niveau",true,"mb5"," 5","80","10","");} else {HTMLString += CreateButton("Niveau",false,"mb5"," 5","80","10","");}}  
  HTMLString += F("<p></p>");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_3(){
String HTMLString = F("");
String DisplayAutomaticOnOff = F("");  
// Afficher automatiquement ALLUMÉ/DÉSACTIVÉ si spécifié dans le point d'accès
  if (DisplayOnTime != "" && DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Afficher l'allumage automatique à &nbsp;"+FDOnTime+"&nbsp;&nbsp;heures<br />Afficher l'DÉSACTIVÉ automatique à &nbsp;"+FDOffTime+"&nbsp;&nbsp;heures";} else {
  if (DisplayOnTime == "" && DisplayOffTime == ""){
  DisplayAutomaticOnOff = F("L'Affichage automatique ALLUMÉ / DÉSACTIVÉ est désactivé");} else {  
  if (DisplayOnTime != ""){
  DisplayAutomaticOnOff = "Afficher l'allumage automatique à &nbsp;"+FDOnTime+"&nbsp;&nbsp;heures";} else {
  if (DisplayOffTime != ""){
  DisplayAutomaticOnOff = "Afficher l'DÉSACTIVÉ automatique à &nbsp;"+FDOffTime+"&nbsp;&nbsp;heures";}}}}
// 3. Une partie de la chaîne HTML
HTMLString = "<h4>"+DisplayAutomaticOnOff+"</h4>";
HTMLString += F("<a href =\"/cdof\"><button class=\"button\" style=\"margin-right: 10px; height: 24px; font-size: 14px; font-weight: bold; width: 110px;\">Changement</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}  

/********************************************************************************************************************************************************/

String RootHTMLString_4(){ // Statut Côlon
  String HTMLString = F("");
  // 4. Une partie de la chaîne HTML
  HTMLString = F("<h2>Sélection - Statut Côlon du Temps:</h2>");
  if (!Display) {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Éclair",true,"","","90","10","disabled");} else {
  HTMLString += CreateButton("Éclair",false,"","","90","10","disabled");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Allumé en Permanence",true,"","","160","10","disabled");} else {
  HTMLString += CreateButton("Allumé en Permanence",false,"","","160","10","disabled");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Désactivé Permanent",true,"","","150","10","disabled");} else {
  HTMLString += CreateButton("Désactivé Permanent",false,"","","150","10","disabled");}  
  } else {
  if (DoublePointStatus == 2){
  HTMLString += CreateButton("Éclair",true,"sdflash","","90","10","");} else {
  HTMLString += CreateButton("Éclair",false,"sdflash","","90","10","");}
  if (DoublePointStatus == 1){
  HTMLString += CreateButton("Allumé en Permanence",true,"sdon","","160","10","");} else {
  HTMLString += CreateButton("Allumé en Permanence",false,"sdon","","160","10","");}
  if (DoublePointStatus == 0){
  HTMLString += CreateButton("Désactivé Permanent",true,"sdoff","","150","10","");} else {
  HTMLString += CreateButton("Désactivé Permanent",false,"sdoff","","150","10","");}}
  HTMLString += F("<p></p>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_5(){ // Niveau de vitesse du texte de défilement
String HTMLString = F("");
  // 5. Une partie de la chaîne HTML
  HTMLString = F("<h2>Sélection - Vitesse de Défilement du Texte:</h2>"); 
  if (!Display) { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Niveau",true,""," 1","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 1","80","10","disabled");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Niveau",true,""," 2","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 2","80","10","disabled");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Niveau",true,""," 3","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 3","80","10","disabled");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Niveau",true,""," 4","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 4","80","10","disabled");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Niveau",true,""," 5","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 5","80","10","disabled");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Niveau",true,""," 6","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 6","80","10","disabled");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Niveau",true,""," 7","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 7","80","10","disabled");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Niveau",true,""," 8","80","10","disabled");} else {HTMLString += CreateButton("Niveau",false,""," 8","80","10","disabled");} 
  } else { 
  if (ScrollTextTime == 60) {HTMLString += CreateButton("Niveau",true,"vlt1"," 1","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt1"," 1","80","10","");}
  if (ScrollTextTime == 55) {HTMLString += CreateButton("Niveau",true,"vlt2"," 2","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt2"," 2","80","10","");}
  if (ScrollTextTime == 50) {HTMLString += CreateButton("Niveau",true,"vlt3"," 3","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt3"," 3","80","10","");}
  if (ScrollTextTime == 45) {HTMLString += CreateButton("Niveau",true,"vlt4"," 4","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt4"," 4","80","10","");}
  if (ScrollTextTime == 40) {HTMLString += CreateButton("Niveau",true,"vlt5"," 5","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt5"," 5","80","10","");}
  if (ScrollTextTime == 35) {HTMLString += CreateButton("Niveau",true,"vlt6"," 6","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt6"," 6","80","10","");}
  if (ScrollTextTime == 30) {HTMLString += CreateButton("Niveau",true,"vlt7"," 7","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt7"," 7","80","10","");}
  if (ScrollTextTime == 25) {HTMLString += CreateButton("Niveau",true,"vlt8"," 8","80","10","");} else {HTMLString += CreateButton("Niveau",false,"vlt8"," 8","80","10","");}}
  HTMLString += F("<p></p>"); 
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_6(){ // Fonctions Supplémentaires
String HTMLString = F("");
String EF = F("");
if (EnhancedFunctions) {
EF =  CreateButton("ALLUMÉ",true,"zfon","","90","10","");
EF += CreateButton("DÉSACTIVÉ",false,"zfoff","","90","10","");} else {
EF =  CreateButton("ALLUMÉ",false,"zfon","","90","10","");
EF += CreateButton("DÉSACTIVÉ",true,"zfoff","","90","10","");}
// 6. Une partie de la chaîne HTML
HTMLString = "<h2>Sélection - Fonctions Supplémentaires &nbsp;ALLUMÉ / DÉSACTIVÉ: &nbsp;(Date, Jours Fériés, Anniversaires)</h2>";
HTMLString += EF; // Fonctions supplémentaires (date, jours fériés, anniversaires) - ALLUMÉ / DÉSACTIVÉ
if (EnhancedFunctions && GreetingsToday) {
if (GT1 != "" && HT1 != "") { // GT1 = Anniversaires et autres vœux HT1 = Vacances
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Aujourd'hui: &nbsp;&nbsp;"+HT1+"&nbsp; / &nbsp;"+GT1+"</a>";} else {  
if (GT1 != "") {    
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Aujourd'hui: &nbsp;&nbsp;"+GT1+"</a>";}
if (HT1 != "") {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">Aujourd'hui: &nbsp;&nbsp;"+HT1+"</a>";}}} else { // Fonctionnalités supplémentaires
if (EnhancedFunctions) {
HTMLString += "<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\">&nbsp;&nbsp;"+DateString+"</a>";} else {
HTMLString += F("<a style=\"font-family:arial; color: aqua; font-size: 16px; margin-left: 40px;\"></a>");}}
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_7() { // Afficher les données météorologiques d'OpenWeatherMap
// Activer/désactiver les données météorologiques
String HTMLString = F("");
String OWMA = F("");
// Afficher ou non le statut d'OpenWeatherMap
if (AccessOpenWeatherMap) {
if (WeatherFunctions) {
OWMA =  CreateButton("ALLUMÉ",true,"owmfon","","90","10","");
OWMA += CreateButton("DÉSACTIVÉ",false,"owmfoff","","90","10","");} else {
OWMA =  CreateButton("ALLUMÉ",false,"owmfon","","90","10","");  
OWMA += CreateButton("DÉSACTIVÉ",true,"owmfoff","","90","10","");}

if (WeatherFunctions && StatusOpenWeatherMap != "enabled" && Display) {
OWMA += CreateButton("Météo",false,"aowmn","","90","10","");} else {
OWMA += CreateButton("Météo",false,"aowmn","","90","10","disabled");}

if (!WeatherFunctions) {StatusOpenWeatherMap = F("disable");} else {
if (OWMAdjustChange) {StatusOpenWeatherMap = F("enabled"); OWMAdjustChange = false;}}
if (WeatherFunctions && StatusOpenWeatherMap == "enabled" && Display) {
OWMA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;activé</a>");} 
if (WeatherFunctions && StatusOpenWeatherMap == "OK" && Display) {
String WSTemp = String(temp, 0); WSTemp.trim();
if (IsNumeric(WSTemp) && IsNumeric(String(humidity))) { // Check Temperatur und Luftfeuchte 
OWMA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;D'ACCORD - "+OWMLastConnectionDateTime+"";
OWMA += "&nbsp;&nbsp;&nbsp;&nbsp;(TE=&nbsp;"+WSTemp+"°C &nbsp;/&nbsp; HE=&nbsp;"+String(humidity)+"%)</a>";} else {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;Erreur - "+OWMLastConnectionDateTime+"</a>";}} 
if (WeatherFunctions && StatusOpenWeatherMap == "Error" && Display) {
OWMA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;Erreur - "+OWMLastConnectionDateTime+"</a>";}
if (WeatherFunctions && !Display) {
OWMA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Statut: &nbsp;+++ Pas de Téléchargement des Données Météo en Affichage DÉSACTIVÉ +++</a>");}}
// 7. Une partie de la chaîne HTML
HTMLString =  F("<h2>Sélection - Afficher les données Météorologiques actuelles de ---OpenWeatherMAP.org--- &nbsp; ALLUMÉ / DÉSACTIVÉ / WEATHER / STATUT:</h2>");
HTMLString += OWMA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_8() { // Afficher les actualités NewsData.io
String HTMLString = F("");
String ANA = F("");
  if (AccessNewsData) {
// NewsData ALLUMÉ / DÉSACTIVÉ
if (NewsDataFunction) {
ANA =  CreateButton("ALLUMÉ",true,"nafon","","90","10","");
ANA += CreateButton("DÉSACTIVÉ",false,"nafoff","","90","10","");} else {
ANA =  CreateButton("ALLUMÉ",false,"nafon","","90","10","");
ANA += CreateButton("DÉSACTIVÉ",true,"nafoff","","90","10","");} 

if (NewsDataFunction && StatusNewsData != "enabled" && Display) {
ANA += CreateButton("Nouvelles",false,"aapin","","90","10","");} else {
ANA += CreateButton("Nouvelles",false,"aapin","","90","10","disabled");}

if (!NewsDataFunction) {StatusNewsData = F("disabled");} else {
if (NAAdjustChange) {StatusNewsData = F("enabled"); NAAdjustChange = false;}} 

if (NewsDataFunction && StatusNewsData == "enabled" && Display) {
ANA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;activé</a>");} 
if (NewsDataFunction && StatusNewsData == "OK" && Display) {
if (NewsOutOfCredits) {  
ANA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;D'ACCORD - "+NewsDataLastConnectionDateTime+"</a>";} else
{ANA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;D'ACCORD - "+NewsDataLastConnectionDateTime+"</a>";}}  
if (NewsDataFunction && StatusNewsData == "Error" && Display) {
ANA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;Erreur - "+NewsDataLastConnectionDateTime+"</a>";}
if (NewsDataFunction && !Display) {
ANA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Statut: &nbsp;+++ Pas de Téléchargement des Messages lorsque Affichage DÉSACTIVÉ +++</a>");}}
// 8. Une partie de la chaîne HTML
HTMLString = F("<h2>Sélection - Afficher les Actualités actuelles de ---NewsData.io--- &nbsp; ALLUMÉ / DÉSACTIVÉ / NOUVELLES / STATUT:</h2>");
HTMLString += ANA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/ 

String RootHTMLString_9() { // Afficher www.gmcmap.com
String HTMLString = F("");
String ARA = F("");
  if (AccessGMCMap) {
// GMCMap allumé/désactivé
if (GMCMapFunction) {
ARA =  CreateButton("ALLUMÉ",true,"gmcmapon","","100","10","");
ARA += CreateButton("DÉSACTIVÉ",false,"gmcmapoff","","100","10","");} else {
ARA =  CreateButton("ALLUMÉ",false,"gmcmapon","","100","10","");
ARA += CreateButton("DÉSACTIVÉ",true,"gmcmapoff","","100","10","");} 

if (GMCMapFunction && StatusGMCMap != "enabled" && Display) {
ARA += CreateButton("Valeur actuelle",false,"agmcmapd","","110","10","");} else {
ARA += CreateButton("Valeur actuelle",false,"agmcmapd","","110","10","disabled");}

if (!GMCMapFunction) {StatusGMCMap = F("disabled");} else {
if (GMCMapAdjustChange) {StatusGMCMap = F("enabled"); GMCMapAdjustChange = false;}} 

if (GMCMapFunction && StatusGMCMap == "enabled" && Display) {
ARA += F("<a style=\"font-family:arial;color:aqua; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;activé</a>");} 
if (GMCMapFunction && StatusGMCMap == "OK" && Display) {
if (GMCMapToHigh != "") {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;D'ACCORD - "+GMCMapLastConnectionDateTime+"</a>";
ARA += "<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; font-weight: bold; margin-left: 40px;\">Attention: &nbsp;Danger de mort !!!</a>";} else {
ARA += "<a style=\"font-family:arial;color:#27e835; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;D'ACCORD - "+GMCMapLastConnectionDateTime+"</a>";}} 
if (GMCMapFunction && StatusGMCMap == "Error" && Display) {
ARA += "<a style=\"font-family:arial;color:#ff525a; font-size: 16px; font-weight: bold; margin-left: 40px;\">Statut: &nbsp;Erreur - "+GMCMapLastConnectionDateTime+"</a>";}
if (GMCMapFunction && !Display) {
ARA += F("<a style=\"font-family:arial;color:#ffcc00; font-size: 16px; margin-left: 40px;\">Status: &nbsp;+++ Pas de Téléchargement des Valeurs GMCMap lorsque l'Affichage désactivé +++</a>");}} 
// 9. Une partie de la chaîne HTML
HTMLString = F("<h2>Sélection - Valeurs actuelles de la Radioactivité de ---www.gmcmap.com--- &nbsp; ALLUMÉ / DÉSACTIVÉ / VALEUR ACTUELLE / STATUT:</h2>");
HTMLString += ARA;
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_10() { // Paramètres du buzzer piézo passif
String HTMLString = F("");
String BSS = F("");
String BHS = F("");
// Son de démarrage ALLUMÉ / DÉSACTIVÉ
  if (StartSound == "On") { 
  BSS =  CreateButton("Son de Démarrage ALLUMÉ",true,"sspbson","","200","10",""); 
  BSS += CreateButton("Son de Démarrage DÉSACTIVÉ",false,"sspbsoff","","210","10","");} else {
  BSS =  CreateButton("Son de Démarrage ALLUMÉ",false,"sspbson","","200","10",""); 
  BSS += CreateButton("Son de Démarrage DÉSACTIVÉ",true,"sspbsoff","","210","10","");} 
  // Son Horaire ALLUMÉ / DÉSACTIVÉ
  if (HourSound == "On") {
  BHS =  CreateButton("Son Horaire ALLUMÉ",true,"hspbson","","170","10","");
  BHS += CreateButton("Son Horaire DÉSACTIVÉ",false,"hspbsoff","","170","10","");} else {
  BHS =  CreateButton("Son Horaire ALLUMÉ",false,"hspbson","","170","10","");
  BHS += CreateButton("Son Horaire DÉSACTIVÉ",true,"hspbsoff","","170","10","");} 

// 10. Une partie de la chaîne HTML   
HTMLString = F("<h2>Sélection - Paramètres Sonores:</h2>");
HTMLString += ""+BSS+BHS+"";
HTMLString += F("<p></p>"); 
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_11() { // Paramètres du portail Captiv
String HTMLString = F("");
// 11. Une partie de la chaîne HTML  
HTMLString = F("<h2>Affichage - Paramètres du Point d'Accès (Portail-Captif):</h2>");
HTMLString += F("<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 110px;\">Paramètres</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_12() { // Version du Logiciel
String HTMLString = F("");
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// int ESPRAM = ESP.getMaxFreeBlockSize() / 1024;  // Vérifier la RAM
int ESPRAM = ESP.getFreeHeap() / 1024; // Vérifier la RAM
String ESPCore = ESP.getCoreVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Core Version
// String ESPCore = ESP.getFullVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Full Version
uint8_t CPUClock = ESP.getCpuFreqMHz(); // CPU-Clock
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 12. Une partie de la chaîne HTML   
HTMLString = "<h7>Version du Logiciel: &nbsp;"+LMCVersion+"</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Version du Matériel: &nbsp;"+ESPCore+"</h7>";
HTMLString += F("<a><br /></a>");
if (ESPRAM > 5) {
HTMLString += "<h7>Mémoire RAM Libre: &nbsp;&nbsp;"+String(ESPRAM)+" kB / 64 kB</h7>";} else {
HTMLString += "<h8>Mémoire RAM Libre: &nbsp;&nbsp;"+String(ESPRAM)+" kB / 64 kB (Danger ! - trop peu)</h8>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Fréquence d'Horloge du Processeur: &nbsp;&nbsp;"+String(CPUClock)+"&nbsp; MHz</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += WiFiSignalStrength(false); // Intensité du Signal WiFi en dBm
HTMLString += F("<a><br /></a>");
if (ClockStatus == "OK") {
HTMLString += F("<h7>Statut du dernier Démarrage: &nbsp;&nbsp;OK</h7>");} else {
HTMLString += "<h10>Statut du dernier Démarrage: &nbsp;&nbsp;"+ClockStatus+"</h10>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Durée d'exécution de l'Horloge: &nbsp;&nbsp;"+rtime+"</h7>";
HTMLString += F("<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px; height: 48px; width: 140px;color: red;float: right; font-size: 20px;\">Réinitialiser !</button></a>");
HTMLString += F("<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px; height: 48px; width: 130px;color: blue;float: right; font-size: 18px;\">Redémarrage</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h6>*) - Sélection actuelle</h6>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
return HTMLString;}

/********************************************************************************************************************************************************/ 

void HTMLAnswer(String WebFunction) {
String HTMLString = F("");
String HTMLAnswerString = F("");  
if (WebFunction == "MatrixOn") {HTMLAnswerString = F("L'Affichage Matriciel LED a été Commuté ALLUMÉ");} 
if (WebFunction == "MatrixOff") {HTMLAnswerString = F("L'Affichage Matriciel LED est Commuté DÉSACTIVÉ");} 
if (WebFunction == "MatrixIsOn") {HTMLAnswerString = F("L'Affichage Matriciel LED est déjà Commuté ALLUMÉ");} 
if (WebFunction == "MatrixIsOff") {HTMLAnswerString = F("L'Affichage Matriciel LED est déjà Commuté DÉSACTIVÉ");}
if (WebFunction == "RebootClock") {HTMLAnswerString = F("Lorsque vous Revenez à la Page d'Accueil<br />l'Horloge Matricielle LED Redémarrera");}
if (WebFunction == "DoublePointFlash") {HTMLAnswerString = F("Le Côlon de l'Horloge va Maintenant Clignoter.");} 
if (WebFunction == "DoublePointOn") {HTMLAnswerString = F("Le Côlon de l'Horloge est désormais Définitivement ALLUMÉ.");} 
if (WebFunction == "DoublePointOff") {HTMLAnswerString = F("Le Côlon de l'Horloge est désormais Définitivement DÉSACTIVÉ.");} 
if (WebFunction == "BrightnessLevel1") {HTMLAnswerString = F("La Matriciel LED Luminosité Niveau 1 a été activée");} 
if (WebFunction == "BrightnessLevel2") {HTMLAnswerString = F("La Matriciel LED Luminosité Niveau 2 a été activée");} 
if (WebFunction == "BrightnessLevel3") {HTMLAnswerString = F("La Matriciel LED Luminosité Niveau 3 a été activée");} 
if (WebFunction == "BrightnessLevel4") {HTMLAnswerString = F("La Matriciel LED Luminosité Niveau 4 a été activée");} 
if (WebFunction == "BrightnessLevel5") {HTMLAnswerString = F("La Matriciel LED Luminosité Niveau 5 a été activée");} 
if (WebFunction == "ScrollTextTimeLevel1") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 1 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel2") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 2 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel3") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 3 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel4") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 4 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel5") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 5 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel6") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 6 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel7") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 7 a été activé");} 
if (WebFunction == "ScrollTextTimeLevel8") {HTMLAnswerString = F("La Texte défilant Vitesse Niveau 8 a été activé");} 
if (WebFunction == "WeatherFunctionsON1") {HTMLAnswerString = F("La Fonction OpenWeatherMap Weather Data a été Commutée ALLUMÉ");}
if (WebFunction == "WeatherFunctionsON2") {HTMLAnswerString = F("La Fonction OpenWeatherMap Weather Data est déjà Commutée ALLUMÉ");}
if (WebFunction == "WeatherFunctionsOFF")  {HTMLAnswerString = F("La Fonction OpenWeatherMap Weather Data a été Commutée DÉSACTIVÉ");}
if (WebFunction == "NewsDataFunctionON1")  {HTMLAnswerString = F("Le NewsData - Actualités actuelles a été Commuté ALLUMÉ");}
if (WebFunction == "NewsDataFunctionON2")  {HTMLAnswerString = F("Le NewsData - Actualités actuelles est déjà Commuté ALLUMÉ");}
if (WebFunction == "NewsDataFunctionOFF")   {HTMLAnswerString = F("Le NewsData - Actualités Actuelles a été Commuté DÉSACTIVÉ");}
if (WebFunction == "GMCMapFunctionON1")  {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels ont été activés");}
if (WebFunction == "GMCMapFunctionON2")  {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels sont déjà activés");}
if (WebFunction == "GMCMapFunctionOFF")   {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels ont été désactivés");}
if (WebFunction == "EnhancedFunctionsON1") {HTMLAnswerString = F("Les Fonctions Supplémentaires ont été activées<br />(Date, Anniversaires)");}
if (WebFunction == "EnhancedFunctionsON2") {HTMLAnswerString = F("The additional Functions are already switched ALLUMÉ<br />(Date, Anniversaires)");}
if (WebFunction == "EnhancedFunctionsOFF") {HTMLAnswerString = F("The additional Functions have been switched DÉSACTIVÉ<br />(Date, Anniversaires)");}
if (WebFunction == "StartSoundOnM") {HTMLAnswerString = F("Le son de Démarrage de l'Horloge est Devenu ALLUMÉ.");}
if (WebFunction == "StartSoundOnMIs") {HTMLAnswerString = F("Le son de Démarrage de l'Horloge est déjà activé ALLUMÉ.");}
if (WebFunction == "StartSoundOffM") {HTMLAnswerString = F("Le son de Démarrage de l'Horloge est devenu DÉSACTIVÉ.");}
if (WebFunction == "StartSoundOffMIs") {HTMLAnswerString = F("Le son de Démarrage de l'Horloge est déjà réglé sur DÉSACTIVÉ.");}
if (WebFunction == "HourSoundOnM") {HTMLAnswerString = F("Le son de l'Heure de l'Horloge est devenu ALLUMÉ.");}
if (WebFunction == "HourSoundOnMIs") {HTMLAnswerString = F("L'Heure de l'Horloge Son est déjà Tourné ALLUMÉ.");}
if (WebFunction == "HourSoundOffM") {HTMLAnswerString = F("L'Heure de l'Horloge Son est déjà Tourné DÉSACTIVÉ.");}
if (WebFunction == "HourSoundOffMIs") {HTMLAnswerString = F("Le son de l'Horloge est déjà désactivé.");}
if (WebFunction == "DisplayCheck") {HTMLAnswerString = F("Le test d'Affichage Matriciel LED est en Cours. S'il vous plaît, attendez ...");}
if (WebFunction == "NoDisplayCheck") {HTMLAnswerString = F("********* est d'Affichage Actuellement Impossible. *********<br />Veuillez attendre que l'Heure s'affiche à Nouveau.");}
if (WebFunction == "ResetClockYes") {HTMLAnswerString = F("Lorsque vous Revenez à la Page d'accueil,<br />le Point d'Accès (Portail Captif) est démarré<br />&nbsp;&nbsp;&nbsp;&nbsp; --- Les Paramètres sont conservés. ---");}
if (WebFunction == "POD1") {HTMLAnswerString = "Le Message de Test Pushover a été Envoyé à "+PushoverDevice1;} 
if (WebFunction == "POD2") {HTMLAnswerString = "Le Message de Test Pushover a été Envoyé à "+PushoverDevice2;} 
if (WebFunction == "POD3") {HTMLAnswerString = "Le Message de Test Pushover a été Envoyé à "+PushoverDevice3;} 
if (WebFunction == "FPO") {HTMLAnswerString = F("Le Message de Test Pushover n'a pas pu être envoyé !");}
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString +=  F("<head>");
HTMLString +=  F("<title>Horloge-Matricielle-LED</title>");
HTMLString +=  F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString +=  F("<style>");
HTMLString +=  F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString +=  F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString +=  F("h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString +=  F("</style>");
HTMLString +=  F("</head>");
HTMLString +=  F("<body>");
HTMLString +=  F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString +=  F("<a><br /></a>");
HTMLString +=  "<h2>"+HTMLAnswerString+"</h2>";
HTMLString +=  F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString +=  F("</body>");
HTMLString +=  F("</html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");}

/********************************************************************************************************************************************************/

void ActualWeather() {
String HTMLString = F(""); 
if (!await) { 
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Weather Data from: "+OWMLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPweatherString+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualAPINews() {
String HTMLString = F("");  
if (!await) {
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>News from: "+NewsDataLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPAPINews+"</h3>";
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ActualGMCMapData() {
String HTMLString = F(""); 
if (!await){   
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 22px; margin-top: 0px; margin-bottom: 50px; }");
HTMLString += F("h3 { font-size: 16px; margin-top: 0px; margin-bottom: 30px; }");
HTMLString += F("h4 { font-size: 16px; margin-top: 0px; color: #ffcc00; margin-bottom: 30px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Last Value of: "+GMCMapLastConnectionDateTime+"</h2>";
HTMLString += "<h3>"+WPGMCMapValue+"</h3>";
HTMLString += "<h4>Attention : Danger à des Valeurs supérieures à 90 CPM!</h4>";
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
HTMLString = F("");} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Matrix On
void MatrixOn() {
if (!MEP) {  
if (!await){  
if (!Display) {
await = true;
MEP = true;    
if (SerialMonitor) {Serial.println(F("La Matrice LED était Allumée."));}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // // Afficher sur
ClockScrollIn(); // L'horloge défile
MyWaitLoop(100); // courte attente
HTMLAnswer("MatrixOn");
Display = true; MEP = false; await = false;} else {HTMLAnswer("MatrixIsOn");}
CheckInHumidity(); // Mesure de l'humidité intérieure
} else {PleaseWait();}}}

/********************************************************************************************************************************************************/

// Matrix Off
void MatrixOff() { 
if (!await){  
if (Display) { 
await = true;   
dots = 1; // Côlon sur  
Display = false;
if (WeatherFunctions) {StatusOpenWeatherMap = F("enabled");}  
if (NewsDataFunction) {StatusNewsData = F("enabled");}   
HTMLAnswer("MatrixOff");
if (SerialMonitor) {Serial.println(F("La Matrice LED est éteinte."));} 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime);
printStringWithShift("           ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,0); await = false;} else {HTMLAnswer("MatrixIsOff");} // Display off
CheckInHumidity(); // Mesure de l'humidité intérieure
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointFlash() {
if (!await){      
DoublePointStatus = 2; // Faire clignoter le Côlon de l'Horloge
// Statut Côlon
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointFlash");
if (SerialMonitor) {Serial.println(F("Le Côlon de l'Horloge clignote."));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void DoublePointOn() {
if (!await){  
DoublePointStatus = 1; // Le Côlon de l'horloge est en permanence MARCHE
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOn");
if (SerialMonitor) {Serial.println(F("Le Côlon de l'Horloge est en permanence MARCHE"));}
} else {PleaseWait();}}


/********************************************************************************************************************************************************/

void DoublePointOff() {
if (!await){  
DoublePointStatus = 0; // Le Côlon de l'horloge est ARRÊTÉ en permanence
if (littleFSInit) {
SetupSave("status_doublepoint.txt", String(DoublePointStatus));}
HTMLAnswer("DoublePointOff");
if (SerialMonitor) {Serial.println(F("Le Côlon de l'Horloge est ARRÊTÉ en permanence"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Réglez les niveaux de luminosité (BrightnessLevel) des LED du Max7219 via le serveur Web
void BrightnessLevel1() {SetBrightnessLevel("L1", 1);}

/********************************************************************************************************************************************************/

void BrightnessLevel2() {SetBrightnessLevel("L2", 2);}

/********************************************************************************************************************************************************/

void BrightnessLevel3() {SetBrightnessLevel("L3", 3);}

/********************************************************************************************************************************************************/

void BrightnessLevel4() {SetBrightnessLevel("L4", 4);}

/********************************************************************************************************************************************************/

void BrightnessLevel5() {SetBrightnessLevel("L5", 5);}

/********************************************************************************************************************************************************/

void SetBrightnessLevel(String BLText, int BLInt) {
if (!await){
sendCmdAll(CMD_INTENSITY, BLInt-1);
Brightness = BLText;
if (littleFSInit) {
SetupSave("matrix_brightness.txt", Brightness);}
HTMLAnswer("BrightnessLevel"+String(BLInt));
if (SerialMonitor) {Serial.println("Luminosité Niveau "+String(BLInt)+" activé");}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

// Ajuster la vitesse du texte défilant
void ScrollTextTimeLevel1() {
SetScrollTimeAndLevel(60,1);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2() { 
SetScrollTimeAndLevel(55,2); }

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel3() { 
SetScrollTimeAndLevel(50,3);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel4() {
SetScrollTimeAndLevel(45,4);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel5() {  
SetScrollTimeAndLevel(40,5);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel6() { 
SetScrollTimeAndLevel(35,6);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel7() {  
SetScrollTimeAndLevel(30,7);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel8() {
SetScrollTimeAndLevel(25,8);}

/********************************************************************************************************************************************************/

void SetScrollTimeAndLevel(int STTime, int STLevel){
if (!await) {
await = true;   
ScrollTextTime = STTime; // Vitesse de défilement du texte 
if (littleFSInit) {
SetupSave("status_scrolltexttime.txt", String(STTime));}
HTMLAnswer("ScrollTextTimeLevel"+String(STLevel)+"");
if (SerialMonitor) {Serial.println("Niveau de Vitesse du Texte défilant "+String(STLevel)+" activé");}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);
ct = cleanText("        +++  Il s'agit d'un Test de Vitesse de défilement du Texte  +++           ");
printStringWithShift(ct.c_str(),ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN, 0);} else {
ct = cleanText("        +++  Il s'agit d'un Test de Vitesse de défilement du Texte  +++           ");  
printStringWithShift(ct.c_str(),ScrollTextTime);
ClockScrollIn();} // Scroll the Time in the Display 
await = false;} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOn() {
if (!await) {  
if (!WeatherFunctions) {
WeatherFunctions = true;
OWMAdjustChange = true;
StatusOpenWeatherMap = F("enabled");
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "On");} 
HTMLAnswer("WeatherFunctionsON1");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Données Météo activées"));}}
else {
HTMLAnswer("WeatherFunctionsON2");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Les données Météorologiques sont déjà activées"));}}   
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOff() {
if (!await) {   
WeatherFunctions = false;
StatusOpenWeatherMap = F("disabled");
if (littleFSInit) {
SetupSave("status_weatherfunctions.txt", "Off");} 
HTMLAnswer("WeatherFunctionsOFF");
if (SerialMonitor) {Serial.println(F("OpenWeatherMap - Données Météo désactivées"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOn() {
if (!await) {   
if (!NewsDataFunction) {
NewsDataFunction = true;
NAAdjustChange = true;
StatusNewsData = F("enabled");
if (littleFSInit) {
SetupSave("status_NewsDatafunctions.txt", "On");}  
HTMLAnswer("NewsDataFunctionON1");
if (SerialMonitor) {Serial.println(F("NewsData - Actualités actuelles activées"));}}
else {
HTMLAnswer("NewsDataFunctionON2");
if (SerialMonitor) {Serial.println(F("NewsData - Les Actualités actuelles sont déjà activées"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOff() {
if (!await) {   
NewsDataFunction = false;
StatusNewsData = F("disabled");
if (littleFSInit) {
SetupSave("status_NewsDatafunctions.txt", "Off");}  
HTMLAnswer("NewsDataFunctionOFF");
if (SerialMonitor) {Serial.println(F("NewsData - Actualités actuelles est déjà désactivé"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOn() {
if (!await) {   
if (!GMCMapFunction) {
GMCMapFunction = true;
GMCMapAdjustChange = true;
StatusGMCMap = F("enabled");
if (littleFSInit) {
SetupSave("status_GMCMapFunction.txt", "On");}  
HTMLAnswer("GMCMapFunctionON1");
if (SerialMonitor) {Serial.println(F("GMCMap - Affichage actuel de la Radioactivité activé"));}}
else {
HTMLAnswer("GMCMapFunctionON2");
if (SerialMonitor) {Serial.println(F("GMCMap - L'Affichage actuel de la Radioactivité est déjà activé"));}} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOff() {
if (!await) {   
GMCMapFunction = false;
StatusGMCMap = F("disabled");
StatusWarningLED("LOW");
if (littleFSInit) {
SetupSave("status_GMCMapFunction.txt", "Off");}  
HTMLAnswer("GMCMapFunctionOFF");
if (SerialMonitor) {Serial.println(F("GMCMap - Affichage actuel de la Radioactivité désactivé"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOn() {
if (!await) {   
if (!EnhancedFunctions) {
EnhancedFunctions = true;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "On");} 
HTMLAnswer("EnhancedFunctionsON1");} else {
HTMLAnswer("EnhancedFunctionsON2");}  
if (SerialMonitor) {Serial.println(F("Fonctions Supplémentaires activées - (Date, Anniversaires)"));} 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOff() {
if (!await) {   
EnhancedFunctions = false;
if (littleFSInit) {
SetupSave("status_enhancedfunctions.txt", "Off");} 
HTMLAnswer("EnhancedFunctionsOFF");
if (SerialMonitor) {Serial.println(F("Fonctions Supplémentaires désactivées - (Date, Anniversaires)"));}
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void GetAdjustments() {
String OWMAPI = F("");
String OWMCID = F("");
String NewsDataKey = F("");
String CheckIP = F(""); 
String HTMLString = F("");
if (!await) {
// Vérifier l'Adresse IP
if (cip.length() > 15 || cip.length() < 7) {CheckIP = F("n/a");} else {CheckIP = cip;} // Vérifier l'Adresse IP
if (AccessOpenWeatherMap){OWMAPI = weatherKey; OWMCID = cityID;} else {OWMAPI = F("n/a");OWMCID = F("n/a");}
if (newsKey != ""){NewsDataKey = newsKey;} else {NewsDataKey = F("n/a");}
HTMLString = F("<!DOCTYPE html><html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
HTMLString += F("h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }");
HTMLString += F("h2 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("h3 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("h4 { font-size: 14px; margin-top: 0px; margin-bottom: 1px; color: #ffcc00; float: right;}");
HTMLString += F("h5 { color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px; }");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "") {
HTMLString += "<h2>( Propriétaire de l'Horloge:&nbsp;&nbsp;"+WatchOwner1FirstName+" "+WatchOwner1LastName+" )</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += F("<h3>Paramètres du Portail Captif</h3>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;"+String(ssid)+"</h2>";
HTMLString += "<h2>Adresse IP:&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += F("<a><br /></a>");
// Propriétaire de l'Horloge en détail
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "" && Gender1 != "" && Birthday1 != "") { 
HTMLString += F("<h2>1er Propriétaire de l'Horloge:</h2>");   
HTMLString += "<h2>Nom de Famille: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner1LastName+"</h2>";
HTMLString += "<h2>Prénom: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner1FirstName+"</h2>";
if (Gender1 == "male") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mâle";}
if (Gender1 == "female") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;femelle";}
HTMLString += "<h2>Date de Naissance: &nbsp;"+Birthday1+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner2FirstName != "" && WatchOwner2LastName != "" && Gender2 != "" && Birthday2 != "") { 
HTMLString += F("<h2>2ème Propriétaire de l'Horloge:</h2>");   
HTMLString += "<h2>Nom de Famille: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner2LastName+"</h2>";
HTMLString += "<h2>Prénom: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner2FirstName+"</h2>";
if (Gender2 == "male") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mâle";}
if (Gender2 == "female") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;femelle";}
HTMLString += "<h2>Date de Naissance: &nbsp;"+Birthday2+"</h2>";
HTMLString += F("<a><br /></a>");}
if (WatchOwner3FirstName != "" && WatchOwner3LastName != "" && Gender3 != "" && Birthday3 != "") { 
HTMLString += F("<h2>3ème Propriétaire de l'Horloge:</h2>");   
HTMLString += "<h2>Nom de Famille: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner3LastName+"</h2>";
HTMLString += "<h2>Prénom: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+WatchOwner3FirstName+"</h2>";
if (Gender3 == "male") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mâle";}
if (Gender3 == "female") {HTMLString += "<h2>Genre: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;femelle";}
HTMLString += "<h2>Date de Naissance: &nbsp;"+Birthday3+"</h2>";
HTMLString += F("<a><br /></a>");}
HTMLString += "<h2>Clé API OpenWeatherMap:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+OWMAPI+"</h2>";
if (WeatherFunctions && String(WeatherLocation) != "" && StatusOpenWeatherMap == "OK") {
HTMLString += "<h2>ID de Ville OpenWeatherMap:&nbsp;&nbsp;&nbsp;"+OWMCID+" ( "+String(WeatherLocation)+" )</h2>";} else {  
HTMLString += "<h2>ID de Ville OpenWeatherMap:&nbsp;&nbsp;&nbsp;"+OWMCID+"</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>Clé API NewsData:&nbsp;&nbsp;"+NewsDataKey+"</h2>";
HTMLString += F("<a><br /></a>");
if (AccessGMCMap) {
HTMLString += "<h2>GMCMap.com --- ID d'Historique:&nbsp;&nbsp;&nbsp;"+GMCMapHistoryID+"</h2>";
HTMLString += "<h2>GMCMap.com --- Décalage-GMT:&nbsp;&nbsp;&nbsp;&nbsp;"+GMCMapGMTOffset+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsPushover) {
HTMLString += "<h2>Pushover-Jeton d'Application sécurisé:  &nbsp;&nbsp;"+SecureAppToken+"</h2>";
HTMLString += "<h2>Pushover-Jeton Utilisateur:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+UserToken+"</h2>";
HTMLString += "<h2>Pushover-Nom de l'Appareil1:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice1+"</h2>";
if (PushoverDevice2 != "") { 
HTMLString += "<h2>Pushover-Nom de l'Appareil2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice2+"</h2>";} else {
HTMLString += "<h2>Pushover-Nom de l'Appareil2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>";}  
if (PushoverDevice3 != "") {
HTMLString += "<h2>Pushover-Nom de l'Appareil3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice3+"</h2>";} else {
HTMLString += "<h2>Pushover-Nom de l'Appareil3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>";}  
HTMLString += "<h2>Pushover-Message Sonore:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverMessageSound+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsBuzzer) {
HTMLString += F("<h2>Buzzer passif Installé</h2>");} else {
HTMLString += F("<h2>Buzzer passif non Installé ou désactivé</h2>");}
HTMLString += F("<a><br /></a>");
if (IsPushover) {
HTMLString += F("<h2><br /></h2>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
HTMLString += F("<h5>Sélection - Envoyer un Message de Test Pushover à l'appareil:</h5>");
if (PushoverDevice1 != "" && Display) {
HTMLString += CreateButton("Appareil 1",false,"potmdevice1","","100","10","");} else {
HTMLString += CreateButton("Appareil 1",false,"potmdevice1","","100","10","disabled");} 
if (PushoverDevice2 != "" && Display) { 
HTMLString += CreateButton("Appareil 2",false,"potmdevice2","","100","10","");} else {
HTMLString += CreateButton("Appareil 2",false,"potmdevice2","","100","10","disabled");}  
if (PushoverDevice3 != "" && Display) { 
HTMLString += CreateButton("Appareil 3",false,"potmdevice3","","100","10","");} else {
HTMLString += CreateButton("Appareil 3",false,"potmdevice3","","100","10","disabled");}  
HTMLString += F("<hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">");
HTMLString += F("<a><br /></a>");}
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 1px; height: 48px; width: 210px\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");
if (SerialMonitor) {Serial.println(F("Les Paramètres du Point d'accès (Portail Captif) sont appelés"));}}
else {PleaseWait();}}

/********************************************************************************************************************************************************/

String CreateButton(String BMark, bool Bselect, String BHRef, String Blevel, String Bwidth, String BRmargin, String Bdisable) {
String HTMLString = F("");
if (Bselect) {
HTMLString  = "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; color: blue; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"*</button></a>";} else {
HTMLString += "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"</button></a>";}
return HTMLString;}

/********************************************************************************************************************************************************/

void ResetClockSettings() {
if (!await) {   
String HTMLString = F("");
HTMLString = F("<!DOCTYPE html> <html lang='fr'>\
  <head>\
  <title>Horloge-Matricielle-LED</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}\
  </style>\
  </head>\
    <body>\
    <h1>Horloge-Matricielle-LED</h1>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Réinitialisation de l'Horloge à Matrice LED !</h2>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Les données d'Accès WLAN seront Supprimées)</h2>\
     <a><br /></a>\
     <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Maintenant, Réinitialisez l'Horloge Matricielle LED ?</h2>\
     <a><br /></a>\
     <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 140px;height: 48px; width: 110px; font-size: 22px; color: red\">Oui</button></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 150px;height: 48px; width: 110px; font-size: 22px;\">Non</button></a>\
  </body>\
    </html>");  
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void ResetClockYes() { 
/*<meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
if (!await) { 
HTMLAnswer("ResetClockYes");
ResetNow = true; // Réinitialiser - Paramètres par défaut 
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void RebootClock()
{
if (!await) {   
HTMLAnswer("RebootClock");
RebootNow = true;
} else {PleaseWait();}}

/********************************************************************************************************************************************************/

void SiteNotFound(){
String HTMLString = F("");
HTMLString = F("<!DOCTYPE html> <html lang='fr'>\
  <head>\
  <title>Horloge-Matricielle-LED</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
   h1 { color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; }\
   h2 { font-size: 26px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>Horloge-Matricielle-LED</h1>\
    <a><br /></a>\
    <h2>Cette Page n'a pas été Trouvée !</h2>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>\
  </body>\
    </html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString = F("");}

/********************************************************************************************************************************************************/

void ChangeDisplayAutoOnOff() {
String HTMLString;
bool MDTime;
if (!await) {
if (server.method() == HTTP_POST) {
    DisplayOnTime = server.arg("DOnT"); // Afficher à temps
    DisplayOffTime = server.arg("DOffT"); // Afficher le temps d'arrêt
    DisplayOnTime.trim();
    DisplayOffTime.trim();

    // Vérifier la plausibilité des informations pour affichage Matriciel automatique Marche/Arrêt
    if (DisplayOnTime != "") {
    MDTime = false;
    if (DisplayOnTime.length() != 5) {
    if (DisplayOnTime.length() == 4) {
    if (DisplayOnTime.indexOf(':') == 1) {
    DisplayOnTime = "0"+DisplayOnTime;
    } else {DisplayOnTime = F("!");}
    } else {DisplayOnTime = F("!");}} else {
    if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = F("!");}}
    if (DisplayOnTime != "!") {
    int DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = CheckMDHourOn.toInt();} else {DisplayOnTime = F("!");}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = CheckMDMinOn.toInt();} else {DisplayOnTime = F("!");}
    if (DisplayOnTime != "" && DisplayOnTime != "!" ) {
    for (int i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");}}}}}

    if (DisplayOffTime != "") {
    MDTime = false;
    if (DisplayOffTime.length() != 5) {
    if (DisplayOffTime.length() == 4) {
    if (DisplayOffTime.indexOf(':') == 1) {
    DisplayOffTime = "0"+DisplayOffTime;
    } else {DisplayOffTime = F("!");}
    } else {DisplayOffTime = F("!");}} else {
    if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = F("!");}}
    if (DisplayOffTime != "!") {
    int DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = CheckMDHourOff.toInt();} else {DisplayOffTime = F("!");}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = CheckMDMinOff.toInt();} else {DisplayOffTime = F("!");}
    if (DisplayOffTime != "" && DisplayOffTime != "!") {
    for (int i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");} else {   
    MDTime = false;    
    for (int i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");}}}}}

    if (DisplayOnTime == "" && DisplayOffTime == "") {
    DisplayOnTime = F("NullOn");
    DisplayOffTime = F("NullOff");}
	
	// Si MidnightGhost s'éteint ensuite une Minute à 00:00
    if (MidnightGhost && DisplayOffTime == "00:00") {
    DisplayOffTime = "00:01";}

    FDOnTime =  DisplayOnTime; // pour l'affichage uniquement
    FDOffTime = DisplayOffTime; // pour l'affichage uniquement

    // Si erreur de saisie
    if (DisplayOnTime == "!" || DisplayOffTime == "!" || DisplayOnTime == DisplayOffTime) {
    if (DisplayOnTime == "!") {DisplayOnTime = F("");} 
    if (DisplayOffTime == "!") {DisplayOffTime = F("");}  
    HTMLString = F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    HTMLString += F("<h5>(Affichage Matriciel LED Automatiquement MARCHE/ARRÊT)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Entrée(s) Invalide(s) - Veuillez Corriger !</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/cdof\"><button>Retour à la Page d'Accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    } else {
    if (DisplayOnTime == "NullOn") {DisplayOnTime = F("");}
    if (DisplayOffTime == "NullOff") {DisplayOffTime = F("");}  
    if (littleFSInit){
    // *** Enregistrer les entrées dans le système de fichiers ***
    // Affichage matriciel de l'heure de MARCHE
    if (DisplayOnTime != ""){
    SetupSave("display-on-time.txt", DisplayOnTime);} else {
    if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
    // Affichage matriciel Heure d'ARRÊT
    if (DisplayOffTime != ""){
    SetupSave("display-off-time.txt", DisplayOffTime);} else {
    if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
    SetupSave("display-change-time.txt", "True"); // Affichage marche/arrêt
    MyWaitLoop(500); // 0,5 secondes. attendez 
    if (SerialMonitor) {Serial.println(F("Affichage automatique MARCHE/ARRÊT Données Enregistrées..."));} 
      
    HTMLString = F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    HTMLString += F("<h5>(Affichage Matriciel LED Automatiquement MARCHE/ARRÊT)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les Paramètres ont été Enregistrés<br /><br />L'Horloge Redémarrera lorsque vous Reviendrez à la Page d'accueil</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>Retour à la Page d'Accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    // Déclencher le redémarrage en revenant à la page d'accueil
    RebootNow = true;
    }
     else
     { // Les données d'accès n'ont pas été enregistrées
     if (SerialMonitor) {Serial.println(F("Les données d'Accès et les Paramètres n'ont pas été Enregistrés !"));}
    HTMLString =  F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");   
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    HTMLString += F("<h5>(Affichage Matriciel LED Automatiquement MARCHE/ARRÊT)</h5>");
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Erreur lors de l'Enregistrement des données d'Accès et des Paramètres<br />L'Horloge est Redémarrée</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>Retour à la Page d'Accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    // Redémarrer le déclencheur
    if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Allumer l'affichage lorsqu'il est éteint
    ct = cleanText("        +++       L'Horloge Redémarrera      +++           ");
    printStringWithShift(ct.c_str(),ScrollTextTime);
    digitalWrite(HWLEDPin, LOW); // LED éteinte
    ESP.restart();  
    } 
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    MyWaitLoop(100);
    CheckInHumidity(); // Mesure de l'humidité intérieure
    } // Fin de l'erreur de saisie
  } else {  
    // Settings
    HTMLString =  F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/cdof' method='post'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    HTMLString += F("<h5>(Affichage Matriciel LED automatiquement MARCHE/ARRÊT)</h5>");
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display MARCHE-Time (Example: 07:00)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOnT' id='DOnT' value='"+FDOnTime+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Matrix Display ARRÊT-Time (Example: 23:30)   -Optional*-</label><input type='text' maxlength='5' class='form-control' name='DOffT' id='DOffT' value='"+FDOffTime+"'></div>";
    // ******************************************* Parameter ************************************************************************************************************************************
    HTMLString += F("<h6>*) - Aucune Information désactive la Fonction</h6>");
    HTMLString += F("<button type='submit'>Sauvegarder</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/cdofc\"><button>Annuler</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);
    MyWaitLoop(100);}} else {PleaseWait();}}  // Envoyer une chaîne HTML or wait

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DisplayCheck() {
if (!await) {
await = true; 
char  MDC   = '9' + 2; // Affichage complet MARCHE
String MDCShow = "      "+String(MDC)+String(MDC)+String(MDC)+String(MDC);
HTMLAnswer("DisplayCheck");
printStringWithShift3(MDCShow.c_str(),ScrollTextTime);
HandleOTAandWebServer();  // Requête OTA et serveur Web
digitalWrite(HWLEDPin, HIGH); // Vérifier la LED
MyWaitLoop(8000); // Courte attente (8 secondes)
HandleOTAandWebServer();  // Requête OTA et serveur Web
printStringWithShift3("      ",ScrollTextTime);
HandleOTAandWebServer();  // Requête OTA et serveur Web
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") {
digitalWrite(HWLEDPin, LOW);}} else {digitalWrite(HWLEDPin, LOW);}
ClockScrollIn(); // Faites défiler l'heure sur l'écran 
await = false; 
} else {HTMLAnswer("NoDisplayCheck");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ChangeDisplayAutoOnOffCancel() {
String HTMLString = F("");
  // Afficher l'heure d'allumage sous forme de chaîne
  if (littleFSInit) {
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOnTime = F("");}} else
         {DisplayOnTime = F("");}
  if (littleFSInit) {
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOffTime = F("");}} else
         {DisplayOffTime = F("");}

  // Vérifier la plausibilité des informations pour affichage matriciel marche/arrêt automatique
  CheckLEDMatrixDisplayOnOff();

    HTMLString = F("<!doctype html><html lang='fr'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    HTMLString += F("<h5>(LED Matrix Display automatically MARCHE/ARRÊT)</h5>");
    HTMLString += F("<br/>");
    if (littleFSInit) {
    HTMLString += F("<h6>Annulation! - L'Ancien Paramètre a été à nouveau Chargé...</h6>");} else {
    HTMLString += F("<h6>Annulation! - L'Ancien paramètre n'a pas pu être Chargé !!! ...</h6>");}
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/\"><button>Retour à la Page d'Accueil</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    CheckInHumidity(); // Mesure de l'humidité intérieure
 HTMLString = F("");}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOn() {
if (StartSound != "On") { 
if (!await) {  
await = true;
StartBuzzerBeep();    
if (littleFSInit) {
StartSound = F("On");   
SetupSave("status_startsound.txt", StartSound);} else {StartSound = F("Off");}
HTMLAnswer("StartSoundOnM");
if (SerialMonitor) {Serial.println(F("Le Buzzer Son de démarrage activé"));}
await = false;} else {PleaseWait();}} else {HTMLAnswer("StartSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOff() {
if (StartSound != "Off") {  
StartSound = F("Off");     
if (littleFSInit) {  
SetupSave("status_startsound.txt", StartSound);} 
HTMLAnswer("StartSoundOffM");
if (SerialMonitor) {Serial.println(F("Le Buzzer Son de démarrage désactivé"));}
} else {HTMLAnswer("StartSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOn() {
if (HourSound != "On") { 
if (!await) {  
await = true;
HourBuzzerBeep();   
if (littleFSInit) {
HourSound = F("On");   
SetupSave("status_hoursound.txt", HourSound);} else {HourSound = F("Off");}
HTMLAnswer("HourSoundOnM");
await = false;
if (SerialMonitor) {Serial.println(F("Le Buzzer horaire activé"));}} else {PleaseWait();}
} else {HTMLAnswer("HourSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOff() {
if (HourSound != "Off") {
HourSound = F("Off");  
if (littleFSInit) {
SetupSave("status_hoursound.txt", HourSound);} 
HTMLAnswer("HourSoundOffM");
if (SerialMonitor) {Serial.println(F("Le Buzzer horaire est désactivé"));}
} else {HTMLAnswer("HourSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PleaseWait() {
// Veuillez patienter, afficher la page
String RefreshTime = F("5"); // Actualiser le site Web après X secondes
String HTMLString;
HTMLString = F("<!DOCTYPE html><html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
// HTMLString += F("<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 { font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 { font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 { font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>S'il vous Plaît, Attendez ...</h2>");
if (Display) {
HTMLString += F("<h3>(Jusqu'à ce que l'heure s'Affiche à Nouveau sur l'affichage Matriciel)</h3>");}
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString); 
CheckInHumidity(); // Mesure de l'humidité intérieure 
Serial.println(F("S'il vous plaît, attendez ..."));}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GoToWebSiteHomePage() { // Redirection automatique vers la Page d'Accueil
String HTMLString;
HTMLString = F("<!DOCTYPE html><html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 { font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 { font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 { font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("</style>");
HTMLString += F("<script>");
// Temps d'attente (ms) jusqu'à ce que vous reveniez à la page d'accueil | par défaut = 1000
HTMLString += F("window.onload = function() {setTimeout(function() {window.location.href = \"/\";}, 2000);}"); 
HTMLString += F("</script>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>S'il vous Plaît, Attendez ...</h2>");
if (Display) {
HTMLString += F("<h3>(Jusqu'à ce que l'heure s'Affiche à Nouveau sur l'affichage Matriciel)</h3>");}
HTMLString += F("</body>");
HTMLString += F("</html>");
server.send(200, "text/html", HTMLString);}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HTTPCodeText(int httpcode) {
String TempHTTPCodeText = F("k.A.");  
if (httpcode == -1) {TempHTTPCodeText = F("ERROR_CONNECTION_REFUSED");}  
if (httpcode == -2) {TempHTTPCodeText = F("ERROR_SEND_HEADER_FAILED");} 
if (httpcode == -3) {TempHTTPCodeText = F("ERROR_SEND_PAYLOAD_FAILED");} 
if (httpcode == -4) {TempHTTPCodeText = F("ERROR_NOT_CONNECTED");} 
if (httpcode == -5) {TempHTTPCodeText = F("ERROR_CONNECTION_LOST");} 
if (httpcode == -6) {TempHTTPCodeText = F("ERROR_NO_STREAM");} 
if (httpcode == -7) {TempHTTPCodeText = F("ERROR_NO_HTTP_SERVER");} 
if (httpcode == -8) {TempHTTPCodeText = F("ERROR_TOO_LESS_RAM");} 
if (httpcode == -9) {TempHTTPCodeText = F("ERROR_ENCODING");} 
if (httpcode == -10) {TempHTTPCodeText = F("ERROR_STREAM_WRITE");} 
if (httpcode == -11) {TempHTTPCodeText = F("ERROR_READ_TIMEOUT");}

if (httpcode == 100) {TempHTTPCodeText = F("Continue");}
if (httpcode == 101) {TempHTTPCodeText = F("Switching Protocols");}
if (httpcode == 102) {TempHTTPCodeText = F("Processing (WebDAV)");}
if (httpcode == 200) {TempHTTPCodeText = F("OK");}
if (httpcode == 201) {TempHTTPCodeText = F("Created");}
if (httpcode == 202) {TempHTTPCodeText = F("Accepted");}
if (httpcode == 203) {TempHTTPCodeText = F("Non-Authoritative Information");}
if (httpcode == 204) {TempHTTPCodeText = F("No Content");}
if (httpcode == 205) {TempHTTPCodeText = F("Reset Content");}
if (httpcode == 206) {TempHTTPCodeText = F("Partial Content");}
if (httpcode == 207) {TempHTTPCodeText = F("Multi-Status (WebDAV)");}
if (httpcode == 208) {TempHTTPCodeText = F("Already Reported (WebDAV)");}
if (httpcode == 226) {TempHTTPCodeText = F("IM Used");}

if (httpcode == 300) {TempHTTPCodeText = F("Multiple Choices");}
if (httpcode == 301) {TempHTTPCodeText = F("Moved Permanently");}
if (httpcode == 302) {TempHTTPCodeText = F("Found");}
if (httpcode == 303) {TempHTTPCodeText = F("See Other");}
if (httpcode == 304) {TempHTTPCodeText = F("Not Modified");}
if (httpcode == 305) {TempHTTPCodeText = F("Use Proxy");}
if (httpcode == 306) {TempHTTPCodeText = F("(Unused)");}
if (httpcode == 307) {TempHTTPCodeText = F("Temporary Redirect");}
if (httpcode == 308) {TempHTTPCodeText = F("Permanent Redirect (experimental)");}

if (httpcode == 400) {TempHTTPCodeText = F("Bad Request");}
if (httpcode == 401) {TempHTTPCodeText = F("Unauthorized");}
if (httpcode == 402) {TempHTTPCodeText = F("Payment Required");}
if (httpcode == 403) {TempHTTPCodeText = F("Forbidden");}
if (httpcode == 404) {TempHTTPCodeText = F("Not Found");}
if (httpcode == 405) {TempHTTPCodeText = F("Method Not Allowed");}
if (httpcode == 406) {TempHTTPCodeText = F("Not Acceptable");}
if (httpcode == 407) {TempHTTPCodeText = F("Proxy Authentication Required");}
if (httpcode == 408) {TempHTTPCodeText = F("Request Timeout");}
if (httpcode == 409) {TempHTTPCodeText = F("Conflict");}
if (httpcode == 410) {TempHTTPCodeText = F("Gone");}
if (httpcode == 411) {TempHTTPCodeText = F("Length Required");}
if (httpcode == 412) {TempHTTPCodeText = F("Precondition Failed");}
if (httpcode == 413) {TempHTTPCodeText = F("Request Entity Too Large");}
if (httpcode == 414) {TempHTTPCodeText = F("Request-URI Too Long");}
if (httpcode == 415) {TempHTTPCodeText = F("Unsupported Media Type");}
if (httpcode == 416) {TempHTTPCodeText = F("Requested Range Not Satisfiable");}
if (httpcode == 417) {TempHTTPCodeText = F("Expectation Failed");}
if (httpcode == 418) {TempHTTPCodeText = F("I'm a teapot (RFC 2324)");}
if (httpcode == 420) {TempHTTPCodeText = F("Enhance Your Calm (Twitter)");}
if (httpcode == 422) {TempHTTPCodeText = F("Unprocessable Entity (WebDAV)");}
if (httpcode == 423) {TempHTTPCodeText = F("Locked (WebDAV)");}
if (httpcode == 424) {TempHTTPCodeText = F("Failed Dependency (WebDAV)");}
if (httpcode == 425) {TempHTTPCodeText = F("Reserved for WebDAV");}
if (httpcode == 426) {TempHTTPCodeText = F("Upgrade Required");}
if (httpcode == 428) {TempHTTPCodeText = F("Precondition Required");}
if (httpcode == 429) {TempHTTPCodeText = F("Too Many Requests");}
if (httpcode == 431) {TempHTTPCodeText = F("Request Header Fields Too Large");}
if (httpcode == 444) {TempHTTPCodeText = F("No Response (Nginx)");}
if (httpcode == 449) {TempHTTPCodeText = F("Retry With (Microsoft)");}
if (httpcode == 450) {TempHTTPCodeText = F("Blocked by Windows Parental Controls (Microsoft)");}
if (httpcode == 451) {TempHTTPCodeText = F("Unavailable For Legal Reasons");}
if (httpcode == 499) {TempHTTPCodeText = F("Client Closed Request (Nginx)");}

if (httpcode == 500) {TempHTTPCodeText = F("Internal Server Error");}
if (httpcode == 501) {TempHTTPCodeText = F("Not Implemented");}
if (httpcode == 502) {TempHTTPCodeText = F("Bad Gateway");}
if (httpcode == 503) {TempHTTPCodeText = F("Service Unavailable");}
if (httpcode == 504) {TempHTTPCodeText = F("Gateway Timeout");}
if (httpcode == 505) {TempHTTPCodeText = F("HTTP Version Not Supported");}
if (httpcode == 506) {TempHTTPCodeText = F("Variant Also Negotiates (Experimental)");}
if (httpcode == 507) {TempHTTPCodeText = F("Insufficient Storage (WebDAV)");}
if (httpcode == 508) {TempHTTPCodeText = F("Loop Detected (WebDAV)");}
if (httpcode == 509) {TempHTTPCodeText = F("Bandwidth Limit Exceeded (Apache)");}
if (httpcode == 510) {TempHTTPCodeText = F("Not Extended");}
if (httpcode == 511) {TempHTTPCodeText = F("Network Authentication Required");}
if (httpcode == 598) {TempHTTPCodeText = F("Network read timeout error");}
if (httpcode == 599) {TempHTTPCodeText = F("Network connect timeout error");}
return String(httpcode) + " - " + TempHTTPCodeText;}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
