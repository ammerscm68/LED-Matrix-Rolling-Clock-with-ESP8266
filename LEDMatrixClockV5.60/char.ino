bool RunText = false; // Laufschrift nicht starten wenn bereits eine andere läuft 

void showDigit(char ch, int col, const uint8_t *data)
{
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<8*NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}

// =======================================================================

void setCol(int col, byte v)
{
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<8*NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

// =======================================================================

int showChar(char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

// =======================================================================

void printCharWithShift(unsigned char c, int shiftDelay) {
  if (c < ' ' || c > '~'+29) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    yield(); // Equivalent zu  Application.ProcessMessages; 
    MyWaitLoop(shiftDelay);
    scrollLeft();
    refreshAll();
    HandleOTAandWebServer();  // OTA und Webserver Abfrage
    yield(); // Equivalent zu  Application.ProcessMessages; 
  }
}

// =======================================================================

void printCharWithShift2(unsigned char c, int shiftDelay) {
  if (c < ' ' || c > '~'+29) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    yield(); // Equivalent zu  Application.ProcessMessages; 
    MyWaitLoop(shiftDelay);
    refreshAll();
    HandleOTAandWebServer();  // OTA und Webserver Abfrage
    yield(); // Equivalent zu  Application.ProcessMessages; 
  }
}

// =======================================================================

void printCharWithShift3(unsigned char c, int shiftDelay) {
  c -= 32;
  int w = showChar(c, clockfont);
  for (int i=0; i<w; i++) {
    yield(); // Equivalent zu  Application.ProcessMessages; 
    MyWaitLoop(shiftDelay);
    scrollLeft();
    refreshAll();
    HandleOTAandWebServer();  // OTA und Webserver Abfrage
    yield(); // Equivalent zu  Application.ProcessMessages; 
  }
}

// =======================================================================

void printStringWithShift(const char* s, int shiftDelay)
{  
if (!RunText) { 
RunText = true;   
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
RunText = false;
}  
}

// =======================================================================

void printStringWithShift2(const char* s, int shiftDelay)
{
if (!RunText) { 
RunText = true; 
  while (*s) {
    printCharWithShift2(*s, shiftDelay);
    s++;
  }
RunText = false;
}  
}

// =======================================================================

void printStringWithShift3(const char* s, int shiftDelay)
{
if (!RunText) { 
RunText = true;  
  while (*s) {
    printCharWithShift3(*s, shiftDelay);
    s++;
  }
RunText = false;
}  
}

// =======================================================================

String cleanText(String text) {
  text.replace("’", "'");
  text.replace("“", "\"");
  text.replace("”", "\"");
  text.replace("`", "'");
  text.replace("‘", "'");
  text.replace("„", "'");
  text.replace("\\\"", "'");
  text.replace("•", "-");
  text.replace("é", "e");
  text.replace("è", "e");
  text.replace("ë", "e");
  text.replace("ê", "e");
  text.replace("à", "a");
  text.replace("â", "a");
  text.replace("ù", "u");
  text.replace("ç", "c");
  text.replace("î", "i");
  text.replace("ï", "i");
  text.replace("ô", "o");
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", "A");
  text.replace("À", "A");
  text.replace("æ", "ae");
  text.replace("Æ", "AE");
  text.replace("É", "E");
  text.replace("È", "E");
  text.replace("Ë", "E");
  text.replace("Ô", "O");
  text.replace("Ö", "Oe");
  text.replace("ö", "oe");
  text.replace("œ", "oe");
  text.replace("Œ", "OE");
  text.replace("Ù", "U");
  text.replace("Û", "U");
  text.replace("Ü", "Ue");
  text.replace("ü", "ue");
  text.replace("Ä", "Ae");
  text.replace("ä", "ae");
  text.replace("ß", "ss");
  text.replace("»", "'");
  text.replace("«", "'");
  return text;
}
// =======================================================================
// =======================================================================
