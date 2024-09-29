#include "basic_imu_node.hpp"

#include "../data_struct/cmd/st_imu_cmd.hpp"

BasicImuNode::BasicImuNode(/* args */)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    is_display_state_ = false;
}

BasicImuNode::~BasicImuNode()
{
}


void BasicImuNode::_initialize_processing()
{
}
void BasicImuNode::_ready_processing()
{
    get_state();
    display_state();
}
void BasicImuNode::_repair_processing()
{
    get_state();
    display_state();
}
void BasicImuNode::_stable_processing()
{
    get_state();
    display_state();
}
void BasicImuNode::_force_stop_processing()
{
    get_state();
    display_state();
}

void BasicImuNode::_end_processing()
{
}

/* node_state change process */
// -> initialize
bool BasicImuNode::_any_to_initialize_processing() {
    return true;
}
// -> ready (reset process)
bool BasicImuNode::_any_to_ready_processing() {return true;} // stable, repair and ready
// -> force stop
bool BasicImuNode::_any_to_force_stop_processing() {return true;} // stable, repair and ready
// -> normal flow
bool BasicImuNode::_ready_to_repair_processing() {return true;}
bool BasicImuNode::_ready_to_stable_processing() {return true;}
bool BasicImuNode::_repair_to_stable_processing() {return true;}
bool BasicImuNode::_stable_to_repair_processing() {return true;}

bool BasicImuNode::_any_to_initialize_processing_after(){return true;}
bool BasicImuNode::_any_to_ready_processing_after(){return true;}
bool BasicImuNode::_any_to_force_stop_processing_after(){return true;}
bool BasicImuNode::_ready_to_repair_processing_after(){return true;}
bool BasicImuNode::_ready_to_stable_processing_after(){return true;}
bool BasicImuNode::_repair_to_stable_processing_after(){return true;}
bool BasicImuNode::_stable_to_repair_processing_after(){return true;}

void BasicImuNode::_set_config(nlohmann::json json_data)
{
}

void BasicImuNode::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    imu_state_ = (st_imu_state*)node_state_->data;
    print_log("BasicImuNode Configure");
}

void BasicImuNode::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicImuNode::get_state()
{
}

void BasicImuNode::display_state()
{
    if (is_display_state_)
    {

    }
}

void BasicImuNode::user_cmd_executor()
{
    st_node_cmd cmd;
    if (node_cmd_->cmd_stack_.size() > 0)
    {
        cmd = node_cmd_->cmd_stack_.pop();
        print_log("[cmd] " + std::to_string(cmd.cmd_code.cmd_type));
        switch ((imu_cmd_list)cmd.cmd_code.cmd_type)
        {
        case imu_cmd_list::COMM_START:
            print_log("[cmd] COMM_START");
            break;
        case imu_cmd_list::COMM_END:
            print_log("[cmd] COMM_END");
            break;
        case imu_cmd_list::SET_PARAM:
            print_log("[cmd] SET_PARAM");
            break;
        case imu_cmd_list::DISPLAY_STATE:
            print_log("[cmd] DISPLAY_STATE");
            if (is_display_state_)
            {
                is_display_state_ = false;
            }
            else
            {
                is_display_state_ = true;
            }
            break;
        case imu_cmd_list::CHANGE_SM_READY:
            print_log("[cmd] CHANGE_SM_READY");
            ChangeReady();
            break;
        case imu_cmd_list::CHANGE_SM_STABLE:
            print_log("[cmd] CHANGE_SM_STABLE");
            ChangeStable();
            break;
        case imu_cmd_list::CHANGE_SM_REPAIR:
            print_log("[cmd] CHANGE_SM_REPAIR");
            ChangeRepair();
            break;
        case imu_cmd_list::CHANGE_SM_FORCE_STOP:
            print_log("[cmd] CHANGE_SM_FORCE_STOP");
            PushForceStop();
            break;
        case imu_cmd_list::RELEASE_FORCE_STOP:
            print_log("[cmd] RELEASE_FORCE_STOP");
            ReleaseForceStop();
            break;
        default:
            break;
        }
    }
}

void BasicImuNode::set_param(st_imu_param_cmd param)
{
}
