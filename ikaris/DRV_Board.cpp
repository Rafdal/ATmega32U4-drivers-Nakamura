/***************************************************************************//**
  @file     DRV_Board.c
  @brief    Driver de LEDs y Botones del PCB
  @author   RAS-ITBA
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_Board.h"
#include "board_definitions.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define BTN_ACTIVE  LOW

#define btn_pin(b)  ((b == BTN1)? PIN_BTN1:     \
                    ((b == BTN2)? PIN_BTN2:     \
                    PIN_BTN3))

#define led_pin(l)  ((l == LEDG)? PIN_LEDG:     \
                    ((l == LEDY)? PIN_LEDY:     \
                    ((l == LEDR)? PIN_LEDR:     \
                    ((l == LEDTX)? PIN_TXLED:     \
                    PIN_RXLED))))

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initBoard(){
    pinMode(PIN_BTN1, INPUT);
    pinMode(PIN_BTN2, INPUT);
    pinMode(PIN_BTN3, INPUT);

    pinMode(PIN_LEDG, OUTPUT);
    pinMode(PIN_LEDY, OUTPUT);
    pinMode(PIN_LEDR, OUTPUT);
    pinMode(PIN_TXLED, OUTPUT);
    pinMode(PIN_RXLED, INPUT);

    digitalWrite(PIN_LEDR, LOW);
    digitalWrite(PIN_LEDY, LOW);
    digitalWrite(PIN_LEDG, LOW);
    digitalWrite(PIN_TXLED, HIGH);  // TX LED is active-low
}

bool readButton(button_t btn){
    return digitalRead(btn_pin(btn)) == BTN_ACTIVE;
}

void setLED(led_t led, bool value){
    digitalWrite(led_pin(led), value);
}