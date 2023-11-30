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

class b_example_sub_servo : public b_node
{
private:
    /* data */
    /* node_state loop process */
    void _initialize_processing() override;
    void _ready_processing() override;
    void _repair_processing() override;
    void _stable_processing() override;
    void _force_stop_processing() override;
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

    void _configure() override;
    void _set_config(nlohmann::json json_data) override;
    void _set_state() override;
    void cmd_executor();
    
    state_example_servo* state_;
    void _update_servo_state();

public:
    b_example_sub_servo(/* args */);
    ~b_example_sub_servo();
}; // class b_example_sub_servo