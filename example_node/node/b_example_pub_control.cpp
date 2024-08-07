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
bool b_example_pub_control::_any_to_ready_processing()
{
    return false;
}
bool b_example_pub_control::_any_to_force_stop_processing(){return true;}
bool b_example_pub_control::_ready_to_repair_processing(){return true;}
bool b_example_pub_control::_ready_to_stable_processing()
{
    return true;
}
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
    state_example_servo* servo_state1 = (state_example_servo*)b_node_state_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->data;
    cmd_example_servo* cmd1;

    //print_log("[servo state]count: " + std::to_string(servo_state1->encoder_count));
    //print_log("[servo state]SM: " + get_node_state_machine_name(servo_state1->state_machine));
    if (servo_state1->state_machine == node_state_machine::READY)
    {
        st_node_cmd cmd;
        cmd.cmd_code.source = (int)behavior_node_list::EXAMPLE_PUB_CONTROL;
        cmd.cmd_code.destination = (int)behavior_node_list::EXAMPLE_SUB_SERVO;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.cmd_type = (int)b_node_cmd_list::CMD_EXAMPLE_SERVO;
        cmd.cmd_code.data_size = sizeof(cmd_example_servo);
        cmd.cmd_code.is_sys_cmd = false;

        cmd1 = (cmd_example_servo*)cmd.data;
        cmd1->cmd_type = cmd_example_servo_type::CHANGE_SM_STABLE;
        b_node_cmd_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->cmd_stack_.push(cmd);
        print_log("[sub servo state]change to stable");
    }else if (servo_state1->state_machine == node_state_machine::STABLE)
    {
        ChangeStable();
    }

    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_repair_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_stable_processing()
{
    state_example_servo* servo_state1 = (state_example_servo*)b_node_state_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->data;
    cmd_example_servo* cmd1;

    if (servo_state1->state_machine == node_state_machine::STABLE)
    {
        st_node_cmd cmd;
        cmd.cmd_code.source = (int)behavior_node_list::EXAMPLE_PUB_CONTROL;
        cmd.cmd_code.destination = (int)behavior_node_list::EXAMPLE_SUB_SERVO;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.cmd_type = (int)b_node_cmd_list::CMD_EXAMPLE_SERVO;
        cmd.cmd_code.data_size = sizeof(cmd_example_servo);
        cmd.cmd_code.is_sys_cmd = false;

        cmd1 = (cmd_example_servo*)cmd.data;
        if (servo_state1->encoder_count < 0)
        {
            cmd1->cmd_type = cmd_example_servo_type::ADD_COUNT;
            b_node_cmd_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->cmd_stack_.push(cmd);
        }
        else if(servo_state1->encoder_count > 0)
        {
            cmd1->cmd_type = cmd_example_servo_type::SUB_COUNT;
            b_node_cmd_map[(int)behavior_node_list::EXAMPLE_SUB_SERVO]->cmd_stack_.push(cmd);
        }
    }

    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void b_example_pub_control::_end_processing()
{
}

void b_example_pub_control::_set_config(nlohmann::json &json_data)
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
