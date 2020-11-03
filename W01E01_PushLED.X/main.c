/*
 * DTEK0068 Embedded Microprocessor Systems
 * Week 44 exercise
 * Simple LED and switch application for the ATmega4809 PCB.
 */


#include <avr/io.h>


int main(void)
{
    // Set PF5 (LED) as out
    PORTF.DIRSET = PIN5_bm;
    PORTF.DIRCLR = PIN6_bm;
    
    // The superloop
    while (1)
    {
        if (PORTF.IN & PIN6_bm) {
            PORTF.OUTSET = PIN5_bm;    
        }
        else {
            PORTF.OUTCLR = PIN5_bm;
        }
    }
}
