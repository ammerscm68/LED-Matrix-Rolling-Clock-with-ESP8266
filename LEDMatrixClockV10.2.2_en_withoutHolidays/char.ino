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
  if (c < ' ' || c > '~'+52) return;
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
  if (c < ' ' || c > '~'+52) return;
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

String cleanText(String text) {
  text.replace("’", "'");
  text.replace("“", "\"");
  text.replace("”", "\"");
  text.replace("`", "'");
  text.replace("‘", "'");
  text.replace("„", "'");
  text.replace("\\\"", "'");
  text.replace("•", "-");
  text.replace("é", String(fre1));
  text.replace("è", String(fre2));
  text.replace("é", String(fre1));
  text.replace("ë", String(fre3));
  text.replace("ê", String(fre4));
  text.replace("à", String(fra1));
  text.replace("â", String(fra2));
  text.replace("ù", String(fru));
  text.replace("ç", String(frc));
  text.replace("î", String(fri1));
  text.replace("ï", String(fri2));
  text.replace("ô", String(fro));
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", String(frA1));
  text.replace("À", String(frA2));
  text.replace("æ", String(uae));
  text.replace("Æ", String(uAe));
  text.replace("É", String(frE1));
  text.replace("È", String(frE2));
  text.replace("Ë", String(frE3));
  text.replace("Ô", String(frO1));
  text.replace("œ", String(uoe));
  text.replace("Œ", String(uOe));
  text.replace("Ù", String(frU1));
  text.replace("Û", String(frU2));
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
  text.replace("Ö", String(uOe)); // Variante 1
  text.replace("Ö", String(uOe)); // Variante 2
  text.replace("ö", String(uoe)); // Variante 1
  text.replace("ö", String(uoe)); // Variante 2
  text.replace("»", "'");
  text.replace("«", "'");
  return text;}
  
// =======================================================================