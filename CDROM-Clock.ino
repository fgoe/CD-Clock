

//=========================================================
//
// GoFlo CDROMclock 1.0
// -----------------------------------
//
//
// GoFlo 22.05.2015
//
//
// ino File for a clock using CDROM drives for displaying the time.
//
// Updates:
// ----------
// - none yet
//
//==========================================================


// Include libraries DCF and Time
#include <DCF77.h>
#include <Time.h>

// Eingangspin für den DCF Empfänger
// Achtung: Bei Änderung des PINs auf den entsprechenden Interrupt achten oder neu setzen
// mittels attachInterrupt
#define DCF_PIN 2

// Interrupt for the receiver
#define DCF_INTERRUPT 0
time_t time;


// Should we show debug data?
boolean debug = true;

// Receiver has syncronized?
boolean synced = false;

// Clock set for the first time
boolean firstset = false;

// PINs for lighting
int LEDlinks = 5;
int LEDrechts = 6;

// Define variables for arduino outputs and set PIN number
// Hour tens digit
int SZ_O = 44; 
int SZ_M = 48;
int SZ_U = 46;
int SZ_OL = 43;
int SZ_OR = 45;
int SZ_UL = 49;
int SZ_UR = 47;

// Hour single digit
int SE_O = 22; 
int SE_M = 25;
int SE_U = 26;
int SE_OL = 23;
int SE_OR = 24;
int SE_UL = 42;
int SE_UR = 27;

// MIN tens digit
int MZ_O = 36; 
int MZ_M = 30;
int MZ_U = 34;
int MZ_OL = 32;
int MZ_OR = 38;
int MZ_UL = 40;
int MZ_UR = 28;

// MIN single digit
int ME_O = 37; 
int ME_M = 35;
int ME_U = 41;
int ME_OL = 29;
int ME_OR = 31; 
int ME_UL = 39;
int ME_UR = 33;

// Arrays for a 7 Segment display
int STD_ZEHNER[] = { SZ_O, SZ_M, SZ_U, SZ_OL, SZ_OR, SZ_UL, SZ_UR };
int STD_EINER[] = { SE_O, SE_M, SE_U, SE_OL, SE_OR, SE_UL, SE_UR };
int MIN_ZEHNER[] = { MZ_O, MZ_M, MZ_U, MZ_OL, MZ_OR, MZ_UL, MZ_UR };
int MIN_EINER[] = { ME_O, ME_M, ME_U, ME_OL, ME_OR, ME_UL, ME_UR };

// Initialize clock variables
int tmphour = 0;
int tmpminute = 0; 

int tmphourTen = 0; 
int tmphourTenLast = 0;
int tmphourOne = 0; 
int tmphourOneLast = 0;
int tmpminuteTen = 0;  
int tmpminuteTenLast = 0;
int tmpminuteOne = 0;
int tmpminuteOneLast = 0;


// Create new DCF Object
DCF77 funkuhr=DCF77(DCF_PIN,DCF_INTERRUPT,true);




// Arduino Setup
// -----------------------------------------------------
void setup() {                

  // Start serial
  Serial.begin(9600);

  // Output to serial console
  Serial.println("-- CDROMclock 1.0 by GoFlo -- "); 
  Serial.println("---------------------------- "); 
  Serial.println(" "); 
  Serial.println("-- Setup wird durchgefuehrt -- "); 
  Serial.println("    . Variablen initialisiert");
  
  // LED Pins als Outputs festlegen
  pinMode(SZ_O, OUTPUT); 
  pinMode(SZ_M, OUTPUT); 
  pinMode(SZ_U, OUTPUT); 
  pinMode(SZ_OL, OUTPUT); 
  pinMode(SZ_OR, OUTPUT); 
  pinMode(SZ_UL, OUTPUT); 
  pinMode(SZ_UR, OUTPUT); 
  pinMode(SE_O, OUTPUT); 
  pinMode(SE_M, OUTPUT); 
  pinMode(SE_U, OUTPUT); 
  pinMode(SE_OL, OUTPUT); 
  pinMode(SE_OR, OUTPUT); 
  pinMode(SE_UL, OUTPUT); 
  pinMode(SE_UR, OUTPUT); 
  pinMode(MZ_O, OUTPUT); 
  pinMode(MZ_M, OUTPUT);
  pinMode(MZ_U, OUTPUT);
  pinMode(MZ_OL, OUTPUT);
  pinMode(MZ_OR, OUTPUT);
  pinMode(MZ_UL, OUTPUT);
  pinMode(MZ_UR, OUTPUT);
  pinMode(ME_O, OUTPUT);
  pinMode(ME_M, OUTPUT);
  pinMode(ME_U, OUTPUT);
  pinMode(ME_OL, OUTPUT);
  pinMode(ME_OR, OUTPUT);
  pinMode(ME_UL, OUTPUT);
  pinMode(ME_UR, OUTPUT);
  pinMode(LEDlinks, OUTPUT);
  pinMode(LEDrechts, OUTPUT);
    
  Serial.println("    . Pinmodes festgelegt"); 
 
  // Set ouput pins to LOW / switch off
  digitalWrite(SZ_O, LOW);
  digitalWrite(SZ_M, LOW);
  digitalWrite(SZ_U, LOW);
  digitalWrite(SZ_OL, LOW);
  digitalWrite(SZ_OR, LOW);
  digitalWrite(SZ_UL, LOW);
  digitalWrite(SZ_UR, LOW);
  digitalWrite(SE_O, LOW);
  digitalWrite(SE_M, LOW);
  digitalWrite(SE_U, LOW);
  digitalWrite(SE_OL, LOW);
  digitalWrite(SE_OR, LOW);
  digitalWrite(SE_UL, LOW);
  digitalWrite(SE_UR, LOW);
  digitalWrite(MZ_O, LOW);
  digitalWrite(MZ_M, LOW);
  digitalWrite(MZ_U, LOW);
  digitalWrite(MZ_OL, LOW);
  digitalWrite(MZ_OR, LOW);
  digitalWrite(MZ_UL, LOW);
  digitalWrite(MZ_UR, LOW);
  digitalWrite(ME_O, LOW);
  digitalWrite(ME_M, LOW);
  digitalWrite(ME_U, LOW);
  digitalWrite(ME_OL, LOW);
  digitalWrite(ME_OR, LOW);
  digitalWrite(ME_UL, LOW);
  digitalWrite(ME_UR, LOW);
  digitalWrite(LEDlinks, LOW);
  digitalWrite(LEDrechts, LOW);
      
  Serial.println("    . Alle Ausgaenge auf LOW");
    
    
  // Uhr starten
  funkuhr.Start();
  Serial.println("    . Uhr starten");
  
  // Turn on lighting
  for (int i=0; i <= 255; i++) {
         LED_dim(i);
         delay(20);
  } 
   
  
  // Output Setup done
  Serial.println("-- Setup done -- "); 
  Serial.println(" "); 
    
}  //End void setup()



// Berechnung der Uhrzeit
// ----------------------------------------------------------
void readclock() {
  
  // Zeit aus Empfänger lesen
  time_t DCFtime = funkuhr.getTime();
  
  // Neues Update empfangen
  if (DCFtime!=0) {
      Serial.println("Time is updated");
      setTime(DCFtime);
      if (synced != true) {
          synced = true;
      }
    
      // In Zwischenvariablen kopieren
      tmphour = hour();
      tmpminute = minute();
  
      // Stundenwert aufteilen
      tmphourTen = tmphour/10; 
      tmphourOne = tmphour/10;
  
      // Minutenwert aufteilen
      tmpminuteTen = tmphour/10;
      tmpminuteOne = tmphour/10;
  
  }
  
  // Kein Signal empfangen
  else {
    Serial.println("No valid time update yet");
  }
  
  if (debug) {
    // Zeit auf Konsole ausgeben
    Serial.print("Uhrzeit:   ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(month());
    Serial.print(" ");
    Serial.print(year());
    Serial.println(); 
    Serial.print("Stunde Zehner:   ");
    Serial.print(tmphourTen);
    Serial.print("    Stunde Einer:   ");
    Serial.print(tmphourOne);
    Serial.println();
    Serial.print("Minute Zehner:    ");
    Serial.print(tmpminuteTen);
    Serial.print("    Minute Einer:   ");
    Serial.print(tmpminuteOne);
    Serial.println(); 
    delay(1000);
  }
  
}  // Ende readclock()





// Zahlanpassung für Konsole
// ---------------------------------------------------
void printDigits(int digits){
  
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
  
}  // Ende printDigits()


// LEDs an
// ----------------------------------------------------------------
void LED_on() {
   // LEDs anschalten
   digitalWrite(LEDlinks, HIGH);
   digitalWrite(LEDrechts, HIGH); 
   
}   // Ende LED_on()


// LEDs aus
// ----------------------------------------------------------------
void LED_off() {
   // LEDs anschalten
   digitalWrite(LEDlinks, LOW);
   digitalWrite(LEDrechts, LOW);
   
}    // Ende LED_off()
 
 
// LED dimmen
// ----------------------------------------------------------------
void LED_dim(int value) {
   digitalWrite(LEDlinks, value);
   digitalWrite(LEDrechts, value);
   
}     // Ende LED_dim()


// Initiales Setzen des Displays
// ----------------------------------------------------------------
// Auf 0 setzen
void set_0(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[5], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[5], LOW);
   digitalWrite(digit[6], HIGH);
}

// Auf 1 setzen
void set_1(int digit[]) {
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 2 setzen
void set_2(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[5], LOW);
}
 
// Auf 3 setzen
void set_3(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 4 setzen
void set_4(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 5 setzen
void set_5(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 6 setzen
void set_6(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[5], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[5], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 7 setzen
void set_7(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 8 setzen
void set_8(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[5], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[5], LOW);
   digitalWrite(digit[6], LOW);
}

// Auf 9 setzen
void set_9(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}   

// Nach der 9 alle Laufwerkschliessen für Funktionstest
void resetDIS(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
}



//  ZAHLENWECHSEL ANZEIGE
// ----------------------------------------------------------------

// Wechsel: Von 0 auf 1 
void to_1(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[5], LOW);
   
} 

// Wechsel: Von 1 auf 2 
void to_2(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[5], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000); 
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[5], LOW);
   digitalWrite(digit[6], LOW);
} 

// Wechsel: Von 2 auf 3 
void to_3(int digit[]) {
   digitalWrite(digit[5], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[5], LOW);
   digitalWrite(digit[6], LOW);
} 

// Wechsel: Von 3 auf 4 
void to_4(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
} 

// Wechsel: Von 4 auf 5 
void to_5(int digit[]) {
   digitalWrite(digit[0], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000);
   digitalWrite(digit[0], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[6], LOW);
} 

// Wechsel: Von 5 auf 6 
void to_6(int digit[]) {
   digitalWrite(digit[5], HIGH);
   delay(1000);
   digitalWrite(digit[5], LOW);
} 

// Wechsel: Von 6 auf 7 
void to_7(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[5], LOW);
} 

// Wechsel: Von 7 auf 8 
void to_8(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[2], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000);
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[2], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[5], LOW);
} 

// Wechsel: Von 8 auf 9 
void to_9(int digit[]) {
   digitalWrite(digit[5], HIGH);
   delay(1000); 
   digitalWrite(digit[5], LOW);
} 

// Wechsel: Von 9 auf 0 
void to_0(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000); 
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[5], LOW);
} 

// Sonderfälle
// Wechsel von 5 auf 0 für Minuten-Zehner
void f5_to_0(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[4], HIGH);
   digitalWrite(digit[5], HIGH);
   delay(1000); 
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[4], LOW);
   digitalWrite(digit[5], LOW);
} 

// Wechsel von 2 auf 0 für Stunden-Zehner
void f2_to_0(int digit[]) {
   digitalWrite(digit[1], HIGH);
   digitalWrite(digit[3], HIGH);
   digitalWrite(digit[6], HIGH);
   delay(1000); 
   digitalWrite(digit[1], LOW);
   digitalWrite(digit[3], LOW);
   digitalWrite(digit[6], LOW);
} 


// Test der Anzeige
// ----------------------------------------------------------------
void testdisplay() {
  
   // Initiale Verzögerung, falls Laufwerk noch von Hand in Grundstellung gefahren
   // werden müssen
   delay(10000);
   
   // LEDs blinken lassen um Start zu signalisieren
   for (int i=0; i <= 5; i++) {
         LED_off();
         delay(300);
         LED_on();
   } 
    
   // Alle Digits von 0 bis 9 durchschalten, danach wieder alle ROMs einfahren
   set_0(STD_ZEHNER);
   set_0(STD_EINER); 
   set_0(MIN_ZEHNER); 
   set_0(MIN_EINER); 
   delay(15000);
   
   to_1(STD_ZEHNER);
   to_1(STD_EINER); 
   to_1(MIN_ZEHNER); 
   to_1(MIN_EINER);
   delay(15000);
   
   to_2(STD_ZEHNER);
   to_2(STD_EINER); 
   to_2(MIN_ZEHNER); 
   to_2(MIN_EINER);
   delay(15000);
   
   to_3(STD_ZEHNER);
   to_3(STD_EINER); 
   to_3(MIN_ZEHNER); 
   to_3(MIN_EINER);
   delay(15000);
   
   to_4(STD_ZEHNER);
   to_4(STD_EINER); 
   to_4(MIN_ZEHNER); 
   to_4(MIN_EINER);
   delay(15000);
   
   to_5(STD_ZEHNER);
   to_5(STD_EINER); 
   to_5(MIN_ZEHNER); 
   to_5(MIN_EINER);
   delay(15000);
   
   to_6(STD_ZEHNER);
   to_6(STD_EINER); 
   to_6(MIN_ZEHNER); 
   to_6(MIN_EINER);
   delay(15000);
   
   to_7(STD_ZEHNER);
   to_7(STD_EINER); 
   to_7(MIN_ZEHNER); 
   to_7(MIN_EINER);
   delay(15000);
   
   to_8(STD_ZEHNER);
   to_8(STD_EINER); 
   to_8(MIN_ZEHNER); 
   to_8(MIN_EINER);
   delay(15000);
   
   to_9(STD_ZEHNER);
   to_9(STD_EINER); 
   to_9(MIN_ZEHNER); 
   to_9(MIN_EINER);
   delay(15000);
   
   resetDIS(STD_ZEHNER);
   resetDIS(STD_EINER); 
   resetDIS(MIN_ZEHNER); 
   resetDIS(MIN_EINER);
   delay(15000);
   
}


// Stundenanzeige Zehner steuern
// ----------------------------------------------------------
void setHourTen() {
  // Wenn sich die Stunden verändert haben 
  if ( tmphourTen != tmphourTenLast ) {
     // und die Uhr schon initial verstellt wurde
     if (!firstset) {
       if (tmphourTenLast == 0) {
         to_1(STD_ZEHNER);
       }
       if (tmphourTenLast == 1) {
         to_2(STD_ZEHNER);
       } 
       if (tmphourTenLast == 2) {
         f2_to_0(STD_ZEHNER);
       }   
     }
     
     // Uhr wurde noch nie gestellt
     else {
       //set to auf aktuelle Uhrzeit
       if (tmphourTen == 0) {
         set_0(STD_ZEHNER);
       }
       if (tmphourTen == 1) {
         set_1(STD_ZEHNER);
       }
       if (tmphourTen == 2) {
         set_1(STD_ZEHNER);
       }
     }
   }      
}   

// Stundenanzeige Einer steuern
// ----------------------------------------------------------
void setHourOne() {
// Wenn sich die Stunden verändert haben
     if ( tmpminuteTen != tmpminuteTenLast) {
        // und die Uhr schon initial verstellt wurde
        if (!firstset) {
            if (tmphourOneLast == 0) {
              to_1(STD_EINER);
            }
            if (tmphourOneLast == 1) {
              to_2(STD_EINER);
            }
            if (tmphourOneLast == 2) {
              to_3(STD_EINER);
            }
            if (tmphourOneLast == 3) {
              to_4(STD_EINER);
            }
            if (tmphourOneLast == 4) {
              to_5(STD_EINER);
            }
            if (tmphourOneLast == 5) {
              to_6(STD_EINER);
            }
            if (tmphourOneLast == 6) { 
              to_7(STD_EINER);
            }
            if (tmphourOneLast == 7) {
              to_8(STD_EINER);
            }
            if (tmphourOneLast == 8) {
              to_9(STD_EINER);
            }  
            if (tmphourOneLast == 9) {
              to_0(STD_EINER);
            }
        }
        
        // Uhr wurde noch nie gestellt
        else {
          //set to auf aktuelle Uhrzeit
          if (tmpminuteOne == 1) {
             set_1(STD_EINER);
          }
          if (tmpminuteOne == 2) {
             set_2(STD_EINER);
          }
          if (tmpminuteOne == 3) {
             set_3(STD_EINER);
          }
          if (tmpminuteOne == 4) {
             set_4(STD_EINER);
          }
          if (tmpminuteOne == 5) {
             set_5(STD_EINER);
          }
          if (tmpminuteOne == 6) {
             set_6(STD_EINER);
          }
          if (tmpminuteOne == 7) {
             set_7(STD_EINER);
          }
          if (tmpminuteOne == 8) {
             set_8(STD_EINER);
          }
          if (tmpminuteOne == 9) {
             set_9(STD_EINER);
          }       
        }
     }
}  


// Minutenanzeige Zehner steuern
// ----------------------------------------------------------
void setMinuteTen() {
// Wenn sich die Minuten verändert haben
     if ( tmpminuteTen != tmpminuteTenLast) {
        // und die Uhr schon initial verstellt wurde
        if (!firstset) {
            if (tmpminuteTenLast == 0) {
              to_1(MIN_ZEHNER);
            }
            if (tmpminuteTenLast == 1) {
              to_2(MIN_ZEHNER);
            }
            if (tmpminuteTenLast == 2) {
              to_3(MIN_ZEHNER);
            }
            if (tmpminuteTenLast == 3) {
              to_4(MIN_ZEHNER);
            }
            if (tmpminuteTenLast == 4) {
              to_5(MIN_ZEHNER);
            }
            if (tmpminuteTenLast == 5) {
              f5_to_0(MIN_ZEHNER);
            }
        }
        
        // Uhr wurde noch nie gestellt
        else {
          //set to auf aktuelle Uhrzeit
          if (tmpminuteTen == 1) {
             set_1(MIN_ZEHNER);
          }
          if (tmpminuteTen == 2) {
             set_2(MIN_ZEHNER);
          }
          if (tmpminuteTen == 3) {
             set_3(MIN_ZEHNER);
          }
          if (tmpminuteTen == 4) {
             set_4(MIN_ZEHNER);
          }
          if (tmpminuteTen == 5) {
             set_5(MIN_ZEHNER);
          }
          if (tmpminuteTen == 0) {
             set_0(MIN_ZEHNER);
          }       
        }
     }
   
}  


// Minutenanzeige Einer steuern
// ----------------------------------------------------------
void setMinuteOne() {
     // Wenn sich die Minuten verändert haben
     if ( tmpminuteOne != tmpminuteOneLast) {
        // und die Uhr schon initial verstellt wurde
        if (!firstset) {
            if (tmpminuteOneLast == 0) {
              to_1(MIN_EINER);
            }
            if (tmpminuteOneLast == 1) {
              to_2(MIN_EINER);
            }
            if (tmpminuteOneLast == 2) {
              to_3(MIN_EINER);
            }
            if (tmpminuteOneLast == 3) {
              to_4(MIN_EINER);
            }
            if (tmpminuteOneLast == 4) {
              to_5(MIN_EINER);
            }
            if (tmpminuteOneLast == 5) {
              to_6(MIN_EINER);
            }
            if (tmpminuteOneLast == 6) { 
              to_7(MIN_EINER);
            }
            if (tmpminuteOneLast == 7) {
              to_8(MIN_EINER);
            }
            if (tmpminuteOneLast == 8) {
              to_9(MIN_EINER);
            }  
            if (tmpminuteOneLast == 9) {
              to_0(MIN_EINER);
            }
        }
        
        // Uhr wurde noch nie gestellt
        else {
          //set to auf aktuelle Uhrzeit
          if (tmpminuteOne == 1) {
             set_1(MIN_EINER);
          }
          if (tmpminuteOne == 2) {
             set_2(MIN_EINER);
          }
          if (tmpminuteOne == 3) {
             set_3(MIN_EINER);
          }
          if (tmpminuteOne == 4) {
             set_4(MIN_EINER);
          }
          if (tmpminuteOne == 5) {
             set_5(MIN_EINER);
          }
          if (tmpminuteOne == 6) {
             set_6(MIN_EINER);
          }
          if (tmpminuteOne == 7) {
             set_7(MIN_EINER);
          }
          if (tmpminuteOne == 8) {
             set_8(MIN_EINER);
          }
          if (tmpminuteOne == 9) {
             set_9(MIN_EINER);
          }       
        }
     }
}  




// Mainloop
// -----------------------------------------------------------
void loop() {
  
  // Test all segments in Debugmode
  if (debug) {
    testdisplay();
  }
  
  // Lighting on
  LED_on();
  
  // Calculate time
  readclock();
  
  // If clock is synced switch on the segments
  if (synced) {
    setHourTen();
    setHourOne();
    setMinuteTen();
    setMinuteOne();
  }
   
} // End Hauptloop 

