#ifndef PWM_H
#define PWM_H
/**
 * @file PWM.h
 * @brief PWM library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */

#include <Arduino.h>

typedef enum OUTPUT_COMPARE_PINS {
	OC4A,   // PC7
	OC4D,   // PD7
} PWM_pin_t;

// See Section 15.8 (p. 151) for more information
enum OUTPUT_COMPARE_MODES {
	FAST_PWM,
	PHASE_FREQ_CORRECT_PWM,
};

enum PWM_PRESCALER {
	PWM_PRESCALER_1 = 1,
	PWM_PRESCALER_2,
	PWM_PRESCALER_4,
	PWM_PRESCALER_8,
	PWM_PRESCALER_16,
	PWM_PRESCALER_32,
	PWM_PRESCALER_64,
	PWM_PRESCALER_128,
	PWM_PRESCALER_256,
	PWM_PRESCALER_512,
	PWM_PRESCALER_1024,
	PWM_PRESCALER_2048,
	PWM_PRESCALER_4096,
	PWM_PRESCALER_8192,
	PWM_PRESCALER_16384,
};

/**
 * @brief Setup PWM
 * 
 * @param prescaler - Prescaler value
 * @param top_val - TOP value (the value at which the counter resets)
 * @param mode - PWM mode (FAST_PWM or PHASE_FREQ_CORRECT_PWM)
 */
void PWM_setup(uint8_t prescaler = PWM_PRESCALER_2, uint8_t top_val = 160, uint8_t mode = PHASE_FREQ_CORRECT_PWM);

void PWM_write(PWM_pin_t pin, uint8_t duty);
void PWM_write_both(uint8_t OC4A_duty, uint8_t OC4D_duty);

#endif // PWM_H