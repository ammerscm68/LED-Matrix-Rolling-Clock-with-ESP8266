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
delay(10); //waits for data
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
const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13);
DynamicJsonBuffer jsonBuffer(capacity);
JsonObject &root = jsonBuffer.parseObject(jsonArray);
if (!root.success())
{
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
//weatherMain = root["weather"]["main"].as<String>();
weatherMain = root["weather"]["main"].as<String>();
weatherDescription = root["weather"]["description"].as<String>();
weatherDescription.toLowerCase();
weatherLocation = root["name"].as<String>();
// country = root["sys"]["country"].as<String>();
country = root["sys"]["country"].as<String>();
temp = root["main"]["temp"];
humidity = root["main"]["humidity"];
pressure = root["main"]["pressure"];
tempMin = root["main"]["temp_min"];
tempMax = root["main"]["temp_max"];
windSpeed = root["wind"]["speed"];
windDeg = root["wind"]["deg"];
clouds = root["clouds"]["all"];

// String windDirection = "N NOO SOS SWW NW"; // Windrichtungen N NO O SO S SW W NW immer 2 char lang
// int wr = (windDeg+22)%360/45;
// Serial.println(wr);
// Serial.println(wr);
String deg = String(char('~' + 25)); // ° Grad Zeichen
ATemp = "     +++      Aussentemperatur:        " + String(temp, 0) + deg + "C  "; // nur aktuelle Temperatur
weatherString = "      +++        Das aktuelle Wetter in " + String(weatherLocation) + ":    ---    "; //Ortsname
//weatherString += String(country); // Länderkennung
weatherString += "Aussentemperatur: " + String(temp, 0) + deg + "C    ---    "; //Temperatur
// weatherString += " TempMin.: " + String(tempMin, 1) + " " + deg + "C "; //Temperatur Min
// weatherString += " TempMax.: " + String(tempMax, 1) + " " + deg + "C "; // Temperatur Max
weatherString += "Luftfeuchte: " + String(humidity) + " %    ---    "; // Luftfeuchtigkeit
weatherString += "Luftdruck: " + String(pressure) + " hPa    ---    "; // Luftdruck
weatherString += "Regenrisiko: " + String(clouds) + " %    ---    "; // Regenrisiko
weatherString += "Wind: " + String(windSpeed*3.6, 0) + " km/h       +++              "; // Windgeschwindigkeit
// weatherString += String(windDeg) + deg + " "; //Windrichtung in Grad
// weatherString += arrowUp + " " + windDirection.substring(2wr,2wr+2) + " "; // Windgeschwindigkeit und Richtung
// Serial.println(weatherString); //Leh
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
