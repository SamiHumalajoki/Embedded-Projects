/*
 * File:   main.c
 * Author: Sami Humalajoki
 *
 * Created on 02 December 2020, 15:41
 */

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

enum run_mode {stop, slow, medium, fast}; 

/* set DACREF to 1.2 Volts for Vref = 1.5Volts */
#define DACREF_VALUE    (1.3 * 256 / 1.5)
#define SLOW_PWM_DUTY   (0x10)
#define MEDIUM_PWM_DUTY (0x20)
#define FAST_PWM_DUTY   (0x40)

void CLOCK_init (void);
void TCB0_init (void);
void SLPCTRL_init (void);
void write_7_segment_display (int8_t value);
void RTC_init (void);
void PORT0_init (void);
void AC0_init (void);
void PORTF_init (void);

volatile int16_t count = 0;
volatile enum run_mode current_run_mode = stop;

void CLOCK_init (void)
{
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc;

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
    TCB0.CCMPL = UINT8_MAX;
    TCB0.CCMPH = 0;

    /* Enable TCB0 and make it run in standby mode */
    TCB0.CTRLA |= TCB_ENABLE_bm | TCB_CLKSEL_CLKDIV1_gc | TCB_RUNSTDBY_bm;
    
    /* Enable Pin Output to PA2 and configure TCB in 8-bit PWM mode */
    TCB0.CTRLB |= TCB_CCMPEN_bm | TCB_CNTMODE_PWM8_gc;
}

void SLPCTRL_init (void)
{
    /* Enable sleep mode and select Standby mode */
    SLPCTRL.CTRLA = SLPCTRL_SMODE_gm | SLPCTRL_SMODE_STDBY_gc;
}

/* Outputs the input value to the seven segment display by first clearing
 * all the output pins and then setting the needed pins high*/
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

/* The RTC initialization routine */
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

/* Interrupt service routine triggered by RTC */
ISR(RTC_PIT_vect)
{
    /* Clear flag by writing '1': */
    RTC.PITINTFLAGS = RTC_PI_bm;
    
    /* Indicate that the device is running by flashing the on-board LED */
    if (current_run_mode != stop)
    {
        PORTF.OUTTGL = PIN5_bm;
    }
    
    /* The count equals half-rounds in half second, which is directly rounds
     per second, so round per minute count is just 60 times of that.*/
    
    uint16_t RPM = 60 * count;
    count = 0;
    write_7_segment_display(RPM / 1000);
    
}

 /* AC interrupt handling */
ISR(AC0_AC_vect)
{
    /* The count-value gets incremented every time the propellor passes
     the gap between LDR and LED*/
    count++;
 
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
    /* Negative input uses internal reference */
    /* Voltage reference at 1.5V */
    VREF.CTRLA = VREF_AC0REFSEL_1V5_gc;    
    
    /* AC0 DACREF reference enable: enabled */
    VREF.CTRLB = VREF_AC0REFEN_bm;         
    
    /* Set DAC voltage reference */
    AC0.DACREF = DACREF_VALUE;             

    /*Select proper inputs for comparator*/
    /* Positive Input - Analog Positive Pin 0 */
    /* Negative Input - DAC Voltage Reference */
    AC0.MUXCTRLA = AC_MUXPOS_PIN0_gc       
                | AC_MUXNEG_DACREF_gc;     
    
    /* Enable analog comparator, make it cause an interrupt at positive edge
     * and select medium hysteresis mode */
    AC0.CTRLA = AC_ENABLE_bm
               | AC_INTMODE_POSEDGE_gc
               | AC_HYSMODE_25mV_gc;        
    
    /* Analog Comparator 0 Interrupt enabled */
    AC0.INTCTRL = AC_CMP_bm;               
}

/* Initializing on-board button and LED*/
void PORTF_init(void)
{
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
    PORTF.DIRSET = PIN5_bm;
    PORTF.OUTSET = PIN5_bm;
}

/* Interrupt routine for on-board button*/
ISR(PORTF_PORT_vect)
{
    /* Clear all interrupt flags */
    PORTF.INTFLAGS = UINT8_MAX;
    
    /* Cycle the run mode: stop -> slow -> medium -> fast -> stop...
     and set the according PWM duty cycle*/
    switch (current_run_mode)
    {
        case stop :
            current_run_mode = slow;
            TCB0.CCMPH = SLOW_PWM_DUTY;
            break;
        case slow :
            current_run_mode = medium;
            TCB0.CCMPH = MEDIUM_PWM_DUTY;
            break;
        case medium :
            current_run_mode = fast;
            TCB0.CCMPH = FAST_PWM_DUTY;
            break;
        case fast :
            current_run_mode = stop;
            TCB0.CCMPH = 0;
            PORTF.OUTSET = PIN5_bm;
            break;
    }
}

int main(void)
{   
    /* Run all the initializing routines */
    CLOCK_init();
    SLPCTRL_init();
    TCB0_init();
    RTC_init();
    PORTF_init();
    PORT0_init();
    AC0_init();
    
    /* Write zero to the seven segment display*/
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | 
                   PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    write_7_segment_display(0);
    
    /* Global interrupts enabled */
    sei();
    
    /* Keep returning into sleep mode*/
    while (1) 
    {
        sleep_mode();    
    }
}

