#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(3333333 * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

void USART0_init(void);
void USART0_sendChar(char c);
void USART0_sendString(char *str);
void RTC_init(void);

// Value of the counter.
uint16_t count = 0;
// Enumeration for keeping track of the state of the counter.
enum State {RESET, RUNNING, STOPPED};
// Current state of the counter
enum State counter_state = RESET;

// The "borrowed" USART initialization routine.
void USART0_init(void)
{
    PORTA.DIR &= ~PIN1_bm;
    PORTA.DIR |= PIN0_bm;

    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
    USART0.CTRLB |= USART_TXEN_bm;
}

// The "borrowed" USART character sending routine.
void USART0_sendChar(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
    {
        ;
    }
    USART0.TXDATAL = c;
}

// The "borrowed" USART string sending routine.
void USART0_sendString(char *str)
{
    for(size_t i = 0; i < strlen(str); i++)
    {
        USART0_sendChar(str[i]);
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
    
    RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc /* RTC Clock Cycles 32768 */
                 | RTC_PITEN_bm; /* Enable: enabled */
}

// Interrupt service routine triggered by RTC
ISR(RTC_PIT_vect)
{
    /* Clear flag by writing '1': */
    RTC.PITINTFLAGS = RTC_PI_bm;
    if (counter_state == RUNNING)
    {
        // Transform count to string format and add a 'new line' to it
        // and send it with the serial connection.
        char count_as_string[5];
        sprintf(count_as_string, "%d", count);
        USART0_sendString(strcat(count_as_string, "\r\n"));
        // Increment the counter value;
        count++;
        // Toggle the LED
        PORTF.OUTTGL |= PIN5_bm;
    }  
}

// Interrupt service routine triggered by the button
// The state of the counter is rotated: RESET -> RUNNING -> STOPPED -> RESET...
// Transition from STOPPED to RESET will set the counter value to zero
// and send "RESET" with the serial connection.
ISR(PORTF_PORT_vect)
{
    switch (counter_state)
    {
        case RESET :
            counter_state = RUNNING;
            break;
        case RUNNING :
            counter_state = STOPPED;
            break;
        case STOPPED :
            counter_state = RESET;
            count = 0;
            USART0_sendString("RESET\r\n");
            break;
    }
    // Clear all interrupt flags
    PORTF.INTFLAGS = 0xFF;
}

int main(void)
{
    // Call the USART0 initialization routine
    USART0_init();
    // Set LED (port F pin 5) as output
    PORTF.DIR |= PIN5_bm;
    // Call the RTC initialization routine
    RTC_init();
    // Configured the button to trigger an interrupt when pushing down
    PORTF.PIN6CTRL = PORT_ISC_FALLING_gc;
    // Set sleep mode to idle.
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    // Enable interrupts
    sei();
    // The idle loop. The CPU keeps going back to sleep after each 
    // interrupt service routine
    while (1) 
    {
        sleep_mode();
    }
}