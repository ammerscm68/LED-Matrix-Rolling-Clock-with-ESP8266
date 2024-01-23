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
 
String First_Monday(int fm) {
int day, d1, y;
String Out;
        d1  = 1; // First Day in Month for weekday calculation
        day = 1; // First Day in Month
        y = ye; // Current Year
        //expression for converting a Gregorian date into a numerical day of the week
        int wd = ((d1 += fm < 3 ? y-- : y - 2, 23*fm/9 + d1 + 4 + y/4- y/100 + y/400)%7);
        if (wd == 0) day = day + 1;
        if (wd == 2) day = day + 6;
        if (wd == 3) day = day + 5;
        if (wd == 4) day = day + 4;
        if (wd == 5) day = day + 3; 
        if (wd == 6) day = day + 2;
        //print the date of the month that is the last Sunday
        return String(day)+"."+String(fm);
}  

 // *************************************************************************************************

String Second_Monday(int sm) {
int day, d1, y;
String Out;
        d1  = 1; // First Day in Month for weekday calculation
        day = 1; // First Day in Month
        y = ye; // Current Year
        //expression for converting a Gregorian date into a numerical day of the week
        int wd = ((d1 += sm < 3 ? y-- : y - 2, 23*sm/9 + d1 + 4 + y/4- y/100 + y/400)%7);
        if (wd == 0) day = day + 8;
        if (wd == 1) day = day + 7;
        if (wd == 2) day = day + 13;
        if (wd == 3) day = day + 12;
        if (wd == 4) day = day + 11;
        if (wd == 5) day = day + 10; 
        if (wd == 6) day = day + 9;
        //print the date of the month that is the last Sunday
        return String(day)+"."+String(sm);
}  

 // *************************************************************************************************

String Third_Monday(int tm) {
int day, d1, y;
String Out;
        d1  = 1; // First Day in Month for weekday calculation
        day = 1; // First Day in Month
        y = ye; // Current Year
        //expression for converting a Gregorian date into a numerical day of the week
        int wd = ((d1 += tm < 3 ? y-- : y - 2, 23*tm/9 + d1 + 4 + y/4- y/100 + y/400)%7);
        if (wd == 0) day = day + 15;
        if (wd == 1) day = day + 14;
        if (wd == 2) day = day + 20;
        if (wd == 3) day = day + 19;
        if (wd == 4) day = day + 18;
        if (wd == 5) day = day + 17; 
        if (wd == 6) day = day + 16;
        //print the date of the month that is the last Sunday
        return String(day)+"."+String(tm);
}  

 // *************************************************************************************************

String Last_Monday(int lm) {
int y, d1;
String Out;
    //number of days of each month
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //checking if february has 28 or 29 days
    days[1] -= (y % 4) || (!(y % 100) && (y % 400));
        //get the number of days from the array for the corresponding month
        d1 = days[lm - 1];
        y = ye;
        //expression for converting a Gregorian date into a numerical day of the week
        int wd = ((d1 += lm < 3 ? y-- : y - 2, 23*lm/9 + d1 + 4 + y/4- y/100 + y/400)%7);
        int day = days[lm-1] - wd;
        if (day < days[lm-1]) day = day + 1; // Monday
        //print the date of the month that is the last Sunday
        return String(day)+"."+String(lm);
}  

 // *************************************************************************************************

String FourthThursday(int ft) {
int day, d1, y;
String Out;
        d1  = 1; // First Day in Month for weekday calculation
        day = 1; // First Day in Month
        y = ye; // Current Year
        //expression for converting a Gregorian date into a numerical day of the week
        int wd = ((d1 += ft < 3 ? y-- : y - 2, 23*ft/9 + d1 + 4 + y/4- y/100 + y/400)%7);
        if (wd == 0) day = day + 25;
        if (wd == 1) day = day + 24;
        if (wd == 2) day = day + 23;
        if (wd == 3) day = day + 22;
        if (wd == 4) day = day + 21;
        if (wd == 5) day = day + 27; 
        if (wd == 6) day = day + 26;
        //print the date of the month that is the last Sunday
        return String(day)+"."+String(ft);
} 
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

String ExpandedDate() {
String DS = "";
String Attach = "";  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
String dayName[] = {"Err", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int AWeekNumber = ymdToWeekNumber(ye, mo, d); 
int Wst[] = {1, 21, 31, 41, 51};
int Wnd[] = {2, 22, 32, 42, 52};
int Wrd[] = {3, 23, 33, 43, 53};
int Wth[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 24, 25, 26, 27, 28, 29, 30, 34, 35, 36, 37, 38,
             39, 40, 44, 45, 46, 47, 48, 49, 50};
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wst[i]) {Attach = "st";}}
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wnd[i]) {Attach = "nd";}}
    for (int i=0; i<5; i++) {
    if (AWeekNumber == Wrd[i]) {Attach = "rd";}}
    for (int i=0; i<38; i++) {
    if (AWeekNumber == Wth[i]) {Attach = "th";}}
DS = "          +++         Today is " + dayName[w] + "   " + String(ye) + "-";
DS +="" + FormatedMonth + "-" + FormatedDay + "   in the "+AWeekNumber+Attach+" Calendar Week        +++               ";
return DS;
}


// *************************************************************************************************
// ************************************************************************************************* 