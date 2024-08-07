#pragma once

#include "cmd_start_node.hpp"
#include "cmd_end_node.hpp"
#include "cmd_pick_shared_ptr.hpp"

enum class b_hub_cmd_list : int
{
    START_NODE = 0,
    END_NODE = 1,
    PICK_SHARED_PTR = 2,
    LOAD_NODE_LIST = 3,
    START_P_NODE = 4,
    DELETE_SHARED_PTR = 5,
    START_GUI = 6,
    END_GUI = 7,
    SEND_HUB_INFO = 8,
    SEND_NODES_INFO = 9,
    STREAM_NODE_DATA = 10,
    CHANGE_NODE_SM = 11,
    RESET_NODE = 12,

    PERIOD_NODE // この行は削除しないこと
};