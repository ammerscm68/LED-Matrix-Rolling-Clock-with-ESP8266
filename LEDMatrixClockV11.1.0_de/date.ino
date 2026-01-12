int Calculate_Eastern() { // Danke an "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916
// nach der Gauß-Formel
// Rückgabewert: Datum von Ostern ab 1. März (max=56 für 25. April)
int Ea, Eb, Ec, Ed, Ee, Ef, Ek, EM, EN, Ep, Eq;
  
  // Die "magische" Gauss-Formel anwenden:
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
  Ef = 22+Ed+Ee;   // Tag auf Maerz bezogen: 32 bedeutet Erster April usw.

  if (Ef==57) {Ef=50;}   // Wenn der 26. April ermittelt wird (31+26=57), dann muss Ostern am 19.4. sein (19+31=50)
  /// Falls der 25. April ermittelt wird, gilt dies nur wenn d=28 und a>10
  if ((Ef==56) && (Ed==28) && (Ea>10))  {Ef=49;}
  return Ef;}

 // *************************************************************************************************

boolean buss_und_bettag() { // Danke an "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916
// Buss- und Bettag fällt auf den Mittwoch zwischen dem 16. und 22. November
if ((mo==11) && (d>15) && (d<23)) {
if (w==4) { return true;}}  // Wenn heute Mittwoch ist
return false;}

 // *************************************************************************************************
 
int ymdToWeekNumber (int y, byte m, byte d) {  // Danke "odometer" für diese Routine
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

String ExpandedDateDE() {
getTimeLocal(); // aktuelle Zeit holen  
String FormatedDay; 
String FormatedMonth; 
String DS;  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
String dayName[] = {"Fehler", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
DS = "          +++         Heute ist " + dayName[w] + " der   " + FormatedDay + "." + FormatedMonth + ".";
DS += "" + String(ye)+ "   in der "+ymdToWeekNumber(ye, mo, d)+". Kalenderwoche        +++               ";
return DS;
}

// *************************************************************************************************

boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month<3 || month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month>3 && month<10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month==3 && (hour + 24 * day)>=(1 + tzHours + 24*(31 - (5 * year /4 + 4) % 7)) || month==10 && (hour + 24 * day)<(1 + tzHours + 24*(31 - (5 * year /4 + 1) % 7)))
    return true;
  else
    return false;
}

// *************************************************************************************************