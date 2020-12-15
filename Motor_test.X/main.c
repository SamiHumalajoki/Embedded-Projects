#define PERIOD_EXAMPLE_VALUE        (0x0100)
#define DUTY_CYCLE_EXAMPLE_VALUE    (0x0000)

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
/*Using default clock 3.33MHz */

volatile uint16_t speed = 0;

void PORT_init(void);
void TCA0_init(void);

void TCA0_init(void)
{
    /* set waveform output on PORT A */
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTA_gc;
    
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm            /* enable compare channel 0 */
                      | TCA_SINGLE_WGMODE_DSBOTTOM_gc;    /* set dual-slope PWM mode */
    
    /* disable event counting */
    TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm); 
    
    /* set PWM frequency and duty cycle (50%) */
    TCA0.SINGLE.PERBUF  = PERIOD_EXAMPLE_VALUE;       
    TCA0.SINGLE.CMP0BUF = 0;  
    
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc        /* set clock source (sys_clk/4) */
                      | TCA_SINGLE_ENABLE_bm;           /* start timer */
}

void PORT_init(void)
{    
    /* set pin 0 of PORT A as output */    
    PORTA.DIRSET = PIN0_bm;
    PORTF.DIRCLR = PIN6_bm;
    PORTF.DIRSET = PIN5_bm;
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
}

ISR(PORTF_PORT_vect)
{
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xff;
    speed += 0x10;
    
    if (speed > PERIOD_EXAMPLE_VALUE)
    {
        speed = 0;
    }
    TCA0.SINGLE.CMP0BUF = speed;  
}

int main(void)
{
    PORT_init();
    
    TCA0_init();
    sei();
    /* Replace with your application code */
    while (1)
    {
        ;
    }
}
