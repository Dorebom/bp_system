#include "basic_pub_control.hpp"

BasicPubControl::BasicPubControl(/* args */)
{
    change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    is_get_rel_shared_ptr = false;
}

BasicPubControl::~BasicPubControl()
{
}

bool BasicPubControl::_any_to_initialize_processing(){return true;}
bool BasicPubControl::_any_to_ready_processing()
{
    return false;
}
bool BasicPubControl::_any_to_force_stop_processing(){return true;}
bool BasicPubControl::_ready_to_repair_processing(){return true;}
bool BasicPubControl::_ready_to_stable_processing()
{
    return true;
}
bool BasicPubControl::_repair_to_stable_processing(){return true;}
bool BasicPubControl::_stable_to_repair_processing(){return true;}

bool BasicPubControl::_any_to_initialize_processing_after(){return true;}
bool BasicPubControl::_any_to_ready_processing_after(){

    if (!is_get_rel_shared_ptr)
    {
        sub_servo_id = rel_node_jsonfile2id_map.find("sub_servo.json")->second;
        is_get_rel_shared_ptr = true;
    }

    return true;
}
bool BasicPubControl::_any_to_force_stop_processing_after(){return true;}
bool BasicPubControl::_ready_to_repair_processing_after(){return true;}
bool BasicPubControl::_ready_to_stable_processing_after(){return true;}
bool BasicPubControl::_repair_to_stable_processing_after(){return true;}
bool BasicPubControl::_stable_to_repair_processing_after(){return true;}

void BasicPubControl::_initialize_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicPubControl::_ready_processing()
{
    st_servo_state* servo_state1 = (st_servo_state*)b_node_state_map[sub_servo_id]->data;
    st_servo_cmd* cmd1;

    //print_log("[servo state]count: " + std::to_string(servo_state1->encoder_count));
    //print_log("[servo state]SM: " + get_node_state_machine_name(servo_state1->state_machine));
    if (servo_state1->state_machine == node_state_machine::READY)
    {
        st_node_cmd cmd;
        cmd.cmd_code.source = node_id_;
        cmd.cmd_code.destination = sub_servo_id;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.cmd_type = 0;
        cmd.cmd_code.data_size = sizeof(cmd);
        cmd.cmd_code.is_sys_cmd = false;

        cmd1 = (st_servo_cmd*)cmd.data;
        cmd1->cmd_type = servo_cmd_list::CHANGE_SM_STABLE;
        b_node_cmd_map[sub_servo_id]->cmd_stack_.push(cmd);
        print_log("[sub servo state]change to stable");
    }else if (servo_state1->state_machine == node_state_machine::STABLE)
    {
        ChangeStable();
    }

    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicPubControl::_repair_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicPubControl::_stable_processing()
{
    st_servo_state* servo_state1 = (st_servo_state*)b_node_state_map[sub_servo_id]->data;
    st_servo_cmd* cmd1;

    if (servo_state1->state_machine == node_state_machine::STABLE)
    {
        st_node_cmd cmd;
        cmd.cmd_code.source = node_id_;
        cmd.cmd_code.destination = sub_servo_id;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.cmd_type = 0;
        cmd.cmd_code.data_size = sizeof(st_servo_cmd);
        cmd.cmd_code.is_sys_cmd = false;

        cmd1 = (st_servo_cmd*)cmd.data;
        if (servo_state1->encoder_count < 0)
        {
            cmd1->cmd_type = servo_cmd_list::ADD_COUNT;
            b_node_cmd_map[sub_servo_id]->cmd_stack_.push(cmd);
        }
        else if(servo_state1->encoder_count > 0)
        {
            cmd1->cmd_type = servo_cmd_list::SUB_COUNT;
            b_node_cmd_map[sub_servo_id]->cmd_stack_.push(cmd);
        }
    }

    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicPubControl::_force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicPubControl::_end_processing()
{
}

void BasicPubControl::_set_config(nlohmann::json &json_data)
{
}

void BasicPubControl::_configure()
{
    // Set node state
    node_state_ = std::make_shared<node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    node_state_->state_code.node_id = node_id_;
    node_state_->state_code.data_size = sizeof(st_servo_state);

}

void BasicPubControl::_set_state()
{
    // Set node state
    node_state_->state_code.state_machine = node_state_machine_;
}
