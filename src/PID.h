#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID
{
private:
    /* data */
    float dT;           // [s]
    float integral;
    float last_error;
    float min;
    float max;

    float integral_limit;
    
public:
    PID();
    ~PID();

    float kP;
    float kI;
    float kD;

    unsigned long deltaT_ms();

    void set_freq(unsigned int freq);
    void set_out_limits(float min, float max);
    void set_integral_limit(float integral_limit);

    float integral_value();

    void reset();

    float run(float current, float target);
};



#endif // PID_H