void getTimeLocal() {
time_t tTl = GetTimeZone(now());
w = weekday(tTl);
d = day(tTl);
mo = month(tTl);
ye = year(tTl);
h = hour(tTl);
m = minute(tTl);
s = second(tTl);
//localMillisAtUpdate = millis();
}

// =======================================================================   

time_t GetTimeZone(time_t tT) {  // bool  XX.locIsDST(tTlocal) - UTC-Offset  |   false = Standardtime , true = Summertime
time_t tTlocal;  
if (MCTZ == "Berlin, Frankfurt, Wien, Paris, Madrid") {tTlocal = CE.toLocal(tT); if (!CE.locIsDST(tTlocal)) {utco = 1;} else {utco = 2;}}  // +1h | +2h
if (MCTZ == "London, Belfast, Dublin") {tTlocal = UK.toLocal(tT); if (!UK.locIsDST(tTlocal)) {utco = 0;} else {utco = 1;}}  // +0h | +1h
if (MCTZ == "New York, Detroit, Miami, Lima") {tTlocal = usET.toLocal(tT); if (!usET.locIsDST(tTlocal)) {utco = -5;} else {utco = -4;}}  // -5h | -4h
if (MCTZ == "Chicago, Houston, Mexico City") {tTlocal = usCT.toLocal(tT); if (!usCT.locIsDST(tTlocal)) {utco = -6;} else {utco = -5;}}  // -6h | -5h
if (MCTZ == "Denver, Salt Lake City, Calgary") {tTlocal = usMT.toLocal(tT); if (!usMT.locIsDST(tTlocal)) {utco = -7;} else {utco = -6;}}  // -7h | -6h
if (MCTZ == "Arizona") {tTlocal = usAZ.toLocal(tT); utco = -8;} // does not use DST
if (MCTZ == "Las Vegas, Los Angeles, Seattle") {tTlocal = usPT.toLocal(tT); if (!usPT.locIsDST(tTlocal)) {utco = -8;} else {utco = -7;}}  // -8h | -7h
if (MCTZ == "Sydney, Melbourne") {tTlocal = ausET.toLocal(tT); if (!ausET.locIsDST(tTlocal)) {utco = 10;} else {utco = 11;}}  // +10h | +11h
if (MCTZ == "Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary") {tTlocal = caMT.toLocal(tT); if (!caMT.locIsDST(tTlocal)) {utco = -7;} else {utco = -6;}}  // -7h | -6h
if (MCTZ == "Vancouver") {tTlocal = caPT.toLocal(tT); if (!caPT.locIsDST(tTlocal)) {utco = -8;} else {utco = -7;}}  // -8h | -7h
if (MCTZ == "Winnipeg, Baker Lake, Kenora, Regina, Creighton") {tTlocal = caCT.toLocal(tT); if (!caCT.locIsDST(tTlocal)) {utco = -6;} else {utco = -5;}}  // -6h | -5h
if (MCTZ == "Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules") {tTlocal = caACT.toLocal(tT); if (!caACT.locIsDST(tTlocal)) {utco = -4;} else {utco = -3;}}  // -4h | -3h
if (MCTZ == "Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa") {tTlocal = caET.toLocal(tT); if (!caET.locIsDST(tTlocal)) {utco = -5;} else {utco = -4;}}  // -5h | -4h
if (MCTZ == "Whitehorse") {tTlocal = tzMST.toLocal(tT); utco = -7;}  // -7h | does not use DST
if (MCTZ != "Berlin, Frankfurt, Wien, Paris, Madrid" && 
    MCTZ != "London, Belfast, Dublin" &&
    MCTZ != "New York, Detroit, Miami, Lima" &&
    MCTZ != "Chicago, Houston, Mexico City" &&
    MCTZ != "Denver, Salt Lake City, Calgary" &&
    MCTZ != "Arizona" &&
    MCTZ != "Las Vegas, Los Angeles, Seattle" &&
    MCTZ != "Sydney, Melbourne" &&
    MCTZ != "Edmonton, Cranbrook, Fort St. John, Inuvik, Cambridge Bay, Lloydminster, Calgary" &&
    MCTZ != "Vancouver" &&
    MCTZ != "Winnipeg, Baker Lake, Kenora, Regina, Creighton" &&
    MCTZ != "Saint John, Happy Valley-Goose Bay, Halifax, Charlottetown, Blanc-Sablon, Cap-aux-Meules" &&
    MCTZ != "Coral Harbour, Pond Inlet, Atikokan, Toronto, Montreal, Ottawa" &&
    MCTZ != "Whitehorse") {MCTZ = "London, Belfast, Dublin"; tTlocal = UK.toLocal(tT);}
/*if (SerialMonitor) {
if (!CE.locIsDST(tTlocal)) {DEBUG_PRINTLN("Standardtime");} else {DEBUG_PRINTLN("Summertime");}}*/    
return tTlocal;  
}

// =======================================================================

void getAnimClock(bool AnimWait) {  
getTimeLocal(); // get current time  
  byte digPos[4]={1,8,17,25};
  int digHt = 12;
  int num = 4; 
  int i;
  if(del==0) {
    del = digHt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    dig[0] = h/10 ? h/10 : 10;
    dig[1] = h%10;
    dig[2] = m/10;
    dig[3] = m%10;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : digHt;
  } else
    del--;
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;}}
  dy=0;
  setCol(15,dots ? B00100100 : 0);
  // setCol(16,dots ? B00100100 : 0);
  refreshAll();
  if (AnimWait) {MyWaitLoop(ScrollClockTime);}} // Time Scrolling Speed

// =======================================================================

int16_t FormatedTime() {  
String fTime, fsh, fsm;  
getTimeLocal(); // get current time 
if (h < 10) {fsh = "0"+String(h);} else {fsh = String(h);}  
if (m < 10) {fsm = "0"+String(m);} else {fsm = String(m);}  
// if (s < 10) {fss = "0"+String(s);} else {fss = String(s);}  
fTime =fsh+fsm;
// DEBUG_PRINTLN("Formated Time: "+fTime); // Only for Debuging 
return (int16_t)strtol(fTime.c_str(), NULL, 10);}

// =======================================================================  