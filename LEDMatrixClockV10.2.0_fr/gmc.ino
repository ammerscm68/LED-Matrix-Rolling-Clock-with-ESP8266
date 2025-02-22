// **********************************(GMC - Radioactivité dans CPM - www.gmcmap.com)  ***********************************
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
Serial.println(F("Téléchargement des données GMCMap - Veuillez Patienter ..."));} 
GMCMapValue = F(""); // Init
getURLBodyParse(GMCMapHhost,GMCMapBody,printToString);
// Date de la première entrée dans l'Histoire à partir du Site Web
if (FindSubString(GMCMapValue, GMCMapHistoryDate()) > 0) {
// Valeur de la Radioactivité en CPM
Len = GMCMapValue.length()-46;
GMCMapValue.remove(0,Len);
GMCMapValue.replace("<td>","");
GMCMapValue.replace("</td>","");
GMCMapValue.trim();
Len = GMCMapValue.length();
GMCMapValue.remove(5,Len);
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeFR(true); // Temps de Connexion

// Testez la Radioactivité en dehors de la Plage normale
// GMCMapValue = F("101");
// GMCMapValue = F("NoGMCMapValue");

if (IsNumeric(GMCMapValue)) {
StatusGMCMap = F("OK"); 
IntGMCMapValue = GMCMapValue.toInt();
if (IntGMCMapValue >= 90) {
GMCMapToHigh = cleanText("        +++       Attention : la Radioactivité à l'emplacement Sélectionné est trop élevée... !!!  --- "+GMCMapValue+" CPM       +++           ");}
MXGMCMapValue = cleanText("            Radioactivité à l'emplacement Sélectionné:           ")+GMCMapValue+" CPM";  // pour l'affichage Matriciel  
WPGMCMapValue = "+++ Radioactivité à l'emplacement Sélectionné: &nbsp;"+GMCMapValue+" CPM +++"; // pour l'affichage de la Page Web
SMXGMCMapValue = "+++ Radioactivité à l'emplacement Sélectionné: "+GMCMapValue+" CPM +++"; // pour la Sortie du Moniteur Série
} else {
StatusGMCMap = F("Error");  
MXGMCMapValue = cleanText("        +++       Aucune Valeur actuelle reçue du Serveur GMCMap !!!       +++           ");  // pour l'affichage Matriciel
WPGMCMapValue = F("+++ Aucune Valeur actuelle reçue du Serveur GMCMap !!! +++"); // pour l'affichage de la Page Web
SMXGMCMapValue = WPGMCMapValue;} // pour la Sortie du Moniteur Série
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
  WiFiClientSecure client; // Client WLAN 
  client.setInsecure(); // Ne pas vérifier la Signature
  client.setTimeout(35000); 
  client.connect(host,443); // Connectez-vous avec l'hôte
while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
if (ConnectCount > 150) {break;}} // courte attente
if (!client.connected()) {FailConnect = true;} else {FailConnect = false;}
if (FailConnect) {     
if (SerialMonitor) {
Serial.println(F("La Connexion au Serveur GMCMap a échoué !"));} // Message d'erreur si le Client n'est pas connecté   
StatusGMCMap = F("Error");
client.stop();
freeRam(); // Defragment Heap 
return "Null";} else {  // Le retour n'est pas nécessaire !
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.connected() == 0) {
    if (millis() - timeout > 35000) {
  if (SerialMonitor) {
  Serial.println(F("La Connexion au Serveur GMCMap a échoué ! - Temps mort"));} // Message d'erreur si le Client n'est pas connecté  
      StatusGMCMap = F("Error"); break;}}
  if (StatusGMCMap != "Error") {    
  lastline = "";
  while(client.connected()){
    String line = client.readStringUntil('\r');
    line.replace("\n","");
    if (line.length()>0) {lastline = line;}
    if (inPage) {g(line);}
    if (line.length()==0) {inPage = true;}
  // Résiliation lors de la recherche d'une chaîne
  if (strstr(lastline.c_str(), "</td>") != NULL) {TagCount++;}
  if (TagCount > 3) {break;}}
  client.stop(); // Clôture du client
  return "Null"; // Le retour n'est pas nécessaire !
  } else {
  client.stop(); // Client close
  freeRam(); // Defragment Heap
  return "Null";}}} // Le retour n'est pas nécessaire !

// =========================================================================================================================

String getURLBodyParse (String host, String url, void (*g)(String l)) {
return getURLParse(host,url,g);}

// =========================================================================================================================

void printToString (String webLine) {
GMCMapValue += webLine;}

// =========================================================================================================================

String GMCMapHistoryDate() {
String FormatedDay = F(""); 
String FormatedMonth = F(""); 
getTimeLocal(); // obtenir l'Heure actuelle  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
return String(ye) + "-" + FormatedMonth + "-" + FormatedDay;}

// =========================================================================================================================

int FindSubString(String str1, String str2)  { // merci à "baaschan" pour cette Fonction
    int found = 0;
    String substr = "";
    int strdiff = str1.length()-str2.length();
    if (strdiff < 0) {return 0;}
    for (int i=0; i <= strdiff; i++){
    substr = str1.substring(i, i+str2.length());
    if (substr.equals(str2)){++found;}}
    return found;}

// =========================================================================================================================

void GMCMapMessage(int HAMR, int AMR) { // HAMR = Répétition du Message d'Alerte élevée ; AMR = Répétition du Message d'Alerte
if (GMCMapToHigh != "") {  // HAMR par défaut = 10 ; AMR par défaut = 5
StatusWarningLED("HIGH");
if (IsPushover) {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}
for (int i=1; i < HAMR; i++){ // Répéter le Message HAMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Laissez la valeur de radioactivité s'exécuter à partir de l'écran
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {StatusWarningLED("LOW");}  
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {MEP=true; PushOverAlert(GMCMapValue); MEP=false;}  
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Afficher la Radioactivité
if (StatusGMCMap == "OK") {MyWaitLoop(10000); // attente Courte (10 secondes - par défaut)
printStringWithShift("          ", ScrollTextTime);} else {
printStringWithShift("          ", ScrollTextTime);  
for (int i=1; i < AMR; i++){ // Répéter le Message AMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Afficher la radioactivité
printStringWithShift("          ", ScrollTextTime);
MyWaitLoop(1000);}}}}  

// =========================================================================================================================

bool GMCMapMidnightAccessDelay(String GDelay) { // GDelay -->  Minute de Minuit  
if (String(h) == "0" && IsNumeric(GDelay)) { // Exemple: 30 --> Donc pas de Téléchargement des données GMCMap actuelles avant 00h30
int GD =GDelay.toInt();  
if (GD > 59) {return true;} else {
if (m >= 0 && m <= GD) {return false;} else {return true;}}
} else {return true;}}

// =========================================================================================================================