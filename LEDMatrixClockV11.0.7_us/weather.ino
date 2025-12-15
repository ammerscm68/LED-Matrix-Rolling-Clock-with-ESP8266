// **********************************(openweathermap.org) - Output Imperial System ***********************************
void getWeatherData() { 
const char *weatherHost = "api.openweathermap.org";  
const uint16_t port = 443; // Secure-Port  
WiFiClientSecure client; // WLAN Client  
client.setInsecure(); // Don't check signature
client.setTimeout(20000); 
bool FailConnect = false;
byte ConnectCount = 0;
int16_t ESPMaxFreeBlockSize;  
StatusOpenWeatherMap = F("n/a");
OWMLastConnectionDateTime = FormatedDateTimeUS(true); // Connection Time;
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Connecting to the OpenWeatherMap.org server..."));}
client.connect(weatherHost, port);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // short Wait
if (!client.connect(weatherHost, port)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// ********************************************************************************************************************** 
String apiGetData = "https://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=imperial&APPID="+weatherKey;
// **********************************************************************************************************************
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Downloading Weather Data - Please wait ..."));}
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
    // JSON Filter
    JsonDocument filter; // ArduinoJSON from Version: 7
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
    // JsonDocument overflow
    SaveClockStatus("Error Weather-JsonFilter:");
    http.end(); client.stop(); filter.clear(); ESP.restart();} else { 

   // DynamicJsonDocument doc(256) ;  // Json-Dokument
    JsonDocument doc; // Create Json-Dokument (from ArduinoJSON 7)
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    if (error != DeserializationError::NoMemory &&  error != DeserializationError::Ok) {
    SaveClockStatus("Weather-JsonDocument Error: ( "+String(error.c_str())+" )");
    http.end(); // HTTP Client close
    client.stop(); doc.clear(); filter.clear(); ESP.restart(); // WiFi Client close
    } else { 

    doc.shrinkToFit();
    // doc.garbageCollect(); // No longer available from ArduinoJSON 7 
    http.end(); // HTTP Client close
    client.stop(); // WiFi Client close

    // Assign Weather Data
    String weatherMain = doc["weather"][0]["main"]; // Rain or Snow ?
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

    // --------------------------------for the MAX7219 Display ------------------------------------------------------------------
    if (weatherLocation !=  "") {
    String ATemperature = String(temp, 0);
    ATemperature.replace("-0", "0");
    ATemperature.replace("+0", "0");
    ATemperature.replace("+", "");
    String dpsunrise = sunrise;
    dpsunrise.replace(":", String(dp));
    String dpsunset = sunset; 
    dpsunset.replace(":", String(dp));
    // ATemperature = "115"; // only for BugFix
    // humidity = 100; // only for BugFix
    ATemperature.trim(); // Remove spaces
    if (!IsNumeric(ATemperature)) {ATemperature = "-99";} // In case of an error
    int16_t IntATemperature = (int16_t)strtol(ATemperature.c_str(), NULL, 10);
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
    weatherString += "Wind:   " + String(windSpeed, 0) + " mph    ---    "; // Wind Speed
    WPweatherString += "Wind:   " + String(windSpeed, 0) + " mph    ---    "; // Wind Speed
    weatherString += "Sunrise:   " + dpsunrise + "    ---    "; // Sunrise
    WPweatherString += "Sunrise:   " + sunrise + " o'clock    ---    "; // Sunrise 
    weatherString += "Sunset:   " + dpsunset + "       +++              "; // Sunset
    WPweatherString += "Sunset:   " + sunset + " o'clock +++"; // Sunset
    // weatherString += String(windDeg) + deg + " "; //Wind direction in Degree
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Wind speed and direction

    // Current Weather Data for Serial Monitor
    if (SerialMonitor) {
    DEBUG_PRINTLN("Weather Data from: "+FormatedDateTimeUS(false));
    DEBUG_PRINTLN("The Current Weather Data for " + String(weatherLocation)+":");
    DEBUG_PRINTLN("Weather Conditions: "+ weatherMain);
    DEBUG_PRINTLN("Outdoor Temperature: " + String(temp, 0) + " °F");
    // DEBUG_PRINTLN("Minimum Temperature: " + String(tempMin, 1) + " °F");
    // DEBUG_PRINTLN("Maximum Temperature: " + String(tempMax, 1) + " °F");
    DEBUG_PRINTLN("Air Humidity: " + String(humidity) + " %");
    DEBUG_PRINTLN("Air Pressure: " + String(pressure) + " hPa");
    DEBUG_PRINTLN(RainORSnow(weatherMain)+": " + String(clouds) + " %");
    DEBUG_PRINTLN("Wind Speed: " + String(windSpeed, 0) + " mph");
    DEBUG_PRINTLN("Sunrise: " + sunrise + " o'clock");
    DEBUG_PRINTLN("Sunset: " + sunset + " o'clock");
    DEBUG_PRINTLN(F(""));} // for DeBug

    if (weatherLocation != "") {StatusOpenWeatherMap = F("OK");} else {StatusOpenWeatherMap = F("Error");}
    // First Data after restart
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {DEBUG_PRINTLN(F("Weather Data loaded successfully ..."));}} else
                               {if (SerialMonitor) {DEBUG_PRINTLN(F("No current Weather Data loaded ..."));}}               
    } else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("No Current Weather Data available! - " + String(apiGetData)); //error message if no client connect
    DEBUG_PRINTLN();} 
    weatherString = F("     +++       No Current Weather Data available!     +++              ");  
    WPweatherString = F("+++ No Current Weather Data available! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");} 
    }}} else {
     if (SerialMonitor) { 
     DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));
     DEBUG_PRINTLN("Invalid OpenWeatherMap Request ! - " + String(apiGetData)); //error message if no client connect
     DEBUG_PRINTLN(F(""));}
     weatherString = F("     +++       Invalid OpenWeatherMap Request !     +++              ");  
     WPweatherString = F("+++ Invalid OpenWeatherMap Request ! +++"); 
     ATemp = weatherString; AHum = weatherString;
     StatusOpenWeatherMap = F("Error"); 
      }} else {
    if (SerialMonitor) {  
    DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    DEBUG_PRINTLN("Connection to OpenWeatherMap-Server failed ! - " + String(apiGetData)); //error message if no client connect
    DEBUG_PRINTLN(F(""));} 
    weatherString = F("     +++       Connection to OpenWeatherMap-Server failed !     +++              ");  
     WPweatherString = F("+++ Connection to OpenWeatherMap-Server failed ! +++"); 
    ATemp = weatherString; AHum = weatherString;
    StatusOpenWeatherMap = F("Error");}} else { 
weatherString = F("     +++       Connection to OpenWeatherMap-Server failed !     +++              ");  
WPweatherString = F("+++ Connection to OpenWeatherMap-Server failed ! +++"); 
ATemp = weatherString; AHum = weatherString; 
StatusOpenWeatherMap = F("Error"); 
client.stop(); // Client close
if (SerialMonitor) {DEBUG_PRINTLN(F("Connection to OpenWeatherMap-Server failed !"));}}} //error message if no client connect

// ==================================================================================================================================

String RainORSnow(String WCondition) {
WCondition.trim(); WCondition.toUpperCase();
if (WCondition == "SNOW") {
return "Chance of Snow";} else {
return "Chance of Rain";}}

// ==================================================================================================================================