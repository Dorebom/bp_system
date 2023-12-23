#pragma once

#include <vector>
#include <map>

#include "../_common/node.hpp"
#include "../p_system/p_hub.hpp"

#include "../_common/node_list.hpp"
#include "../_common/node_util.hpp"

#include "b_hub_state.hpp"
#include "b_hub_cmd/b_hub_cmd_list.hpp"
#include "b_hub_sys_cmd/b_hub_sys_cmd_list.hpp"
#include "b_node_sys_cmd/b_node_sys_cmd_list.hpp"

#include "node/b_simple_node_a.hpp"
#include "node/b_simple_node_b.hpp"
#include "node/b_example_sub_servo.hpp"
#include "node/b_example_pub_control.hpp"

#define BEHAVIOR_HUB_CONFIG_FILE "behavior_hub.json"

class b_hub : public node
{
private:
    /* data */
    /* node_state loop process */
    void initialize_processing() override;
    void ready_processing() override;
    void repair_processing() override;
    void stable_processing() override;
    void force_stop_processing() override;
    void transit_processing() override;
    // just once execute in End()
    void end_processing() override;
    /* node state change process */
    // -> initialize
    bool any_to_initialize_processing() override;
    // -> ready (reset process)
    bool any_to_ready_processing() override; // stable, repair and ready
    // -> force stop
    bool any_to_force_stop_processing() override; // stable, repair and ready
    // -> normal flow
    bool ready_to_repair_processing() override;
    bool ready_to_stable_processing() override;
    bool repair_to_stable_processing() override;
    bool stable_to_repair_processing() override;

    void _configure() override;
    void set_config(nlohmann::json json_data) override;
    void _set_state() override;
    void _require_node_list(behavior_node_list destination_node);

    std::vector<int> running_node_list;
    bool check_usable_node_list(const behavior_node_list value);
    
    void cmd_executor();
    void _sys_cmd_executor();
    std::vector<int> requirement_list;
    std::map<int, int> waiting_cmd_list;
    std::map<int, int> waiting_state_list;
    std::map<int, std::shared_ptr<node_cmd>> node_cmd_list;
    std::map<int, std::shared_ptr<node_state>> node_state_list;
    std::map<int, std::shared_ptr<node_cmd>> node_sys_cmd_list;
    std::map<int, std::shared_ptr<b_node>> b_node_list;
    /* Add B node */
    //std::shared_ptr<b_node> p_hub_ = std::make_shared<p_hub>();
    //std::shared_ptr<b_node> b_simple_node_a_ = std::make_shared<b_simple_node_a>();
    //std::shared_ptr<b_node> b_simple_node_b_ = std::make_shared<b_simple_node_b>();
    //std::shared_ptr<b_node> b_example_sub_servo_ = std::make_shared<b_example_sub_servo>();
    //std::shared_ptr<b_node> b_example_pub_control_ = std::make_shared<b_example_pub_control>();   
public:
    b_hub(/* args */);
    ~b_hub();
    void End();
    void exec_node(behavior_node_list node_type, behavior_node_list source_node_type);
    void _exec_node(behavior_node_list node_type);
    void exit_node(behavior_node_list node_type);
    void _exit_node(behavior_node_list node_type);
    void pick_shared_ptr(behavior_node_list node_type);
    void _get_node_shared_ptr(behavior_node_list node_type);
};

