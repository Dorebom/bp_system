#pragma once

enum class b_hub_sys_cmd_list : int
{
    CHANGE_STABLE = 0,
    CHANGE_REPAIR = 1,
    PUSH_FORCE_STOP = 2,
    RELEASE_FORCE_STOP = 3,
    RESET = 4,
    START = 5,
    END = 6,

    PERIOD_NODE // この行は削除しないこと
};