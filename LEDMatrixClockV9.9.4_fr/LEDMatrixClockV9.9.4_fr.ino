/*******************************************************************************************
 ** Author:  Mario Ammerschuber                                                           **
 ** Date:    November 2024                                                                **
 ** Version: 9.9.4 (Français)                                                             ** 
 ** Board:   LOLIN (WEMOS) D1 R2 et mini                                                  **  
 ** Sensor:  DHT11 ou DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Jaune                                                                    **  
 ** Website: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          **  
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
String LMCVersion = "9.9.4 fr";
/************************( Importing the libraries )************************/

#include <ESP8266WiFi.h>                    // Installer via la bibliothèque
#include <WiFiClientSecure.h>               // Installer via la bibliothèque
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>              // HTTPClient - Installer via la bibliothèque
#include <ESP8266WebServer.h>               // Webserver - Installer via la bibliothèque
#include <ArduinoOTA.h>                     // Installer via la bibliothèque
#include <ArduinoJson.h>                    // Installer via la bibliothèque Version: 7
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include "LittleFS.h"                       // Little Système de fichiers - Installer via la bibliothèque
#include <DHTesp.h>                         // DHT-11 / DHT-22 Capteur de température - Installer via la bibliothèque
#include <pitches.h>                        // pour buzzer passif

ESP8266WebServer server(80); // Serveur Web sur le port 80     border-radius: 50\%

/********************( Définir des variables et des constantes globales )********************/
String BoardName = "Wemos/Lolin D1 mini"; // Nom du tableau ESP8266

// ###################################### Propriétaire de l'horloge ######################################################
// ************************ Propriétaire principal ************************************  [pour void Greetings_and_Holiday()]
String WatchOwner1FirstName = ""; // Prénom du propriétaire principal - Exemple: Mike
String WatchOwner1LastName  = ""; // Nom de famille du propriétaire principal - Exemple: Johnson
String Gender1 = "";  // male=masculine | female=female
String Birthday1 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 2. Propriétaire (Optional) **********************************************
String WatchOwner2FirstName = ""; // Prénom du deuxième propriétaire
String WatchOwner2LastName  = ""; // Nom de famille du 2ème propriétaire (facultatif) - n'est pas encore utilisé
String Gender2 = "";  // male=masculine | female=female
String Birthday2 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************
// ************************ 3. Propriétaire (Optional) **********************************************
String WatchOwner3FirstName = ""; // Prénom du 3ème propriétaire
String WatchOwner3LastName  = ""; // Nom de famille du 3ème propriétaire (facultatif) - n'est pas encore utilisé
String Gender3 = "";  // male=masculine | female=female
String Birthday3 = ""; // Example: 17.3 or 2.4 or 5.10 or 23.11
// **************************************************************************************************

// ########################## Adresse IP du point d'accès (Portail Captif) #####################################################################
int APIPA1 = 192; int APIPA2 = 168; int APIPA3 = 4; int APIPA4 = 1; // Défaut = 192.168.4.1
// #############################################################################################################################################

// #############################################################################################################################################
int ScrollClockTime  = 80; // Délai de défilement lorsque le temps est animé   80 = par Défaut
// #############################################################################################################################################

// #################### Capteur de température et d'humidité (DHT 11 ou DHT 22) ################################################################
bool DHT11Sensor = true; // true = DHT11  /  false = DHT22   Capteur    (Afficher la Température en Celsius)
// #############################################################################################################################################

// ########################## Montrer un fantôme à minuit ######################################################################################
bool MidnightGhost = true; // true = afficher un fantôme à minuit / false= ne pas afficher de Fantôme à minuit
// #############################################################################################################################################

// #############################################################################################################################################
bool SerialMonitor = false; // Moniteur série   true = Sur / false = Désactivé
// #############################################################################################################################################

// ############################################################################################################################################# 
int MinComfortHum = 32; // Humidité minimale de confort (par défaut = 32) - Valeur en %
int MaxComfortHum = 68; // Confort maximum Humidité (par défaut = 68) - Valeur en %
// #############################################################################################################################################

// ########################### !!!!!!!!!!! #####################################################################################################
bool FormatFileSystem = false; // true = Supprime tous les fichiers stockés dans le système de fichiers !!!  /  false = défaut
// #############################################################################################################################################

// #############################################################################################################################################
const int PushButtonPin = D3; // Matériel-PushButton-Pin (D3 = GPIO 0)
const int buzzerPin = D2; // initialisation du buzzer passif (D2 = GPIO 4) || Pour microcontrôleur ESP8266 (D4 ne fonctionne pas avec le microcontrôleur ESP8266)
const int HWLEDPin = D5; // Broche LED pour avertissement d'humidité élevée (D5 = GPIO 14)
// #############################################################################################################################################

// ###### Horaires d'affichage pour "OpenWeatherMAP" et "NewsData" ainsi que pour la température et l'humidité intérieure/extérieure (capteur DHT) ####
// ********** La minute 0 est réservée à l'évaluation de l'humidité - Ne l'utilisez pas ! ***********

// Actions toutes les 3 minutes
int OnlyTempViewTimeCount = 6; // Nombre de fois
String OnlyTempViewTime[] = {"3","12","21","36","48","57"}; // Afficher uniquement la température intérieure et extérieure (minute)
int OnlyHumViewTimeCount = 5; // Nombre de fois
String OnlyHumViewTime[] = {"6","24","33","39","51"}; // Afficher uniquement l'humidité de l'air intérieur et extérieur (minute)
int DateNewsViewTimeCount = 5; // Nombre de fois
String DateNewsViewTime[] = {"9","18","27","42","54"}; // Date actuelle, salutations et nouvelles (minute)
int FullWeatherViewTimeCount = 3; // Nombre de fois
String FullWeatherViewTime[] = {"15","30","45"}; // Météo complète (minute)

/// Actions every 5 Minutes
/*int OnlyTempViewTimeCount = 3; // Nombre de fois
String OnlyTempViewTime[] = {"20", "35", "50"}; // Afficher uniquement la température intérieure et extérieure (minute)
int OnlyHumViewTimeCount = 3; // Nombre de fois
String OnlyHumViewTime[] = {"5", "30", "55"}; // Afficher uniquement l'humidité de l'air intérieur et extérieur (minute)
int DateNewsViewTimeCount = 3; // Nombre de fois
String DateNewsViewTime[] = {"10", "25", "40"}; // Date actuelle, salutations et nouvelles (minute)
int FullWeatherViewTimeCount = 2; // Nombre de fois
String FullWeatherViewTime[] = {"15", "45"}; // Météo complète (minute)*/
//##############################################################################################################################################

DHTesp dht; // Capteur DHT pour la mesure de la température à l'intérieur (par défaut = D1)

String ssid = "";       // SSID du réseau local 
String TempSSID = "";   // SSID temporaire du réseau local 
String password = "";   // Mot de passe sur le réseau 
String weatherKey = ""; // Clé API de OpenWeatherMap - https://openweathermap.org
String cityID = "";     // ID de ville OpenWeatherMap 
bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;

String WeatherLocation = "";
String StatusOpenWeatherMap = "";
bool FirstWeatherData = true;

String SecureAppToken = ""; // Pushover Jeton d'application sécurisé
String UserToken = ""; // Pushover Jeton utilisateur
String PushoverDevice1 = ""; // Pushover Appareil (Exemple : Smartphone)
String PushoverDevice2 = ""; // Pushover Appareil
String PushoverDevice3 = ""; // Pushover Appareil
String PushoverMessageSound = "none"; // Pushover Son des messages
bool IsPushover = false; // Pushover désactivé par défaut

// Pool de serveurs NTP pour la France:
static const char ntpServerName[] = "fr.pool.ntp.org";    //Find local Server under http://www.pool.ntp.org/zone/fr
const int timeZone = 0;                     // 0 lorsque vous travaillez avec <Timezone.h> !
WiFiUDP Udp;
unsigned int localPort = 1701; // port local pour écouter les paquets UDP
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// SalutationsAujourd'hui
bool GreetingsToday = false;

// Pour le redémarrage et la réinitialisation de l'horloge
bool ResetNow = false; // Pour la réinitialisation de l'horloge
bool RebootNow = false; // Pour le redémarrage de l'horloge

uint16_t bootTime; // Heure de démarrage Horloge matricielle LED

String newsKey = ""; // Clé NewsData
String APINews = "";
String WPAPINews = "";
String StatusNewsData = "";
bool NewsOutOfCredits = false;

String DisplayOnTime = "";
String DisplayOffTime = "";
String FDOnTime = "";
String FDOffTime = "";
String Brightness = "L1";
bool MEP = false;
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool ClockWiFiConnectFail = false; // fausse norme
bool SIC = false; // Faire défiler l'horloge

float temp;
String SText = "";
String weatherString = "";
String WPweatherString = "";
String ATemp = "";
String AHum = "";
String InHumMessage = "";
String InHumidityStatus = "";
String DateString = "";
String cip = "";
String cip2 = "";

String ct = ""; // texte propre

#define NUM_MAX 4

// pour module ESP-01 - PINs MAX7219 Écran matriciel LED
//#define DIN_PIN 2 // D4 - ESP-01
//#define CS_PIN  3 // D9/RX - ESP-01
//#define CLK_PIN 0 // D3 - ESP-01

// pour NodeMCU 1.0 et WEMOS D1 mini - PINs MAX7219 LED-Matrix Display
#define DIN_PIN 15  // D8 - NodeMCU ou WEMOS D1 mini
#define CS_PIN  13  // D7 - NodeMCU ou WEMOS D1 mini
#define CLK_PIN 12  // D6 - NodeMCU ou WEMOS D1 mini

#include "max7219.h" // Écran matriciel LED MAX7219
#include "fonts.h"

int  WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound = "";
String HourSound = "";

String GT =  ""; // Salutations
String GT1 = ""; // Salutations
String HT =  ""; // Vacances
String HT1 = ""; // Vacances
int GNCounter = 1; // Comptoir pour les salutations/vacances et nouvelles

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
unsigned long hwsTime1 = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
String OWMLastConnectionDateTime = ""; // Dernière heure de connexion OpenWeatherMap
String NewsDataLastConnectionDateTime = ""; // Dernière heure de connexion
long localMillisAtUpdate = 0;
int ScrollTextTime = 0; 
int waitloop = 0;
int WiFiConnectLoop = 0;
int MaxWiFiConnectLoop = 30;  // Nombre maximum de boucles lorsqu'il est connecté au WiFi
bool StartFail = false;
bool Display = true; // Mode d'affichage - SUR
int DoublePointStatus = 1; // Statut Côlon - Par défaut = SUR permanent
bool PDP = false;
bool EnhancedFunctions = true; // Fonctions supplémentaires SUR / Désactivé - Standard = SUR (Date, Jours fériés, Anniversaires, etc.)
bool WeatherFunctions = true; // Fonction de données météorologiques activée/désactivée – Par défaut = activé (clé API OpenWeatherMap et identifiant de ville disponibles)
bool NewsDataFunction = true; // Fonction d'actualités actuelles SUR / Désactivé - Par défaut = SUR (clé API NewsData disponible)
int StartStopServers = 0;
int APStart = 0;
bool ClockStart = true;
int IntInTempValue = -1;
int IntInHumValue = -1;
int humidity = -1; // Humidité d’OpenWeatherMAP
int InTempCounter = 0; // Compteur de température intérieure
int InHumCounter = 0; // Humidité
String InTempValue = "-"; // Température intérieure
String InHumValue = ""; // Humidité
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset = "";   // Veuillez vous référer au portail captif
String InHumOffset = "";   //  Veuillez vous référer au portail captif
int InTempOffsetInt = -1; //   Décalage de la température intérieure (Exemple : -1 = décalage --> -1 Grad ou +1 = décalage --> +1 Grad - dans le portail captif)
int InHumOffsetInt = -1; //    Décalage de l'humidité intérieure (Exemple : -1 = décalage --> -1 % ou +1 = décalage --> +1 % - dans le portail captif)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "n/a";

/***************************************** ( Installation ) ****************************************/
void setup() {
// Initialiser la Matrice
initMAX7219();  

LoadMatrixAdjustments(); // Charger les paramètres de l'horloge matricielle LED....

if (SerialMonitor) {
InitSerial(115200, 120); Serial.println("");} // En Attente de Connexion COM  /  Attente Défaut= 120

// #####################################################################################
// pour la luminosité automatique (photorésistance)
// pinMode(A0, INPUT); 
  // Initialize Button
   pinMode(PushButtonPin, INPUT_PULLUP); // (PushButtonPin par défaut = D3)
  // Initialiser l'horloge LED
   pinMode(HWLEDPin, OUTPUT); // (HWLEDPin par défaut = D5)
   // Connectez le capteur DHT au GPIO 5 (D1) - Capteur de température et d'humidité intérieure
   if (DHT11Sensor) {// Capteur DHT11
   dht.setup(5, DHTesp::DHT11);} else { // Capteur DHT22
   dht.setup(5, DHTesp::DHT22);}
// #####################################################################################

   if (SerialMonitor) {
   Serial.println(F("*** Horloge-Matricielle-LED ***"));
   Serial.println("   (Version: "+LMCVersion+")");
   Serial.println(F(""));}

    // Init passif Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (par défaut : buzzerPin = D2)
    noTone(buzzerPin);} // no Sound

  // Formater le petit système de fichiers
  if (FormatFileSystem) {
  if (littleFSInit) {
  LittleFS.format(); 
  ct = cleanText("     +++       Le Système de fichiers a été Formaté !  \
  --- Redéfinissez la Variable -FormatFileSystem- Sur -false- et \
  Téléchargez à Nouveau le Croquis.....");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  ct = cleanText("                              +++\
         L'Horloge va Redémarrer      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(5000); // courte attente
  ESP.restart();}} // Redémarrer l'horloge
  
  if (SerialMonitor) {
  if (ssid == "") {
    Serial.println(F(""));
    Serial.println(F("***** Horloge du Premier Démarrage *****")); // Horloge du premier démarrage
    Serial.println(F(""));}}

  WiFi.hostname("LEDMatriceHorloge"); // Clock Hostname

  if (ssid != ""){
  if (SerialMonitor) {  
  Serial.print(F("Connectez-vous avec le SSID WiFi: "));
  Serial.println(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // Établir une connexion sans fil
  } else {ClockWiFiConnectFail = true;} // Contournez l'enregistrement WLAN lorsque vous démarrez l'horloge pour la première fois ! - Démarrez le point d'accès immédiatement
  
  if (!ClockWiFiConnectFail) { 
  // Afficher l'adresse IP attribuée sur Matrix
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             Adresse IP:  "+cip+"                "; // Determined IP address (DHCP)

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Afficher l'adresse IP MyWaitLoop(10); // courte attente

  if (SerialMonitor) {Serial.println(F("Démarrer le serveur UDP  ..."));}
  WiFiUDPStart();  // Démarrer le serveur UPD

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {Serial.println(F("Démarrer le serveur Arduino OTA/DNS ... - URL: http://ledmatricehorloge.local"));}
  ArduinoOTA.setHostname("ledmatricehorloge");
  ArduinoOTA.setPassword("74656"); // Mot de passe par défaut pour OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {Serial.println("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; 
  digitalWrite(HWLEDPin, LOW); ESP.restart();}); // Redémarrer en cas d'erreur OTA
  // Maintenant, démarrez Arduino maintenant OTA/DNS
  ArduinoOTA.begin();
  // Démarrer le serveur TCP (HTTP)
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  Serial.println(F(""));
  Serial.println(F("OpenWeatherMap a été désactivé - Aucune donnée météo"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}

  if (!NewsDataFunction) {
  Serial.println(F(""));
  Serial.println(F("NewsData a été désactivé - Aucune actualité actuelle"));
  Serial.println(F("")); 
   } else {Serial.println(F(""));Serial.println(F(""));}}
   
    // Obtenez toutes les informations de votre LittleFS
    if (littleFSInit) {  
    if (SerialMonitor) {  
    Serial.println(F("Petit Init du système de fichiers - terminé."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.println(F("Informations sur le système de fichiers."));
    Serial.print(F("Espace total:      "));
    Serial.print(fs_info.totalBytes);
    Serial.println(F("byte"));
    Serial.print(F("Espace total utilisé: "));
    Serial.print(fs_info.usedBytes);
    Serial.println(F("byte"));
    Serial.print(F("Taille du bloc:       "));
    Serial.print(fs_info.blockSize);
    Serial.println(F("byte"));
    Serial.print(F("Taille des pages:        "));
    Serial.print(fs_info.totalBytes);
    Serial.println(F("byte"));
    Serial.print(F("Nombre maximum de fichiers ouverts:   "));
    Serial.println(fs_info.maxOpenFiles);
    Serial.print(F("Longueur maximale du chemin:  "));
    Serial.println(fs_info.maxPathLength);
    Serial.println(F(""));}
    // Ouvrir le dossier répertoire
    Dir dir = LittleFS.openDir("/");
    // Cycle tout le contenu
    while (dir.next()) {
        // obtenir le nom du fichier
        if (SerialMonitor) {Serial.print(dir.fileName());
        Serial.print(F(" - "));}
        // Si l'élément a une taille d'affichage, sinon il écrit 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            if (SerialMonitor) {Serial.println(f.size());}
            f.close();
        }else{if (SerialMonitor) {Serial.println(F("0"));}}}
   if (SerialMonitor) {
   Serial.println(F("_______________________________"));
   Serial.println(F(""));}} else {
   if (SerialMonitor) {  
   Serial.println(F("Init du petit système de fichiers - Erreur."));}}
  if (SerialMonitor) {   
  Serial.println("StartTime: "+FormatedDateTimeFR(false));
  Serial.println(F(""));
  Serial.println(F(""));}

  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Vérifiez si le capteur DHT est connecté (3 tentatives)
  // Vérifier la température
   for (int i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "-") {
   break;}} 
   // Vérifier l'humidité
   for (int i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

  if (InTempValue == "-") {InTempValue = "!";}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("Le Capteur DHT11 est Connecté. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("Le Capteur DHT22 est Connecté. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("Le Capteur DHT est Défectueux !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("Aucun Capteur DHT Connecté"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

    // Serveur Web - si le lien est erroné, revenez à la page d'accueil
  server.onNotFound(SiteNotFound);

  // Serveur Web - Page d'accueil, modes de luminosité et Test d'affichage
  server.on("/", handleRoot);
  server.on("/moff", MatrixOff);
  server.on("/mon",MatrixOn);
  server.on("/mb1", BrightnessLevel1);
  server.on("/mb2", BrightnessLevel2);
  server.on("/mb3", BrightnessLevel3);
  server.on("/mb4", BrightnessLevel4);
  server.on("/mb5", BrightnessLevel5);
  server.on("/cdof", ChangeDisplayAutoOnOff);
  server.on("/cdofc", ChangeDisplayAutoOnOffCancel);
  server.on("/mdt", DisplayCheck);

  // Statut Côlon
  server.on("/sdflash", DoublePointFlash);
  server.on("/sdon", DoublePointOn);
  server.on("/sdoff", DoublePointOff);

  // Vitesse de défilement
  server.on("/vlt1", ScrollTextTimeLevel1);
  server.on("/vlt2", ScrollTextTimeLevel2);
  server.on("/vlt3", ScrollTextTimeLevel3);
  server.on("/vlt4", ScrollTextTimeLevel4);
  server.on("/vlt5", ScrollTextTimeLevel5);
  server.on("/vlt6", ScrollTextTimeLevel6);
  server.on("/vlt7", ScrollTextTimeLevel7);
  server.on("/vlt8", ScrollTextTimeLevel8);

  // Activer/désactiver des fonctions supplémentaires
  server.on("/zfon", EnhancedFunctionsOn);
  server.on("/zfoff", EnhancedFunctionsOff);

  // Activer/Désactiver les données météorologiques
  if (AccessOpenWeatherMap) {
  server.on("/owmfon", WeatherFunctionsOn);
  server.on("/owmfoff", WeatherFunctionsOff);
  server.on("/aowmn", ActualWeather);}

   // Activer/Désactiver les Actualités actuelles
  if (AccessNewsData) {
  server.on("/nafon", NewsDataFunctionOn);
  server.on("/nafoff", NewsDataFunctionOff);
  server.on("/aapin", ActualAPINews);}

  // Pushover Message de test
  if (IsPushover) {
  server.on("/potmdevice1", TMPushoverDevice1);
  server.on("/potmdevice2", TMPushoverDevice2);
  server.on("/potmdevice3", TMPushoverDevice3);}

  // Configuration du son du buzzer
  if (IsBuzzer) { 
  server.on("/sspbson", StartSoundOn);
  server.on("/sspbsoff", StartSoundOff);
  server.on("/hspbson", HourSoundOn);
  server.on("/hspbsoff", HourSoundOff);}

  // Effacer les paramètres - Réinitialiser
  server.on("/reset", ResetClockSettings); 
  server.on("/goreset", ResetClockYes); 

  // Redémarrez l'horloge
  server.on("/reboot", RebootClock); 

  // Mesure de la température intérieure
  if (InTempValue != "!" && InHumValue != "-") {
  server.on("/tempvalue", DHTHTML); 
  server.on("/dhtcalibrate", DHTSensorCalibrate);}  

  // Afficher les clés API
  server.on("/cpadjust", GetAdjustments); 

  // Éteindre l'affichage si le temps « Arrêt automatique » est dépassé
  if (FormatedDisplayOffTime() != 0) {
  if (FormatedTime() >= FormatedDisplayOffTime() && String(d) + "." + String(mo) != "31.12") {
  Display = false; 
  ClockStart = false;
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Affichage désactivé
  if (SerialMonitor) {Serial.println("Matrice éteinte à "+ String(h) + ":" + String(m));}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {Serial.println(F("Le Buzzer n'est pas actif"));}} else {
  if (SerialMonitor) {Serial.println(F("Le Buzzer est actif"));}}

  DateString = ExpandedDateFR(); // Calculer la date et la semaine civile

  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(DateString);
  Serial.println(F(""));}

  // Sauvegarde des millièmes après la configuration du module d'exécution - Connexion WLAN OK - Démarrer l'horloge
  bootTime = millis();} else { 
  // ++++ Si la connexion WiFi échoue ++++
  if (ssid != ""){ 
  if (SerialMonitor) {Serial.println(F("La connexion WiFi a échoué !"));} 
  ct = cleanText("        +++  La connexion WiFi a échoué !  +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(100);} // courte attente
  else {
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else { 
  // Avant de démarrer le point d'accès - Vérifiez si le capteur DHT est connecté (3 tentatives)
    // Vérifier la température
   for (int i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(500);
   if (InTempValue != "-") {
   break;}}
   // Check Humity
   for (int i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}} 

  if (InTempValue == "-") {InTempValue = F("!");}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {Serial.println(F(""));} 
  if (DHT11Sensor) { 
  if (SerialMonitor) {Serial.println("Le Capteur DHT11 est Connecté. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {Serial.println("Le Capteur DHT22 est Connecté. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {Serial.println(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("!");    
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("Le capteur DHT est défectueux !"));
  Serial.println(F(""));}} else {
  InHumValue = F("!"); 
  if (SerialMonitor) {
  Serial.println(F(""));  
  Serial.println(F("Aucun Capteur DHT Connecté"));
  Serial.println(F(""));}
  if (SerialMonitor) {Serial.println(F(""));}}}}

  // Aucune donnée enregistrée - Démarrer le point d'accès (portail captif).
  if (SerialMonitor) {  
  Serial.println(F("Exécuter le point d'accès... [Horloge Matricielle LED]")); 
  Serial.println(F(""));}
  String APIPAdresse = "        +++       Le Point d'Accès Démarre      +++       Adresse IP: "+String(APIPA1)+"."+String(APIPA2)+"."+String(APIPA3)+"."+String(APIPA4)+"      +++           ";
  printStringWithShift(APIPAdresse.c_str(),ScrollTextTime);  
  printCharWithShift('.',ScrollTextTime);
    printCharWithShift('.',ScrollTextTime);
      printCharWithShift('.',ScrollTextTime);
      // ########################################################
       CaptivePortal();}}} // Démarrer le portail captif (WiFi-Manager)
      // ######################################################## 

/*************************************(Programme Principal)**************************************/

void loop() {  
if (ssid == "") {getTimeLocal(); server.handleClient();} else {

// contrast(); // Luminosité automatique

 // Allumer et éteindre l'écran à l'aide d'un bouton-poussoir
 if (!await) {MatrixButtonOnOff();} 

  // Vérifiez s'il existe toujours une connexion Internet
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // Établir une connexion sans fil
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Arrêtez le serveur une seule fois
  StartStopServers += 1;  
  WebServerStop(); // Arrêter le serveur Web
  WiFiUDPStop(); // Arrêter le serveur UDP
  }
  if (Display){
  ct = cleanText("        +++       Connexion WiFi : Déconnectée       +++           ");  
  printStringWithShift(ct.c_str(),ScrollTextTime); 
  MyWaitLoop(100); // 0,1 deuxième attente
  if (SerialMonitor) {Serial.println("S'il vous Plaît, Attendez ... "+FormatedDateTimeFR(false));}
  ct = cleanText("        +++       S'il vous Plaît, Attendez ...      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);}
  MyWaitLoop(1000);}} else { // 1 deuxième attente
  ClockWiFiConnectFail = false;  // Connexion WLAN OK
  if (StartStopServers == 1)
  { // Redémarrez le serveur une seule fois
  StartStopServers -= 1;
  if (SerialMonitor) {Serial.println("Connexion WiFi : Restaurée - "+FormatedDateTimeFR(false));}
  WiFiUDPStart(); // démarrer le serveur UDP
  WebServerStart(); // démarrer le serveur Web
  if (Display) {
  ct = cleanText("        +++       Connexion WiFi : Restaurée...       +++           ");  
  printStringWithShift(ct.c_str(),ScrollTextTime);}
  MyWaitLoop(1000);
  sendCmdAll(CMD_SHUTDOWN,0); // Affichage désactivé
  if (DoublePointStatus == 0) {
  dots = 0;} else {dots = 1;}  // Statut Côlon
  for (int i=0; i <= 60; i++){
  MyWaitLoop(10); // courte attente
  getTimeLocal(); // obtenir l'heure locale actuelle     
  HandleOTAandWebServer();  // Requête OTA et serveur Web  
  getAnimClock(false);} // Afficher / Calculer l'heure
  printStringWithShift("           ",1);
  sendCmdAll(CMD_SHUTDOWN,1); // Afficher sur
  ClockScrollIn();}} // Faites défiler l'heure sur l'écran 

  // Seulement s'il y a une connexion WiFi
  if (!ClockWiFiConnectFail) { 
  HandleOTAandWebServer();  // Requête OTA et serveur Web 
  if (StartFail) {if (SerialMonitor) {Serial.println(F("L'Horloge Redémarrera..."));}
  if (!Display) {sendCmdAll(CMD_SHUTDOWN, 1);} // Allumer l'affichage lorsqu'il est éteint
  ct = cleanText("        +++       L'Horloge Redémarrera      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime); 
  digitalWrite(HWLEDPin, LOW); // La LED s'éteint
  ESP.restart();} // Redémarrez l'horloge en cas de problèmes de WiFi

if (!await) {
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
/*if (String(s) == "5" || String(s) == "30") {
await = true;
getNews();
// getWeatherData();
MyWaitLoop(1000); // courte attente 
await = false;} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
/*if (String(s) == "3") { // FONT Test
await = true; 
String TestString = cleanText("+++   ö     +++");
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(8500); // courte attente
await = false;
} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*if (String(s) == "5" || String(s) == "15"  || String(s) == "30" || String(s) == "45") { 
await = true;   // FONT Test 2
FT++;
char  test = '~' + FT; // Signe
String TestString = "     "+String(test)+"     ";
if (FT > 7) {FT = 0;} 
printStringWithShift(TestString.c_str(),ScrollTextTime); 
MyWaitLoop(8500); // courte attente
await = false;
} // */
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Démarrer l'Horloge
if (ClockStart) {
ClockStart = false; // Configuration effectuée 
await = true; 
// *********************  Un bonjour au départ au Propriétaire Principal **********************************************************
if (WatchOwner1FirstName != "") {
String SHT= cleanText("       +++   Bonjour  "+WatchOwner1FirstName+"    ---    L'Horloge est Démarrée...       +++                    "); 
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
String SHT = cleanText("       +++     L'Horloge est Démarrée...     +++                    ");   
printStringWithShift(SHT.c_str(), ScrollTextTime);} 
// ***************************************************************************************************************************
Greetings_and_Holiday(true); // Tout d'abord, salutations et jours fériés si disponibles 
sendCmdAll(CMD_SHUTDOWN,0); // Affichage éteint
if (DoublePointStatus == 0)
{dots = 0;} else {dots = 1;}  // Statut Côlon
// Première mesure régulière lorsque le capteur est prêt 
InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
if (InHumidityStatus != "FAIL") {
// Montrer si la LED est OK
if (InTempOffset != "999") {digitalWrite(HWLEDPin, HIGH);} // Allumage de la LED 
MyWaitLoop(100);
if (InHumidityStatus != "MIN" && InHumidityStatus != "MAX") { 
digitalWrite(HWLEDPin, LOW);} else {
if (IsPushover) {PushOverAlert(InHumidityStatus);}}} else {digitalWrite(HWLEDPin, LOW);}
if (IsBuzzer && StartSound == "On") {StartBuzzerBeep();} // Son au démarrage     
ClockScrollIn(); // Faites défiler l'heure sur l'écran 
await = false;} // Début du chronomètre - Fin

  // Action de Minuit
  if (MidnightGhost || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "5" && Display) {PDP = true;} // DoublePoint activé    
  if (String(h) == "0" && String(m) == "0" && String(s) == "6") {
  await = true;
  MyWaitLoop(1000); // courte attente
  if (MidnightGhost && String(d) + "." + String(mo) != "1.1") {
  if (SerialMonitor) {Serial.println(F("Afficher le Fantôme"));}  
  ShowGhost();} // Fantôme à minuit
  getTimeLocal(); 
  if (EnhancedFunctions) {Greetings_and_Holiday(false); // Déterminer les salutations et les jours fériés pour le nouveau jour  
  DateString = ExpandedDateFR();}
  PDP = false; // Côlon éteint définitivement en mode clignotant
  await = false;}}

  // ****** Éteindre Matrix Automatiquement *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Laissez l'affichage allumé le soir du Nouvel An
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":7") {PDP = true;} // DoublePoint activé 
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":8") {     
  await = true;
  Display = false;
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800); MyWaitLoop(1000);} // Matrice éteinte Bip  
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  digitalWrite(HWLEDPin, LOW); // LED éteinte puis allumée 
  sendCmdAll(CMD_SHUTDOWN,0); // Arrêt de la matrice
  if (SerialMonitor) {Serial.println("Matrice éteinte à "+ String(h) + ":" + String(m));} 
  PDP = false; // Côlon éteint définitivement en mode clignotant
  await = false;}}

  // ***** Allumer Matrix automatiquement ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":8") {   
  await = true; 
  for (int i=0; i <= 50; i++){ 
  MyWaitLoop(10);     
  HandleOTAandWebServer();  // Requête OTA et serveur Web  
  getAnimClock(false);} // Afficher / Calculer l'heure   
  printStringWithShift("           ",1);  
  sendCmdAll(CMD_SHUTDOWN,1); // La matrice s'allume
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800);} // La matrice active le bip
  MyWaitLoop(1500); // courte attente
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  ClockScrollIn(); // Faites défiler l'heure sur l'écran
  if (SerialMonitor) {Serial.println("Matrice allumée à "+ String(h) + ":" + String(m));} 
  Display = true;
  CheckInHumidity();  // Mesure de l'humidité intérieure
  await = false;}}

  // ******************** Commencer le Programme actuel *******************************
  if (Display) {
  // Son à pleine heure lorsqu'il est activé 
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // Bip d'une heure complète
  await = false;}} 

  if (GoMatrixAction()) { // Plus d'actions 10 minutes avant que la Matrix ne s'éteigne automatiquement !
                          // (pour un arrêt sécurisé de la matrice - Pas de chevauchement possible avec une action)
  // Évaluation de l'humidité sur l'heure si le capteur est installé
  if  (String(m)+":"+String(s) == "0:9") { 
  InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
  if (InHumidityStatus != "FAIL") {   
  PDP = true; // Double point sur     
  await = true; 
  // Humidité intérieure trop faible
  if (InHumidityStatus == "MIN") { 
  for (int i=0; i <= 1; i++) { // Repeat message 2x
  ct = cleanText("        +++       Attention : L'Humidité à l'emplacement de l'Horloge est trop faible... !      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(2500);}
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Envoyer un message PushOver
  ClockScrollIn();}
  if (InHumidityStatus == "MAX") {   
  MyWaitLoop(1500);  
  // Humidité intérieure trop élevée
  for (int i=0; i <= 1; i++){ // Répéter le message 2x
  ct = cleanText("        +++       Attention : L'Humidité à l'emplacement de l'Horloge est trop élevée... !      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(2500);}
  if (IsPushover) {PushOverAlert(InHumidityStatus);}  // Envoyer un message PushOver
  ClockScrollIn();}
  if (InHumidityStatus != "NORMAL") {digitalWrite(HWLEDPin, LOW);} 
  PDP = false; // Côlon éteint définitivement en mode clignotant
  await = false;}}

  // // uniquement température intérieure et extérieure --- 3x par heure (par défaut)
  if ((InTempValue != "-" && InTempValue != "!")  || WeatherFunctions) {
  for (int i=0; i <= OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Double point sur
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
     // *** Température intérieure ***
    if (InTempValue != "-") { // DHT active
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();}  // Mesure de température
    MyWaitLoop(100); // courte attente
    String InTempString = "";
    if (InTempValue != "-") {
    IntInTempValue = InTempValue.toInt();  
    if (InTempCounter > 0) {InTempCounter = 0;} // Réinitialisation des variables
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Remplacez Minus par Minus "court"
    if (IntInTempValue > -10) {
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT2) + " " + InTempValue + deg + "C";}} else {
    if (IntInTempValue > 9) {  
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           "+cleanText("Température Intérieure")+":       "+String(IT) + "  " + InTempValue + deg + "C ";}} 
    SIC = true; // Horloge de défilement
    if (InTempOffset != "999") {printStringWithShift(InTempString.c_str(), ScrollTextTime); // Display Indoor Temperature
    MyWaitLoop(10000); // courte attente (10 sec. - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez la température intérieure s'afficher à partir de l'écran
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {Serial.println(F("DHT Sensor disabled or broken"));}
    digitalWrite(HWLEDPin, LOW); // La LED s'éteint
    InTempValue = "-";}} else {digitalWrite(HWLEDPin, LOW);} // La LED s'éteint

    // *** Température Extérieure ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData(); // Obtenez les données météorologiques actuelles (télécharger)
    SIC = true; // Horloge de défilement
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Afficher la température extérieure
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // courte attente (10 sec. - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez la température extérieure s'afficher à partir de l'écran
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement
    PDP = false; // Côlon éteint définitivement en mode clignotant
    await = false;}}}

  // Uniquement l'humidité de l'air intérieur et extérieur --- 3x par heure (par défaut)
  if ((InHumValue != "-" && InHumValue != "!") || WeatherFunctions) {
  for (int i=0; i <= OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":10") {PDP = true;} // Double point sur
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":11") { 
    await = true;
    // *** Humidité de l'air intérieur ***
    InHumidityStatus = CheckInHumidity(); // Innenluftfeuchte Messen
    if (InHumidityStatus != "FAIL") {   
    String InHumString = "";
    if (IntInHumValue > 99) {
    InHumString = "           "+cleanText("Humidité de l'Air Intérieur")+":  "+ String(IL2) + " " + InHumValue + " %";} else {
    if (IntInHumValue < 10) {
    InHumString = "           "+cleanText("Humidité de l'Air Intérieur")+":  "+ String(IL) + "  " + InHumValue + " % ";} else {    
    InHumString = "           "+cleanText("Humidité de l'Air Intérieur")+":  "+ String(IL) + " " + InHumValue + " % ";}}
    SIC = true; // Horloge de défilement
    if (InTempOffset != "999") {printStringWithShift(InHumString.c_str(), ScrollTextTime); // Mesure de l'humidité intérieure
    MyWaitLoop(10000); // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez l'humidité intérieure et extérieure s'écouler à partir de l'écran.
    } else {if (SerialMonitor) {Serial.println(F("Capteur DHT désactivé ou cassé"));}}

    // *** Humidité de l'air extérieur ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    getWeatherData();  // Obtenez les données météorologiques actuelles (télécharger)
    SIC = true; // Horloge de défilement
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Afficher l'humidité extérieure
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez l'humidité de l'air extérieur s'écouler à partir de l'écran
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement
    PDP = false; // Côlon éteint définitivement en mode clignotant
    await = false;}}}

  // Fonctions supplémentaires (date, vacances, anniversaires) et/ou affichage des actualités --- toutes les 10 minutes
  if (NewsDataFunction || EnhancedFunctions) {
  for (int i=0; i <= DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Double point sur     
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
    await = true;
    CheckInHumidity(); // Mesure de l'humidité intérieure
    if (NewsDataFunction && AccessNewsData) {getNews();} // obtenir des nouvelles actuelles

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (EnhancedFunctions) {
    DateString = ExpandedDateFR(); // Calculer la date avec la semaine civile
    SIC = true; // ScrollIn Clock
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Date de sortie avec semaine civile
    if (GNCounter == 0) {Greetings_and_Holiday(false);} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Afficher alternativement les salutations et les jours fériés ainsi que les actualités
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Messages de données d'actualité
    // Afficher le message sur l'affichage matriciel si aucun message d'accueil
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    CheckInHumidity(); // Mesure de l'humidité intérieure
    if (APINews != "") {
    SIC = true; // Horloge de défilement  
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Afficher les actualités
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement 
    waitloop = 0; // Réinitialisation des variables
    PDP = false; // Côlon éteint définitivement en mode clignotant
    await = false;}}}

  // Météo détaillée --- 2x par heure (par défaut)
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (int i=0; i <= FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Double point sur  
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  CheckInHumidity(); // Mesure de l'humidité intérieure
  getWeatherData(); // Obtenez les données météorologiques actuelles (télécharger)
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  ClockScrollIn(); // Horloge de défilement 
  PDP = false; // Côlon éteint définitivement en mode clignotant
  await = false;}}}
}  // OnMatrixAction() = True

// Laissez le Côlon clignoter
if (DoublePointStatus == 2) { 
 if (millis() - dotTime > 500) { 
 dotTime = millis();
 if (!PDP) {dots = !dots;} else {dots = 1;}
 // courte attente
 delay(1);}}
// Double point désactivé 
if (DoublePointStatus == 0) {dots = 0;} 
// Côlon allumé en permanence
if (DoublePointStatus == 1) {dots = 1;} 
} // Display = true 
getAnimClock(true); // Afficher / Calculer l'heure
ResetAPCounter(); // Bouton du point d'accès réinitialisé toutes les 2 minutes
}}}
} // Boucle - Fin