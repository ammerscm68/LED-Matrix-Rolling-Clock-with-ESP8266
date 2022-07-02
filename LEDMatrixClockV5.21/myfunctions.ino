// ****************************Hilfsfunktionen******************************************************
void clearscreen() { 
for(int i=0; i<10; i++) {
Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
}

// *************************************************************************************************

void LoadMatrixAdjustments(){
// *** Einstellungen laden .... ***

// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    delay(250); //waits for data

if (littleFSInit == true)
{
bool MDTime; // Hilfsvariable
  Serial.println("Load Clock Adjustments ...");
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  lssid = SetupLoad("ssid.txt");} else {lssid = "";}
  lssid.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/ssid-passwd.txt")){
  lpassword = SetupLoad("ssid-passwd.txt");} else {lpassword = "";}
  lpassword.trim(); // Leerzeichen vorne und Hinten entfernen
  ssid = lssid.c_str();
  password = lpassword.c_str();
  // *** Zugangsdaten OpenWeatherMap laden ***
  // OpenWeatherMap API-Key als String
  if (LittleFS.exists("/owm-apikey.txt")){
  weatherKey = SetupLoad("owm-apikey.txt");} else {weatherKey = "";}
  weatherKey.trim(); // Leerzeichen vorne und Hinten entfernen
  // OpenWeatherMap City-ID als String
  if (LittleFS.exists("/owm-cityid.txt")){
  cityID = SetupLoad("owm-cityid.txt");} else {cityID = "";} 
  cityID.trim(); // Leerzeichen vorne und Hinten entfernen
  weatherKey.toCharArray(OWMapiKey, 40);  // API-Key
  cityID.toCharArray(OWMCityID, 10);  // City-ID
  // Display Einschaltzeit als String
  if (LittleFS.exists("/display-on-time.txt")){
  DisplayOnTime = SetupLoad("display-on-time.txt");} else {DisplayOnTime = "";}
  DisplayOnTime.trim(); // Leerzeichen vorne und Hinten entfernen
  // Display Ausschaltzeit als String
  if (LittleFS.exists("/display-off-time.txt")){
  DisplayOffTime = SetupLoad("display-off-time.txt");} else {DisplayOffTime = "";} 
  DisplayOffTime.trim(); // Leerzeichen vorne und Hinten entfernen

   // Display On/Off Time auf Plausibilität prüfen
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
  if (DisplayOnTime != "") {
  if (strstr(DisplayOnTime.c_str(), ":") == NULL) {DisplayOnTime = "";};
  if (DisplayOnTime != "") {
  if (DisplayOnTime.substring(0,1) == "0") {DisplayOnTime.remove(0, 1);}
  int DP = DisplayOnTime.indexOf(':')+1;
  if (DisplayOnTime.substring(DP,DP+1) == "0") {DisplayOnTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
if (DisplayOnTime != "") {
MDTime = false;
int DP = DisplayOnTime.indexOf(':'); 
String CheckMDHourOn = DisplayOnTime.substring(0,DP); 
String CheckMDMinOn =  DisplayOnTime.substring(DP+1,6);
CheckMDMinOn.trim();
for (int i=0; i <= 23; i++)
  {
    if (String(i) ==  CheckMDHourOn) {MDTime = true; break;}     
    } 
if (!MDTime) {DisplayOnTime = "";} else {   
MDTime = false;    
  for (int i=0; i <= 59; i++)
  {
if (String(i) ==  CheckMDMinOn) {MDTime = true; break;}  
    } 
if (!MDTime) {DisplayOnTime = "";}    
}
}
  }
  }
  if (DisplayOffTime != "") {
  if (strstr(DisplayOffTime.c_str(), ":") == NULL) {DisplayOffTime = "";};
  if (DisplayOffTime != "") {
  if (DisplayOffTime.substring(0,1) == "0") {DisplayOffTime.remove(0, 1);}
  int DP = DisplayOffTime.indexOf(':')+1;
  if (DisplayOffTime.substring(DP,DP+1) == "0") {DisplayOffTime.remove(DP, 1);}
  if (DisplayOnTime.length()  > 5 || DisplayOnTime.length()  < 3) {DisplayOnTime = "";}
  if (DisplayOffTime.length()  > 5 || DisplayOffTime.length()  < 3) {DisplayOffTime = "";}
if (DisplayOffTime != "") {
MDTime = false;
int DP = DisplayOffTime.indexOf(':'); 
String CheckMDHourOff = DisplayOffTime.substring(0,DP); 
String CheckMDMinOff=  DisplayOffTime.substring(DP+1,6);
CheckMDMinOff.trim(); 
for (int i=0; i <= 23; i++)
  {
    if (String(i) ==  CheckMDHourOff) {MDTime = true; break;}     
    } 
if (!MDTime) {DisplayOffTime = "";} else {   
MDTime = false;    
  for (int i=0; i <= 59; i++)
  {
if (String(i) ==  CheckMDMinOff) {MDTime = true; break;}  
    } 
if (!MDTime) {DisplayOffTime = "";}    
}
}
  }
  }
  
  // Serial.println("DisplayOnTime: "+DisplayOnTime); // nur für Debug Modus
  // Serial.println("DisplayOffTime: "+DisplayOffTime); // nur für Debug Modus

  // Helligkeit der LED-Matrix
  Brightness = SetupLoad("matrix_brightness.txt");
  if (Brightness == "" || Brightness == "L1") {Brightness = "L1"; sendCmdAll(CMD_INTENSITY, 0);} // bei leerer Datei oder Fehler Stufe 1
  if (Brightness == "L2") {sendCmdAll(CMD_INTENSITY, 1);} // Stufe 2
  if (Brightness == "L3") {sendCmdAll(CMD_INTENSITY, 2);} // Stufe 3
  if (Brightness == "L4") {sendCmdAll(CMD_INTENSITY, 3);} // Stufe 4
  if (Brightness == "L5") {sendCmdAll(CMD_INTENSITY, 4);} // Stufe 5
  // Doppelpunktstatus
  DPS = SetupLoad("status_doublepoint.txt");
  if (DPS == "" || DPS == "1") {DoublePointStatus = 1;} // bei leerer Datei oder Fehler
  if (DPS == "0") {DoublePointStatus = 0;}
  if (DPS == "2") {DoublePointStatus = 2;}
  // ScrollText Geschwindigkeit
  STT = SetupLoad("status_scrolltexttime.txt");
  if (STT == "25") {ScrollTextTime = 25;} // Stufe 8
  if (STT == "30") {ScrollTextTime = 30;} // Stufe 7
  if (STT == "35") {ScrollTextTime = 35;} // Stufe 6
  if (STT == "" || STT == "40") {ScrollTextTime = 40;} // Stufe 5
  if (STT == "45") {ScrollTextTime = 45;} // Stufe 4
  if (STT == "50") {ScrollTextTime = 50;} // Stufe 3
  if (STT == "55") {ScrollTextTime = 55;} // Stufe 2
  if (STT == "60") {ScrollTextTime = 60;} // Stufe 1
  
  // Zusatzfunktionen EIN/AUS
  EF = SetupLoad("status_enhancedfunctions.txt");
  if (EF == "" || EF == "On") {EnhancedFunctions = true;}
  if (EF == "Off") {EnhancedFunctions = false;}

  // Wetterdatenfunktion EIN/AUS
  if (LittleFS.exists("/status_weatherfunctions.txt")){
  OWMF = SetupLoad("status_weatherfunctions.txt");
  OWMF.trim();
  if (OWMF == "" || OWMF == "On") {WeatherFunctions = true;}
  if (OWMF == "Off") {WeatherFunctions = false;}} else {WeatherFunctions = true;}

   if (LittleFS.exists("/newsapi-apikey.txt")){
   newsKey = SetupLoad("newsapi-apikey.txt");
   newsKey.trim();} // https://newsapi.org

  // Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  // Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OpenweatherMap] API-Key: " + weatherKey); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  // Serial.println("[Loaded-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  // Serial.println("[Loaded-MatrixDisplay] OnTime: " + DisplayOnTime);  // Only for Debuging  
  // Serial.println("[Loaded-MatrixDisplay] OffTime: " + DisplayOffTime);  // Only for Debuging 
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    Brightness = "L1";
    sendCmdAll(CMD_INTENSITY, 0);
    DoublePointStatus = 1;
    ScrollTextTime = 40;
    EnhancedFunctions = true;
    WeatherFunctions = false;
    weatherKey = ""; // API-Key von OpenWeatherMap - https://openweathermap.org
    cityID = ""; // City ID von OpenWeatherMap
    newsKey = ""; // API-Key von NewsAPI - https://newsapi.org
    }  
}

// *************************************************************************************************

void SaveMatrixAdjustments(){
Serial.println("Save Clock Adjustments ...");  
// Serial.println("[Save-WIFI] SSID: " + (String)wifiManager.getWiFiSSID());  // Only for Debuging 
  // Serial.println("[Save-WIFI] PASS: " + (String)wifiManager.getWiFiPass());   // Only for Debuging 
  // Serial.println("[Save-OpenWeatherMap] API-Key: " + (String)OWMapiKey);  // Only for Debuging 
  // Serial.println("[Save-WIFI-OpenweatherMap] City-ID: " + cityID); // Only for Debuging 
  // Serial.println("[Save-WIFI-NewsAPI] API-Key: " + newsKey); // Only for Debuging 
  // Serial.println("[Save-MatrixDisplay] OnTime: " + (String)MDOnTime);  // Only for Debuging  
  // Serial.println("[Save-MatrixDisplay] OffTime: " + (String)MDOffTime);  // Only for Debuging 
   
  // SSID speichern
  SetupSave("ssid.txt", (String)wifiManager.getWiFiSSID()); // SSID WLAN
  SetupSave("ssid-passwd.txt", (String)wifiManager.getWiFiPass()); // Password WLAN
  if ((String)OWMapiKey != "" && (String)OWMCityID != "") {
  SetupSave("owm-apikey.txt", (String)OWMapiKey); // OpenWeatherMap API-Key
  SetupSave("owm-cityid.txt", (String)OWMCityID);} // OpenWeatherMap City-ID
  if ((String)NAPIKey != "") {
  SetupSave("newsapi-apikey.txt", (String)NAPIKey);} // NewsAPI API-Key
  if ((String)MDOnTime != ""){
  SetupSave("display-on-time.txt", (String)MDOnTime);} // Matrix Display Einschaltzeit
  if ((String)MDOffTime != ""){
  SetupSave("display-off-time.txt", (String)MDOffTime);} // Matrix Display Ausschaltzeit

  MyWaitLoop(500); // 0,5 sek. warten 
  Serial.println("Zugangsdaten gespeichert... "); 
  printStringWithShift("        +++       Zugangsdaten gespeichert...      +++           ",ScrollTextTime);  
}

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
String result = ""; // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return result;
}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // failed to open the file, return false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return true;
}

// *************************************************************************************************

// Display per Taster Ein- und Ausschalten
void MatrixButtonOnOff(){
if(digitalRead(TasterPin) == LOW){
MyWaitLoop(500); //wait
Display=!Display;
if (Display == 0 && ClockWiFiConnectFail == false){
sendCmdAll(CMD_SHUTDOWN,0); // Display ausschalten
Serial.println("Matrix per Taster um "+ FormatedDateTime() + " ausgeschaltet."); 
}
else
    {
    if (ClockWiFiConnectFail == false){  
    getTimeLocal(); // aktuelle Uhrzeit holen 
    sendCmdAll(CMD_SHUTDOWN, 1); // Display einschalten
    sendCmdAll(CMD_INTENSITY, 0); // LED-Matrix Helligkeit --> 0= Minimum
    Serial.println("Matrix per Taster um "+ FormatedDateTime() + " eingeschaltet.");} else
    {
    sendCmdAll(CMD_SHUTDOWN, 1); // nur Display einschalten
    printCharWithShift('.',5);
    }
    // Nach 10x Ausschalten den "Access Point" aktivieren
    APStart += 1; // inc  
    // Serial.println("Taster Count: "+String(APStart));
    if (APStart == 10 && littleFSInit == true) {
    ResetClock();} // Uhr Zurücksetzen 
    }
    delay(250); //wait
   }
}

// *************************************************************************************************

String FormatedDateTime()
{
String DateTimeString;
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10)
    {
    DateTimeString = "0"+String(h);  
    }
    else 
    {
    DateTimeString = String(h);  
    }
    if (m < 10)
    {
    DateTimeString += ":0"+String(m);  
    }
    else
    {
    DateTimeString += ":"+String(m);     
    }
    if (s < 10)
    {
    DateTimeString += ":0"+String(s)+" Uhr / ";  
    }
    else
    {
    DateTimeString += ":"+String(s)+" Uhr / ";     
    }
// Datum
if (d < 10)
    {
    DateTimeString += "0"+String(d);  
    }
    else 
    {
    DateTimeString += String(d);  
    }
    if (mo < 10)
    {
    DateTimeString += ".0"+String(mo)+"."+String(ye);  
    }
    else
    {
    DateTimeString += "."+String(mo)+"."+String(ye);     
    }
return DateTimeString;    
}

// *************************************************************************************************

// Webserverabfrage nur jede 0,5 sek.
void HandleWebServer()
{ 
if (millis() - hwsTime > 500) 
 { 
 hwsTime = millis();
 server.handleClient();  // Webserver Abfrage
 delay(1); // kurze Pause 
 } 
}

// *************************************************************************************************

void wificonnect() 
{
WiFiConnectLoop = 0; // Variable zurücksetzen  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
Serial.print("."); 
if (ProgramSetup == false && Display == 1) {printCharWithShift2('.',1);}
MatrixButtonOnOff(); // Taster abfragen
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED)
{ClockWiFiConnectFail = true;
clr(); // Display Clear
refreshAll(); 
APStart = 0; // "Access Point" Button Counter zurücksetzen
} else
{
ClockWiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("");
Serial.print("Verbunden mit IP-Adresse: "); Serial.println(WiFi.localIP());
}
}

// *************************************************************************************************

void WiFiUDPStart()
{ 
Udp.begin(localPort);
Serial.print("lokaler Port: ");
Serial.println(Udp.localPort());
Serial.println("NTP-Time - Warten auf die Synchronisation");
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
Serial.println("UDP Server gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WiFiUDPStop()
{
Udp.stop();
Serial.println("UDP Server gestoppt - "+FormatedDateTime());   
}

// *************************************************************************************************

void WebServerStart()
{
server.begin();
Serial.println("Webserver gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WebServerStop()
{
server.stop();
Serial.println("Webserver gestoppt - "+FormatedDateTime()); 
}

// *************************************************************************************************

void MyWaitLoop(int wlt)
{// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) // Pause in Millisekunden
{ 
waitloop += 1; // inc waitloop 1
delay(1);
yield(); // Equivalent zu  Application.ProcessMessages; 
// Serial.println(String(waitloop));
}
}

// *************************************************************************************************

void ResetAPCounter()
{
String ResetValue[] = {"0","2", "4", "6", "8", "10", "12", "14" ,"16", "18", 
                       "20", "22", "24", "26", "28", "30", "32", "34", "36",
                       "38", "40", "42", "44", "46", "48", "50", "52", "54",
                       "56", "58"}; 
for (int i=0; i <= 30; i++){                      
if  (String(m)+":"+String(s) == ResetValue[i]+":2")
{ 
// Serial.println("Access Point Button Reset");
APStart = 0; // "Access Point" Button Counter alle 2 Minuten zurücksetzen
} 
}    
}

// *************************************************************************************************

void Download()
{
if (ProgramSetup == true) {  
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
printCharWithShift(dl,ScrollTextTime-10);
printCharWithShift(' ',ScrollTextTime-10);
} 
}

// *************************************************************************************************

void DownloadComplet()
{
if (ProgramSetup == true) { 
// Anzeige löschen    
clr(); 
refreshAll();
}
}

// *************************************************************************************************

void ResetClock()
{
APStart = 0; // zurücksetzen
bool IsFileExist = false; // zurücksetzen
// Little-Filesystem formatieren
LittleFS.format();
wifiManager.resetSettings(); // alle Einstellungen zurücksetzen
// Wenn OpenWeatherMap aktiv - die Zugangsdaten wieder speichern
if (weatherKey != "" && cityID != "") {
SetupSave("owm-apikey.txt", weatherKey); 
SetupSave("owm-cityid.txt", cityID);}
else
{
// Deitei'n löschen wenn vorhanden
IsFileExist = LittleFS.exists("/owm-apikey.txt");
if (IsFileExist){LittleFS.remove("/owm-apikey.txt");}  
IsFileExist = LittleFS.exists("/owm-cityid.txt");
if (IsFileExist){LittleFS.remove("/owm-cityid.txt");}  
}
// NewsAPI API-Key
if (newsKey != "") {
SetupSave("newsapi-apikey.txt", (String)NAPIKey);} else 
{
IsFileExist = LittleFS.exists("/newsapi-apikey.txt");
if (IsFileExist){LittleFS.remove("/newsapi-apikey.txt");}   
}
if (DisplayOnTime != "") { 
// Matrix Display Einschaltzeit  
SetupSave("display-on-time.txt", DisplayOnTime);} else 
{
IsFileExist = LittleFS.exists("/display-on-time.txt");
if (IsFileExist){LittleFS.remove("/display-on-time.txt");}    
}
if (DisplayOffTime != "") { 
// Matrix Display Ausschaltzeit  
SetupSave("display-off-time.txt", DisplayOffTime);} else  
{
IsFileExist = LittleFS.exists("/display-off-time.txt");
if (IsFileExist){LittleFS.remove("/display-off-time.txt");}   
} 
MyWaitLoop(500); // 0,5 sek. warten 
Serial.println("Reset - Uhr wird neu gestartet");
printStringWithShift("        +++     Reset - Uhr wird neu gestartet      +++           ",ScrollTextTime);
ESP.restart();   
}

// *************************************************************************************************

/*void TelegramMessages(){
if (BOTtoken != "" && BOTname != "" && BOTusername != "") { 
bot.getUpdates(bot.message[0][1]);   
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++){
// bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
Serial.println(bot.message[i][4]+ "   "+bot.message[i][5]);
  }
// bot.message[0][0] = "";   // All messages have been replied - reset new messages  
}
}*/

// *************************************************************************************************

void greatings()
{
if  ((String(d) + "." + String(mo)) == "1.1") // Neujahrsgruß
{
GreetingsToday = true;
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Alles gute im neuen Jahr");
printStringWithShift("       +++     Alles gute im neuen Jahr     +++                ", ScrollTextTime);
}
/*if  ((String(d) + "." + String(mo)) == "18.4") // Geburtstagsgruß
{
GreetingsToday = true;
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Alles gute zum Geburtstag liebe Rosi");
printStringWithShift("       +++     Alles gute zum Geburtstag liebe Rosi     +++                ", ScrollTextTime);
}
if  ((String(d) + "." + String(mo)) == "10.7") // Geburtstagsgruß
{
GreetingsToday = true;
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Alles gute zum Geburtstag lieber Klaus");
printStringWithShift("       +++     Alles gute zum Geburtstag lieber Klaus     +++                ", ScrollTextTime); 
}*/
if  ((String(d) + "." + String(mo)) == "25.12" || (String(d) + "." + String(mo)) == "26.12") // Weihnachtsgruß
{
GreetingsToday = true;  
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Frohe Weihnachten");
printStringWithShift("       +++     Frohe Weihnachten     +++                ", ScrollTextTime); 
}
if  ((String(d) + "." + String(mo)) == "31.12") // Silvester
{
GreetingsToday = true;
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Guten Rutsch ins neue Jahr");
printStringWithShift("       +++     Guten Rutsch ins neue Jahr     +++                ", ScrollTextTime);
}
}
// *************************************************************************************************
// *************************************************************************************************
