#include "basic_sub_servo.hpp"

#include <random>


void BasicSubServo::_update_servo_state(int add_value)
{
    if(node_state_machine_ == node_state_machine::STABLE || node_state_machine_ == node_state_machine::REPAIR)
    {
        state_->encoder_count += add_value;
    }
}

void BasicSubServo::_reset_encoder_count()
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(-20, 20);
    if (node_state_machine_ != node_state_machine::FORCE_STOP)
    {
        state_->encoder_count = distr(eng);
    }
}

void BasicSubServo::_display_servo_state()
{
    print_log("[encoder count] " + std::to_string(state_->encoder_count));
}

BasicSubServo::BasicSubServo(/* args */)
{
    change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
}

BasicSubServo::~BasicSubServo()
{
}

bool BasicSubServo::_any_to_initialize_processing()
{
    _reset_encoder_count();
    print_log("[encoder count] " + std::to_string(state_->encoder_count));
    return true;
}
bool BasicSubServo::_any_to_ready_processing(){return true;}
bool BasicSubServo::_any_to_force_stop_processing(){return true;}
bool BasicSubServo::_ready_to_repair_processing(){return true;}
bool BasicSubServo::_ready_to_stable_processing(){return true;}
bool BasicSubServo::_repair_to_stable_processing(){return true;}
bool BasicSubServo::_stable_to_repair_processing(){return true;}

bool BasicSubServo::_any_to_initialize_processing_after(){return true;}
bool BasicSubServo::_any_to_ready_processing_after(){return true;}
bool BasicSubServo::_any_to_force_stop_processing_after(){return true;}
bool BasicSubServo::_ready_to_repair_processing_after(){return true;}
bool BasicSubServo::_ready_to_stable_processing_after(){return true;}
bool BasicSubServo::_repair_to_stable_processing_after(){return true;}
bool BasicSubServo::_stable_to_repair_processing_after(){return true;}

void BasicSubServo::_configure()
{
    // Set node state
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    state_ = (st_servo_state*)node_state_->data;
    node_state_->state_code.node_id = node_id_;
    node_state_->state_code.data_size = sizeof(st_servo_state);
}

void BasicSubServo::_set_config(nlohmann::json &json_data)
{
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
}

void BasicSubServo::_set_state()
{
    // Set node state
    node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;
}

void BasicSubServo::cmd_executor()
{
    st_node_cmd cmd;
    st_servo_cmd* servo_cmd;

    if (node_cmd_->cmd_stack_.size() != 0)
    {
        cmd = node_cmd_->cmd_stack_.pop();
        //print_log("[cmd] " + std::to_string(cmd.cmd_code.cmd_type));
        servo_cmd = (st_servo_cmd*)cmd.data;
        switch ((servo_cmd_list)servo_cmd->cmd_type)
        {
        case servo_cmd_list::RESET:
            _reset_encoder_count();
            break;
        case servo_cmd_list::ADD_COUNT:
            _update_servo_state(1);
            break;
        case servo_cmd_list::SUB_COUNT:
            _update_servo_state(-1);
            break;
        case servo_cmd_list::CHANGE_SM_READY:
            ChangeReady();
            break;
        case servo_cmd_list::CHANGE_SM_STABLE:
            ChangeStable();
            break;
        case servo_cmd_list::CHANGE_SM_FORCE_STOP:
            PushForceStop();
            break;
        case servo_cmd_list::RELEASE_FORCE_STOP:
            ReleaseForceStop();
            break;
        default:
            break;
        }
    }
}

void BasicSubServo::_ready_processing()
{
    cmd_executor();
    _display_servo_state();
    _set_state();
}

void BasicSubServo::_repair_processing()
{
}

void BasicSubServo::_stable_processing()
{
    cmd_executor();
    _display_servo_state();
    _set_state();
}

void BasicSubServo::_force_stop_processing()
{
}

void BasicSubServo::_end_processing()
{
}

void BasicSubServo::_initialize_processing()
{
}