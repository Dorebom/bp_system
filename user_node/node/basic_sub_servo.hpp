#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"

#include "../data_struct/state/st_servo_state.hpp"
#include "../data_struct/cmd/st_servo_cmd.hpp"

#include <random>

class BasicSubServo : public b_node
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

    /* user data */
    st_servo_state* state_;
    bool is_display_state_;

    /* user function */
    void _update_servo_state(int add_value);
    void _reset_encoder_count();
    void _display_servo_state();

    //void set_param(st_imu_param_cmd param);
public:
    BasicSubServo(/* args */);
    ~BasicSubServo();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<BasicSubServo>(*this);
    }
};  // class simple_node_a