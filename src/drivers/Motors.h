#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

typedef enum MOTOR_DIR {
    FORWARD = 0,
    REVERSE = 1
} motor_dir_t;

class Motors {
private:
        typedef struct MOTOR_CFG {
            MOTOR_CFG() : active_low(0), default_dir(0), pin_pwm(0), pin_dir(0), pin_en(0), comp(0) {}
            bool active_low;
            bool default_dir;
            uint8_t pin_pwm;
            uint8_t pin_dir;
            uint8_t pin_en;
            float comp;
        } motor_cfg_t;

        motor_cfg_t L_cfg;
        motor_cfg_t R_cfg;


public:

    Motors();
    ~Motors();

    void begin(void);

    // Electric Setup
    void flip_L_R(bool flip);
    void set_logic(bool dir_L, bool dir_R, bool EN_active_low = false);
    void set_compensation(float comp_L, float comp_R);

    // Test
    void test_L_motor(uint8_t power, motor_dir_t dir = FORWARD, unsigned long msecs = 1000);
    void test_R_motor(uint8_t power, motor_dir_t dir = FORWARD, unsigned long msecs = 1000);
    void test_both(uint8_t power, motor_dir_t dir = FORWARD, unsigned long msecs = 1000);

    // Activar puente H (true = activo, false = desactivado)
    void enable(bool enable_L, bool enable_R);
    void disable(void);

    /**
     * @brief Maneja la velocidad y dirección de los motores
     * @param power_L Potencia del motor izquierdo (0-100)
     * @param power_R Potencia del motor derecho (0-100)
     * @param dir_L Dirección del motor izquierdo (FORWARD o REVERSE)
     * @param dir_R Dirección del motor derecho (FORWARD o REVERSE)
     */
    void drive(uint8_t power_L, uint8_t power_R, motor_dir_t dir_L, motor_dir_t dir_R);

    /**
     * @brief Maneja la velocidad y dirección de los motores de forma más simple
     * @param speed Potencia de ambos motores (-100 a 100)
     * @param turn Dirección de ambos motores, de -100 (Izq) a 100 (Der)
     */
    void drive_high_level(uint8_t power, int speed, int turn);

};

#endif // MOTORS_H