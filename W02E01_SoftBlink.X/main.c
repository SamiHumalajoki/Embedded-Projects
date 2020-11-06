/*
 * DTEK0068 Embedded Microprocessor Systems
 * Assignment 2
 * Sami Humalajoki
 */

#define F_CPU   3333333

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void pwm_period(uint8_t duty) {
    /*
    uint8_t logDuty = 7;
    while ((duty / 128) == 0)
    {
        logDuty--;
        duty = duty<<1;
    }
    */
    //duty = 1;
    PORTF.OUTSET = PIN5_bm;
    for (uint8_t i = 0; i < duty; i++)
    {
        _delay_us(7);
    }
    PORTF.OUTCLR = PIN5_bm;
    for (uint8_t i = 0; i < (0xff - duty); i++)
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
        for (uint8_t duty = 0; duty < 0xff; duty++)
        {
            pwm_period(duty);
        }
        for (uint8_t duty = 0xff; duty >= 0; duty--)
        {
            pwm_period(duty);
        }
    }
}
