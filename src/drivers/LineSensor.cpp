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


static const uint8_t sensor_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};


static line_color_t line_color;
static bool line_detected = false;
static bool line_out_of_range = false;
static bool last_direction = true;
static int sensor_min[N_SENSORS]; // Valor minimo de cada sensor
static int sensor_max[N_SENSORS]; // Valor maximo de cada sensor

static uint8_t read_calibrated(uint8_t sensor_index);



void LineSensor_init(line_color_t l_color)
{
    line_color = l_color;

    for (int x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = 1023;
        sensor_max[x] = 0; // Se inicializan tal que se puedan sobreescribir
    }
}

void LineSensor_initFromEEPROM(line_color_t l_color)
{
    line_color = l_color;
    EEPROM.begin();
    uint8_t y;
    for (uint8_t x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = ((int)EEPROM.read(2*x)) << 8 | ((int)EEPROM.read(2*x + 1));
        y = x + N_SENSORS;
        sensor_max[x] = ((int)EEPROM.read(2*y)) << 8 | ((int)EEPROM.read(2*y + 1));
    }
}

void LineSensor_saveCalibrationToEEPROM()
{
    uint8_t y;
    for (uint8_t x = 0; x < N_SENSORS; x++)
    {
        EEPROM.write(2*x, (uint8_t)(sensor_min[x] >> 8));
        EEPROM.write(2*x + 1, (uint8_t)(sensor_min[x] & 0x00FF));
        y = x + N_SENSORS;
        EEPROM.write(2*y, sensor_max[x] >> 8);
        EEPROM.write(2*y + 1, (uint8_t)(sensor_max[x] & 0x00FF));
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
        // los mando como 16 bits, Little Endian
        Serial.write(read_calibrated(x));
        Serial.write((uint8_t)0x00);
    }
}

void LineSensor_resetCalibration()
{
    for (int x = 0; x < N_SENSORS; x++)
    {
        sensor_min[x] = 1023;
        sensor_max[x] = 0;
    }
}

void LineSensor_calibrateSensors()
{
        // La calibración sirve para tener el máximo rango de variación de los sensores
        // independientemente de las condiciones de luz y superficies
    int val;
    for (int x = 0; x < N_SENSORS; x++)
    {
        val = ADC_read(sensor_pins[x]);
        sensor_min[x] = (val < sensor_min[x]) ? val : sensor_min[x];
        sensor_max[x] = (val > sensor_max[x]) ? val : sensor_max[x];
    }
}

int LineSensor_read(uint8_t threshold)
{
    line_detected = false;

    unsigned long w_sum = 0;
    unsigned long tot_sum = 0;

    uint8_t detected_lines = 0; // Si se detectan muchas lineas, el robot se levanto

    for (uint8_t x = 0; x < N_SENSORS && (detected_lines < N_SENSORS/2); x++)
    {
        uint8_t val = read_calibrated(x);

        if (val >= threshold)
        { 
            line_detected = true;
            w_sum += val * x * POINTS_PER_SENSOR;    // Suma ponderada por distancia
            tot_sum += val;                         // Valor total acumulado
            detected_lines++;
        }
    }
    int line;
    if (detected_lines >= N_SENSORS/2)
    {
        line_out_of_range = true;
        line_detected = false;
        return 0;
    }
    line_out_of_range = false;
    if (line_detected && (detected_lines < N_SENSORS/2))
    {
        line = (w_sum / tot_sum);                  // Similar a un centro de masa
        line = line - MID_VALUE;                   // El centro queda en 0
    }
    else
    {
        line = (last_direction) ? (MID_VALUE) : -MID_VALUE;                 // Use last direction to calculate error as the maximum value
    }

    last_direction = line > 0;

    return line;
}

static uint8_t read_calibrated(uint8_t idx)
{
    // Scale from 0 to 255
    int val = ADC_read(sensor_pins[idx]);
    val = constrain(val, sensor_min[idx], sensor_max[idx]);
    val = map(val, sensor_min[idx], sensor_max[idx], 0, 255);
    return (line_color == LINE_BLACK) ? (val) : (255 - val);
}

bool LineSensor_lineDetected()
{
    return line_detected;
}

bool LineSensor_lineOutOfRange()
{
    return line_out_of_range;
}