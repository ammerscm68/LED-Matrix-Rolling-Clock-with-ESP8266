// **********************************(openweathermap.org)***********************************
const char *weatherHost = "api.openweathermap.org";
String result = "";
void getWeatherData()
{
Serial.println("Verbinde mit OpenweatherMap ...");
// Serial.println(weatherHost);
if (client.connect(weatherHost, 80)) 
{
client.println("GET /data/2.5/weather?id="+cityID+"&units=metric&APPID="+weatherKey);
client.println("Host: api.openweathermap.org");
client.println("User-Agent: ArduinoWiFi/1.1");
client.println("Connection: close");
client.println();

OWwaitloop = 0; // Variable zurücksetzen
WeatherFail = false; // Variable zurücksetzen
while(client.connected() && !client.available())
{
MyWaitLoop(10); //waits for data
yield(); // Equivalent zu  Application.ProcessMessages; 
OWwaitloop += 1; // inc waitloop 1
if (OWwaitloop >= MaxWaitLoop) 
{
WeatherFail = true; 
Serial.println("OpenWeatherMap - Zeitüberschreitung!"); 
break; 
}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("OpenWeatherMap Connect-Loop: "+String(OWwaitloop)+"/"+String(MaxWaitLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if (WeatherFail == false)
{
Serial.println("Mit OpenWeatherMap verbunden - Bitte warten ...");  
while (client.connected() || client.available())
{ //connected or data available  
result = client.readStringUntil('\r');
delay(1); //waits for data
yield(); // Equivalent zu  Application.ProcessMessages; 
}

client.stop(); // Client anhalten

// Daten auswerten
result.replace('[', ' ');
result.replace(']', ' ');
// Serial.println(result);
char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';
// StaticJsonDocument<1024> doc;
DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512); // for ESP8266
// DynamicJsonDocument doc(ESP.getMaxAllocHeap()); for ESP32
DeserializationError error = deserializeJson(doc, jsonArray);
if (error){
WeatherFail = true;
StatusOpenWeatherMap = "Fehler";
return;
}
else
{
WeatherFail = false;  
}

if (WeatherFail == false)
{  
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

// String windDirection = "N NOO SOS SWW NW"; // Windrichtungen N NO O SO S SW W NW immer 2 char lang
// int wr = (windDeg+22)%360/45;
// Serial.println(wr);
// Serial.println(wr);

// Sonderzeichen und Umlaute
String deg = String(char('~' + 25)); // ° Grad Zeichen
String uue = String(char('~' + 26)); // ü Zeichen
String uoe = String(char('~' + 27)); // ö Zeichen
String uae = String(char('~' + 28)); // ä Zeichen
String uss = String(char('~' + 29)); // ß Zeichen

ATemp = "     +++      Au"+uss+"entemperatur:        " + String(temp, 0) + deg + "C  "; // nur aktuelle Temperatur
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
// Serial.println(weatherString); // Weather String
StatusOpenWeatherMap = "OK";
Serial.println("Wetterdaten geladen ...");
Serial.println("Wetterdaten von: "+FormatedDateTime());
}
else {
Serial.println("OpenWeatherMap - Parsing der Daten fehlgeschlagen! - "+FormatedDateTime());
Serial.println();
StatusOpenWeatherMap = "Fehler";
client.stop();
}
}
else
{
Serial.println("OpenWeatherMap - Verbindung fehlgeschlagen! - "+FormatedDateTime());
Serial.println();
StatusOpenWeatherMap = "Fehler";
client.stop();  
}
}
}
// =======================================================================
