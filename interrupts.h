#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000
void Interrupts_init(void);
void dayShift(int hour, int week, int year);
void daylightSaving(int firstSunday, int year, int week);
void leapYear(int year, int day, int second);
void __interrupt(high_priority) HighISR();
extern unsigned int second, hour, day, week, year;
extern unsigned int clockFwd, clockBwd;
void __interrupt(low_priority) LowISR();

#endif
