/*******************************************************************************************
 ** Author:  Mario Ammerschuber                                                           **
 ** Date:    Décembre 2025                                                                **
 ** Version: 11.0.9 (Français)                                                            ** 
 ** Board:   LOLIN (WEMOS) D1 R2 and mini                                                 **  
 ** Sensor:  DHT11 or DHT22                                                               **
 ** Buzzer:  Passiv Piezo Buzzer KY-006                                                   **
 ** LED:     3mm Yellow                                                                   **  
 ** Website: https://github.com/ammerscm68/LED-Matrix-Rolling-Clock-with-ESP8266          **  
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/
const char* LMCVersion = "11.0.9 fr";
/************************( Importing the libraries )************************/

#include <ESP8266WiFi.h>                    // Installer via la bibliothèque
#include <WiFiClientSecure.h>               // Installer via la bibliothèque
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
const char* BoardName = "Wemos/Lolin D1 mini"; // Nom du tableau ESP8266

/********************( Arduino OTA/DNS Mot de Passe)********************/
const char* OTAPassWD = "74656"; // défaut = 74656

// ############## Propriétaire de l'Horloge ######## Peut être modifié dans le portail captif à partir de la version 10.0.1 #########
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
byte APIPA1 = 192; byte APIPA2 = 168; byte APIPA3 = 4; byte APIPA4 = 1; // Défaut = 192.168.4.1
// #############################################################################################################################################

// #############################################################################################################################################
byte ScrollClockTime  = 80; // Délai de défilement lorsque le temps est animé   80 = par Défaut   (Maximum = 255)
// #############################################################################################################################################

// #################### Capteur de température et d'humidité (DHT 11 ou DHT 22) ################################################################
String DHTSensor = "NoDHT"; // DHT11 / DHT22 / NoDHT   Capteur    (Afficher la Température en Celsius)
// #############################################################################################################################################

// ########################## Montrer un fantôme à minuit ########### Peut être modifié dans le Portail Captif à partir de la Version 10.1.0 ###
String MidnightGhost = "YES"; // YES = afficher un fantôme à minuit / NO = ne pas afficher de Fantôme à minuit
// #############################################################################################################################################

// #############################################################################################################################################
#define DEBUG 0   // Debugging  -  1=Sur, 0=Désactivé
// #############################################################################################################################################

// ############################################################################################################################################# 
byte MinComfortHum = 32; // Humidité minimale de confort (par défaut = 32) - Valeur en %
byte MaxComfortHum = 68; // Confort maximum Humidité (par défaut = 68) - Valeur en %
// #############################################################################################################################################

// #############################################################################################################################################
int16_t MinRAMMem   = 15784; // Minimale RAM in Byte  par défaut = 15784
// #############################################################################################################################################

// ########################## GMCMapMidnightAccessDelay >> défaut = 30ème Minute | Minimum = 0 | Maximum = 59 ##################################
const char* GMCMMAD = "30";  // Exemple: 30 --> Donc pas de Téléchargement des données actuelles de GMCMap avant 00h30
// #############################################################################################################################################

// ############ Préréglage du fuseau horaire dans lequel se trouve l'Horloge (modifications possibles dans le portail Captiv) ##################
String MCTZ = "Berlin, Frankfurt, Vienna, Paris, Madrid"; 
// #############################################################################################################################################

// #############################################################################################################################################
String NTPSN = "fr.pool.ntp.org";
const char* ntpServerName = "fr.pool.ntp.org";  // Rechercher un serveur local sous http://www.pool.ntp.org/zone
// #############################################################################################################################################

// ########################### !!!!!!!!!!! #####################################################################################################
bool FormatFileSystem = false; // true = Supprime tous les fichiers stockés dans le système de fichiers !!!  /  false = défaut
// #############################################################################################################################################

// #############################################################################################################################################
byte MaxWiFiConnectLoop = 20;  // Nombre maximal de boucles lors de la connexion au WiFi  -  Minimum = 15   Maximum = 255
// #############################################################################################################################################

// #############################################################################################################################################
const byte PushButtonPin = D3; // Matériel-PushButton-Pin (D3 = GPIO 0)
const byte buzzerPin = D2; // initialisation du buzzer passif (D2 = GPIO 4) || Pour microcontrôleur ESP8266 (D4 ne fonctionne pas avec le microcontrôleur ESP8266)
const byte HWLEDPin = D5; // Broche LED pour avertir d'une Humidité trop élevée ou trop faible ou d'une Radioactivité trop élevée (D5 = GPIO 14) -1 = pas de LED
// #############################################################################################################################################

DHTesp dht; // Capteur DHT pour la mesure de la température à l'intérieur (par défaut = D1)

String ssid = "";       // SSID du réseau local 
String TempSSID = "";   // SSID temporaire du réseau local 
String password = "";   // Mot de passe sur le réseau 
String weatherKey;      // Clé API de OpenWeatherMap - https://openweathermap.org
String cityID;          // ID de ville OpenWeatherMap 

String rtime = "S'il vous plaît, attendez"; // Temps de fonctionnement de l'Horloge
String ClockStatus = ""; // État de l'Horloge

bool AccessOpenWeatherMap = false;
bool AccessNewsData = false;
bool AccessGMCMap = false;

String WeatherLocation = "";
String StatusOpenWeatherMap;
bool FirstWeatherData = true;

String SecureAppToken = ""; // Pushover Jeton d'application sécurisé
String UserToken = ""; // Pushover Jeton utilisateur
String PushoverDevice1 = ""; // Pushover Appareil (Exemple : Smartphone)
String PushoverDevice2 = ""; // Pushover Appareil
String PushoverDevice3 = ""; // Pushover Appareil
String PushoverMessageSound = "none"; // Pushover Son des messages
bool IsPushover = false; // Pushover désactivé par défaut
bool FailPushOver = false; // En cas d'erreur PushOver
bool PushOverTest = false; // Récepteur de dispositif de Test Pushover
int16_t  WSR = 800; // Taux de Rafraîchissement du Serveur Web | par défaut = 800

// NTP-Init (Première Partie)
const byte timeZone = 0; // 0 lorsque vous travaillez avec <Timezone.h> !
WiFiUDP Udp;
unsigned int localPort = 1701; // port local pour écouter les paquets UDP

// Salutations aujourd'hui
bool GreetingsToday = false;

// Pour le redémarrage et la réinitialisation de l'horloge
bool ResetNow = false; // Pour la réinitialisation de l'horloge
bool RebootNow = false; // Pour le redémarrage de l'horloge

uint16_t bootTime; // Heure de démarrage Horloge matricielle LED

String newsKey; // Clé NewsData
String APINews = "";
String WPAPINews = "";
String StatusNewsData;
bool NewsOutOfCredits = false;

String DisplayOnTime;
String DisplayOffTime;
String FDOnTime;
String FDOffTime;
String Brightness;
bool MatrixAction = true;
bool MEP = false;
bool OWMAdjustChange = false;
bool NAAdjustChange = false;
bool GMCMapAdjustChange = false;
bool ClockWiFiConnectFail = false; // fausse norme
bool SIC = false; // Faire défiler l'Horloge
bool SerialMonitor = false; // Debuging - Par défaut = désactivé

float temp;
String SText = "";
String weatherString = "";
String WPweatherString = "";
String ATemp = "";
String AHum = "";
String InHumMessage = "";
String InHumidityStatus;
String DateString = "";
String cip = "";
String cip2 = "";

// GMCMap Données 
String GMCMapValue = "";
String WPGMCMapValue = "";
String MXGMCMapValue = "";
String GMCMapHistoryID = "";
String GMCMapGMTOffset = "";
String StatusGMCMap = "";
String GMCMapToHigh = "";

#define NUM_MAX 4

// pour module ESP-01 - PINs MAX7219 Écran matriciel LED
//#define DIN_PIN 2 // D4 - ESP-01
//#define CS_PIN  3 // D9/RX - ESP-01
//#define CLK_PIN 0 // D3 - ESP-01

// pour NodeMCU 1.0 et WEMOS D1 mini - PIN MAX7219 Affichage à Matrice LED
#define DIN_PIN 15  // D8 - NodeMCU ou WEMOS D1 mini
#define CS_PIN  13  // D7 - NodeMCU ou WEMOS D1 mini
#define CLK_PIN 12  // D6 - NodeMCU ou WEMOS D1 mini

#include "max7219.h" // Écran matriciel LED MAX7219
#include "fonts.h"

byte  WaitForNTPIP = 0;
bool await = false;
bool swait = false;
bool littleFSInit = true;

// Buzzer
bool IsBuzzer = false;
String StartSound;
String HourSound;

String GT =  ""; // Salutations
String GT1 = ""; // Salutations
String HT =  ""; // Vacances
String HT1 = ""; // Vacances
byte GNCounter = 1; // Comptoir pour les salutations/vacances et nouvelles

uint32_t ESPRAM; // ESP8266-Heap
bool OneTimeCall = false;

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
byte dots = 0;
unsigned long dotTime = 0;
unsigned long hwsTime = 0;
unsigned long hwsTime1 = 0;
int16_t dx = 0; // doit être de type "int16_t"
int16_t dy = 0; // doit être de type "int16_t"
byte del = 0; // doit être de type "int16_t"
int16_t h, m, s, w, mo, ye, d; // doit être de type "int16_t"
String OWMLastConnectionDateTime = ""; // dernière heure de connexion OpenWeatherMap
String NewsDataLastConnectionDateTime = ""; // Dernière heure de connexion NewsData
String GMCMapLastConnectionDateTime = ""; // Dernière heure de connexion GMCMap
long localMillisAtUpdate = 0;
byte ScrollTextTime = 0; 
int16_t waitloop = 0;
byte WiFiConnectLoop = 0;
bool Display = true; // Mode d'affichage - ON
byte DoublePointStatus = 1; // Statut Doublepoint - Par défaut = ON permanent
bool PDP = false;
bool EnhancedFunctions = true; // Fonctions supplémentaires ON / OFF - Standard = ON (Date, Jours fériés, Anniversaires, etc.)
bool WeatherFunctions = true; // Fonction de données météorologiques activée/désactivée - Par défaut = activée (clé API OpenWeatherMap et ID de ville disponibles)
bool NewsDataFunction = true; // Fonction d'actualités activée/désactivée - Par défaut = activée (clé API NewsData disponible)
bool GMCMapFunction = false; // Radioactivité actuelle à l'emplacement sélectionné
byte StartStopServers = 0;
byte APStart = 0;
bool ClockStart = true;
int16_t IntInTempValue = -1;
int16_t IntInHumValue = -1;
int16_t humidity = -1; // Humidité d'OpenWeatherMAP
byte InTempCounter = 0; // Compteur de température intérieure
byte InHumCounter = 0; // Humidité
String InTempValue; // Température intérieure
String InHumValue; // Humidité
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
String InTempOffset; // Veuillez vous référer au portail captif
String InHumOffset;  // Veuillez vous référer au portail captif
int16_t InTempOffsetInt = -1; // Décalage de la température intérieure (exemple : -1 = décalage --> -1 degré ou +1 = décalage --> +1 degré - dans un portail captif)
int16_t InHumOffsetInt = -1;  // Décalage de l'humidité intérieure (exemple : -1 = décalage --> -1 % ou +1 = décalage --> +1 % - dans le portail captif)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String LastInTempMessureDateTime = "n/a";

/***************************************** ( Installation ) ****************************************/
void setup() {
// Matrice LED d'initialisation
initMAX7219();

StringMemoryLimitation(); // Limiter les chaînes

CheckGlobalVariableRange(); // Vérifier les Variables

#if DEBUG // Debugging
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(fmt, ...) ((void)0)
#endif
if (DEBUG == 1) {SerialMonitor = true;} else {SerialMonitor = false;}

if (SerialMonitor) {
InitSerial(115200, 120); DEBUG_PRINTLN("");} // Attendre la connexion COM / Attendre par défaut = 120

if (SerialMonitor) {
   DEBUG_PRINTLN(F("*** Horloge-Matricielle-LED ***"));
   DEBUG_PRINTLN("  (Version: "+String(LMCVersion)+")");
   DEBUG_PRINTLN(F(""));}

LoadMatrixAdjustments(); // Charger les paramètres de l'Horloge matricielle LED....

// Init NTP (Deuxième Partie)
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// #####################################################################################
// pour la luminosité automatique (photorésistance)
// pinMode(A0, INPUT); 
  // Initialize Button
   pinMode(PushButtonPin, INPUT_PULLUP); // (PushButtonPin par défaut = D3)
  // Initialiser l'horloge LED
   if (HWLEDPin != -1) {pinMode(HWLEDPin, OUTPUT); digitalWrite(HWLEDPin, LOW);} // (HWLEDPin par défaut = D5)
   // Connectez le capteur DHT au GPIO 5 (D1) - Capteur de température et d'humidité intérieure
   if (DHTSensor != "NoDHT") {
   if (DHTSensor == "DHT11") {// Capteur DHT11
   dht.setup(5, DHTesp::DHT11);} else { // Capteur DHT22
   dht.setup(5, DHTesp::DHT22);}}
// #####################################################################################

   // Init passif Piezo Buzzer
    if (IsBuzzer) {
    pinMode(buzzerPin, OUTPUT); // (par défaut : buzzerPin = D2)
    noTone(buzzerPin);} // no Sound

  // Formater le petit système de fichiers
  if (FormatFileSystem) {
  if (littleFSInit) { 
  LittleFS.format(); 
  printStringWithShift(cleanText("     +++       Le Système de fichiers a été Formaté !  \
  --- Redéfinissez la Variable -FormatFileSystem- Sur -false- et \
  Téléchargez à Nouveau le Croquis.....").c_str(),ScrollTextTime);
  printStringWithShift(cleanText("                              +++\
         L'Horloge va Redémarrer      +++           ").c_str(),ScrollTextTime);
  MyWaitLoop(5000); // courte attente
  ESP.restart();}} // Redémarrer l'horloge

  if (SerialMonitor) {
  if (ssid == "") {
    DEBUG_PRINTLN(F(""));
    DEBUG_PRINTLN(F("***** Horloge du Premier Démarrage *****")); // Horloge du premier démarrage
    DEBUG_PRINTLN(F(""));}}

  WiFi.hostname("LEDMatriceHorloge"); // Clock Hostname

  if (ssid != ""){
  if (SerialMonitor) {  
  DEBUG_PRINT(F("Connectez-vous avec le SSID WiFi: "));
  DEBUG_PRINTLN(ssid);}
  printStringWithShift("WiFi  ", ScrollTextTime);
  wificonnect(true); // Établir une connexion sans fil
  } else {ClockWiFiConnectFail = true;} // Contournez l'enregistrement WLAN lorsque vous démarrez l'horloge pour la première fois ! - Démarrez le point d'accès immédiatement
  
  if (!ClockWiFiConnectFail) { 
  // Afficher l'adresse IP attribuée sur Matrix
  cip = (String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3];
  cip2 = "             Adresse IP:  "+cip+"                "; // Determined IP address (DHCP)

  printStringWithShift(cip2.c_str(), ScrollTextTime); // Afficher l'adresse IP MyWaitLoop(10); // courte attente

  if (SerialMonitor) {DEBUG_PRINTLN(F("Démarrer le serveur UDP  ..."));}
  WiFiUDPStart();  // Démarrer le serveur UPD

  // #######################################################################################
  // Arduino OTA/DNS
  if (SerialMonitor) {DEBUG_PRINTLN(F("Démarrer le serveur Arduino OTA/DNS ... - URL: http://ledmatricehorloge.local"));}
  ArduinoOTA.setHostname("ledmatricehorloge");
  ArduinoOTA.setPassword(OTAPassWD); // Mot de passe OTA
  ArduinoOTA.onEnd([]() {if (SerialMonitor) {DEBUG_PRINTLN("\nEnd");}});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  if (SerialMonitor) {DEBUG_PRINTF("Progress: %u%%\r", (progress / (total / 100)));}});
  ArduinoOTA.onError([](ota_error_t error) {(void)error; 
  digitalWrite(HWLEDPin, LOW); ESP.restart();}); // Redémarrer en cas d'erreur OTA
  // Maintenant, démarrez Arduino maintenant OTA/DNS
  ArduinoOTA.begin();
  // Démarrer le serveur TCP (HTTP)
  WebServerStart();
  // #####################################################################################
  if (SerialMonitor) {
  if (!WeatherFunctions) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("OpenWeatherMap a été désactivé - Aucune donnée météo"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!NewsDataFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("NewsData a été désactivé - Aucune actualité actuelle"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}

  if (!GMCMapFunction) {
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F("GMCMap a été désactivé - Aucune valeur de Radioactivité actuelle"));
  DEBUG_PRINTLN(F("")); 
   } else {DEBUG_PRINTLN(F(""));DEBUG_PRINTLN(F(""));}}
   
    // Obtenez toutes les informations de votre LittleFS
    if (littleFSInit) {  
    if (SerialMonitor) {  
    DEBUG_PRINTLN(F("Petit Init du système de fichiers - terminé."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    DEBUG_PRINTLN(F("Informations sur le système de fichiers."));
    DEBUG_PRINT(F("Espace total:      "));
    DEBUG_PRINT(fs_info.totalBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Espace total utilisé: "));
    DEBUG_PRINT(fs_info.usedBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Taille du bloc:       "));
    DEBUG_PRINT(fs_info.blockSize);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Taille des pages:        "));
    DEBUG_PRINT(fs_info.totalBytes);
    DEBUG_PRINTLN(F("byte"));
    DEBUG_PRINT(F("Nombre maximum de fichiers ouverts:   "));
    DEBUG_PRINTLN(fs_info.maxOpenFiles);
    DEBUG_PRINT(F("Longueur maximale du chemin:  "));
    DEBUG_PRINTLN(fs_info.maxPathLength);
    DEBUG_PRINTLN(F(""));}
    // Ouvrir le dossier répertoire
    Dir dir = LittleFS.openDir("/");
    // Cycle tout le contenu
    while (dir.next()) {
        // obtenir le nom du fichier
        if (SerialMonitor) {DEBUG_PRINT(dir.fileName());
        DEBUG_PRINT(F(" - "));}
        // Si l'élément a une taille d'affichage, sinon il écrit 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            if (SerialMonitor) {DEBUG_PRINTLN(f.size());}
            f.close();
        }else{if (SerialMonitor) {DEBUG_PRINTLN(F("0"));}}}
   if (SerialMonitor) {
   DEBUG_PRINTLN(F("_______________________________"));
   DEBUG_PRINTLN(F(""));}} else {
   if (SerialMonitor) {  
   DEBUG_PRINTLN(F("Init du petit système de fichiers - Erreur."));}}
  if (SerialMonitor) {   
  DEBUG_PRINTLN("StartTime: "+FormatedDateTimeFR(false));
  DEBUG_PRINTLN(F(""));
  DEBUG_PRINTLN(F(""));}

  if (DHTSensor != "NoDHT") {
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Avant de démarrer le point d'accès - Vérifiez si le capteur DHT est connecté (3 tentatives)
    // Vérifier la température
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "!") {
   break;}} 
   // Vérifier l'humidité
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHTSensor == "DHT11") { 
  if (SerialMonitor) {DEBUG_PRINTLN("Le Capteur DHT11 est Connecté. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("Le Capteur DHT22 est Connecté. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("-");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Le capteur DHT est défectueux !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("-"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Aucun Capteur DHT connecté"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}} else {
  InTempValue = F("!"); InHumValue = F("-");
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Aucun Capteur DHT connecté"));
  DEBUG_PRINTLN(F(""));}}

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

  // Activer/Désactiver l'affichage actuel de la Radioactivité
  if (AccessGMCMap) {
  server.on("/gmcmapon", GMCMapFunctionOn);
  server.on("/gmcmapoff", GMCMapFunctionOff);
  server.on("/agmcmapd", ActualGMCMapData);}

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

  // Redémarrez l'Horloge
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
  digitalWrite(HWLEDPin, LOW); // LED éteinte
  if (SerialMonitor) {DEBUG_PRINTLN("Matrice éteinte à "+ String(h) + ":" + String(m));}}}

  if (!IsBuzzer) {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer n'est pas actif"));}} else {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Le Buzzer est actif"));}}

  DateString = ExpandedDateFR(); // Calculer la date et la semaine civile

  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(DateString);
  DEBUG_PRINTLN(F(""));}

  // Sauvegarde des millièmes après la configuration du module d'exécution - Connexion WLAN OK - Démarrer l'horloge
  bootTime = millis();} else { 
  // ++++ Si la connexion WiFi échoue ++++
  if (ssid != ""){ 
  if (SerialMonitor) {DEBUG_PRINTLN(F("La Connexion WiFi a échoué !"));} 
  printStringWithShift(cleanText("        +++  La Connexion WiFi a échoué !  +++           ").c_str(),ScrollTextTime);
  printStringWithShift(cleanText("        +++  S'il vous plaît, attendez  +++           ").c_str(),ScrollTextTime);
  WaitForClockStartWIFI(MaxWiFiConnectLoop); // Essayez d'établir une Connexion WiFi
  if (ClockWiFiConnectFail) {
  printStringWithShift(cleanText("        +++       La Connexion WiFi n'a pas pu être établie      +++           ").c_str(),ScrollTextTime);
  printStringWithShift(cleanText("        +++       Prochaine tentative - S'il vous plaît, attendez...      +++           ").c_str(),ScrollTextTime);} else {
  printStringWithShift(cleanText("        +++       La Connexion WiFi a été Rétablie      +++           ").c_str(),ScrollTextTime);
  printStringWithShift(cleanText("        +++       L'Horloge va Redémarrer ...      +++           ").c_str(),ScrollTextTime);
  SaveClockStatus("La Connexion WiFi a échoué !");} // Save Status of the Clock
  ESP.restart();} // Redémarrez l'Horloge
  else {
  if (DHTSensor != "NoDHT") {
  if (InTempOffset == "999") {InTempValue = "0"; InHumValue = "0";} else {
  // Avant de démarrer le point d'accès - Vérifiez si le capteur DHT est connecté (3 tentatives)
    // Vérifier la température
   for (byte i=0; i <= 3; i++){
   InTempValue = InTempMessure();
   MyWaitLoop(100);
   if (InTempValue != "!") {
   break;}} 
   // Vérifier l'humidité
   for (byte i=0; i <= 3; i++){
   InHumValue = InHumMessure();
   MyWaitLoop(100);
   if (InHumValue != "-") {
   break;}}

  if (InTempValue != "!" && InHumValue != "-") {
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));} 
  if (DHTSensor == "DHT11") { 
  if (SerialMonitor) {DEBUG_PRINTLN("Le Capteur DHT11 est Connecté. ("+InTempValue+"°C)");}} else {
  if (SerialMonitor) {DEBUG_PRINTLN("Le Capteur DHT22 est Connecté. ("+InTempValue+"°C)");}}  
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}} else {
  if ((InTempValue != "!" && InHumValue == "-") || (InTempValue == "!" && InHumValue != "-")) {
  InHumValue = F("-");    
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Le capteur DHT est défectueux !"));
  DEBUG_PRINTLN(F(""));}} else {
  InHumValue = F("-"); 
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Aucun Capteur DHT connecté"));
  DEBUG_PRINTLN(F(""));}
  if (SerialMonitor) {DEBUG_PRINTLN(F(""));}}}}} else {
  InTempValue = F("!"); InHumValue = F("-");
  if (SerialMonitor) {
  DEBUG_PRINTLN(F(""));  
  DEBUG_PRINTLN(F("Aucun Capteur DHT connecté"));
  DEBUG_PRINTLN(F(""));}}

  // Aucune donnée enregistrée - Démarrer le point d'accès (portail captif).
  if (SerialMonitor) {  
  DEBUG_PRINTLN(F("Exécuter le point d'accès... [Horloge Matricielle LED]")); 
  DEBUG_PRINTLN(F(""));}
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

  //------------------------------------------------------------------------------------ 
  rtime = runtime(); // Calculer le temps de fonctionnement de l'Horloge (Clock Runtime)
  //------------------------------------------------------------------------------------

 // Allumer et éteindre l'écran à l'aide d'un bouton-poussoir
 if (!await) {MatrixButtonOnOff(false);} 

  // Vérifiez si la Connexion Internet est toujours disponible
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
  wificonnect(false); // Établir une Connexion sans fil
  if (ClockWiFiConnectFail) {
  if (StartStopServers == 0) { // Arrêtez le Serveur une seule fois
  SaveClockStatus("Connexion WiFi déconnectée -/- "+FormatedDateTimeFR(false)); // Enregistrer l'état de l'Horloge 
  StartStopServers += 1;  
  WebServerStop(); // Arrêter le Serveur Web
  WiFiUDPStop();} // Arrêter le Serveur UDP
  if (SerialMonitor) {DEBUG_PRINTLN("Connexion WiFi : déconnectée - "+FormatedDateTimeFR(false));
  DEBUG_PRINTLN(F("S'il vous Plaît, attendez ..."));} 
  if (Display){
  printStringWithShift(cleanText("        +++       Connexion WiFi : déconnectée       +++           ").c_str(),ScrollTextTime); 
  MyWaitLoop(100); // 0,1 sek. attendez 
  printStringWithShift(cleanText("        +++       S'il vous Plaît, attendez ...      +++           ").c_str(),ScrollTextTime);}
  MyWaitLoop(500);}} else { // 0,5 sek. wait
  if (StartStopServers > 0) {
  if (SerialMonitor) {DEBUG_PRINTLN("Connexion WiFi Restaurée - "+FormatedDateTimeFR(false));
  DEBUG_PRINTLN(F("L'Horloge Redémarrera..."));}
  if (Display) {sendCmdAll(CMD_SHUTDOWN, 0); // Affichage court éteint  
  ClearMatrix(); 
  printStringWithShift(cleanText("           +++       Connexion WiFi : Restaurée...       +++           ").c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift(cleanText("           +++       L'Horloge Redémarrera       +++           ").c_str(),ScrollTextTime);} 
  StatusWarningLED("LOW"); // LED OFF
  ESP.restart();}} // Restart the Clock if there are WiFi problems

   // Uniquement lorsqu'il y a une Connexion WiFi
  if (!ClockWiFiConnectFail) { 
  HandleOTAandWebServer();  // Requête OTA et Serveur Web 

if (!await) {
// Démarrer l'Horloge
if (ClockStart) {
ClockStart = false; // Configuration effectuée
await = true; 
MyWaitLoop(50); // courte attente
// **** Un Bonjour au démarrage au propriétaire principal ou indication de la force du Signal WiFi *************************************************************
String SHT = WiFiSignalStrength(true); // Lecture de la force du Signal WiFi
if (SHT == "bad" || SHT == "very bad") {
if (SHT == "bad") { 
SHT = cleanText("       +++     Attention : La puissance du Signal WiFi est mauvaise ! --- L'Horloge démarrera toujours normalement...       +++                    ");}   
if (SHT == "very bad") {  
SHT = cleanText("       +++     Attention : La puissance du Signal WiFi est très mauvaise ! --- L'Horloge démarre sans aucune autre Fonction en ligne ...       +++                    "); 
// Désactiver toutes les Fonctions
if (GMCMapFunction) {GMCMapFunction = false;}
if (WeatherFunctions) {WeatherFunctions = false;}
if (NewsDataFunction) {NewsDataFunction = false;}}
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
if (WatchOwner1FirstName != "") {
SHT= cleanText("       +++   Bonjour  "+WatchOwner1FirstName+"    ---    L'Horloge est Démarrée...       +++                    "); 
printStringWithShift(SHT.c_str(), ScrollTextTime);} else {
SHT = cleanText("       +++     L'Horloge est Démarrée...     +++                    ");   
printStringWithShift(SHT.c_str(), ScrollTextTime);}} 
// *************************************************************************************************************************************************************
MEP=true; Greetings_and_Holiday(true); MEP=false; // Tout d'abord, salutations et jours fériés si disponibles 
if (DoublePointStatus == 0) {dots = 0;} else {dots = 1;}  // // Statut Côlon
StatusWarningLED("CHECK"); // Vérifier la LED
// Première mesure régulière lorsque le Capteur DHT est prêt 
CheckInHumidityStatus(); // Mesure de l'humidité intérieure lorsque le capteur est connecté et OK

// Mesurer la Radioactivité
if (GMCMapFunction && AccessGMCMap) {
if (GMCMapMidnightAccessDelay(GMCMMAD)) {   
if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) { // uniquement si suffisamment de RAM est disponible 
SIC = true; 
printStringWithShift("GMCMap",ScrollTextTime);  // Attendez le Résultat de GMCMap
MEP=true; getGMCMapData(); MEP=false; // Obtenez les valeurs actuelles de Radioactivité
printStringWithShift("             ", ScrollTextTime); // Run GMCMap from the Display
GMCMapMessage(5,2);}}} // GMCMap Évaluer et afficher les données
if (SIC) {ClockScrollIn(); SIC = false;} // Faire défiler l'heure sur l'écran
await = false;} // Début de l'Horloge - Fin

  // Action de Minuit
  if (MidnightGhost == "YES" || EnhancedFunctions) {
  if (String(h) == "0" && String(m) == "0" && String(s) == "3" && Display) {PDP = true;} // DoublePoint activé  
  if (String(h) == "0" && String(m) == "0" && String(s) == "4") {
  await = true;
  MyWaitLoop(1000); // courte attente
  if (MidnightGhost && String(d) + "." + String(mo) != "1.1") {
  if (SerialMonitor) {DEBUG_PRINTLN(F("Show Ghost"));}  
  ShowGhost();} // Fantôme à minuit
  getTimeLocal(); 
  if (EnhancedFunctions) {MEP=true; Greetings_and_Holiday(false); MEP=false; // Déterminer les Salutations et les jours fériés pour le nouveau Jour
  DateString = ExpandedDateFR();}
  if (WeatherFunctions && AccessOpenWeatherMap) {
  if (IsEnoughRAM(MinRAMMem,"Météo") && IsWiFiSignalOK()) {MEP=true; getWeatherData(); MEP=false;}} // // Déterminer les nouvelles heures de lever et de coucher du soleil
  PDP = false; // Doublepoint est à nouveau désactivé en permanence en mode clignotant
  await = false;}}

  // ****** Éteindre Matrix automatiquement *******
  if (Display && DisplayOffTime != "" && String(d) + "." + String(mo) != "31.12") { // Laissez l'écran allumé le soir du Nouvel An
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":5") {PDP = true;} // Colon activé
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOffTime+":6") {     
  await = true;
  Display = false; 
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800); MyWaitLoop(1000);} // Bip d'arrêt de la Matrice
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  sendCmdAll(CMD_SHUTDOWN,0); // Arrêt de la Matrice
  digitalWrite(HWLEDPin, LOW); // LED éteinte
  if (SerialMonitor) {DEBUG_PRINTLN("Matrice éteinte à "+ String(h) + ":" + String(m));} 
  PDP = false; // Colon désactivé en permanence en mode clignotant
  await = false;}}

  // ***** Allumer Matrix automatiquement ******
  if (!Display && DisplayOnTime != "") {    
  if  (String(h)+":"+String(m)+":"+String(s) == DisplayOnTime+":6") {   
  await = true; 
  ClearMatrix();
  if (IsBuzzer && (StartSound == "On" || HourSound == "On")) {beep(NOTE_F4, 800);} // Bip d'activation de la Matrice
  MyWaitLoop(1500); // courte attente
  if (WatchOwner1FirstName != "") { 
  SText = Politeness(true);} else {SText = Politeness(false);}
  printStringWithShift(SText.c_str(),ScrollTextTime);
  CheckInHumidityStatus(); // Mesure de l'humidité intérieure lorsque le capteur est connecté et OK
  
  // Mesurer la Radioactivité
  if (GMCMapFunction && AccessGMCMap) {
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {   
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) { // seulement si suffisamment de RAM est disponible 
  printStringWithShift("        ", ScrollTextTime); // Laisse le temps s'écouler depuis l'affichage  
  printStringWithShift("GMCMap",ScrollTextTime);  // Attendez le résultat de GMCMap
  MEP=true; getGMCMapData(); MEP=false; // Obtenez les valeurs actuelles de Radioactivité
  printStringWithShift("             ", ScrollTextTime); // Exécutez GMCMap à partir de l'écran
  GMCMapMessage(5,2);}}} // GMCMap Évaluer et afficher les données
  ClockScrollIn(); // Faire défiler l'heure sur l'écran
  if (SerialMonitor) {DEBUG_PRINTLN("Matrice allumée à "+ String(h) + ":" + String(m));} 
  Display = true;
  await = false;}}

  // ******************** Commencer le programme actuel *******************************
  if (Display) {
  // Son à l'heure complète lorsqu'il est activé
  if (IsBuzzer) {  
  if (String(m) == "59" && String(s) == "59" && HourSound == "On") {
  await = true;  
  HourBuzzerBeep(); // Bip d'une heure complète
  await = false;}}

  MatrixAction = GoMatrixAction();
  if (MatrixAction) { // Plus d'actions 10 minutes avant que la Matrix ne s'éteigne automatiquement !
                          // (pour un arrêt sécurisé de la matrice - Pas de chevauchement possible avec une action)
  // Évaluation de l'humidité sur l'heure si le capteur est installé
  if ((InHumValue != "-") || GMCMapFunction) {
  if  (String(m)+":"+String(s) == "0:7") {PDP = true;} // Colon on
  if  (String(m)+":"+String(s) == "0:8") { 
  await = true;  
  CheckInHumidityStatus(); // Mesure de l'humidité intérieure lorsque le capteur est connecté et OK
  // Télécharger la valeur de Radioactivité
  if (GMCMapFunction && AccessGMCMap) {
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {  
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // seulement si suffisamment de RAM est disponible
  SIC = true; // ScrollIn Clock  
  printStringWithShift("             ", ScrollTextTime); // Laisse le temps s'écouler
  printStringWithShift("GMCMap",ScrollTextTime);  // Attendez le résultat de GMCMap
  MEP=true; getGMCMapData(); MEP=false; // Obtenir la valeur actuelle de la Radioactivité
  printStringWithShift("             ", ScrollTextTime); // Exécutez « GMCMap » à partir de l'écran
  GMCMapMessage(10,2);}}} // Évaluer et afficher les données GMCMap
  if (SIC) {ClockScrollIn(); SIC = false;} // Laissez la valeur de radioactivité s'exécuter à partir de l'écran 
  PDP = false; // Les deux points sont à nouveau éteints en permanence en mode clignotant
  await = false;}}

  // ###### Display Times for "OpenWeatherMAP", "NewsData" and "GMCMap" as well as Indoor/Outdoor Temperature and Humidity (DHT Sensor) ####
// ********** Minute 0 is Reserved for the Evaluation of Humidity and Radioactivity (if Activated) - do not use! ***********
// Actions toutes les 3 minutes
byte OnlyTempViewTimeCount = 6; // Nombre de fois
String OnlyTempViewTime[OnlyTempViewTimeCount] = {F("3"),F("12"),F("21"),F("36"),F("48"),F("57")}; // Afficher uniquement la température intérieure et extérieure (minute)
byte OnlyHumViewTimeCount = 5; // Nombre de fois
String OnlyHumViewTime[OnlyHumViewTimeCount]   = {F("6"),F("24"),F("33"),F("39"),F("51")}; // Afficher uniquement l'humidité de l'air intérieur et extérieur (minute)
byte DateNewsViewTimeCount = 5; // Nombre de fois
String DateNewsViewTime[DateNewsViewTimeCount] = {F("9"),F("18"),F("27"),F("42"),F("54")}; // Date actuelle, salutations et nouvelles (minute)
byte FullWeatherViewTimeCount = 2; //Nombre de fois
String FullWeatherViewTime[FullWeatherViewTimeCount] = {F("15"),F("45")}; // Météo complète (minute)
byte GMCMapViewTimeCount = 1; // Nombre de fois
String GMCMapViewTime[GMCMapViewTimeCount] = {F("30")}; // Radioactivité à l'emplacement sélectionné (ou météo complète si la radioactivité est désactivée) (minute)

//##############################################################################################################################################

  // Température intérieure et extérieure uniquement --- 3x par heure (par défaut)
  if ((InTempValue != "!")  || WeatherFunctions) {
  for (byte i=0; i < OnlyTempViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":10") {PDP = true;} // Colon sur
  if  (String(m)+":"+String(s) == OnlyTempViewTime[i]+":11") {    
    await = true;
     // *** Température intérieure ***
    if (InTempValue != "!") { // DHT active ?
    if (InTempCounter < 5) {
    if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();}  // Mesure de température
    MyWaitLoop(100); // courte attente
    String InTempString = "";
    if (InTempValue != "!") {
    IntInTempValue = (int16_t)strtol(InTempValue.c_str(), NULL, 10);
    if (InTempCounter > 0) {InTempCounter = 0;} // Réinitialisation des variables
    if (IntInTempValue < 0) {
    InTempValue.replace("-", String(SM)); // Remplacez Moins par Moins « court »
    if (IntInTempValue > -10) {
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT2) + " " + InTempValue + deg + "C";}} else {
    if (IntInTempValue > 9) {  
    InTempString = "           "+cleanText("Température Intérieure")+":      "+String(IT) + " " + InTempValue + deg + "C ";} else {
    InTempString = "           "+cleanText("Température Intérieure")+":       "+String(IT) + "  " + InTempValue + deg + "C ";}} 
    SIC = true; // Horloge de défilement
    if (InTempOffset != "999") {printStringWithShift(InTempString.c_str(), ScrollTextTime); // Afficher la Température intérieure
    MyWaitLoop(10000); // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez la Température intérieure s'afficher à partir de l'écran
    } else {InTempCounter += 1;} // inc
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("Capteur DHT désactivé ou cassé"));}
    StatusWarningLED("LOW"); // La LED s'éteint
    InTempValue = F("-");}} else {StatusWarningLED("LOW");} // La LED s'éteint

    // *** Température extérieure ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(MinRAMMem,"Météo") && IsWiFiSignalOK()) { // seulement si suffisamment de RAM est disponible 
    MEP=true; getWeatherData(); MEP=false; // Obtenez les données météorologiques actuelles (téléchargement)
    SIC = true; // Horloge de défilement
    if (InTempValue == "!") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(ATemp.c_str(), ScrollTextTime); // Afficher la Température extérieure
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime); // Laissez la température extérieure s'afficher à partir de l'écran
    }} // ou défragmenter la RAM  
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement
    PDP = false; // Colon désactivé en permanence en mode clignotant
    await = false;}}}

  // Humidité de l'air intérieur et extérieur uniquement --- 3x par heure (par défaut)
  if ((InHumValue != "-") || WeatherFunctions) {
  for (byte i=0; i < OnlyHumViewTimeCount; i++){
  if  (String(m)+":"+String(s) == OnlyHumViewTime[i]+":10") {PDP = true;} // Colon sur
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
    if (InTempOffset != "999") {printStringWithShift(InHumString.c_str(), ScrollTextTime); // Mesure de l'Humidité intérieure
    MyWaitLoop(10000); // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime);} // Laissez l'humidité intérieure et extérieure s'écouler à partir de l'écran.
    } else {if (SerialMonitor) {DEBUG_PRINTLN(F("Capteur DHT désactivé ou cassé"));}}

    // *** Humidité de l'air extérieur ***
    if (WeatherFunctions && AccessOpenWeatherMap){
    if (IsEnoughRAM(MinRAMMem,"Météo") && IsWiFiSignalOK()) {  // seulement si suffisamment de RAM est disponible
    MEP=true; getWeatherData(); MEP=false;  // Obtenez les données météorologiques actuelles (télécharger)
    SIC = true; // Horloge de défilement
    if (InHumidityStatus == "FAIL") {printStringWithShift("          ", ScrollTextTime);}
    printStringWithShift(AHum.c_str(), ScrollTextTime); // Afficher l'humidité extérieure
    if (StatusOpenWeatherMap == "OK") { MyWaitLoop(10000);} // attente courte (10 secondes - par défaut)
    printStringWithShift("             ", ScrollTextTime); // Laissez l'humidité de l'air extérieur s'écouler à partir de l'écran
    }} // ou défragmenter la RAM  
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement
    PDP = false; // Côlon éteint définitivement en mode clignotant
    await = false;}}}

  // Fonctions supplémentaires (date, vacances, anniversaires) et/ou affichage des actualités --- toutes les 10 minutes
  if (NewsDataFunction || EnhancedFunctions) {
  for (byte i=0; i < DateNewsViewTimeCount; i++){ 
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":10") {PDP = true;} // Double point sur     
  if  (String(m)+":"+String(s) == DateNewsViewTime[i]+":11") { 
  await = true;
  InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
  if (NewsDataFunction && AccessNewsData) {
  if (IsEnoughRAM(MinRAMMem,"Nouvelles") && IsWiFiSignalOK()) {MEP=true; getNews(); MEP=false;} else {APINews.clear();}} // obtenir des nouvelles actuelles
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (EnhancedFunctions) {
    DateString = ExpandedDateFR(); // Calculer la date avec la semaine civile
    SIC = true; // ScrollIn Clock
    printStringWithShift("        ", ScrollTextTime);
    printStringWithShift(DateString.c_str(), ScrollTextTime); // Date de sortie avec semaine civile
    if (GNCounter == 0) {MEP=true; Greetings_and_Holiday(false); MEP=false;} else {GreetingsToday = false;}
    GNCounter++;
    if (GNCounter > 1) {GNCounter = 0;}} // Afficher alternativement les salutations et les jours fériés ainsi que les actualités
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Messages de données d'actualité
    // Afficher le message sur l'affichage matriciel si aucun message d'accueil
    if (!GreetingsToday && NewsDataFunction && AccessNewsData) {
    InHumidityStatus = CheckInHumidity(); // Mesure de l'humidité intérieure
    if (APINews != "") {
    SIC = true; // Horloge de défilement  
    printStringWithShift("        ", ScrollTextTime);
    printStringWithShift(APINews.c_str(), ScrollTextTime);}} // Afficher les actualités
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (SIC) {ClockScrollIn(); SIC = false;} // Horloge de défilement 
    waitloop = 0; // Réinitialisation des variables
    PDP = false; // Côlon éteint définitivement en mode clignotant
    await = false;}}}

  // ---------- Météo détaillée ----------------
  if (WeatherFunctions && AccessOpenWeatherMap) { 
  for (byte i=0; i < FullWeatherViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":10") {PDP = true;} // Côlon activé     
  if  (String(m)+":"+String(s) == FullWeatherViewTime[i]+":11") { 
  await = true;
  if (IsEnoughRAM(MinRAMMem,"Météo") && IsWiFiSignalOK()) {  // seulement si suffisamment de RAM est disponible
  SIC = true;
  MEP=true; getWeatherData(); MEP=false; // Obtenez les données météorologiques actuelles
  printStringWithShift("          ", ScrollTextTime);
  printStringWithShift(weatherString.c_str(), ScrollTextTime);} // Faites défiler l'écran pour afficher l'heure ou défragmenter la RAM.
  if (SIC) {ClockScrollIn(); SIC = false;} // Faire défiler l'heure dans l'affichage 
  PDP = false; // Le côlon est définitivement désactivé lorsqu'il est en mode clignotant.
  await = false;}}}

  // GMCMap - Affichage de la Radioactivité
  if (GMCMapFunction && AccessGMCMap) { 
  if (GMCMapMidnightAccessDelay(GMCMMAD)) {  
  for (byte i=0; i < GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Colon sur     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true; 
  if (IsEnoughRAM(MinRAMMem,"GMC") && IsWiFiSignalOK()) {  // nur wenn genügend RAM zur Verfügung steht
  SIC = true;
  printStringWithShift("             ", ScrollTextTime); // Laisse le temps s'écouler
  printStringWithShift("GMCMap",ScrollTextTime);  // Attendez le résultat de GMCMap
  MEP=true; getGMCMapData(); MEP=false; // Obtenir la valeur actuelle de la Radioactivité
  printStringWithShift("             ", ScrollTextTime); // Exécutez « GMCMap » à partir de l'écran
  GMCMapMessage(10,2);} // Évaluer et afficher les données GMCMap ou défragmenter la RAM
  if (SIC) {ClockScrollIn(); SIC = false;} // Faire défiler l'heure dans l'affichage
  PDP = false; // Colon s'éteint à nouveau en permanence en mode clignotant
  await = false;}}}} else { // -------------------------------------------------------------------
  if (WeatherFunctions && AccessOpenWeatherMap) { // Météo détaillée si GMCMap est désactivé
  for (byte i=0; i < GMCMapViewTimeCount; i++) { 
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":10") {PDP = true;} // Colon sur     
  if  (String(m)+":"+String(s) == GMCMapViewTime[i]+":11") { 
  await = true;
  if (IsEnoughRAM(MinRAMMem,"Météo") && IsWiFiSignalOK()) {  // seulement si suffisamment de RAM est disponible
  SIC = true;
  MEP=true; getWeatherData(); MEP=false; // Obtenez des données météorologiques actuelles
  printStringWithShift("        ", ScrollTextTime);
  printStringWithShift(weatherString.c_str(), ScrollTextTime);
  printStringWithShift("               ", ScrollTextTime); // Laisse le temps s'écouler
  StatusWarningLED("LOW");}
  if (SIC) {ClockScrollIn(); SIC = false;} // Faire défiler l'heure dans l'affichage 
  PDP = false; // Colon s'éteint à nouveau en permanence en mode clignotant
  await = false;}}}}
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
}}}
} // Boucle - Fin