/*
 * Eco Lame Light
 * 
 * DTEK0068 Embedded Microprocessor Systems
 * Week 3 - Assignment 2
 * Author: Sami Humalajoki
 *
 * Created on 12 November 2020, 08:32
 */

#include <avr/interrupt.h>
#include <avr/sleep.h>

int main(void) {
    
    // Set PF5 (LED) as out
    PORTF.DIRSET = PIN5_bm;
  
    // Turn off the LED
    PORTF.OUTSET = PIN5_bm;
    
    // PF6 configured to trigger an interrupt when pushing down
    // or releasing the button.
    PORTF.PIN6CTRL = PORT_ISC_BOTHEDGES_gc;
  
    // Set IDLE sleep mode
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    // enable interrupts
    sei();
    // The idle loop.
    while (1)
    {
        //  Keep re-entering sleep mode after each interrupt wake-up
        sleep_mode();
    }
}

// The interrupt routine
ISR(PORTF_PORT_vect)
{
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xff;
    
    // Turn the LED on if the button is pressed.
    // Otherwise turn it off.
    if (!(PORTF.IN & PIN6_bm))
    {
        PORTF.OUTCLR = PIN5_bm;
    }
    else
    {
        PORTF.OUTSET = PIN5_bm;
    }
}
