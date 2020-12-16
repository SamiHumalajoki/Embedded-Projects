#include <avr/io.h>
#include <stdbool.h>

volatile uint8_t adcVal;

void ADC0_init(void);
uint8_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conversionDone(void);
void write_7_segment_display(int8_t value);
void ADC0_init(void)

{
    PORTF.DIRSET = PIN5_bm;
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | 
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    write_7_segment_display(0);
    
    /* Disable digital input buffer */
    PORTF.PIN4CTRL &= ~PORT_ISC_gm;
    PORTF.PIN4CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    
    /* Disable pull-up resistor */
    PORTF.PIN4CTRL &= ~PORT_PULLUPEN_bm;

    ADC0.CTRLC = ADC_PRESC_DIV16_gc      /* CLK_PER divided by 4 */
               | ADC_REFSEL_VDDREF_gc;  /* Internal reference */
    
    ADC0.CTRLA = ADC_ENABLE_bm          /* ADC Enable: enabled */
               | ADC_RESSEL_8BIT_gc;   /* 10-bit mode */
    
    /* Select ADC channel */
    ADC0.MUXPOS  = ADC_MUXPOS_AIN14_gc;
    
    /* Enable FreeRun mode */
    ADC0.CTRLA |= ADC_FREERUN_bm;
}

uint8_t ADC0_read(void)
{
    /* Clear the interrupt flag by writing 1: */
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    
    return ADC0.RES;
}

void ADC0_start(void)
{
    /* Start conversion */
    ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC0_conversionDone(void)
{
    return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}

void write_7_segment_display(int8_t value)
{

    PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm; 
    /*
    switch (value)
    {
        case 0 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                           PIN4_bm | PIN5_bm;
            break;
        case 1 :
            PORTC.OUTSET = PIN1_bm | PIN2_bm ;
            break;
        case 2 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN3_bm | PIN4_bm | PIN6_bm;
            break;
        case 3 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm| PIN6_bm;
            break;
        case 4 :
            PORTC.OUTSET = PIN1_bm | PIN2_bm | PIN5_bm | PIN6_bm;
            break;
        case 5 :
            PORTC.OUTSET = PIN0_bm | PIN2_bm | PIN3_bm | PIN5_bm | PIN6_bm;
            break;
        case 6 :
            PORTC.OUTSET = PIN0_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm |
                           PIN6_bm;
            break;
        case 7 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm;
            break;
        case 8 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                           PIN4_bm | PIN5_bm | PIN6_bm;
            break;
        case 9 :
            PORTC.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                           PIN5_bm | PIN6_bm;
            break;
    }
     */
}

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
    TCB0.CCMP = 0x00FF;

    /* Enable TCB0 and Divide CLK_PER by 2 */
    TCB0.CTRLA |= TCB_ENABLE_bm;
    TCB0.CTRLA |= TCB_CLKSEL_CLKDIV1_gc | TCB_RUNSTDBY_bm;
    
    /* Enable Pin Output and configure TCB in 8-bit PWM mode */
    TCB0.CTRLB |= TCB_CCMPEN_bm;
    TCB0.CTRLB |= TCB_CNTMODE_PWM8_gc;
}

int main(void)
{
    ADC0_init();
    ADC0_start();
    CLOCK_init();
    TCB0_init();
    
    while(1)
    {
        if (ADC0_conversionDone())
        {
            adcVal = ADC0_read();
            if (adcVal < 10) {adcVal = 0;}
            TCB0.CCMPH = adcVal;
            write_7_segment_display(adcVal/25);
            
            /* In FreeRun mode, the next conversion starts automatically */
        }
    }
}