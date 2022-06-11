// ****************************Hilfsfunktionen******************************************************
void clearscreen() { 
for(int i=0; i<10; i++) {
Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
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
    APStart = 0; // zurücksetzen
    // Little-Filesystem formatieren
    LittleFS.format();
    wifiManager.resetSettings(); // alle Einstellungen zurücksetzen
    // Wenn OpenWeatherMap aktiv - die Zugangsdaten wieder speichern
    if (weatherKey != "" && cityID != "") {
    SetupSave("owm-apikey.txt", weatherKey); 
    SetupSave("owm-cityid.txt", cityID);
    if (DisplayOnTime != "") { 
    SetupSave("display-on-time.txt", DisplayOnTime);} // Matrix Display Einschaltzeit
    if (DisplayOffTime != "") { 
    SetupSave("display-off-time.txt", DisplayOffTime);} // Matrix Display Ausschaltzeit
    MyWaitLoop(500);} // 0,5 sek. warten 
    Serial.println("Reset - Uhr wird neu gestartet");
    printStringWithShift("        +++     Reset - Uhr wird neu gestartet      +++           ",ScrollTextTime);
    ESP.restart();  
    }  
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
{ClockWiFiConnectFail = false;}
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

void greatings()
{
if  ((String(d) + "." + String(mo)) == "1.1") // Neujahrsgruß
{
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Alles gute im neuen Jahr");
printStringWithShift("       +++     Alles gute im neuen Jahr     +++                ", ScrollTextTime);
}
if  ((String(d) + "." + String(mo)) == "25.12" || (String(d) + "." + String(mo)) == "26.12") // Weihnachtsgruß
{
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Frohe Weihnachten");
printStringWithShift("       +++     Frohe Weihnachten     +++                ", ScrollTextTime); 
}
if  ((String(d) + "." + String(mo)) == "31.12") // Silvester
{
// Serial.println(String(h)+":"+String(m)+":" + String(s)+" --- Guten Rutsch ins neue Jahr");
printStringWithShift("       +++     Guten Rutsch ins neue Jahr     +++                ", ScrollTextTime);
}
}
// *************************************************************************************************
// *************************************************************************************************
