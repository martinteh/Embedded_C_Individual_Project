#include <xc.h>
#include <proc/pic18lf67k40.h>
#include "interrupts.h"
#include "timers.h"
#include <math.h>

unsigned int second = 0, hour = 0, day = 1, year = 0; // set the second of the day, the hour of day, the day of year and year
unsigned int clockFwd, clockBwd; // sets the number of days to when clocks go back and forward

void Interrupts_init(void) // initiate the interrupts
{
    IPR0bits.TMR0IP = 1; // high priority timer interrupt
    IPR2bits.C1IP = 0; // low priority comparator interrupt
    PIE2bits.C1IE = 1; // enable the peripheral interrupt for comparator
    PIE0bits.TMR0IE = 1; // enable peripheral interrupt for timer
    INTCONbits.GIE = 1; // enables global interrupt
    INTCONbits.PEIE = 1; // disables only the peripheral interrupt sources when GIE is set
    INTCONbits.IPEN = 1; // interrupt priority enable
}

void dayShift(int hour, int week, int year){
    if (hour == 24) { // after 24 hours
            day++; // add one to integer "day"
            hour = 0; // reset hour to zero
            if (week & 0b0000001) {  // if Sunday
                week = 0b1000000; // reset the week to Monday
                }
            else {
                week = week >> 1; // otherwise shift the day of the week to the next day
            }
    }
}
void daylightSaving(int firstSunday, int year, int week) {
    if (day == 1) { // if first day of the year, check if leap year
            if (year % 4 == 0) { // if leap year
                firstSunday = (log(week)/log(2)) - 1; // number of days from 1st Jan to first Sunday is log_2(day_of_week)
                clockFwd = firstSunday + 91; // number of days to final Sunday in March is no. of days to first Sunday + 91, clocks go forward
                clockBwd = firstSunday + 301; // number of days to final Sunday in October is no. of days to first Sunday + 301, clocks go back
            }
            else { // if not leap year
                firstSunday = (log(week)/log(2)) - 1; // number of days from 1st Jan to first Sunday is log_2(day_of_week)
                clockFwd = firstSunday + 92;    // number of days to final Sunday in March is no. of days to first Sunday + 92, clocks go forward
                clockBwd = firstSunday + 302;   // number of days to final Sunday in October is no. of days to first Sunday + 302, clocks go back
            }    
        }
}

void leapYear(int year, int day, int second) {
    if (year % 4 == 0) { // if leap year
            if (day == 367) { // reset the day to one on day 367
                day = 1;
                second = 0;
            }
        }
    else { // not a leap year
            if (day == 366) { // reset the day to one on day 366
                day = 1;
                second = 0;
            }
        }
}

/************************************
@@ -17,6 +22,13 @@ void Interrupts_init(void)
************************************/
int lightData, highLow = 1; // lightData is the current LAT value as the interrupt is flagged
                            // highLow is 1 when between 1am and 5am and 0 otherwise
                            // this is useful for after sunrise (ambient light changes), the LAT value returns to the previous one before which is stored in lightData
                            // this keeps the LED in sync

void __interrupt(low_priority) LowISR() { // low priority ISR
    if (PIR2bits.C1IF) { // if interrupt is flagged
        lightData =~ lightData; // toggle LED based on current LED state
        if (highLow == 1) { // if between high and low
            LATHbits.LATH3 = lightData; // LAT value returns to previous value
        }
        PIR2bits.C1IF = 0; // interrupt flag off
    }
}
    
void __interrupt(high_priority) HighISR() { // high priority interrupt
    if (PIR0bits.TMR0IF) { // if timer interrupt flagged
        second++; // increase second tally by one each second
        if (second % 3600 == 0) { // if second/3600 has no remainders
            hour++; // add one to hour tally
        }
        if (day >= clockFwd && day < clockBwd) { // if the day is in the daylight saving time period
            if (hour >= 5 || hour < 1) { // 5am -> 1am
                PIE2bits.C1IE = 1; // interrupt init.
                highLow = 1; // set highLow to 1
                LATHbits.LATH3 = lightData; // record the LED state
                }
        
            if (hour >= 2 && hour < 6) { // between 1am and 5am BST
                PIE2bits.C1IE = 0; // interrupt off
                highLow = 0; // set highLow to 0
                LATHbits.LATH3 = 0; // set LED state to 0
                }
        }
        
        else { // outside daylight saving time
            if (hour >= 5 || hour < 1) { // 5am -> 1am
                PIE2bits.C1IE = 1; // interrupt on
                highLow = 1; // set highLow to 1
                LATHbits.LATH3 = lightData; // record the LED state
            }
            if (hour >= 1 && hour < 5) { // between 1am and 5am GMT
                PIE2bits.C1IE = 0; // interrupt off
                highLow = 0; // set highLow to 0
                LATHbits.LATH3 = 0; // set LED state to 0
            }   
        }
    }
    TMR0H = 0b00001011; // reset the timer interrupt after it flows to 1 sec
    TMR0L = 0b11011011;
    PIR0bits.TMR0IF = 0; // turn timer interrupt to 0
}