#pragma once

// Base node, cmd and state
#include "../b_system/b_node.hpp"
#include "../_common/node_cmd.hpp"
#include "../_common/node_state.hpp"
//
#include "p_hub_state.hpp"
#include "p_node.hpp"
// List
#include "../_common/node_list.hpp"
#include "../b_system/b_hub_cmd/b_hub_cmd_list.hpp"
// Setting file name
#define PHYSICS_HUB_CONFIG_FILE "physics_hub.json"

class p_hub : public b_node
{
private:
    /* data */
    std::vector<int> running_node_list;
    std::map<int, std::shared_ptr<p_node>> p_node_list;

    /* node_state loop process */
    void _initialize_processing() override;
    void _ready_processing() override;
    void _repair_processing() override;
    void _stable_processing() override;
    void _force_stop_processing() override;
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



    /* Add P Node */

protected:
    std::shared_ptr<node_state> p_hub_state_;    
    std::shared_ptr<node_cmd> p_hub_cmd_;
    std::shared_ptr<node_cmd> p_hub_sys_cmd_;
public:
    p_hub(/* args */);
    ~p_hub();
    void End();
    void exec_node(physics_node_list node_type, behavior_node_list source_node_type);
    void exec_node(physics_node_list node_type, physics_node_list source_node_type);
    void _exec_node(physics_node_list node_type);
    void exit_node(physics_node_list node_type);
    void _exit_node(physics_node_list node_type);
    void pick_shared_ptr(physics_node_list node_type);
    void _get_node_shared_ptr(behavior_node_list node_type);
    void _get_node_shared_ptr(physics_node_list node_type);
};

