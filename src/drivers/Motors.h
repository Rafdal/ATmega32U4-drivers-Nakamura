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
            MOTOR_CFG() : active_low(0), default_dir(0), pin_pwm(0), pin_dir(0), pin_en(0) {}
            bool active_low;
            bool default_dir;
            uint8_t pin_pwm;
            uint8_t pin_dir;
            uint8_t pin_en;
        } motor_cfg_t;

        motor_cfg_t L_cfg;
        motor_cfg_t R_cfg;

public:

    Motors();
    ~Motors();

    void begin(void);

    // Electric Setup
    void flip_L_R(bool flip);
    void set_logic(bool dir_L, bool dir_R, bool EN_active_low);

    // Test
    void test_L_motor(uint8_t power, motor_dir_t dir = FORWARD, unsigned long msecs = 1000);
    void test_R_motor(uint8_t power, motor_dir_t dir = FORWARD, unsigned long msecs = 1000);
    void test_both(uint8_t power, motor_dir_t dir, unsigned long msecs = 1000);

    // control
    void enable(bool enable_L, bool enable_R);
    void drive(uint8_t power_L, uint8_t power_R, motor_dir_t dir_L, motor_dir_t dir_R);

};

#endif // MOTORS_H