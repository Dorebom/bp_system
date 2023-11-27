#include "p_hub.hpp"

#include <iostream>
#include <chrono>

p_hub::p_hub(/* args */)
{
    is_main_thread_running_= false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    this_node = behavior_node_list::PHYSICS_HUB;
    set_config_file_name(PHYSICS_HUB_CONFIG_FILE);
}

p_hub::~p_hub()
{
}

void p_hub::End()
{
    for (running_node_list.size(); running_node_list.size() > 0;)
    {
        exit_node((physics_node_list)running_node_list.back());
    }
    // exit_node(behavior_node_list::PHYSICS_HUB);
    is_main_thread_running_ = false;
}

/* node_state loop process */
void p_hub::_initialize_processing() 
{
}
void p_hub::_ready_processing() 
{
}
void p_hub::_repair_processing() 
{
}
void p_hub::_stable_processing() 
{
}
void p_hub::_force_stop_processing() 
{
}
/* node state change process */
// -> initialize
bool p_hub::_any_to_initialize_processing() {return true;}
// -> ready (reset process)
bool p_hub::_any_to_ready_processing() {return true;} // stable, repair and ready
// -> force stop
bool p_hub::_any_to_force_stop_processing() {return true;} // stable, repair and ready
// -> normal flow
bool p_hub::_ready_to_repair_processing() {return true;}
bool p_hub::_ready_to_stable_processing() {return false;}
bool p_hub::_repair_to_stable_processing() {return true;}
bool p_hub::_stable_to_repair_processing() {return true;}

void p_hub::_set_config(nlohmann::json json_data) 
{
}

void p_hub::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
}

void p_hub::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void p_hub::cmd_executor()
{
    _sys_cmd_executor();
    if (node_cmd_->cmd_stack_.size() != 0)
    {
    }
}


void p_hub::exit_node(physics_node_list node_type)
{
    std::vector<int>::iterator itr = std::find(running_node_list.begin(), running_node_list.end(), (int)node_type);
    if (itr != running_node_list.end())
    {
        running_node_list.erase(itr);
    }
    _exit_node(node_type);
}
void p_hub::_exit_node(physics_node_list node_type)
{
    p_node_list[(int)node_type]->End();
}
