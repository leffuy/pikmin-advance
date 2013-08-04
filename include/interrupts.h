#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define REG_IE  (*(unsigned short*) 0x4000200)

#define IRQ_VBLANK (1 << 0)
#define IRQ_HBLANK (1 << 1)
#define IRQ_VCOUNT (1 << 2)
#define IRQ_TIMER0 (1 << 3)
#define IRQ_TIMER1 (1 << 4)
#define IRQ_TIMER2 (1 << 5)
#define IRQ_TIMER3 (1 << 6)
#define IRQ_COM    (1 << 7)
#define IRQ_DMA0   (1 << 8)
#define IRQ_DMA1   (1 << 9)
#define IRQ_DMA2   (1 << 10)
#define IRQ_DMA3   (1 << 11)
#define IRQ_KEYS   (1 << 12)
#define IRQ_CART   (1 << 13)

//when an interrupt occurs, a bit is flipped in REG_IF to tell you which one
#define REG_IF  (*(unsigned short*) 0x4000202)

//interrupt master enable.  set to 1 or 0 to enable or disable interrupts
#define REG_IME (*(unsigned short*) 0x4000208)

// Location of the interrupt service routine
#define REG_INTERRUPT (*((int *) 0x03007FFC))

//Function prototype
void interruptHandler(void);

#endif
