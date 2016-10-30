/*
 * project_bier.c
 *    
 * Created: 10/5/2016 12:57:02 PM
 * Author : Kai Harten
 */ 

/* Define the states */
#define START_UP		0
#define RUNNING			1
#define SHORT_CIRCUIT	2
#define NO_LOAD			3

/* Include needed files */
#include "system.h"
#include "Sepic_control.h"

void PWM_Write(uint8_t duty);

/* Declare global variables for ISR */
volatile	uint8_t	 state		= 0;
volatile	uint8_t  start_up	= 0;
			uint16_t source_adc = 0;
			uint16_t sepic_adc	= 0;
			uint8_t cool_off	= 0;
			uint8_t new_duty	= 0;
 uint32_t cnt		 = 0;
 uint16_t temp_val = 0;
 uint8_t	chan	 = 0;
 int8_t correction	= 0;
 	uint8_t duty		= 0;

ISR (ADC_vect)
{
	/* - Get ADC Value
	 * - Store Value in Array of corresponding channel 
	 * - Set Next ADC channel
	 * - Use ADC frequency for counting up the dutycycle for start up routine
	 */
	
	
	/* Read and store ADC value */	
	temp_val = ADC;

	switch (chan)
	{
		case 0:

			source_adc = temp_val;
			
			ADMUX	&=	0xF8;	
			ADMUX	|=	_BV(MUX0)|_BV(MUX1);

			break;
		case 1:

			sepic_adc = temp_val;
			
			ADMUX	&=	0xF8;
			ADMUX	&=	~_BV(MUX0) & ~_BV(MUX1);

			break;			
	}
	 chan++;
	
	if (state == START_UP || state == NO_LOAD)
	{	
		cnt++;
		if (cnt >= 30)
		{
			PORTB &= ~_BV(SW_PIN);
			start_up++;
			cnt = 0;
			if (sepic_adc > 710)
			{
				cool_off++;	
			}
		}
	}

	if (chan >= 2)
	{
		chan = 0;
	}
	correction	= Get_FeedBack_Correction(sepic_adc);
	ADCSRA |= _BV(ADSC);
	/* Start next conversion */

	
}
int main(void)
{
	uint8_t offset		= 2;

	cli();
	PORTB |= _BV(SW_PIN);
	
	SYS_Clock_Init();
	SYS_Timer_Init_PWM();
	SYS_Io_Init();
	SYS_Adc_Init();
	sei();
	
	state = START_UP;
    while (1) 
    {	
		uint16_t setpoint = 645;
		uint16_t diode	  = 26;
	
		duty = 171776/(source_adc+671);
		
		if (sepic_adc > 710)
		{
			state = NO_LOAD;
		}
		offset = Get_Offset(source_adc);
		switch (state)
		{
			case START_UP:
				/* Outputs from 0 to Feedforward value as PWM Duty-Cycle with ISR freq */
				PWM_Write(start_up);
				
				/* Checks if start up routine is finished */
				if (start_up >= duty)
				{
					state = RUNNING;
				}
				break;
			case RUNNING:
				/* Outputs Feedforward + Feedback Control Value as PWM Duty-Cycle */
				correction	= Get_FeedBack_Correction(sepic_adc);

				//PWM_Write(duty+correction+offset);

				if (source_adc < 510)
				{
					PWM_Write(160);	
				}
				if (source_adc >= 510)
				{
					PWM_Write(duty+correction+offset+1);
				}
				

							
				/* Checks input from Short Circuit Detector */
				if (PINB & _BV(SC_PIN))
				{
					state = SHORT_CIRCUIT;
				}
				break;
			case NO_LOAD:
				PWM_Write(5);

				break;
			case SHORT_CIRCUIT:
				/* Closes the PNP MOSFET to stop the SEPIC */
				PORTB |= _BV(SW_PIN);
				OCR0B = 0;
				
				/* Checks input from reset button */
				/*if (PINB & _BV(RST_PIN))
				{
					start_up = 0;
					state	 = START_UP;
				}*/
				break;
		}
    }
}

void PWM_Write(uint8_t duty)
{
	OCR0B = duty;
}
