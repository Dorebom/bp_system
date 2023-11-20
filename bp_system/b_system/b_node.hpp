#pragma once

#include "../_common/node.hpp"
/* Behavior System */
#include "../_common/node_cmd.hpp"
#include "b_node_sys_cmd/b_node_sys_cmd_list.hpp"
#include "b_hub_cmd/b_hub_cmd_list.hpp"
#include "../_common/node_list.hpp"
/* Physics System */
#include "../_common/p_node_state.hpp"
#include "../_common/p_node_cmd.hpp"

class b_node : public node
{
private:
protected:
    behavior_node_list this_node;
    std::vector<int> requirement_b_node_list;
    std::vector<int> requirement_p_node_list;
    /* My Item */
    std::shared_ptr<st_node_state> b_node_state_;
    std::shared_ptr<node_cmd> b_node_cmd_;
    std::shared_ptr<node_cmd> b_node_sys_cmd_;
    /* B Hub Item */
    std::shared_ptr<node_cmd> b_hub_cmd_;
    /* Relative B Nodes Item */
    std::map<int, std::shared_ptr<node_cmd>> b_node_cmd_map;
    std::map<int, std::shared_ptr<st_node_state>> b_node_state_map;
    std::map<int, std::shared_ptr<node_cmd>> b_node_sys_cmd_map;
    std::map<int, std::shared_ptr<st_node_state>> user_b_node_state_map;
    /* Relative P Node Item */
    std::map<int, std::shared_ptr<st_p_node_state>> p_node_state_map;
    std::map<int, std::shared_ptr<node_cmd>> p_node_cmd_map;
    std::map<int, std::shared_ptr<node_cmd>> p_node_sys_cmd_map;
    std::map<int, std::shared_ptr<st_p_node_state>> user_p_node_state_map;

    void _sys_cmd_executor();
    void _start_node(behavior_node_list source_node);
    void set_shared_ptr(st_node_cmd cmd);
    void hub_cmd_set_shared_ptr(behavior_node_list source_node);
    void set_config(nlohmann::json json_data) override;
    virtual void _set_config(nlohmann::json json_data) = 0;
    
    /* node_state loop process */
    void initialize_processing() override;
    void ready_processing() override;
    void repair_processing() override;
    void stable_processing() override;
    void force_stop_processing() override;
    void transit_processing() override;

    virtual void _initialize_processing() = 0;
    virtual void _ready_processing() = 0;
    virtual void _repair_processing() = 0;
    virtual void _stable_processing() = 0;
    virtual void _force_stop_processing() = 0;

    /* node state change process */
    bool any_to_initialize_processing() override;
    bool any_to_ready_processing() override;
    bool any_to_force_stop_processing() override;
    bool ready_to_repair_processing() override;
    bool ready_to_stable_processing() override;
    bool repair_to_stable_processing() override;
    bool stable_to_repair_processing() override;
    // -> initialize
    virtual bool _any_to_initialize_processing() = 0;
    // -> ready (reset process)
    virtual bool _any_to_ready_processing() = 0; // stable, repair
    // -> force stop
    virtual bool _any_to_force_stop_processing() = 0; // stable, repair and ready
    // -> normal flow
    virtual bool _ready_to_repair_processing() = 0;
    virtual bool _ready_to_stable_processing() = 0;
    virtual bool _repair_to_stable_processing() = 0;
    virtual bool _stable_to_repair_processing() = 0;

public:
    b_node(/* args */)
    {
        is_main_thread_running_ = false;
    }
    void Start_Node(bool use_udp_communication, std::shared_ptr<node_cmd> hub_cmd);
    std::shared_ptr<st_node_state> get_state_ptr()
    {
        return b_node_state_;
    }
    std::shared_ptr<node_cmd> get_cmd_ptr()
    {
        return b_node_cmd_;
    }   
    std::shared_ptr<node_cmd> get_sys_cmd_ptr()
    {
        return b_node_sys_cmd_;
    }
    // Behavior System
    void set_relative_b_node_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> cmd);
    void set_relative_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state);
    void set_relative_b_node_sys_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> sys_cmd);
    void set_user_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state);
    // Physics System
    void set_relative_p_node_state_ptr(physics_node_list node_type, std::shared_ptr<st_p_node_state> state);
    void set_relative_p_node_cmd_ptr(physics_node_list node_type, std::shared_ptr<node_cmd> cmd);
    void set_relative_p_node_sys_cmd_ptr(physics_node_list node_type, std::shared_ptr<node_cmd> sys_cmd);
    void set_user_p_node_state_ptr(physics_node_list node_type, std::shared_ptr<st_p_node_state> state);
};