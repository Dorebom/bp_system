#pragma once

#include "_common/node_state.hpp"

struct st_gui_comm_state
{
    /* common data */
    node_state_machine state_machine;
    int error_count;
    int alert_count;
    /* data */
    int encoder_count;

    st_gui_comm_state(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        error_count = 0;
        alert_count = 0;

        encoder_count = 0;
    }
};
