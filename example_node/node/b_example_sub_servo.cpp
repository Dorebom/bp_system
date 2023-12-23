#include "b_example_sub_servo.hpp"

#include <random>

// Setting file name
#define B_NODE_CONFIG_DIRECTORY "../example_node/config/"
#define B_NODE_CONFIG_FILE "b_example_sub_servo.json"

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

    // Init servo anglur velocity
    state_->cmd_servo_anglur_velocity = 0.0;
    // Init servo angle
    constexpr double MIN = -M_PI ;
    constexpr double MAX = M_PI;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(MIN, MAX);
    state_->servo_angle = distr(eng);
    print_log("[servo angle] " + std::to_string(state_->servo_angle) + " [rad]");

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
}

void b_example_sub_servo::cmd_executor()
{
    st_node_cmd cmd;
    if (node_cmd_->cmd_stack_.size() != 0)
    {
        cmd = node_cmd_->cmd_stack_.pop();
        if ((b_node_cmd_list)cmd.cmd_code.cmd_type == b_node_cmd_list::CMD_EXAMPLE_SERVO)
        {
            cmd_example_servo* cmd_data = (cmd_example_servo*)cmd.data;
            state_->cmd_servo_anglur_velocity = cmd_data->cmd_servo_anglur_velocity;
            print_log("[cmd servo anglur velocity] " + std::to_string(state_->cmd_servo_anglur_velocity) + " [rad/s]");
        }
    }
}

void b_example_sub_servo::_ready_processing()
{
    // Update servo state
    _update_servo_state();
}

void b_example_sub_servo::_repair_processing()
{
    // Update servo state
    _update_servo_state();
}

void b_example_sub_servo::_stable_processing()
{
    // Update servo state
    _update_servo_state();
}

void b_example_sub_servo::_force_stop_processing()
{
    // Update servo state
    _update_servo_state();
}

void b_example_sub_servo::_end_processing()
{
}

void b_example_sub_servo::_initialize_processing()
{
}

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