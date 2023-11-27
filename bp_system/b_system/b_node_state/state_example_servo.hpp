#pragma once

#include <cmath>

struct state_example_servo
{
    /* data */
    double servo_angle;
    double cmd_servo_anglur_velocity;
    state_example_servo(/* args */)
    {
        cmd_servo_anglur_velocity = 0.0;
    }    
};
