/*
 * DTEK0068 Embedded Microprocessor Systems
 * Week 44 exercise
 * Simple LED and switch application for the ATmega4809 PCB.
 */

#define F_CPU   3333333

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

uint8_t count = 0;

void pwm_period(uint8_t duty)
{
    PORTA.OUTSET = PIN2_bm;
    for (uint8_t i = 0; i < duty; i++)
    {
        _delay_us(100);
    }
    PORTA.OUTCLR = PIN2_bm;
    for (uint8_t i = 0; i < (9 - duty); i++)
    {
        _delay_us(100);
    }
}

void set(uint8_t value)
{
    switch (value)
    {
        case 0 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm| PIN4_bm | PIN5_bm;
            PORTC.OUTCLR = PIN6_bm;
            break;
        case 1 :
            PORTC.OUTSET = PIN1_bm | PIN2_bm ;
            PORTC.OUTCLR = PIN0_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm;
            break;
        case 2 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN3_bm | PIN4_bm | PIN6_bm;
            PORTC.OUTCLR = PIN2_bm | PIN5_bm;
            break;
        case 3 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm| PIN6_bm;
            PORTC.OUTCLR = PIN5_bm | PIN4_bm;
            break;
        case 4 :
            PORTC.OUTSET = PIN1_bm | PIN2_bm | PIN5_bm | PIN6_bm;
            PORTC.OUTCLR = PIN0_bm | PIN3_bm | PIN4_bm;
            break;
        case 5 :
            PORTC.OUTSET = PIN0_bm | PIN2_bm | PIN3_bm | PIN5_bm | PIN6_bm;
            PORTC.OUTCLR = PIN1_bm | PIN4_bm;
            break;
        case 6 :
            PORTC.OUTSET = PIN0_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm;
            PORTC.OUTCLR = PIN1_bm;
            break;
        case 7 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm;
            PORTC.OUTCLR = PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm;
            break;
        case 8 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                           PIN4_bm | PIN5_bm | PIN6_bm;
            break;
        case 9 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                           PIN5_bm | PIN6_bm;
            PORTC.OUTCLR = PIN4_bm;
            break;
    }
}

int main(void) {
    
    PORTF.DIRCLR = PIN6_bm;
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    
    PORTA.DIRSET = PIN2_bm | PIN3_bm;
    PORTA.OUTCLR = PIN2_bm | PIN3_bm;
    
    set(0);
    
    // PF6 configured to trigger an interrupt when pushing down
    // or releasing the button.
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
  
    // Set sleep mode to Power-down.
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    // enable interrupts
    sei();
    // The idle loop.
    while (1)
    {
        //  Keep re-entering sleep mode after each interrupt wake-up
        // sleep_mode();
        //pwm_period(count);
    }
}

// The interrupt routine
ISR(PORTF_PORT_vect)
{
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xff;
    count++;
    if (count > 9)
    {
        count = 0;
    }
    set(count);
}
