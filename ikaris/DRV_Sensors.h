/***************************************************************************//**
  @file     DRV_Sensors.h
  @brief    Drivers del arreglo de sensores de linea
  @author   RAS-ITBA
 ******************************************************************************/

#ifndef _DRV_SENSORS_H_
#define _DRV_SENSORS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define N_SENSORS       8   // Cantidad de sensores
#define LINE_RANGE      1000

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum{
    LINE_BLACK,
    LINE_WHITE
}line_color_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializar sensores
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void initSensors(line_color_t l_color);

/**
 * @brief   Actualizar los valores minimos y máximos del rango posible de brillo
 *          para cada sensor mediante una única medición nueva por sensor
*/
void calibrateSensors();

/**
 * @brief Retorna la posición de la linea respecto del arreglo de sensores
 * @param minimum_brightness    Mínimo valor de brillo que se considera parte de la línea
 *                              (porcentaje expresado de 0.0 a 1.0)
 * @return Posición de la línea entre -LINE_RANGE y LINE_RANGE
*/
float getLinePosition(float minimum_brightness);

/**
 * @brief Retorna true cuando la linea fue detectada en el último llamado a getLinePosition
 * @return true si la línea fue detectada
*/
bool lineWasDetected();


/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_SENSORS_H_