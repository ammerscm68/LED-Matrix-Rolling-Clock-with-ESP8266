/********************************************************
*            Buzzer piézo passif KY-006                 *
*********************************************************/

// *************************************************************************************************

void beep(int note, int duration){
tone(buzzerPin, note, duration);
MyWaitLoop(duration);}

// *************************************************************************************************

void StartBuzzerBeep() {
beep(NOTE_A4, 500);
beep(NOTE_A4, 500);    
beep(NOTE_A4, 500);
beep(NOTE_F4, 280);
beep(NOTE_C5, 250);  
beep(NOTE_A4, 500);
beep(NOTE_F4, 280);
beep(NOTE_C5, 250);
beep(NOTE_A4, 750);}

// *************************************************************************************************

void HourBuzzerBeep() {   
beep(NOTE_A4, 300);
beep(NOTE_F4, 300);
beep(NOTE_A4, 300);}

// *************************************************************************************************

void GMCMapAlarmBeep() {  
beep(NOTE_C5, 500);
beep(NOTE_C5, 500);
beep(NOTE_C5, 500);
beep(NOTE_C5, 500);
beep(NOTE_C5, 500);
beep(NOTE_C5, 500);}

// *************************************************************************************************

void StarWarsBuzzerBeep() {
beep(NOTE_A4, 500);
 beep(NOTE_A4, 500);    
  beep(NOTE_A4, 500);
   beep(NOTE_F4, 350);
    beep(NOTE_C5, 150);  
     beep(NOTE_A4, 500);
      beep(NOTE_F4, 350);
       beep(NOTE_C5, 150);
        beep(NOTE_A4, 650);
 
MyWaitLoop(500);
 
beep(NOTE_E5, 500);
 beep(NOTE_E5, 500);
  beep(NOTE_E5, 500);  
   beep(NOTE_F5, 350);
    beep(NOTE_C5, 150);
     beep(NOTE_GS4, 500);
      beep(NOTE_F4, 350);
       beep(NOTE_C5, 150);
        beep(NOTE_A4, 650);
        
MyWaitLoop(500);

beep(NOTE_A5, 500);
 beep(NOTE_A4, 300);
  beep(NOTE_A4, 150);
   beep(NOTE_A5, 500);
    beep(NOTE_GS5, 325);
     beep(NOTE_G5, 175);
      beep(NOTE_FS5, 125);
       beep(NOTE_F5, 125);    
        beep(NOTE_FS5, 250);
        
MyWaitLoop(325);

beep(NOTE_AS4, 250);
 beep(NOTE_DS5, 500);
  beep(NOTE_D5, 325);  
   beep(NOTE_CS5, 175);  
    beep(NOTE_C5, 125);  
     beep(NOTE_AS4, 125);  
      beep(NOTE_C5, 250);  
 
MyWaitLoop(350);}

// *************************************************************************************************
// *************************************************************************************************