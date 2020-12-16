#include <avr/io.h>
#include <avr/sleep.h>
#define TCB_CMP_EXAMPLE_VALUE   (0x60FF)

void CLOCK_init (void);
void PORT_init (void);
void TCB0_init (void);

void CLOCK_init (void)
{
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc;
    /* Enable Prescaler and set Prescaler Division to 64 */
    //CLKCTRL.MCLKCTRLB = CLKCTRL_PEN_b | CLKCTRL_PDIV_64X_gc;
     /* Disable CLK_PER Prescaler */
    CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp;
  
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

void PORT_init (void)
{
    //PORTA.DIRSET = PIN3_bm;
    //PORTA.OUTCLR = PIN3_bm;
}

void TCB0_init (void)
{
    /* Load CCMP register with the period and duty cycle of the PWM */
    TCB0.CCMP = TCB_CMP_EXAMPLE_VALUE;

    /* Enable TCB0 and Divide CLK_PER by 2 */
    TCB0.CTRLA |= TCB_ENABLE_bm;
    TCB0.CTRLA |= TCB_CLKSEL_CLKDIV1_gc | TCB_RUNSTDBY_bm;
    
    /* Enable Pin Output and configure TCB in 8-bit PWM mode */
    TCB0.CTRLB |= TCB_CCMPEN_bm;
    TCB0.CTRLB |= TCB_CNTMODE_PWM8_gc;
}

int main(void)
{
    CLOCK_init();
    PORT_init();
    TCB0_init();
    SLPCTRL.CTRLA = SLPCTRL_SMODE_gm | SLPCTRL_SMODE_STDBY_gc;
    
    while (1)
    {
        //sleep_mode();
    }
}