// **********************************(openweathermap.org) - Sortie en système métrique ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Port sécurisé  
WiFiClientSecure client; // Client WLAN  
client.setInsecure(); // Ne vérifie pas la signature
client.setTimeout(10000); 
bool FailConnect;
int ESPMaxFreeBlockSize;  
OWMLastConnectionDateTime = FormatedDateTimeFR(true); // Temps de connexion;
if (!client.connect(weatherHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// **********************************************************************************************************************  
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Téléchargement des données Météorologiques - Veuillez Patienter ..."));}
   HTTPClient http;
   http.setRedirectLimit(3); // nombre maximal de tentatives de connexion client HTTP
   http.begin(client, apiGetData); // Connecter
   http.useHTTP10(true);
   MyWaitLoop(1000); // attendez
    int httpCode = http.GET();
    if (httpCode > 0) {  // vérifie la connexion
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    // Filter
    StaticJsonDocument<128> filter; 
    filter["weather"]["main"] = true;
    filter["name"] = true;
    filter["sys"]["country"] = true;
    filter["main"]["temp"] = true;
    filter["main"]["humidity"] = true;
    filter["main"]["pressure"] = true;
    filter["main"]["temp_min"] = true;
    filter["main"]["temp_max"] = true;
    filter["wind"]["speed"] = true;
    filter["clouds"]["all"] = true;
    filter["wind"]["deg"] = true;

    // Evaluate Data
    DynamicJsonDocument doc(256); // pour ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024); // pour ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxAllocHeap()); // pour ESP32 
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    doc.shrinkToFit();
    doc.garbageCollect();

    // Assign Weather Data
    String weatherMain = doc["weather"]["main"];
    String weatherLocation = doc["name"];
    WeatherLocation = weatherLocation;
    String country = doc["sys"]["country"];
    temp = doc["main"]["temp"];
    humidity =  doc["main"]["humidity"];
    int pressure = doc["main"]["pressure"];
    float tempMin =  doc["main"]["temp_min"];
    float tempMax =  doc["main"]["temp_max"];
    float windSpeed = doc["wind"]["speed"];
    int clouds = doc["clouds"]["all"];
    // int windDeg =  doc["wind"]["deg"];
    doc.clear(); //}

    // -------------------------------- pour l'écran MAX7219 ------------------------------------------------------------------
    if (weatherLocation !=  "") {
    String ATemperature = String(temp, 0);
    ATemperature.replace("-0", "0");
    ATemperature.replace("+0", "0");
    ATemperature.replace("+", "");
    // ATemperature = "-10"; // uniquement pour BugFix
    // humidity = 100; // uniquement pour BugFix
    ATemperature.trim(); // Supprimer les espaces
    if (!IsNumeric(ATemperature)) {ATemperature = "-99";} // En cas d'erreur
    int IntATemperature = ATemperature.toInt();
    if (IntATemperature < 0) {
    ATemperature.replace("-", String(SM)); // Remplacer le signe moins par le signe moins « court »
    if (IntATemperature > -10) { 
    ATemp = "            "+cleanText("Température Extérieure")+":   " + String(AT) + " " + ATemperature + String(deg) + "C ";} else { // seulement la température actuelle  
    ATemp = "            "+cleanText("Température Extérieure")+":   " + String(AT2) + " " + ATemperature + String(deg) + "C";}} else { // seulement la température actuelle
    if (IntATemperature > 9) {   
    ATemp = "            "+cleanText("Température Extérieure")+":   " + String(AT) + " " + ATemperature + String(deg) + "C ";} else { // seulement la température actuelle
    ATemp = "            "+cleanText("Température Extérieure")+":   " + String(AT) + "  " + ATemperature + String(deg) + "C ";}} // seulement la température actuelle
    if (humidity > 99) {
    AHum =  "            "+cleanText("Humidité Extérieur")+":       " + String(AL) + " " + String(humidity) + " %";} else {
    if (humidity < 10) { 
    AHum =  "            "+cleanText("Humidité Extérieur")+":       " + String(AL) + "   " + String(humidity) + " % ";} else {   
    AHum =  "            "+cleanText("Humidité Extérieur")+":       " + String(AL) + " " + String(humidity) + " % ";}}

    weatherString = "      +++        "+cleanText("Les données Météorologiques Actuelles pour")+" " + String(cleanText(weatherLocation)) + ":    ---    "; // Nom de la ville
    WPweatherString = "+++ Les données Météorologiques Actuelles pour " + String(weatherLocation) + ":    ---    "; // Nom de la ville
    //weatherString += String(country); // Code du pays
    weatherString += cleanText("Température Extérieure")+":  " + ATemperature + String(deg) + "C    ---    "; // Température Extérieure
    WPweatherString += "Température Extérieure:  " + ATemperature + "°C    ---    "; // Température Extérieure
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "C "; // Temperature Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "C "; // Temperature Max
    weatherString += cleanText("Humidité de l'Air")+":  " + String(humidity) + " %    ---    "; // Humidité de l'Air
    WPweatherString += "Humidité de l'Air:  " + String(humidity) + " %    ---    "; // Humidité de l'Air
    weatherString += cleanText("Pression Atmosphérique")+":  " + String(pressure) + " hPa    ---    "; // Pression Atmosphérique
    WPweatherString += "Pression Atmosphérique:  " + String(pressure) + " hPa    ---    "; // Pression Atmosphérique
    weatherString += cleanText("Risque de Pluie")+":   " + String(clouds) + " %    ---    "; // Risque de Pluie
    WPweatherString += "Risque de Pluie:   " + String(clouds) + " %    ---    "; // Risque de Pluie
    weatherString += cleanText("Vitesse du Vent")+":   " + String(windSpeed*3.6, 0) + " km/h       +++              "; // Vitesse du vent
    WPweatherString += "Vitesse du Vent:   " + String(windSpeed*3.6, 0) + " km/h +++"; // Wind speed
    // weatherString += String(windDeg) + deg + " "; //Wind direction in Degree
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Vitesse et direction du vent

    // Données météorologiques actuelles pour le moniteur série
    if (SerialMonitor) {
    Serial.println("Données Météorologiques de: "+FormatedDateTimeFR(false));
    Serial.println("Les données Météorologiques actuelles pour " + String(weatherLocation)+":");
    Serial.println("Température Extérieure: " + String(temp, 0) + " °C");
    Serial.println("Température minimale: " + String(tempMin, 1) + " °C");
    Serial.println("Température maximale: " + String(tempMax, 1) + " °C");
    Serial.println("Humidité de l'Air: " + String(humidity) + " %");
    Serial.println("Pression Atmosphérique: " + String(pressure) + " hPa");
    Serial.println("Risque de Pluie: " + String(clouds) + " %");
    Serial.println("Vitesse du Vent: " + String(windSpeed*3.6, 0) + " km/h");
    Serial.println(F(""));} // pour DéBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Error");}
    // Premières données après redémarrage
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {Serial.println(F("Données Météo chargées avec succès ..."));}} else
                               {if (SerialMonitor) {Serial.println(F("Aucune donnée Météo actuelle chargée ..."));}}               
    } else {
    if (SerialMonitor) {  
    Serial.println("Aucune donnée Météo actuelle disponible ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
    Serial.println();} 
    weatherString = F("     +++       Aucune donnée Météo actuelle disponible !     +++              ");  
    WPweatherString = F("+++ Aucune donnée Météo actuelle disponible ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");} 
    } else {
     if (SerialMonitor) { 
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
     Serial.println("Demande OpenWeatherMap invalide ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
     Serial.println(F(""));}
     weatherString = F("     +++       Demande OpenWeatherMap invalide !     +++              ");  
     WPweatherString = F("+++ Demande OpenWeatherMap invalide ! +++"); 
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Error"); 
      }} else {
    if (SerialMonitor) {  
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("La connexion à OpenWeatherMap-Server a échoué ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
    Serial.println(F(""));} 
    weatherString = F("     +++       La connexion à OpenWeatherMap-Server a échoué !     +++              ");  
     WPweatherString = F("+++ La connexion à OpenWeatherMap-Server a échoué ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");}   
http.end(); // Fin du client HTTP     
client.flush();
client.stop(); // Fin client
} else { 
weatherString = F("     +++       La connexion à OpenWeatherMap-Server a échoué !     +++              ");  
WPweatherString = F("+++ La connexion à OpenWeatherMap-Server a échoué ! +++"); 
ATemp = weatherString; AHum = weatherString; 
StatusOpenWeatherMap = F("Error"); 
client.flush();
client.stop(); // Fin client
if (SerialMonitor) {Serial.println(F("La connexion à OpenWeatherMap-Server a échoué !"));}}} // message d'erreur si aucun client ne se connecte
// =============================================================================================================================================
// =============================================================================================================================================