#include "b_simple_node_b.hpp"
// Setting file name
#define B_NODE_CONFIG_FILE "b_simple_node_b.json"

b_simple_node_b::b_simple_node_b(/* args */)
{
    is_main_thread_running_ = false;
    node_state_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_ = node_state_machine::UNCONFIGURED;

    this_node = behavior_node_list::SIMPLE_NODE_B;
    set_config_file_name(B_NODE_CONFIG_FILE);
}

b_simple_node_b::~b_simple_node_b()
{
}

void b_simple_node_b::_initialize_processing()
{
    b_node_state_->state_code.state_machine = node_state_;
    b_node_state_->state_code.node_id = 1;
}
void b_simple_node_b::_ready_processing()
{
    b_node_state_->state_code.state_machine = node_state_;
    b_node_state_->state_code.node_id = 1;
}
void b_simple_node_b::_repair_processing()
{
    b_node_state_->state_code.state_machine = node_state_;
}
void b_simple_node_b::_stable_processing()
{
    b_node_state_->state_code.state_machine = node_state_;
}
void b_simple_node_b::_force_stop_processing()
{
    b_node_state_->state_code.state_machine = node_state_;
}
void b_simple_node_b::transit_processing()
{
    uint64_t now_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (now_time - transit_start_time_ > node_config_.transit_watch_dog_time)
    {
        transit_start_time_ = now_time;
        node_state_ = prev_node_state_;
        // Add warning message
    }
    // TODO: implement
    // すべての子Nodeがcmd_node_state_になったら、node_state_をtransit_destination_node_state_に変更する
    node_state_ = transit_destination_node_state_;
    print_log("[node_processing]NODE " 
            + get_node_state_machine_name(node_state_) 
            + " -> " + get_node_state_machine_name(cmd_node_state_));
}
/* node_state change process */
// -> initialize
bool b_simple_node_b::_any_to_initialize_processing() {return true;}
// -> ready (reset process)
bool b_simple_node_b::_any_to_ready_processing() {return true;} // stable, repair and ready
// -> force stop
bool b_simple_node_b::_any_to_force_stop_processing() {return true;} // stable, repair and ready
// -> normal flow
bool b_simple_node_b::_ready_to_repair_processing() {return true;}
bool b_simple_node_b::_ready_to_stable_processing() {return true;}
bool b_simple_node_b::_repair_to_stable_processing() {return true;}
bool b_simple_node_b::_stable_to_repair_processing() {return true;}

void b_simple_node_b::_set_config(nlohmann::json json_data) 
{
}
void b_simple_node_b::_configure()
{
    b_node_state_ = std::make_shared<st_node_state>();
    b_node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    b_node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
}
void b_simple_node_b::_set_state()
{
    b_node_state_->state_code.state_machine = node_state_;
}

void b_simple_node_b::cmd_executor()
{
    _sys_cmd_executor();
    if (b_node_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = b_node_cmd_->cmd_stack_.pop();
    }
    else
    {
        // Add warning message
    }
}
