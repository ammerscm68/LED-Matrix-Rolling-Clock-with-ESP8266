/*-------- NTP-Server code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; // Buffer for incoming and outgoing packets

time_t getNtpTime() {
  IPAddress ntpServerIP; // NTP server's ip Address
  while (Udp.parsePacket() > 0) ; // discard all previously received packets
  // Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
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
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Read packet into buffer
      unsigned long secsSince1900;
      // convert four bytes from position 40 into a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;}}
  Serial.println("NTP-Time - No NTP response - "+FormatedDateTime());
  // return 0; // returns 0 if the time cannot be determined.
  printStringWithShift("        +++        The NTP Server is not responding !      +++           ",ScrollTextTime);
  // Neustart Uhr
  Serial.println("The Clock will restart - "+FormatedDateTime());
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
