#include <xc.h>
#include "timers.h"
#include "LEDarray.h"

/************************************
 * Function to set up timer 0
************************************/
extern int count();

void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // 1:256 (PS = 64MHz/(65535*4) = 244.144)
    T0CON0bits.T016BIT=1;	//8bit mode
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=0b00001011;            //write High reg first, update happens when low reg is written to
    TMR0L=0b11011011;         
    
    // if prescaler set mode it 256
    // T = 1/Max value
    // PS = (1/max value)*(Fosc/4)
    // max value = Fosc/(4*PS)
    // max value = (64*10^6)/(4*256) = 62500
    // 65535 - 62500 = 3035
    // hence start at 3035 on the TMR0L register
    
    T0CON0bits.T0EN=1;	//start the timer
}

/************************************
 * Function to return the full 16bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
************************************/
unsigned int get16bitTMR0val(void)
{
	//add your code to get the full 16 bit timer value here
    unsigned int time = 0b0000000000000000;  // set a 16bit binary number zero
    time = time | TMR0L; // use logic OR of int time with TMR0L
    time = time | (TMR0H << 8); // use logic OR of the new int time with TMR0H
    return time; // return the 16 bit combined number from the two 8 bit registers
}
