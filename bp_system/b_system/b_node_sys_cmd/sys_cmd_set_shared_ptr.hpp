#pragma once

#include <iostream>
#include <memory>

#include "../../_common/node_cmd.hpp"
#include "../../_common/node_state.hpp"

struct sys_cmd_set_shared_ptr
{
    int node_type;
    int shared_ptr_type;
    std::shared_ptr<node_cmd> node_cmd_ptr;
    std::shared_ptr<node_cmd> node_sys_cmd_ptr;
    std::shared_ptr<st_node_state> node_state_ptr;
};