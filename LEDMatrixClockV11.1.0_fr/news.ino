// **********************************(newsdata.io)******************************************
// Test -->  https://newsdata.io/api/1/latest?country=fr&language=fr&apikey==yourAPIKey
// *****************************************************************************************
// News-Sources ---> https://newsdata.io/news-sources
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // Hôte de nouvelles
const uint16_t port = 443; // Port sécurisé  
WiFiClientSecure client; // Client WLAN  
client.setInsecure(); // Ne vérifie pas la signature
client.setTimeout(25000); 
/*****************************************************************************************************************************/
byte authors_Count = 12;  
String authors[authors_Count] = {"20 Minutes", "Le10sport", "Bfm Tv", "Investing French",// Json "source_name" FR Auteurs
                                 "Challenges", "Knowledia News", "Le Progrès", "Le Monde", "Bfm Rmc Sport",
                                 "Onze Mondial", "Lecho", "Le Figaro"};

/*****************************************************************************************************************************/ 
String Country = F("fr"); // Pays La valeur par défaut est fr = Français
String Language = F("french"); // Langue par défaut is Français
String ShortLanguage = F("fr"); // Langue par défaut is fr=Français
/*****************************************************************************************************************************/
bool FailConnect = false;
bool aFound = false;   
bool IsTitle = false;
byte ConnectCount = 0;
aFound = false; // Init
StatusNewsData = F("n/a");
NewsDataLastConnectionDateTime = FormatedDateTimeFR(true); // temps de connexion
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Connectez-vous au Serveur NewsData.io..."));}
client.connect(NewsHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100); if (ConnectCount > 45) {break;}} // attends brièvement
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) { 
// *****************************************************************************************************************************************
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&language="+ ShortLanguage +"&apiKey=" + newsKey;
// *****************************************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Télécharger les actualités - Veuillez patienter ..."));} 
HTTPClient http;  
   http.setRedirectLimit(5); // nombre maximal de tentatives de connexion client HTTP 
   http.setTimeout(25000);
   http.useHTTP10(true);
   http.begin(client, apiGetData); // Connecter
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 45) {break;}} // attends brièvement
    int16_t httpCode = http.GET();
    if (httpCode > 0) {  // vérifie la connexion
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    NewsOutOfCredits = false; // Réinitialiser les crédits  
    // Évaluer les données
    JsonDocument filter;  // ArduinoJSON version 7 et ultérieures
    for (byte i = 0; i < 10; i++) { 
    filter["results"][i]["source_name"] = true;
    filter["results"][i]["title"] = true;
    filter["results"][i]["language"] = true;}
    filter.shrinkToFit(); 

    if (filter.overflowed()) {
    // JsonDocument overflow / trop petite mémoire
    SaveClockStatus("Erreur News-JsonFilter:");
    http.end(); client.stop(); filter.clear(); ESP.restart();} else { 

    // DynamicJsonDocument doc(1024) ;  // Document Json
    JsonDocument doc; // Créer un document JSON (à partir d'ArduinoJSON 7)
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    if (error != DeserializationError::NoMemory &&  error != DeserializationError::Ok) {
    SaveClockStatus("Erreur News-JsonDocument: ( "+String(error.c_str())+" )");
    http.end(); // Quitter le client HTTP
    client.stop(); doc.clear(); filter.clear(); ESP.restart(); // Fermer le client Wi-Fi
    } else { 

    doc.shrinkToFit();
    // doc.garbageCollect(); // Gibt es nicht mehr ab ArduinoJSON 7
    http.end(); // Quitter le client HTTP
    client.stop(); //Fermer le client Wi-Fi

    if (SerialMonitor) {
    DEBUG_PRINTLN(F("Liste de tous les Auteurs:"));
    for (byte i = 0; i < 10; i++) {
    String articles_author = doc["results"][i]["source_name"];
    DEBUG_PRINTLN(String(i)+". Author: "+ articles_author);} // for Debug*/
    DEBUG_PRINTLN(F(""));}

    // ++++++++++++++ Articles de test_auteur +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    bool Author_Test = false;  // true = Test On    false= Test off
    if (Author_Test && !SerialMonitor) {
    APINews = "           +++    ";  
    for (byte i = 0; i < 10; i++) {
    String articles_author = doc["results"][i]["source_name"];
    APINews += String(i+1)+". Auteur: "+ cleanText(articles_author) + "   +++          ";}} else {
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Rechercher des données 
    if (SerialMonitor) {DEBUG_PRINTLN("Analyse des Auteurs: "+FormatedDateTimeFR(false));}
    for (byte i = 0; i < 10; i++) { // Il y a 10 Auteurs sur NewsData
    String articles_author = doc["results"][i]["source_name"]; 
    articles_author.toUpperCase();  
    String articles_title = doc["results"][i]["title"];
    articles_title.trim();
    String news_language = doc["results"][i]["language"];
    news_language.trim();
    if (articles_author != "" && articles_author != "null"){
    for (byte l = 0; l < authors_Count; l++) {  
    authors[l].toUpperCase();      
    if (articles_author == authors[l] && news_language == Language) {aFound = true;}} // Auteurs-Filtre
    if (aFound){ 
    if (articles_title != "") {
    if (SerialMonitor) {DEBUG_PRINTLN("Filter: "+String(i)+". Auteur: "+ articles_author);} 
    articles_title += " - "+articles_author;
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // for Matrix Display
    WPAPINews = "+++ "+articles_title+" +++"; // pour la Page Web
    if (SerialMonitor) {DEBUG_PRINTLN("Nouvelles: +++ "+articles_title+" +++");}  // for Serial Monitor
    break;}}} else {APINews.clear(); WPAPINews.clear();}}
    if (SerialMonitor) {DEBUG_PRINTLN(F(""));}
    if (APINews != "") {StatusNewsData = F("OK");
    if (SerialMonitor) {DEBUG_PRINTLN(F("Actualités Chargées avec Succès ..."));}} 
    else {StatusNewsData = F("enabled");
    if (SerialMonitor) {
    DEBUG_PRINTLN(F("Actualités Chargées avec Succès ..."));  
    DEBUG_PRINTLN(F("Aucune Nouvelle disponible ... !"));}}
    doc.clear(); filter.clear();}}}} else {
    if(httpCode == 429) { // Trop de demandes - quota dépassé
    NewsOutOfCredits = true; // Crédits dépassés
    if (APINews != "") {StatusNewsData = F("OK");} else {StatusNewsData = F("enabled");}
    if (SerialMonitor) {DEBUG_PRINTLN(F("Accès au News-Server réussi ..."));
    DEBUG_PRINTLN(String(NewsHost)+": Quota API épuisé pour un Jour !");} // Quota API = 200 Demandes par Jour
    } else {
    APINews = F("     +++       Demande NewsData Invalide !     +++              ");  
    WPAPINews = F("+++ Demande NewsData Invalide ! +++");  
    StatusNewsData = F("Error"); 
    if (SerialMonitor) {  
    DEBUG_PRINTLN("Demande NewsData Invalide ! - " + String(apiGetData));
    DEBUG_PRINTLN(F(""));}}}} else {
    APINews = cleanText("     +++       La connexion au NewsData-Server a échoué !     +++              ");  
    WPAPINews = F("+++ La connexion au NewsData-Server a échoué ! +++"); 
    StatusNewsData = F("Error");  
    if (SerialMonitor) {
    DEBUG_PRINTLN("La connexion au NewsData-Server a échoué ! - " + String(apiGetData)); 
    DEBUG_PRINTLN(F(""));}}} else {APINews = cleanText("     +++       La connexion au NewsData-Server a échoué !     +++              ");  
WPAPINews = F("+++ La connexion au NewsData-Server a échoué ! +++"); 
StatusNewsData = F("Error");  
client.stop();
if (SerialMonitor) {DEBUG_PRINTLN(F("La connexion au NewsData-Server a échoué !"));}}} // message d'erreur si aucun client ne se connecte
 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++