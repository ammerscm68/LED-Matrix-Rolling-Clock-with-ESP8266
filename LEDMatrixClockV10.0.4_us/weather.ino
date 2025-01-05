// **********************************(openweathermap.org) - Output Imperial System ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Don't check signature
client.setTimeout(10000); 
bool FailConnect;
int ESPMaxFreeBlockSize;  
OWMLastConnectionDateTime = FormatedDateTimeUS(true); // Connection Time;
if (!client.connect(weatherHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// ********************************************************************************************************************** 
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=imperial&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Downloading Weather Data - Please wait ..."));}
   HTTPClient http;
   http.setRedirectLimit(3); // maximum connection attempts HTTP client
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
   MyWaitLoop(1000); //wait
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
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

    // Evaluate Data
    DynamicJsonDocument doc(512); // for ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024); // for ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxAllocHeap()); // for ESP32 
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    doc.shrinkToFit();
    doc.garbageCollect();

    // Assign Weather Data
    String weatherMain = doc["weather"][0]["main"]; // Rain or Snow ?
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

    // --------------------------------for the MAX7219 Display ------------------------------------------------------------------
    if (weatherLocation !=  "") {
    String ATemperature = String(temp, 0);
    ATemperature.replace("-0", "0");
    ATemperature.replace("+0", "0");
    ATemperature.replace("+", "");
    // ATemperature = "115"; // only for BugFix
    // humidity = 100; // only for BugFix
    ATemperature.trim(); // Remove spaces
    if (!IsNumeric(ATemperature)) {ATemperature = "-99";} // In case of an error
    int IntATemperature = ATemperature.toInt();
    if (IntATemperature < 0) {
    ATemperature.replace("-", String(SM)); // Replace Minus Sign with "short" Minus Sign 
    if (IntATemperature > -10) { 
    ATemp = "            Outdoor Temperature:      " + String(AT) + " " + ATemperature + String(deg) + "F ";} else { // only current Temperature  
    ATemp = "            Outdoor Temperature:      " + String(AT2) + " " + ATemperature + String(deg) + "F";}} else { // only current Temperature
    if (IntATemperature > 9) {   
    if (IntATemperature < 100) {  
    ATemp = "            Outdoor Temperature:      " + String(AT) + " " + ATemperature + String(deg) + "F ";} else { // only current Temperature
    ATemp = "            Outdoor Temperature:      " + String(AT) + " " + ATemperature + String(deg) + "F";}} else { // only current Temperature
    ATemp = "            Outdoor Temperature:      " + String(AT) + "  " + ATemperature + String(deg) + "F ";}} // only current Temperature
    if (humidity > 99) {
    AHum =  "            Outdoor Air Humidity:      " + String(AL) + " " + String(humidity) + " %";} else {
    if (humidity < 10) { 
    AHum =  "            Outdoor Air Humidity:      " + String(AL) + "   " + String(humidity) + " % ";} else {   
    AHum =  "            Outdoor Air Humidity:      " + String(AL) + " " + String(humidity) + " % ";}}

    weatherString = "      +++        The Current Weather Data for " + cleanText(weatherLocation) + ":    ---    "; // City Name
    WPweatherString = "+++ The Current Weather Data for " + String(weatherLocation) + ":    ---    "; // City Name
    //weatherString += String(country); // Country code
    weatherString += "Outdoor Temperature:   " + ATemperature + String(deg) + "F    ---    "; //Temperature
    WPweatherString += "Outdoor Temperature:   " + String(temp,0) + "°F    ---    "; //Temperature
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "F "; //Temperature Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "F "; // Temperature Max
    weatherString += "Air Humidity:  " + String(humidity) + " %    ---    "; // Air Humidity
    WPweatherString += "Air Humidity:  " + String(humidity) + " %    ---    "; // Air Humidity
    weatherString += "Air Pressure:  " + String(pressure) + " hPa    ---    "; // Air pressure
    WPweatherString += "Air Pressure:  " + String(pressure) + " hPa    ---    "; // Air pressure
    weatherString += RainORSnow(weatherMain)+":   " + String(clouds) + " %    ---    "; // Chance of Rain
    WPweatherString += RainORSnow(weatherMain)+":   " + String(clouds) + " %    ---    "; // Chance of Rain
    weatherString += "Wind:   " + String(windSpeed, 0) + " mph       +++              "; // Wind speed
    WPweatherString += "Wind: " + String(windSpeed, 0) + " mph +++"; // Wind speed
    // weatherString += String(windDeg) + deg + " "; //Wind direction in Degree
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Wind speed and direction

    // Current Weather Data for Serial Monitor
    if (SerialMonitor) {
    Serial.println("Weather Data from: "+FormatedDateTimeUS(false));
    Serial.println("The Current Weather Data for " + String(weatherLocation)+":");
    Serial.println("Outdoor Temperature: " + String(temp, 0) + " °F");
    Serial.println("Minimum Temperature: " + String(tempMin, 1) + " °F");
    Serial.println("Maximum Temperature: " + String(tempMax, 1) + " °F");
    Serial.println("Air Humidity: " + String(humidity) + " %");
    Serial.println("Air Pressure: " + String(pressure) + " hPa");
    Serial.println(RainORSnow(weatherMain)+": " + String(clouds) + " %");
    Serial.println("Wind Speed: " + String(windSpeed, 0) + " mph");
    Serial.println(F(""));} // for DeBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Error");}
    // First Data after restart
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {Serial.println(F("Weather Data loaded successfully ..."));}} else
                               {if (SerialMonitor) {Serial.println(F("No current Weather Data loaded ..."));}}               
    } else {
    if (SerialMonitor) {  
    Serial.println("No Current Weather Data available! - " + String(apiGetData)); //error message if no client connect
    Serial.println();} 
    weatherString = F("     +++       No Current Weather Data available!     +++              ");  
    WPweatherString = F("+++ No Current Weather Data available! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");} 
    } else {
     if (SerialMonitor) { 
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
     Serial.println("Invalid OpenWeatherMap Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println(F(""));}
     weatherString = F("     +++       Connection to OpenWeatherMap-Server failed !     +++              ");  
     WPweatherString = F("+++ Connection to OpenWeatherMap-Server failed ! +++"); 
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Error"); 
      }} else {
    if (SerialMonitor) {  
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("Connection to OpenWeatherMap-Server failed ! - " + String(apiGetData)); //error message if no client connect
    Serial.println(F(""));} 
    weatherString = F("     +++       Connection to OpenWeatherMap-Server failed !     +++              ");  
     WPweatherString = F("+++ Connection to OpenWeatherMap-Server failed ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");}   
http.end(); // HTTP Client beenden      
client.flush();
client.stop(); // Client beenden
} else { 
weatherString = F("     +++       Connection to OpenWeatherMap-Server failed !     +++              ");  
WPweatherString = F("+++ Connection to OpenWeatherMap-Server failed ! +++"); 
ATemp = weatherString; AHum = weatherString; 
StatusOpenWeatherMap = F("Error"); 
client.flush();
client.stop(); // Client beenden
if (SerialMonitor) {Serial.println(F("Connection to OpenWeatherMap-Server failed !"));}}} //error message if no client connect

// ==================================================================================================================================

String RainORSnow(String WCondition) {
WCondition.trim(); WCondition.toUpperCase();
if (SerialMonitor) {Serial.println("Weather Conditions: "+ WCondition);}
if (WCondition == "SNOW") {
return "Chance of Snow";} else {
return "Chance of Rain";}}

// ==================================================================================================================================
// ==================================================================================================================================
