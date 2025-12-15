/*-------- NTP-Server code ----------*/

// ---------------------------- Timezone. -------------------------------- //
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Standard Time (Berlin, Frankfurt, Wien, Paris, Madrid)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Berlin, Frankfurt, Wien, Paris, Madrid)
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

const int NTP_PACKET_SIZE = 48; // NTP-Zeit in den ersten 48 Bytes der Nachricht
byte packetBuffer[NTP_PACKET_SIZE]; //Puffer für eingehende und ausgehende Pakete

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip Adresse
  while (Udp.parsePacket() > 0) ; // alle zuvor empfangenen Pakete verwerfen
  // DEBUG_PRINTLN(F("Transmit NTP Request"));
  // einen zufälligen Server aus dem Pool holen
  WiFi.hostByName(ntpServerName, ntpServerIP);
  if (SerialMonitor) {
  DEBUG_PRINT(ntpServerName);
  DEBUG_PRINT(": ");
  DEBUG_PRINTLN(ntpServerIP);}
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
  SaveClockStatus("Der NTP-Server antwortet nicht !"); // Status der Uhr speichern 
  MyWaitLoop(500);
  ESP.restart();}}
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    delay(1); // kurze Pause
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      WaitForNTPIP = 0;
      if (SerialMonitor) {DEBUG_PRINTLN(F("Empfange Antwort vom NTP-Server"));}
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Paket in den Puffer einlesen
      unsigned long secsSince1900;
      // vier Bytes ab Position 40 in eine lange Ganzzahl umwandeln
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  if (SerialMonitor) {DEBUG_PRINTLN("NTP-Time - keine NTP Antwort");}
  // return 0; // gibt 0 zurück, wenn die Zeit nicht ermittelt werden kann.
  printStringWithShift("        +++        Der NTP-Server antwortet nicht !      +++           ",ScrollTextTime);
  // Neustart Uhr
  if (SerialMonitor) {DEBUG_PRINTLN("Die Uhr wird neu gestartet");}
  printStringWithShift("        +++       Die Uhr wird neu gestartet      +++           ",ScrollTextTime);
  SaveClockStatus("Der NTP-Server antwortet nicht !"); // Status der Uhr speichern 
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