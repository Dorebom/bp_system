#include "hub.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <type_traits>
#include <string>

#ifdef __linux__
    #include <unistd.h>
#endif

void hub::_task_send()
{
    print_log("Send Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_send_periodic_time * 0.001)) + "ms");

    comm_gui_.set_send_address(node_config_.send_ip.c_str(), node_config_.send_port);

    // 一度に送信可能な st_node_state数
    int max_send_data_cnt = (MAX_UDP_DATA_SIZE - MAX_UDP_DATA_SIZE % sizeof(st_node_state)) / sizeof(st_node_state);
    int send_data_cnt = 0;
    st_node_state send_data_;
    std::vector<st_node_state> send_data_list;
    send_data_list.reserve(max_send_data_cnt);

    while (check_node_running())
    {
        if (check_allowed_comm_udp() && is_gui_connected)
        {
            if (b_system_state->state_stack_.size() > 0)
            {
                send_data_cnt = 0;
                while (send_data_cnt < max_send_data_cnt && b_system_state->state_stack_.size() > 0)
                {
                    send_data_list.push_back(b_system_state->state_stack_.pop());
                    //send_data_ = b_system_state->state_stack_.pop();
                    send_data_cnt++;
                }
                comm_gui_.send_data(reinterpret_cast<uint8_t*>(&send_data_list), sizeof(st_node_state) * send_data_cnt);
                send_data_list.clear();
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_send_periodic_time));
    }
    print_log("Send Thread End");
}

void hub::_task_recv()
{
    //RecvData recv_data_;
    print_log("Recv Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_recv_periodic_time * 0.001)) + "ms");

    comm_gui_.set_recv_address(node_config_.recv_ip.c_str(), node_config_.recv_port);

    st_node_cmd recv_data_;
    int recv_data_size = 0;
    while (check_node_running())
    {
        if (check_allowed_comm_udp())
        {
            recv_data_size = comm_gui_.recv_data(reinterpret_cast<uint8_t*>(&recv_data_), sizeof(st_node_cmd));
            if (recv_data_size > 0)
            {
                if (recv_data_.cmd_code.is_sys_cmd)
                {
                    node_sys_cmd_->cmd_stack_.push(recv_data_);
                }
                else
                {
                    node_cmd_->cmd_stack_.push(recv_data_);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_recv_periodic_time));
    }
    print_log("Recv Thread End");
}

hub::hub(/* args */)
{
    is_main_thread_running_ = false;
    //change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
    allocated_node_id = 0;
    // init gui
    is_gui_connected = false;

    set_config_file_name(BEHAVIOR_HUB_CONFIG_FILE);
}

hub::hub(std::string setting_json_file_name, std::string setting_json_folder_name)
{
    is_main_thread_running_ = false;
    //change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
    allocated_node_id = 0;
    // init gui
    is_gui_connected = false;

    set_config_file_name(setting_json_file_name);
    set_config_directory_name(setting_json_folder_name);
}

hub::~hub()
{
}

/* node_state loop process */
void hub::initialize_processing()
{
    print_log("initialize_processing");
    cmd_executor();
    set_state_machine();
}
void hub::ready_processing()
{
    cmd_executor();
    set_state_machine();
}
void hub::repair_processing()
{
    cmd_executor();
    set_state_machine();
}
void hub::stable_processing()
{
    cmd_executor();
    set_state_machine();
}
void hub::force_stop_processing()
{
    cmd_executor();
    set_state_machine();
}
void hub::transit_processing()
{
}
void hub::end_processing()
{
    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        if (state_->running_node_id[i] != NO_NODE_ID)
        {
            exit_node(state_->running_node_id[i]);
        }
    }
    print_log("End Processing");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

/* node state change process */
// -> initialize
bool hub::any_to_initialize_processing()
{
    print_log("any_to_initialize_processing");
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

void hub::set_config(nlohmann::json &json_data) {
    print_log("SET CONFIG");
    print_log("[requirement list]Total: "+ std::to_string(json_data.at("requirement_node_list").size()));

    for (auto &&requirement : json_data.at("requirement_node_list"))
    {
        //requirement_node_list[requirement.at("name")] = requirement.at("type");
        //print_log("[requirement list]type:"+requirement.at("type"));
    }
    node_config_.cmd_stack_size = json_data["cmd_stack_size"];
    node_config_.sys_cmd_stack_size = json_data["sys_cmd_stack_size"];
    node_config_.sys_state_stack_size = json_data["sys_state_stack_size"];

    running_node_id2name_list[BEHAVIOR_HUB_ID] = node_config_.node_name;
    running_node_name2id_list[node_config_.node_name] = BEHAVIOR_HUB_ID;
    running_node_name2type_list[node_config_.node_name] = node_config_.node_type;
}

void hub::_set_state()
{
    //print_log("_set_state");
    state_->state_machine = node_state_machine_;
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
                        print_log("Unpack Error" + std::string(e.what()));
                    }
                    catch(msgpack::type_error& e)
                    {
                        print_log("Type Error" + std::string(e.what()));
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
                print_log("[cmd]DELETE_SHARED_PTR:");
                delete_shared_ptr(cmd.cmd_code.source);
                break;
            case b_hub_cmd_list::START_GUI:
                print_log("[cmd]START_GUI");
                is_gui_connected = true;
                break;
            case b_hub_cmd_list::END_GUI:
                print_log("[cmd]END_GUI");
                is_gui_connected = false;
                b_system_state->state_stack_.clear();
                break;
            case b_hub_cmd_list::SEND_HUB_INFO:
                print_log("[cmd]SEND_HUB_INFO");
                b_system_state->state_stack_.push(*node_state_);
                break;
            case b_hub_cmd_list::SEND_NODES_INFO:
                print_log("[cmd]SEND_NODES_INFO");
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
    state_ = (b_hub_state*)node_state_->data;
    b_system_state = std::make_shared<node_state_stack>(node_config_.sys_state_stack_size);
    print_log("[cmd stack size]"+std::to_string(node_config_.cmd_stack_size));
    print_log("[sys cmd stack size]"+std::to_string(node_config_.sys_cmd_stack_size));
    print_log("[sys state stack size]"+std::to_string(node_config_.sys_state_stack_size));

    node_state_->state_code.node_id = BEHAVIOR_HUB_ID;
    node_state_->state_code.data_size = sizeof(b_hub_state);

    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        state_->running_node_id[i] = NO_NODE_ID;
        for (size_t j = 0; j < MAX_NODE_NAME; j++)
        {
            state_->running_node_name[i][j] = '\0';
        }
    }
    // Exec node in requirement list
    for (auto &&requirement : requirement_node_list)
    {
        //print_log("[requirement list]exec node: " + get_b_node_name((behavior_node_list)requirement));
        //exec_node((behavior_node_list)requirement, behavior_node_list::HUB);
        //exec_node(requirement.first, requirement.second, );
    }
    print_log("CONFIGURE DONE");
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
    nlohmann::json j_data;

    std::ifstream ifs(setting_json_folder_name + setting_json_file_name);
    if (!ifs) {
        std::cerr << "File not found: " << setting_json_folder_name + setting_json_file_name << std::endl;
        std::exit(1);
    }
    else
    {
        print_log("File found: " + setting_json_folder_name + setting_json_file_name);
    }

    ifs >> j_data;

    std::string j_node_type_name = j_data["node_type"];
    std::string j_node_name = j_data["node_name"];

    print_log("j_node_type_name: " + j_node_type_name);
    print_log("j_node_name: " + j_node_name);

    j_data.clear();
    ifs.close();

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

    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        if (state_->running_node_id[i] == NO_NODE_ID)
        {
            state_->running_node_id[i] = allocated_node_id;
            for (size_t j = 0; j < MAX_NODE_NAME; j++)
            {
                state_->running_node_name[i][j] = '\0';
            }
            for (size_t j = 0; j < j_node_name.size(); j++)
            {
                state_->running_node_name[i][j] = j_node_name[j];
            }
            break;
        }
    }
}

void hub::exec_node(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name, int waiting_node_id)
{
    print_log("hogehoge2");

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

    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        if (state_->running_node_id[i] == NO_NODE_ID)
        {
            state_->running_node_id[i] = allocated_node_id;
            for (size_t j = 0; j < MAX_NODE_NAME; j++)
            {
                state_->running_node_name[i][j] = '\0';
            }
            for (size_t j = 0; j < j_node_name.size(); j++)
            {
                state_->running_node_name[i][j] = j_node_name[j];
            }
            break;
        }
    }
}

void hub::exit_node(int node_id)
{
    std::string node_name = running_node_id2name_list[node_id];
    print_log("exit_node: " + node_name);
    running_node_id2name_list.erase(node_id);
    running_node_name2id_list.erase(node_name);
    running_node_name2type_list.erase(node_name);
    running_node_list[node_id]->End();
    bool is_running = true;
    while (is_running)
	{
		is_running = running_node_list[node_id]->check_node_running();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
    running_node_list.erase(node_id);
    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        if (node_id == state_->running_node_id[i])
        {
            state_->running_node_id[i] = NO_NODE_ID;
            for (size_t j = 0; j < MAX_NODE_NAME; j++)
            {
                state_->running_node_name[i][j] = '\0';
            }
            break;
        }
    }
}

void hub::exit_node(std::string setting_json_file_name)
{
    int node_id = running_node_name2id_list[setting_json_file_name];
    exit_node(node_id);    
}

void hub::show_node_json_file(std::string node_type_name, std::string setting_json_file_name, std::string setting_json_folder_name)
{
    print_log("show_node_json_file: " + node_type_name);

    nlohmann::json j;
    std::ifstream ifs(setting_json_folder_name + setting_json_file_name);

    if (!ifs) {
        print_log("File not found: " + setting_json_folder_name + setting_json_file_name);
        std::exit(1);
    }
    else
    {
        print_log("File found: " + setting_json_folder_name + setting_json_file_name);
        print_log( ifs.is_open() ? "File is open" : "File is not open" );
        print_log( ifs.good() ? "File is good" : "File is not good" );
        print_log( ifs.fail() ? "File is fail" : "File is not fail" );
        print_log( ifs.bad() ? "File is bad" : "File is not bad" );
    }    
    ifs >> j;

    std::string j_node_type_name = j["node_type"];
    std::string j_node_name = j["node_name"];

    print_log("j_node_type_name: " + j_node_type_name);
    print_log("j_node_name: " + j_node_name);
    
    j.clear();
    ifs.close();
}

void hub::display_state()
{
    print_log("display_state");
    for (size_t i = 0; i < MAX_NODE_NUM; i++)
    {
        if (state_->running_node_id[i] != NO_NODE_ID)
        {
            print_log("Running ID List: " + std::to_string(i)
                    + ", node_id: " + std::to_string(state_->running_node_id[i])
                    + ", node_name: " + std::string(state_->running_node_name[i])
            );
        }
    }
}

void hub::set_state_machine()
{
    //node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;
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
    print_log("pick_shared_ptr: " + running_node_id2name_list[node_id] + " is done");
    send_your_requirement_node_list(node_id);
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

bool hub::check_hub_running()
{
    return check_node_running();
}