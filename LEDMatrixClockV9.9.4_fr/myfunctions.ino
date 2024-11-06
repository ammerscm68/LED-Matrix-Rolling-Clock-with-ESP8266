// **************************** My Functions ***************************************************
// *************************************************************************************************

void Greetings_and_Holiday(bool GHStart) {   // uae = ä Signe 
getTimeLocal(); // get current time          // uoe = ö Signe 
GreetingsToday = false;                      // uue = ü Signe                                         
GT = F(""); HT = F("");                      // deg = ° Degrés Signe
String GreetingText = F("");                 // Propriétaire de la montre uniquement
int HDay = 0;                                // uss = ß Signe  
int EasterSunday;
WatchOwner1FirstName.trim(); WatchOwner1LastName.trim(); Gender1.trim(); Birthday1.trim();
WatchOwner2FirstName.trim(); WatchOwner2LastName.trim(); Gender2.trim(); Birthday2.trim();
WatchOwner3FirstName.trim(); WatchOwner3LastName.trim(); Gender3.trim(); Birthday3.trim();
// ***************************** "Birthdays or other Greetings" ********************************* 
// 1. WatchOwner 
if (WatchOwner1FirstName != "" && Gender1 != "" && Birthday1 != "") { // Uniquement les anniversaires des propriétaires de cette montre
if  ((String(d) + "." + String(mo)) == Birthday1) {  
if (Gender1 == "male") {  // Salutations d'anniversaire au propriétaire principal, un homme
GreetingText = "Joyeux Anniversaire chérie "+WatchOwner1FirstName;} 
if (Gender1 == "female") { // Salutations d'anniversaire à la propriétaire principale, une femme
GreetingText = "Joyeux Anniversaire, chérie "+WatchOwner1FirstName;}
if (Gender1 == "male" || Gender1 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Salutations: "+GT);}  
SIC == true; // pour l'horloge ScrollIn
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}
// 2. Propriétaire de la montre (facultatif)
if (WatchOwner2FirstName != "" && Gender2 != "" && Birthday2 != "") {
if  ((String(d) + "." + String(mo)) == Birthday2) {  
if (Gender2 == "male") { // Salutations d'anniversaire au 2ème propriétaire masculin
GreetingText = "Joyeux Anniversaire chérie "+WatchOwner2FirstName;} 
if (Gender2 == "female") { // Salutations d'anniversaire à la 2ème femme propriétaire
GreetingText = "Joyeux Anniversaire chérie "+WatchOwner2FirstName;}
if (Gender2 == "male" || Gender2 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Salutations: "+GT);}  
SIC == true; // pour l'horloge ScrollIn
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}
// 3. Propriétaire de la montre (facultatif)
if (WatchOwner3FirstName != "" && Gender3 != "" && Birthday3 != "") {
if  ((String(d) + "." + String(mo)) == Birthday3) {  
if (Gender3 == "male") { // Salutations d'anniversaire au 3ème propriétaire masculin
GreetingText = "Joyeux Anniversaire chérie "+WatchOwner3FirstName;} 
if (Gender3 == "female") { // Salutations d'anniversaire à la 3ème femme propriétaire
GreetingText = "Joyeux Anniversaire chérie "+WatchOwner3FirstName;}
if (Gender3 == "male" || Gender3 == "female") {
if (ClockStart) {
GT= "       +++     "+cleanText(GreetingText)+"     +++                  ";
GT1= "+++ "+GreetingText+" +++";} else {  
GT= "       +++     "+cleanText(GreetingText)+"     +++           ";
GT1= "+++ "+GreetingText+" +++";}  
GreetingsToday = true;
if (SerialMonitor) {Serial.println("Salutations: "+GT);}  
SIC == true; // pour l'horloge ScrollIn
if (Display) {printStringWithShift(GT.c_str(), ScrollTextTime);}}}}} // Sortie du texte de salutation

// ***************************** "Jours Fériés" *********************************************************
String HolidayDate = String(d) + "." + String(mo);
String HoliDayText = F("");
// Vacances
if (HolidayDate == "1.1") {HoliDayText = F("Jour de l'An");}   // Jour de l'An
if (HolidayDate == "1.5") {HoliDayText = F("Fête du travail");} // Fête du travail
if (HolidayDate == "1.8") {HoliDayText = F("Victoire 1945");} // Victoire 1945
if (HolidayDate == "14.7") {HoliDayText = F("Fête Nationale Française");} // Fête Nationale Française
if (HolidayDate == "15.8") {HoliDayText = F("Assomption");} // Assomption
if (HolidayDate == "1.11") {HoliDayText = F("Toussaint");} // Toussaint
if (HolidayDate == "11.11") {HoliDayText = F("Armistice 1918");} // Armistice 1918
if (HolidayDate == "25.12") {HoliDayText = F("Noël");} // Noël

// Déterminer les jours fériés non fixes (grâce à "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916)
if (mo>2 && mo<7) {
if (mo==3) {HDay=d;}  // Si mars, déterminez le jour actuel
if (mo==4) {HDay=d+31;} 
if (mo==5) {HDay=d+31+30;} 
if (mo==6) {HDay=d+31+30+31;} 
EasterSunday = Calculate_Eastern();
if (SerialMonitor) {
Serial.println(F("")); 
Serial.println("EasterSunday: "+String(EasterSunday)); // Voir la valeur du dimanche de Pâques
Serial.println(F("")); } 
if (EasterSunday+1==HDay)  {HoliDayText = F("Lundi de Pâques");}   // Lundi de Pâques
if (EasterSunday+39==HDay)  {HoliDayText = F("Ascension");}  // Ascension
if (EasterSunday+50==HDay)  {HoliDayText = F("Lundi de Pentecôte");}}  // Lundi de Pentecôte

if (HoliDayText != "" || HolidayDate == "31.12") {
GreetingsToday = true;
if (HolidayDate != "31.12") {
HT= "          +++     Aujourd'hui:   "+cleanText(HoliDayText)+"     +++                ";  
HT1= "+++ "+HoliDayText+" +++";  
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Vacances: "+HT1); // Afficher le texte de vacances
Serial.println(F(""));}} else { // Réveillon du Nouvel An
HT= F("       +++     Nous vous Souhaitons un bon début d'Année     +++                ");
HT1= F("+++ Nous vous Souhaitons un bon début d'Année +++");
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println("Réveillon du Nouvel An: "+HT1); // Afficher le texte du réveillon du Nouvel An
Serial.println(F(""));}}
SIC == true; // pour l'horloge ScrollIn  
if (Display && !GHStart) {
HT = cleanText(HT);  
printStringWithShift(HT.c_str(), ScrollTextTime);}} else { // Sortie HoliDayText
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println(F("Jours fériés : Pas de Jours fériés"));
Serial.println(F(""));}}} // Fin de fonction "void Greetings_and_Holiday()"

// *************************************************************************************************

void InitSerial(int Baud, int Wait) { 
Serial.begin(Baud);  
while (!Serial) {Serial.print(F("*"));}
printStringWithShift("COM  ",ScrollTextTime); // COM INIT
for (int i = 0; i < Wait; i++) {
Serial.print(F("*")); MyWaitLoop(100);}
printStringWithShift("           ",ScrollTextTime); 
/*for(int i=0; i<10; i++) { 
Serial.println(F("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"));}*/}

// *************************************************************************************************

void ClockScrollIn() {
String TimeString; 
sendCmdAll(CMD_SHUTDOWN,0); // Affichage désactivé
for (int i=0; i <= 50; i++){ 
MyWaitLoop(10);     
HandleOTAandWebServer();  // Requête OTA et serveur Web  
getAnimClock(false);
if (s == 59 && m < 59) {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m+1);} else {TimeString = "  "+String(h)+String(m+1);}
} else {
if (m < 10) {TimeString = "  "+String(h)+"0"+String(m);} else {TimeString = "  "+String(h)+String(m);}}}  
printStringWithShift("           ",1);
sendCmdAll(CMD_SHUTDOWN,1); // Afficher sur 
printStringWithShift3(TimeString.c_str(),ScrollTextTime-5);}

// *************************************************************************************************

void ShowGhost() {
String GShow = F(""); 
// Signes et trémas (Police : clockfont[])
char  ghost = '9' + 1; // Signe fantôme
if (SerialMonitor) {Serial.println(F("L'Heure des Sorcières ....."));} 
if (Display) {
sendCmdAll(CMD_SHUTDOWN,0); // Arrêt de la matrice
printStringWithShift3("         ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,1); // Mise en marche de la matrice
GShow = String(ghost)+"  "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(5000); // Courte attente
HandleOTAandWebServer();  // Requête OTA et serveur Web 
printStringWithShift3("      ",ScrollTextTime+10);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Courte attente
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
printStringWithShift3("      ",ScrollTextTime+5);
MyWaitLoop(100); // Courte attente
ClockScrollIn();
} else {
printStringWithShift3("      ",ScrollTextTime-5);  
sendCmdAll(CMD_SHUTDOWN,1); // Mise en marche de la matrice
GShow = String(ghost)+"  "+String(ghost);
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Courte attente
HandleOTAandWebServer();  // Requête OTA et serveur Web 
printStringWithShift3("      ",ScrollTextTime+5);
GShow = " "+String(ghost)+String(ghost)+" "; 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
MyWaitLoop(3000); // Courte attente
GShow = "       "+String(ghost); 
printStringWithShift3(GShow.c_str(),ScrollTextTime+5);
HandleOTAandWebServer();  // Requête OTA et serveur Web 
printStringWithShift3("      ",ScrollTextTime+5);
sendCmdAll(CMD_SHUTDOWN,0);}} // Arrêt de la matrice

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Charger les paramètres .... ***
swait = true; // pendant le chargement, aucun identifiant de site Web - activé
bool MDTime; // Variable auxiliaire
// Activer le système de fichiers WEMOS D1 Little
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); // attend les données

if (littleFSInit) {
if (SerialMonitor) {Serial.println(F("Charger les Ajustements de l’horloge ..."));}
  // Charger les données d'accès WiFi
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt").c_str();
  } else {ssid = F("");}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = F("");}
  // *** Charger les données d'accès à OpenWeatherMap ***
  // Clé API OpenWeatherMap sous forme de chaîne
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt").c_str();
  } else {weatherKey = F("");}
  // OpenWeatherMap City-ID sous forme de chaîne
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt").c_str();
  } else {cityID = F("");} 
  // NewsData-Key
  if (LittleFS.exists("/newsdata-apikey.txt")){
   newsKey = SetupLoad("newsdata-apikey.txt");
   newsKey.trim();} else {newsKey = F("");} // https://NewsData.io

   // Désactivez OpenWeatherMAP si aucune clé API ou identifiant de ville n'est disponible
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // Désactivez NewsData si aucune clé API n'est disponible
   if (newsKey != "") {AccessNewsData = true;} else {
   AccessNewsData = false;}   

  // Fonction données météo activée/désactivée
  if (AccessOpenWeatherMap) {
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  String OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}
  if (OWMF == "Off") {WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}} else {
  WeatherFunctions = true; StatusOpenWeatherMap = F("enabled");}} else {
  WeatherFunctions = false; StatusOpenWeatherMap = F("disabled");}

  // Fonction NewsData activée/désactivée
  if (AccessNewsData) { 
  if (LittleFS.exists("/status_NewsDatafunctions.txt")){
  String NAF = SetupLoad("status_NewsDatafunctions.txt");
  NAF.trim();
  if (NAF == "" || NAF == "On") {NewsDataFunction = true; StatusNewsData = F("enabled");}
  if (NAF == "Off") {NewsDataFunction = false; StatusNewsData = F("disabled");}} else {
  NewsDataFunction = true; StatusNewsData = F("enabled");}} else {
  NewsDataFunction = false; StatusNewsData = F("disabled");}  

  // Charger les données pushover
  if (LittleFS.exists("/po-secureapptoken.txt")){
  SecureAppToken = SetupLoad("po-secureapptoken.txt");} else {SecureAppToken = F("");}
  SecureAppToken.trim(); // Supprimer les espaces à l'avant et à l'arrière
  if (LittleFS.exists("/po-usertoken.txt")){
  UserToken = SetupLoad("po-usertoken.txt");} else {UserToken = F("");}
  UserToken.trim(); // Supprimer les espaces à l'avant et à l'arrière
  if (LittleFS.exists("/po-device1.txt")){
  PushoverDevice1 = SetupLoad("po-device1.txt");} else {PushoverDevice1 = F("");}
  PushoverDevice1.trim(); // Supprimer les espaces à l'avant et à l'arrière
  if (LittleFS.exists("/po-device2.txt")){
  PushoverDevice2 = SetupLoad("po-device2.txt");} else {PushoverDevice2 = F("");}
  PushoverDevice2.trim(); // Supprimer les espaces à l'avant et à l'arrière
  if (LittleFS.exists("/po-device3.txt")){
  PushoverDevice3 = SetupLoad("po-device3.txt");} else {PushoverDevice3 = F("");}
  PushoverDevice3.trim(); // Supprimer les espaces à l'avant et à l'arrière
  if (LittleFS.exists("/po-messagesound.txt")){
  PushoverMessageSound = SetupLoad("po-messagesound.txt");} else {PushoverMessageSound= F("none");}
  if (SecureAppToken != "" && UserToken != "" && PushoverDevice1 != "") {
  IsPushover = true;} else {IsPushover = false;}

  // Afficher l'heure d'allumage sous forme de chaîne
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");
  DisplayOnTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOnTime = F("");}
  // Afficher l'heure d'arrêt sous forme de chaîne
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");
  DisplayOffTime.trim(); // Supprimer les espaces à l'avant et à l'arrière
  } else {DisplayOffTime = F("");} 

  if (SerialMonitor) {
  Serial.println("");  
  Serial.println("Affichage à l'Heure: "+DisplayOnTime);
  Serial.println("Afficher l'Heure d'arrêt: "+DisplayOffTime);
  Serial.println("");}

  // Décalage du capteur DHT
  if (LittleFS.exists("/dht-intempoffset.txt")){
  InTempOffset = SetupLoad("dht-intempoffset.txt");
  InTempOffset.trim(); // Supprimer les espaces à l'avant et à l'arrière
  InTempOffsetInt = InTempOffset.toInt(); // IntegerCheck was carried out in the Captive Portal
  } else {InTempOffset = F("0");
          InTempOffsetInt = 0;}  // Temperature 
  if (LittleFS.exists("/dht-inhumoffset.txt")){
  InHumOffset = SetupLoad("dht-inhumoffset.txt");
  InHumOffset.trim(); // Supprimer les espaces à l'avant et à l'arrière
  InHumOffsetInt = InHumOffset.toInt(); // IntegerCheck a été effectué dans le portail captif
  } else {InHumOffset = F("0");
          InHumOffsetInt = 0;}  // Humidité

  // Un buzzer passif est-il activé ?
  if (LittleFS.exists("/buzzer-install.txt")){
  if (SetupLoad("buzzer-install.txt") == "True") {IsBuzzer = true;} else {IsBuzzer = false;}}
  // Modes de sonnerie d'état
  if (IsBuzzer) {
  if (LittleFS.exists("/status_startsound.txt")){
  StartSound = SetupLoad("status_startsound.txt");} else {StartSound = F("Off");}
  if (LittleFS.exists("/status_hoursound.txt")){
  HourSound = SetupLoad("status_hoursound.txt");} else {HourSound = F("Off");}}

  // Vérifier la plausibilité des informations sur Matrix Display Automatic On/Off
  CheckLEDMatrixDisplayOnOff();

  // Luminosité de la matrice LED
  if (LittleFS.exists("/matrix_brightness.txt")){
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // si le Fichier est vide ou erreur --> Niveau 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Niveau 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Niveau 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Niveau 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);}} else {Brightness = F("L1"); sendCmdAll(CMD_INTENSITY, 0);} // Niveau 5 - Niveau 1
  // Double Point status
  if (LittleFS.exists("/status_doublepoint.txt")){
  String DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // si le fichier est vide ou erreur
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}} else {DoublePointStatus = 1;}
  // Vitesse de défilement du texte
  if (LittleFS.exists("/status_scrolltexttime.txt")){
  String STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "25") {ScrollTextTime = 25;} // Niveau 8
  if (STT == "30") {ScrollTextTime = 30;} // Niveau 7
  if (STT == "35") {ScrollTextTime = 35;} // Niveau 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Défaut = Niveau 5
  if (STT == "45") {ScrollTextTime = 45;} // Niveau 4
  if (STT == "50") {ScrollTextTime = 50;} // Niveau 3
  if (STT == "55") {ScrollTextTime = 55;} // Niveau 2
  if (STT == "60") {ScrollTextTime = 60;}} else {ScrollTextTime = 40;} // Niveau 1 - Niveau 5 
  
  // Fonctions supplémentaires marche/arrêt
  if (LittleFS.exists("/status_enhancedfunctions.txt")){
  String EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}} else {EnhancedFunctions = true;} 
} else
    {
     // Définir les valeurs par défaut pour l'erreur "Little FS"
    ssid = F("");
    password = F(""); 
    Brightness = F("L1");
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    DisplayOnTime = F("6:30");
    DisplayOffTime = F("23:15");
    EnhancedFunctions = true;
    WeatherFunctions = false;
    weatherKey = F(""); // Clé API de OpenWeatherMap - https://openweathermap.org
    cityID = F(""); // ID de la ville par OpenWeatherMap
    newsKey = F(""); // Clé API de NewsData - https://NewsData.io
    InTempOffsetInt = -1; // Décalage de température
    InHumOffsetInt = -1; // Compensation d'humidité
    StartSound = F("Off"); // pas de buzzer
    HourSound = F("Off"); // pas de buzzer
    }
 swait = false;} // pendant le chargement, aucun identifiant de site Web - désactivé

// *************************************************************************************************

void CheckLEDMatrixDisplayOnOff() {
bool MDTime;  
if (ssid != "") {
  FDOnTime = DisplayOnTime; // Temps gagné d'origine
  FDOffTime = DisplayOffTime; // Temps gagné d'origine

   // Check display on/off time for plausibility
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
  if (DisplayOnTime != "") {
  if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = F("");};
  if (DisplayOnTime != "") {
  if (DisplayOnTime.substring(0,1) == "0") {DisplayOnTime.remove(0, 1);}
  int DP = DisplayOnTime.indexOf(':')+1;
  if (DisplayOnTime.substring(DP,DP+1) == "0") {DisplayOnTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
if (DisplayOnTime != "") {
MDTime = false;
int DP = DisplayOnTime.indexOf(':'); 
String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = F("");} else {   
MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}} 
if (!MDTime) {DisplayOnTime = F("");}}}}}

  if (DisplayOffTime != "") {
  if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = F("");};
  if (DisplayOffTime != "") {
  if (DisplayOffTime.substring(0,1) == "0") {DisplayOffTime.remove(0, 1);}
  int DP = DisplayOffTime.indexOf(':')+1;
  if (DisplayOffTime.substring(DP,DP+1) == "0") {DisplayOffTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = F("");}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = F("");}
if (DisplayOffTime != "") {
MDTime = false;
int DP = DisplayOffTime.indexOf(':'); 
String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
String CheckMDMinOff=  DisplayOffTime.substring(DP+1,6);
CheckMDMinOff.trim(); 

// Entrées de processus pour Matrix On/Off
for (int i=0; i <= 23; i++){
if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");} else {MDTime = false;    
for (int i=0; i <= 59; i++){
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}} 
if (!MDTime) {DisplayOffTime = F("---");}}}}}}} // SSID - Ende  

// *************************************************************************************************

void SaveMatrixAdjustments(){
  swait = true;  // while Enregistrer aucun identifiant de site Web - activé
  if (littleFSInit) {
  if (SerialMonitor) {Serial.println(F("Enregistrer les Réglages de l'horloge ..."));}    
  // Garniture
  TempSSID.trim(); // SSID WLAN
  newsKey.trim(); // Clé API NewsData
  DisplayOnTime.trim(); // Heure d'allumage de l'affichage matriciel
  DisplayOffTime.trim(); // Heure d'extinction de l'affichage matriciel
  InTempOffset.trim(); // Décalage du capteur DHT (Température)
  InHumOffset.trim(); // Décalage du capteur DHT (humidité de l'air)
  
  // Spécification du SSID
  SetupSave("ssid.txt", TempSSID); // SSID WLAN
  SetupSave("ssid-passwd.txt", password); // Mot de passe Wi-Fi
  // Clé API OpenWeatherMap et ID de ville OpenWeatherMap
  if (weatherKey != "" && cityID != "") {
  SetupSave("owm-apikey.txt", weatherKey);
  SetupSave("owm-cityid.txt", cityID);} else {
  if (LittleFS.exists("/owm-apikey.txt")){LittleFS.remove("/owm-apikey.txt");}
  if (LittleFS.exists("/owm-cityid.txt")){LittleFS.remove("/owm-cityid.txt");}} 
  // Clé API NewsData
  if (newsKey != "") {
  SetupSave("newsdata-apikey.txt", newsKey);} else {
  if (LittleFS.exists("/newsdata-apikey.txt")){LittleFS.remove("/newsdata-apikey.txt");}}
  // Heure d'allumage de l'affichage matriciel
  if (DisplayOnTime != ""){
  SetupSave("display-on-time.txt", DisplayOnTime);} else {
  if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
  // Heure d'extinction de l'affichage matriciel
  if (DisplayOffTime != ""){
  SetupSave("display-off-time.txt", DisplayOffTime);} else {
  if (LittleFS.exists("/display-off-time.txt")){LittleFS.remove("/display-off-time.txt");}}
  // Un Buzzer passif est-il activé ?
  if (IsBuzzer){
  SetupSave("buzzer-install.txt", "True");} else {
  if (LittleFS.exists("/buzzer-install.txt")){LittleFS.remove("/buzzer-install.txt");}}

  // *** Enregistrer les données Pushover ***
  // eton SecureApp Pushover
  if (SecureAppToken == "") {
  if (LittleFS.exists("/po-secureapptoken.txt")){LittleFS.remove("/po-secureapptoken.txt");}  
  } else {SetupSave("po-secureapptoken.txt", SecureAppToken);} 
  // Jeton utilisateur Pushover
  if (UserToken == "") {
  if (LittleFS.exists("/po-usertoken.txt")){LittleFS.remove("/po-usertoken.txt");} 
  } else {SetupSave("po-usertoken.txt", UserToken);}
  // Nom du périphérique Pushover 1
  if (PushoverDevice1 == "") {
  if (LittleFS.exists("/po-device1.txt")){LittleFS.remove("/po-device1.txt");} 
  } else {SetupSave("po-device1.txt", PushoverDevice1);}
  // Nom du périphérique Pushover 2
  if (PushoverDevice2 == "") {
  if (LittleFS.exists("/po-device2.txt")){LittleFS.remove("/po-device2.txt");} 
  } else {SetupSave("po-device2.txt", PushoverDevice2);}
  // Nom du périphérique Pushover 3
  if (PushoverDevice3 == "") {
  if (LittleFS.exists("/po-device3.txt")){LittleFS.remove("/po-device3.txt");} 
  } else {SetupSave("po-device3.txt", PushoverDevice3);}
  // Son du message Pushover
  if (PushoverMessageSound == "none" || PushoverMessageSound == "") {
  if (LittleFS.exists("/po-messagesound.txt")){LittleFS.remove("/po-messagesound.txt");} 
  } else {SetupSave("po-messagesound.txt", PushoverMessageSound);}

  // Vérifier le décalage DHT
  if (!IsNumeric(InTempOffset) || InTempOffset == "") {InTempOffset = F("0");} // Température
  if (!IsNumeric(InHumOffset) ||  InHumOffset == "") {InHumOffset = F("0");} // Humidité de l'Air
  
  // Décalage du Capteur DHT
  if (InTempOffset != ""){
  SetupSave("dht-intempoffset.txt", InTempOffset);} else {
  if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
  if (InHumOffset != ""){
  SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
  if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}}
 swait = false;}  // while Enregistrer aucun identifiant de site Web - désactivé

// *************************************************************************************************

// Charger et enregistrer les paramètres de configuration
String SetupLoad(String file_name) {
swait = true;  
String result = F(""); // initialisation
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // échec de l'ouverture du fichier, résultat vide
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  result.trim();
  MyWaitLoop(10); // attendez
  swait = false;
  return result;}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {
  swait = true;  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // n'a pas réussi à ouvrir le fichier, renvoie false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // échec d'écriture
      return false;
  }
  this_file.close();
  MyWaitLoop(10); // attendez
  swait = false;
  return true;}

// *************************************************************************************************

// Allumer et éteindre l'écran à l'aide d'un bouton
void MatrixButtonOnOff(){
if(digitalRead(PushButtonPin) == LOW){
MyWaitLoop(500); // attendez
Display = !Display;
if (!Display && ClockWiFiConnectFail == false){  
sendCmdAll(CMD_SHUTDOWN,0); // L'affichage s'éteint
if (SerialMonitor) {Serial.println("Matrice éteinte via le Bouton à "+ FormatedDateTimeFR(false));} 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // obtenir l'heure actuelle
    sendCmdAll(CMD_SHUTDOWN, 1); // L'affichage s'allume
    sendCmdAll(CMD_INTENSITY, 0); // Luminosité de la matrice LED --> 0= Minimum
    // ShortBuzzerBeep();
    if (SerialMonitor) {Serial.println("Matrice activée via le Bouton à "+ FormatedDateTimeFR(false));}} else
    {
    sendCmdAll(CMD_SHUTDOWN, 1); // only Display on
    printCharWithShift('.',5);
    }
    // Activez le « Access Point » après l’avoir éteint 10 fois
    APStart += 1; // inc  
    if (APStart == 10 && littleFSInit == true) {
    ResetClock();} // Réinitialisation de l'horloge 
    }
    delay(250);}} // attendez

// *************************************************************************************************

String FormatedDateTimeFR(bool FDTHTML) {
String DateTimeString = F("");
getTimeLocal(); // obtenir l'heure actuelle 
// Time
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (FDTHTML) {DateTimeString += " heures  &nbsp;-/-&nbsp;  ";} else {DateTimeString += " heures -/- ";} // Time
// Date
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += "/0"+String(mo)+"/"+String(ye);} else {DateTimeString += "/"+String(mo)+"/"+String(ye);}
return DateTimeString;}

// *************************************************************************************************

// Requête Arduino OTA uniquement toutes les 0,8 secondes et requête du serveur Web uniquement toutes les 0,8 secondes.
void HandleOTAandWebServer(){ 
if (millis() - hwsTime > 800) { 
hwsTime = millis(); 
if (!swait) {
server.handleClient();}} else {
ArduinoOTA.handle();}} // Requête OTA

// *************************************************************************************************

void wificonnect(bool Start) {
WiFiConnectLoop = 0; // Réinitialisation des variables  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
if (SerialMonitor) {Serial.print(".");} 
if (!ClockStart && Display) {printCharWithShift2('.',1);}
MatrixButtonOnOff(); // Bouton de requête
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED)
{ClockWiFiConnectFail = true;
clr(); // Affichage clair
refreshAll(); 
APStart = 0; // Bouton « Point d'accès » Réinitialisation du compteur
if (!Start) {if (SerialMonitor) {Serial.println(F("Connexion WiFi déconnectée ..."));}} else {StartFail = true;} 
} else
{
ClockWiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop));} // Time until connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if (SerialMonitor) {
Serial.println(F(""));
Serial.print(F("Connecté à l'Adresse IP: ")); 
Serial.println(WiFi.localIP());}}}

// *************************************************************************************************

void WiFiUDPStart() { 
Udp.begin(localPort);
if (SerialMonitor) {
Serial.print(F("Port localisateur: "));
Serial.println(Udp.localPort());
Serial.println(F("NTP-Time - En attente de Synchronisation"));}
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Nombre de secondes entre les resynchronisations. 86400 = 1 jour
// setSyncInterval(60); // Test
if (SerialMonitor) {Serial.println("Le Serveur UDP a démarré - "+FormatedDateTimeFR(false));}}

// *************************************************************************************************

void WiFiUDPStop() {
Udp.stop();
if (SerialMonitor) {Serial.println("Serveur UDP arrêté - "+FormatedDateTimeFR(false));}}

// *************************************************************************************************

void WebServerStart() {
server.begin();
if (SerialMonitor) {Serial.println("Serveur Web démarré - "+FormatedDateTimeFR(false));}}

// *************************************************************************************************

void WebServerStop() {
server.stop();
if (SerialMonitor) {Serial.println("Serveur Web arrêté - "+FormatedDateTimeFR(false));}}

// *************************************************************************************************

void MyWaitLoop(int wlt){// Boucle d'attente
waitloop = 0; // Réinitialisation des variables
while (waitloop < wlt) { // Pause en millisecondes{ 
waitloop += 1; // inc boucle d'attente 1
delay(1);
HandleOTAandWebServer();  // Requête OTA et serveur Web
yield();}} 

// *************************************************************************************************

// vérifiez une chaîne pour voir si elle est numérique et acceptez le point décimal et la valeur positive et négative
boolean IsNumeric(String str) {
if(str.length()<1){return false;}
bool bPoint=false;
if ((str.charAt(0) == '-' || str.charAt(0) == '+') && (str.length()>1)) {  
for(unsigned char i = 1; i < str.length(); i++) {
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};}} else {
for(unsigned char i = 0; i < str.length(); i++) {
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};}}
return true;}

// *************************************************************************************************

void ResetAPCounter() {
String ResetValue[] = {"0","2", "4", "6", "8", "10", "12", "14" ,"16", "18", 
                       "20", "22", "24", "26", "28", "30", "32", "34", "36",
                       "38", "40", "42", "44", "46", "48", "50", "52", "54",
                       "56", "58"}; 
for (int i=0; i <= 30; i++){                      
if  (String(m)+":"+String(s) == ResetValue[i]+":2") { 
APStart = 0;}}} // Compteur du bouton "Réinitialiser le point d'accès" toutes les 2 minutes

// ********************** Routine by freddy64 ******************************************************

String runtime() {
  static uint8_t rolloverCounter = 0;
  static uint32_t lastMillis = 0;
  uint32_t currentMillis = millis() - bootTime;
  if (currentMillis < lastMillis) {       // vérifier le débordement en millis
    rolloverCounter++;}
  lastMillis = currentMillis;
  uint32_t secs = (0xFFFFFFFF / 1000 ) * rolloverCounter + (currentMillis / 1000);
  char buf[20];
  sprintf(buf, "%ld %s %02ld:%02ld:%02ld", secs / 86400, (secs < 86400 || secs > 172800) ? "Jours" : "Jour", secs / 3600 % 24, secs / 60 % 60, secs % 60);
  return buf;}

// ************************************************************************************************

void ResetClock() {
APStart = 0; // Réinitialiser
swait = true; // plus de gestion de site Web
bool IsFileExist = false; // Réinitialiser
digitalWrite(HWLEDPin, LOW); // LED éteinte  
if (LittleFS.exists("/ssid.txt")){  
LittleFS.remove("/ssid.txt");}
if (LittleFS.exists("/ssid-passwd.txt")){  
LittleFS.remove("/ssid-passwd.txt");}
MyWaitLoop(500); // 0,5 sek. wait 
if (SerialMonitor) {Serial.println(F("Réinitialiser - L'Horloge Redémarrera"));}
if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Switch on Display when off
ct = cleanText("        +++     Réinitialiser - L'Horloge Redémarrera      +++           ");
printStringWithShift(ct.c_str(),ScrollTextTime);
ESP.restart();}

// ************************************************************************************************

int FormatedDisplayOffTime() {
String fTime;
fTime = FDOffTime;
if (fTime != "") {
int DP = fTime.indexOf(':'); 
fTime.remove(DP, 1);
} else {fTime = F("0");} 
return fTime.toInt();}

// *************************************************************************************************

bool GoMatrixAction() { 
bool LMOTH, DOffMinIntOffset;  
String TempDOffHour, TempDOffMin;
String TempActualTimeHour, TempActualTimeMin;              
if (DisplayOffTime != "") { 
int DP = DisplayOffTime.indexOf(':'); 
String DOffHour = DisplayOffTime.substring(0,DP); 
String DOffMin=  DisplayOffTime.substring(DP+1,6);
DOffHour.trim(); 
DOffMin.trim(); 
int DOffHourInt = DOffHour.toInt();
int DOffMinInt = DOffMin.toInt(); 
DOffMinInt -= 10; // Moins 10 minutes
if (DOffMinInt <= 0) {
DOffMinInt = 50 + DOffMin.toInt(); 
DOffMinIntOffset = true;
DOffHourInt -= 1;
if (DOffHourInt < 0) {DOffHourInt = 23;}} else {DOffMinIntOffset = false;} 

// Affichage du temps d'arrêt - Moins 10 minutes
if (DOffHourInt < 10) {TempDOffHour = "0"+String(DOffHourInt);} else {TempDOffHour = String(DOffHourInt);}
if (DOffMinInt < 10)  {TempDOffMin = "0"+String(DOffMinInt);} else {TempDOffMin = String(DOffMinInt);}
String ActionTime = TempDOffHour+TempDOffMin;  
int ActionTimeInt = ActionTime.toInt();

int ActualHour = h; // Heure actuelle
int ActualMin = m;  // Procès-verbal actuel
// Heure actuelle sous forme de chaîne
if (ActualHour < 10) {TempActualTimeHour = "0"+String(ActualHour);} else 
                     {TempActualTimeHour = String(ActualHour);}
if (ActualMin < 10) {TempActualTimeMin = "0"+String(ActualMin);} else 
                    {TempActualTimeMin = String(ActualMin);}
String ActualTime = TempActualTimeHour + TempActualTimeMin;
int ActualTimeInt = ActualTime.toInt();

int TimeDifference = ActualTimeInt - ActionTimeInt;

if (DOffMinIntOffset) {
if (TimeDifference >= 0 && TimeDifference <= 50) {LMOTH = false;} else {LMOTH = true;}} else {
if (TimeDifference >= 0 && TimeDifference <= 10) {LMOTH = false;} else {LMOTH = true;}}
} else {LMOTH = true;}
return LMOTH;}

// *************************************************************************************************

void SendHTMLString(String SHTMLString) {
server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
server.sendHeader("Pragma", "no-cache");
server.sendHeader("Expires", "-1");  
server.send(200, "text/html", SHTMLString);
SHTMLString = F("");}

// *************************************************************************************************

void StringBytes(String myString) { // Vérifiez la quantité de mémoire qu'une chaîne occupe.
unsigned int len = myString.length() + 1;
byte buff[len];
String GesBuff;
int gesBuff = 0;
myString.getBytes(buff, len);
if (SerialMonitor) {Serial.print("Taille de chaîne en octets: ");}
for (int i = 0; i < len; i++){
if (SerialMonitor) {  
GesBuff = Serial.println(buff[i]);}
gesBuff += GesBuff.toInt();}
if (SerialMonitor) {
Serial.println(F("+++++++"));
Serial.println(gesBuff);
Serial.println(F("+++++++"));}}

// *************************************************************************************************

String Politeness(bool WO1F) {
String PReturn = F("");  
if (WO1F) {
if (h >= 0 && h < 12)   {PReturn = "        +++  Bonjour "+WatchOwner1FirstName+"   "+cleanText("Passe une Bonne Journée")+"  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = "        +++  "+cleanText("Passe une Bonne Journée")+"  +++           ";} 
if (h >= 18 && h < 21)  {PReturn = "        +++  "+cleanText("Bonne soirée")+" "+WatchOwner1FirstName+"   "+cleanText("à bientôt")+" +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Bonne nuit "+WatchOwner1FirstName+" +++           ";}} else {
if (h >= 0 && h < 12)   {PReturn = "        +++  "+cleanText("Bonjour... Bonne Journée")+"  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = "        +++  "+cleanText("Passe une Bonne Journée")+"  +++           ";} 
if (h >= 18 && h < 21)  {PReturn = "        +++  "+cleanText("Bonsoir... à bientôt")+" +++           ";} 
if (h >= 21 && h <= 23) {PReturn = F("        +++  Bonne nuit  +++           ");}} 
return cleanText(PReturn); 
}

// *************************************************************************************************

int freeRam() {
 extern int __heap_start, *__brkval;
 int v;
 return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// *************************************************************************************************

/*void TelegramMessages(){
if (BOTtoken != "" && BOTname != "" && BOTusername != "") { 
bot.getUpdates(bot.message[0][1]);   
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++){
// bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
if (SerialMonitor) {Serial.println(bot.message[i][4]+ "   "+bot.message[i][5]);}
  }
// bot.message[0][0] = "";   // All messages have been replied - reset new messages  
}
}*/

// *************************************************************************************************
// *************************************************************************************************
