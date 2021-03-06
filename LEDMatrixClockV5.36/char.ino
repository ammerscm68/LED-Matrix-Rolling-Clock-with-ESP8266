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
    HandleWebServer();  // Webserver Abfrage
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
    HandleWebServer();  // Webserver Abfrage
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
    HandleWebServer();  // Webserver Abfrage
    yield(); // Equivalent zu  Application.ProcessMessages; 
  }
}

// =======================================================================

void printStringWithShift(const char* s, int shiftDelay)
{
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}

// =======================================================================

void printStringWithShift2(const char* s, int shiftDelay)
{
  while (*s) {
    printCharWithShift2(*s, shiftDelay);
    s++;
  }
}

// =======================================================================

void printStringWithShift3(const char* s, int shiftDelay)
{
  while (*s) {
    printCharWithShift3(*s, shiftDelay);
    s++;
  }
}

// =======================================================================

String cleanText(String text) {
  text.replace("???", "'");
  text.replace("???", "\"");
  text.replace("???", "\"");
  text.replace("`", "'");
  text.replace("???", "'");
  text.replace("???", "'");
  text.replace("\\\"", "'");
  text.replace("???", "-");
  text.replace("??", "e");
  text.replace("??", "e");
  text.replace("??", "e");
  text.replace("??", "e");
  text.replace("??", "a");
  text.replace("??", "a");
  text.replace("??", "u");
  text.replace("??", "c");
  text.replace("??", "i");
  text.replace("??", "i");
  text.replace("??", "o");
  text.replace("???", "...");
  text.replace("???", "-");
  text.replace("??", "A");
  text.replace("??", "A");
  text.replace("??", "ae");
  text.replace("??", "AE");
  text.replace("??", "E");
  text.replace("??", "E");
  text.replace("??", "E");
  text.replace("??", "O");
  text.replace("??", "Oe");
  text.replace("??", uoe);
  text.replace("??", "oe");
  text.replace("??", "OE");
  text.replace("??", "U");
  text.replace("??", "U");
  text.replace("??", "Ue");
  text.replace("??", uue);
  text.replace("??", "Ae");
  text.replace("??", uae);
  text.replace("??", uss);
  text.replace("??", "'");
  text.replace("??", "'");
  return text;
}
// =======================================================================
// =======================================================================
