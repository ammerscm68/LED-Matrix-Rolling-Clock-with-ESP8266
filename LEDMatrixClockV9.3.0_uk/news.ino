// **********************************(openweathermap.org)***********************************
// Test -->  http://newsapi.org/v2/top-headlines?country=de&apiKey=yourAPIKey
// *****************************************************************************************
const char *Host = "newsapi.org";
void getNews() {
/*****************************************************************************************************************************/
int authors_Count = 11;  
String authors[authors_Count] = {"The Independent", "Manchester Evening News", "The Mirror", "BBC", "Sky Sports",
                                 "The Guardian", "Financial Times", "Sky News", "The Times", "Daily Mail", "The Telegraph"};  // United Kingdom Authors
/*****************************************************************************************************************************/ 
String Country = "gb"; // Default gb = United Kingdom English
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
bool IsTitle = false;
aFound = false; // Init
WiFiClient client; // WLAN Client  
HTTPClient http;  
APINews = ""; // Initialize variable
StatusNewsAPI = "n/a"; 
NewsAPILastConnectionDateTime = FormatedDateTime(); // Connection Time;
if (!client.connect(Host, 80)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
String apiGetData = "http://" + String(Host) + "/v2/top-headlines?country=" + Country + "&apiKey=" + newsKey;
if (SerialMonitor) {
Serial.println("");
Serial.println("News werden heruntergeladen - Bitte warten ...");} 
   http.setRedirectLimit(3); // maximum connection attempts HTTP client 
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
    MyWaitLoop(1000); //wait
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    // Evaluate Data
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
    Serial.println("List all Authors:");
    for (int i = 0; i < 20; i++) {
    String articles_author = doc2["articles"][i]["author"];
    Serial.println(String(i)+". Author: "+ articles_author);} // for Debug*/
    Serial.println("");}

    // Search Data  
    if (SerialMonitor) {Serial.println("Parse Authors: "+FormatedDateTime());}
    for (int i = 0; i < 20; i++) { // There are 20 authors at NewsAPI
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
    APINews = "     +++       "+cleanText(articles_title)+"     +++              "; // for Matrix Display
    if (SerialMonitor) {Serial.println("News: +++ "+articles_title+" +++");}  // for Serial Monitor
    break;}}}}
    if (SerialMonitor) {Serial.println("");}
    if (APINews != "") {StatusNewsAPI = "OK";
    if (SerialMonitor) {Serial.println("News loaded successfully ...");}} else {
    StatusNewsAPI = "Error";
    if (SerialMonitor) {Serial.println("Error parsing News Data... !");}}
    doc2.clear();
    } else {
     APINews = "     +++       Invalid NewsAPI Request !     +++              ";
     StatusNewsAPI = "Error"; 
     if (SerialMonitor) {
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));  
     Serial.println("Invalid NewsAPI Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println("");}  
     newsKey = ""; 
     if (SerialMonitor) {
     Serial.println("APINews will be disabled! ..."); 
     Serial.println("");}}} else {
    APINews = "     +++       Connection to NewsAPI failed !     +++              ";   
    StatusNewsAPI = "Error";  
    if (SerialMonitor) {
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("Connection to NewsAPI failed ! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}}      
http.end();
client.flush();
client.stop();
} else {
APINews = "     +++       Connection to NewsAPI Server failed !     +++              ";  
StatusNewsAPI = "Error";  
client.flush();
client.stop();  
if (SerialMonitor) {Serial.println("Connection to NewsAPI Server failed !");}}} //error message if no client connect 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
