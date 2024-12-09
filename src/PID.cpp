#include "PID.h"

PID::PID()
{
    this->kP = 0.0;
    this->kI = 0.0;
    this->kD = 0.0;
    this->dT = 1.0;
    this->integral = 0.0;
    this->last_error = 0.0;
    this->min = NAN;
    this->max = NAN;
}

PID::~PID(){}


void PID::set_freq(unsigned int freq)
{
    this->dT = 1.0/freq;
}

unsigned long PID::deltaT_ms()
{
    return (unsigned long)(this->dT * 1000);
}

void PID::set_K(float kP, float kI, float kD)
{
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

void PID::set_out_limits(float min, float max)
{
    this->min = min;
    this->max = max;
}

void PID::set_integral_limit(float integral_limit)
{
    this->integral_limit = integral_limit;
}

float PID::integral_value()
{
    return this->integral;
}

void PID::reset()
{
    this->integral = 0;
    this->last_error = 0;
}

float PID::run(float current, float target)
{
    float error = target - current;
    float out = 0;

    if (kI != 0)
    {
        integral += error * dT;
        // Saturate integral
        if (integral_limit != 0 && abs(integral) > integral_limit)
            integral = (integral > 0) ? integral_limit : -integral_limit;
        out += kI * integral;
    }
    if (kD != 0)
    {
        out += kD * (error - last_error) / dT;;
    }

    out += kP * error;

    if (!isnan(max))
        out = (out > max) ? max : out;
    if (!isnan(min))

        out = (out < min) ? min : out;
    last_error = error;

    return out;
}