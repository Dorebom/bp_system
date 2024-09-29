#pragma once

#include <cmath>

#include "_common/node_state.hpp"

struct st_imu_state
{
    /* data */
    node_state_machine state_machine;
    int error_count;
    int alert_count;

    double linear_acceleration[3];
    double angular_velocity[3];
    double orientation[3];
    double mag[3];
    double temperature;
    double pressure;
    double altitude;

    st_imu_state(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        error_count = 0;
        alert_count = 0;
    }
};
