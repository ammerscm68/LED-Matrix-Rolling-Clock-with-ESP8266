// **********************************(openweathermap.org) - Ausgabe im metrischen System ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client
client.setInsecure(); // Signatur nicht prüfen
client.setTimeout(15000); 
bool FailConnect;
int ConnectCount = 0;
int ESPMaxFreeBlockSize;  
StatusOpenWeatherMap = F("k.A.");
OWMLastConnectionDateTime = FormatedDateTimeDE(true); // Connection Time;
if (SerialMonitor) {Serial.println(F(""));
Serial.println(F("Verbindung zum  OpenWeatherMap.org - Server herstellen..."));}
client.connect(weatherHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // kurz warten
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) { 
// ********************************************************************************************************************** 
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Wetterdaten werden heruntergeladen - Bitte warten ..."));}
   HTTPClient http; 
   http.setRedirectLimit(5); // maximale Verbindungsversuche HTTP Client 
   http.setTimeout(30000);
   http.useHTTP10(false);
   http.begin(client, apiGetData); // Verbinden
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {break;}} // kurz warten
    int httpCode = http.GET();
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if (httpCode > 0) {  // Verbindung prüfen
    if(httpCode == HTTP_CODE_OK) {
    // Filter
    StaticJsonDocument<128> filter; 
    filter["weather"][0]["main"] = true;
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

    // Daten auswerten
    DynamicJsonDocument doc(512); // für ESP8266 
    // DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024); // für ESP8266 
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    doc.shrinkToFit();
    doc.garbageCollect();

    // Wetterdaten zuordnen
    String weatherMain = doc["weather"][0]["main"]; // Regen oder Schnee ?
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

    // --------------------------------für die MAX7219 Anzeige ------------------------------------------------------------------
    if (weatherLocation !=  "") {
    String ATemperature = String(temp, 0);
    ATemperature.replace("-0", "0");
    ATemperature.replace("+0", "0");
    ATemperature.replace("+", "");
    // ATemperature = "-10"; // only for BugFix
	// humidity = 100; // only for BugFix											
    ATemperature.trim(); // Leerzeichen entfernen
    if (!IsNumeric(ATemperature)) {ATemperature = "-99";} // Im Fehlerfall
    int IntATemperature = ATemperature.toInt();
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
    WPweatherString = "+++ Die aktuellen Wetterdaten für " + String(weatherLocation) + ":    ---    "; //Ortsname
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
    weatherString += "Wind:   " + String(windSpeed*3.6, 0) + " km/h       +++              "; // Windgeschwindigkeit
    WPweatherString += "Wind:   " + String(windSpeed*3.6, 0) + " km/h +++"; // Windgeschwindigkeit
    // weatherString += String(windDeg) + deg + " "; //Windrichtung in Grad
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Windgeschwindigkeit und Richtung

    // Aktuelle Wetterdaten für Serial Monitor
    if (SerialMonitor) {
    Serial.println("Wetterdaten von: "+FormatedDateTimeDE(false));
    Serial.println("Die aktuellen Wetterdaten für " + String(weatherLocation)+":");
    Serial.println("Wetterverhältnis: "+ weatherMain);
    Serial.println("Außentemperatur: " + String(temp, 0) + " °C");
    Serial.println("Minimaltemperatur: " + String(tempMin, 1) + " °C");
    Serial.println("Maximaltemperatur: " + String(tempMax, 1) + " °C");
    Serial.println("Luftfeuchte: " + String(humidity) + " %");
    Serial.println("Luftdruck: " + String(pressure) + " hPa");
    Serial.println(RainORSnow(weatherMain)+": " + String(clouds) + " %");
    Serial.println("Windgeschwindigkeit: " + String(windSpeed*3.6, 0) + " km/h");
    Serial.println(F(""));} // for DeBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Fehler");}
    // Erste Daten nach Neustart
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {Serial.println(F("Wetterdaten erfolgreich geladen ..."));}} else
                               {if (SerialMonitor) {Serial.println(F("keine aktuellen Wetterdaten geladen ..."));}}          
    } else {
    if (SerialMonitor) {  
    Serial.println("keine aktuellen Wetterdaten vorhanden! - " + String(apiGetData)); 
    Serial.println();} 
    weatherString = F("     +++       keine aktuellen Wetterdaten vorhanden!     +++              ");  // Fehlermeldung wenn Client nicht verbunden
    WPweatherString = F("+++ keine aktuellen Wetterdaten vorhanden! +++"); // Fehlermeldung wenn Client nicht verbunden
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Fehler");} 
    } else {
     if (SerialMonitor) { 
	 Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));																			   
     Serial.println("ungültiger OpenWeatherMap Request ! - " + String(apiGetData)); // Fehlermeldung bei ungültigem API Request
     Serial.println(F(""));}
     weatherString = F("     +++       ungültiger OpenWeatherMap Request !     +++              ");  // Fehlermeldung bei ungültigem API Request
     WPweatherString = F("+++ ungültiger OpenWeatherMap Request ! +++"); // Fehlermeldung bei ungültigem API Request
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Fehler"); 
      }} else {
    if (SerialMonitor) {  
	Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));																				 
    Serial.println("Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! - " + String(apiGetData)); // Fehlermeldung wenn Client nicht verbunden
    Serial.println(F(""));} 
    weatherString = F("     +++       Verbindung zum OpenWeatherMap-Server fehlgeschlagen !     +++              ");  // Fehlermeldung wenn Client nicht verbunden
    WPweatherString = F("+++ Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! +++"); // Fehlermeldung wenn Client nicht verbunden
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Fehler");}   
http.end(); // HTTP Client beenden      
client.flush();
client.stop(); // Client beenden
freeRam(); // Defragment Heap
} else { 
weatherString = F("     +++       Verbindung zum OpenWeatherMap-Server fehlgeschlagen !     +++              ");  // Fehlermeldung wenn Client nicht verbunden
WPweatherString = F("+++ Verbindung zum OpenWeatherMap-Server fehlgeschlagen ! +++");  // Fehlermeldung wenn Client nicht verbunden
ATemp = weatherString; AHum = weatherString; 
StatusOpenWeatherMap = F("Fehler"); 
client.flush();
client.stop(); // Client beenden
freeRam(); // Defragment Heap
if (SerialMonitor) {Serial.println(F("Verbindung zum OpenWeatherMap-Server fehlgeschlagen !"));}}} // Fehlermeldung wenn Client nicht verbunden

// ==================================================================================================================================

String RainORSnow(String WCondition) {
WCondition.trim(); WCondition.toUpperCase();
if (WCondition == "SNOW") {
return "Schneefallwahrscheinlichkeit";} else {
return "Regenwahrscheinlichkeit";}}

// ==================================================================================================================================
// ==================================================================================================================================
