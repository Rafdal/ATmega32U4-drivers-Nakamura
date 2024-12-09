/***************************************************************************/ /**
   @file     DRV_Sensors.c
   @brief    Drivers del arreglo de sensores de linea
   @author   RAS-ITBA
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "ADC.h"
#include "LineSensor.h"
#include <EEPROM.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const uint8_t sensor_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static line_color_t line_color;
static bool line_detected = false;
static uint8_t sensor_min[N_SENSORS]; // Valor minimo de cada sensor
static uint8_t sensor_max[N_SENSORS]; // Valor maximo de cada sensor

static uint8_t read_calibrated(uint8_t sensor_index);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void LineSensor_init(line_color_t l_color)
{
    line_color = l_color;

    for (int x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = 255;
        sensor_max[x] = 0; // Se inicializan tal que se puedan sobreescribir
    }
}

void LineSensor_initFromEEPROM(line_color_t l_color)
{
    line_color = l_color;
    EEPROM.begin();

    for (int x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = EEPROM.read(x);
        sensor_max[x] = EEPROM.read(x + N_SENSORS);
    }
}

void LineSensor_saveCalibrationToEEPROM()
{
    for (int x = 0; x < N_SENSORS; x++)
    {
        EEPROM.update(x, sensor_min[x]);
        EEPROM.update(x + N_SENSORS, sensor_max[x]);
    }
}

void LineSensor_printCalibration()
{
    char buf[10];
    Serial.print("SENSOR");
    for (int x = 0; x < N_SENSORS; x++)
    {
        sprintf(buf, "\t%3u", x);
        Serial.print(buf);
    }
    Serial.print("\nMIN");
    for (int x = 0; x < N_SENSORS; x++)
    {
        sprintf(buf, "\t%3u", sensor_min[x]);
        Serial.print(buf);
    }
    Serial.print("\nMAX");
    for (int x = 0; x < N_SENSORS; x++)
    {
        sprintf(buf, "\t%3u", sensor_max[x]);
        Serial.print(buf);
    }
    Serial.println();
}

void LineSensor_printReadings()
{
    char buf[10];
    Serial.print("SENSOR");
    for (int x = 0; x < N_SENSORS; x++)
    {
        sprintf(buf, "\t%3u", x);
        Serial.print(buf);
    }
    Serial.print("\nVAL");
    for (int x = 0; x < N_SENSORS; x++)
    {
        sprintf(buf, "\t%3u", read_calibrated(x));
        Serial.print(buf);
    }
    Serial.println();
}

void LineSensor_printReadingsBinary()
{
    for (int x = 0; x < N_SENSORS; x++)
    {
        Serial.write(read_calibrated(x));
        Serial.write((uint8_t)0x00);
    }
}

void LineSensor_resetCalibration()
{
    for (int x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = 255;
        sensor_max[x] = 0;
    }
}

void LineSensor_calibrateSensors()
{
    uint8_t val;
    for (int x = 0; x < N_SENSORS; x++)
    {
        val = ADC_read(sensor_pins[x]) >> 2;                         // Usamos 8 de los 10 bits
        sensor_min[x] = (val < sensor_min[x]) ? val : sensor_min[x]; // Si es menor al valor minimo guardado, lo reemplazo
        sensor_max[x] = (val > sensor_max[x]) ? val : sensor_max[x]; // Analogo al minimo
    }
}

int LineSensor_read(uint8_t minimum_brightness)
{
    
    static bool last_direction;

    line_detected = false;

    unsigned long w_sum = 0;
    unsigned long tot_sum = 0;

    for (unsigned long x = 0; x < N_SENSORS; x++)
    {
        unsigned long val = read_calibrated(x);

        if (val >= minimum_brightness)
        { 
            // At least one sensor has to detect a line
            line_detected = true;
            w_sum += val * x;    // Weighted sum
            tot_sum += val;       // Sum of all valid sensors
        }
    }
    unsigned long line;
    if (line_detected)
    {
        line = (w_sum / tot_sum);                             // Weighted average
    }
    else
    {
        line = (last_direction) ? (999) : -999;                 // Use last direction to calculate error as the maximum value
    }

    last_direction = line > 0;

    return (int)line;
}

static uint8_t read_calibrated(uint8_t idx)
{
    // Scale from 0 to 100
    uint8_t val = ADC_read(sensor_pins[idx]) >> 2;
    val = constrain(val, sensor_min[idx], sensor_max[idx]);
    val = map(val, sensor_min[idx], sensor_max[idx], 0, 100);
    return (line_color == LINE_BLACK) ? (val) : (100 - val);
}

bool LineSensor_lineDetected()
{
    return line_detected;
}