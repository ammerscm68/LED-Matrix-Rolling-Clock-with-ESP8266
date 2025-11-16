// **********************************(openweathermap.org) - Sortie en système métrique ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Port sécurisé  
WiFiClientSecure client; // Client WLAN  
client.setInsecure(); // Ne vérifie pas la signature
client.setTimeout(20000); 
bool FailConnect = false;
int ConnectCount = 0;
int ESPMaxFreeBlockSize;  
StatusOpenWeatherMap = F("n/a");
OWMLastConnectionDateTime = FormatedDateTimeFR(true); // Connection Time;
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Connexion au Serveur OpenWeatherMap.org..."));}
client.connect(weatherHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // short Wait
if (!client.connect(weatherHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// **********************************************************************************************************************  
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Téléchargement des données Météorologiques - Veuillez Patienter ..."));}
   HTTPClient http;
   http.setRedirectLimit(5); // nombre maximal de tentatives de connexion client HTTP 
   http.setTimeout(20000);
   http.useHTTP10(true);
   http.begin(client, apiGetData); // Connecter
   MyWaitLoop(1000); // attendez
    int httpCode = http.GET();
    if (httpCode > 0) {  // vérifie la connexion
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    // Filter
    StaticJsonDocument<64> filter; 
    filter["weather"][0]["main"] = true;
    filter["name"] = true;
    filter["sys"]["country"] = true;
    filter["sys"]["sunrise"] = true;
    filter["sys"]["sunset"] = true;
    filter["main"]["temp"] = true;
    filter["main"]["humidity"] = true;
    filter["main"]["pressure"] = true;
    filter["main"]["temp_min"] = true;
    filter["main"]["temp_max"] = true;
    filter["wind"]["speed"] = true;
    filter["clouds"]["all"] = true;
    filter["wind"]["deg"] = true;

    // Evaluate Data
    DynamicJsonDocument doc(64); // pour ESP8266 
    // DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024); // pour ESP8266 
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    doc.shrinkToFit();
    doc.garbageCollect();

    http.end(); // Quitter le Client HTTP
    client.stop(); // Quitter le Client WiFi

    // Assign Weather Data
    String weatherMain = doc["weather"][0]["main"]; // Pluie ou Neige ?
    String weatherLocation = doc["name"];
    WeatherLocation = weatherLocation;
    String country = doc["sys"]["country"];
    String sunrise = SunBow(doc["sys"]["sunrise"]);
    String sunset = SunBow(doc["sys"]["sunset"]);
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
    String dpsunrise = sunrise;
    dpsunrise.replace(":", String(dp));
    String dpsunset = sunset; 
    dpsunset.replace(":", String(dp));
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

    weatherString = "      +++        "+cleanText("Les données Météorologiques Actuelles pour")+" " + cleanText(weatherLocation) + ":    ---    "; // Nom de la ville
    WPweatherString = "+++ Les données Météorologiques Actuelles pour " + String(weatherLocation) + ":    ---    "; // Nom de la ville
    //weatherString += String(country); // Code du pays
    weatherString += cleanText("Température Extérieure")+":  " + ATemperature + String(deg) + "C    ---    "; // Température Extérieure
    WPweatherString += "Température Extérieure:  " + String(temp,0) + "°C    ---    "; // Température Extérieure
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "C "; // Temperature Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "C "; // Temperature Max
    weatherString += cleanText("Humidité de l'Air")+":  " + String(humidity) + " %    ---    "; // Humidité de l'Air
    WPweatherString += "Humidité de l'Air:  " + String(humidity) + " %    ---    "; // Humidité de l'Air
    weatherString += cleanText("Pression Atmosphérique")+":  " + String(pressure) + " hPa    ---    "; // Pression Atmosphérique
    WPweatherString += "Pression Atmosphérique:  " + String(pressure) + " hPa    ---    "; // Pression Atmosphérique
    weatherString += cleanText(RainORSnow(weatherMain))+":   " + String(clouds) + " %    ---    "; // Risque de Pluie
    WPweatherString += RainORSnow(weatherMain)+":   " + String(clouds) + " %    ---    "; // Risque de Pluie
    weatherString += cleanText("Vitesse du Vent")+":   " + String(windSpeed*3.6, 0) + " km/h    ---    "; // Vitesse du vent
    WPweatherString += "Vitesse du Vent:   " + String(windSpeed*3.6, 0) + " km/h    ---    "; // Vitesse du vent
    weatherString += cleanText("Lever du Soleil")+":   " + dpsunrise + "    ---    "; // Lever du Soleil
    WPweatherString += "Lever du Soleil:   " + sunrise + " Heures    ---    "; // Lever du Soleil 
    weatherString += cleanText("Coucher de Soleil")+":   " + dpsunset + "       +++              "; // Coucher de Soleil
    WPweatherString += "Coucher de Soleil:   " + sunset + " Heures +++"; // Coucher de Soleil
    // weatherString += String(windDeg) + deg + " "; //Wind direction in Degree
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Vitesse et direction du vent

    // Données météorologiques actuelles pour le moniteur série
    if (SerialMonitor) {
    DEBUG_PRINTLN("Données Météorologiques de: "+FormatedDateTimeFR(false));
    DEBUG_PRINTLN("Conditions Météorologiques: "+ weatherMain);
    DEBUG_PRINTLN("Les données Météorologiques actuelles pour " + String(weatherLocation)+":");
    DEBUG_PRINTLN("Température Extérieure: " + String(temp, 0) + " °C");
    DEBUG_PRINTLN("Température minimale: " + String(tempMin, 1) + " °C");
    DEBUG_PRINTLN("Température maximale: " + String(tempMax, 1) + " °C");
    DEBUG_PRINTLN("Humidité de l'Air: " + String(humidity) + " %");
    DEBUG_PRINTLN("Pression Atmosphérique: " + String(pressure) + " hPa");
    DEBUG_PRINTLN(RainORSnow(weatherMain)+": " + String(clouds) + " %");
    DEBUG_PRINTLN("Vitesse du Vent: " + String(windSpeed*3.6, 0) + " km/h");
    DEBUG_PRINTLN("Lever du Soleil: " + sunrise + " Heures");
    DEBUG_PRINTLN("Coucher de Soleil: " + sunset + " Heures");
    DEBUG_PRINTLN(F(""));} // pour DéBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Error");}
    // Premières données après redémarrage
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {DEBUG_PRINTLN(F("Données Météo chargées avec succès ..."));}} else
                               {if (SerialMonitor) {DEBUG_PRINTLN(F("Aucune donnée Météo actuelle chargée ..."));}}               
    } else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("Aucune donnée Météo actuelle disponible ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
    DEBUG_PRINTLN();} 
    weatherString = cleanText("     +++       Aucune donnée Météo actuelle disponible !     +++              ");  
    WPweatherString = F("+++ Aucune donnée Météo actuelle disponible ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");} 
    } else {
     if (SerialMonitor) { 
     DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));
     DEBUG_PRINTLN("Demande OpenWeatherMap invalide ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
     DEBUG_PRINTLN(F(""));}
     weatherString = F("     +++       Demande OpenWeatherMap invalide !     +++              ");  
     WPweatherString = F("+++ Demande OpenWeatherMap invalide ! +++"); 
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Error"); 
      }} else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    DEBUG_PRINTLN("La connexion à OpenWeatherMap-Server a échoué ! - " + String(apiGetData)); // message d'erreur si aucun client ne se connecte
    DEBUG_PRINTLN(F(""));} 
    weatherString = cleanText("     +++       La connexion à OpenWeatherMap-Server a échoué !     +++              ");  
     WPweatherString = F("+++ La connexion à OpenWeatherMap-Server a échoué ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");}} else { 
weatherString = cleanText("     +++       La connexion à OpenWeatherMap-Server a échoué !     +++              ");  
WPweatherString = F("+++ La connexion à OpenWeatherMap-Server a échoué ! +++"); 
ATemp = weatherString; AHum = weatherString; 
StatusOpenWeatherMap = F("Error"); 
client.stop(); // Fin client
freeRam(); // Defragment Heap
if (SerialMonitor) {DEBUG_PRINTLN(F("La connexion à OpenWeatherMap-Server a échoué !"));}}} // message d'erreur si aucun client ne se connecte

// ==================================================================================================================================

String RainORSnow(String WCondition) {
WCondition.trim(); WCondition.toUpperCase();
if (WCondition == "SNOW") {
return "Risque de Neige";} else {
return "Risque de Pluie";}}

// =============================================================================================================================================