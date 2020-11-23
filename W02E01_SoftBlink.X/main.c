/*
 * DTEK0068 Embedded Microprocessor Systems
 * Week 2, Exercise 1
 * Soft Blink 
 * Author: Sami Humalajoki
 */

#define F_CPU   3333333

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void pwm_period(uint8_t duty) 
{  
    PORTF.OUTCLR = PIN5_bm;
    for (uint8_t i = 0; i < duty; i++)
    {
        _delay_us(7);
    }
    PORTF.OUTSET = PIN5_bm;
    for (uint8_t i = 0; i < (UINT8_MAX - duty); i++)
    {
        _delay_us(7);
    }
}

int main(void)
{
    // Set PF5 (LED) as out
    PORTF.DIRSET = PIN5_bm;
    
    // The superloop
    while (1)
    {   
        for (uint8_t duty = 1; duty <= UINT8_MAX; duty++)
        {
            pwm_period(duty);
        }
        for (uint8_t duty = UINT8_MAX; duty > 0; duty--)
        {
            pwm_period(duty);
        }
    }
}
