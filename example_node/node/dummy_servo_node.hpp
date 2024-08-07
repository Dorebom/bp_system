﻿#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"
// Data struct
#include "../data_struct/state/state_dummy_servo.hpp"
#include "../data_struct/cmd/cmd_dummy_servo.hpp"

class dummy_servo_node : public b_node
{
private:
    /* data */
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

    /* data */
    state_dummy_servo* state_;

    void _update_servo_state();
    //void _reset_encoder_count();

public:
    dummy_servo_node(/* args */);
    ~dummy_servo_node();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<dummy_servo_node>(*this);
    }
};  // class simple_node_a