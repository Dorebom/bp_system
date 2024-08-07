#include "dummy_servo_node.hpp"

#define B_NODE_CONFIG_DIRECTORY "../example_node/config/"
#define B_NODE_CONFIG_FILE "b_example_sub_servo.json"

void dummy_servo_node::_update_servo_state()
{
}

dummy_servo_node::dummy_servo_node()
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    set_config_directory_name(B_NODE_CONFIG_DIRECTORY);
    set_config_file_name(B_NODE_CONFIG_FILE);
}

dummy_servo_node::~dummy_servo_node()
{
}

void dummy_servo_node::_initialize_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void dummy_servo_node::_ready_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
    cmd_executor();
}

void dummy_servo_node::_repair_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
    cmd_executor();
}

void dummy_servo_node::_stable_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void dummy_servo_node::_force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void dummy_servo_node::_end_processing()
{
}

/* node_state change process */
// -> initialize
bool dummy_servo_node::_any_to_initialize_processing() {return true;}
// -> ready (reset process)
bool dummy_servo_node::_any_to_ready_processing() {return true;} // stable, repair and ready
// -> force stop
bool dummy_servo_node::_any_to_force_stop_processing() {return true;} // stable, repair and ready
// -> normal flow
bool dummy_servo_node::_ready_to_repair_processing() {return true;}
bool dummy_servo_node::_ready_to_stable_processing() {return true;}
bool dummy_servo_node::_repair_to_stable_processing() {return true;}
bool dummy_servo_node::_stable_to_repair_processing() {return true;}

bool dummy_servo_node::_any_to_initialize_processing_after(){return true;}
bool dummy_servo_node::_any_to_ready_processing_after(){return true;}
bool dummy_servo_node::_any_to_force_stop_processing_after(){return true;}
bool dummy_servo_node::_ready_to_repair_processing_after(){return true;}
bool dummy_servo_node::_ready_to_stable_processing_after(){return true;}
bool dummy_servo_node::_repair_to_stable_processing_after(){return true;}
bool dummy_servo_node::_stable_to_repair_processing_after(){return true;}

void dummy_servo_node::_set_config(nlohmann::json &json_data)
{
}

void dummy_servo_node::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    // node_state_->data を 構造体のポインタにキャスト
    state_ = (state_dummy_servo*)node_state_->data;
}

void dummy_servo_node::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void dummy_servo_node::cmd_executor()
{
    _sys_cmd_executor();
    if (node_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = node_cmd_->cmd_stack_.pop();
        //cmd_dummy_servo* cmd_dummy_servo = (cmd_dummy_servo*)cmd;

        switch ((cmd_dummy_servo_type)cmd.cmd_code.cmd_type)
        {
        // P systemに送るコマンド
        case cmd_dummy_servo_type::SERVO_OFF:
            /* code */
            break;
        case cmd_dummy_servo_type::SERVO_ON:
            /* code */
            break;
        case cmd_dummy_servo_type::STREAM_ON:
            /* code */
            break;
        case cmd_dummy_servo_type::STREAM_OFF:
            /* code */
            break;
        case cmd_dummy_servo_type::ONE_SHOT:
            /* code */
            break;
        // B node(API) 内部で処理するコマンド
        default:
            break;
        }
    }
    else
    {
        // Do nothing
    }
}