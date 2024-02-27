void SendPushoverMessage(String POMessage, String PODevice, String POSound){
bool FailConnect;
if (SerialMonitor) {
Serial.println(F(""));
Serial.println("[Pushover] - Sende Nachricht: "+POMessage);}
WiFiClient client;
HTTPClient http;
MyWaitLoop(100);
if (!client.connect("api.pushover.net", 80)) {FailConnect = true;} else {FailConnect = false;}
if (!FailConnect) {
String postmessage = "token="+SecureAppToken+"&user="+UserToken+"&title=Meldung von: "+ProgramName+"&message="+POMessage+"&device="+PODevice+"&sound="+POSound;  
http.begin(client,"http://api.pushover.net/1/messages.json");
MyWaitLoop(500); //wait
int httpCode = http.POST(postmessage);
if (SerialMonitor) {Serial.println("[HTTP] POST... code: " + HTTPCodeText(httpCode));}
if(httpCode == HTTP_CODE_OK) {
if (SerialMonitor) {Serial.println(F("[Pushover] - Die Nachricht wurde erfolgreich gesendet"));}} else {
if (SerialMonitor) {Serial.println(F("[Pushover] - Die Nachricht konnte nicht gesendet werden !"));
Serial.println(F(""));}}
http.end();
client.flush();
client.stop();
} else {
client.flush();
client.stop();
if (SerialMonitor) {Serial.println(F("[Pushover] - Verbindung zum Dienst fehlgeschlagen !"));
Serial.println(F(""));}}
}

// =========================================================================================================================

void PushOverAlert(String IHS) {
String MPushOverMessage;  
if (IHS == "MIN" || IHS == "MAX") {  
if (IHS == "MIN") {   
MPushOverMessage = F("Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu niedrig ... !");}
if (IHS == "MAX") { 
MPushOverMessage = F("Achtung:  Die Luftfeuchtigkeit am Standort der Uhr ist zu hoch ... !");}     
if (PushoverDevice1 != ""){
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice1+" senden");}    
SendPushoverMessage(MPushOverMessage,PushoverDevice1,"none");}   // Nachricht an Gerät 1 senden
if (PushoverDevice2 != ""){ 
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice2+" senden");}   
SendPushoverMessage(MPushOverMessage,PushoverDevice2,"none");}   // Nachricht an Gerät 2 senden
if (PushoverDevice3 != ""){
if (SerialMonitor) {Serial.println("Pushover Nachricht an "+PushoverDevice3+" senden");}    
SendPushoverMessage(MPushOverMessage,PushoverDevice3,"none");}}  // Nachricht an Gerät 3 senden
} 

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
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice1+" senden ...");}  
SendPushoverMessage("Testnachricht für "+PushoverDevice1,PushoverDevice1,PushoverMessageSound); // Nachricht an Gerät 1 
HTMLAnswer("TMPushoverDevice1");  
}

// =========================================================================================================================

void TMPushoverDevice2() { 
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice2+" senden ...");}  
SendPushoverMessage("Testnachricht für "+PushoverDevice2,PushoverDevice2,PushoverMessageSound); // Nachricht an Gerät 2 
HTMLAnswer("TMPushoverDevice2");  
}

// =========================================================================================================================

void TMPushoverDevice3() { 
if (SerialMonitor) {Serial.println("Pushover Testnachricht an "+PushoverDevice3+" senden ...");}  
SendPushoverMessage("Testnachricht für "+PushoverDevice3,PushoverDevice3,PushoverMessageSound); // Nachricht an Gerät 3 
HTMLAnswer("TMPushoverDevice3");  
}

// =========================================================================================================================
