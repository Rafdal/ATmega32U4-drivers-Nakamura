#include "ADC.h"
#include <wiring_private.h> // .platformio\packages\framework-arduino-avr\cores\arduino\wiring_private.h
/**
 * @file ADC.cpp
 * @brief ADC library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */

#define AREFS_MODE 1	// AVCC with external capacitor on AREF pin

int ADC_read(uint8_t channel)
{
	// 24.9.4 ADC Control and Status Register B (Page 317)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((channel >> 3) & 0x01) << MUX5);
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
	ADMUX = (AREFS_MODE << 6) | (channel & 0x07);

	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; reading ADCL second would cause 
	// the results of each conversion to be discarded
	uint8_t low  = ADCL;
	return (ADCH << 8) | low;
}