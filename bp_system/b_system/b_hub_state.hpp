#pragma once

#include "../_common/node_state.hpp"
#include <vector>

#define MAX_NODE_NUM 6
#define MAX_NODE_NAME 20
#define NO_NODE_ID -1

struct b_hub_state
{
    /* data */
    node_state_machine state_machine;
    int running_node_id[MAX_NODE_NUM];    // 空き枠は-1
    char running_node_name[MAX_NODE_NUM][MAX_NODE_NAME];
    b_hub_state(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        for (int i = 0; i < MAX_NODE_NUM; i++)
        {
            running_node_id[i] = NO_NODE_ID;
            memset(running_node_name[i], 0, MAX_NODE_NAME);
        }
    }
};
