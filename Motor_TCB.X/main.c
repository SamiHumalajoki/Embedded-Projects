#include <avr/io.h>
#include <avr/interrupt.h>

#define TCB_CMP_EXAMPLE_VALUE   (0x80FF)

volatile uint8_t speed = 0;

void CLOCK_init (void);
void PORT_init (void);
void TCB3_init (void);

void CLOCK_init (void)
{
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc;
    /* Enable Prescaler and set Prescaler Division to 64 */
    //CLKCTRL.MCLKCTRLB = CLKCTRL_PEN_bm;
    
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc;
    /* Select 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    
    /* Wait for system oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }
}

void TCB3_init (void)
{
    /* Load CCMP register with the period and duty cycle of the PWM */
    TCB3.CCMP = TCB_CMP_EXAMPLE_VALUE;

    /* Enable TCB3 */
    TCB3.CTRLA |= TCB_ENABLE_bm;
    
    /* Enable Pin Output and configure TCB in 8-bit PWM mode */
    TCB3.CTRLB |= TCB_CCMPEN_bm;
    TCB3.CTRLB |= TCB_CNTMODE_PWM8_gc;
}

void PORT_init(void)
{    
    /* set pin 5 of PORT B as output */    
    PORTB.DIRSET = PIN5_bm;
    
    PORTF.DIRCLR = PIN6_bm;
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
}

ISR(PORTF_PORT_vect)
{
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xff;
    speed += 0x10;
    TCB3.CCMPH = speed;
}

int main(void)
{
    CLOCK_init();
    PORT_init();
    TCB3_init();
    sei();
    while (1)
    {
        ;
    }
}