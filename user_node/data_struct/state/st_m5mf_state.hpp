#pragma once

/*
 * M5Stackの基本データ構造
 * ---
 * BNodeでもStateMachineとか使うので、M5Stack側のStateMachineとかは
 * st_m5_system_stateの中に含めてやる。
 */

// >> API State
#include "m5mf/m5mf_api_state.hpp"

// >> State
#include "m5mf/m5mf_system_state.hpp"
#include "m5mf/m5mf_control_state.hpp"
// #include "m5mf/m5mf_control_state_limited.hpp"
// >> List
#include "m5mf/m5mf_ctrl_mode_list.hpp"