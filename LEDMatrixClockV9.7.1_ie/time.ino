
void getTimeLocal() {
  time_t tT = now();
  // local time
  time_t tTlocal = IE.toLocal(tT);
  w = weekday(tTlocal);
  d = day(tTlocal);
  mo = month(tTlocal);
  ye = year(tTlocal);
  h = hour(tTlocal);
  m = minute(tTlocal);
  s = second(tTlocal);
  //localMillisAtUpdate = millis();
  //client.stop();
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
  if (AnimWait) {MyWaitLoop(ScrollClockTime);}} // Time scrolling speed

// =======================================================================

int FormatedTime() {
String fTime, fsh, fsm;  
getTimeLocal(); // get current time
if (h < 10) {fsh = "0"+String(h);} else {fsh = String(h);}  
if (m < 10) {fsm = "0"+String(m);} else {fsm = String(m);}  
// if (s < 10) {fss = "0"+String(s);} else {fss = String(s);}  
fTime =fsh+fsm;
return fTime.toInt();}

// =======================================================================