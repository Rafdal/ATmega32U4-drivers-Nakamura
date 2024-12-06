/***************************************************************************//**
  @file     App.c
  @brief    Aplicación de alto nivel
  @author   RAS-ITBA
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Arduino.h"
#include "App.h"
#include "DRV_Board.h"
#include "DRV_Motors.h"
#include "DRV_Sensors.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LINE_COLOR   LINE_WHITE         //Line color

#define LEFT_DIR             0
#define RIGHT_DIR            1
#define CAL_SPEED           300   //Sensor calibration speed
#define CAL_TIME            300   //Calibration time
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


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void race(void);
static void calibration_routine(int cal_time, int cal_speed, int cal_dir);
static float get_PID_correction(float line, float last_line, float kp, float kd, float ki);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static unsigned long ms = 0;
static int start = 0;
static float line_pos = 0;
static float last_line_pos = 0;


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

    setLED(LEDG, readButton(BTN1));
    setLED(LEDY, readButton(BTN2));
    setLED(LEDR, readButton(BTN3));

    if(readButton(BTN1)){
        setMotorSpeed(200, 200);
    }else if(readButton(BTN2)){
        setMotorSpeed(400, -200);
    }else if(readButton(BTN3)){
        setMotorSpeed(-600, 400);
    }else{
        setMotorSpeed(0, 0);
    }

    /*
    if(readButton(BTN1)){    //Calibrate sensors
        while(readButton(BTN1));
        delay(500);
        
        calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
    }
    
    if(readButton(BTN2)){              //Start race
        start = 1;
        while(readButton(BTN2)){
            setLED(LEDY, millis()%100 < 50);
            if(readButton(BTN1)){      //Cancel race
                start = 0;              
            }
        }
        if(start == 1){         //Race not cancelled
            delay(100);         //Wait for hand to get out of the way
            while( !readButton(BTN1) || !readButton(BTN2) ){      //Press any button to exit, otherwise race
                race();
                delay(15);
            }
            delay(2000);
        }
    }*/
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

void calibration_routine(int cal_time, int cal_speed, int cal_dir){
  unsigned long long ms = millis();
  setLED(LEDY, 0);
  while((ms + cal_time) > millis()){
    setLED(LEDY, millis()%100 < 50);        //Blink led
    if(cal_dir == RIGHT_DIR)  setMotorSpeed(cal_speed, -cal_speed);
    if(cal_dir == LEFT_DIR)  setMotorSpeed(-cal_speed, cal_speed);
    
    calibrateSensors();
  }
  setMotorSpeed(0, 0);
  setLED(LEDY, 1);
}

float get_PID_correction(float line, float last_line, float kp, float kd, float ki){
  float proportional = line;
  float derivative = line - last_line;
  float integral = line + last_line;
  float correction = (kp * proportional + kd * derivative + ki * integral);

  return correction;
}