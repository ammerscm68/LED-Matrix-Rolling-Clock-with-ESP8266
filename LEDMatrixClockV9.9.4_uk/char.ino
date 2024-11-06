bool RunText = false; // Do not start a ticker if another one is already running

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
  if (c < ' ' || c > '~'+32) return;
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
  if (c < ' ' || c > '~'+32) return;
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

void printStringWithShift2(const char* s, int shiftDelay){
if (!RunText) { 
RunText = true; 
  while (*s) {
    printCharWithShift2(*s, shiftDelay);
    s++;}
RunText = false;}  }

// =======================================================================

void printStringWithShift3(const char* s, int shiftDelay){
if (!RunText) { 
RunText = true;  
  while (*s) {
    printCharWithShift3(*s, shiftDelay);
    s++;}
RunText = false;}}

// =======================================================================

tring cleanText(String text) {
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
  text.replace("à", String(uae));
  text.replace("â", String(uae));
  text.replace("ù", String(uue));
  text.replace("ç", "c");
  text.replace("î", "i");
  text.replace("ï", "i");
  text.replace("ô", String(uoe));
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", String(uAe));
  text.replace("À", String(uAe));
  text.replace("æ", String(uae));
  text.replace("Æ", String(uAe));
  text.replace("É", "E");
  text.replace("È", "E");
  text.replace("Ë", "E");
  text.replace("Ô", String(uOe));
  text.replace("Ö", String(uOe)); // Variante 1
  text.replace("Ö", String(uOe)); // Variante 2
  text.replace("ö", String(uoe)); // Variante 1
  text.replace("ö", String(uoe)); // Variante 2
  text.replace("œ", String(uoe));
  text.replace("Œ", String(uOe));
  text.replace("Ù", String(uUe));
  text.replace("Û", String(uUe));
  text.replace("Ü", String(uUe)); // Variante 1
  text.replace("Ü", String(uUe)); // Variante 2
  text.replace("ü", String(uue)); // Variante 1
  text.replace("ü", String(uue)); // Variante 2
  text.replace("Ä", String(uAe)); // Variante 1
  text.replace("Ä", String(uAe)); // Variante 2
  text.replace("ä", String(uae)); // Variante 1
  text.replace("ä", String(uae)); // Variante 2
  text.replace("ß", String(uss)); // Variante 1
  text.replace("ß", String(uss)); // Variante 2
  text.replace("»", "'");
  text.replace("«", "'");
  return text;}
  
// =======================================================================
// =======================================================================
