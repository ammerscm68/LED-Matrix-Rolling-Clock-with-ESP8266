/*-------- Code du Serveur NTP ----------*/

// -------------------------------- Fuseau Horaire. ----------------------------------- //
// Modifiez ces lignes en fonction de votre fuseau horaire et de l'heure d'été.
// Détails des Paramètres de fuseau Horaire https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Standard Time (Berlin, Frankfurt, Vienna, Paris, Madrid)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Berlin, Frankfurt, Vienna, Paris, Madrid)
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast, Dublin)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

// UTC
/*TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);*/

// US Eastern Time Zone (New York, Detroit, Miami, Lima)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston, Mexico City)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City, Calgary)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);

// US Pacific Time Zone (Las Vegas, Los Angeles, Seattle)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);

// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
Timezone tzMSK(msk);

// =============================================================================================================================

const int NTP_PACKET_SIZE = 48; // Heure NTP dans les 48 premiers octets du message
byte packetBuffer[NTP_PACKET_SIZE]; // Tampon pour les paquets entrants et sortants

time_t getNtpTime() {
  IPAddress ntpServerIP; // Adresse IP du serveur NTP
  while (Udp.parsePacket() > 0) ; // rejette tous les paquets reçus précédemment
  // obtenir un serveur aléatoire du pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  if (SerialMonitor) {
  Serial.print(ntpServerName);
  Serial.print(F(": "));
  Serial.println(ntpServerIP);}
  WaitForNTPIP += 1;
  if(ntpServerIP.toString() == IPAddress(INADDR_NONE).toString()) {
  if (WaitForNTPIP > 2) { 
  ct = cleanText("        +++        Le Serveur NTP ne Répond pas !      +++           ");   
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  ct = cleanText("        +++        Vérifiez la Connexion Internet      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  // Redémarrer l'horloge
  ct = cleanText("        +++       L'Horloge Redémarrera      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  MyWaitLoop(500);
  ESP.restart();}}
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    delay(1); 
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      WaitForNTPIP = 0;
      if (SerialMonitor) {Serial.println(F("Receive NTP Response"));}
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Lire le paquet dans le tampon
      unsigned long secsSince1900;
      // convertit quatre octets de la position 40 en un entier long
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  if (SerialMonitor) {Serial.println("NTP-Time - Aucune réponse NTP - "+FormatedDateTimeFR(false));}
  // return 0; // renvoie 0 si l'heure ne peut pas être déterminée.
  ct = cleanText("        +++        Le Serveur NTP ne Répond pas !      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
  // Neustart Uhr
  if (SerialMonitor) {Serial.println("L'Horloge Redémarrera - "+FormatedDateTimeFR(false));}
  ct = cleanText("        +++       L'Horloge Redémarrera      +++           ");
  printStringWithShift(ct.c_str(),ScrollTextTime);
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
