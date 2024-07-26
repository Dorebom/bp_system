#pragma once

#include "../_common/node.hpp"
/* Behavior System */
#include "../_common/node_cmd.hpp"
#include "b_node_sys_cmd/b_node_sys_cmd_list.hpp"
#include "b_hub_cmd/b_hub_cmd_list.hpp"

struct st_require_node_info
{
    std::string node_type;
    std::string node_jsonfile;
    std::string node_folder;
    st_require_node_info(/* args */)
    {
        node_type = "";
        node_jsonfile = "";
        node_folder = "";
    }
};

class b_node : public node
{
private:
protected:
    int node_id_;
    std::vector<st_require_node_info> requirement_node_list;
    /* B Hub Item */
    std::shared_ptr<node_cmd> b_hub_cmd_;
    /* Relative B Nodes Item */
    std::map<int, std::shared_ptr<node_cmd>> b_node_cmd_map;
    std::map<int, std::shared_ptr<node_state>> b_node_state_map;
    std::map<int, std::shared_ptr<node_cmd>> b_node_sys_cmd_map;
    std::map<int, std::shared_ptr<node_state>> user_b_node_state_map;

    std::map<int, std::string> rel_node_id2name_map;
    std::map<std::string, std::string> rel_node_jsonfile2name_map;
    std::map<std::string, int> rel_node_jsonfile2id_map;
    std::map<std::string, int> rel_node_name2id_map;

    void _sys_cmd_executor();
    void _start_node(int source_node_id);
    void set_shared_ptr(st_node_cmd cmd);
    void set_config(nlohmann::json json_data) override;
    virtual void _set_config(nlohmann::json json_data) = 0;
    common_cmd_code get_invoice_to_hub();
    void send_cmd_to_hub(b_hub_cmd_list cmd_type, std::uint8_t* cmd_data);
<<<<<<< HEAD
    void send_sys_cmd_to_node(b_hub_cmd_list cmd_type, std::uint8_t* cmd_data, behavior_node_list destination_node);
=======
    //void send_sys_cmd_to_node(b_hub_cmd_list cmd_type, std::uint8_t* cmd_data, behavior_node_list destination_node);
>>>>>>> separate_system_usernode

    /* node_state loop process */
    void initialize_processing() override;
    void ready_processing() override;
    void repair_processing() override;
    void stable_processing() override;
    void force_stop_processing() override;
    void transit_processing() override;
    // just once execute in End()
    void end_processing() override;

    virtual void _initialize_processing() = 0;
    virtual void _ready_processing() = 0;
    virtual void _repair_processing() = 0;
    virtual void _stable_processing() = 0;
    virtual void _force_stop_processing() = 0;
    virtual void _end_processing() = 0;

    /* node state change process */
    bool any_to_initialize_processing() override;
    bool any_to_ready_processing() override;
    bool any_to_force_stop_processing() override;
    bool ready_to_repair_processing() override;
    bool ready_to_stable_processing() override;
    bool repair_to_stable_processing() override;
    bool stable_to_repair_processing() override;
    // Before transiting
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
    // After transiting
    void _change_node_state(node_state_machine prev_, node_state_machine transit_destination_);
    virtual bool _any_to_initialize_processing_after() = 0;
    virtual bool _any_to_ready_processing_after() = 0;
    virtual bool _any_to_force_stop_processing_after() = 0;
    virtual bool _ready_to_repair_processing_after() = 0;
    virtual bool _ready_to_stable_processing_after() = 0;
    virtual bool _repair_to_stable_processing_after() = 0;
    virtual bool _stable_to_repair_processing_after() = 0;

public:
    b_node(/* args */)
    {
        is_main_thread_running_ = false;
    }
    void Start_Node(std::shared_ptr<node_cmd> hub_cmd);
    std::shared_ptr<st_node_state> get_state_ptr()
    {
        return node_state_;
    }
    std::shared_ptr<node_cmd> get_cmd_ptr()
    {
        return node_cmd_;
    }
    std::shared_ptr<node_cmd> get_sys_cmd_ptr()
    {
        return node_sys_cmd_;
    }
    // Behavior System
    void set_relative_node_name_and_id(std::string json_file_name_of_requirement_node,
                                         int node_id,
                                         std::string node_name);
    void set_relative_node_cmd_ptr(int node_id,
                                   std::shared_ptr<node_cmd> cmd);
    void set_relative_node_state_ptr(int node_id,
                                     std::shared_ptr<node_state> state);
    void set_relative_node_sys_cmd_ptr(int node_id,
                                       std::shared_ptr<node_cmd> sys_cmd);
    //void set_relative_b_node_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> cmd);
    //void set_relative_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state);
    //void set_relative_b_node_sys_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> sys_cmd);
    //void set_user_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state);
    void set_node_id(int node_id);
    virtual std::shared_ptr<b_node> Clone() const = 0;
    virtual ~b_node() = default;
};