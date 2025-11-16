void SendPushoverMessage(String POMessage, String PODevice, String POSound){
bool FailConnect;
int ConnectCount = 0;
WiFiClient client;
FailPushOver = false;
client.setTimeout(15000); 
if (SerialMonitor) {DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN(F("Connectez-vous au Serveur PushOver.net..."));}
client.connect("api.pushover.net", 80);
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100); if (ConnectCount > 150) {FailPushOver = true; break;}} // short Wait
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}  
if (!FailConnect) {
HTTPClient http;
http.setRedirectLimit(5); // Nombre maximal de tentatives de Connexion client HTTP
http.setTimeout(30000);
http.useHTTP10(false);  
String postmessage = "token="+SecureAppToken+"&user="+UserToken+"&title=Report from: "+ProgramName+"&message="+POMessage+"&device="+PODevice+"&sound="+POSound;  
http.begin(client,"http://api.pushover.net/1/messages.json");
ConnectCount = 0; // Réinitialiser la Variable
while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 150) {FailPushOver = true; break;}} // courte attente
if (SerialMonitor) {
DEBUG_PRINTLN(F(""));
DEBUG_PRINTLN("[Pushover] - Envoyer un Message: "+POMessage); DEBUG_PRINTLN(F(""));}
int httpCode = http.POST(postmessage);
if (SerialMonitor) {DEBUG_PRINTLN("[HTTP] POST... code: " + HTTPCodeText(httpCode));}
if(httpCode == HTTP_CODE_OK) {
if (SerialMonitor) {DEBUG_PRINTLN(F("[Pushover] - Le Message a été envoyé avec Succès"));}} else {
if (SerialMonitor) {DEBUG_PRINTLN(F("[Pushover] - Le Message n'a pas pu être Envoyé !"));
DEBUG_PRINTLN(F(""));} FailPushOver = true;}
http.end();
client.stop();
} else {
client.stop();
freeRam(); // Defragment Heap
if (SerialMonitor) {DEBUG_PRINTLN(F("[Pushover] - La Connexion au Service a échoué !"));
DEBUG_PRINTLN(F(""));}}
}

// =========================================================================================================================

void PushOverAlert(String IHS) {
String MPushOverMessage; int NIHS;
MPushOverMessage = F("");
if (IsNumeric(IHS)) {NIHS = IHS.toInt();} else {NIHS = -1;}
if (IHS == "MIN" || IHS == "MAX" || IHS == "NoGMCMapValue" || NIHS >= 0) {  
if (IHS == "MIN") {   
MPushOverMessage = F("Attention : L'Humidité à l'emplacement de l'Horloge est trop faible... !");}
if (IHS == "MAX") { 
MPushOverMessage = F("Attention : L'Humidité à l'emplacement de l'Horloge est trop élevée... !");}  
if (IHS == "NoGMCMapValue" ) { // pas de données !!!
MPushOverMessage = "Aucune valeur actuelle reçue du Serveur GMCMap !!!";}
if (NIHS >= 90 ) { // Danger >= 90 CPM  !!!
MPushOverMessage = "Attention : La radioactivité à l'emplacement sélectionné est trop élevée... !!!  ---  "+String(NIHS)+" CPM";}

if (PushoverDevice1 != "" && MPushOverMessage != ""){
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message Pushover à "+PushoverDevice1);}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);    
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice1,"siren"); // Envoyer un message à l'appareil 1
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice1+" n'a pas pu être envoyé !");} 
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice1,"none"); // Envoyer un message à l'appareil 1
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice1+" n'a pas pu être envoyé !");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice2 != "" && MPushOverMessage != ""){
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message Pushover à "+PushoverDevice2);}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);    
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice2,"siren"); // Envoyer un message à l'appareil 2
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice2+" n'a pas pu être envoyé !");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime);      
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice2,"none"); // Envoyer un message à l'appareil 2 
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice2+" n'a pas pu être envoyé !");}
printStringWithShift("             ", ScrollTextTime);}}   
if (PushoverDevice3 != "" && MPushOverMessage != ""){
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message Pushover à "+PushoverDevice3);}
if (NIHS >= 90 ) {
printStringWithShift("             ", ScrollTextTime);    
printStringWithShift("P-Over", ScrollTextTime);   
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice3,"siren"); // Envoyer un message à l'appareil 3
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice3+" n'a pas pu être envoyé !");}
printStringWithShift("             ", ScrollTextTime);} else {
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage(cleanText(MPushOverMessage),PushoverDevice3,"none"); // Envoyer un message à l'appareil 3
if (SerialMonitor && FailPushOver) {
DEBUG_PRINTLN("Le Message Pushover à "+PushoverDevice3+" n'a pas pu être envoyé !");}
printStringWithShift("             ", ScrollTextTime);}}  
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
String WSS = WiFiSignalStrength(true); // Lecture de la force du Signal WiFi
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {DEBUG_PRINTLN("La Force du Signal WiFi est trop faible !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true; 
PushOverTest = true;   
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message de Test Pushover à: "+PushoverDevice1);} 
printStringWithShift("             ", ScrollTextTime);
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage("Message de Test pour: "+PushoverDevice1,PushoverDevice1,PushoverMessageSound); // Message sur l'appareil 1
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD1");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice2() {
String WSS = WiFiSignalStrength(true); // Lecture de la force du Signal WiFi
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {DEBUG_PRINTLN("La Force du Signal WiFi est trop faible !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true;    
PushOverTest = true;
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message de Test Pushover à: "+PushoverDevice2);} 
printStringWithShift("             ", ScrollTextTime);
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage("Message de Test pour: "+PushoverDevice2,PushoverDevice2,PushoverMessageSound); // Message sur l'appareil 2
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD2");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================

void TMPushoverDevice3() {
String WSS = WiFiSignalStrength(true); // Lecture de la force du Signal WiFi
if (WSS == "bad" || WSS == "very bad") {HTMLAnswer("FPO"); MyWaitLoop(500);
if (SerialMonitor) {DEBUG_PRINTLN("La Force du Signal WiFi est trop faible !");}} else {  
if (!MEP) {  
if (!await){ 
await = true;
MEP = true; 
PushOverTest = true;   
if (SerialMonitor) {DEBUG_PRINTLN("Envoyer un Message de Test Pushover à: "+PushoverDevice3);} 
printStringWithShift("             ", ScrollTextTime);
printStringWithShift("P-Over", ScrollTextTime); 
SendPushoverMessage("Message de Test pour: "+PushoverDevice3,PushoverDevice3,PushoverMessageSound); // Message sur l'appareil 3
if (FailPushOver) {HTMLAnswer("FPO");} else {HTMLAnswer("POD3");}
HandleOTAandWebServer(); printStringWithShift("                 ", ScrollTextTime); HandleOTAandWebServer();
HandleOTAandWebServer(); ClockScrollIn(); HandleOTAandWebServer();
PushOverTest = false;
MEP = false;
await = false;} else {HandleOTAandWebServer();}} else {HandleOTAandWebServer();}}}

// =========================================================================================================================