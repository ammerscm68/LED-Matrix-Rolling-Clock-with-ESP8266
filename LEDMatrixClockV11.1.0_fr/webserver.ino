// Page d'accueil du Serveur Web
void handleRoot() {  
// envoyer des données
if (!await){
IsEnoughRAM(MinRAMMem,"Website"); // Vérifier la RAM
if (!OneTimeCall) {if (ESPRAM < MinRAMMem-256) {
OnlyReboot = true; ResetClockSettings(); OnlyReboot = false;}} // Redémarrez simplement au lieu de réinitialiser
server.setContentLength(CONTENT_LENGTH_UNKNOWN);  
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
MyWaitLoop(100); // petite attente pour que la mémoire puisse être vidée
server.client().stop();
// freeRam();
if (RebootNow) {
if (!Display) {
printStringWithShift("           ",ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN,1); // Afficher sur  
} // Allumer l'affichage lorsqu'il est éteint 
printStringWithShift(cleanText("                  +++       L'Horloge va Redémarrer      +++           ").c_str(),ScrollTextTime); 
StatusWarningLED("LOW"); // LED éteinte
ESP.restart();} // Redémarrer l'Horloge
if (littleFSInit && ResetNow) {ResetClock();}} else { // Paramètres sur Standard - Les données d'accès WLAN seront supprimées
HTMLAnswer("PW");}} 

/********************************************************************************************************************************************************/

String RootHTMLString_1(){
/* &nbsp = Space | <meta http-equiv=\"refresh\" content=\"60\">\ = Refresh all 60 sec*/ 
String RefreshTime = F("60"); // Actualiser le site Web après X secondes
  String HTMLString; HTMLString.reserve(220);
   String InTempOnOff1;
    String InTempOnOff2;

    InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure

  // Afficher la Température intérieure OUI/NON (DHT11 or DHT22)
  if (InTempCounter < 5) {
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Mesure de Température 
  if (InTempValue != "!") {
  IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10);
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
  InTempOnOff2 += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  } else {InTempOnOff1.clear();
          InTempOnOff1.clear();}
  } else {InTempOnOff1.clear();
          InTempOnOff2.clear();
          InTempValue = F("!");}

  // Vérifier la sous-chaîne HTML
 //  if (SerialMonitor) {DEBUG_PRINTLN("<a"+InTempOnOff2+"</a>\");} // Uniquement pour le débogage
// 1. Une partie de la chaîne HTML
  HTMLString = F("<!DOCTYPE html><html lang='fr'>"); 
  HTMLString += F("<head>");
  HTMLString += F("<title>Horloge-Matricielle-LED</title>");
  HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body {background-color: #585858; font-size: 28px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
  HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}");
  HTMLString += F("h2 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h3 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h4 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h5 {color: white;font-size: 22px; margin-left: 1px; margin-top: 0px; margin-bottom: 10px;}");
  HTMLString += F("h6 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}");
  HTMLString += F("h7 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffffff;}");
  HTMLString += F("h8 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #f74f4f; font-weight: bold;}");
  HTMLString += F("h9 {font-size: 18px; margin-left: 80px; margin-top: 0px; margin-bottom: 0px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("h10 {font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #ffcc00;}");
  HTMLString += F("h11 {font-size: 14px; margin-left: 10px; color: #f79d36; font-weight: bold;}");
  HTMLString += F("#Rvalue {color: #FC3A30;}");
  HTMLString += F("#Gvalue {color: lime;}");
  HTMLString += F("#Bvalue {color: #2ECCFA;}");
  HTMLString += F("</style>");
  HTMLString += F("</head>");
  HTMLString += F("<body>");
  HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  HTMLString += "<h5 "+InTempOnOff1+"</h5>";
  HTMLString += InTempOnOff2;
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_2(){ 
// Sélection de la luminosité de la matrice et du temps d'exécution de l'Horloge
  String HTMLString; HTMLString.reserve(220);
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
String HTMLString; HTMLString.reserve(220);
String DisplayAutomaticOnOff;  
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
if (!MatrixAction && Display) {HTMLString += F("<h11>[ Tant que l'écran ne s'éteint pas automatiquement, seule l'heure sera affichée ! ]</h11>");}
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}  

/********************************************************************************************************************************************************/

String RootHTMLString_4(){ // Statut Côlon
  String HTMLString; HTMLString.reserve(220);
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
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_5(){ // Niveau de vitesse du texte de défilement
String HTMLString; HTMLString.reserve(220);
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
  HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
  return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_6(){ // Fonctions Supplémentaires
String HTMLString; HTMLString.reserve(220);
String EF;
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_7() { // Afficher les données météorologiques d'OpenWeatherMap
// Activer/désactiver les données météorologiques
String HTMLString; HTMLString.reserve(220);
String OWMA;
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/

String RootHTMLString_8() { // Afficher les actualités NewsData.io
String HTMLString; HTMLString.reserve(220);
String ANA;
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/ 

String RootHTMLString_9() { // Afficher www.gmcmap.com
String HTMLString; HTMLString.reserve(220);
String ARA;
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_10() { // Paramètres du buzzer piézo passif
String HTMLString; HTMLString.reserve(220);
String BSS;
String BHS;
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_11() { // Paramètres du portail Captiv
String HTMLString; HTMLString.reserve(220);
// 11. Une partie de la chaîne HTML  
HTMLString = F("<h2>Affichage - Paramètres du Point d'Accès (Portail-Captif):</h2>");
HTMLString += F("<a href =\"/cpadjust\"><button class=\"button\" style=\"margin-right: 10px; height: 26px; width: 110px;\">Paramètres</button></a>");
HTMLString += F("<p></p>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
return HTMLString;}

/********************************************************************************************************************************************************/  

String RootHTMLString_12() { // Version du Logiciel
String HTMLString; HTMLString.reserve(220);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ESPRAM = ESP.getMaxFreeBlockSize() / 1024;  // Vérifier la RAM
String ESPCore = ESP.getCoreVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Core Version
// String ESPCore = ESP.getFullVersion() + "&nbsp; ("+BoardName+")"; // ESP8266 Full Version
uint8_t CPUClock = ESP.getCpuFreqMHz(); // CPU-Clock
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 12. Une partie de la chaîne HTML   
HTMLString = "<h7>Version du Logiciel: &nbsp;"+String(LMCVersion)+"</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Version du Matériel: &nbsp;"+ESPCore+"</h7>";
HTMLString += F("<a><br /></a>");
if (ESPRAM > MinRAMMem-128) {
HTMLString += "<h7>Mémoire RAM Libre: &nbsp;&nbsp;"+String(ESPRAM)+" Byte / 49152 Byte</h7>";} else {
HTMLString += "<h8>Mémoire RAM Libre: &nbsp;&nbsp;"+String(ESPRAM)+" Byte / 49152 Byte (Danger ! - trop peu)</h8>";}
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Fréquence d'Horloge du Processeur: &nbsp;&nbsp;"+String(CPUClock)+"&nbsp; MHz</h7>";
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Date de Début: &nbsp;&nbsp;"+StartDate+"</h7>";
HTMLString += F("<a><br /></a>");
if (ClockStatus == "OK") {
HTMLString += F("<h7>Statut du dernier Démarrage: &nbsp;&nbsp;OK</h7>");} else {
HTMLString += "<h10>Statut du dernier Démarrage: &nbsp;&nbsp;"+ClockStatus+"</h10>";}
HTMLString += F("<a><br /></a>");
HTMLString += WiFiSignalStrength(false); // Intensité du Signal WiFi en dBm
HTMLString += F("<a><br /></a>");
HTMLString += "<h7>Durée d'exécution de l'Horloge: &nbsp;&nbsp;"+rtime+"</h7>";
HTMLString += F("<a href =\"/reset\"><button class=\"button\" style=\"margin-right: 20px; height: 48px; width: 140px;color: red;float: right; font-size: 20px;\">Réinitialiser !</button></a>");
HTMLString += F("<a href =\"/reboot\"><button class=\"button\" style=\"margin-right: 30px; height: 48px; width: 130px;color: blue;float: right; font-size: 18px;\">Redémarrage</button></a>");
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-right: 60px; height: 48px; width: 140px;color: navy;float: right; font-size: 18px;\">Rafraîchir</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h6>*) - Sélection actuelle</h6>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
return HTMLString;}

/********************************************************************************************************************************************************/ 

void HTMLAnswer(String WebFunction) {
String HTMLString; HTMLString.reserve(220);
String HTMLAnswerString; OneTimeCall = false; 
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
if (WebFunction == "NewsDataFunctionON1")  {HTMLAnswerString = F("NewsData.io - Actualités actuelles a été Commuté ALLUMÉ");} 
if (WebFunction == "NewsDataFunctionON2")  {HTMLAnswerString = F("NewsData.io - Actualités actuelles est déjà Commuté ALLUMÉ");}
if (WebFunction == "NewsDataFunctionOFF")   {HTMLAnswerString = F("NewsData.io - Actualités Actuelles a été Commuté DÉSACTIVÉ");}
if (WebFunction == "GMCMapFunctionON1")  {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels ont été activés");}  
if (WebFunction == "GMCMapFunctionON2")  {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels sont déjà activés");}
if (WebFunction == "GMCMapFunctionOFF")   {HTMLAnswerString = F("GMCMap.com - Les Niveaux de Radioactivité actuels ont été désactivés");}
if (WebFunction == "EnhancedFunctionsON1") {HTMLAnswerString = F("Les Fonctions Supplémentaires ont été activées<br />(Date, Anniversaires, Jours Fériés)");}
if (WebFunction == "EnhancedFunctionsON2") {HTMLAnswerString = F("The additional Functions are already switched ALLUMÉ<br />(Date, Anniversaires, Jours Fériés)");}
if (WebFunction == "EnhancedFunctionsOFF") {HTMLAnswerString = F("The additional Functions have been switched DÉSACTIVÉ<br />(Date, Anniversaires, Jours Fériés)");}
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
if (WebFunction == "PW") {HTMLAnswerString = F("PleaseWait");}
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString +=  F("<head>");
HTMLString +=  F("<title>Horloge-Matricielle-LED</title>");
HTMLString +=  F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString +=  F("<style>");
HTMLString +=  F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; text-align: center;}");
HTMLString +=  F("h1 {color: #2ECCFA; margin-top: 30px;  margin-bottom: 0px;}");
HTMLString +=  F("h2 {font-size: 26px; margin-top: 0px;  margin-bottom: 50px;}");
HTMLString +=  F("h3 {font-size: 36px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString +=  F("h4 {font-size: 48px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString +=  F("</style>");
HTMLString +=  F("</head>");
HTMLString +=  F("<body>");
HTMLString +=  F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString +=  F("<a><br /></a>");
if (HTMLAnswerString == "PleaseWait") {
HTMLString += F("<h4>S'il vous Plaît, Attendez ...</h4>");
if (Display) {HTMLString += F("<h3>(Jusqu'à ce que l'heure s'Affiche à Nouveau sur l'affichage Matriciel)</h3>");}
} else {HTMLString += "<h2>"+HTMLAnswerString+"</h2>";}
HTMLString +=  F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
HTMLString +=  F("</body>");
HTMLString +=  F("</html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString.clear();}

/********************************************************************************************************************************************************/

void ActualWeather() {
String HTMLString; HTMLString.reserve(220); 
if (!await) { 
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 50px;}");
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
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString.clear();} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void ActualAPINews() {
String HTMLString; HTMLString.reserve(220);  
if (!await) {
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 50px;}");
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
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString.clear();} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void ActualGMCMapData() {
String HTMLString; HTMLString.reserve(220); 
if (!await){   
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px;}");
HTMLString += F("h2 {font-size: 22px; margin-bottom: 50px;}");
HTMLString += F("h3 {font-size: 16px; margin-bottom: 30px;}");
HTMLString += F("h4 {font-size: 16px; color: #ffcc00; margin-bottom: 30px;}");
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
HTMLString.clear();} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

// Matrix On
void MatrixOn() {  
if (!await){  
if (!Display) {
await = true;
HTMLAnswer("MatrixOn");
if (SerialMonitor) {DEBUG_PRINTLN(F("La Matrice LED était Allumée."));}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Display on
printStringWithShift("           ",ScrollTextTime); 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime); 
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
if (InHumidityStatus != "FAIL") {
if (InHumidityStatus == "MIN" || InHumidityStatus == "MAX") {
digitalWrite(HWLEDPin, HIGH);}}  // LED allumée
ClockScrollIn(); // L'horloge défile
MyWaitLoop(100); // courte attente
Display = true; await = false;} else {HTMLAnswer("MatrixIsOn");}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

// Matrix Off
void MatrixOff() { 
if (!await){  
if (Display) { 
await = true;   
dots = 1; // Côlon sur
Display = false;   
HTMLAnswer("MatrixOff"); 
if (SerialMonitor) {DEBUG_PRINTLN(F("La Matrice LED est éteinte."));} 
if (WatchOwner1FirstName != "") { 
SText = Politeness(true);} else {SText = Politeness(false);}
printStringWithShift(SText.c_str(),ScrollTextTime);
printStringWithShift("           ",ScrollTextTime);  
sendCmdAll(CMD_SHUTDOWN,0);
digitalWrite(HWLEDPin, LOW); // LED éteinte
await = false;} else {HTMLAnswer("MatrixIsOff");} 
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void DoublePointFlash() {
if (!await){      
DoublePointStatus = 2; // Faire clignoter le Côlon de l'Horloge
// Statut Côlon
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointFlash");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Côlon de l'Horloge clignote."));}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void DoublePointOn() {
if (!await){  
DoublePointStatus = 1; // Le Côlon de l'horloge est en permanence MARCHE
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointOn");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Côlon de l'Horloge est en permanence MARCHE"));}
} else {HTMLAnswer("PW");}}


/********************************************************************************************************************************************************/

void DoublePointOff() {
if (!await){  
DoublePointStatus = 0; // Le Côlon de l'horloge est ARRÊTÉ en permanence
if (littleFSInit) {
SetupSave(F("status_doublepoint.txt"), String(DoublePointStatus));}
HTMLAnswer("DoublePointOff");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Côlon de l'Horloge est ARRÊTÉ en permanence"));}
} else {HTMLAnswer("PW");}}

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

void SetBrightnessLevel(String BLText, byte BLInt) {
if (!await){
sendCmdAll(CMD_INTENSITY, BLInt-1);
Brightness = BLText;
if (littleFSInit) {
SetupSave(F("matrix_brightness.txt"), Brightness);}
HTMLAnswer("BrightnessLevel"+String(BLInt));
if (SerialMonitor) {DEBUG_PRINTLN("Luminosité Niveau "+String(BLInt)+" activé");}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

// Ajuster la vitesse du texte défilant
void ScrollTextTimeLevel1() {
SetScrollTimeAndLevel(60,1);}

/********************************************************************************************************************************************************/

void ScrollTextTimeLevel2() { 
SetScrollTimeAndLevel(55,2);}

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

void SetScrollTimeAndLevel(byte STTime, byte STLevel){
if (!await) {
await = true;   
ScrollTextTime = STTime; // Vitesse de défilement du texte 
if (littleFSInit) {
SetupSave(F("status_scrolltexttime.txt"), String(STTime));}
HTMLAnswer("ScrollTextTimeLevel"+String(STLevel)+"");
if (SerialMonitor) {DEBUG_PRINTLN("Niveau de Vitesse du Texte défilant "+String(STLevel)+" activé");}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);
printStringWithShift(cleanText("        +++  Il s'agit d'un Test de Vitesse de défilement du Texte  +++           ").c_str(),ScrollTextTime);
sendCmdAll(CMD_SHUTDOWN, 0);} else {
printStringWithShift(cleanText("        +++  Il s'agit d'un Test de Vitesse de défilement du Texte  +++           ").c_str(),ScrollTextTime);
ClockScrollIn();} // Scroll the Time in the Display 
await = false;} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOn() {
if (!await) {  
if (!WeatherFunctions) {
WeatherFunctions = true;
OWMAdjustChange = true;
StatusOpenWeatherMap = F("enabled");
if (littleFSInit) {
SetupSave(F("status_weatherfunctions.txt"), "On");} 
HTMLAnswer("WeatherFunctionsON1");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Données Météo activées"));}}
else {
HTMLAnswer("WeatherFunctionsON2");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Les données Météorologiques sont déjà activées"));}}   
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void WeatherFunctionsOff() {
if (!await) {   
WeatherFunctions = false;
StatusOpenWeatherMap = F("disabled");
if (littleFSInit) {
SetupSave(F("status_weatherfunctions.txt"), "Off");} 
HTMLAnswer("WeatherFunctionsOFF");
if (SerialMonitor) {DEBUG_PRINTLN(F("OpenWeatherMap - Données Météo désactivées"));}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOn() {
if (!await) { 
if (!NewsDataFunction) {        
NewsDataFunction = true;
NAAdjustChange = true;
StatusNewsData = F("enabled");
if (littleFSInit) {
SetupSave(F("status_NewsDatafunctions.txt"), "On");}  
if (SerialMonitor) {
DEBUG_PRINTLN(F("NewsData - Les actualités actuelles ont été activées"));
if (AccessGMCMap && AccessNewsData) {DEBUG_PRINTLN(F("+++   L'Horloge redémarrera   +++"));}}
HTMLAnswer("NewsDataFunctionON1");} else {
HTMLAnswer("NewsDataFunctionON2");
if (SerialMonitor) {DEBUG_PRINTLN(F("NewsData - Les actualités actuelles sont déjà activées"));}} 
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void NewsDataFunctionOff() {
if (!await) {   
NewsDataFunction = false;
StatusNewsData = F("disabled");
if (littleFSInit) {
SetupSave(F("status_NewsDatafunctions.txt"), "Off");}  
HTMLAnswer("NewsDataFunctionOFF");
if (SerialMonitor) {DEBUG_PRINTLN(F("NewsData - Les actualités actuelles sont désactivées"));}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOn() {
if (!await) { 
if (!GMCMapFunction) {     
GMCMapFunction = true;
GMCMapAdjustChange = true;
StatusGMCMap = F("enabled");
if (littleFSInit) {
SetupSave(F("status_GMCMapFunction.txt"), "On");}  
if (SerialMonitor) {
DEBUG_PRINTLN(F("GMCMap - Les Niveaux de Radioactivité actuels ont été activés"));
if (AccessGMCMap && AccessNewsData) {DEBUG_PRINTLN(F("+++   L'Horloge redémarrera   +++"));}}
HTMLAnswer("GMCMapFunctionON1");} else {
HTMLAnswer("GMCMapFunctionON2");
if (SerialMonitor) {DEBUG_PRINTLN(F("GMCMap - Les Niveaux de Radioactivité actuels sont déjà activés"));}} 
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void GMCMapFunctionOff() {
if (!await) {   
GMCMapValue = "";
GMCMapToHigh == "";     
GMCMapFunction = false;
StatusGMCMap = F("disabled");
StatusWarningLED("LOW");
if (littleFSInit) {
SetupSave(F("status_GMCMapFunction.txt"), "Off");}  
HTMLAnswer("GMCMapFunctionOFF");
if (SerialMonitor) {DEBUG_PRINTLN(F("GMCMap - Affichage actuel de la Radioactivité éteint"));}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOn() {
if (!await) {   
if (!EnhancedFunctions) {
EnhancedFunctions = true;
if (littleFSInit) {
SetupSave(F("status_enhancedfunctions.txt"), "On");} 
HTMLAnswer("EnhancedFunctionsON1");} else {
HTMLAnswer("EnhancedFunctionsON2");}  
if (SerialMonitor) {DEBUG_PRINTLN(F("Fonctions supplémentaires activées - (Date, Anniversaires, Jours Fériés)"));} 
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void EnhancedFunctionsOff() {
if (!await) {   
EnhancedFunctions = false;
if (littleFSInit) {
SetupSave(F("status_enhancedfunctions.txt"), "Off");} 
HTMLAnswer("EnhancedFunctionsOFF");
if (SerialMonitor) {DEBUG_PRINTLN(F("Fonctions Supplémentaires désactivées - (Date, Anniversaires, Jours Fériés)"));}
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void GetAdjustments() {
String OWMAPI, OWMCID, NewsDataKey, CheckIP, HTMLString;
HTMLString.reserve(220);
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
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}");
HTMLString += F("h2 {font-size: 18px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("h3 {font-size: 24px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("h4 {font-size: 14px; margin-top: 0px; margin-bottom: 1px; color: #ffcc00; float: right;}");
HTMLString += F("h5 {color: white; font-size: 16px; margin-top: 0px; margin-bottom: 10px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
if (WatchOwner1FirstName != "" && WatchOwner1LastName != "") {
HTMLString += "<h2>( Propriétaire de l'Horloge:&nbsp;&nbsp;"+WatchOwner1FirstName+" "+WatchOwner1LastName+" )</h2>";}
HTMLString += F("<a><br /></a>");
HTMLString += F("<h3>Paramètres du Portail Captif</h3>");
HTMLString += F("<a><br /></a>");
HTMLString += "<h2>WLAN SSID:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+String(ssid)+"</h2>";
HTMLString += "<h2>Adresse IP:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+CheckIP+"</h2>";
HTMLString += "<h2>Serveur NTP:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+NTPSN+"</h2>";
HTMLString += "<h2>Fuseau Horaire:&nbsp;&nbsp;"+MCTZ+"</h2>";
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
HTMLString += "<h2>GMCMap.com --- Décalage-GMT:&nbsp;&nbsp;&nbsp;&nbsp;"+String(utco)+"</h2>";
HTMLString += F("<a><br /></a>");}
if (IsPushover) {
HTMLString += "<h2>Pushover-Jeton d'Application sécurisé:  &nbsp;&nbsp;"+SecureAppToken+"</h2>";
HTMLString += "<h2>Pushover-Jeton Utilisateur:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+UserToken+"</h2>";
HTMLString += "<h2>Pushover-Nom de l'Appareil1:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice1+"</h2>";
if (PushoverDevice2 != "") { 
HTMLString += "<h2>Pushover-Nom de l'Appareil2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice2+"</h2>";} else {
HTMLString += F("<h2>Pushover-Nom de l'Appareil2:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>");}  
if (PushoverDevice3 != "") {
HTMLString += "<h2>Pushover-Nom de l'Appareil3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverDevice3+"</h2>";} else {
HTMLString += F("<h2>Pushover-Nom de l'Appareil3:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;n/a</h2>");}  
HTMLString += "<h2>Pushover-Message Sonore:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+PushoverMessageSound+"</h2>";
HTMLString += F("<a><br /></a>");}
if (MidnightGhost == "OUI") {HTMLString += F("<h2>Afficher les Fantômes à Minuit ?:  &nbspOUI</h2>");} else {HTMLString += F("<h2>Afficher les Fantômes à Minuit ?:  &nbspNON</h2>");}
HTMLString += F("<a><br /></a>");
if (InTempOffset == "999") {HTMLString += F("<h2>Capteur de Température et d'Humidité ?:  &nbspdésactivé</h2>");} else {
if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {HTMLString += F("<h2>Capteur de Température et d'Humidité ?:  &nbspNON</h2>");} else {
if (DHTSensor == "DHT11") {HTMLString += F("<h2>Capteur de Température et d'Humidité ?:  &nbspDHT11</h2>");} else {HTMLString += F("<h2>Capteur de Température et d'Humidité ?:  &nbspDHT22</h2>");}}}
HTMLString += F("<a><br /></a>");
if (IsBuzzer) {
HTMLString += F("<h2>Buzzer passif Installé</h2>");} else {
HTMLString += F("<h2>Buzzer passif non Installé ou désactivé</h2>");}
HTMLString += F("<a><br /></a>");
if (IsPushover) {
HTMLString += F("<h2><br /></h2>");
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
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
HTMLString += F("<hr style=\"border-top: 3px solid #095484;width: auto;\">");
HTMLString += F("<a><br /></a>");}
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 1px; height: 48px; width: 210px\">Retour à la Page d'Accueil</button></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a><br /></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString.clear();
if (SerialMonitor) {DEBUG_PRINTLN(F("Les Paramètres du Point d'accès (Portail Captif) sont appelés"));}}
else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

String CreateButton(String BMark, bool Bselect, String BHRef, String Blevel, String Bwidth, String BRmargin, String Bdisable) {
String HTMLString; HTMLString.reserve(220);
if (Bselect) {
HTMLString  = "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; color: blue; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"*</button></a>";} else {
HTMLString += "<a href =\"/"+BHRef+"\"><button class=\"button\" style=\"margin-right: "+BRmargin+"px; height: 26px; width: "+Bwidth+"px;\" "+Bdisable+">"+BMark+Blevel+"</button></a>";}
return HTMLString;}

/********************************************************************************************************************************************************/

void ResetClockSettings() {
if (!await) {   
String HTMLString; HTMLString.reserve(180);
HTMLString = F("<!DOCTYPE html> <html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align:center;}");
HTMLString += F("h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}");
HTMLString += F("h2 {font-size: 22px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}");
HTMLString += F("h3 {font-size: 28px; margin-top: 0px; margin-bottom: 10px; margin-left: 10px;}");
HTMLString += F("</style>");
HTMLString += F("</head>");
HTMLString += F("<body>");
HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
HTMLString += F("<a><br /></a>");
if (OnlyReboot) {
HTMLString += F("<h3>Mémoire vive insuffisante !</h3>");
HTMLString += F("<h2>(Redémarrer l'Horloge résout le Problème.)</h2>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Redémarrer l'Horloge Matricielle LED maintenant ?</h2>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/reboot\"><button class=\"button\" style=\"height: 48px; width: 110px; font-size: 22px; color: red\">OUI</button></a>");  
} else {
HTMLString += F("<h3>Réinitialisation de l'Horloge à Matrice LED !</h3>");
HTMLString += F("<h2>(Les données d'Accès WLAN seront Supprimées)</h2>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<h2>Maintenant, Réinitialisez l'Horloge Matricielle LED ?</h2>");
HTMLString += F("<a><br /></a>");
HTMLString += F("<a href =\"/goreset\"><button class=\"button\" style=\"height: 48px; width: 110px; font-size: 22px; color: red\">OUI</button></a>");}
HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 110px; font-size: 22px;\">NON</button></a>");
HTMLString += F("</body>");
HTMLString += F("</html>");  
OneTimeCall = true; 
SendHTMLString(HTMLString); // Send HTMLString
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité de l'air intérieur
HTMLString.clear();
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void ResetClockYes() { 
/*<meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
if (!await) { 
HTMLAnswer("ResetClockYes");
ResetNow = true; // Réinitialiser - Paramètres par défaut 
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void RebootClock()
{
if (!await) {   
HTMLAnswer("RebootClock");
RebootNow = true;
} else {HTMLAnswer("PW");}}

/********************************************************************************************************************************************************/

void SiteNotFound(){
String HTMLString; HTMLString.reserve(220);
HTMLString = F("<!DOCTYPE html> <html lang='fr'>\
  <head>\
  <title>Horloge-Matricielle-LED</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
   body {background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; text-align: center;}\
   h1 {color: #2ECCFA; margin-top: 30px; margin-bottom: 0px;}\
   h2 {font-size: 26px; margin-top: 0px; margin-bottom: 10px;}\
   h3 {font-size: 14px; margin-top: 0px; margin-bottom: 50px;}\
  </style>\
  </head>\
    <body>\
    <h1>Horloge-Matricielle-LED</h1>\
    <a><br /></a>\
    <h2>Cette Page n'a pas été Trouvée !</h2>\
    <h3>(Redémarrer la montre pourrait résoudre le problème)</h3>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>\
  </body>\
    </html>"); 
SendHTMLString(HTMLString); // Envoyer une chaîne HTML
MyWaitLoop(100);
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
HTMLString.clear();}

/********************************************************************************************************************************************************/

void ChangeDisplayAutoOnOff() {
String HTMLString; HTMLString.reserve(220);
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
    int16_t DP = DisplayOnTime.indexOf(':'); 
    String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
    String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
    CheckMDMinOn.trim();
    int16_t CheckMDHourOnInt, CheckMDMinOnInt;
    if (IsNumeric(CheckMDHourOn)) {CheckMDHourOnInt = (int16_t)strtol(CheckMDHourOn.c_str(), NULL, 10);} else {DisplayOnTime = F("!");}
    if (IsNumeric(CheckMDMinOn))  {CheckMDMinOnInt = (int16_t)strtol(CheckMDMinOn.c_str(), NULL, 10);} else {DisplayOnTime = F("!");}
    if (DisplayOnTime != "" && DisplayOnTime != "!" ) {
    for (byte i=0; i <= 23; i++){
    if (i ==  CheckMDHourOnInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOnTime = F("!");} else {   
    MDTime = false;    
    for (byte i=0; i <= 59; i++){
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
    int16_t DP = DisplayOffTime.indexOf(':'); 
    String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
    String CheckMDMinOff =  DisplayOffTime.substring(DP+1,6);
    CheckMDMinOff.trim();
    int16_t CheckMDHourOffInt, CheckMDMinOffInt;
    if (IsNumeric(CheckMDHourOff)) {CheckMDHourOffInt = (int16_t)strtol(CheckMDHourOff.c_str(), NULL, 10);} else {DisplayOffTime = F("!");}
    if (IsNumeric(CheckMDMinOff))  {CheckMDMinOffInt = (int16_t)strtol(CheckMDMinOff.c_str(), NULL, 10);} else {DisplayOffTime = F("!");}
    if (DisplayOffTime != "" && DisplayOffTime != "!") {
    for (byte i=0; i <= 23; i++){
    if (i== CheckMDHourOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");} else {   
    MDTime = false;    
    for (byte i=0; i <= 59; i++){
    if (i ==  CheckMDMinOffInt) {MDTime = true; break;}} 
    if (!MDTime) {DisplayOffTime = F("!");}}}}}

    if (DisplayOnTime == "" && DisplayOffTime == "") {
    DisplayOnTime = F("NullOn");
    DisplayOffTime = F("NullOff");}
	
	// Si MidnightGhost s'éteint ensuite une Minute à 00:00
    if (MidnightGhost == "OUI" && DisplayOffTime == "00:00") {
    DisplayOffTime = "00:01";}

    FDOnTime =  DisplayOnTime; // pour l'affichage uniquement
    FDOffTime = DisplayOffTime; // pour l'affichage uniquement

    // Si erreur de saisie
    if (DisplayOnTime == "!" || DisplayOffTime == "!" || DisplayOnTime == DisplayOffTime) {
    if (DisplayOnTime == "!") {DisplayOnTime = F("");} 
    if (DisplayOffTime == "!") {DisplayOffTime = F("");}  
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
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
    SetupSave(F("display-on-time.txt"), DisplayOnTime);} else {
    if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
    // Affichage matriciel Heure d'ARRÊT
    if (DisplayOffTime != ""){
    SetupSave(F("display-off-time.txt"), DisplayOffTime);} else {
    if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
    SetupSave(F("display-change-time.txt"), "True"); // Affichage marche/arrêt
    MyWaitLoop(500); // 0,5 secondes. attendez 
    if (SerialMonitor) {DEBUG_PRINTLN(F("Affichage automatique MARCHE/ARRÊT Données Enregistrées..."));} 
      
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
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
     if (SerialMonitor) {DEBUG_PRINTLN(F("Les données d'Accès et les Paramètres n'ont pas été Enregistrés !"));}
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");   
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
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
    printStringWithShift(cleanText("        +++       L'Horloge Redémarrera      +++           ").c_str(),ScrollTextTime);
    digitalWrite(HWLEDPin, LOW); // LED éteinte
    ESP.restart();  
    } 
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
    MyWaitLoop(100);
    InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
    } // Fin de l'erreur de saisie
  } else {  
    // Settings
    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
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
    MyWaitLoop(100);}} else {HTMLAnswer("PW");}}  // Envoyer une chaîne HTML or wait

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
String HTMLString; HTMLString.reserve(220);
  // Afficher l'heure d'allumage sous forme de chaîne
  if (littleFSInit) {
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOnTime.clear();}} else
         {DisplayOnTime.clear();}
  if (littleFSInit) {
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOffTime.clear();}} else
         {DisplayOffTime.clear();}

  // Vérifier la plausibilité des informations pour affichage matriciel marche/arrêt automatique
  CheckLEDMatrixDisplayOnOff();

    HTMLString = HTMLStringPart1();
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 {font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px;}");
    HTMLString += F("h5 {font-size: 16px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 {font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
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
    InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
 HTMLString.clear();}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOn() {
if (StartSound != "On") { 
if (!await) {  
await = true;
StartBuzzerBeep();    
if (littleFSInit) {
StartSound = F("On");   
SetupSave(F("status_startsound.txt"), StartSound);} else {StartSound = F("Off");}
HTMLAnswer("StartSoundOnM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer Son de démarrage activé"));}
await = false;} else {HTMLAnswer("PW");}} else {HTMLAnswer("StartSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void StartSoundOff() {
if (StartSound != "Off") {  
StartSound = F("Off");     
if (littleFSInit) {  
SetupSave(F("status_startsound.txt"), StartSound);} 
HTMLAnswer("StartSoundOffM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer Son de démarrage désactivé"));}
} else {HTMLAnswer("StartSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOn() {
if (HourSound != "On") { 
if (!await) {  
await = true;
HourBuzzerBeep();   
if (littleFSInit) {
HourSound = F("On");   
SetupSave(F("status_hoursound.txt"), HourSound);} else {HourSound = F("Off");}
HTMLAnswer("HourSoundOnM");
await = false;
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer horaire activé"));}} else {HTMLAnswer("PW");}
} else {HTMLAnswer("HourSoundOnMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void HourSoundOff() {
if (HourSound != "Off") {
HourSound = F("Off");  
if (littleFSInit) {
SetupSave(F("status_hoursound.txt"), HourSound);} 
HTMLAnswer("HourSoundOffM");
if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer horaire est désactivé"));}
} else {HTMLAnswer("HourSoundOffMIs");}}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GoToWebSiteHomePage() { // Redirection automatique vers la Page d'Accueil
String HTMLString; HTMLString.reserve(220);
HTMLString = F("<!DOCTYPE html><html lang='fr'>");
HTMLString += F("<head>");
HTMLString += F("<title>Horloge-Matricielle-LED</title>");
HTMLString += F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">");
HTMLString += F("<style>");
HTMLString += F("body {background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; text-align: center;}");
HTMLString += F("h1 {font-size: 64px; color: #2ECCFA; margin-top: 30px; margin-bottom: 0px; text-align: center;}");
HTMLString += F("h2 {font-size: 48px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
HTMLString += F("h3 {font-size: 36px; margin-top: 0px; margin-bottom: 10px; text-align: center;}");
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

String HTTPCodeText(int16_t httpcode) {
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
if (httpcode == 999) {TempHTTPCodeText = F("No Data Found");}
return String(httpcode) + " - " + TempHTTPCodeText;}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++