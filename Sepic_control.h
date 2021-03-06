/*
 * Sepic_control.h
 *
 * Created: 10/13/2016 1:07:07 PM
 *  Author: Kai Harten
 */ 


#ifndef SEPIC_CONTROL_H_
#define SEPIC_CONTROL_H_

#include "avr/io.h"

#define milliVolt_2_Bit(val, reso, ref, ampl)     (uint16_t)((val*reso)/(ref*ampl))
#define Bit_2_MilliVolt(val, reso, ref, ampl)     (uint16_t)((val*ref*ampl)/reso)

uint8_t Set_Duty(uint16_t src_volt);
int8_t Set_Correction(uint16_t feedback);

#endif /* SEPIC_CONTROL_H_ */
