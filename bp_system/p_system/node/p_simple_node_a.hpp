#pragma once

#include "../../p_system/p_node.hpp"
#include "../../_common/p_node_cmd.hpp"
#include "../../_common/p_node_state.hpp"
// List
#include "../../_common/node_list.hpp"
#include "../../p_system/p_hub_cmd/p_hub_cmd_list.hpp"

class p_simple_node_a : public p_node
{
private:
    /* data */
    /* node_state loop process */
    void _initialize_processing() override;
    void _ready_processing() override;
    void _repair_processing() override;
    void _stable_processing() override;
    void _force_stop_processing() override;
    //void _transit_processing() override;
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
public:
    p_simple_node_a(/* args */);
    ~p_simple_node_a();
};  // class simple_node_a
