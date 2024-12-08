#ifndef BOARD_H
#define BOARD_H

/**
 * @file board.h
 * @brief Board pinout for IEEE RAS Nakamura (based on ATmega32U4)
 * @author Rafael Dalzotto y Agustin Casas
 */

#include "ADC.h" 	// ADC0, ADC1, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, ADC10, ADC11, ADC12, ADC13
#include "PWM.h"	// OC4A, OC4D
#include "GPIO.h" 	// PC6, PD3, PD2, PD1, PD0, PB7, PE6, PB3, PB2, PB1, PB0, PD5

#define M1_PWM OC4A
#define M2_PWM OC4D

#define M1_EN PD1       // Enable
#define M2_EN PD2       // Enable

#define M1_DIR PC6      // Direction
#define M2_DIR PD3      // Direction

#define SENSOR_1 ADC0
#define SENSOR_2 ADC1
#define SENSOR_3 ADC4
#define SENSOR_4 ADC5
#define SENSOR_5 ADC6
#define SENSOR_6 ADC7
#define SENSOR_7 ADC8
#define SENSOR_8 ADC9

#define SENSOR_IR_ENABLE PD0

#define SENS_OUT_B ADC12        // Current sensor
#define SENS_OUT_A ADC13        // Current sensor

#define LED_G1	PB1
#define LED_G2	PB2
#define LED_B	PB3

#define BTN1 PB7
#define BTN2 PE6

#endif // BOARD_H