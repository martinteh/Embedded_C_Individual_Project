// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <math.h>
#include "interrupts.h"
#include "comparator1.h"
#include "timers.h"
#include "LEDarray.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

// Energy Saving Light Project
// Martin Teh
// See the interrupts.c file for more detailed function breakdown
// Make sure to switch the system on in the dark!

// This system requires you to input the day of the week (main.c), second of minute (interrupt1.c), the hour of day (interrupt1.c) and the year (interrupt1.c). 

void main(void) {
    LATHbits.LATH3=1;   //set initial output state of RH3 LED
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    
    Interrupts_init();  // initialise the interrupt
    LEDarray_init();    // initialise the LED array
    Comp1_init();   // initialise the compiler
    Timer0_init();  // initialise the timer
    
    int week = 0b0001000; // input the initial day of the week (eg/ Mon = 0b1000000, Tues = 0b0100000, ...)
    int firstSunday; // how many days from day one of the year to the first Sunday of January

    while (1) {
        dayShift(hour, week, day); // bit-shifts the day of the week every 24 hours (after Sunday, it returns to Monday = 0b1000000)
        daylightSaving(firstSunday, year, week); // daylight saving time function
        leapYear(year, day, second); // leap year function
        LEDarray_disp_bin(hour); // displays the hour of the day in binary on the LED array
    }
}