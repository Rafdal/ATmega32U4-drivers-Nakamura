/***************************************************************************//**
  @file     board_definitions.h
  @brief    Definiciones generales del PCB de Ikaris
  @author   RAS-ITBA
 ******************************************************************************/

#ifndef _BOARD_DEFINITIONS_H
#define _BOARD_DEFINITIONS_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Arduino.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PIN_LEDG    2
#define PIN_LEDY    3
#define PIN_LEDR    11

#define PIN_BTN1    14  // D14 PB3 MISO,PCINT3
#define PIN_BTN2    16  // D16 PB2 MOSI,PCINT2
#define PIN_BTN3    15  // D15 PB1 SCK,PCINT1

#define PIN_RXLED   14  // D14 PB0 RXLED,SS/PCINT0
#define PIN_TXLED   30  // D30 PD5 TXLED,XCK1

#define PIN_MEN1    9   // D9# A9 PB5 PWM16,OC1A/#OC4B/ADC12/PCINT5
#define PIN_MIN1A   0
#define PIN_MIN1B   1
#define PIN_MEN2    10  // D10# A10 PB6 PWM16,OC1B/0c4B/ADC13/PCINT6
#define PIN_MIN2A   5
#define PIN_MIN2B   13

#define PIN_SENSEN  7
#define PIN_SENS1   A6
#define PIN_SENS2   A7
#define PIN_SENS3   A0
#define PIN_SENS4   A1
#define PIN_SENS5   A2
#define PIN_SENS6   A3
#define PIN_SENS7   A4
#define PIN_SENS8   A5

// Evitar carteles de error en otros editores:
#ifndef OUTPUT
    #define OUTPUT  0
    #define INPUT   1
    #define LOW     0
    #define HIGH    1
#endif

/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_DEFINITIONS_H