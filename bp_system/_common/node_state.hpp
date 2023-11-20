#pragma once

#include "node_state_machine.hpp"

struct common_state_code
{
    /* data */
    int node_id;
    node_state_machine state_machine;
    node_state_machine transit_destination_node_state;
    int data_size;
    common_state_code(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        transit_destination_node_state = node_state_machine::UNCONFIGURED;
        data_size = 0;
    }
};


struct st_node_state
{
    common_state_code state_code;
    std::uint8_t data[1024];
};