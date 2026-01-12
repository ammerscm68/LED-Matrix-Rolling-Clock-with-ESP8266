// **********************************(GMC - Radioaktivität in CPM - www.gmcmap.com)  ***********************************
void getGMCMapData() {
unsigned int Len = 0;
String SMXGMCMapValue;  
WPGMCMapValue.clear();
MXGMCMapValue.clear();
GMCMapValue.clear();
GMCMapToHigh.clear();
int16_t IntGMCMapValue = -1;
WiFiClient client;
String GMCMapHhost = F("www.gmcmap.com");  
String GMCMapBody = "/historyData.asp?Param_ID="+GMCMapHistoryID+"&systemTimeZone="+String(utco); 
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("GMCMap-Daten werden heruntergeladen - Bitte warten ..."));} 
GMCMapValue.clear(); // Init
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
  boolean inPage;
  inPage = false;
  byte TagCount = 0;
  bool FailConnect = false;
  bool foundToken = false;
  byte ConnectCount = 0;
  WiFiClientSecure client; // WLAN Client  
  client.setInsecure(); // Signatur nicht prüfen
  client.setTimeout(25000); // Default = 25000 
  client.connect(host,443); // mit Host Verbinden
  while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
  if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}
  if (ConnectCount > 45) {FailConnect = true; break;}} // kurz warten

if (FailConnect) {     
if (SerialMonitor) {
DEBUG_PRINTLN(F("Verbindung zum GMCMap-Server fehlgeschlagen !"));} // Fehlermeldung wenn Client nicht verbunden  
StatusGMCMap = F("Fehler");
client.stop();
return "Null";} else {  // Return wird nicht gebraucht ! 
if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}  
// **************************************** 
String GetData = "https://" + host + url;
// ****************************************
HTTPClient http; 
   http.setRedirectLimit(5); // maximale Verbindungsversuche HTTP Client 
   http.setTimeout(25000);
   http.useHTTP10(true);
   http.begin(client, GetData); // Verbinden
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 45) {break;}} // kurz warten
    int16_t httpCode = http.GET();
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    // Verbindung prüfen
    if(httpCode != HTTP_CODE_OK) {StatusGMCMap = "Fehler";}

  if (StatusGMCMap != "Fehler") { 
  WiFiClient& stream = http.getStream();     
  String line; line.reserve(40);
  String lastline; lastline.reserve(40);
  line.clear(); lastline.clear();
  while(stream.available()){ 
    line = stream.readStringUntil('\n');
    line.replace(F("\n"),""); line.trim();
    if (line.length()== 0) {line.clear();}
    if (line.indexOf("<tr>") != -1) {foundToken = true;}
    if (foundToken) {
    if (SerialMonitor) {DEBUG_PRINTLN(line);}
    int16_t posHeader = line.indexOf("<td>");
    if (posHeader != 0) {line.clear();}
    if (line.length()>0) {lastline = line;}
    if (inPage) {g(line);}
    if (line.length()==0) {inPage = true;}
  // Abbruch bei Find String
  if (strstr(lastline.c_str(), "</td>") != NULL) {TagCount++;}
  if (TagCount > 3) {break;}} else {line.clear();}}
  http.end();
  client.stop(); // Client beenden
  return "Null"; // Return wird nicht gebraucht !
  } else {
  http.end(); 
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
if (IsPushover) {PushOverAlert(GMCMapValue);}
for (byte i=1; i < HAMR; i++){ // Nachricht HAMRx wiederholen
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Radioaktivitätswert aus dem Display laufen lassen
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {
InHumidityStatus = CheckInHumidity();}
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {PushOverAlert(GMCMapValue);}  
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
bool GMCReturn = false;
if (IsNumeric(GDelay)) { // Beispiel: 30 --> Also von 00:00 bis 00:30 kein Download der aktuellen GMCMap-Daten
if (GDelay.substring(0,1) == "0") {GDelay.remove(0, 1);} // führende Null entfernen
int16_t IntGDelay = (int16_t)strtol(GDelay.c_str(), NULL, 10);
if ((IntGDelay > 0) && (IntGDelay <= 59)) {
if ((h == 0) && (m <= IntGDelay)) {GMCReturn = false;} else {GMCReturn = true;}} else {GMCReturn = false;}
} else {GMCReturn = false;}
return GMCReturn;} 

// =========================================================================================================================
