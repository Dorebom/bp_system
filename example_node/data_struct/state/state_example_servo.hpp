#pragma once

#include <cmath>

#include "_common/node_state.hpp"

struct state_example_servo
{
    /* data */
    node_state_machine state_machine;
    int error_count;
    int alert_count;
    int encoder_count;

    state_example_servo(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        error_count = 0;
        alert_count = 0;

        encoder_count = 0;
    }    
};
