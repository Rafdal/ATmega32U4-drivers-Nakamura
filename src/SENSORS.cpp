/***************************************************************************//**
  @file     DRV_Sensors.c
  @brief    Drivers del arreglo de sensores de linea
  @author   RAS-ITBA
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_Sensors.h"
#include "board_definitions.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
 
#define SENSOR_RANGE    1000        //Sensor readings are mapped to this range
#define SENSOR_WEIGHTS  LINE_RANGE  //Unit for weighted average, should be the same as the resulting line range

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const uint8_t sensor_pins[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static line_color_t line_color;
static bool line_detected = false;
static uint8_t sensor_max[N_SENSORS];          //Valor maximo de cada sensor
static uint8_t sensor_min[N_SENSORS];          //Valor minimo de cada sensor

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initSensors(line_color_t l_color){
    line_color = l_color;
    
    for(int x = 0; x < N_SENSORS; x++){ 
        sensor_max[x] = 0;                  //Se inicializan tal que se puedan sobreescribir
        sensor_min[x] = 255;
    }
}

void calibrateSensors(){
    int sensor_value[N_SENSORS];
    for(int x = 0; x < N_SENSORS; x++){
      sensor_value[x] = ADC_read(sensor_pins[i]) >> 2;          // Usamos 8 de los 10 bits
      sensor_min[x] = (sensor_value[x] < sensor_min[x]) ? sensor_value[x] : sensor_min[x];      //Si es menor al valor minimo guardado, lo reemplazo
      sensor_max[x] = (sensor_value[x] > sensor_max[x]) ? sensor_value[x] : sensor_max[x];      // Analogo al minimo
    }
}

float getLinePosition(float minimum_brightness){
    float line = 0;
    float sensor_scaled[N_SENSORS];
    float avg_num = 0;                  //Average numerator
    float avg_den = 0;                  //Average denominator
    static bool last_direction;

    line_detected = 0;
    
    for(int x = 0; x < N_SENSORS; x++){
        //Scale from 0 to SENSOR_RANGE
        sensor_scaled[x] = map(analogRead(SENSOR_PIN[x]), sensor_min[x], sensor_max[x], 0, SENSOR_RANGE);
        
        if(line_color == LINE_BLACK){
            sensor_scaled[x] = SENSOR_RANGE - sensor_scaled[x];             //Reverse scale to go from SENSOR_RANGE to 0
        }
        if(sensor_scaled[x] >= (float) SENSOR_RANGE * minimum_brightness){  //At least one sensor has to detect a line
            line_detected = 1;
        }
        
        avg_num += sensor_scaled[x] * x * SENSOR_WEIGHTS;
        avg_den += sensor_scaled[x];        
    }
    if(line_detected == 1){
        line = avg_num / avg_den;                               //Weighted average
        line = line/((N_SENSORS - 1) / 2) - SENSOR_WEIGHTS;     //Change scale from 0_7000 to -1000 _ 1000
    }else{
        line = SENSOR_WEIGHTS * last_direction;                 //Use last direction to calculate error as the maximum value
    }
    
    last_direction = line > 0;

    return line;
}


bool lineWasDetected(){
    return line_detected;
}