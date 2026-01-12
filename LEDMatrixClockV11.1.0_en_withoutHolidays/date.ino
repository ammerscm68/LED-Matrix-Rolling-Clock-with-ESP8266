int Calculate_Eastern() { // thanks to "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916
// according to the Gauss formula
// Return value: Date of Easter from March 1st (max=56 for April 25th)
int Ea, Eb, Ec, Ed, Ee, Ef, Ek, EM, EN, Ep, Eq;
  
  // Apply the "magic" Gauss formula:
  Ea = ye % 19;
  Eb = ye % 4;
  Ec = ye % 7;
  Ek = ye / 100;
  Ep = (8*Ek+13) / 25;
  Eq = Ek / 4;
  EM =(15 + Ek - Ep - Eq) % 30;
  EN = (4 + Ek-Eq) % 7;
  Ed = (19*Ea + EM) % 30;
  Ee = (2*Eb + 4*Ec + 6*Ed + EN) % 7;
  Ef = 22+Ed+Ee;   // Day related to March: 32 means April First etc.

  if (Ef==57) {Ef=50;}   // If April 26th is determined (31+26=57), then Easter must be on April 19th. be (19+31=50)
  /// If April 25th is determined, this only applies if d=28 and a>10
  if ((Ef==56) && (Ed==28) && (Ea>10))  {Ef=49;}
  return Ef;}

 // *************************************************************************************************

 int ymdToWeekNumber (int y, int m, int d) {  // Thanks to "odometer" for this Routine
  // reject out-of-range dates
  if ((y < 1901)||(y > 2099)) return 0;
  if ((m < 1)||(m > 12)) return 0;
  if ((d < 1)||(d > 31)) return 0;
  // (It is useful to know that Jan. 1, 1901 was a Tuesday)
  // compute adjustment for dates within the year
  //     If Jan. 1 falls on: Mo Tu We Th Fr Sa Su
  // then the adjustment is:  6  7  8  9  3  4  5
  int adj = (((y-1901) + ((y-1901)/4) + 4) % 7) + 3;
  // compute day of the year (in range 1-366)
  int doy = d;
  if (m > 1) doy += 31;
  if (m > 2) {
    if ((y%4)==0) doy += 29;
    else doy += 28;
  }
  if (m > 3) doy += 31;
  if (m > 4) doy += 30;
  if (m > 5) doy += 31;
  if (m > 6) doy += 30;
  if (m > 7) doy += 31;
  if (m > 8) doy += 31;
  if (m > 9) doy += 30;
  if (m > 10) doy += 31;
  if (m > 11) doy += 30;
  // compute week number
  int wknum = (adj + doy) / 7;
  // check for boundary conditions
  if (wknum < 1) {
    // last week of the previous year
    // check to see whether that year had 52 or 53 weeks
    // re-compute adjustment, this time for previous year
    adj = (((y-1902) + ((y-1902)/4) + 4) % 7) + 3;
    // all years beginning on Thursday have 53 weeks
    if (adj==9) return 53;
    // leap years beginning on Wednesday have 53 weeks
    if ((adj==8) && ((y%4)==1)) return 53;
    // other years have 52 weeks
    return 52;
  }
  if (wknum > 52) {
    // check to see whether week 53 exists in this year
    // all years beginning on Thursday have 53 weeks
    if (adj==9) return 53;
    // leap years beginning on Wednesday have 53 weeks
    if ((adj==8) && ((y%4)==0)) return 53;
    // other years have 52 weeks
    return 1;
  }
  return wknum;
}

// *************************************************************************************************

String ExpandedDateEN() {
getTimeLocal(); // get current time  
String DS;
String Attach; 
String FormatedDay;
String FormatedMonth; 
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
String dayName[] = {"Error", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int AWeekNumber = ymdToWeekNumber(ye, mo, d); 
int Wst[] = {1, 21, 31, 41, 51};
int Wnd[] = {2, 22, 32, 42, 52};
int Wrd[] = {3, 23, 33, 43, 53};
int Wth[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 24, 25, 26, 27, 28, 29, 30, 34, 35, 36, 37, 38,
             39, 40, 44, 45, 46, 47, 48, 49, 50};
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wst[i]) {Attach = F("st");}}
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wnd[i]) {Attach = F("nd");}}
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wrd[i]) {Attach = F("rd");}}
    for (int i=0; i<38; i++) {
    if (AWeekNumber == Wth[i]) {Attach = F("th");}}
DS = "          +++         Today is " + dayName[w] + "   " + FormatedDay + "/";
DS +="" + FormatedMonth + "/" + String(ye) + "   in the "+AWeekNumber+Attach+" Calendar Week        +++               ";
return DS;
}

// ************************************************************************************************* ExpandedDateEN