int Calculate_Eastern() { // grâce à "bospre" - https://forum.arduino.cc/t/feiertagsberechnung/308916
// selon la formule de Gauss
// Valeur de retour : Date de Pâques à partir du 1er mars (max=56 pour le 25 avril)
int Ea, Eb, Ec, Ed, Ee, Ef, Ek, EM, EN, Ep, Eq;
  
  // Appliquez la formule « magique » de Gauss :
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
  Ef = 22+Ed+Ee;   // Jour lié au mois de mars : le 32 signifie le premier avril, etc.

  if (Ef==57) {Ef=50;}   // Si le 26 avril est déterminé (31+26=57), alors Pâques doit être le 19 avril. être (19+31=50)
  // Si le 25 avril est déterminé, cela ne s'applique que si d=28 et a>10
  if ((Ef==56) && (Ed==28) && (Ea>10))  {Ef=49;}
  return Ef;}

 // *************************************************************************************************

 int ymdToWeekNumber (int y, int m, int d) {  // Merci à "odomètre" pour cette Routine
  // rejeter les dates hors plage
  if ((y < 1901)||(y > 2099)) return 0;
  if ((m < 1)||(m > 12)) return 0;
  if ((d < 1)||(d > 31)) return 0;
  // (Il est utile de savoir que le 1er janvier 1901 était un mardi)
  // calculer l'ajustement pour les dates de l'année
  //     Si le 1er janvier tombe : Lu Tu We Th Fr Sa Di
  // alors l'ajustement est : 6 7 8 9 3 4 5
  int adj = (((y-1901) + ((y-1901)/4) + 4) % 7) + 3;
  // calculer le jour de l'année (dans la plage 1-366)
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
  // calculer le numéro de la semaine
  int wknum = (adj + doy) / 7;
  // vérifier les conditions aux limites
  if (wknum < 1) {
    // dernière semaine de l'année précédente
    // vérifiez si cette année-là comptait 52 ou 53 semaines
    // recalculer l'ajustement, cette fois pour l'année précédente
    adj = (((y-1902) + ((y-1902)/4) + 4) % 7) + 3;
    // toutes les années commençant le jeudi comptent 53 semaines
    if (adj==9) return 53;
    // les années bissextiles commençant mercredi comptent 53 semaines
    if ((adj==8) && ((y%4)==1)) return 53;
    // les autres années ont 52 semaines
    return 52;
  }
  if (wknum > 52) {
    // vérifiez si la semaine 53 existe cette année
    // toutes les années commençant le jeudi comptent 53 semaines
    if (adj==9) return 53;
    // les années bissextiles commençant mercredi comptent 53 semaines
    if ((adj==8) && ((y%4)==0)) return 53;
    // les autres années ont 52 semaines
    return 1;
  }
  return wknum;
}

// *************************************************************************************************

String ExpandedDateFR() {
getTimeLocal(); 
String FormatedDay; 
String FormatedMonth; 
String DS;  
if (d < 10){FormatedDay = "0"+String(d);} else {FormatedDay = String(d);}
if (mo < 10){FormatedMonth = "0"+String(mo);} else {FormatedMonth = String(mo);}
String dayName[] = {"Erreur", "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
DS = "          +++         Aujourd'hui c'est " + dayName[w] + " le   " + FormatedDay + "/" + FormatedMonth + "/";
DS += "" + String(ye)+ "   dans le "+ymdToWeekNumber(ye, mo, d)+". Semaine Civile        +++               ";
return DS;
}

// *************************************************************************************************  