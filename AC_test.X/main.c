#include <avr/io.h>
#include <avr/interrupt.h>

/* set DACREF to 1.2 Volts for Vref = 1.5 Volts */
#define DACREF_VALUE    (1.2 * 256 / 1.5)

void PORT0_init (void);
void AC0_init(void);
            
ISR(AC0_AC_vect)
{
    /* Insert AC interrupt handling code here */
    PORTF.OUTCLR = PIN5_bm;
    /* The interrupt flag has to be cleared manually */
    AC0.STATUS = AC_CMP_bm;
}

void PORT0_init (void)
{
	/* Positive Input - Disable digital input buffer */
	PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
}

void PORTF_init (void)
{
    PORTF.DIRSET = PIN5_bm;
    PORTF.OUTSET = PIN5_bm;
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
}

ISR(PORTF_PORT_vect)
{
    /* Clear all interrupt flags */
    PORTF.INTFLAGS = UINT8_MAX;
    PORTF.OUTSET = PIN5_bm;
}

void AC0_init(void)
{
 
    /* Negative input uses internal reference - voltage reference should be enabled */
    VREF.CTRLA = VREF_AC0REFSEL_1V5_gc;    /* Voltage reference at 1.5V */
    VREF.CTRLB = VREF_AC0REFEN_bm;         /* AC0 DACREF reference enable: enabled */

    AC0.DACREF = DACREF_VALUE;             /* Set DAC voltage reference */

    /*Select proper inputs for comparator*/
    AC0.MUXCTRLA = AC_MUXPOS_PIN0_gc       /* Positive Input - Analog Positive Pin 0 */
                | AC_MUXNEG_DACREF_gc;     /* Negative Input - DAC Voltage Reference */
    
    AC0.CTRLA = AC_ENABLE_bm | AC_INTMODE_POSEDGE_gc              /* Enable analog comparator */
               | AC_OUTEN_bm;              /* Output Buffer Enable: enabled */
    
    AC0.INTCTRL = AC_CMP_bm;               /* Analog Comparator 0 Interrupt enabled */

}


int main(void)
{
    PORTF_init();
    PORT0_init();
    AC0_init();
    sei();            /* Global interrupts enabled */

    while (1) 
    {
        ;    
    }
}

