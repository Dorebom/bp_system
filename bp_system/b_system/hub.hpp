#pragma once

#include <vector>
#include <map>

#include "../_common/node.hpp"
#include "../_common/node_util.hpp"

#include "node_store.hpp"

#include "b_hub_cmd/b_hub_cmd_list.hpp"



#define BEHAVIOR_HUB_CONFIG_FILE "behavior_hub.json"

class hub : public node
{
private:
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
    void cmd_executor();

    /*DATA*/
    NodeStore node_store;
    std::map<std::string, std::string> requirement_node_list;

    int allocated_node_id;
    std::map<std::string, std::string> running_node_name2type_list; // key: node_name, value: node_type
    std::map<std::string, int> running_node_name2id_list;           // key: node_name, value: node_id
    std::map<int, std::string> running_node_id2name_list;           // key: node_id,   value: node_name
    std::map<int, std::string> running_node_id2json_file_name_list; // key: node_id,   value: json_file_name
                                                                    // json_file_name is used as unique node name.
    std::map<int, std::shared_ptr<b_node>> running_node_list;       // key: node_id,   value: node_instance

    std::map<int, std::shared_ptr<node_state>> node_state_list;
    std::map<int, std::shared_ptr<node_cmd>> node_cmd_list;
    std::map<int, std::shared_ptr<node_cmd>> node_sys_cmd_list;

    std::map<int, int> waiting_node_list;                   // key: node_id,   value: node_name

    /*Sub-Function*/
    void pick_shared_ptr(int node_id);
    void delete_shared_ptr(int node_id);
    void exec_node(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name, int waiting_node_id);
    void send_your_requirement_node_list(int node_id);

public:
    hub(/* args */);
    ~hub();
    void store_node(b_node& stored_node, std::string node_name);
    void show_stored_node();
    void exec_node(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name);
    void exit_node(int node_id);
};