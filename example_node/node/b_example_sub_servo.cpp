#include "b_example_sub_servo.hpp"

#include <random>

// Setting file name
#define B_NODE_CONFIG_DIRECTORY "../example_node/config/"
#define B_NODE_CONFIG_FILE "b_example_sub_servo.json"

void b_example_sub_servo::_update_servo_state(int add_value)
{
    if(node_state_machine_ == node_state_machine::STABLE or node_state_machine_ == node_state_machine::REPAIR)
    {
        state_->encoder_count += add_value;
    }
}

void b_example_sub_servo::_reset_encoder_count()
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(-20, 20);
    if (node_state_machine_ != node_state_machine::FORCE_STOP)
    {
        state_->encoder_count = distr(eng);
    }
}

void b_example_sub_servo::_display_servo_state()
{
    print_log("[encoder count] " + std::to_string(state_->encoder_count));
}

b_example_sub_servo::b_example_sub_servo(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    this_node = behavior_node_list::EXAMPLE_SUB_SERVO;
    set_config_directory_name(B_NODE_CONFIG_DIRECTORY);
    set_config_file_name(B_NODE_CONFIG_FILE);

    //node_state_->state_code.state_machine = node_state_machine_;
    //node_state_->state_code.transit_destination_node_state= node_state_machine_;
    //node_state_->state_code.node_id = 1;
}

b_example_sub_servo::~b_example_sub_servo()
{
}

bool b_example_sub_servo::_any_to_initialize_processing()
{
    node_state_->state_code.data_size = sizeof(state_example_servo);

    _reset_encoder_count();
    print_log("[encoder count] " + std::to_string(state_->encoder_count));
    return true;
}
bool b_example_sub_servo::_any_to_ready_processing(){return true;}
bool b_example_sub_servo::_any_to_force_stop_processing(){return true;}
bool b_example_sub_servo::_ready_to_repair_processing(){return true;}
bool b_example_sub_servo::_ready_to_stable_processing(){return true;}
bool b_example_sub_servo::_repair_to_stable_processing(){return true;}
bool b_example_sub_servo::_stable_to_repair_processing(){return true;}

bool b_example_sub_servo::_any_to_initialize_processing_after(){return true;}
bool b_example_sub_servo::_any_to_ready_processing_after(){return true;}
bool b_example_sub_servo::_any_to_force_stop_processing_after(){return true;}
bool b_example_sub_servo::_ready_to_repair_processing_after(){return true;}
bool b_example_sub_servo::_ready_to_stable_processing_after(){return true;}
bool b_example_sub_servo::_repair_to_stable_processing_after(){return true;}
bool b_example_sub_servo::_stable_to_repair_processing_after(){return true;}

void b_example_sub_servo::_configure()
{
    // Set node state
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    state_ = (state_example_servo*)node_state_->data;

}

void b_example_sub_servo::_set_config(nlohmann::json json_data)
{
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
    for (auto &&requirement : json_data.at("requirement"))
    {
        if (requirement.at("system") == node_system_list::PHYSICS)
        {
            requirement_p_node_list.push_back(requirement.at("type"));
            print_log("[requirement p node list]Node Name: " + get_p_node_name((physics_node_list)requirement.at("type")) + " (" + std::to_string((int)requirement.at("type")) + ")");
        }
    }
}

void b_example_sub_servo::_set_state()
{
    // Set node state
    node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;    
}

void b_example_sub_servo::cmd_executor()
{
    st_node_cmd cmd;
    if (node_cmd_->cmd_stack_.size() != 0)
    {
        print_log("HogeHoge");
        cmd = node_cmd_->cmd_stack_.pop();
        print_log(std::to_string(cmd.cmd_code.cmd_type));
        if ((b_node_cmd_list)cmd.cmd_code.cmd_type == b_node_cmd_list::CMD_EXAMPLE_SERVO)
        {
            cmd_example_servo* cmd_data = (cmd_example_servo*)cmd.data;
            switch (cmd_data->cmd_type)
            {
                case cmd_example_servo_type::RESET:
                    _reset_encoder_count();
                    break;
                case cmd_example_servo_type::ADD_COUNT:
                    _update_servo_state(1);
                    break;
                case cmd_example_servo_type::SUB_COUNT:
                    _update_servo_state(-1);
                    break;
                case cmd_example_servo_type::CHANGE_SM_READY:
                    ChangeReady();
                    break;
                case cmd_example_servo_type::CHANGE_SM_STABLE:
                    print_log("HogeHoge");
                    ChangeStable();
                    break;
                case cmd_example_servo_type::CHANGE_SM_FORCE_STOP:
                    PushForceStop();
                    break;
                case cmd_example_servo_type::RELEASE_FORCE_STOP:
                    ReleaseForceStop();
                    break;            
                default:
                    break;
            }
        }
    }
}

void b_example_sub_servo::_ready_processing()
{
    // Update servo state
    //_update_servo_state();
    cmd_executor();
    _display_servo_state();
    _set_state();
}

void b_example_sub_servo::_repair_processing()
{
    // Update servo state
    //_update_servo_state();
    _display_servo_state();
}

void b_example_sub_servo::_stable_processing()
{
    // Update servo state
    //_update_servo_state();
    cmd_executor();
    _display_servo_state();
    _set_state();
}

void b_example_sub_servo::_force_stop_processing()
{
    // Update servo state
    //_update_servo_state();
    cmd_executor();
    _display_servo_state();
}

void b_example_sub_servo::_end_processing()
{
}

void b_example_sub_servo::_initialize_processing()
{
}

/*
void b_example_sub_servo::_update_servo_state()
{
    // Update servo state
    state_->servo_angle += state_->cmd_servo_anglur_velocity * node_cycle_time_;
    if (fabs(state_->servo_angle) > M_PI)
    {
        is_occured_error_ = true;
    }
    else
    {
        is_occured_error_ = false;
    }
    print_log("[servo angle] " + std::to_string(state_->servo_angle) + " [rad]");
}
*/