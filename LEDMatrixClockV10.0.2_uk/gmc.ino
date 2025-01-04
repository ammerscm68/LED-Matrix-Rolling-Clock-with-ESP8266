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
Len = GMCMapValue.length()-46;
GMCMapValue.remove(0,Len);
GMCMapValue.replace("<td>","");
GMCMapValue.replace("</td>","");
GMCMapValue.trim();
Len = GMCMapValue.length();
GMCMapValue.remove(5,Len);
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeEN(true); // Connection Time;

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
  WiFiClientSecure client; // WLAN Client  
  client.setInsecure(); // Don't check Signature
  client.setTimeout(30000); 
  if (!client.connect(host,443)) {
  if (SerialMonitor) {
  Serial.println(F("Connection to GMCMap Server failed !"));} // Error Message if Client not connected  
    StatusGMCMap = "Error";  
    return "ERR:noconnect";}
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 45000) {
        client.flush();
        client.stop();
        freeRam(); // Defragment Heap
  if (SerialMonitor) {
  Serial.println(F("Connection to GMCMap Server failed ! - TimeOut"));} // Error Message if Client not connected
      StatusGMCMap = F("Error");    
      return "ERR:timeout";}}
  lastline = "";
  while(client.available()){
    String line = client.readStringUntil('\r');
    line.replace("\n","");
    if (line.length()>0) {
      lastline = line;}
    if (inPage) {g(line);}
    if (line.length()==0) {inPage = true;}
  // Cancel when find String
  if (strstr(lastline.c_str(), "</td>") != NULL) {TagCount++;}
  if (TagCount > 3) {break;}}
  client.flush();
  client.stop(); // Client Close
  freeRam(); // Defragment Heap
  return lastline;
}

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
    if (strdiff < 0) {
      return 0;
    }
    for (int i=0; i <= strdiff; i++){
      substr = str1.substring(i, i+str2.length());
      if (substr.equals(str2)){
        ++found;
      }      
    }
    return found;
  }

// =========================================================================================================================

void GMCMapMessage(int HAMR, int AMR) { // HAMR = High Alert Message Repeat ; AMR = Alert Message Repeat 
if (GMCMapToHigh != "") {  // Default HAMR = 10  ; Default AMR = 5
StatusWarningLED("HIGH");
if (IsPushover) {PushOverAlert(GMCMapValue);}
for (int i=1; i < HAMR; i++){ // Repeat Message HAMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Let the Radioactivity Value run from the Display
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {StatusWarningLED("LOW");}  
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {PushOverAlert(GMCMapValue);}  
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
// =========================================================================================================================
