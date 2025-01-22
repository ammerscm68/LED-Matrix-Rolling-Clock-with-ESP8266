// **************************** Mes Fonctions ***************************************************
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
if (HolidayDate == "14.2") {HoliDayText = F("Saint Valentin");} // La Saint-Valentin (Pas un Jour Férié – Exception Spéciale)
if (HolidayDate == "1.5") {HoliDayText = F("Fête du travail");} // Fête du travail
if (HolidayDate == "1.8") {HoliDayText = F("Victoire 1945");} // Victoire 1945
if (HolidayDate == "14.7") {HoliDayText = F("Fête Nationale Française");} // Fête Nationale Française
if (HolidayDate == "15.8") {HoliDayText = F("Assomption");} // Assomption
if (HolidayDate == "1.11") {HoliDayText = F("Toussaint");} // Toussaint
if (HolidayDate == "11.11") {HoliDayText = F("Armistice 1918");} // Armistice 1918
if (HolidayDate == "25.12") {HoliDayText = F("Noël");} // Noël
if (HolidayDate == "31.12") {HoliDayText = F("Réveillon du Nouvel An");}   // Réveillon du Nouvel An (Pas de Vacances – Cas Exceptionnel)

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

if (HoliDayText != "") {
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
Serial.println("Aujourd'hui : Le Réveillon du Nouvel An"); // Voir le texte du Réveillon du Nouvel An
Serial.println(F(""));}}
if (Display && !GHStart) { // Sortie HoliDayText lorsque l'affichage est activé
if (HolidayDate != "1.1") {
HT = cleanText(HT);     
printStringWithShift(HT.c_str(), ScrollTextTime);
if (HolidayDate == "14.2") {ValentinesDay();}} else { 
HT = cleanText("          +++     Bonne Année     +++                ");   
printStringWithShift("           ", ScrollTextTime); 
for (int i=0; i < 3; i++){ // 3x Vœux de Nouvel An   
printStringWithShift(HT.c_str(), ScrollTextTime);}
ClockScrollIn(); SIC = false;}}} else {
if (SerialMonitor) {
Serial.println(F(""));   
Serial.println(F("Jours Fériés : Pas de Jours Fériés"));
Serial.println(F(""));}}} // Fin de Fonction "void Greetings_and_Holiday()"

// *************************************************************************************************

void InitSerial(int Baud, int Wait) { 
Serial.begin(Baud);  
while (!Serial) {Serial.print(F("*"));}
printStringWithShift("COM !  ",38); // COM INIT
for (int i = 0; i < Wait; i++) {
Serial.print(F("*")); MyWaitLoop(100);}
printStringWithShift("           ",38); 
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
// *** Charger les Paramètres .... ***
swait = true; // pendant le Chargement, aucun identifiant de Site Web - activé
bool MDTime; // Variable auxiliaire
// Activer le Système de fichiers WEMOS D1 Little
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); // attend les données

if (littleFSInit == true) {
if (SerialMonitor) {Serial.println(F("Charger les Ajustements de l’Horloge ..."));}
  // Charger les données d'Accès WiFi
  if (LittleFS.exists("/ssid.txt")){
  ssid = SetupLoad("ssid.txt").c_str();
  } else {ssid = F("");}
  if (LittleFS.exists("/ssid-passwd.txt")){
  password = SetupLoad("ssid-passwd.txt").c_str();
  } else {password = F("");}

  // Charger le fuseau Horaire
  if (LittleFS.exists("/clocktimezone.txt")){
  MCTZ = SetupLoad("clocktimezone.txt").c_str();}

  // Charger le propriétaire de l'Horloge
 if (LittleFS.exists("/owner1-firstname.txt")){
 WatchOwner1FirstName = SetupLoad("owner1-firstname.txt").c_str();}
 if (LittleFS.exists("/owner1-lastname.txt")){
 WatchOwner1LastName = SetupLoad("owner1-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner1-gender.txt")){
 Gender1 = SetupLoad("owner1-gender.txt").c_str();} 
 if (LittleFS.exists("/owner1-birthday.txt")){
 Birthday1 = SetupLoad("owner1-birthday.txt").c_str();} 
 if (LittleFS.exists("/owner2-firstname.txt")){
 WatchOwner2FirstName = SetupLoad("owner2-firstname.txt").c_str();} 
 if (LittleFS.exists("/owner2-lastname.txt")){
 WatchOwner2LastName = SetupLoad("owner2-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner2-gender.txt")){
 Gender2 = SetupLoad("owner2-gender.txt").c_str();} 
 if (LittleFS.exists("/owner2-birthday.txt")){
 Birthday2 = SetupLoad("owner2-birthday.txt").c_str();} 
 if (LittleFS.exists("/owner3-firstname.txt")){
 WatchOwner3FirstName = SetupLoad("owner3-firstname.txt").c_str();} 
 if (LittleFS.exists("/owner3-lastname.txt")){
 WatchOwner3LastName = SetupLoad("owner3-lastname.txt").c_str();} 
 if (LittleFS.exists("/owner3-gender.txt")){
 Gender3 = SetupLoad("owner3-gender.txt").c_str();} 
 if (LittleFS.exists("/owner3-birthday.txt")){
 Birthday3 = SetupLoad("owner3-birthday.txt").c_str();}

 if (LittleFS.exists("/midnightghost.txt")){
 MidnightGhost = SetupLoad("midnightghost.txt").c_str();} 
 if (SerialMonitor) {
 Serial.println(F(""));  
 Serial.println(F("Afficher les Fantômes de Minuit ?: [YES / NO] ")+ MidnightGhost);
 Serial.println(F(""));}

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
  // GMCMap-History ID / GMT Offset
  if (LittleFS.exists("/gmcmaphistoryid.txt")){
  GMCMapHistoryID = SetupLoad("gmcmaphistoryid.txt");
  GMCMapHistoryID.trim();} else {GMCMapHistoryID = F("");} // www.gmcmap.com 
  if (LittleFS.exists("/gmcmapgmtoffset.txt")){
  GMCMapGMTOffset = SetupLoad("gmcmapgmtoffset.txt");
  GMCMapGMTOffset.trim();} else {GMCMapGMTOffset = F("");} // www.gmcmap.com Décalage GMT

   // Désactivez OpenWeatherMAP si aucune clé API ou identifiant de ville n'est disponible
   if (weatherKey != "" && cityID != "") {
   AccessOpenWeatherMap = true;} else {
   AccessOpenWeatherMap = false;}
   // Désactivez NewsData si aucune clé API n'est disponible
   if (newsKey != "") {AccessNewsData = true;} else {
   AccessNewsData = false;}   
   // GMCMap Désactiver si aucun HistoryID n'est disponible
   if (GMCMapHistoryID != "" && GMCMapGMTOffset != "") {AccessGMCMap = true;} else {
   AccessGMCMap = false;}  

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

    // GMCMap function
  if (AccessGMCMap) { 
  if (LittleFS.exists("/status_GMCMapFunction.txt")){
  String GMCF = SetupLoad("status_GMCMapFunction.txt");
  GMCF.trim();
  if (GMCF == "" || GMCF == "On") {GMCMapFunction = true; StatusGMCMap = F("enabled");}
  if (GMCF == "Off") {GMCMapFunction = false; StatusGMCMap = F("disabled");}} else {
  GMCMapFunction = true; StatusGMCMap = F("enabled");}} else {
  GMCMapFunction = false; StatusGMCMap = F("disabled");}  

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

  // Vérifiez la plausibilité des informations pour l'affichage matriciel marche/arrêt automatique
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

  LoadClockStatus(); // Charger le dernier statut de l'Horloge
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
    GMCMapHistoryID = F(""); // www.GMCMap.com - History ID
    GMCMapGMTOffset = F(""); // www.GMCMap.com - GMTOffset
    InTempOffsetInt = -1; // Décalage de température
    InHumOffsetInt = -1; // Compensation d'humidité
    StartSound = F("Off"); // pas de buzzer
    HourSound = F("Off"); // pas de buzzer
    MCTZ = "Berlin, Frankfurt, Vienna, Paris, Madrid"; // Fuseau Horaire par défaut
    SaveClockStatus("Système de Fichiers défectueux !"); // Enregistrer l'état de l'Horloge
    LoadClockStatus(); // Charger le dernier statut de l'Horloge
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
  swait = true;  // pendant que Enregistrer aucun identifiant de Site Web - activé
  if (littleFSInit == true) {
  if (SerialMonitor) {Serial.println(F("Enregistrer les Réglages de l'Horloge ..."));}    
  // Trim
  TempSSID.trim(); // SSID WLAN
  newsKey.trim(); // Clé API NewsData
  DisplayOnTime.trim(); // Heure d'allumage de l'affichage matriciel
  DisplayOffTime.trim(); // Heure d'extinction de l'affichage matriciel
  InTempOffset.trim(); // Décalage du capteur DHT (Température)
  InHumOffset.trim(); // Décalage du capteur DHT (humidité de l'air)
  WatchOwner1FirstName.trim(); // 1er Propriétaire - Prénom
  WatchOwner1LastName.trim(); // 1er Propriétaire - Nom de Famille
  Gender1.trim(); // 1er Propriétaire - Sexe
  Birthday1.trim(); // 1er Propriétaire - Date de Naissance
  WatchOwner2FirstName.trim(); //  2ème Propriétaire - Prénom
  WatchOwner2LastName.trim(); // 2ème Propriétaire - Nom de Famille
  Gender2.trim(); // 2ème Propriétaire - Sexe
  Birthday2.trim(); // 2ème Propriétaire - Date de Naissance
  WatchOwner3FirstName.trim(); // 3ème Propriétaire - Prénom
  WatchOwner3LastName.trim(); // 3ème Propriétaire - Nom de Famille
  Gender3.trim(); // // 3ème Propriétaire - Sexe
  Birthday3.trim(); // 3ème Propriétaire - Date de Naissance
  
  // Enregistrer le SSID
  SetupSave("ssid.txt", TempSSID); // SSID WLAN
  SetupSave("ssid-passwd.txt", password); // Mot de passe WiFi

  // Sauvegarde du fuseau Horaire
  SetupSave("clocktimezone.txt", MCTZ);

  // Sauver le Propriétaire de l'Horloge
  if (WatchOwner1FirstName != "") {
  SetupSave("owner1-firstname.txt", WatchOwner1FirstName);} else {
  if (LittleFS.exists("/owner1-firstname.txt")){LittleFS.remove("/owner1-firstname.txt");}}  
  if (WatchOwner1LastName != "") {
  SetupSave("owner1-lastname.txt", WatchOwner1LastName);} else {
  if (LittleFS.exists("/owner1-lastname.txt")){LittleFS.remove("/owner1-lastname.txt");}}  
  if (Gender1 != "") {
  SetupSave("owner1-gender.txt", Gender1);} else {
  if (LittleFS.exists("/owner1-gender.txt")){LittleFS.remove("/owner1-gender.txt");}}  
  if (Birthday1 != "") {
  SetupSave("owner1-birthday.txt", Birthday1);} else {
  if (LittleFS.exists("/owner1-birthday.txt")){LittleFS.remove("/owner1-birthday.txt");}}  
  if (WatchOwner2FirstName != "") {
  SetupSave("owner2-firstname.txt", WatchOwner2FirstName);} else {
  if (LittleFS.exists("/owner2-firstname.txt")){LittleFS.remove("/owner2-firstname.txt");}}  
  if (WatchOwner2LastName != "") {
  SetupSave("owner2-lastname.txt", WatchOwner2LastName);} else {
  if (LittleFS.exists("/owner2-lastname.txt")){LittleFS.remove("/owner2-lastname.txt");}}  
  if (Gender2 != "") {
  SetupSave("owner2-gender.txt", Gender2);} else {
  if (LittleFS.exists("/owner2-gender.txt")){LittleFS.remove("/owner2-gender.txt");}}  
  if (Birthday2 != "") {
  SetupSave("owner2-birthday.txt", Birthday2);} else {
  if (LittleFS.exists("/owner2-birthday.txt")){LittleFS.remove("/owner2-birthday.txt");}}  
  if (WatchOwner3FirstName != "") {
  SetupSave("owner3-firstname.txt", WatchOwner3FirstName);} else {
  if (LittleFS.exists("/owner3-firstname.txt")){LittleFS.remove("/owner3-firstname.txt");}}  
  if (WatchOwner3LastName != "") {
  SetupSave("owner3-lastname.txt", WatchOwner3LastName);} else {
  if (LittleFS.exists("/owner3-lastname.txt")){LittleFS.remove("/owner3-lastname.txt");}}  
  if (Gender3 != "") {
  SetupSave("owner3-gender.txt", Gender1);} else {
  if (LittleFS.exists("/owner3-gender.txt")){LittleFS.remove("/owner3-gender.txt");}}  
  if (Birthday3 != "") {
  SetupSave("owner3-birthday.txt", Birthday3);} else {
  if (LittleFS.exists("/owner3-birthday.txt")){LittleFS.remove("/owner3-birthday.txt");}}

  SetupSave("midnightghost.txt", MidnightGhost);
  
  // Clé API OpenWeatherMap et Identifiant de Ville OpenWeatherMap
  if (weatherKey != "" && cityID != "") {
  SetupSave("owm-apikey.txt", weatherKey);
  SetupSave("owm-cityid.txt", cityID);} else {
  if (LittleFS.exists("/owm-apikey.txt")){LittleFS.remove("/owm-apikey.txt");}
  if (LittleFS.exists("/owm-cityid.txt")){LittleFS.remove("/owm-cityid.txt");}} 
  // NewsData API-Key
  if (newsKey != "") {
  SetupSave("newsdata-apikey.txt", newsKey);} else {
  if (LittleFS.exists("/newsdata-apikey.txt")){LittleFS.remove("/newsdata-apikey.txt");}}
  // Données de la carte GMC 
  if (GMCMapHistoryID != "") {
  SetupSave("gmcmaphistoryid.txt", GMCMapHistoryID);} else {
  if (LittleFS.exists("/gmcmaphistoryid.txt")){LittleFS.remove("/gmcmaphistoryid.txt");}}
  if (GMCMapGMTOffset != "") {
  SetupSave("gmcmapgmtoffset.txt", GMCMapGMTOffset);} else {
  if (LittleFS.exists("/gmcmapgmtoffset.txt")){LittleFS.remove("/gmcmapgmtoffset.txt");}}
  // Heure d'activation de l'affichage Matriciel
  if (DisplayOnTime != ""){
  SetupSave("display-on-time.txt", DisplayOnTime);} else {
  if (LittleFS.exists("/display-on-time.txt")){LittleFS.remove("/display-on-time.txt");}}
  // Heure d'extinction de l'affichage Matriciel
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
if (PushOverTest) {WSR = 20000;} else {WSR = 800;}   // Taux de Rafraîchissement du Serveur Web | par défaut = 800
if (millis() - hwsTime > WSR) { 
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
if (SerialMonitor) {Serial.println(F("Connexion WiFi déconnectée ..."));} 
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

// Vérifier une chaîne pour voir si elle est de type float et accepter à partir du point décimal
boolean IsFloat(String str) {  
bool bPoint=false;  
if(str.length()<1){return false;} else {
for(int i = 1; i < str.length(); i++) {
if(str.charAt(i) == '.') {bPoint=true; break;}}
return bPoint;}}

// *************************************************************************************************																												 
void ResetAPCounter() {
String ResetValue[] = {"0","2", "4", "6", "8", "10", "12", "14" ,"16", "18", 
                       "20", "22", "24", "26", "28", "30", "32", "34", "36",
                       "38", "40", "42", "44", "46", "48", "50", "52", "54",
                       "56", "58"}; 
for (int i=0; i <= 30; i++){                      
if  (String(m)+":"+String(s) == ResetValue[i]+":2") { 
APStart = 0;}}} // Compteur du bouton "Réinitialiser le point d'accès" toutes les 2 minutes

// *************************************************************************************************

void Download() {
if (ClockStart) { 
char dl = char('~' + 24); //  // Flèche vers le bas pour Télécharger  
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);}}

// *************************************************************************************************

void DownloadComplet() {
if (ClockStart) { 
// Publicité claire    
clr(); 
refreshAll();}}

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
SHTMLString = F(""); // Chaîne vide
freeRam();} // Defragment Heap

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
if (h >= 0 && h < 12)   {PReturn = "        +++  Bonjour "+WatchOwner1FirstName+"   Passe une Bonne Journée  +++           ";} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Passe une Bonne Journée  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = "        +++  Bonne soirée "+WatchOwner1FirstName+"   à Bientôt  +++           ";} 
if (h >= 21 && h <= 23) {PReturn = "        +++  Bonne nuit "+WatchOwner1FirstName+" +++           ";}} else {
if (h >= 0 && h < 12)   {PReturn = F("        +++  Bonjour... Bonne Journée  +++           ");} 
if (h >= 12 && h < 18)  {PReturn = F("        +++  Passe une Bonne Journée  +++           ");} 
if (h >= 18 && h < 21)  {PReturn = F("        +++  Bonsoir... à bientôt  +++           ");} 
if (h >= 21 && h <= 23) {PReturn = F("        +++  Bonne nuit  +++           ");}} 
return cleanText(PReturn);}

// *************************************************************************************************

int freeRam() {
 extern int __heap_start, *__brkval;
 int v;
 return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// *******************************************************************************************************************

void StatusWarningLED(String LEDStatus) {
if (HWLEDPin != -1) {
if (LEDStatus == "CHECK") {digitalWrite(HWLEDPin, HIGH); MyWaitLoop(150); digitalWrite(HWLEDPin, LOW);} else {
if (LEDStatus == "HIGH") {if (Display) {digitalWrite(HWLEDPin, HIGH);}}
if (LEDStatus == "LOW") {
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX" && GMCMapValue != "NoGMCMapValue" && GMCMapToHigh == "") {  
digitalWrite(HWLEDPin, LOW);}}}}}

// *******************************************************************************************************************

String WiFiSignalStrength(bool MStart) {
long WifiRSSI = WiFi.RSSI(); // Intensité du Signal WiFi en dBm
String WiFiSignalQuality = F(""); 
if (SerialMonitor) {Serial.println("Force du Signal WiFi: "+ String(WifiRSSI) +" dBm");}
if (WifiRSSI >= -40) {
if (MStart) {WiFiSignalQuality = F("very good");} else {  
WiFiSignalQuality = "<h7>Force du Signal WiFi: &nbsp;&nbsp; très bien ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -40 && WifiRSSI >= -60) {
if (MStart) {WiFiSignalQuality = F("good");} else {   
WiFiSignalQuality = "<h7>Force du Signal WiFi: &nbsp;&nbsp; bien ("+String(WifiRSSI)+" dBm)</h7>";}} 
if (WifiRSSI < -60 && WifiRSSI >= -70) {
if (MStart) {WiFiSignalQuality = F("sufficient");} else {   
WiFiSignalQuality = "<h7>Force du Signal WiFi: &nbsp;&nbsp; suffisant ("+String(WifiRSSI)+" dBm)</h7>";}}
if (WifiRSSI < -70 && WifiRSSI >= -80) {
if (MStart) {WiFiSignalQuality = F("bad");} else {
WiFiSignalQuality = "<h10>Force du Signal WiFi: &nbsp;&nbsp; mauvais ("+String(WifiRSSI)+" dBm)</h10>";}}  
if (WifiRSSI < -80) {
 if (MStart) {WiFiSignalQuality = F("very bad");} else {
WiFiSignalQuality = "<h10>Force du Signal WiFi: &nbsp;&nbsp; très mauvais ("+String(WifiRSSI)+" dBm)</h10>";}} 
return WiFiSignalQuality;}

// *************************************************************************************************

void LoadClockStatus() {   
if (LittleFS.exists("/clockstatus.txt")){
ClockStatus = SetupLoad("clockstatus.txt").c_str();
SetupSave("clockstatus.txt", "OK");
} else {ClockStatus = "OK"; SetupSave("clockstatus.txt", ClockStatus);}}

// *************************************************************************************************

void SaveClockStatus(String StatusText) {
SetupSave("clockstatus.txt", StatusText);}

// *************************************************************************************************

void ValentinesDay() {
String VDHart = F("");
for (int i=0; i <= 3; i++){ 
VDHart += String(Hart2);   
printStringWithShift(VDHart.c_str(), ScrollTextTime+5);
MyWaitLoop(2000);
printStringWithShift("             ", ScrollTextTime+5);}}

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
