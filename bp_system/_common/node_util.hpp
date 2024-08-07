#pragma once

#include <string>
#include "node_state_machine.hpp"

#define BEHAVIOR_HUB_ID 0

/* 目的：デバッグ用の、名前を取得する関数群 */

std::string get_node_state_machine_name(node_state_machine state);