/*-------- NTP-Server code ----------*/

// ---------------------------- Timezone. -------------------------------- //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Standard Time (Frankfurt, Paris, Wien)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris, Wien)
Timezone CE(CEST, CET);

/*
// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);

// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);
*/

// =============================================================================================================================

const int NTP_PACKET_SIZE = 48; // NTP-Zeit in den ersten 48 Bytes der Nachricht
byte packetBuffer[NTP_PACKET_SIZE]; //Puffer für eingehende und ausgehende Pakete

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip Adresse
  while (Udp.parsePacket() > 0) ; // alle zuvor empfangenen Pakete verwerfen
  // einen zufälligen Server aus dem Pool holen
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  WaitForNTPIP += 1;
  if(ntpServerIP.toString() == IPAddress(INADDR_NONE).toString()) {
  if (WaitForNTPIP > 2) {  
  printStringWithShift("        +++        Der NTP-Server antwortet nicht !      +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  String CheckWebConnection = "        +++        "+String(uUe)+"berpr"+String(uue)+"fen Sie die Internetverbindung      +++           ";
  printStringWithShift(CheckWebConnection.c_str(),ScrollTextTime);
  MyWaitLoop(1000);
  // Neustart Uhr
  printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
  MyWaitLoop(500);
  ESP.restart();}}
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    delay(1); // kurze Pause
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      WaitForNTPIP = 0;
      Serial.println(F("Empfange Antwort vom NTP-Server"));
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Paket in den Puffer einlesen
      unsigned long secsSince1900;
      // vier Bytes ab Position 40 in eine lange Ganzzahl umwandeln
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  Serial.println("NTP-Time - keine NTP Antwort - "+FormatedDateTime());
  // return 0; // gibt 0 zurück, wenn die Zeit nicht ermittelt werden kann.
  printStringWithShift("        +++        Der NTP-Server antwortet nicht !      +++           ",ScrollTextTime);
  // Neustart Uhr
  Serial.println("Die Uhr wird neu gestartet - "+FormatedDateTime());
  printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
  ESP.restart();
  return false;}

// =============================================================================================================================

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // alle Bytes im Puffer auf 0 setzen
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialisieren von Werten, die für die Bildung von NTP-Requests benötigt werden.
  // (siehe URL oben für Details zu den Paketen)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // alle NTP-Felder wurden jetzt mit Werten versehen
  // Sie können ein Paket senden, das einen Zeitstempel anfordert.:
  Udp.beginPacket(address, 123); //NTP-Requests sollen auf Port 123 erfolgen
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  yield();} // Equivalent zu  Application.ProcessMessages; 
