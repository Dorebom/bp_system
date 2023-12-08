#pragma once

// Base node, cmd and state
#include "../../b_system/b_node.hpp"
#include "../../_common/node_cmd.hpp"
#include "../../_common/node_state.hpp"
// List
#include "../../_common/node_list.hpp"
#include "../../b_system/b_hub_cmd/b_hub_cmd_list.hpp"
#include "../../b_system/b_node_state/b_node_state_list.hpp"
#include "../../b_system/b_node_cmd/b_node_cmd_list.hpp"

class b_example_pub_control : public b_node
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
    
    //state_example_control* state_;
    void _update_servo_control();
public:
    b_example_pub_control(/* args */);
    ~b_example_pub_control();
}; // class b_example_pub_control