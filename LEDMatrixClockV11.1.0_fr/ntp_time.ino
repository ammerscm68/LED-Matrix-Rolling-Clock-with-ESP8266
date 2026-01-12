/*-------- Code du Serveur NTP ----------*/

// -------------------------------- Fuseau Horaire. ----------------------------------- //
// Détails des Paramètres de fuseau Horaire https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time (Berlin, Frankfurt, Wien, Paris, Madrid) | UTC + 2 hours
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time (Berlin, Frankfurt, Wien, Paris, Madrid) | UTC + 1 hours
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast, Dublin)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time UTC + 1 hours
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time UTC 0 hours
Timezone UK(BST, GMT);

// UTC
/*TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);*/

// US Eastern Time Zone (New York, Detroit, Miami, Lima)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston, Mexico City)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300}; // Daylight UTC -5 hours
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360}; //  Standardtime UTC -6 hours
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City, Calgary)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360}; // Daylight UTC -6 hours
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420}; //  Standardtime UTC -7 hours
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);

// US Pacific Time Zone (Las Vegas, Los Angeles, Seattle)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420}; // Daylight UTC -7 hours
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};  // Standardtime UTC -8 hours
Timezone usPT(usPDT, usPST);

// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Canada Mountain Time Zone 
// (Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary)
TimeChangeRule caMDT = {"MDT", Second, Sun, Mar, 2, -360}; // UTC -6 Hours
TimeChangeRule caMST = {"MST", First, Sun, Nov, 2, -420};  // UTC -7 Hours
Timezone caMT(caMDT, caMST);

// Canada Pacific Time Zone (Vancouver)
TimeChangeRule caPDT = {"PDT", Second, Sun, Mar, 2, -420}; // UTC -7 Hours
TimeChangeRule caPST = {"PST", First, Sun, Nov, 2, -480}; // UTC -8 Hours
Timezone caPT(caPDT, caPST);

// Canada Central Time Zone (Winnipeg, Baker Lake, Kenora, Regina, Creighton)
TimeChangeRule caCDT = {"CDT", Second, Sun, Mar, 2, -300}; // UTC -5 Hours
TimeChangeRule caCST = {"CST", First, Sun, Nov, 2, -360}; // UTC -6 Hours
Timezone caCT(caCDT, caCST);

// Canada Atlantic Central Time Zone 
// (Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules )
TimeChangeRule caADT = {"ADT", Second, Sun, Mar, 2, -180}; // UTC -3 Hours
TimeChangeRule caAST = {"AST", First, Sun, Nov, 2, -240}; // UTC -4 Hours
Timezone caACT(caADT, caAST);

// Canada Eastern Time Zone (Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa)
TimeChangeRule caEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule caEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone caET(caEDT, caEST);

// Canada Whitehorse Standard Time (MST, does not observe DST)
TimeChangeRule mst = {"MST", Last, Sun, Mar, 1, -420}; // UTC -7 Hours
Timezone tzMST(mst);

// =============================================================================================================================

const int NTP_PACKET_SIZE = 48; // Heure NTP dans les 48 premiers octets du message
byte packetBuffer[NTP_PACKET_SIZE]; // Tampon pour les paquets entrants et sortants

time_t getNtpTime() {
  IPAddress ntpServerIP; // Adresse IP du serveur NTP
  while (Udp.parsePacket() > 0) ; // rejette tous les paquets reçus précédemment
  // obtenir un serveur aléatoire du pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  if (SerialMonitor) {
  DEBUG_PRINT(ntpServerName);
  DEBUG_PRINT(F(": "));
  DEBUG_PRINTLN(ntpServerIP);}
  WaitForNTPIP += 1;
  if(ntpServerIP.toString() == IPAddress(INADDR_NONE).toString()) {
  if (WaitForNTPIP > 2) {  
  printStringWithShift(cleanText("        +++        Le Serveur NTP ne Répond pas !      +++           ").c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift(cleanText("        +++        Vérifiez la Connexion Internet      +++           ").c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  // Redémarrer l'horloge
  printStringWithShift(cleanText("        +++       L'Horloge Redémarrera      +++           ").c_str(),ScrollTextTime);
  SaveClockStatus("Le Serveur NTP ne Répond pas !"); // Enregistrer l'état de l'Horloge 
  MyWaitLoop(500);
  ESP.restart();}}
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    delay(1); 
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      WaitForNTPIP = 0;
      if (SerialMonitor) {DEBUG_PRINTLN(F("Receive NTP Response"));}
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Lire le paquet dans le tampon
      unsigned long secsSince1900;
      // convertit quatre octets de la position 40 en un entier long
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  if (SerialMonitor) {DEBUG_PRINTLN("NTP-Time - Aucune réponse NTP");}
  // return 0; // renvoie 0 si l'heure ne peut pas être déterminée.
  printStringWithShift(cleanText("        +++        Le Serveur NTP ne Répond pas !      +++           ").c_str(),ScrollTextTime);
  // Neustart Uhr
  if (SerialMonitor) {DEBUG_PRINTLN("L'Horloge Redémarrera");}
  printStringWithShift(cleanText("        +++       L'Horloge Redémarrera      +++           ").c_str(),ScrollTextTime);
  SaveClockStatus("Le Serveur NTP ne Répond pas !"); // Enregistrer l'état de l'Horloge
  ESP.restart();
  return false;}

// =============================================================================================================================

// envoyer une requête NTP au serveur de temps à l'adresse indiquée
void sendNTPpacket(IPAddress &address) {
  // mettre tous les octets du tampon à 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialisez les valeurs requises pour la formation des requêtes NTP.
  // (voir l'URL ci-dessus pour les détails du package)
  packetBuffer[0] = 0b11100011;   // LI, version, mode
  packetBuffer[1] = 0;     // Strate, ou type d'horloge
  packetBuffer[2] = 6;     // Intervalle d'interrogation
  packetBuffer[3] = 0xEC;  // Précision de l'horloge des pairs
  // 8 octets de zéro pour le délai de racine et la dispersion des racines
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // tous les champs NTP ont maintenant reçu des valeurs
  // Vous pouvez envoyer un paquet demandant un horodatage :
  Udp.beginPacket(address, 123); // Les requêtes NTP doivent être effectuées sur le port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  yield();}
