/***********************
 *                     *
 *    Timer Stuff      *
 *                     *
 ***********************/
#define REG_TM0CNT *(volatile unsigned short*)0x4000102
#define REG_TM1CNT *(volatile unsigned short*)0x4000106
#define REG_TM2CNT *(volatile unsigned short*)0x400010A
#define REG_TM3CNT *(volatile unsigned short*)0x400010E

/* REG_TMxCNT Bits
0-1  Frequency
2    Overflow from previous timer?
3-5  Unused
6    Overflow generates interrupt?
7    Enable this timer
8-15 Unused */
#define TIMER_OVERFLOW 0x4
#define TIMER_IRQ_ENABLE 0x40
#define TIMER_ENABLE 0x80

#define TIMER_FREQUENCY_SYSTEM 0x0
#define TIMER_FREQUENCY_64 0x1
#define TIMER_FREQUENCY_256 0x2
#define TIMER_FREQUENCY_1024 0x3

//Holds the current value of the timer.
//Check it for value when polling.
//
//Set this to initial values to put into the timers
//to cause them to overflow at a predictable,
//desired number of ticks.
#define REG_TM0D *(volatile unsigned short*)0x4000100
#define REG_TM1D *(volatile unsigned short*)0x4000104
#define REG_TM2D *(volatile unsigned short*)0x4000108
#define REG_TM3D *(volatile unsigned short*)0x400010C
