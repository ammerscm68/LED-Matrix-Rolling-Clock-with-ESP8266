// **********************************(openweathermap.org) - Output Imperial System ***********************************
const char *weatherHost = "api.openweathermap.org";
void getWeatherData() { 
bool FailConnect;
int ESPMaxFreeBlockSize; 
WiFiClient client; // WLAN Client  
HTTPClient http;  
OWMLastConnectionDateTime = FormatedDateTime(); // Connection Time;
if (!client.connect(weatherHost, 80)) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) { 
// cityID = "5128581"; // only for BugFix  
String apiGetData = "http://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=imperial&APPID="+weatherKey;
if (SerialMonitor) {
Serial.println("");
Serial.println("Downloading Weather Data - Please wait ...");}
   http.setRedirectLimit(3); // maximum connection attempts HTTP client
   http.begin(client, apiGetData); // Connect
   http.useHTTP10(true);
   MyWaitLoop(1000); //wait
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    if (SerialMonitor) {Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    if(httpCode == HTTP_CODE_OK) {
    // Evaluate Data
    DynamicJsonDocument doc(1024); // for ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024); // for ESP8266 
 // DynamicJsonDocument doc(ESP.getMaxAllocHeap()); // for ESP32 
    DeserializationError error = deserializeJson(doc, http.getStream());
    doc.shrinkToFit();
    doc.garbageCollect();

    // Assign Weather Data
    String weatherMain = doc["weather"]["main"];
    String weatherLocation = doc["name"];
    WeatherLocation = weatherLocation;
    String country = doc["sys"]["country"];
    float temp = doc["main"]["temp"];
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

    weatherString = "      +++        The Current Weather Data for " + String(weatherLocation) + ":    ---    "; // City Name
    //weatherString += String(country); // Country code
    weatherString += "Outdoor Temperature:   " + ATemperature + String(deg) + "F    ---    "; //Temperature
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "F "; //Temperature Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "F "; // Temperature Max
    weatherString += "Air Humidity:  " + String(humidity) + " %    ---    "; // Air Humidity
    weatherString += "Air Pressure:  " + String(pressure) + " hPa    ---    "; // Air pressure
    weatherString += "Chance of Rain:   " + String(clouds) + " %    ---    "; // Chance of Rain
    weatherString += "Wind:   " + String(windSpeed, 0) + " mph       +++              "; // Wind speed
    // weatherString += String(windDeg) + deg + " "; //Wind direction in Degree
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Wind speed and direction

    // Current Weather Data for Serial Monitor
    if (SerialMonitor) {
    Serial.println("Weather Data from: "+FormatedDateTime());
    Serial.println("The Current Weather Data for " + String(weatherLocation)+":");
    Serial.println("Outdoor Temperature: " + String(temp, 0) + " °F");
    Serial.println("Minimum Temperature: " + String(tempMin, 1) + " °F");
    Serial.println("Maximum Temperature: " + String(tempMax, 1) + " °F");
    Serial.println("Air Humidity: " + String(humidity) + " %");
    Serial.println("Air Pressure: " + String(pressure) + " hPa");
    Serial.println("Chance of Rain: " + String(clouds) + " %");
    Serial.println("wind Speed: " + String(windSpeed, 0) + " mph");
    Serial.println("");} // for DeBug

    if (weatherLocation != "") {StatusOpenWeatherMap = "OK";} else {StatusOpenWeatherMap = "Error";}
    // First Data after restart
    if (FirstWeatherData && weatherLocation != "") {FirstWeatherData = false;}
    if (weatherLocation != "") {if (SerialMonitor) {Serial.println("Weather Data loaded successfully ...");}} else
                               {if (SerialMonitor) {Serial.println("No current Weather Data loaded ...");}}          
    } else {
    if (SerialMonitor) {  
    Serial.println("No Current Weather Data available! - " + String(apiGetData)); //error message if no client connect
    Serial.println();} 
    StatusOpenWeatherMap = "Error";} 
    } else {
     if (SerialMonitor) { 
     Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
     Serial.println("Invalid OpenWeatherMap Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println();}
     StatusOpenWeatherMap = "Error"; 
     AccessOpenWeatherMap = false; 
     WeatherFunctions = false;
     weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
     cityID = ""; // City ID von OpenWeatherMap
     if (SerialMonitor) {
     Serial.println("OpenWeatherMap is disabled ! ..."); 
     Serial.println("");}  
      }} else {
    if (SerialMonitor) {  
    Serial.println("[HTTP] GET... code: " + HTTPCodeText(httpCode));
    Serial.println("Connection to OpenWeatherMap failed ! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");} 
    StatusOpenWeatherMap = "Error";}   
http.end(); // HTTP Client close      
client.flush();
client.stop(); // Client close
} else { 
StatusOpenWeatherMap = "Error"; 
client.flush();
client.stop(); // Client close
if (SerialMonitor) {Serial.println("Connection to OpenWeatherMap Server failed !");}}} //error message if no client connect
// ==================================================================================================================================
// ==================================================================================================================================
