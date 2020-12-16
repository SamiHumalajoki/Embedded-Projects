/*
 * File:   main.c
 * Author: dtek0068
 *
 * Created on 02 December 2020, 15:41
 */

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

/* set DACREF to 0.8 Volts for Vref = 1.5Volts */
#define DACREF_VALUE    (0.8 * 256 / 1.5)

#define TCB_CMP_EXAMPLE_VALUE   (0x80ff)

void CLOCK_init (void);
void TCB0_init(void);
void SLPCTRL_init (void);
void write_7_segment_display(int8_t value);
void RTC_init(void);
void PORT0_init (void);
void AC0_init(void);

volatile int16_t count = 0;
volatile uint8_t running = 0;
volatile uint16_t speed = 0x00ff;

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

void SLPCTRL_init (void)
{
    /* Enable sleep mode and select Standby mode */
    SLPCTRL.CTRLA = SLPCTRL_SMODE_gm | SLPCTRL_SMODE_STDBY_gc;
}

void write_7_segment_display(int8_t value)
{

    PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm |
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm; 
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
}

// The "borrowed" RTC initialization routine
void RTC_init(void)
{
    uint8_t temp;
    
    /* Initialize 32.768kHz Oscillator: */
    /* Disable oscillator: */
    temp = CLKCTRL.XOSC32KCTRLA;
    temp &= ~CLKCTRL_ENABLE_bm;
    /* Writing to protected register */
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
    
    while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
    {
        ; /* Wait until XOSC32KS becomes 0 */
    }
    
    /* SEL = 0 (Use External Crystal): */
    temp = CLKCTRL.XOSC32KCTRLA;
    temp &= ~CLKCTRL_SEL_bm;
    /* Writing to protected register */
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
    
    /* Enable oscillator: */
    temp = CLKCTRL.XOSC32KCTRLA;
    temp |= CLKCTRL_ENABLE_bm;
    /* Writing to protected register */
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
    
    /* Initialize RTC: */
    while (RTC.STATUS > 0)
    {
        ; /* Wait for all register to be synchronized */
    }

    /* 32.768kHz External Crystal Oscillator (XOSC32K) */
    RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;

    /* Run in debug: enabled */
    RTC.DBGCTRL = RTC_DBGRUN_bm;
    
    RTC.PITINTCTRL = RTC_PI_bm; /* Periodic Interrupt: enabled */
    
    RTC.PITCTRLA = RTC_PERIOD_CYC16384_gc /* RTC Clock Cycles 16384 */
                 | RTC_PITEN_bm; /* Enable: enabled */
}

// Interrupt service routine triggered by RTC
ISR(RTC_PIT_vect)
{
    /* Clear flag by writing '1': */
    RTC.PITINTFLAGS = RTC_PI_bm;
    write_7_segment_display(count/1000);  
}

 /* AC interrupt handling */
ISR(AC0_AC_vect)
{
    count++;
    if (count > 9999)
    {
        count = 0;
    }
    /* The interrupt flag has to be cleared manually */
    AC0.STATUS = AC_CMP_bm;
}

void PORT0_init (void)
{
	/* Positive Input - Disable digital input buffer */
	PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
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
    
    AC0.CTRLA = AC_ENABLE_bm
               | AC_INTMODE_POSEDGE_gc
               | AC_HYSMODE_50mV_gc        /* Enable analog comparator */
               | AC_OUTEN_bm;              /* Output Buffer Enable: enabled */
    
    AC0.INTCTRL = AC_CMP_bm;               /* Analog Comparator 0 Interrupt enabled */
}

int main(void)
{   
    
    CLOCK_init();
    SLPCTRL_init();
    TCB0_init();
    RTC_init();
    
    PORT0_init();
    AC0_init();
    
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | 
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    write_7_segment_display(0);
    

    sei();            /* Global interrupts enabled */

    while (1) 
    {
        sleep_mode();    
    }
}

