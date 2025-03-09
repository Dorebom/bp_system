#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"

#include "../data_struct/state/st_m5mf_state.hpp"
#include "../data_struct/cmd/st_m5mf_cmd.hpp"
#include "_common/comm/st_udp_frame.hpp"
//
#include "../data_struct//state/m5mf/m5mf_ctrl_mode_list.hpp"
#include "../data_struct//state/m5mf/m5mf_mechanical_frame_list.hpp"
//
#include "../data_struct/cmd/st_joycon_cmd.hpp"
//
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class M5mfApi : public b_node
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
    void cmd_executor();

    /* UDP */
    void _task_recv() override;
    CommUdp comm_m5stack_;


    /* user data */
    M5MF::M5mfApiState* state_;
    //bool is_display_state_;
    bool is_logging_ = false;
    std::vector<M5MF::M5MFAPIControlState> control_state_log;
    bool is_streaming_ = false;
    int stream_watchdog_cnt = 0;

    /* user function */
    void check_error_and_alert();
    void _update_state();
    void cmd_connect();
    // >> change State Machine
    void cmd_change_forcestop();
    void cmd_release_forcestop();
    // >> state stream
    void cmd_start_stream();
    void cmd_stop_stream();
    // >> logging
    void cmd_start_logging();
    void cmd_stop_logging();
    void start_logging();
    void stop_logging();
    void logging();

    // >> control system command (for M5Stack)
    void cmd_servo_on();
    void cmd_servo_off();
    //
    void cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST ctrl_mode);
    void cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST mf_type);
    void cmd_set_mechanical_position_to_zero();
    //
    void cmd_move(JoyconMoveCmd* cmd);
    void cmd_alljoint_position_control();

    
    // >> API command
    int display_cnt = 0;
    void display_state();

    //void set_param(st_imu_param_cmd param);
public:
    M5mfApi(/* args */);
    ~M5mfApi();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<M5mfApi>(*this);
    }
};  // class M5mfApi