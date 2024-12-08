#include "PWM.h"
#include <wiring_private.h> // .platformio\packages\framework-arduino-avr\cores\arduino\wiring_private.h
/**
 * @file PWM.cpp
 * @brief PWM library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */


void PWM_setup(uint8_t prescaler, uint8_t top_value, uint8_t mode)
{
	DDRC |= _BV(7); // Set PC7 as output	(OC4A)
	DDRD |= _BV(7); // Set PD7 as output	(OC4D)

	// Set PWM mode
	sbi(TCCR4C, PWM4D);
	sbi(TCCR4A, PWM4A);
	switch (mode)
	{
		case FAST_PWM:
			cbi(TCCR4D, WGM40);
			cbi(TCCR4D, WGM41);	
			break;
		case PHASE_FREQ_CORRECT_PWM:
			sbi(TCCR4D, WGM40);
			cbi(TCCR4D, WGM41);	
			break;
		default:
			break;
	}

	// Set TOP value
	OCR4C = top_value;	// 8-bit PWM

	// Prescaler Setup
	// 15.12.2 TCCR4B = Timer/Counter Control Register B (p. 167)
	TCCR4B &= 0xF0;
	TCCR4B |= (0x0F & prescaler);	// set the prescaler
	
	// Set Clear OCW4A on compare match (Table 15-5) (p. 164) (Section 15.12.1)
	sbi(TCCR4A, COM4A1);
	cbi(TCCR4A, COM4A0);
	// Set Clear OCW4D on compare match  (Table 15-15) (p. 169) (Section 15.12.3)
	sbi(TCCR4C, COM4D1);
	cbi(TCCR4C, COM4D0);
}

void PWM_write(PWM_pin_t pin, uint8_t val)
{
	switch(pin)
	{
		case OC4A:
			OCR4A = val;	// set pwm duty
			break;
			
		case OC4D:				
			OCR4D = val;	// set pwm duty
			break;
		default:
			break;
	}
}

void PWM_write_both(uint8_t OC4A_val, uint8_t OC4D_val)
{
	OCR4A = OC4A_val;
	OCR4D = OC4D_val;
}