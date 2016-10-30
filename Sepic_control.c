/*
 * Sepic_control.c
 *
 * Created: 10/13/2016 2:24:15 PM
 *  Author: Kai Harten
 */ 
#include "Sepic_control.h"

// 118 bij 15V Source
// 132 bij 12.5 Source
// 147 bij 10.33V Source
// 149 bij 10V Source
		
uint8_t Get_FeedForward_Duty(uint16_t adc_val)
{	
	uint16_t setpoint	= 645;
	uint16_t diode		= 26;	
	
	uint8_t duty		= ((setpoint + diode)*256)/(adc_val+setpoint+diode); 
	
	return duty;
}

int8_t Get_FeedBack_Correction(uint16_t feedback)
{
	static int8_t   correction	= 0;
	uint16_t setpoint	= 645;
	int8_t   error		= setpoint - feedback;
	if (error >= 45)
	{
		correction = 6;
	}
	if (error >= 30 && error < 45)
	{
		correction = 4;
	}
	if (error >= 15 && error < 30)
	{
		correction = 2;
	}
	if (error < 15 && error > -15)
	{
		correction = 0;
	}
	if (error <= -15 && error < -30)
	{
		correction = -2;
	}
	if (error <= -30 && error > -45)
	{
		correction = -4;
	}
	if (error <= -45)
	{
		correction = -6;
	}
	if (error <= -65)
	{
		correction -= 6;
	}
	else
	{
		correction = 2;
	}
	return correction;
}

uint8_t Get_Offset(uint16_t source_adc)
{
	static uint8_t offset = 0;
			if (source_adc <= 640 && source_adc > 590)
			{
				offset = 4;
			}
			if (source_adc <= 590 && source_adc > 565)
			{
				offset = 6;
			}
			
			if (source_adc <= 565 && source_adc > 530)
			{
				offset = 7;
			}
			if (source_adc <= 530 && source_adc > 510)
			{
				offset = 9;
			}
			if (source_adc > 715)
			{
				offset = 1;
			}
			
			return offset;
}
