/*-------- NTP-Server code ----------*/

// ---------------------------- Timezone. -------------------------------- //
// Edit these Lines According to your Time Zone and Daylight saving Time.
// TimeZone Settings Details https://github.com/JChristensen/Timezone

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

const int NTP_PACKET_SIZE = 48; // NTP time in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; // Buffer for incoming and outgoing packets

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip Address
  while (Udp.parsePacket() > 0) ; // discard all previously received packets
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  if (SerialMonitor) {
  Serial.print(ntpServerName);
  Serial.print(F(": "));
  Serial.println(ntpServerIP);}
  WaitForNTPIP += 1;
  if(ntpServerIP.toString() == IPAddress(INADDR_NONE).toString()) {
  if (WaitForNTPIP > 2) {  
  printStringWithShift("        +++        The NTP Server is not responding !      +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift("        +++        Check the Internet Connection      +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  // Restart Clock
  printStringWithShift("        +++       The Clock will restart      +++           ",ScrollTextTime);
  MyWaitLoop(500);
  ESP.restart();}}
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    delay(1); // kurze Pause
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      WaitForNTPIP = 0;
      if (SerialMonitor) {Serial.println(F("Receive NTP Response"));}
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Read packet into buffer
      unsigned long secsSince1900;
      // convert four bytes from position 40 into a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  if (SerialMonitor) {Serial.println("NTP-Time - No NTP response - "+FormatedDateTimeEN(false));}
  // return 0; // returns 0 if the time cannot be determined.
  printStringWithShift("        +++        The NTP Server is not responding !      +++           ",ScrollTextTime);
  // Restart Clock
  if (SerialMonitor) {Serial.println("The Clock will restart - "+FormatedDateTimeEN(false));}
  printStringWithShift("        +++       The Clock will restart      +++           ",ScrollTextTime);
  ESP.restart();
  return false;}

// =============================================================================================================================

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values ​​that are required for the formation of NTP requests.
  // (see URL above for package details)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have now been assigned values
  // You can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests should be made on port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  yield();}