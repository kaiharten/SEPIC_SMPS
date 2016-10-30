/*
 * system.h
 *
 * Created: 10/6/2016 6:00:21 PM
 *  Author: Kai Harten
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "avr/io.h"
#include "avr/interrupt.h"

#define F_CPU	9600000L
#define SC_PIN	PB4
#define SW_PIN	PB0
#define PWM_PIN	PB1
#define RST_PIN	PB2
	 
void SYS_Clock_Init();
void SYS_Timer_Init_PWM();
void SYS_Adc_Init();
void SYS_Io_Init();


#endif /* SYSTEM_H_ */