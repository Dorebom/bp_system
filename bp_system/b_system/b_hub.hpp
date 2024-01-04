#pragma once

#include <vector>
#include <map>

#include "../_common/node.hpp"
//#include "../p_system/p_hub.hpp"

#include "node_store.hpp"

//#include "../_common/node_list.hpp"
#include "../_common/node_util.hpp"

#include "b_node.hpp"
#include "b_hub_state.hpp"
#include "b_hub_cmd/b_hub_cmd_list.hpp"
#include "b_hub_sys_cmd/b_hub_sys_cmd_list.hpp"
#include "b_node_sys_cmd/b_node_sys_cmd_list.hpp"

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
    
    void _set_node_name_and_type(std::string node_name, std::string node_type);

    void cmd_executor();
    void _sys_cmd_executor();
    std::map<std::string, std::string> running_node_name2type_list; // key: node_name, value: node_type
    std::shared_ptr<std::map<std::string, int>> running_node_name2id_list;           // key: node_name, value: node_id
    std::map<int, std::string> running_node_id2name_list;           // key: node_id,   value: node_name

    std::vector<int> requirement_list;
    std::map<int, int> waiting_cmd_list;
    std::map<int, int> waiting_state_list;
    std::map<int, std::shared_ptr<node_cmd>> node_cmd_list;
    std::map<int, std::shared_ptr<node_state>> node_state_list;
    std::map<int, std::shared_ptr<node_cmd>> node_sys_cmd_list;
    std::map<int, std::shared_ptr<b_node>> b_node_list;
    std::map<std::string, std::shared_ptr<b_node>> node_list;
    std::map<int, std::shared_ptr<b_node>> b_stored_node_list;
    NodeStore b_node_store;
public:
    b_hub(/* args */);
    ~b_hub();
    void End();
    void store_node(b_node& stored_node, std::string node_name);
    void show_stored_node();
    void exec_node(behavior_node_list node_type, behavior_node_list source_node_type);
    void exec_node(std::string node_type_name, std::string unique_node_name, std::string setting_json_file_name, std::string setting_json_folder_name);
    void _exec_node(behavior_node_list node_type);
    void exit_node(behavior_node_list node_type);
    void _exit_node(behavior_node_list node_type);
    void pick_shared_ptr(behavior_node_list node_type);
    void _get_node_shared_ptr(behavior_node_list node_type);
};

