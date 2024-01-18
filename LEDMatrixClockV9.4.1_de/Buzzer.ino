/********************************************************
*            Passiv Piezo Buzzer KY-006                 *
*********************************************************/

// passiv buzzer melody: (Big Ben)
int melody[] = {
NOTE_E3, NOTE_GS3, NOTE_FS3, NOTE_B2, 0,
NOTE_E3, NOTE_FS3, NOTE_GS3, NOTE_E3, 0,
NOTE_GS3, NOTE_E3, NOTE_FS3, NOTE_B2, 0,
NOTE_B2, NOTE_FS3, NOTE_GS3, NOTE_E3
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
4,4,4,2.5,4,
4,4,4,2.5,4,
4,4,4,2.5,4,
4,4,4,2.5};

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

void BigBenBeep() {
beep(NOTE_E3, 400);
beep(NOTE_GS3, 400);   
beep(NOTE_FS3, 400);
beep(NOTE_B2, 400);
beep(0, 500);
beep(NOTE_E3, 400);
beep(NOTE_FS3, 400);
beep(NOTE_GS3, 400);
beep(NOTE_E3, 400);
beep(0, 500);
beep(NOTE_GS3, 400);
beep(NOTE_E3, 400);
beep(NOTE_FS3, 400);
beep(NOTE_B2, 400);
beep(0, 500);
beep(NOTE_B2, 400);
beep(NOTE_FS3, 400);
beep(NOTE_GS3, 400);
beep(NOTE_E3, 400);}

// *************************************************************************************************

void HourBuzzerBeep() {   
beep(NOTE_A4, 300);
beep(NOTE_F4, 300);
beep(NOTE_A4, 300);}

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