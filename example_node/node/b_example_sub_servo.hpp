#pragma once

// Base node, cmd and state
#include "b_system/b_node.hpp"
#include "_common/node_cmd.hpp"
#include "_common/node_state.hpp"
// List
#include "_common/node_list.hpp"
#include "b_system/b_hub_cmd/b_hub_cmd_list.hpp"
#include "../data_struct/state/b_node_state_list.hpp"
#include "../data_struct/cmd/b_node_cmd_list.hpp"

#include <random>

class b_example_sub_servo : public b_node
{
private:
    /* node_state loop process */
    void _initialize_processing() override;
    void _ready_processing() override;
    void _repair_processing() override;
    void _stable_processing() override;
    void _force_stop_processing() override;
    void _end_processing() override;

    /* once node state change process */
    bool _any_to_initialize_processing() override;
    bool _any_to_ready_processing() override;
    bool _any_to_force_stop_processing() override;
    bool _ready_to_repair_processing() override;
    bool _ready_to_stable_processing() override;
    bool _repair_to_stable_processing() override;
    bool _stable_to_repair_processing() override;

    // once after transiting
    bool _any_to_initialize_processing_after() override;
    bool _any_to_ready_processing_after() override;
    bool _any_to_force_stop_processing_after() override;
    bool _ready_to_repair_processing_after() override;
    bool _ready_to_stable_processing_after() override;
    bool _repair_to_stable_processing_after() override;
    bool _stable_to_repair_processing_after() override;

    void _configure() override;
    void _set_config(nlohmann::json json_data) override;
    void _set_state() override;
    void cmd_executor();
    
    /* data */
    state_example_servo* state_;

    void _update_servo_state(int add_value);
    void _reset_encoder_count();
    void _display_servo_state();

public:
    b_example_sub_servo(/* args */);
    ~b_example_sub_servo();
    std::shared_ptr<b_node> Clone() const override{
        return std::make_shared<b_example_sub_servo>(*this);
    }
}; // class b_example_sub_servo
