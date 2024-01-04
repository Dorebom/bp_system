#include "b_hub.hpp"

#include <iostream>
#include <chrono>
#include <type_traits>

#include <unistd.h>

b_hub::b_hub(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
    set_config_file_name(BEHAVIOR_HUB_CONFIG_FILE);
}

b_hub::~b_hub()
{
}

void b_hub::End()
{
    for (running_node_list.size(); running_node_list.size() > 0;)
    {
        exit_node((behavior_node_list)running_node_list.back());
    }
    // exit_node(behavior_node_list::PHYSICS_HUB);
    is_main_thread_running_ = false;
}

void b_hub::store_node(b_node& stored_node, std::string node_name)
{
    b_node_store.set_node(stored_node, node_name);
}

void b_hub::show_stored_node()
{
    b_node_store.show_stored_node();
}

/* node_state loop process */
void b_hub::initialize_processing() 
{
    cmd_executor();
}
void b_hub::ready_processing() 
{
    cmd_executor();
}
void b_hub::repair_processing() 
{
    cmd_executor();
}
void b_hub::stable_processing() 
{
    cmd_executor();
}
void b_hub::force_stop_processing() 
{
    cmd_executor();
}
void b_hub::transit_processing() 
{
    uint64_t now_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (now_time - transit_start_time_ > node_config_.transit_watch_dog_time)
    {
        transit_start_time_ = now_time;
        node_state_machine_ = prev_node_state_machine_;
        print_log("[transit_processing]Watch dog timer expired");
    }
    // すべての子Nodeがcmd_node_state_になったら、node_state_をtransit_destination_node_state_に変更する
    if (node_state_list.size() != 0)
    {
        if (transit_destination_node_state_machine_ == node_state_list.find((int)behavior_node_list::PHYSICS_HUB)->second->state_code.state_machine)
        {
            node_state_machine_ = transit_destination_node_state_machine_;
        }
    }
}
void b_hub::end_processing()
{
}

/* node state change process */
// -> initialize
bool b_hub::any_to_initialize_processing() 
{
    return true;
}
// -> ready (reset process)
bool b_hub::any_to_ready_processing() 
{
    if (node_state_machine_ == node_state_machine::FORCE_STOP)
    {
        b_node_list[(int)behavior_node_list::PHYSICS_HUB]->Reset();
    }
    return true;
} // stable, repair and ready
// -> force stop
bool b_hub::any_to_force_stop_processing()
{
    b_node_list[(int)behavior_node_list::PHYSICS_HUB]->PushForceStop();
    return true;
} // stable, repair and ready

// -> normal flow
bool b_hub::ready_to_repair_processing() 
{
    b_node_list[(int)behavior_node_list::PHYSICS_HUB]->ChangeRepair();
    return true;
}
bool b_hub::ready_to_stable_processing() 
{
    b_node_list[(int)behavior_node_list::PHYSICS_HUB]->ChangeStable();
    return true;
}
bool b_hub::repair_to_stable_processing() 
{
    b_node_list[(int)behavior_node_list::PHYSICS_HUB]->ChangeStable();
    return true;
}
bool b_hub::stable_to_repair_processing() 
{
    b_node_list[(int)behavior_node_list::PHYSICS_HUB]->ChangeRepair();
    return true;
}

void b_hub::set_config(nlohmann::json json_data) {
    print_log("SET CONFIG");
    print_log("[requirement list]Total: "+ std::to_string(json_data.at("requirement").size()));
    for (auto &&requirement : json_data.at("requirement"))
    {
        requirement_list.push_back(requirement.at("type"));
        print_log("[requirement list]Element: " + get_b_node_name((behavior_node_list)requirement.at("type")));
    }
    node_config_.cmd_stack_size = json_data["cmd_stack_size"];
    node_config_.sys_cmd_stack_size = json_data["sys_cmd_stack_size"];
}

void b_hub::_configure() 
{
    // Make shared ptr
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    print_log("[cmd stack size]"+std::to_string(node_config_.cmd_stack_size));
    print_log("[sys cmd stack size]"+std::to_string(node_config_.sys_cmd_stack_size));
    // Exec node in requirement list
    for (auto &&requirement : requirement_list)
    {
        print_log("[requirement list]exec node: " + get_b_node_name((behavior_node_list)requirement));
        exec_node((behavior_node_list)requirement, behavior_node_list::HUB);
    }
}

void b_hub::_set_state() 
{
    node_state_->state_code.state_machine = node_state_machine_;
}

bool b_hub::check_usable_node_list(const behavior_node_list node_type)
{
    std::vector<int> result;
    for (auto value = static_cast<std::underlying_type_t<behavior_node_list>>(behavior_node_list::HUB); 
         value < static_cast<std::underlying_type_t<behavior_node_list>>(behavior_node_list::PERIOD_NODE); 
         value++)
    {
        result.push_back(static_cast<int>(value));
        //std::cout << "[" << node_config_.node_name << "][check_usable_node_list]" << result[value] << std::endl;
    }
    std::vector<int>::iterator itr;
    itr = std::find(result.begin(), result.end(), (int)node_type);    

    if (itr == result.end())
    {
        return true;
    }
    return false;
}

void b_hub::_set_node_name_and_type(std::string node_name, std::string node_type)
{
    if (running_node_type_list.find(node_name) != running_node_type_list.end())
    {
        print_log("Error: Your node's name(" + node_name + ") is already used.");
        return;
    }
    else{
        print_log("Set node name: " + node_name);
        print_log("Set node type: " + node_type);
        running_node_type_list[node_name] = node_type;
    }
}

void b_hub::_sys_cmd_executor()
{
    if (node_sys_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = node_sys_cmd_->cmd_stack_.pop();
        print_log("[cmd_executor]" + get_b_node_name((behavior_node_list)cmd.cmd_code.cmd_type));

        switch ((b_hub_sys_cmd_list)cmd.cmd_code.cmd_type)
        {
        // If it is not started, the command cannot be executed.
        //
        //case b_hub_sys_cmd_list::START:
        //    Start(true);
        //    break;
        case b_hub_sys_cmd_list::END:
            End();
            break;
        case b_hub_sys_cmd_list::CHANGE_STABLE:
            ChangeStable();
            break;
        case b_hub_sys_cmd_list::CHANGE_REPAIR:
            ChangeRepair();
            break;
        case b_hub_sys_cmd_list::PUSH_FORCE_STOP:
            PushForceStop();
            break;
        case b_hub_sys_cmd_list::RELEASE_FORCE_STOP:
            ReleaseForceStop();
            break;
        case b_hub_sys_cmd_list::RESET:
            Reset();
            break;
        default:
            break;
        }
    }
}

void b_hub::cmd_executor()
{
    _sys_cmd_executor();
    if (node_cmd_->cmd_stack_.size() != 0)
    {
        st_node_cmd ret;
        st_cmd_start_node* tmp_scsn;
        st_cmd_end_node* tmp_scen;
        st_cmd_set_node_name* tmp_scsnn;

        auto cmd = node_cmd_->cmd_stack_.pop();
        switch ((b_hub_cmd_list)cmd.cmd_code.cmd_type)
        {
        case b_hub_cmd_list::START_NODE:
            print_log("[cmd_executor]START_NODE");
            tmp_scsn = (st_cmd_start_node*)cmd.data;
            //exec_node(tmp_scsn->node_type, (behavior_node_list)cmd.cmd_code.source);
            break;
        case b_hub_cmd_list::END_NODE:
            print_log("[cmd_executor]END_NODE");
            tmp_scen = (st_cmd_end_node*)cmd.data;
            //exit_node((behavior_node_list)tmp_scen->node_type);
            break;
        case b_hub_cmd_list::PICK_SHARED_PTR:
            print_log("[cmd_executor]PICK_SHARED_PTR");
            //pick_shared_ptr((behavior_node_list)cmd.cmd_code.source);
            break;
        case b_hub_cmd_list::DELETE_SHARED_PTR:
            print_log("[cmd_executor]DELETE_SHARED_PTR");
            //node_cmd_list.erase((int)cmd.cmd_code.source);
            //node_state_list.erase((int)cmd.cmd_code.source);
            //node_sys_cmd_list.erase((int)cmd.cmd_code.source);
            break;
        case b_hub_cmd_list::SET_NODE_NAME:
            print_log("[cmd_executor]SET_NODE_NAME");
            tmp_scsnn = (st_cmd_set_node_name*)cmd.data;
            _set_node_name_and_type(tmp_scsnn->node_name, tmp_scsnn->node_type);
            break;
        default:
            print_log("[cmd_executor]Invalid cmd_type" + get_b_node_name((behavior_node_list)cmd.cmd_code.cmd_type));
            break;
        }
    }
}

void b_hub::_exec_node(behavior_node_list node_type)
{
    print_log(get_b_node_name(node_type));
    b_node_list[(int)node_type]->Start_Node(node_cmd_);
    /*
    switch (node_type)
    {
    case behavior_node_list::PHYSICS_HUB:
        p_hub_->Start_Node(false, b_hub_cmd_);
        break;
    case behavior_node_list::SIMPLE_NODE_A:
        b_simple_node_a_->Start_Node(false, b_hub_cmd_);
        break;
    case behavior_node_list::SIMPLE_NODE_B:
        b_simple_node_b_->Start_Node(false, b_hub_cmd_);
        break;
    default:
        break;
    }
    */
}

void b_hub::exec_node(behavior_node_list node_type, behavior_node_list source_node_type)
{
    std::vector<int>::iterator itr;

    if (check_usable_node_list(node_type))
    {
        print_log("Error: Your node's type(" + get_b_node_name(node_type) + ") is not usable.");
        return;
    }
    else{
        itr = std::find(running_node_list.begin(), running_node_list.end(), (int)node_type);
        if (itr == running_node_list.end())
        {
            running_node_list.push_back((int)node_type);
            print_log("Add running node(" + get_b_node_name(node_type) + ")");
            if (source_node_type != behavior_node_list::HUB)
            {
                waiting_cmd_list[(int)node_type] = (int)source_node_type;
                waiting_state_list[(int)node_type] = (int)source_node_type;
            }
            else
            {
                print_log("<<NOTION>>Waiting list is skipped, because source node is HUB.");
            }
            _exec_node(node_type);
        }
        else
        {
            print_log("Your node's type(" + get_b_node_name(node_type) + ") is already running.");
        }
    }
}

void b_hub::exec_node(std::string node_type_name, std::string unique_node_name, std::string setting_json_file_name, std::string setting_json_folder_name)
{
    print_log("exec_node: " + node_type_name);
    node_list[unique_node_name] = b_node_store.get_node(node_type_name);
    node_list[unique_node_name]->set_config_directory_name(setting_json_folder_name);
    node_list[unique_node_name]->set_config_file_name(setting_json_file_name);
    node_list[unique_node_name]->Start_Node(node_cmd_);
}

void b_hub::_exit_node(behavior_node_list node_type)
{
    b_node_list[(int)node_type]->End();
}

void b_hub::exit_node(behavior_node_list node_type)
{
    std::vector<int>::iterator itr = std::find(running_node_list.begin(), running_node_list.end(), (int)node_type);
    if (itr != running_node_list.end())
    {
        running_node_list.erase(itr);
    }
    _exit_node(node_type);
}

void b_hub::_get_node_shared_ptr(behavior_node_list node_type)
{
    node_cmd_list[(int)node_type] = b_node_list[(int)node_type]->get_cmd_ptr();
    node_state_list[(int)node_type] = b_node_list[(int)node_type]->get_state_ptr();
    node_sys_cmd_list[(int)node_type] = b_node_list[(int)node_type]->get_sys_cmd_ptr();
}

void b_hub::pick_shared_ptr(behavior_node_list node_type)
{
    print_log("Add Shared ptr(" + get_b_node_name(node_type) + ")");
    _get_node_shared_ptr(node_type);

    print_log("Waiting cmd list");
    for (const auto& pair : waiting_cmd_list) {
        print_log("Item: " + get_b_node_name((behavior_node_list)pair.first) 
                + ", Ordered user: " + get_b_node_name((behavior_node_list)pair.second));
    }
    print_log("Waiting state list");
    for (const auto& pair : waiting_state_list) {
        print_log("Item: " + get_b_node_name((behavior_node_list)pair.first) 
                + ", Ordered user: " + get_b_node_name((behavior_node_list)pair.second));
    }

    if (waiting_cmd_list.find((int)node_type) != waiting_cmd_list.end())
    {
        if (waiting_cmd_list.find((int)node_type)->second != (int)behavior_node_list::HUB)
        {
            b_node_list[(int)waiting_cmd_list.find((int)node_type)->second]
            ->set_relative_b_node_cmd_ptr(
                (behavior_node_list)node_type, 
                node_cmd_list[(int)node_type]);
            waiting_cmd_list.erase((int)node_type);
        }
    }
    if (waiting_state_list.find((int)node_type) != waiting_state_list.end())
    {
        if (waiting_state_list.find((int)node_type)->second != (int)behavior_node_list::HUB)
        {
            b_node_list[(int)waiting_state_list.find((int)node_type)->second]
            ->set_relative_b_node_state_ptr(
                (behavior_node_list)node_type, 
                node_state_list[(int)node_type]);

            b_node_list[(int)node_type]->set_user_b_node_state_ptr(
                (behavior_node_list)waiting_state_list.find((int)node_type)->second,
                node_state_list[(int)node_type]);

            waiting_state_list.erase((int)node_type);
        }
    }
    print_log("pick_shared_ptr: " + get_b_node_name(node_type));
    _require_node_list(node_type);
}

void b_hub::_require_node_list(behavior_node_list destination_node)
{
    st_node_cmd node_cmd;
    node_cmd.cmd_code.source = (int)behavior_node_list::HUB;
    node_cmd.cmd_code.destination = (int)destination_node;
    node_cmd.cmd_code.priority = 0;
    node_cmd.cmd_code.cmd_id = 0;
    node_cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::REQUIRE_NODE_LIST;
    node_cmd.cmd_code.data_size = 0;

    print_log("Require node list: " + get_b_node_name(destination_node));

    node_sys_cmd_list.find((int)destination_node)->second->cmd_stack_.push(node_cmd);
}