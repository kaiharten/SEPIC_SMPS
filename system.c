/*
 * system.c
 *
 * Created: 10/6/2016 6:02:19 PM
 *  Author: Kai Harten
 */ 

#include "system.h"

void SYS_Clock_Init()
{
	/* Enable Set Prescaling */
	CLKPR = (1<<CLKPCE);
	
	/* Set System Clock to No Prescaling */
	 CLKPR &= (0<<CLKPS3) & (0<<CLKPS2) & (0<<CLKPS1) & (0<<CLKPS0);
	
	/* Calibrate Internal RC Oscillator*/
	 OSCCAL |= _BV(CAL3);
	 OSCCAL += 7;
	
}

void SYS_Adc_Init()
{
	/* Use Vcc as analog reference & No Left Adjustment  */
	ADMUX &= ~_BV(REFS0) & ~_BV(ADLAR);
	
	/* Enable ADC & Enable Interrupt */
	ADCSRA |= _BV(ADEN) | _BV(ADIE);
	
	/* Set Prescaler to /128 */
	ADCSRA |= _BV(ADPS2)|_BV(ADPS1);
	
	ADCSRA |= _BV(ADSC);
}

void SYS_Timer_Init_PWM()
{
	/* Pin PB1 as output for PWM Signal */
	DDRB |= _BV(PB1);
	
	/* Set Prescaler to No Prescaling */
	TCCR0B |= _BV(CS00);
	TCCR0B &= ~_BV(CS01) & ~_BV(CS02);
	
	/* Set mode to Fast PWM */
	TCCR0A |= _BV(WGM01)|_BV(WGM00);
	TCCR0A &= ~_BV(WGM02);
		
	/* Clear OC0B on Compare Match, set OC0B at TOP */
	TCCR0A |= _BV(COM0B1);	
}

void SYS_Io_Init()
{
	/* Short Circuit Digital Input */
	DDRB &= ~_BV(SC_PIN);
	DDRB &= ~_BV(RST_PIN);
	
	/* Digital Outputs to Switch (FET) */
	DDRB |=	_BV(SW_PIN) | _BV(PWM_PIN);
	PORTB &= ~_BV(SW_PIN) & ~_BV(PWM_PIN);	
}