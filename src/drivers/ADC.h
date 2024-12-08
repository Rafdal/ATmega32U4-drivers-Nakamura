#ifndef ADC_H
#define ADC_H
/**
 * @file ADC.h
 * @brief ADC library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */

#include <Arduino.h>

// Analog Channels
#define ADC0 0	// PF0
#define ADC1 1	// PF1
#define ADC4 4	// PF4
#define ADC5 5	// PF5
#define ADC6 6	// PF6
#define ADC7 7	// PF7
#define ADC8 8	// PD4
#define ADC9 9	// PD6
#define ADC10 10 // PD7	(OC4D) (PWM)
#define ADC11 11 // PB4
#define ADC12 12 // PB5
#define ADC13 13 // PB6

int ADC_read(uint8_t channel);

#endif // ADC_H