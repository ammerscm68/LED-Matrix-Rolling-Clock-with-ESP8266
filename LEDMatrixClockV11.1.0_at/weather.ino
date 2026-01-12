// **********************************(openweathermap.org) - Ausgabe im metrischen System ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client
client.setInsecure(); // Signatur nicht prüfen
client.setTimeout(25000); 
bool FailConnect = false;
byte ConnectCount = 0;  
StatusOpenWeatherMap = F("k.A.");
OWMLastConnectionDateTime = FormatedDateTimeDE(true); // Connection Time;
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Verbindung zum  OpenWeatherMap.org - Server herstellen..."));}
client.connect(weatherHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100); if (ConnectCount > 45) {break;}} // kurz warten
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) { 
// ********************************************************************************************************************** 
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Wetterdaten werden heruntergeladen - Bitte warten ..."));}
   HTTPClient http; 
   http.setRedirectLimit(5); // maximale Verbindungsversuche HTTP Client 
   http.setTimeout(25000);
   http.useHTTP10(true);
   http.begin(client, apiGetData); // Verbinden
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 45) {break;}} // kurz warten
    int16_t httpCode = http.GET();
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if (httpCode > 0) {  // Verbindung prüfen
    if(httpCode == HTTP_CODE_OK) {
    // Daten auswerten
    JsonDocument filter; // ArduinoJSON ab Version: 7
    filter["weather"][0]["main"] = true;
    filter["name"] = true;
    // filter["sys"]["country"] = true;
    filter["sys"]["sunrise"] = true;
    filter["sys"]["sunset"] = true;
    filter["main"]["temp"] = true;
    filter["main"]["humidity"] = true;
    filter["main"]["pressure"] = true;
    // filter["main"]["temp_min"] = true;
    // filter["main"]["temp_max"] = true;
    filter["wind"]["speed"] = true;
    // filter["wind"]["deg"] = true;
    filter["clouds"]["all"] = true;
    filter.shrinkToFit();

    if (filter.overflowed()) {
    // JsonDocument overflow / zu kleiner Speicher
    SaveClockStatus("Fehler Wetter-JsonFilter:");
    http.end(); client.stop(); filter.clear(); ESP.restart();} else { 

   // DynamicJsonDocument doc(256) ;  // Json-Dokument
    JsonDocument doc; // Json-Dokument erstellen (ab ArduinoJSON 7)
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    if (error != DeserializationError::NoMemory &&  error != DeserializationError::Ok) {
    SaveClockStatus("Wetter-JsonDocument Fehler: ( "+String(error.c_str())+" )");
    http.end(); // HTTP Client beenden
    client.stop(); doc.clear(); filter.clear(); ESP.restart(); // WiFi Client beenden
    } else { 

    doc.shrinkToFit();
    // doc.garbageCollect(); // Gibt es nicht mehr ab ArduinoJSON 7
    http.end(); // HTTP Client beenden
    client.stop(); // WiFi Client beenden

    // Wetterdaten zuordnen
    String weatherMain = doc["weather"][0]["main"]; // Regen oder Schnee ?
    String weatherLocation = doc["name"];
    WeatherLocation = weatherLocation;
    // String country = doc["sys"]["country"];
    String sunrise;
    sunrise.reserve(6);
    sunrise = SunBow(doc["sys"]["sunrise"]);
    String sunset;
    sunset.reserve(6);
    sunset = SunBow(doc["sys"]["sunset"]);
    temp = doc["main"]["temp"];
    humidity =  doc["main"]["humidity"];
    int16_t pressure = doc["main"]["pressure"];
    // float tempMin =  doc["main"]["temp_min"];
    // float tempMax =  doc["main"]["temp_max"];
    float windSpeed = doc["wind"]["speed"];
    // int16_t windDeg =  doc["wind"]["deg"];
    byte clouds = doc["clouds"]["all"]; // 0-100%
    doc.clear(); filter.clear();

    // --------------------------------für die MAX7219 Anzeige ------------------------------------------------------------------
    if (weatherLocation !=  "") {
    String ATemperature = String(temp, 0);
    ATemperature.replace("-0", "0");
    ATemperature.replace("+0", "0");
    ATemperature.replace("+", "");
    String dpsunrise = sunrise;
    dpsunrise.replace(":", String(dp));
    String dpsunset = sunset; 
    dpsunset.replace(":", String(dp));
    // ATemperature = "-10"; // only for BugFix
	// humidity = 100; // only for BugFix									 
    ATemperature.trim(); // Leerzeichen entfernen
    if (!IsNumeric(ATemperature)) {ATemperature = "-99";} // Im Fehlerfall
    int16_t IntATemperature = (int16_t)strtol(ATemperature.c_str(), NULL, 10);
    if (IntATemperature < 0) {
    ATemperature.replace("-", String(SM)); // Minus durch "kurzes" Minus ersetzen  
    if (IntATemperature > -10) { 
    ATemp = "            Au"+String(uss)+"entemperatur:          " + String(AT) + " " + ATemperature + String(deg) + "C ";} else { // nur aktuelle Temperatur  
    ATemp = "            Au"+String(uss)+"entemperatur:          " + String(AT2) + " " + ATemperature + String(deg) + "C";}} else { // nur aktuelle Temperatur
    if (IntATemperature > 9) {   
    ATemp = "            Au"+String(uss)+"entemperatur:          " + String(AT) + " " + ATemperature + String(deg) + "C ";} else { // nur aktuelle Temperatur
    ATemp = "            Au"+String(uss)+"entemperatur:          " + String(AT) + "  " + ATemperature + String(deg) + "C ";}} // nur aktuelle Temperatur
    if (humidity > 99) {
    AHum =  "            Au"+String(uss)+"enluftfeuchte:          " + String(AL) + " " + String(humidity) + " %";} else {
    if (humidity < 10) { 
    AHum =  "            Au"+String(uss)+"enluftfeuchte:          " + String(AL) + "   " + String(humidity) + " % ";} else {   
    AHum =  "            Au"+String(uss)+"enluftfeuchte:          " + String(AL) + " " + String(humidity) + " % ";}}

    weatherString = "      +++        Die aktuellen Wetterdaten f"+String(uue)+"r " + cleanText(weatherLocation) + ":    ---    "; //Ortsname
    WPweatherString = "+++ Die aktuellen Wetterdaten für " + weatherLocation + ":    ---    "; //Ortsname
    //weatherString += String(country); // Länderkennung
    weatherString += "Au"+String(uss)+"entemperatur:   " + ATemperature + String(deg) + "C    ---    "; //Temperatur
    WPweatherString += "Außentemperatur:   " + String(temp,0) + "°C    ---    "; //Temperatur
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "C "; //Temperatur Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "C "; // Temperatur Max
    weatherString += "Luftfeuchte:  " + String(humidity) + " %    ---    "; // Luftfeuchtigkeit
    WPweatherString += "Luftfeuchte:  " + String(humidity) + " %    ---    "; // Luftfeuchtigkeit
    weatherString += "Luftdruck:  " + String(pressure) + " hPa    ---    "; // Luftdruck
    WPweatherString += "Luftdruck:  " + String(pressure) + " hPa    ---    "; // Luftdruck
    weatherString += RainORSnow(weatherMain)+":   " + String(clouds) + " %    ---    "; // Regenrisiko
    WPweatherString += RainORSnow(weatherMain)+":   " + String(clouds) + " %    ---    "; // Regenrisiko
    weatherString += "Wind:   " + String(windSpeed*3.6, 0) + " km/h    ---    "; // Windgeschwindigkeit
    WPweatherString += "Wind:   " + String(windSpeed*3.6, 0) + " km/h    ---    "; // Windgeschwindigkeit
    weatherString += "Sonnenaufgang:   " + dpsunrise + " Uhr    ---    "; // Sonnenaufgang
    WPweatherString += "Sonnenaufgang:   " + sunrise + " Uhr    ---    "; // Sonnenaufgang 
    weatherString += "Sonnenuntergang:   " + dpsunset + " Uhr       +++              "; // Sonnenuntergang
    WPweatherString += "Sonnenuntergang:   " + sunset + " Uhr +++"; // Sonnenuntergang
    // weatherString += String(windDeg) + deg + " "; //Windrichtung in Grad
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Windgeschwindigkeit und Richtung

    // Aktuelle Wetterdaten für Serial Monitor
    if (SerialMonitor) {
    DEBUG_PRINTLN("Wetterdaten von: "+FormatedDateTimeDE(false));
    DEBUG_PRINTLN("Die aktuellen Wetterdaten für " + weatherLocation+":");
    DEBUG_PRINTLN("Wetterverhältnis: "+ weatherMain);
    DEBUG_PRINTLN("Außentemperatur: " + String(temp, 0) + " °C");
    // DEBUG_PRINTLN("Minimaltemperatur: " + String(tempMin, 1) + " °C");
    // DEBUG_PRINTLN("Maximaltemperatur: " + String(tempMax, 1) + " °C");
    DEBUG_PRINTLN("Luftfeuchte: " + String(humidity) + " %");
    DEBUG_PRINTLN("Luftdruck: " + String(pressure) + " hPa");
    DEBUG_PRINTLN(RainORSnow(weatherMain)+": " + String(clouds) + " %");
    DEBUG_PRINTLN("Windgeschwindigkeit: " + String(windSpeed*3.6, 0) + " km/h");
    DEBUG_PRINTLN("Sonnenaufgang: " + sunrise + " Uhr");
    DEBUG_PRINTLN("Sonnenuntergang: " + sunset + " Uhr");
    DEBUG_PRINTLN(F(""));} // for DeBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Fehler");}
    // Erste Daten nach Neustart
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {DEBUG_PRINTLN(F("Wetterdaten erfolgreich geladen ..."));}} else
                               {if (SerialMonitor) {DEBUG_PRINTLN(F("keine aktuellen Wetterdaten geladen ..."));}}          
    } else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("keine aktuellen Wetterdaten vorhanden! - " + String(apiGetData)); 
    DEBUG_PRINTLN();} 
    weatherString = F("     +++       keine aktuellen Wetterdaten vorhanden!     +++              ");  // Fehlermeldung wenn Client nicht verbunden
    WPweatherString = F("+++ keine aktuellen Wetterdaten vorhanden! +++"); // Fehlermeldung wenn Client nicht verbunden
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Fehler");} 
    }}} else {
     if (SerialMonitor) { 
	 DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));																 
     DEBUG_PRINTLN("ungültiger OpenWeatherMap Request ! - " + String(apiGetData)); // Fehlermeldung bei ungültigem API Request
     DEBUG_PRINTLN(F(""));}
     weatherString = F("     +++       ungültiger OpenWeatherMap Request !     +++              ");  // Fehlermeldung bei ungültigem API Request
     WPweatherString = F("+++ ungültiger OpenWeatherMap Request ! +++"); // Fehlermeldung bei ungültigem API Request
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Fehler"); 
      }} else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));																	
    DEBUG_PRINTLN("Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    DEBUG_PRINTLN(F(""));} 
    weatherString = F("     +++       Verbindung zum OpenWeatherMap-Server fehlgeschlagen !     +++              ");  // Fehlermeldung wenn Client nicht verbunden
    WPweatherString = F("+++ Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! +++"); // Fehlermeldung wenn Client nicht verbunden
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Fehler");}} else { 
    weatherString = F("     +++       Verbindung zum OpenWeatherMap-Server fehlgeschlagen !     +++              ");  // Fehlermeldung wenn Client nicht verbunden
    WPweatherString = F("+++ Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! +++");  // Fehlermeldung wenn Client nicht verbunden
    ATemp = weatherString; AHum = weatherString; 
    StatusOpenWeatherMap = F("Fehler"); 
    client.stop(); // Client beenden
    if (SerialMonitor) {DEBUG_PRINTLN(F("Verbindung zum OpenWeatherMap-Server fehlgeschlagen !"));}}} // Fehlermeldung wenn Client nicht verbunden

// ==================================================================================================================================

String RainORSnow(String WCondition) {
WCondition.trim(); WCondition.toUpperCase();
if (WCondition == "SNOW") {
return "Schneefallwahrscheinlichkeit";} else {
return "Regenwahrscheinlichkeit";}}

// ==================================================================================================================================