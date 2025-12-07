// **********************************(GMC - Radioaktivität in CPM - www.gmcmap.com)  ***********************************
void getGMCMapData() {
unsigned int Len = 0;
String SMXGMCMapValue;  
WPGMCMapValue = F("");
MXGMCMapValue = F("");
GMCMapValue = F("");
GMCMapToHigh = F("");
int16_t IntGMCMapValue = -1;
WiFiClient client;
String GMCMapHhost = F("www.gmcmap.com");  
String GMCMapBody = "/historyData.asp?Param_ID="+GMCMapHistoryID+"&systemTimeZone="+GMCMapGMTOffset; 
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("GMCMap-Daten werden heruntergeladen - Bitte warten ..."));} 
GMCMapValue = F(""); // Init
getURLBodyParse(GMCMapHhost,GMCMapBody,printToString);
// Datum des Eintrags in der History
if (FindSubString(GMCMapValue, GMCMapHistoryDate()) > 0) {
// Radioaktivitäswert in CPM
Len = GMCMapValue.length()-21;
GMCMapValue.remove(0,Len); // Datum und Uhrzeit entfernen
Len = GMCMapValue.length();
int16_t posHeader = GMCMapValue.indexOf("</td>");
GMCMapValue.remove(posHeader,Len); // Rest entfernen
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeDE(true); // Connection Time;

// Test Radioaktivität außerhalb des normalen Bereiches
/*GMCMapValue = F("101");
GMCMapValue = F("NoGMCMapValue");*/

if (IsNumeric(GMCMapValue)) {
StatusGMCMap = F("OK");
IntGMCMapValue = (int16_t)strtol(GMCMapValue.c_str(), NULL, 10);
if (IntGMCMapValue >= 90) {
GMCMapToHigh = cleanText("        +++       Achtung:  Radioaktivität am gewählten Standort ist zu hoch ... !!!  --- "+GMCMapValue+" CPM       +++           ");}
MXGMCMapValue = cleanText("            Radioaktivität am gewählten Standort:           ")+GMCMapValue+" CPM";  // für die Matrix Anzeige  
WPGMCMapValue = "+++ Radioaktivität am gewählten Standort: &nbsp;"+GMCMapValue+" CPM +++"; // Für die WebPage Anzeige
SMXGMCMapValue = "+++ Radioaktivität am gewählten Standort: "+GMCMapValue+" CPM +++"; // Für die Serial-Monitor Ausgabe
} else {
StatusGMCMap = F("Fehler");  
MXGMCMapValue = F("        +++       Keine aktuellen Werte vom GMCMap-Server empfangen !!!       +++           ");  // für die Matrixanzeige
WPGMCMapValue = F("+++ Keine aktuellen Werte vom GMCMap-Server empfangen !!! +++"); // Für die WebPage Anzeige
SMXGMCMapValue = WPGMCMapValue;} // Für die Serial-Monitor Ausgabe
if (SerialMonitor) {
DEBUG_PRINTLN(SMXGMCMapValue);}}

// =========================================================================================================================

String getURLParse (String host, String url, void (*g)(String l)) {
  String lastline;
  boolean inPage;
  inPage = false;
  byte TagCount = 0;
  bool FailConnect = false;
  byte ConnectCount = 0;
  WiFiClientSecure client; // WLAN Client  
  client.setInsecure(); // Signatur nicht prüfen
  client.setTimeout(35000); 
  client.connect(host,443); // mit Host Verbinden
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // kurz warten
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (FailConnect) {     
if (SerialMonitor) {
DEBUG_PRINTLN(F("Verbindung zum GMCMap-Server fehlgeschlagen !"));} // Fehlermeldung wenn Client nicht verbunden  
StatusGMCMap = F("Fehler");
client.stop();
return "Null";} else {  // Return wird nicht gebraucht !   
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.connected() == 0) {
    if (millis() - timeout > 35000) {
  if (SerialMonitor) {
  DEBUG_PRINTLN(F("Verbindung zum GMCMap-Server fehlgeschlagen ! - TimeOut"));} // Fehlermeldung wenn Client nicht verbunden  
      StatusGMCMap = F("Fehler"); break;}}
  if (StatusGMCMap != "Fehler") {    
  lastline = "";
  while(client.connected()){
    String line = client.readStringUntil('\r');
    line.replace("\n","");
    line.trim();
    int16_t posHeader = line.indexOf("<td>");
    if (posHeader != 0) {line = "";}
    if (line.length()>0) {lastline = line;}
    if (inPage) {g(line);}
    if (line.length()==0) {inPage = true;}
  // Abbruch bei Find String
  if (strstr(lastline.c_str(), "</td>") != NULL) {TagCount++;}
  if (TagCount > 3) {break;}}
  client.stop(); // Client beenden
  return "Null"; // Return wird nicht gebraucht !
  } else {
  client.stop(); // Client beenden
  return "Null";}}} // Return wird nicht gebraucht !
   
// =========================================================================================================================

String getURLBodyParse (String host, String url, void (*g)(String l)) {
return getURLParse(host,url,g);}

// =========================================================================================================================

void printToString (String webLine) {
GMCMapValue += webLine;}

// =========================================================================================================================

String GMCMapHistoryDate() {
String FormatedDay; 
String FormatedMonth; 
getTimeLocal(); // aktuelle Zeit holen
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
return String(ye) + "-" + FormatedMonth + "-" + FormatedDay;}

// =========================================================================================================================

int16_t FindSubString(String str1, String str2)  { // Danke an "baaschan" für diese Funktion
    int16_t found = 0;
    String substr = "";
    int16_t strdiff = str1.length()-str2.length();
    if (strdiff < 0) {return 0;}
    for (int16_t i=0; i <= strdiff; i++){
    substr = str1.substring(i, i+str2.length());
    if (substr.equals(str2)){++found;}}
    return found;}

// =========================================================================================================================  

void GMCMapMessage(byte HAMR, byte AMR) { // HAMR = High Alert Message Repeat ; AMR = Alert Message Repeat 
if (GMCMapToHigh != "") {  // Default HAMR = 10  ; Default AMR = 5  Maximum = 255
StatusWarningLED("HIGH");
if (IsPushover) {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}
for (byte i=1; i < HAMR; i++){ // Nachricht HAMRx wiederholen
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Radioaktivitätswert aus dem Display laufen lassen
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {StatusWarningLED("LOW");}
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}  
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Radioaktivität anzeigen
if (StatusGMCMap == "OK") {MyWaitLoop(10000); // kurz warten (10 sek. - default)
printStringWithShift("          ", ScrollTextTime);} else {
printStringWithShift("          ", ScrollTextTime);  
for (byte i=1; i < AMR; i++){ // Nachricht AMRx wiederholen
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Radioaktivität anzeigen
printStringWithShift("          ", ScrollTextTime);
MyWaitLoop(1000);}}}}

// =========================================================================================================================

bool GMCMapMidnightAccessDelay(String GDelay) { // GDelay -->  Minute der Mitternachtsstunde  
if (String(h) == "0" && IsNumeric(GDelay)) {    // Beispiel: 30 --> Also bis 00:30 kein Donload der aktuellen GMCMap-Daten
int16_t GD = (int16_t)strtol(GDelay.c_str(), NULL, 10); 
if (GD > 59) {return true;} else {
if (m >= 0 && m <= GD) {return false;} else {return true;}}
} else {return true;}}

// =========================================================================================================================
