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

#include <Arduino.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define POINTS_PER_SENSOR   10
#define N_SENSORS           8       // Cantidad de sensores

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
 * @return Descripcion valor que devuelve
*/
void LineSensor_init(line_color_t l_color);

void LineSensor_initFromEEPROM(line_color_t l_color);

void LineSensor_saveCalibrationToEEPROM();

void LineSensor_printCalibration();

void LineSensor_printReadings();

void LineSensor_printReadingsBinary();

/**
 * @brief   Obtiene los valores minimos y maximos de cada sensor para normalizar la medicion
*/
void LineSensor_calibrateSensors();

void LineSensor_resetCalibration();

/**
 * @brief Retorna la posición de la linea respecto del arreglo de sensores
 * @param minimum_brightness    Mínimo valor de brillo que se considera parte de la línea
 *                              (valor expresado de 0 a 255)
 * @return Posición de la línea entre -LINE_RANGE y LINE_RANGE
*/
int LineSensor_read(uint8_t minimum_brightness);

/**
 * @brief Retorna true cuando la linea fue detectada en el último llamado a LineSensor_read
 * @return true si la línea fue detectada
*/
bool LineSensor_lineDetected();


/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_SENSORS_H_