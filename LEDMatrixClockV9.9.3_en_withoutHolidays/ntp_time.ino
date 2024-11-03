/*-------- NTP-Server code ----------*/

// -------------------------------- Timezone. ----------------------------------- //
// Edit these Lines According to your Time Zone and Daylight saving Time.
// TimeZone Settings Details https://github.com/JChristensen/Timezone
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

/*
// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
Timezone tzMSK(msk);

// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

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

const int NTP_PACKET_SIZE = 48; // NTP time in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; // Buffer for incoming and outgoing packets

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip Address
  while (Udp.parsePacket() > 0) ; // discard all previously received packets
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  if (SerialMonitor) {
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);}
  WaitForNTPIP += 1;
  if(ntpServerIP.toString() == IPAddress(INADDR_NONE).toString()) {
  if (WaitForNTPIP > 2) {  
  printStringWithShift("        +++        The NTP Server is not responding !      +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  printStringWithShift("        +++        Check the Internet Connection      +++           ",ScrollTextTime);
  MyWaitLoop(1000);
  // Neustart Uhr
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
  // Clock Restart
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
