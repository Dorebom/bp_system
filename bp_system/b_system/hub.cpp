#include "hub.hpp"

#include <iostream>
#include <fstream>
#include <chrono>
#include <type_traits>
#include <unistd.h>

hub::hub(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
    allocated_node_id = 0;
    set_config_file_name(BEHAVIOR_HUB_CONFIG_FILE);
}

hub::~hub()
{
}

/* node_state loop process */
void hub::initialize_processing()
{
    cmd_executor();
}
void hub::ready_processing()
{
    cmd_executor();
}
void hub::repair_processing()
{
    cmd_executor();
}
void hub::stable_processing()
{
    cmd_executor();
}
void hub::force_stop_processing()
{
    cmd_executor();
}
void hub::transit_processing()
{
}
void hub::end_processing()
{
}

/* node state change process */
// -> initialize
bool hub::any_to_initialize_processing()
{
    return true;
}
// -> ready (reset process)
bool hub::any_to_ready_processing()
{
    return true;
} // stable, repair and ready
// -> force stop
bool hub::any_to_force_stop_processing()
{
    return true;
} // stable, repair and ready
// -> normal flow
bool hub::ready_to_repair_processing()
{
    return true;
}
bool hub::ready_to_stable_processing()
{
    return true;
}
bool hub::repair_to_stable_processing()
{
    return true;
}
bool hub::stable_to_repair_processing()
{
    return true;
}

void hub::set_config(nlohmann::json json_data) {
    print_log("SET CONFIG");
    print_log("[requirement list]Total: "+ std::to_string(json_data.at("requirement_node_list").size()));

    for (auto &&requirement : json_data.at("requirement_node_list"))
    {
        //requirement_node_list[requirement.at("name")] = requirement.at("type");
        //print_log("[requirement list]type:"+requirement.at("type"));
    }
    node_config_.cmd_stack_size = json_data["cmd_stack_size"];
    node_config_.sys_cmd_stack_size = json_data["sys_cmd_stack_size"];

    running_node_id2name_list[BEHAVIOR_HUB_ID] = node_config_.node_name;
    running_node_name2id_list[node_config_.node_name] = BEHAVIOR_HUB_ID;
    running_node_name2type_list[node_config_.node_name] = node_config_.node_type;
}

void hub::_set_state()
{
}

void hub::cmd_executor()
{
    st_cmd_start_node cmd_start_node;

    if (node_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = node_cmd_->cmd_stack_.pop();

        print_log("[cmd]data size: " + std::to_string(cmd.cmd_code.data_size));
        switch ((b_hub_cmd_list)cmd.cmd_code.cmd_type)
        {
            case b_hub_cmd_list::START_NODE:
                print_log("[cmd]START_NODE");
                if (cmd.cmd_code.is_used_msgpack == true)
                {
                    try{
                        size_t data_size = cmd.cmd_code.data_size;
                        uint8_t* data = cmd.data;
                        msgpack::object_handle oh = msgpack::unpack((const char*)data, data_size);
                        const msgpack::object& obj = oh.get();
                        obj.convert(cmd_start_node);
                        print_log("[cmd]START_NODE: " + cmd_start_node.node_type);
                        print_log("[cmd]START_NODE: " + cmd_start_node.setting_json_file_name);
                        print_log("[cmd]START_NODE: " + cmd_start_node.setting_json_folder_name);
                        exec_node(cmd_start_node.node_type, cmd_start_node.setting_json_file_name, cmd_start_node.setting_json_folder_name, cmd.cmd_code.source);
                    }
                    catch(msgpack::unpack_error& e)
                    {
                        print_log("Unpack Error");
                    }
                    catch(msgpack::type_error& e)
                    {
                        print_log("Type Error");
                    }
                }
                else
                {
                    print_log("[cmd]START_NODE: is not used msgpack! " + std::to_string(cmd.cmd_code.source));
                }
                break;
            case b_hub_cmd_list::END_NODE:
                print_log("[cmd]END_NODE");
                break;
            case b_hub_cmd_list::PICK_SHARED_PTR:
                print_log("[cmd]PICK_SHARED_PTR");
                pick_shared_ptr(cmd.cmd_code.source);
                break;
            case b_hub_cmd_list::DELETE_SHARED_PTR:
                print_log("[cmd]DELETE_SHARED_PTR");
                delete_shared_ptr(cmd.cmd_code.source);
                break;
            default:
                break;
        }

    }
}

void hub::_configure()
{
    // Make shared ptr
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    print_log("[cmd stack size]"+std::to_string(node_config_.cmd_stack_size));
    print_log("[sys cmd stack size]"+std::to_string(node_config_.sys_cmd_stack_size));
    // Exec node in requirement list
    for (auto &&requirement : requirement_node_list)
    {
        //print_log("[requirement list]exec node: " + get_b_node_name((behavior_node_list)requirement));
        //exec_node((behavior_node_list)requirement, behavior_node_list::HUB);
        //exec_node(requirement.first, requirement.second, );
    }
}

void hub::store_node(b_node& stored_node, std::string node_name)
{
    node_store.set_node(stored_node, node_name);
}

void hub::show_stored_node()
{
    node_store.show_stored_node();
}

void hub::exec_node(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name)
{
    print_log("exec_node: " + node_type_name);
    nlohmann::json j;
    std::ifstream i(setting_json_folder_name + setting_json_file_name);
    if (!i) {
        std::cerr << "File not found: " << setting_json_folder_name + setting_json_file_name << std::endl;
        std::exit(1);
    }
    i >> j;

    std::string j_node_type_name = j["node_type"];
    std::string j_node_name = j["node_name"];

    j.clear();
    i.close();

    if (j_node_type_name != node_type_name) {
        std::cerr << "node_type_name is not match" << std::endl;
        std::exit(1);
    }
    if (running_node_name2type_list.find(j_node_name) != running_node_name2type_list.end()) {
        std::cerr << "node_name is already used" << std::endl;
        std::exit(1);
    }

    running_node_name2type_list[j_node_name] = j_node_type_name;

    // Check unique node id
    while (running_node_id2name_list.find(allocated_node_id) != running_node_id2name_list.end())
    {
        allocated_node_id++;
    }
    print_log("allocated_node_id: " + std::to_string(allocated_node_id));
    running_node_name2id_list[j_node_name] = allocated_node_id;
    running_node_id2name_list[allocated_node_id] = j_node_name;
    running_node_id2json_file_name_list[allocated_node_id] = setting_json_file_name;

    running_node_list[allocated_node_id] = node_store.get_node(node_type_name);
    running_node_list[allocated_node_id]->set_node_id(allocated_node_id);
    running_node_list[allocated_node_id]->set_config_directory_name(setting_json_folder_name);
    running_node_list[allocated_node_id]->set_config_file_name(setting_json_file_name);
    running_node_list[allocated_node_id]->Start_Node(node_cmd_);
}

void hub::exec_node(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name, int waiting_node_id)
{
    print_log("exec_node: " + node_type_name);
    nlohmann::json j;
    std::ifstream i(setting_json_folder_name + setting_json_file_name);
    print_log("setting_json_folder_name + setting_json_file_name: " + setting_json_folder_name + setting_json_file_name);
    if (!i) {
        std::cerr << "File not found: " << setting_json_folder_name + setting_json_file_name << std::endl;
        return;
    }
    i >> j;

    std::string j_node_type_name = j["node_type"];
    std::string j_node_name = j["node_name"];

    j.clear();
    i.close();

    if (j_node_type_name != node_type_name) {
        std::cerr << "node_type_name is not match" << std::endl;
        return;
    }
    if (running_node_name2type_list.find(j_node_name) != running_node_name2type_list.end()) {
        std::cerr << "node_name is already used" << std::endl;
        return;
    }

    running_node_name2type_list[j_node_name] = j_node_type_name;

    // Check unique node id
    while (running_node_id2name_list.find(allocated_node_id) != running_node_id2name_list.end())
    {
        allocated_node_id++;
    }
    print_log("allocated_node_id: " + std::to_string(allocated_node_id));
    running_node_name2id_list[j_node_name] = allocated_node_id;
    running_node_id2name_list[allocated_node_id] = j_node_name;
    running_node_id2json_file_name_list[allocated_node_id] = setting_json_file_name;

    // set_waiting_node_list
    if (waiting_node_id != BEHAVIOR_HUB_ID)
    {
        waiting_node_list[allocated_node_id] = waiting_node_id;
    }

    running_node_list[allocated_node_id] = node_store.get_node(node_type_name);
    running_node_list[allocated_node_id]->set_node_id(allocated_node_id);
    running_node_list[allocated_node_id]->set_config_directory_name(setting_json_folder_name);
    running_node_list[allocated_node_id]->set_config_file_name(setting_json_file_name);
    running_node_list[allocated_node_id]->Start_Node(node_cmd_);

}

void hub::exit_node(int node_id)
{
    std::string node_name = running_node_id2name_list[node_id];
    print_log("exit_node: " + node_name);
    running_node_id2name_list.erase(node_id);
    running_node_name2id_list.erase(node_name);
    running_node_name2type_list.erase(node_name);
    running_node_list[node_id]->End();
    running_node_list.erase(node_id);
}

void hub::pick_shared_ptr(int node_id)
{
    print_log("pick_shared_ptr: " + running_node_id2name_list[node_id]);

    node_state_list[node_id] = running_node_list[node_id]->get_state_ptr();
    node_cmd_list[node_id] = running_node_list[node_id]->get_cmd_ptr();
    node_sys_cmd_list[node_id] = running_node_list[node_id]->get_sys_cmd_ptr();

    if (waiting_node_list.find(node_id) != waiting_node_list.end())
    {
        auto waiting_node_id = waiting_node_list[node_id];
        running_node_list[waiting_node_id]->set_relative_node_name_and_id(
                running_node_id2json_file_name_list[node_id],
                node_id,
                running_node_id2name_list[node_id]);

        running_node_list[waiting_node_id]->set_relative_node_cmd_ptr(node_id, node_cmd_list[node_id]);
        running_node_list[waiting_node_id]->set_relative_node_state_ptr(node_id, node_state_list[node_id]);
        running_node_list[waiting_node_id]->set_relative_node_sys_cmd_ptr(node_id, node_sys_cmd_list[node_id]);
        waiting_node_list.erase(node_id);
    }
    send_your_requirement_node_list(allocated_node_id);
}

void hub::send_your_requirement_node_list(int node_id)
{
    st_node_cmd node_cmd;
    node_cmd.cmd_code.source = BEHAVIOR_HUB_ID;
    node_cmd.cmd_code.destination = node_id;
    node_cmd.cmd_code.cmd_id = 0;
    node_cmd.cmd_code.priority = 0;
    node_cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::REQUIRE_NODE_LIST;
    node_cmd.cmd_code.is_sys_cmd = true;
    node_cmd.cmd_code.data_size = 0;

    print_log("send_your_requirement_node_list: " + running_node_id2name_list[node_id]);
    node_sys_cmd_list.find(node_id)->second->cmd_stack_.push(node_cmd);
}

void hub::delete_shared_ptr(int node_id)
{
    node_state_list.erase(node_id);
    node_cmd_list.erase(node_id);
    node_sys_cmd_list.erase(node_id);
}
