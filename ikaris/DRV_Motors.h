/***************************************************************************//**
  @file     DRV_Motors.h
  @brief    Driver de motores
  @author   RAS_ITBA
 ******************************************************************************/

#ifndef _DRV_MOTORS_H_
#define _DRV_MOTORS_H_

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializar los motores
*/
void initMotors();

/**
 * @brief Configurar la velocidad de los motores
 * @param left_speed: Velocidad del motor izquierdo (-1000 a 1000)
 * @param right_speed: Velocidad del motor derecho (-1000 a 1000)
*/
void setMotorSpeed(int left_speed, int right_speed);    //From -1000 to 1000

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_MOTORS_H_
