void SendPushoverMessage(String POMessage, String PODevice, String POSound){
bool FailConnect;
int ConnectCount = 0;
WiFiClient client;
FailPushOver = false;
client.setTimeout(15000); 
if (SerialMonitor) {Serial.println(F(""));
Serial.println(F("Connect to the PushOver.net Server..."));}
client.connect("api.pushover.net", 80);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100); if (ConnectCount > 150) {FailPushOver = true; break;}} // short Wait
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) {
HTTPClient http;
http.setRedirectLimit(5); // Maximum Connection attempts HTTP client
http.setTimeout(30000);
http.useHTTP10(false);  
String postmessage = "token="+SecureAppToken+"&user="+UserToken+"&title=Report from: "+ProgramName+"&message="+POMessage+"&device="+PODevice+"&sound="+POSound;  
http.begin(client,"http://api.pushover.net/1/messages.json");
ConnectCount = 0; // Reset Variable
while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {FailPushOver = true; break;}} // kurz warten
if (SerialMonitor) {
Serial.println(F(""));
Serial.println("[Pushover] - Send Message: "+POMessage); Serial.println(F(""));}
int httpCode = http.POST(postmessage);
if (SerialMonitor) {Serial.println("[HTTP] POST... code: " + HTTPCodeText(httpCode));}
if(httpCode == HTTP_CODE_OK) {
if (SerialMonitor) {Serial.println(F("[Pushover] - The Message was sent successfully"));}} else {  
if (SerialMonitor) {Serial.println(F("[Pushover] - The Message could not be sent !"));
Serial.println(F(""));} FailPushOver = true;}
http.end();
client.flush();
client.stop();
freeRam(); // Defragment Heap
} else {
client.flush();
client.stop();
freeRam(); // Defragment Heap
if (SerialMonitor) {Serial.println(F("[Pushover] - Connection to Service failed !"));
Serial.println(F("")); FailPushOver = true;}}
}

// =========================================================================================================================

void PushOverAlert(String IHS) {
String MPushOverMessage; int NIHS;
MPushOverMessage = F("");
if (IsNumeric(IHS)) {NIHS = IHS.toInt();} else {NIHS = -1;}
if (IHS == "MIN" || IHS == "MAX" || IHS == "NoGMCMapValue" || NIHS >= 0) {  
if (IHS == "MIN") {   
MPushOverMessage = F("Attention: The Humidity at the Location of the Watch is too low ... !");}
if (IHS == "MAX") { 
MPushOverMessage = F("Attention: The Humidity at the Location of the Watch is too high ... !");} 
if (IHS == "NoGMCMapValue" ) { // no Data !!!
MPushOverMessage = "No current Values Received from the GMCMap-Server !!!";}
if (NIHS >= 90 ) { // Danger >= 90 CPM  !!!
MPushOverMessage = "Attention: Radioactivity at the selected Location is too high ... !!!  ---  "+String(NIHS)+" CPM";}

if (PushoverDevice1 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Send Pushover Message to "+PushoverDevice1);}
if (NIHS >= 90 ) {
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice1,"siren"); // Send Message to Device 1
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice1+" could not be sent!");} 
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(MPushOverMessage,PushoverDevice1,"none"); // Send Message to Device 1
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice1+" could not be sent!");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice2 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Send Pushover Message to "+PushoverDevice2);}
if (NIHS >= 90 ) {
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice2,"siren"); // Send Message to Device 2
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice2+" could not be sent!");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(MPushOverMessage,PushoverDevice2,"none"); // Send Message to Device 2 
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice2+" could not be sent!");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice3 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Send Pushover Message to "+PushoverDevice3);}
if (NIHS >= 90 ) {
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice3,"siren"); // Send Message to Device 3
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice3+" could not be sent!");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage(MPushOverMessage,PushoverDevice3,"none"); // Send Message to Device 3
if (SerialMonitor && FailPushOver) {
Serial.println("The Pushover Message to "+PushoverDevice3+" could not be sent!");}
printStringWithShift("          ", ScrollTextTime);}}  
}} 

// =========================================================================================================================

void CheckPushOver() {
SecureAppToken.trim(); UserToken.trim(); PushoverDevice1.trim(); PushoverDevice2.trim();
PushoverDevice3.trim(); PushoverMessageSound.trim(); PushoverMessageSound.toLowerCase(); 
if (PushoverMessageSound == "") {PushoverMessageSound = "none";} // Pushover Message Sound
if (PushoverMessageSound != "pushover" && PushoverMessageSound != "bike" &&  PushoverMessageSound != "bugle" &&
    PushoverMessageSound != "cashregister" && PushoverMessageSound != "classical" && PushoverMessageSound != "cosmic" &&
    PushoverMessageSound != "falling" && PushoverMessageSound != "gamelan" && PushoverMessageSound != "incoming" &&
    PushoverMessageSound != "intermission" && PushoverMessageSound != "magic" && PushoverMessageSound != "mechanical" &&
    PushoverMessageSound != "pianobar" && PushoverMessageSound != "siren" && PushoverMessageSound != "spacealarm" &&
    PushoverMessageSound != "tugboat" && PushoverMessageSound != "alien" && PushoverMessageSound != "climb" &&
    PushoverMessageSound != "persistent" && PushoverMessageSound != "echo" && PushoverMessageSound != "updown" &&
    PushoverMessageSound != "none") {PushoverMessageSound = F("none");} 
}

// =========================================================================================================================

void TMPushoverDevice1() {
String WSS = WiFiSignalStrength(true); // WiFi Signal Strength Reading
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("The WiFi Signal Strength is too low !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;
PushOverTest = true;
if (SerialMonitor) {Serial.println("Send Pushover Test Message to: "+PushoverDevice1);} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Test Message for: "+PushoverDevice1,PushoverDevice1,PushoverMessageSound); // Message on Device 1 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD1");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice2() {
String WSS = WiFiSignalStrength(true); // WiFi Signal Strength Reading
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("The WiFi Signal Strength is too low !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;
PushOverTest = true;
if (SerialMonitor) {Serial.println("Send Pushover Test Message to: "+PushoverDevice2);} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Test Message for: "+PushoverDevice2,PushoverDevice2,PushoverMessageSound); // Message on Device 2 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD2");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice3() {
String WSS = WiFiSignalStrength(true); // WiFi Signal Strength Reading
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("The WiFi Signal Strength is too low !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;
PushOverTest = true;
if (SerialMonitor) {Serial.println("Send Pushover Test Message to: "+PushoverDevice3);} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Test Message for: "+PushoverDevice3,PushoverDevice3,PushoverMessageSound); // Message on Device 3 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD3");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================
