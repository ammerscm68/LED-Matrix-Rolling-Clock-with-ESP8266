// **********************************(newsdata.io)***********************************
// Test -->  https://newsdata.io/api/1/latest?country=us&apikey==yourAPIKey
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // News-Host
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Don't check signature
client.setTimeout(10000); 
/*****************************************************************************************************************************/
int authors_Count = 20; 
String authors[authors_Count] = {"The Washington Post", "Yahoo! News",  // Json "source_name" US Authors
                                 "Chronicle Online", "Norfolk Daily News", "Americanbankingnews", "News-daily",  
                                 "Nbc 5 Dallas", "News-gazette", "Newspressnow", "Gazette", "Dailygazette.com",
                                 "Upi", "Newsitem", "The Business Journals", "Cbs News", "Daily Journal",
                                 "Daily Item", "Thedailynewsonline", "Lincoln Journal Star", "Idahostatejournal"};  

/*****************************************************************************************************************************/ 
String Country = F("us"); // Country Default is us = United States
String Language = F("english"); // Language Default is english
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
bool IsTitle = false;
aFound = false; // Init
StatusNewsData = F("k.A.");
NewsDataLastConnectionDateTime = FormatedDateTimeUS(true); // Connection Time;
if (!client.connect(NewsHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// *****************************************************************************************************************  
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&apiKey=" + newsKey;
// *****************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Download News - Please wait ..."));} 
HTTPClient http;  
   http.setRedirectLimit(3); // maximum connection attempts HTTP client 
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
    MyWaitLoop(1000); // short wait
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
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
    Serial.println(F("List all Authors:"));
    for (int i = 0; i < 10; i++) {
    String articles_author = doc2["results"][i]["source_name"];
    Serial.println(String(i)+". Author: "+ articles_author);} // for Debug*/
    Serial.println(F(""));}

    // Daten suchen  
    if (SerialMonitor) {Serial.println("Parse Authors: "+FormatedDateTimeUS(false));}
    for (int i = 0; i < 10; i++) { // There are 10 authors at NewsData
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
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // for Matrix Display
    WPAPINews = "+++ "+articles_title+" +++"; // for WebPage
    if (SerialMonitor) {Serial.println("News: +++ "+articles_title+" +++");}  // for Serial Monitor
    break;}}} else {APINews = F(""); WPAPINews = F("");}}
    if (SerialMonitor) {Serial.println(F(""));}
    if (APINews != "") {StatusNewsData = F("OK");
    if (SerialMonitor) {Serial.println(F("News loaded successfully ..."));}} 
    else {StatusNewsData = F("enabled");
    if (SerialMonitor) {
    Serial.println(F("News loaded successfully ..."));  
    Serial.println(F("No News available ... !"));}}
    doc2.clear();} else {
    if(httpCode == 429) { // too Many Requests - Quota Exceeded
    NewsOutOfCredits = true;
    if (APINews != "") {StatusNewsData = F("OK");} else {StatusNewsData = F("enabled");}
    if (SerialMonitor) {Serial.println(F("Access to News-Server Successful ..."));
    Serial.println(String(NewsHost)+": API Quota Exhausted for one Day !");}  // 200 Requests per Day 
    } else {
    APINews = F("     +++       Invalid NewsData Request !     +++              ");  
    WPAPINews = F("+++ Invalid NewsData Request ! +++");  
    StatusNewsData = F("Error"); 
    if (SerialMonitor) {  
    Serial.println("Invalid NewsData Request ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));}}}} else {
    APINews = F("     +++       Connection to NewsData-Server failed !     +++              ");  
    WPAPINews = F("+++ Connection to NewsData-Server failed ! +++"); 
    StatusNewsData = F("Error");  
    if (SerialMonitor) {
    Serial.println("Connection to NewsData-Server failed ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));}}      
http.end();
client.flush();
client.stop();
freeRam(); // Defragment Heap
} else {APINews = F("     +++       Connection to NewsData-Server failed !     +++              ");  
WPAPINews = F("+++ Connection to NewsData-Server failed ! +++"); 
StatusNewsData = F("Error");  
client.flush();
client.stop();
freeRam(); // Defragment Heap 
if (SerialMonitor) {Serial.println(F("Connection to NewsData-Server failed !"));}}} // error message if no client connect 
 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
