// **********************************(GMC - Radioactivité dans CPM - www.gmcmap.com)  ***********************************
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
DEBUG_PRINTLN(F("Téléchargement des données GMCMap - Veuillez Patienter ..."));} 
GMCMapValue.clear(); // Init
getURLBodyParse(GMCMapHhost,GMCMapBody,printToString);
// Date de la première entrée dans l'Histoire à partir du Site Web
if (FindSubString(GMCMapValue, GMCMapHistoryDate()) > 0) {
// Valeur de la Radioactivité en CPM
Len = GMCMapValue.length()-21;
GMCMapValue.remove(0,Len); // Supprimer la date et l'heure
Len = GMCMapValue.length();
int16_t posHeader = GMCMapValue.indexOf("</td>");
GMCMapValue.remove(posHeader,Len); // Supprimer le Reste
GMCMapValue.trim();} else {GMCMapValue = F("NoGMCMapValue");}

GMCMapLastConnectionDateTime = FormatedDateTimeFR(true); // Temps de Connexion

// Testez la Radioactivité en dehors de la Plage normale
// GMCMapValue = F("101");
// GMCMapValue = F("NoGMCMapValue");

if (IsNumeric(GMCMapValue)) {
StatusGMCMap = F("OK"); 
IntGMCMapValue = (int16_t)strtol(GMCMapValue.c_str(), NULL, 10);
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
DEBUG_PRINTLN(SMXGMCMapValue);}}

// =========================================================================================================================

String getURLParse (String host, String url, void (*g)(String l)) {
  boolean inPage;
  inPage = false;
  byte TagCount = 0;
  bool FailConnect = false;
  bool foundToken = false;
  byte ConnectCount = 0;
  WiFiClientSecure client; // Client WLAN 
  client.setInsecure(); // Ne pas vérifier la Signature
  client.setTimeout(25000); // Défaut = 25000 
  client.connect(host,443); // Connectez-vous avec l'hôte
  while (!client.connected()) {ConnectCount += 1;MyWaitLoop(100);
  if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}
  if (ConnectCount > 45) {FailConnect = true; break;}}

if (FailConnect) {     
if (SerialMonitor) {
DEBUG_PRINTLN(F("La Connexion au Serveur GMCMap a échoué !"));} // Message d'erreur si le Client n'est pas connecté   
StatusGMCMap = F("Error");
client.stop();
return "Null";} else {  // Le retour n'est pas nécessaire !
if (SerialMonitor) {DEBUG_PRINTLN("WiFiClient-ConnectCount: "+String(ConnectCount));}
// **************************************** 
String GetData = "https://" + host + url;
// ****************************************
HTTPClient http; 
   http.setRedirectLimit(5); // // nombre maximal de tentatives de connexion client HTTP 
   http.setTimeout(25000);
   http.useHTTP10(true);
   http.begin(client, GetData); // Connecter
   ConnectCount = 0; // Reset Variable
   while (!http.connected()) {ConnectCount += 1;MyWaitLoop(100);if (ConnectCount > 45) {break;}} // attends brièvement
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
  client.stop(); // Clôture du client
  return "Null"; // Le retour n'est pas nécessaire !
  } else {
  http.end();  
  client.stop(); // Client close
  return "Null";}}} // Le retour n'est pas nécessaire !

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
getTimeLocal(); // obtenir l'Heure actuelle  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
return String(ye) + "-" + FormatedMonth + "-" + FormatedDay;}

// =========================================================================================================================

int16_t FindSubString(String str1, String str2)  { // merci à "baaschan" pour cette Fonction
    int16_t found = 0;
    String substr = "";
    int16_t strdiff = str1.length()-str2.length();
    if (strdiff < 0) {return 0;}
    for (int16_t i=0; i <= strdiff; i++){
    substr = str1.substring(i, i+str2.length());
    if (substr.equals(str2)){++found;}}
    return found;}

// =========================================================================================================================

void GMCMapMessage(byte HAMR, byte AMR) { // HAMR = Répétition du Message d'Alerte élevée ; AMR = Répétition du Message d'Alerte
if (GMCMapToHigh != "") {  // HAMR par défaut = 10 ; AMR par défaut = 5
StatusWarningLED("HIGH");
if (IsPushover) {PushOverAlert(GMCMapValue);}
for (byte i=1; i < HAMR; i++){ // Répéter le Message HAMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(GMCMapToHigh.c_str(), ScrollTextTime);
MyWaitLoop(1000);} 
printStringWithShift("          ", ScrollTextTime); // Laissez la valeur de radioactivité s'exécuter à partir de l'écran
} else {if (GMCMapValue == "NoGMCMapValue") {StatusWarningLED("HIGH");} else {
InHumidityStatus = CheckInHumidity();}
if (IsBuzzer && GMCMapValue == "NoGMCMapValue") {GMCMapAlarmBeep();}
if (IsPushover && GMCMapValue == "NoGMCMapValue") {PushOverAlert(GMCMapValue);}  
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Afficher la Radioactivité
if (StatusGMCMap == "OK") {MyWaitLoop(10000); // attente Courte (10 secondes - par défaut)
printStringWithShift("          ", ScrollTextTime);} else {
printStringWithShift("          ", ScrollTextTime);  
for (byte i=1; i < AMR; i++){ // Répéter le Message AMRx
if (IsBuzzer) {GMCMapAlarmBeep();}
printStringWithShift(MXGMCMapValue.c_str(), ScrollTextTime); // Afficher la radioactivité
printStringWithShift("          ", ScrollTextTime);
MyWaitLoop(1000);}}}}  

// =========================================================================================================================

bool GMCMapMidnightAccessDelay(String GDelay) { // GDelay -->  Minute de Minuit 
bool GMCReturn = false;
if (IsNumeric(GDelay)) { // Exemple: 30 --> Donc pas de Téléchargement des données GMCMap actuelles avant 00h30
if (GDelay.substring(0,1) == "0") {GDelay.remove(0, 1);} // supprime le zéro non significatif
int16_t IntGDelay = (int16_t)strtol(GDelay.c_str(), NULL, 10);
if ((IntGDelay > 0) && (IntGDelay <= 59)) {
if ((h == 0) && (m <= IntGDelay)) {GMCReturn = false;} else {GMCReturn = true;}} else {GMCReturn = false;}
} else {GMCReturn = false;}
return GMCReturn;} 

// =========================================================================================================================