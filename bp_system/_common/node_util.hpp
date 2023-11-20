#pragma once

#include <string>
#include "node_state_machine.hpp"
#include "node_list.hpp"

/* 目的：デバッグ用の、名前を取得する関数群 */

std::string get_node_state_machine_name(node_state_machine state);

std::string get_b_node_name(behavior_node_list node);
std::string get_p_node_name(physics_node_list node);
