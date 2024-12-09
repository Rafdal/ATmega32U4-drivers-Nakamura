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

#define POINTS_PER_SENSOR   10      // Cantidad de puntos (eje x) por sensor
#define N_SENSORS           8       // Cantidad de sensores
#define MID_VALUE ((N_SENSORS-1)*POINTS_PER_SENSOR/2) // La mitad del rango de valores de la línea

typedef enum{
    LINE_BLACK,
    LINE_WHITE
}line_color_t;


void LineSensor_init(line_color_t l_color);

// Levanta los valores de calibración de los sensores desde la EEPROM
void LineSensor_initFromEEPROM(line_color_t l_color);

void LineSensor_saveCalibrationToEEPROM();

// Imprime los valores de calibración actuales
void LineSensor_printCalibration();

// Imprime los valores de los sensores con el ajuste de calibración
void LineSensor_printReadings();

// Manda los valores de los sensores en binario (para SerialPlot, custom frame)
void LineSensor_printReadingsBinary();

/**
 * @brief   Obtiene los valores minimos y maximos de cada sensor para normalizar la medicion
*/
void LineSensor_calibrateSensors();

void LineSensor_resetCalibration();

/**
 * @brief Retorna la posición de la linea respecto del arreglo de sensores (desde -MID_VALUE a +MID_VALUE)
 * @param threshold    Umbral de activación para considerar la presencia de la línea (0-255)
 * @returns Posición de la línea entre -MID_VALUE y MID_VALUE
*/
int LineSensor_read(uint8_t threshold = 120);

/**
 * @brief Retorna true cuando la linea fue detectada en el último llamado a LineSensor_read
 * @return true si la línea fue detectada
*/
bool LineSensor_lineDetected();

/**
 * @brief Retorna true cuando la linea se encuentra fuera del rango de detección
 */
bool LineSensor_lineOutOfRange();


/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_SENSORS_H_