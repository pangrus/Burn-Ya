/*
            (       )         )         
   (        )\ ) ( /(      ( /(  (      
 ( )\    ( (()/( )\())     )\()) )\     
 )((_)   )\ /(_)|(_)\ ___ ((_)((((_)(   
((_)_ _ ((_|_))  _((_)___|_ ((_)\ _ )\  
 | _ ) | | | _ \| \| |   \ \ / (_)_\(_) 
 | _ \ |_| |   /| .` |    \ V / / _ \   
 |___/\___/|_|_\|_|\_|     |_| /_/ \_\  
                                        
 Burn-Ya sound generator
 Hardware based on ATtiny Multiboard
 https://github.com/pangrus/ATtiny_Multiboard
 Pangrus 2018
*/

char burn;
char ya;
char parameter1 = 0;
char parameter2 = 0;
int potentiometer1 = 0;
int potentiometer2 = 0;

void setup() {
  PLLCSR = 1 << PCKE | 1 << PLLE;               // Enable 64 MHz PLL and use as source for Timer1
  TIMSK = 0;                                    // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
  TCCR0A = 3 << WGM00;                          // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;              // 1/8 prescale
  TIMSK = 1 << OCIE0A;                          // Enable compare match
  pinMode(1, OUTPUT);
}

void loop() {
  potentiometer1 = analogRead(A2);
  potentiometer2 = analogRead(A3);
  parameter1 = map(potentiometer1, 0, 1023, 5, 100);
  parameter2 = map(potentiometer2, 0, 1023, 1, 190);
  OCR0A = map(potentiometer2, 0, 1023, 213, 1);
}

// Interrupt service routine
ISR(TIMER0_COMPA_vect) {
  burn = burn + parameter1;
  ya = ya + parameter2;
  OCR1A = (((burn ^ ya) >> (parameter1 / parameter2)) ^ (parameter2)); //Formula defined waveform
}

