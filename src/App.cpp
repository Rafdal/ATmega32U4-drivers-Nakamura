/***************************************************************************//**
  @file     App.c
  @author   RAS-ITBA
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <Arduino.h>
#include "App.h"
#include "ADC.h"
#include "GPIO.h"
#include "PWM.h"
#include "SENSORS.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LINE_COLOR   LINE_WHITE         //Line color

#define LEFT_DIRECTION             0
#define RIGHT_DIRECTION            1
#define CAL_TIME            4000   //Calibration time in ms
#define P_LINE_MIN          0.75  //Minimum brightness percentage to consider part of the line
#define P_MAX_CORRECTION    175   //(100 + percentage) Maximum percentage of SPEED that a wheel can go in reverse for maximum correction
#define POR_UMBRAL_LINEA    85    //Porcentaje considerado recta
#define SP_REDUCTION        0.5       //Porcentaje de 0 a 1 para bajar la velocidad

#define SPEED   750
#define KP      0.7
#define KD      1.4
#define KI      0



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {
  IDLE,
  CALIBRATING,
  RACING,
}rece_mode_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void race(void);
static void calibration_routine(int cal_time);
static float get_PID_correction(float line, float last_line, float kp, float kd, float ki);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static unsigned long ms = 0;
static int start = 0;
static float line_pos = 0;
static float last_line_pos = 0;
static rece_mode_t robot_state =IDLE;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initApplication() {
  initBoard();
  initMotors();
  initSensors(LINE_COLOR);
}

void runApplication() {

  switch (robot_state)
  {
  case IDLE:
    
    break;

  case CALIBRATING:
    calibration_routine(CAL_TIME);
    robot_state = IDLE;
    break;

  case RACING:
    /* code */
    break;

  default:
    break;
  }
   
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void race(void){
  last_line_pos = line_pos;
  line_pos = getLinePosition(LINE_COLOR);
  float PID_correction, max_correction, mspeed;

  PID_correction = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);
  mspeed = SPEED;
  max_correction = (P_MAX_CORRECTION * mspeed) / 100;      //Can be changed to a lower value in order to limit the correction, needs to be at most 2*SPEED
    
  if(PID_correction > 0){
    PID_correction = (PID_correction > max_correction)? max_correction : PID_correction;
    setMotorSpeed(mspeed, mspeed - PID_correction);
  }else{
    PID_correction = (PID_correction < -max_correction)? -max_correction : PID_correction;
    setMotorSpeed(mspeed + PID_correction, mspeed);
  }    
}

void calibration_routine(int cal_time){
  unsigned long long ms = millis();
	GPIO_write(LED_G1, LOW);
  while((ms + cal_time) > millis()){
    GPIO_write(LED_G1, (millis()%100 < 50));        //Blink cada 50ms
    calibrateSensors();
  }

}

float get_PID_correction(float line, float last_line, float kp, float kd, float ki){
  float proportional = line;
  float derivative = line - last_line;
  float integral = line + last_line;
  float correction = (kp * proportional + kd * derivative + ki * integral);

  return correction;
}