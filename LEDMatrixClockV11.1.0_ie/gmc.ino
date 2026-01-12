// **********************************(GMC - Radioactivity in CPM - www.gmcmap.com)  ***********************************
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
DEBUG_PRINTLN(F("Downloading GMCMap Data - Please wait ..."));} 
GMCMapValue.clear(); // Init
getURLBodyParse(GMCMapHhost,GMCMapBody,printToString);
// Date of first Entry in History from Website
if (FindSubString(GMCMapValue, GMCMapHistoryDate()) > 0) {
// Radioactivity Value in CPM
Len = GMCMapValue.length()-21;
GMCMapValue.remove(0,Len); // Remove Date and Time
Len = GMCMapValue.length();
int16_t posHeader = GMCMapValue.indexOf("</td>");
GMCMapValue.remove(posHeader,Len); // Remove remainder
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeEN(true); // Connection Time;

// Test Radioactivity outside the normal Range
// GMCMapValue = F("101");
// GMCMapValue = F("NoGMCMapValue");

if (IsNumeric(GMCMapValue)) {
StatusGMCMap = F("OK"); 
IntGMCMapValue = (int16_t)strtol(GMCMapValue.c_str(), NULL, 10);
if (IntGMCMapValue >= 90) {
GMCMapToHigh = cleanText("        +++       Attention: Radioactivity at the selected Location is too high ... !!!  --- "+GMCMapValue+" CPM       +++           ");}
MXGMCMapValue = cleanText("            Radioactivity at the selected Location:           ")+GMCMapValue+" CPM";  // for the Matrix Display  
WPGMCMapValue = "+++ Radioactivity at the selected Location: &nbsp;"+GMCMapValue+" CPM +++"; // for the WebPage Display
SMXGMCMapValue = "+++ Radioactivity at the selected Location: "+GMCMapValue+" CPM +++"; // for the Serial-Monitor Output
} else {
StatusGMCMap = F("Error");  
MXGMCMapValue = F("        +++       No current Values Received from GMCMap-Server !!!       +++           ");  // for the Matrix Display
WPGMCMapValue = F("+++ No current Values Received from GMCMap-Server !!! +++"); // for the WebPage Display
SMXGMCMapValue = WPGMCMapValue;} // for the Serial-Monitor Output
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
  client.setInsecure(); // Don't check Signature
  client.setTimeout(25000); // Default = 25000
  client.connect(host,443); // Connect with Host
  while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
  if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}
  if (ConnectCount > 45) {FailConnect = true; break;}}

if (FailConnect) {     
if (SerialMonitor) {
DEBUG_PRINTLN(F("Connection to GMCMap Server failed !"));} // Error Message if Client not connected  
StatusGMCMap = F("Error");
client.stop();
return "Null";} else {  // Return is not needed! 
if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}
// **************************************** 
String GetData = "https://" + host + url;
// ****************************************
HTTPClient http; 
   http.setRedirectLimit(5); // Maximum connection attempts HTTP Client
   http.setTimeout(25000);
   http.useHTTP10(true);
   http.begin(client, GetData); // connect
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 45) {break;}} // short wait
    int16_t httpCode = http.GET();
    if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] GET... code: " + HTTPCodeText(httpCode));}
    // Verbindung prüfen
    if(httpCode != HTTP_CODE_OK) {StatusGMCMap = "Error";}

  if (StatusGMCMap != "Error") { 
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
  client.stop(); // Client close
  return "Null"; // Return is not needed!
  } else {
  http.end();  
  client.stop(); // Client close
  return "Null";}}} // Return is not needed!

// =========================================================================================================================

String getURLBodyParse (String host, String url, void (*g)(String l)) {
return getURLParse(host,url,g);}

// =========================================================================================================================

void printToSerial (String webLine) {
DEBUG_PRINTLN("@:"+webLine);}

// =========================================================================================================================

void printToString (String webLine) {
GMCMapValue += webLine;}

// =========================================================================================================================

String GMCMapHistoryDate() {
String FormatedDay; 
String FormatedMonth; 
getTimeLocal(); // get current time  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
return String(ye) + "-" + FormatedMonth + "-" + FormatedDay;}

// =========================================================================================================================

int16_t FindSubString(String str1, String str2)  { // thanks to "baaschan" for this function
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
if (GMCMapToHigh != "") {  // Default HAMR = 10  ; Default AMR = 5
StatusWarningLED("HIGH");
if (IsPushover) {PushOverAlert(GMCMapValue);}
for (byte i=1; i < HAMR; i++){ // Repeat Message HAMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Let the Radioactivity Value run from the Display
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {
InHumidityStatus = CheckInHumidity();} 
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {PushOverAlert(GMCMapValue);}  
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Show Radioactivity
if (StatusGMCMap == "OK") {MyWaitLoop(10000); // short wait (10 sek. - default)
printStringWithShift("          ", ScrollTextTime);} else {
printStringWithShift("          ", ScrollTextTime);  
for (byte i=1; i < AMR; i++){ // Repeat Message AMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Show Radioactivity
printStringWithShift("          ", ScrollTextTime);
MyWaitLoop(1000);}}}}

// =========================================================================================================================

bool GMCMapMidnightAccessDelay(String GDelay) { // GDelay -->  Minute of Midnight 
bool GMCReturn = false;
if (IsNumeric(GDelay)) { // Example: 30 --> So no Download of the current GMCMap Data until 00:30 o'clock
if (GDelay.substring(0,1) == "0") {GDelay.remove(0, 1);} // remove leading zero
int16_t IntGDelay = (int16_t)strtol(GDelay.c_str(), NULL, 10);
if ((IntGDelay > 0) && (IntGDelay <= 59)) {
if ((h == 0) && (m <= IntGDelay)) {GMCReturn = false;} else {GMCReturn = true;}} else {GMCReturn = false;}
} else {GMCReturn = false;}
return GMCReturn;} 

// =========================================================================================================================