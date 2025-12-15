// **********************************(newsdata.io)******************************************
// Test -->  https://newsdata.io/api/1/latest?country=de&language=de&apikey==yourAPIKey
// *****************************************************************************************
// News-Sources ---> https://newsdata.io/news-sources
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // News-Host
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Signatur nicht prüfen
client.setTimeout(20000); 
/*****************************************************************************************************************************/
byte authors_Count = 22;  // Maximal = 255
String authors[authors_Count] = {F("Der Tagesspiegel"), F("Zdfmediathek"), F("Leipziger Zeitung"), F("Nordbayern"), F("Westfalenpost"),
                                 F("Frankfurther Allgemeine Zeitung"), F("Handelsblatt"), F("Tz"), F("Mannheimer Morgen"),
                                 F("Frankfurter Rundschau"), F("Tagesschau"), F("Sportschau"), F("News.de"), F("Merkur"), F("T-online"),
                                 F("Mdr.de"), F("Ntv"), F("Freie Presse"), F("Sueddeutsche Zeitung"), F("Bayerischer Rundfunk"),
                                 F("Neue Ruhr Zeitung"), F("Eurosport")}; // Json "source_name" - Deutsche Autoren

/*****************************************************************************************************************************/ 
String Country = F("de"); // Land Default de = Deutschland
String Language = F("german"); // Sprache Default german = Deutsch
String ShortLanguage = F("de"); // Sprache Default de = Deutsch
/*****************************************************************************************************************************/
bool FailConnect = false;
bool aFound = false; 
bool IsTitle = false;
byte ConnectCount = 0;
aFound = false; // Init
StatusNewsData = F("k.A.");
NewsDataLastConnectionDateTime = FormatedDateTimeDE(true); // Connection Time;
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Verbindung zum  NewsData.io - Server herstellen..."));}
client.connect(NewsHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // kurz warten
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) { 
// *****************************************************************************************************************************************
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&language="+ ShortLanguage +"&apiKey=" + newsKey;
// *****************************************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("News werden heruntergeladen - Bitte warten ..."));} 
HTTPClient http;  
   http.setRedirectLimit(5); // maximale Verbindungsversuche HTTP Client 
   http.setTimeout(20000);
   http.useHTTP10(true);
   http.begin(client, apiGetData); // Connect
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {break;}} // kurz warten
    int16_t httpCode = http.GET();
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if (httpCode > 0) {  // Verbindung prüfen
    if(httpCode == HTTP_CODE_OK) {
    NewsOutOfCredits = false; // Reset Credits
    // Daten auswerten
    JsonDocument filter;  // ArduinoJSON ab Version: 7
    for (byte i = 0; i < 10; i++) { 
    filter["results"][i]["source_name"] = true;
    filter["results"][i]["title"] = true;
    filter["results"][i]["language"] = true;}
    filter.shrinkToFit(); 

    if (filter.overflowed()) {
    // JsonDocument overflow / zu kleiner Speicher
    SaveClockStatus("Fehler News-JsonFilter:");
    http.end(); client.stop(); filter.clear(); ESP.restart();} else { 

    // DynamicJsonDocument doc(1024); // Json-Dokument erstellen
    JsonDocument doc; // Json-Dokument erstellen (ab ArduinoJSON 7)
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    if (error != DeserializationError::NoMemory &&  error != DeserializationError::Ok) {
    SaveClockStatus("News-JsonDocument Fehler: ( "+String(error.c_str())+" )");
    http.end(); // HTTP Client beenden
    client.stop(); doc.clear(); filter.clear(); ESP.restart(); // WiFi Client beenden
    } else { 

    doc.shrinkToFit();
    // doc.garbageCollect(); // Gibt es nicht mehr ab ArduinoJSON 7
    http.end(); // HTTP Client beenden
    client.stop(); // WiFi Client beenden

    if (SerialMonitor) {
    DEBUG_PRINTLN(F("Liste aller Autoren:"));
    for (byte i = 0; i < 10; i++) {
    String articles_author = doc["results"][i]["source_name"];
    DEBUG_PRINTLN(String(i)+". Author: "+ articles_author);} // for Debug*/
    DEBUG_PRINTLN(F(""));}

    // ++++++++++++++ Test articles_author's +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    bool Author_Test = false;  // true = Test On    false= Test off
    if (Author_Test && !SerialMonitor) {
    APINews = "           +++    ";  
    for (byte i = 0; i < 10; i++) {
    String articles_author = doc["results"][i]["source_name"];
    APINews += String(i+1)+". Author: "+ cleanText(articles_author) + "   +++          ";}} else {
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Daten suchen  
    if (SerialMonitor) {DEBUG_PRINTLN("Parse Authors: "+FormatedDateTimeDE(false));}
    for (byte i = 0; i < 10; i++) { // 10 Autoren bei NewsData vorhanden
    String articles_author = doc["results"][i]["source_name"]; 
    articles_author.toUpperCase();  
    String articles_title = doc["results"][i]["title"];
    articles_title.trim();
    String news_language = doc["results"][i]["language"];
    news_language.trim();
    if (articles_author != "" && articles_author != "null"){
    for (byte l = 0; l < authors_Count; l++) {  
    authors[l].toUpperCase();      
    if (articles_author == authors[l] && news_language == Language) {aFound = true;}} // Authors-Filter
    if (aFound){ 
    if (articles_title != "") {
    if (SerialMonitor) {DEBUG_PRINTLN("Filter: "+String(i)+". Author: "+ articles_author);} 
    articles_title += " - "+articles_author;
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // für Matrix Display
    // APINews += "     +++       News JSON Status: "+String(error.c_str())+"     +++              ";
    WPAPINews = "+++ "+articles_title+" +++"; // für WebPage
    // WPAPINews += "     +++ News JSON Status: "+String(error.c_str())+" +++        ";
    if (SerialMonitor) {DEBUG_PRINTLN("News: +++ "+articles_title+" +++");}  // für Serial Monitor
    break;}}} else {APINews.clear(); WPAPINews.clear();}}
    if (SerialMonitor) {DEBUG_PRINTLN(F(""));}
    if (APINews != "") {StatusNewsData = F("OK");
    if (SerialMonitor) {DEBUG_PRINTLN(F("News erfolgreich geladen ..."));}} 
    else {StatusNewsData = F("aktiviert");
    if (SerialMonitor) {
    DEBUG_PRINTLN(F("News erfolgreich geladen ..."));  
    DEBUG_PRINTLN(F("keine neuen Nachrichten vorhanden... !"));}}
    doc.clear(); filter.clear();}}}} else {
    if(httpCode == 429) { // zu viele Requests - Kontingent überschritten
    NewsOutOfCredits = true; // Credits überschritten
    if (APINews != "") {StatusNewsData = F("OK");} else {StatusNewsData = F("aktiviert");}
    if (SerialMonitor) {DEBUG_PRINTLN(F("Zugriff auf News-Server erfolgreich ..."));
    DEBUG_PRINTLN(String(NewsHost)+": API-Kontingent für einen Tag aufgebraucht !");}
    } else {
    APINews = F("     +++       ungültiger NewsData Request !     +++              ");  
    WPAPINews = F("+++ ungültiger NewsData Request ! +++");  
    StatusNewsData = F("Fehler"); 
    if (SerialMonitor) {  
    DEBUG_PRINTLN("ungültiger NewsData Request ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    DEBUG_PRINTLN(F(""));}}}} else {
    APINews = F("     +++       Verbindung zum NewsData-Server fehlgeschlagen !     +++              ");  
    WPAPINews = F("+++ Verbindung zum NewsData-Server fehlgeschlagen ! +++"); 
    StatusNewsData = F("Fehler");  
    if (SerialMonitor) {
    DEBUG_PRINTLN("Verbindung zu NewsData fehlgeschlagen ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    DEBUG_PRINTLN(F(""));}}} else {APINews = F("     +++       Verbindung zum NewsData-Server fehlgeschlagen !     +++              ");  
    WPAPINews = F("+++ Verbindung zum NewsData-Server fehlgeschlagen ! +++"); 
    StatusNewsData = F("Fehler");  
    client.stop(); 
    if (SerialMonitor) {DEBUG_PRINTLN(F("Verbindung zum NewsData-Server fehlgeschlagen !"));}}} // Fehlermeldung wenn Client nicht verbunden 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
