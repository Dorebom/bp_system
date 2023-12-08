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

    PERIOD_NODE // この行は削除しないこと
};