/*
 * DTEK0068 Embedded Microprocessor Systems
 * Week 2, Exercise 2
 * LED Dimmer
 * File:   main.c
 * Author: Sami Humalajoki

 * Created on 06 November 2020, 17:24
 */

#define F_CPU   3333333

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void pwm_period(uint8_t duty) {

    PORTF.OUTCLR = PIN5_bm;
    for (uint8_t i = 0; i < duty; i++)
    {
        _delay_us(7);
    }
    PORTF.OUTSET = PIN5_bm;
    for (uint8_t i = 0; i < (0xff - duty); i++)
    {
        _delay_us(7);
    }
}

enum State {Down, Up};

int main(void) {
    PORTF.DIRSET = PIN5_bm;
    PORTF.DIRCLR = PIN6_bm;
    enum State currentState = Down;
    enum State previousState = Down;
    int8_t changeBrightness = 1;
    uint8_t brightness = 0x80;
    
    while (1) 
    {    
        if (PORTF.IN & PIN6_bm)
        {
            currentState = Up;
            pwm_period(brightness);
        }
        else
        {
            currentState = Down;
            if (previousState == Up)
            {
                changeBrightness *= -1;
            }            
            if ((brightness > 0 && changeBrightness == -1) ||
                (brightness < 0xff && changeBrightness == 1))
            {
                brightness += changeBrightness;
            }
            pwm_period(brightness);
        }
        previousState = currentState;
    }
}
