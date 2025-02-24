void SendPushoverMessage(String POMessage, String PODevice, String POSound){
bool FailConnect;
int ConnectCount = 0;
WiFiClient client;
FailPushOver = false;
client.setTimeout(15000); 
if (SerialMonitor) {Serial.println(F(""));
Serial.println(F("Verbindung zum  PushOver.net - Server herstellen..."));}
client.connect("api.pushover.net", 80);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100); if (ConnectCount > 150) {FailPushOver = true; break;}} // kurz warten
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) {
HTTPClient http;
http.setRedirectLimit(5); // maximale Verbindungsversuche HTTP Client 
http.setTimeout(30000);
http.useHTTP10(false);  
String postmessage = "token="+SecureAppToken+"&user="+UserToken+"&title=Meldung von: "+ProgramName+"&message="+POMessage+"&device="+PODevice+"&sound="+POSound;  
http.begin(client,"http://api.pushover.net/1/messages.json");
ConnectCount = 0; // Reset Variable
while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {FailPushOver = true; break;}} // kurz warten
if (SerialMonitor) {
Serial.println(F(""));
Serial.println("[Pushover] - Sende Nachricht: "+POMessage); Serial.println(F(""));}
int httpCode = http.POST(postmessage);
if (SerialMonitor) {Serial.println("[HTTP] POST... code: " + HTTPCodeText(httpCode));}
if(httpCode == HTTP_CODE_OK) {
if (SerialMonitor) {Serial.println(F("[Pushover] - Die Nachricht wurde erfolgreich gesendet"));}} else {  
if (SerialMonitor) {Serial.println(F("[Pushover] - Die Nachricht konnte nicht gesendet werden !"));
Serial.println(F(""));} FailPushOver = true;}
http.end();
client.stop();
} else {
client.stop();
freeRam(); // Defragment Heap
if (SerialMonitor) {Serial.println(F("[Pushover] - Verbindung zum Dienst fehlgeschlagen !"));
Serial.println(F("")); FailPushOver = true;}}
}

// =========================================================================================================================

void PushOverAlert(String IHS) {
String MPushOverMessage; int NIHS;
MPushOverMessage = F("");
if (IsNumeric(IHS)) {NIHS = IHS.toInt();} else {NIHS = -1;}
if (IHS == "MIN" || IHS == "MAX" || IHS == "NoGMCMapValue" || NIHS >= 0) {  
if (IHS == "MIN") {   
MPushOverMessage = F("Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu niedrig ... !");}
if (IHS == "MAX") { 
MPushOverMessage = F("Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu hoch ... !");} 
if (IHS == "NoGMCMapValue" ) { // keine Daten !!!
MPushOverMessage = "Keine aktuellen Werte vom GMCMap-Server empfangen !!!";}
if (NIHS >= 90 ) { // Gefahr bei >= 90 CPM  !!!
MPushOverMessage = "Achtung:  Radioaktivität am gewählten Standort ist zu hoch ... !!!  ---  "+String(NIHS)+" CPM";}

if (PushoverDevice1 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice1+" senden");}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);  
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice1,"siren"); // Nachricht an Gerät 1 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice1+" konnte nicht gesendet werden !");} 
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(MPushOverMessage,PushoverDevice1,"none"); // Nachricht an Gerät 1 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice1+" konnte nicht gesendet werden !");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice2 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice2+" senden");}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);  
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice2,"siren"); // Nachricht an Gerät 2 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice2+" konnte nicht gesendet werden !");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(MPushOverMessage,PushoverDevice2,"none"); // Nachricht an Gerät 2 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice2+" konnte nicht gesendet werden !");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice3 != "" && MPushOverMessage != ""){
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice3+" senden");}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);  
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(MPushOverMessage,PushoverDevice3,"siren"); // Nachricht an Gerät 3 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice3+" konnte nicht gesendet werden !");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage(MPushOverMessage,PushoverDevice3,"none"); // Nachricht an Gerät 3 senden
if (SerialMonitor && FailPushOver) {
Serial.println("Die Pushover Nachricht an "+PushoverDevice3+" konnte nicht gesendet werden !");}
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
String WSS = WiFiSignalStrength(true); // WiFi Signalstärke auslesen
if (WSS == "schlecht" || WSS == "sehr schlecht") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("Die WiFi Signalstärke ist zu niedrig !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true; 
PushOverTest = true;   
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice1+" senden ...");} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Testnachricht für: "+PushoverDevice1,PushoverDevice1,PushoverMessageSound); // Nachricht an Gerät 1 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD1");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice2() { 
String WSS = WiFiSignalStrength(true); // WiFi Signalstärke auslesen
if (WSS == "schlecht" || WSS == "sehr schlecht") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("Die WiFi Signalstärke ist zu niedrig !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;  
PushOverTest = true;  
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice2+" senden ...");} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Testnachricht für: "+PushoverDevice2,PushoverDevice2,PushoverMessageSound); // Nachricht an Gerät 2 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD2");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice3() { 
String WSS = WiFiSignalStrength(true); // WiFi Signalstärke auslesen
if (WSS == "schlecht" || WSS == "sehr schlecht") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {Serial.println("Die WiFi Signalstärke ist zu niedrig !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;  
PushOverTest = true;  
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice3+" senden ...");} 
printStringWithShift("             P-Over", ScrollTextTime); 
SendPushoverMessage("Testnachricht für: "+PushoverDevice3,PushoverDevice3,PushoverMessageSound); // Nachricht an Gerät 3 
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD3");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================
