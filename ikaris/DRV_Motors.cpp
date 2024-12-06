/***************************************************************************//**
  @file     DRV_Motors.c
  @brief    Driver de motores
  @author   RAS_ITBA
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_Motors.h"
#include "board_definitions.h"

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initMotors(){
    pinMode(PIN_MEN1, OUTPUT);
    pinMode(PIN_MIN1A, OUTPUT);
    pinMode(PIN_MIN1B, OUTPUT);

    pinMode(PIN_MEN2, OUTPUT);
    pinMode(PIN_MIN2A, OUTPUT);
    pinMode(PIN_MIN2B, OUTPUT);

    setMotorSpeed(0, 0);
}

void setMotorSpeed(int left_speed, int right_speed){
    int pwm1 = map(abs(left_speed), 0, 1000, 0, 255);
    int pwm2 = map(abs(right_speed), 0, 1000, 0, 255);
    pwm1 = (left_speed>0) ? 255-pwm1 : pwm1;
    pwm2 = (right_speed>=0) ? pwm2 : 255-pwm2; 

    analogWrite(PIN_MEN1, pwm1);
    digitalWrite(PIN_MIN1A, (left_speed > 0) ? HIGH : LOW);
    digitalWrite(PIN_MIN1B, (left_speed >= 0) ? LOW : HIGH);

    analogWrite(PIN_MEN2, pwm2);
    digitalWrite(PIN_MIN2A, (right_speed > 0) ? HIGH : LOW);
    digitalWrite(PIN_MIN2B, (right_speed >= 0) ? LOW : HIGH);
}