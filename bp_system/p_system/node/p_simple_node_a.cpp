#include "p_simple_node_a.hpp"
// Setting file name
#define P_NODE_CONFIG_FILE "p_simple_node_a.json"

p_simple_node_a::p_simple_node_a(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    this_node = physics_node_list::SIMPLE_NODE_A;
    set_config_file_name(P_NODE_CONFIG_FILE);
}

p_simple_node_a::~p_simple_node_a()
{
}

void p_simple_node_a::_initialize_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void p_simple_node_a::_ready_processing()
{
    cmd_executor();
    node_state_->state_code.state_machine = node_state_machine_;
}

void p_simple_node_a::_repair_processing()
{
    cmd_executor();
    node_state_->state_code.state_machine = node_state_machine_;

}

void p_simple_node_a::_stable_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void p_simple_node_a::_force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

bool p_simple_node_a::_any_to_initialize_processing() {return true;}
bool p_simple_node_a::_any_to_ready_processing() {return true;} // stable, repair and ready
bool p_simple_node_a::_any_to_force_stop_processing() {return true;} // stable, repair and ready
bool p_simple_node_a::_ready_to_repair_processing() {return true;}
bool p_simple_node_a::_ready_to_stable_processing() {return true;}
bool p_simple_node_a::_repair_to_stable_processing() {return true;}
bool p_simple_node_a::_stable_to_repair_processing() {return true;}

void p_simple_node_a::_set_config(nlohmann::json json_data) 
{
}
void p_simple_node_a::_configure()
{
    // Set node state
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
}

void p_simple_node_a::_set_state()
{
    // Set node state
    node_state_->state_code.state_machine = node_state_machine_;
}

void p_simple_node_a::cmd_executor()
{
    if (node_cmd_->cmd_stack_.size() != 0)
    {
        node_cmd_->cmd_stack_.pop();
    }
}