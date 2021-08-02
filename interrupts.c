#include <xc.h>
#include <proc/pic18lf67k40.h>
#include "interrupts.h"
#include "timers.h"
#include "ADC.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
 */

void Interrupts_init(void)
{
    // turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.

    // INTCONbits.PEIE = 1; // enable low-priority interrupts
    IPR0bits.TMR0IP = 1;
    IPR2bits.C1IP = 0;
    PIE2bits.C1IE = 1;
    PIE0bits.TMR0IE = 1;
    INTCONbits.GIE = 1; // enable interrupts globally
    INTCONbits.PEIE = 1;
    INTCONbits.IPEN = 1;    
}
unsigned int second = 0, hour = 0, day = 1, week = 0b1000000, year = 0;
unsigned int clkFwd, clkBwd;

/************************************
@@ -17,6 +22,13 @@ void Interrupts_init(void)
************************************/
void __interrupt(low_priority) LowISR()
{
    if (PIR2bits.C1IF) {
        LATHbits.LATH3 =~ LATHbits.LATH3;
        PIR2bits.C1IF = 0;
    }
}
    
void __interrupt(high_priority) HighISR() 
{
    if (PIR0bits.TMR0IF) {
        second++;
        if (second % 3600 == 0) {
            hour++;
                if (hour == 24) {
                    day++;
                    hour = 0;
                }
            }
        }
    
        
        


    if (day >= clkFwd && day < clkBwd) {
        
        if (hour >= 5 && hour < 1) {
        PIE2bits.C1IE = 1;
        LATHbits.LATH3 = 1;
        }
        
        if (hour >= 2 && hour < 6) {
        PIE2bits.C1IE = 0;
        LATHbits.LATH3 = 0;
        }
    }
        
    else {
        if (hour >= 5 && hour < 1) {
        PIE2bits.C1IE = 1;
        LATHbits.LATH3 = 1;
        }
    if (hour >= 1 && hour < 5) {
        PIE2bits.C1IE = 0;
        LATHbits.LATH3 = 0;
    }
    }

        TMR0H = 0b00001011;
        TMR0L = 0b11011011;
        PIR0bits.TMR0IF = 0;
}
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...

// code