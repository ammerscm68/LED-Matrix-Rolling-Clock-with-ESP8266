// **********************************(openweathermap.org)***********************************
const char *weatherHost = "api.openweathermap.org";
void getWeatherData()
{
String apiGetData = "http://" + String(weatherHost) + "/data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey;
Serial.println("Wetterdaten werden heruntergeladen - Bitte warten ...");
   http.begin(wifiClient, apiGetData);
    int httpCode = http.GET();
    if (httpCode > 0) {  // checks for connection
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
    // Daten auswerten
    DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 1024);
    deserializeJson(doc, http.getStream());
    
    // Wetterdaten zuordnen
    String weatherMain = doc["weather"]["main"];
    String weatherDescription = doc["weather"]["description"];
    weatherDescription.toLowerCase();
    String weatherLocation = doc["name"];
    WeatherLocation = weatherLocation;
    String country = doc["sys"]["country"];
    float temp = doc["main"]["temp"];
    int humidity =  doc["main"]["humidity"];
    int pressure = doc["main"]["pressure"];
    float tempMin =  doc["main"]["temp_min"];
    float tempMax =  doc["main"]["temp_max"];
    float windSpeed = doc["wind"]["speed"];
    int windDeg =  doc["wind"]["deg"];
    int clouds = doc["clouds"]["all"];
    
    if (weatherLocation !=  "") {
    ATemp = "            Au"+uss+"entemperatur:        " + String(temp, 0) + deg + "C  "; // nur aktuelle Temperatur
    weatherString = "      +++        Die aktuellen Wetterdaten f"+uue+"r " + String(weatherLocation) + ":    ---    "; //Ortsname
    //weatherString += String(country); // Länderkennung
    weatherString += "Au"+uss+"entemperatur:   " + String(temp, 0) + deg + "C    ---    "; //Temperatur
    // weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "C "; //Temperatur Min
    // weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "C "; // Temperatur Max
    weatherString += "Luftfeuchte:  " + String(humidity) + " %    ---    "; // Luftfeuchtigkeit
    weatherString += "Luftdruck:  " + String(pressure) + " hPa    ---    "; // Luftdruck
    weatherString += "Regenwahrscheinlichkeit:   " + String(clouds) + " %    ---    "; // Regenrisiko
    weatherString += "Wind:   " + String(windSpeed*3.6, 0) + " km/h       +++              "; // Windgeschwindigkeit
    // weatherString += String(windDeg) + deg + " "; //Windrichtung in Grad
    // weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Windgeschwindigkeit und Richtung
    Serial.print("Das aktuelle Wetter: - "+FormatedDateTime()); // Weather String
    StatusOpenWeatherMap = "OK";
    Serial.println("Wetterdaten geladen ...");
    Serial.println("Wetterdaten von: "+FormatedDateTime());               
    } else
    {
    Serial.println("keine aktuellen Wetterdaten vorhanden! - " + String(apiGetData)); //error message if no client connect
    Serial.println(); 
    StatusOpenWeatherMap = "Fehler";  
    }
    } else
    {
     Serial.println("ungültiger OpenWeatherMap Request ! - " + String(apiGetData)); //error message if no client connect
     Serial.println();
     StatusOpenWeatherMap = "Fehler"; 
     AccessOpenWeatherMap = false; 
     WeatherFunctions = false;
     weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
     cityID = ""; // City ID von OpenWeatherMap
     Serial.println("OpenWeatherMap wird deaktiviert! ..."); 
     Serial.println("");  
      }  
    } else
    {
    Serial.println("Verbindung zu OpenWeatherMap fehlgeschlagen ! - " + String(apiGetData)); //error message if no client connect
    Serial.println("");  
    StatusOpenWeatherMap = "Fehler"; 
      }
http.end();
wifiClient.flush();
wifiClient.stop();
}
// =======================================================================
