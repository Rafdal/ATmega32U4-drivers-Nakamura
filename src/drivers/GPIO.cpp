#include "GPIO.h"
/**
 * @file GPIO.cpp
 * @brief GPIO library for IEEE RAS Nakamura (based on ATmega32U4)
 * 
 * @author Rafael Dalzotto (rdalzotto@itba.edu.ar)
 */

void GPIO_mode(GPIO_PIN_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

uint8_t GPIO_read(GPIO_PIN_t pin)
{
	return (*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin));
}

void GPIO_write(GPIO_PIN_t pin, uint8_t val)
{
	uint8_t bit = digitalPinToBitMask(pin);
	volatile uint8_t *out = portOutputRegister(digitalPinToPort(pin));

	uint8_t oldSREG = SREG;
	cli();

	if (val) {
		*out |= bit;
	} else {
		*out &= ~bit;
	}

	SREG = oldSREG;
}

void GPIO_toggle(GPIO_PIN_t pin)
{
	uint8_t bit = digitalPinToBitMask(pin);
	volatile uint8_t *out = portOutputRegister(digitalPinToPort(pin));

	uint8_t oldSREG = SREG;
	cli();

	*out ^= bit;

	SREG = oldSREG;
}