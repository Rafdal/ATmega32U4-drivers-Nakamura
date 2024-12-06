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

static const int SENSOR_PIN[] = {PIN_SENS1, PIN_SENS2, PIN_SENS3, PIN_SENS4, PIN_SENS5, PIN_SENS6, PIN_SENS7, PIN_SENS8};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static line_color_t line_color;
static bool line_detected = false;
static int sensor_max[N_SENSORS];          //Maximum value each sensor measures (calibrated)
static int sensor_min[N_SENSORS];          //Minimum value each sensor measures (calibrated)

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initSensors(line_color_t l_color){
    line_color = l_color;

    for(int x = 0; x < N_SENSORS; x++){
        sensor_max[x] = 0;
        sensor_min[x] = 1023;
    }

    pinMode(PIN_SENSEN, OUTPUT);
    for(int x = 0; x <= N_SENSORS; x++){
        pinMode(SENSOR_PIN[x], INPUT);
    }
}

void calibrateSensors(){
    int sensor_value[N_SENSORS];
    for(int x = 0; x < N_SENSORS; x++){
      sensor_value[x] = analogRead(SENSOR_PIN[x]);
      sensor_min[x] = (sensor_value[x] < sensor_min[x]) ? sensor_value[x] : sensor_min[x];
      sensor_max[x] = (sensor_value[x] > sensor_max[x]) ? sensor_value[x] : sensor_max[x];
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