#pragma once

#include "../_common/node_state.hpp"

struct  p_hub_state
{
    /* data */
    node_state_machine state_machine;
    p_hub_state(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
    }
};
