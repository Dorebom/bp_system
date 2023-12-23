#include "b_example_pub_control.hpp"

// Setting file name
#define B_NODE_CONFIG_DIRECTORY "../example_node/config/"
#define B_NODE_CONFIG_FILE "b_example_pub_control.json"

b_example_pub_control::b_example_pub_control(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    this_node = behavior_node_list::EXAMPLE_PUB_CONTROL;
    set_config_directory_name(B_NODE_CONFIG_DIRECTORY);
    set_config_file_name(B_NODE_CONFIG_FILE);
}

b_example_pub_control::~b_example_pub_control()
{
}

bool b_example_pub_control::_any_to_initialize_processing(){return true;}
bool b_example_pub_control::_any_to_ready_processing(){return true;}
bool b_example_pub_control::_any_to_force_stop_processing(){return true;}
bool b_example_pub_control::_ready_to_repair_processing(){return true;}
bool b_example_pub_control::_ready_to_stable_processing(){return true;}
bool b_example_pub_control::_repair_to_stable_processing(){return true;}
bool b_example_pub_control::_stable_to_repair_processing(){return true;}

bool b_example_pub_control::_any_to_initialize_processing_after(){return true;}
bool b_example_pub_control::_any_to_ready_processing_after(){return true;}
bool b_example_pub_control::_any_to_force_stop_processing_after(){return true;}
bool b_example_pub_control::_ready_to_repair_processing_after(){return true;}
bool b_example_pub_control::_ready_to_stable_processing_after(){return true;}
bool b_example_pub_control::_repair_to_stable_processing_after(){return true;}
bool b_example_pub_control::_stable_to_repair_processing_after(){return true;}

void b_example_pub_control::_initialize_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_ready_processing()
{
    cmd_executor();

    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_repair_processing()
{
    cmd_executor();
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_stable_processing()
{
    cmd_executor();
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_end_processing()
{
}

void b_example_pub_control::_set_config(nlohmann::json json_data)
{
}

void b_example_pub_control::_configure()
{
    // Set node state
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
}

void b_example_pub_control::_set_state()
{
    // Set node state
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::cmd_executor()
{
}

void b_example_pub_control::_update_servo_control()
{
    state_example_servo* servo_state1 = (state_example_servo*)b_node_state_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->data;
    cmd_example_servo cmd1;

    if (servo_state1->servo_angle > 0)
    {
        cmd1.cmd_servo_anglur_velocity = -1.0;

    }
    else
    {
        cmd1.cmd_servo_anglur_velocity = 1.0;
    } 
}
