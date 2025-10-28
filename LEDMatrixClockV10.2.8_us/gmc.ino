// **********************************(GMC - Radioactivity in CPM - www.gmcmap.com)  ***********************************
void getGMCMapData() {
unsigned int Len = 0;
String SMXGMCMapValue = F("");  
WPGMCMapValue = F("");
MXGMCMapValue = F("");
GMCMapValue = F("");
GMCMapToHigh = F("");
int IntGMCMapValue = -1;
WiFiClient client;
String GMCMapHhost = F("www.gmcmap.com");  
String GMCMapBody = "/historyData.asp?Param_ID="+GMCMapHistoryID+"&systemTimeZone="+GMCMapGMTOffset; 
if (SerialMonitor) {
Serial.println(F(""));
Serial.println(F("Downloading GMCMap Data - Please wait ..."));} 
GMCMapValue = F(""); // Init
getURLBodyParse(GMCMapHhost,GMCMapBody,printToString);
// Date of first Entry in History from Website
if (FindSubString(GMCMapValue, GMCMapHistoryDate()) > 0) {
// Radioactivity Value in CPM
Len = GMCMapValue.length()-21;
GMCMapValue.remove(0,Len); // Remove Date and Time
Len = GMCMapValue.length();
int posHeader = GMCMapValue.indexOf("</td>");
GMCMapValue.remove(posHeader,Len); // Remove remainder
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeUS(true); // Connection Time;

// Test Radioactivity outside the normal Range
// GMCMapValue = F("101");
// GMCMapValue = F("NoGMCMapValue");

if (IsNumeric(GMCMapValue)) {
StatusGMCMap = F("OK"); 
IntGMCMapValue = GMCMapValue.toInt();
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
Serial.println(SMXGMCMapValue);}}

// =========================================================================================================================

String getURLParse (String host, String url, void (*g)(String l)) {
  String lastline;
  boolean inPage;
  inPage = false;
  int TagCount = 0;
  bool FailConnect = false;
  int ConnectCount = 0;
  WiFiClientSecure client; // WLAN Client  
  client.setInsecure(); // Don't check Signature
  client.setTimeout(35000); 
  client.connect(host,443); // Connect with Host
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // short wait
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (FailConnect) {     
if (SerialMonitor) {
Serial.println(F("Connection to GMCMap Server failed !"));} // Error Message if Client not connected  
StatusGMCMap = F("Error");
client.stop();
freeRam(); // Defragment Heap 
return "Null";} else {  // Return is not needed!  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.connected() == 0) {
    if (millis() - timeout > 35000) {
  if (SerialMonitor) {
  Serial.println(F("Connection to GMCMap Server failed ! - TimeOut"));} // Error Message if Client not connected  
      StatusGMCMap = F("Error"); break;}}
  if (StatusGMCMap != "Error") {    
  lastline = "";
  while(client.connected()){
    String line = client.readStringUntil('\r');
    line.replace("\n","");
    line.trim();
    int posHeader = line.indexOf("<td>");
    if (posHeader != 0) {line = "";}
    if (line.length()>0) {lastline = line;}
    if (inPage) {g(line);}
    if (line.length()==0) {inPage = true;}
  // Termination on Find String
  if (strstr(lastline.c_str(), "</td>") != NULL) {TagCount++;}
  if (TagCount > 3) {break;}}
  client.stop(); // Client close
  return "Null"; // Return is not needed!
  } else {
  client.stop(); // Client close
  freeRam(); // Defragment Heap
  return "Null";}}} // Return is not needed!

// =========================================================================================================================

String getURLBodyParse (String host, String url, void (*g)(String l)) {
return getURLParse(host,url,g);
}

// =========================================================================================================================

void printToSerial (String webLine) {
Serial.println("@:"+webLine);
}

// =========================================================================================================================

void printToString (String webLine) {
GMCMapValue += webLine;
}

// =========================================================================================================================

String GMCMapHistoryDate() {
String FormatedDay = F(""); 
String FormatedMonth = F(""); 
getTimeLocal(); // get current time  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
return String(ye) + "-" + FormatedMonth + "-" + FormatedDay;}

// =========================================================================================================================

int FindSubString(String str1, String str2)  { // thanks to "baaschan" for this function
    int found = 0;
    String substr = "";
    int strdiff = str1.length()-str2.length();
    if (strdiff < 0) {return 0;}
    for (int i=0; i <= strdiff; i++){
    substr = str1.substring(i, i+str2.length());
    if (substr.equals(str2)){++found;}}
    return found;}

// =========================================================================================================================  

void GMCMapMessage(int HAMR, int AMR) { // HAMR = High Alert Message Repeat ; AMR = Alert Message Repeat 
if (GMCMapToHigh != "") {  // Default HAMR = 10  ; Default AMR = 5
StatusWarningLED("HIGH");
if (IsPushover) {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}
for (int i=1; i < HAMR; i++){ // Repeat Message HAMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Let the Radioactivity Value run from the Display
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {StatusWarningLED("LOW");} 
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}  
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Show Radioactivity
if (StatusGMCMap == "OK") {MyWaitLoop(10000); // short wait (10 sek. - default)
printStringWithShift("          ", ScrollTextTime);} else {
printStringWithShift("          ", ScrollTextTime);  
for (int i=1; i < AMR; i++){ // Repeat Message AMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Show Radioactivity
printStringWithShift("          ", ScrollTextTime);
MyWaitLoop(1000);}}}}

// =========================================================================================================================

bool GMCMapMidnightAccessDelay(String GDelay) { // GDelay -->  Minute of Midnight  
if (String(h) == "0" && IsNumeric(GDelay)) { // Example: 30 --> So no Download of the current GMCMap Data until 00:30 o'clock
int GD =GDelay.toInt();  
if (GD > 59) {return true;} else {
if (m >= 0 && m <= GD) {return false;} else {return true;}}
} else {return true;}}

// =========================================================================================================================