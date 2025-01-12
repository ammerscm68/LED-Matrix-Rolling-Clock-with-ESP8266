// **********************************(newsdata.io)***********************************
// Test -->  https://newsdata.io/api/1/latest?country=gb&apikey==yourAPIKey
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // News-Host
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Don't check signature
client.setTimeout(10000); 
/*****************************************************************************************************************************/
int authors_Count = 20;
String authors[authors_Count] = {"News9live", "Mail Online", // Json "source_name" GB Authors
                                 "Mirror", "Google News", "Newbury Today", "Independent", "Lancashire Telegraph", 
                                 "Euronews", "Knowledia News", "Southern Daily Echo", "Manchester Evening News", 
                                 "The Worcester News", " Wales Online", "Dailystar Co Uk", "The Herald",
                                 "Bristolpost Co Uk", "Medical News", "The Times", "Metro", "Express"};

/*****************************************************************************************************************************/ 
String Country = F("gb"); // Country Default is gb = United Kindom
String Language = F("english"); // Language Default is english
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
bool IsTitle = false;
int ConnectCount = 0;					 
aFound = false; // Init
StatusNewsData = F("k.A.");
NewsDataLastConnectionDateTime = FormatedDateTimeEN(true); // Connection Time;
if (SerialMonitor) {Serial.println(F(""));
Serial.println(F("Connecting to the NewsData.io Server..."));}
client.connect(NewsHost, port);
while (!client.connected()) {ConnectCount += 1; MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // short Wait
if (!client.connect(NewsHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) {
// *****************************************************************************************************************  
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&apiKey=" + newsKey;
// *****************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Download News - Please wait ..."));} 
HTTPClient http;  
   http.setRedirectLimit(5); // maximum connection attempts HTTP client 
   http.setTimeout(30000);
   http.useHTTP10(false);
   http.begin(client, apiGetData); // Connect
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {break;}} // short Wait
    int httpCode = http.GET();
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if (httpCode > 0) {  // checks for connection
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

    // Data Search
    if (SerialMonitor) {Serial.println("Parse Authors: "+FormatedDateTimeEN(false));}
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
    Serial.println("Invalid NewsData Request ! - " + String(apiGetData)); // Error Message if Client not connected
    Serial.println(F(""));}}}} else {
    APINews = F("     +++       Connection to NewsData-Server failed !     +++              ");  
    WPAPINews = F("+++ Connection to NewsData-Server failed ! +++"); 
    StatusNewsData = F("Error");  
    if (SerialMonitor) {
    Serial.println("Connection to NewsData-Server failed ! - " + String(apiGetData)); // Error Message if Client not connected
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
