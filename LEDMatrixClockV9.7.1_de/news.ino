// **********************************(openweathermap.org)***********************************
// Test -->  http://newsapi.org/v2/top-headlines?country=de&apiKey=yourAPIKey
// *****************************************************************************************
const char *Host = "newsapi.org";
void getNews() {
/*****************************************************************************************************************************/
int authors_Count = 17;  
String authors[authors_Count] = {"Tagesschau.de", "Tagesspiegel", "Der Spiegel", "WDR Nachrichten", "n-tv NACHRICHTEN",
                                 "NDR.de", "rbb24", "WELT", "Rheinische Post", "NOZ", "Süddeutsche Zeitung - SZ.de",
                                 "finanzen.net", "MDR", "Zeit.de", "FAZ - Frankfurter Allgemeine Zeitung", "HAZ",
                                 "EXPRESS"};  // Deutsche Autoren
/*****************************************************************************************************************************/ 
String Country = F("de"); // Default de = Deutschland
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
bool IsTitle = false;
aFound = false; // Init
WiFiClient client; // WLAN Client  
HTTPClient http;  
APINews = F(""); // Variable initialisieren 
StatusNewsAPI = F("k.A.");
NewsAPILastConnectionDateTime = FormatedDateTimeDE(); // Connection Time;
if (!client.connect(Host, 80)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// *****************************************************************************************************************  
String apiGetData = "http://" + String(Host) + "/v2/top-headlines?country=" + Country + "&apiKey=" + newsKey;
// *****************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("News werden heruntergeladen - Bitte warten ..."));} 
   http.setRedirectLimit(3); // maximale Verbindungsversuche HTTP Client 
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
    MyWaitLoop(1000); //kurz warten
    int httpCode = http.GET();
    if (httpCode > 0) {  // Verbindung prüfen
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    // Daten auswerten
    StaticJsonDocument<256> filter; 
    for (int i = 0; i < 20; i++) { 
    filter["articles"][i]["author"] = true;
    filter["articles"][i]["title"] = true;}

 // DynamicJsonDocument doc2(1024); // for ESP8266 
    DynamicJsonDocument doc2(ESP.getMaxFreeBlockSize() - 1024); // for ESP8266 
 // DynamicJsonDocument doc2(ESP.getMaxAllocHeap()); // for ESP32
    DeserializationError error = deserializeJson(doc2, http.getStream(), DeserializationOption::Filter(filter));
    doc2.garbageCollect();
    doc2.shrinkToFit();

    if (SerialMonitor) {
    Serial.println(F("Liste aller Autoren:"));
    for (int i = 0; i < 20; i++) {
    String articles_author = doc2["articles"][i]["author"];
    Serial.println(String(i)+". Author: "+ articles_author);} // for Debug*/
    Serial.println(F(""));}

    // Daten suchen  
    if (SerialMonitor) {Serial.println("Parse Authors: "+FormatedDateTimeDE());}
    for (int i = 0; i < 20; i++) { // 20 Autoren bei NewsAPI vorhanden
    String articles_author = doc2["articles"][i]["author"]; 
    articles_author.toUpperCase();  
    String articles_title = doc2["articles"][i]["title"];
    articles_title.trim();
    if (articles_author != "" && articles_author != "null"){
    for (int l = 0; l < authors_Count; l++) {  
    authors[l].toUpperCase();      
    if (articles_author == authors[l]) {aFound = true;}} // Authors-Filter
    if (aFound){ 
    if (articles_title != "") {
    if (SerialMonitor) {Serial.println("Filter: "+String(i)+". Author: "+ articles_author);} 
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // für Matrix Display
    WPAPINews = "+++ "+articles_title+" +++"; // für WebPage
    if (SerialMonitor) {Serial.println("News: +++ "+articles_title+" +++");}  // für Serial Monitor
    break;}}}}
    if (SerialMonitor) {Serial.println(F(""));}
    if (APINews != "") {StatusNewsAPI = F("OK");
    if (SerialMonitor) {Serial.println(F("News erfolgreich geladen ..."));}} else {
    StatusNewsAPI = F("aktiviert");
    if (SerialMonitor) {Serial.println(F("keine neuen Nachrichten vorhanden... !"));}}
    doc2.clear();
    } else {
     APINews = F("     +++       Verbindung zum NewsAPI-Server fehlgeschlagen !     +++              ");  
     WPAPINews = F("+++ Verbindung zum NewsAPI-Server fehlgeschlagen ! +++");  
     StatusNewsAPI = F("Fehler"); 
     if (SerialMonitor) {
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));  
     Serial.println("ungültiger NewsAPI Request ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
     Serial.println(F(""));}}} else {
    APINews = F("     +++       Verbindung zum NewsAPI-Server fehlgeschlagen !     +++              ");  
    WPAPINews = F("+++ Verbindung zum NewsAPI-Server fehlgeschlagen ! +++"); 
    StatusNewsAPI = F("Fehler");  
    if (SerialMonitor) {
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("Verbindung zu NewsAPI fehlgeschlagen ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));}}      
http.end();
client.flush();
client.stop();
} else {
APINews = F("     +++       Verbindung zum NewsAPI-Server fehlgeschlagen !     +++              ");  
WPAPINews = F("+++ Verbindung zum NewsAPI-Server fehlgeschlagen ! +++"); 
StatusNewsAPI = F("Fehler");  
client.flush();
client.stop();  
if (SerialMonitor) {Serial.println(F("Verbindung zum NewsAPI-Server fehlgeschlagen !"));}}} // Fehlermeldung wenn Client nicht verbunden 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
