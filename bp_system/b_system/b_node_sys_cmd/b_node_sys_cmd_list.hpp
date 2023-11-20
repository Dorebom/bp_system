#pragma once

#include "sys_cmd_set_shared_ptr.hpp"

enum class b_node_sys_cmd_list : int
{
    CHANGE_STABLE = 0,
    CHANGE_REPAIR = 1,
    CHANGE_READY =2,
    PUSH_FORCE_STOP = 3,
    RELEASE_FORCE_STOP = 4,
    RESET = 5,
    START = 6,
    END = 7,
    SET_SHARED_PTR = 8,
    REQUIRE_NODE_LIST = 9,

    PERIOD_NODE // この行は削除しないこと
};

enum class shared_ptr_type : int
{
    NODE_CMD = 0,
    NODE_SYS_CMD = 1,
    NODE_STATE = 2,

    PERIOD_NODE // この行は削除しないこと
};