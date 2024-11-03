bool RunText = false; // Ne démarrez pas un ticker si un autre est déjà en cours d'exécution

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
  if (c < ' ' || c > '~'+51) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    yield(); 
    MyWaitLoop(shiftDelay);
    scrollLeft();
    refreshAll();
    HandleOTAandWebServer();
    yield();
  }
}

// =======================================================================

void printCharWithShift2(unsigned char c, int shiftDelay) {
  if (c < ' ' || c > '~'+51) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    yield();
    MyWaitLoop(shiftDelay);
    refreshAll();
    HandleOTAandWebServer();  
    yield();
  }
}

// =======================================================================

void printCharWithShift3(unsigned char c, int shiftDelay) {
  c -= 32;
  int w = showChar(c, clockfont);
  for (int i=0; i<w; i++) {
    yield(); 
    MyWaitLoop(shiftDelay);
    scrollLeft();
    refreshAll();
    HandleOTAandWebServer();  
    yield(); 
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
  text.replace("à", String(fra1));
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
  text.replace("Ö", String(uOe));
  text.replace("ö", String(uoe));
  text.replace("œ", String(uoe));
  text.replace("Œ", String(uOe));
  text.replace("Ù", String(frU1));
  text.replace("Û", String(frU2));
  text.replace("Ü", String(uUe));
  text.replace("ü", String(uue));
  text.replace("Ä", String(uAe));
  text.replace("ä", String(uae));
  text.replace("ß", String(uss));
  text.replace("»", "'");
  text.replace("«", "'");
  return text;}
// =======================================================================
// =======================================================================
