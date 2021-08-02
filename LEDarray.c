#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{
    TRISGbits.TRISG0 = 0;
    TRISGbits.TRISG1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISFbits.TRISF6 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISFbits.TRISF0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    //set up TRIS registers for pins connected to LED array
	
    LATGbits.LATG0 = 0;
    LATGbits.LATG1 = 0;
    LATAbits.LATA2 = 0;
    LATFbits.LATF6 = 1;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATFbits.LATF0 = 1;
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
    //set initial output LAT values (they may have random values when powered on)
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number)
{
    //some code to turn on/off the pins connected to the LED array
	if (number & 0b000000001) {
        LATGbits.LATG0 = 1;
    }
    else {
        LATGbits.LATG0 = 0;
    }
    
    if (number & 0b000000010) {
        LATGbits.LATG1 = 1;
    }
    else{
        LATGbits.LATG1 = 0;
    }
    
    if (number & 0b000000100) {
        LATAbits.LATA2 = 1;
    }
    else {
        LATAbits.LATA2 = 0;
    }
    
    if (number & 0b000001000) {
        LATFbits.LATF6 = 1;
    }
    else {
        LATFbits.LATF6 = 0;
    }
    
    if (number & 0b000010000) {
        LATAbits.LATA4 = 1;
    }
    else {
        LATAbits.LATA4 = 0;
    }
    
    if (number & 0b000100000) {
        LATAbits.LATA5 = 1;
    }
    else {
        LATAbits.LATA5 = 0;
    }
    
    if (number & 0b001000000) {
        LATFbits.LATF0 = 1;
    }
    else {
        LATFbits.LATF0 = 0;
    }
    
    if (number & 0b010000000) {
        LATBbits.LATB0 = 1;
    }
    else {
        LATBbits.LATB0 = 0;
    }
    
    if (number & 0b100000000) {
        LATBbits.LATB1 = 1;
    }
    else {
        LATBbits.LATB1 = 0;
    }
    //if statements and bit masks can be used to determine if a particular pin should be on/off
    //see Readme.md for examples
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number){
	unsigned int disp_val;
	
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    if (number >= 10 && number < 20) {
        disp_val = 2;
    }
    
    else if (number >= 20 && number < 30) {
        disp_val = 4;
    }
    
    else if (number >= 30 && number < 40) {
        disp_val = 8;
    }
    
    else if (number >= 40 && number < 50) {
        disp_val = 16;
    }
    
    else if (number >= 50 && number < 60) {
        disp_val = 32;
    }
    
    else if (number >= 60 && number < 70) {
        disp_val = 64;
    }
    
    else if (number >= 70 && number < 80) {
        disp_val = 128;
    }
    
    else if (number >= 80 && number < 90) {
        disp_val = 256;
    }
    
    else if (number >= 90 && number < 100) {
        disp_val = 512;
    }
    
    else {
        disp_val = 0;
    }
 	LEDarray_disp_bin(disp_val);//display value on LED array
}

void LEDarray_disp_asc(unsigned int number){
	unsigned int disp_val;
	
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    if (number >= 10 && number < 20) {
        disp_val = 1;
    }
    
    else if (number >= 20 && number < 30) {
        disp_val = 3;
    }
    
    else if (number >= 30 && number < 40) {
        disp_val = 7;
    }
    
    else if (number >= 40 && number < 50) {
        disp_val = 15;
    }
    
    else if (number >= 50 && number < 60) {
        disp_val = 31;
    }
    
    else if (number >= 60 && number < 70) {
        disp_val = 63;
    }
    
    else if (number >= 70 && number < 80) {
        disp_val = 127;
    }
    
    else if (number >= 80 && number < 90) {
        disp_val = 255;
    }
    
    else if (number >= 90 && number < 100) {
        disp_val = 511;
    }
    
    else {
        disp_val = 0;
    }
    
 	LEDarray_disp_bin(disp_val);//display value on LED array
}

/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
	unsigned int disp_val = 0b111111111, max_val;
	
	// some code to format the variable current_val and previous max value, store in disp_val for display on the LED array
	// hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
    if (cur_val < 90) {
        disp_val = disp_val >> 1; // keep shifting the bits to the left by 1
    }    
    if (cur_val < 80) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 70) {
        disp_val = disp_val >> 1;
    }    
    if (cur_val < 60) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 50) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 40) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 30) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 20) {
        disp_val = disp_val >> 1;
    }
    if (cur_val < 10) {
        disp_val = disp_val >> 1;
    }
    
    if (max > 10) {
        max_val = 0b000000001;
    }
    
    if (max > 20) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 30) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 40) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 50) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 60) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 70) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 80) {
        max_val = max_val << 1; // logical shift up
    }
    
    if (max > 90) {
        max_val = max_val << 1; // logical shift up
    }
    
    
	LEDarray_disp_bin((disp_val) | (max_val));	//display value on LED array
}


