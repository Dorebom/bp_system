#pragma once

#include "node_state_machine.hpp"

struct p_state_code
{
    /* data */
    int node_id;
    node_state_machine state_machine;
    node_state_machine transit_destination_node_state;
    int data_size;
    p_state_code(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        transit_destination_node_state = node_state_machine::UNCONFIGURED;
        data_size = 0;
    }
};

struct st_p_node_state
{
    p_state_code state_code;
    std::uint8_t data[1024];
};