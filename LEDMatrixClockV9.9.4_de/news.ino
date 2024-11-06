// **********************************(newsdata.io)***********************************
// Test -->  https://newsdata.io/api/1/latest?country=de&apikey==yourAPIKey
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // News-Host
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Signatur nicht prüfen
client.setTimeout(10000); 
/*****************************************************************************************************************************/
int authors_Count = 12;  
String authors[authors_Count] = {"Der Tagesspiegel", "Zdfmediathek",
                                 "Frankfurther Allgemeine Zeitung", "Handelsblatt", "Tz", 
                                 "Frankfurter Rundschau", "Tagesschau", "Sportschau", "News.de", 
                                 "Mdr.de", "Ntv", "Freie Presse"};  // // Json "source_name" - Deutsche Autoren

/*****************************************************************************************************************************/ 
String Country = F("de"); // Land Default de = Deutschland
String Language = F("german"); // Sprache Default german = Deutsch
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
bool IsTitle = false;
aFound = false; // Init
StatusNewsData = F("k.A.");
NewsDataLastConnectionDateTime = FormatedDateTimeDE(true); // Connection Time;
if (!client.connect(NewsHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// *****************************************************************************************************************  
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&apiKey=" + newsKey;
// *****************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("News werden heruntergeladen - Bitte warten ..."));} 
HTTPClient http;  
   http.setRedirectLimit(3); // maximale Verbindungsversuche HTTP Client 
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
    MyWaitLoop(1000); //kurz warten
    int httpCode = http.GET();
    if (httpCode > 0) {  // Verbindung prüfen
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    NewsOutOfCredits = false; // Reset Credits  
        // Daten auswerten
    StaticJsonDocument<128> filter; 
    for (int i = 0; i < 10; i++) { 
    filter["results"][i]["source_name"] = true;
    filter["results"][i]["title"] = true;
    filter["results"][i]["language"] = true;}

    DynamicJsonDocument doc2(512); // for ESP8266 
  //DynamicJsonDocument doc2(ESP.getMaxFreeBlockSize() - 1024); // for ESP8266 
    DeserializationError error = deserializeJson(doc2, http.getStream(), DeserializationOption::Filter(filter));
    doc2.garbageCollect();
    doc2.shrinkToFit();

    if (SerialMonitor) {
    Serial.println(F("Liste aller Autoren:"));
    for (int i = 0; i < 10; i++) {
    String articles_author = doc2["results"][i]["source_name"];
    Serial.println(String(i)+". Author: "+ articles_author);} // for Debug*/
    Serial.println(F(""));}

    // Daten suchen  
    if (SerialMonitor) {Serial.println("Parse Authors: "+FormatedDateTimeDE(false));}
    for (int i = 0; i < 10; i++) { // 10 Autoren bei NewsData vorhanden
    String articles_author = doc2["results"][i]["source_name"]; 
    articles_author.toUpperCase();  
    String articles_title = doc2["results"][i]["title"];
    articles_title.trim();
    String news_language = doc2["results"][i]["language"];
    news_language.trim();
    if (articles_author != "" && articles_author != "null"){
    for (int l = 0; l < authors_Count; l++) {  
    authors[l].toUpperCase();      
    if (articles_author == authors[l] && news_language == Language) {aFound = true;}} // Authors-Filter
    if (aFound){ 
    if (articles_title != "") {
    if (SerialMonitor) {Serial.println("Filter: "+String(i)+". Author: "+ articles_author);} 
    articles_title += " - "+articles_author;
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // für Matrix Display
    WPAPINews = "+++ "+articles_title+" +++"; // für WebPage
    if (SerialMonitor) {Serial.println("News: +++ "+articles_title+" +++");}  // für Serial Monitor
    break;}}} else {APINews = F(""); WPAPINews = F("");}}
    if (SerialMonitor) {Serial.println(F(""));}
    if (APINews != "") {StatusNewsData = F("OK");
    if (SerialMonitor) {Serial.println(F("News erfolgreich geladen ..."));}} 
    else {StatusNewsData = F("aktiviert");
    if (SerialMonitor) {
    Serial.println(F("News erfolgreich geladen ..."));  
    Serial.println(F("keine neuen Nachrichten vorhanden... !"));}}
    doc2.clear();} else {
    if(httpCode == 429) { // zu viele Requests - Kontingent überschritten
    NewsOutOfCredits = true; // Credits überschritten
    if (APINews != "") {StatusNewsData = F("OK");} else {StatusNewsData = F("aktiviert");}
    if (SerialMonitor) {Serial.println(F("Zugriff auf News-Server erfolgreich ..."));
    Serial.println(String(NewsHost)+": API-Kontingent für einen Tag aufgebraucht !");}
    } else {
    APINews = F("     +++       ungültiger NewsData Request !     +++              ");  
    WPAPINews = F("+++ ungültiger NewsData Request ! +++");  
    StatusNewsData = F("Fehler"); 
    if (SerialMonitor) {  
    Serial.println("ungültiger NewsData Request ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));}}}} else {
    APINews = F("     +++       Verbindung zum NewsData-Server fehlgeschlagen !     +++              ");  
    WPAPINews = F("+++ Verbindung zum NewsData-Server fehlgeschlagen ! +++"); 
    StatusNewsData = F("Fehler");  
    if (SerialMonitor) {
    Serial.println("Verbindung zu NewsData fehlgeschlagen ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));}}      
http.end();
client.flush();
client.stop();
freeRam(); // Defragment Heap
} else {APINews = F("     +++       Verbindung zum NewsData-Server fehlgeschlagen !     +++              ");  
WPAPINews = F("+++ Verbindung zum NewsData-Server fehlgeschlagen ! +++"); 
StatusNewsData = F("Fehler");  
client.flush();
client.stop();
freeRam(); // Defragment Heap 
if (SerialMonitor) {Serial.println(F("Verbindung zum NewsData-Server fehlgeschlagen !"));}}} // Fehlermeldung wenn Client nicht verbunden 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
