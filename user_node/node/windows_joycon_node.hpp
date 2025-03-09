#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"

#include "../data_struct/state/st_joycon_state.hpp"
//
#include "../data_struct/state/st_m5mf_state.hpp"
#include "../data_struct/cmd/st_m5mf_cmd.hpp"
//
#include "../data_struct/state/m5mf/m5mf_ctrl_mode_list.hpp"
#include "../data_struct/state/m5mf/m5mf_mechanical_frame_list.hpp"

#include "../data_struct/cmd/st_joycon_cmd.hpp"

#pragma comment(lib, "winmm.lib")

#include <stdio.h>
#include <windows.h>

#include <iostream>


class BasicJoyconNode : public b_node
{
private:
    /* node_state loop process */
    void _initialize_processing() override;
    void _ready_processing() override;
    void _repair_processing() override;
    void _stable_processing() override;
    void _force_stop_processing() override;
    void _end_processing() override;
    //void transit_processing() override;
    /* node state change process */
    // -> initialize
    bool _any_to_initialize_processing() override;
    // -> ready (reset process)
    bool _any_to_ready_processing() override; // stable, repair and ready
    // -> force stop
    bool _any_to_force_stop_processing() override; // stable, repair and ready
    // -> normal flow
    bool _ready_to_repair_processing() override;
    bool _ready_to_stable_processing() override;
    bool _repair_to_stable_processing() override;
    bool _stable_to_repair_processing() override;
    // -> after transiting
    bool _any_to_initialize_processing_after() override;
    bool _any_to_ready_processing_after() override;
    bool _any_to_force_stop_processing_after() override;
    bool _ready_to_repair_processing_after() override;
    bool _ready_to_stable_processing_after() override;
    bool _repair_to_stable_processing_after() override;
    bool _stable_to_repair_processing_after() override;

    void _configure() override;
    void _set_config(nlohmann::json &json_data) override;
    void _set_state() override;
    //void cmd_executor();

    /* user data */
    JOYINFOEX JoyInfoEx;

    st_joycon_state* joycon_state_;
    int joy_fd;
    int num_of_axis;
    int num_of_buttons;
    char name_of_joystick[80];
    st_joycon_state prev_joycon_state_;

    //
    st_node_state* m5mf_node_state_;
    M5MF::M5mfApiState* m5mf_api_state_;
    bool is_get_rel_shared_ptr;
    int m5mf_api_id;

    bool is_start_stream = false;
    bool is_start_logging = false;
    bool is_force_stop = false;
    bool is_power_on = false;
    bool flag_a = false;
    bool flag_b = false;
	bool flag_c = false;
	bool flag_d = false;
    bool is_set_joint_zero = false;
    bool prev_is_set_joint_zero = false;
	bool is_set_torque_limit_released = false;
	bool prev_is_set_torque_limit_released = false;

    /* user function */
    void get_state();
    void display_state();
    void push_cmd_to_m5mf_node(st_node_cmd* cmd);
public:
    BasicJoyconNode(/* args */);
    ~BasicJoyconNode();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<BasicJoyconNode>(*this);
    }
};  // class simple_node_a