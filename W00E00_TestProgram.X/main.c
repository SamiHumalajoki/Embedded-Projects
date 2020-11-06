/*
 * Simple LED blinking application for testing that the ATmega4809 PCB
 * can be programmed with the course specific virtual machine.
 */

#define F_CPU   3333333

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    VPORTB.DIR |= PIN2_bm;
    
    // The superloop
    while (1)
    if (VPORTB.OUT & PIN2_bm)
{
    VPORTB.OUT &= ~PIN2_bm;
}
else
{
    VPORTB.OUT |= PIN2_bm;
}
}
