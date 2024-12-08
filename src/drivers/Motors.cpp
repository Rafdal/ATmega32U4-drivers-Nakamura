#include "Motors.h"

#include "board.h"
#include "PWM.h"

Motors::Motors()
{
    this->L_cfg.pin_pwm = M1_PWM;
    this->L_cfg.pin_dir = M1_DIR;
    this->L_cfg.pin_en = M1_EN;

    this->R_cfg.pin_pwm = M2_PWM;
    this->R_cfg.pin_dir = M2_DIR;
    this->R_cfg.pin_en = M2_EN;
}
Motors::~Motors(){}

void Motors::begin(void)
{
    GPIO_mode(M1_EN, OUTPUT);
	GPIO_mode(M1_DIR, OUTPUT);
	GPIO_mode(M2_EN, OUTPUT);
	GPIO_mode(M2_DIR, OUTPUT);

    // Disable motors
	GPIO_write(this->L_cfg.pin_en, this->L_cfg.active_low);
	GPIO_write(this->L_cfg.pin_dir, this->L_cfg.default_dir);
    GPIO_write(this->R_cfg.pin_en, this->R_cfg.active_low);
    GPIO_write(this->R_cfg.pin_dir, this->R_cfg.default_dir);
    
    // PWM ya tiene hardcodeado los pines PWM
    PWM_setup(PWM_PRESCALER_2, 160, PHASE_FREQ_CORRECT_PWM);
    this->drive(0, 0, FORWARD, FORWARD);
}

// Electric Setup
void Motors::flip_L_R(bool flip)
{
    uint8_t aux_pwm = this->L_cfg.pin_pwm;
    uint8_t aux_dir = this->L_cfg.pin_dir;
    uint8_t aux_en = this->L_cfg.pin_en;
    this->L_cfg.pin_pwm = this->R_cfg.pin_pwm;
    this->L_cfg.pin_dir = this->R_cfg.pin_dir;
    this->L_cfg.pin_en = this->R_cfg.pin_en;
    this->R_cfg.pin_pwm = aux_pwm;
    this->R_cfg.pin_dir = aux_dir;
    this->R_cfg.pin_en = aux_en;
}

void Motors::set_logic(bool dir_L, bool dir_R, bool EN_active_low)
{
    this->L_cfg.active_low = EN_active_low;
    this->L_cfg.default_dir = dir_L;
    this->R_cfg.active_low = EN_active_low;
    this->R_cfg.default_dir = dir_R;

    GPIO_write(this->L_cfg.pin_en, EN_active_low);
    GPIO_write(this->R_cfg.pin_en, EN_active_low);

    GPIO_write(this->L_cfg.pin_dir, dir_L);
    GPIO_write(this->R_cfg.pin_dir, dir_R);
}

// Test
void Motors::test_L_motor(uint8_t power, motor_dir_t dir, unsigned long ms)
{
    this->enable(true, false);
    this->drive(power, 0, dir, FORWARD);
    delay(ms);
    this->drive(0, 0, FORWARD, FORWARD);
    this->enable(false, false);
}

void Motors::test_R_motor(uint8_t power, motor_dir_t dir, unsigned long ms)
{
    this->enable(false, true);
    this->drive(0, power, FORWARD, dir);
    delay(ms);
    this->drive(0, 0, FORWARD, FORWARD);
    this->enable(false, false);
}

void Motors::test_both(uint8_t power, motor_dir_t dir, unsigned long ms)
{
    this->enable(true, true);
    this->drive(power, power, dir, dir);
    delay(ms);
    this->drive(0, 0, FORWARD, FORWARD);
    this->enable(false, false);
}

void Motors::enable(bool enable_L, bool enable_R)
{
    GPIO_write(this->L_cfg.pin_en, this->L_cfg.active_low ^ enable_L);
    GPIO_write(this->R_cfg.pin_en, this->R_cfg.active_low ^ enable_R);
}

void Motors::drive(uint8_t power_L, uint8_t power_R, motor_dir_t dir_L, motor_dir_t dir_R)
{
    GPIO_write(this->L_cfg.pin_dir, this->L_cfg.default_dir ^ dir_L);
    GPIO_write(this->R_cfg.pin_dir, this->R_cfg.default_dir ^ dir_R);

    power_L = (power_L > 100) ? (100) : (power_L);
    power_R = (power_R > 100) ? (100) : (power_R);

    dir_L = (motor_dir_t) this->L_cfg.default_dir ^ dir_L;
    dir_R = (motor_dir_t) this->R_cfg.default_dir ^ dir_R;
    power_L = (dir_L == FORWARD) ? (power_L) : (100 - power_L);
    power_R = (dir_R == FORWARD) ? (power_R) : (100 - power_R);
    PWM_write((PWM_pin_t)this->L_cfg.pin_pwm, power_L);
    PWM_write((PWM_pin_t)this->R_cfg.pin_pwm, power_R);
}