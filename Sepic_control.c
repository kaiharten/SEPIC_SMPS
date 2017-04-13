/*
 * Sepic_control.c
 *
 * Created: 10/13/2016 2:24:15 PM
 *  Author: Kai Harten
 */ 
#include "Sepic_control.h"
	
uint8_t Set_Duty(uint16_t adc_val)
{	
	duty = 171776/(adc_val+671);
	return duty;
}

int8_t Set_Correction(uint16_t feedback)
{
	static int8_t   correction	= 0;
	uint16_t setpoint		= 645;
	int16_t   error			= setpoint - feedback;
	uint8_t p = 2;
	
	correction = error*p;
	return correction;
}
