// **********************************(openweathermap.org)***********************************
// Test -->  http://newsapi.org/v2/top-headlines?country=de&apiKey=yourAPIKey
// *****************************************************************************************
const char *Host = "newsapi.org";
void getNews()
{
/*****************************************************************************************************************************/
int authors_Count = 12;  
String authors[authors_Count] = {"Tagesschau.de", "Bild", "Der Spiegel", "Zweites Deutsches Fernsehen", "n-tv NACHRICHTEN",
                                 "NDR.de", "rbb24", "WELT", "Rheinische Post", "NOZ", "Süddeutsche Zeitung - SZ.de",
                                 "finanzen.net"};  // Germany Authors
/*****************************************************************************************************************************/ 
String Country = "de"; // Default Germany
/*****************************************************************************************************************************/
bool FailConnect, aFound;  
aFound = false; // Init
String PrintNewsAPI;
WiFiClient client; // WLAN Client  
HTTPClient http;  
http.setRedirectLimit(10); // maximale Verbindungsversuche HTTP Client  
APINews = ""; // Variable initialisieren  
StatusNewsAPI = "k.A."; 
NewsAPILastConnectionDateTime = FormatedDateTime(); // Connection Time;
if (!client.connect(Host, 80)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
String apiGetData = "http://" + String(Host) + "/v2/top-headlines?country=" + Country + "&apiKey=" + newsKey;
Serial.println("");
Serial.println("News werden heruntergeladen - Bitte warten ...");
// http.addHeader("Content-Type", " application/x-www-form-urlencoded; charset=UTF-8");
   http.addHeader("Content-Type", "application/json");
   http.addHeader("Connection", "keep-alive");
   http.addHeader("User-Agent", "ESP8266/Arduino");
   http.begin(client, apiGetData); // Connect
   MyWaitLoop(1000); //wait
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    if(httpCode == HTTP_CODE_OK) {
    // Daten auswerten
    DynamicJsonDocument doc2(ESP.getMaxFreeBlockSize() - 1024);
    deserializeJson(doc2, http.getStream());

    // Newsdaten zuordnen
    JsonArray articles = doc2["articles"];
    JsonObject articles_0 = articles[0];
    JsonObject articles_1 = articles[1];
    JsonObject articles_2 = articles[2];
    JsonObject articles_3 = articles[3];
    JsonObject articles_4 = articles[4];
    JsonObject articles_5 = articles[5];
    JsonObject articles_6 = articles[6];
    JsonObject articles_7 = articles[7];
    JsonObject articles_8 = articles[8];
    JsonObject articles_9 = articles[9];
    JsonObject articles_10 = articles[10];
    JsonObject articles_11 = articles[11];
    JsonObject articles_12 = articles[12];
    JsonObject articles_13 = articles[13];
    JsonObject articles_14 = articles[14];
    JsonObject articles_15 = articles[15];
    JsonObject articles_16 = articles[16];
    JsonObject articles_17 = articles[17];
    JsonObject articles_18 = articles[18];
    JsonObject articles_19 = articles[19];
    String articles_0_author = articles_0["author"]; // Author von Title 0
    String articles_1_author = articles_1["author"]; // Author von Title 1
    String articles_2_author = articles_2["author"]; // Author von Title 2
    String articles_3_author = articles_3["author"]; // Author von Title 3
    String articles_4_author = articles_4["author"]; // Author von Title 4
    String articles_5_author = articles_5["author"]; // Author von Title 5
    String articles_6_author = articles_6["author"]; // Author von Title 6
    String articles_7_author = articles_7["author"]; // Author von Title 7
    String articles_8_author = articles_8["author"]; // Author von Title 8
    String articles_9_author = articles_9["author"]; // Author von Title 9
    String articles_10_author = articles_10["author"]; // Author von Title 10
    String articles_11_author = articles_11["author"]; // Author von Title 11
    String articles_12_author = articles_12["author"]; // Author von Title 12
    String articles_13_author = articles_13["author"]; // Author von Title 13
    String articles_14_author = articles_14["author"]; // Author von Title 14
    String articles_15_author = articles_15["author"]; // Author von Title 15
    String articles_16_author = articles_16["author"]; // Author von Title 16
    String articles_17_author = articles_17["author"]; // Author von Title 17
    String articles_18_author = articles_18["author"]; // Author von Title 18
    String articles_19_author = articles_19["author"]; // Author von Title 19
    articles_0_author.toUpperCase();
    articles_1_author.toUpperCase();
    articles_2_author.toUpperCase();
    articles_3_author.toUpperCase();
    articles_4_author.toUpperCase();
    articles_5_author.toUpperCase();
    articles_6_author.toUpperCase();
    articles_7_author.toUpperCase();
    articles_8_author.toUpperCase();
    articles_9_author.toUpperCase();
    articles_10_author.toUpperCase();
    articles_11_author.toUpperCase();
    articles_12_author.toUpperCase();
    articles_13_author.toUpperCase();
    articles_14_author.toUpperCase();
    articles_15_author.toUpperCase();
    articles_16_author.toUpperCase();
    articles_17_author.toUpperCase();
    articles_18_author.toUpperCase();
    articles_19_author.toUpperCase();

    // Serial.println(articles_5_author); // only for Debug Mode
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_0_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_0_title = articles_0["title"]; // Title 0  
    if (articles_0_title !=  "") {
    Serial.println("News articles 0 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_0_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_1_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_1_title = articles_1["title"]; // Title 1  
    if (articles_1_title !=  "") {
    Serial.println("News articles 1 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_1_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
       {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_2_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_2_title = articles_2["title"]; // Title 2 
    if (articles_2_title !=  "") {
    Serial.println("News articles 2 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_2_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
       {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_3_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_3_title = articles_3["title"]; // Title 3  
    if (articles_3_title !=  "") {
    Serial.println("News articles 3 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_3_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
     {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_4_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_4_title = articles_4["title"]; // Title 4  
    if (articles_4_title !=  "") {
    Serial.println("News articles 4 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_4_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_5_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_5_title = articles_5["title"]; // Title 5  
    if (articles_5_title !=  "") {
    Serial.println("News articles 5 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_5_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_6_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_6_title = articles_6["title"]; // Title 6  
    if (articles_6_title !=  "") {
    Serial.println("News articles 6 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_6_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_7_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_7_title = articles_7["title"]; // Title 7  
    if (articles_7_title !=  "") {
    Serial.println("News articles 7 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_7_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_8_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_8_title = articles_8["title"]; // Title 8  
    if (articles_8_title !=  "") {
    Serial.println("News articles 8 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_8_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_9_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_9_title = articles_9["title"]; // Title 9  
    if (articles_9_title !=  "") {
    Serial.println("News articles 9 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_9_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");   
    }
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_10_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_10_title = articles_10["title"]; // Title 10  
    if (articles_10_title !=  "") {
    Serial.println("News articles 10 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_10_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
      {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_11_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_11_title = articles_11["title"]; // Title 11  
    if (articles_11_title !=  "") {
    Serial.println("News articles 11 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_11_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
      {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_12_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_12_title = articles_12["title"]; // Title 12  
    if (articles_12_title !=  "") {
    Serial.println("News articles 12 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_12_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
       {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_13_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_13_title = articles_13["title"]; // Title 13  
    if (articles_13_title !=  "") {
    Serial.println("News articles 13 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_13_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
     {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_14_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_14_title = articles_14["title"]; // Title 14 
    if (articles_14_title !=  "") {
    Serial.println("News articles 14 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_14_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_15_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_15_title = articles_15["title"]; // Title 15 
    if (articles_15_title !=  "") {
    Serial.println("News articles 15 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_15_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
       {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_16_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_16_title = articles_16["title"]; // Title 16 
    if (articles_16_title !=  "") {
    Serial.println("News articles 16 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_16_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_17_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_17_title = articles_17["title"]; // Title 17 
    if (articles_17_title !=  "") {
    Serial.println("News articles 17 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_17_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_18_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_18_title = articles_18["title"]; // Title 18 
    if (articles_18_title !=  "") {
    Serial.println("News articles 18 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_18_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}
    } else
    {
    for (int i = 0; i < authors_Count; i++) {
    authors[i].toUpperCase();  
    if (articles_19_author == authors[i]) {aFound = true;}
    } 
    if (aFound = true){
    aFound = false;  
    const char* articles_19_title = articles_19["title"]; // Title 19 
    if (articles_19_title !=  "") {
    Serial.println("News articles 19 von: - "+FormatedDateTime()); 
    APINews = "     +++       "+cleanText(articles_19_title)+"     +++              ";
    PrintNewsAPI = APINews;
    PrintNewsAPI.trim();
    Serial.println("News: "+PrintNewsAPI);
    StatusNewsAPI = "OK";
    } else
    {
    StatusNewsAPI = "Fehler";
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");}}}}}}}}}}}}}}}}}}}}}
    
    Serial.println("News erfolgreich geladen ...");
    } else
     {
     StatusNewsAPI = "Fehler"; 
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));  
     Serial.println("ungültiger NewsAPI Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println("");  
     newsKey = ""; 
     Serial.println("APINews wird deaktiviert! ..."); 
     Serial.println("");  
      }  
    } else
    {
    StatusNewsAPI = "Fehler";  
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("Verbindung zu NewsAPI fehlgeschlagen ! - " + String(apiGetData)); //error message if no client connect
    Serial.println(""); 
      }
http.end();
client.flush();
client.stop();
} else {
StatusNewsAPI = "Fehler";  
Serial.println("Verbindung zum NewsAPI-Server fehlgeschlagen !"); //error message if no client connect}
}
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
