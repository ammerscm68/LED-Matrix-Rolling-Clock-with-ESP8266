// **********************************(openweathermap.org)***********************************
const char *Host = "newsapi.org";
void getNews()
{
String apiGetData = "http://" + String(Host) + "/v2/top-headlines?country=de" + "&apiKey=" + newsKey;
Serial.println("News werden heruntergeladen - Bitte warten ...");
   http.begin(wifiClient, apiGetData);
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
    // Daten auswerten
    DynamicJsonDocument doc2(ESP.getMaxFreeBlockSize() - 1024);
    deserializeJson(doc2, http.getStream());

    // Newsdaten zuordnen
    JsonArray articles = doc2["articles"];
    JsonObject articles_0 = articles[0];
    const char* articles_0_title = articles_0["title"]; // Title 0
    if (articles_0_title !=  "") {
    Serial.println("News von: - "+FormatedDateTime()+" - "+String(articles_0_title)); 
    String News = "     +++       "+cleanText(articles_0_title)+"     +++                      ";
    printStringWithShift(News.c_str(), ScrollTextTime); // Anzeige News
    } else
    {
    Serial.println("keine News vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println();   
    }
    } else
    {
     Serial.println("ungültiger NewsAPI Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println();   
      }  
    } else
    {
    Serial.println("Verbindung zu NewsAPI fehlgeschlagen ! - " + String(apiGetData)); //error message if no client connect
    Serial.println();  
      }
http.end();
wifiClient.flush();
wifiClient.stop();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
