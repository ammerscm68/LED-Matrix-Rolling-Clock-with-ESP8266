// **********************************(newsdata.io)******************************************
// Test -->  https://newsdata.io/api/1/latest?country=us&language=en&apikey==yourAPIKey
// *****************************************************************************************
// News-Sources ---> https://newsdata.io/news-sources
// *****************************************************************************************
void getNews() {
const char *NewsHost = "newsdata.io"; // News-Host
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Don't check signature
client.setTimeout(20000); 
/*****************************************************************************************************************************/
byte authors_Count = 20;  // Maximum: 255
String authors[authors_Count] = {F("News9live"), F("Mail Online"), // Json "source_name" GB Authors
                                 F("Mirror"), F("Google News"), F("Newbury Today"), F("Independent"), F("Lancashire Telegraph"), 
                                 F("Euronews"), F("Knowledia News"), F("Southern Daily Echo"), F("Manchester Evening News"), 
                                 F("The Worcester News"), F(" Wales Online"), F("Dailystar Co Uk"), F("The Herald"),
                                 F("Bristolpost Co Uk"), F("Medical News"), F("The Times"), F("Metro"), F("Express")};

/*****************************************************************************************************************************/ 
String Country = F("gb"); // Country Default is gb = United Kindom
String Language = F("english"); // Language Default is english
String ShortLanguage = F("en"); // Language Default is en=english
/*****************************************************************************************************************************/
bool FailConnect = false;
bool aFound = false;   
bool IsTitle = false;
byte ConnectCount = 0;						   
aFound = false; // Init
StatusNewsData = F("n/a");
NewsDataLastConnectionDateTime = FormatedDateTimeEN(true); // Connection Time;
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Connecting to the NewsData.io Server..."));}
client.connect(NewsHost, port);
while (!client.connected()) {ConnectCount += 1; MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // short Wait
if (!client.connect(NewsHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// *****************************************************************************************************************************************
String apiGetData = "https://" + String(NewsHost) + "/api/1/latest?country=" + Country + "&language="+ ShortLanguage +"&apiKey=" + newsKey;
// *****************************************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Download News - Please wait ..."));} 
HTTPClient http;  
   http.setRedirectLimit(5); // maximum connection attempts HTTP client 
   http.setTimeout(20000);
   http.useHTTP10(true);
   http.begin(client, apiGetData); // Connect
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {break;}} // short Wait
    int16_t httpCode = http.GET();
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if (httpCode > 0) {  // checks for connection
    if(httpCode == HTTP_CODE_OK) {
    NewsOutOfCredits = false; // Reset Credits  
     // JSON Filter
    JsonDocument filter;  // ArduinoJSON from Version: 7
    for (byte i = 0; i < 10; i++) { 
    filter["results"][i]["source_name"] = true;
    filter["results"][i]["title"] = true;
    filter["results"][i]["language"] = true;}
    filter.shrinkToFit(); 

    if (filter.overflowed()) {
    // JsonDocument overflow
    SaveClockStatus("Error News-JsonFilter:");
    http.end(); client.stop(); filter.clear(); ESP.restart();} else { 

    // DynamicJsonDocument doc(1024); // Json-Dokument erstellen
    JsonDocument doc; // Create Json-Dokument (from ArduinoJSON 7)
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    if (error != DeserializationError::NoMemory &&  error != DeserializationError::Ok) {
    SaveClockStatus("News-JsonDocument Error: ( "+String(error.c_str())+" )");
    http.end(); // HTTP Client close
    client.stop(); doc.clear(); filter.clear(); ESP.restart(); // Close WiFi Client
    } else { 

    doc.shrinkToFit();
    // doc.garbageCollect(); // No longer available from ArduinoJSON 7 
    http.end(); // HTTP Client close
    client.stop(); // WiFi Client close

    if (SerialMonitor) {
    DEBUG_PRINTLN(F("List all Authors:"));
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

    // Data Search
    if (SerialMonitor) {DEBUG_PRINTLN("Parse Authors: "+FormatedDateTimeEN(false));}
    for (byte i = 0; i < 10; i++) { // There are 10 authors at NewsData
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
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // for Matrix Display
    WPAPINews = "+++ "+articles_title+" +++"; // for WebPage
    if (SerialMonitor) {DEBUG_PRINTLN("News: +++ "+articles_title+" +++");}  // for Serial Monitor
    break;}}} else {APINews.clear(); WPAPINews.clear();}}
    if (SerialMonitor) {DEBUG_PRINTLN(F(""));}
    if (APINews != "") {StatusNewsData = F("OK");
    if (SerialMonitor) {DEBUG_PRINTLN(F("News loaded successfully ..."));}} 
    else {StatusNewsData = F("enabled");
    if (SerialMonitor) {
    DEBUG_PRINTLN(F("News loaded successfully ..."));  
    DEBUG_PRINTLN(F("No News available ... !"));}}
    doc.clear(); filter.clear();}}}} else {
    if(httpCode == 429) { // too Many Requests - Quota Exceeded
    NewsOutOfCredits = true;
    if (APINews != "") {StatusNewsData = F("OK");} else {StatusNewsData = F("enabled");}
    if (SerialMonitor) {DEBUG_PRINTLN(F("Access to News-Server Successful ..."));
    DEBUG_PRINTLN(String(NewsHost)+": API Quota Exhausted for one Day !");}  // 200 Requests per Day 
    } else {
    APINews = F("     +++       Invalid NewsData Request !     +++              ");  
    WPAPINews = F("+++ Invalid NewsData Request ! +++");  
    StatusNewsData = F("Error"); 
    if (SerialMonitor) {  
    DEBUG_PRINTLN("Invalid NewsData Request ! - " + String(apiGetData)); // Error Message if Client not connected
    DEBUG_PRINTLN(F(""));}}}} else {
    APINews = F("     +++       Connection to NewsData-Server failed !     +++              ");  
    WPAPINews = F("+++ Connection to NewsData-Server failed ! +++"); 
    StatusNewsData = F("Error");  
    if (SerialMonitor) {
    DEBUG_PRINTLN("Connection to NewsData-Server failed ! - " + String(apiGetData)); // Error Message if Client not connected
    DEBUG_PRINTLN(F(""));}}} else {APINews = F("     +++       Connection to NewsData-Server failed !     +++              ");  
WPAPINews = F("+++ Connection to NewsData-Server failed ! +++"); 
StatusNewsData = F("Error");  
client.stop();
if (SerialMonitor) {DEBUG_PRINTLN(F("Connection to NewsData-Server failed !"));}}} // error message if no client connect 
 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++