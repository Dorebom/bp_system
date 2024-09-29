#include "basic_gui_comm.hpp"

void BasicGuiComm::_update_state()
{
    state_->encoder_count = std::rand();
}

BasicGuiComm::BasicGuiComm()
{
    change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;
}

BasicGuiComm::~BasicGuiComm()
{
}

void BasicGuiComm::_initialize_processing(){}
void BasicGuiComm::_ready_processing()
{
    cmd_executor();
    _set_state();
    _update_state();
    print_log("[encoder count] " + std::to_string(state_->encoder_count));
}
void BasicGuiComm::_stable_processing()
{
    cmd_executor();
    _set_state();
    _update_state();
}
void BasicGuiComm::_repair_processing(){}
void BasicGuiComm::_force_stop_processing(){}
void BasicGuiComm::_end_processing(){}

bool BasicGuiComm::_any_to_initialize_processing(){
    node_state_->state_code.data_size = sizeof(st_gui_comm_state);
    return true;
}
bool BasicGuiComm::_any_to_ready_processing(){return true;}
bool BasicGuiComm::_any_to_force_stop_processing(){return true;}
bool BasicGuiComm::_ready_to_repair_processing(){return true;}
bool BasicGuiComm::_ready_to_stable_processing(){return true;}
bool BasicGuiComm::_repair_to_stable_processing(){return true;}
bool BasicGuiComm::_stable_to_repair_processing(){return true;}
bool BasicGuiComm::_any_to_initialize_processing_after(){return true;}
bool BasicGuiComm::_any_to_ready_processing_after(){return true;}
bool BasicGuiComm::_any_to_force_stop_processing_after(){return true;}
bool BasicGuiComm::_ready_to_repair_processing_after(){return true;}
bool BasicGuiComm::_ready_to_stable_processing_after(){return true;}
bool BasicGuiComm::_repair_to_stable_processing_after(){return true;}
bool BasicGuiComm::_stable_to_repair_processing_after(){return true;}

void BasicGuiComm::_set_config(nlohmann::json &json_data)
{
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
}

void BasicGuiComm::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    state_ = (st_gui_comm_state*)node_state_->data;
}

void BasicGuiComm::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;
}

void BasicGuiComm::cmd_executor()
{
}

