#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"

#include "../data_struct/state/st_basic_m5stack_state.hpp"
#include "../data_struct/cmd/st_basic_m5stack_cmd.hpp"
#include "_common/comm/st_udp_frame.hpp"

#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class BasicM5stack : public b_node
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
    st_basic_m5stack_state* state_;
    //bool is_display_state_;
    bool is_logging_ = false;
    std::vector<st_m5_control_state> control_state_log;


    /* user function */
    void _update_state();
    void connect();
    void start_logging();
    void stop_logging();
    void logging();

    //void set_param(st_imu_param_cmd param);
public:
    BasicM5stack(/* args */);
    ~BasicM5stack();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<BasicM5stack>(*this);
    }
};  // class BasicM5stack