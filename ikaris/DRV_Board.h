/***************************************************************************//**
  @file     DRV_Board.h
  @brief    Driver de LEDs y Botones del PCB
  @author   RAS-ITBA
 ******************************************************************************/

#ifndef _DRV_BOARD_H_
#define _DRV_BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum{
    LEDR,
    LEDY,
    LEDG,
    LEDTX,
    LEDRX
} led_t;

typedef enum{
    BTN1,
    BTN2,
    BTN3
} button_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializar LEDs y Botones
*/
void initBoard();

/**
 * @brief Leer el estado de un botón
 * @param btn: botón a leer (BTN1, BTN2, BTN3)
 * @return true cuando el botón se encuentra apretado
*/
bool readButton(button_t btn);

/**
 * @brief Encender o apagar un LED
 * @param led: LED a setear (LEDG, LEDY, LEDR)
 * @param value: Encendido o apagado (0/1)
*/
void setLED(led_t led, bool value);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_BOARD_H_