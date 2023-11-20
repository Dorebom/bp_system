#include "p_node.hpp"

void p_node::Start_Node(bool use_udp_communication, std::shared_ptr<p_node_cmd> hub_cmd)
{
    p_hub_cmd_ = hub_cmd;
    Start(use_udp_communication);
}

void p_node::set_config(nlohmann::json json_data)
{
    print_log("set config");
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
    _set_config(json_data);
}

void p_node::initialize_processing()
{
    _initialize_processing();
}
void p_node::ready_processing()
{
    _ready_processing();
}
void p_node::repair_processing()
{
    _repair_processing();
}
void p_node::stable_processing()
{
    _stable_processing();
}
void p_node::force_stop_processing()
{
    _force_stop_processing();   
}
void p_node::transit_processing()
{}

bool p_node::any_to_initialize_processing()
{
    return _any_to_initialize_processing();
}
bool p_node::any_to_ready_processing()
{
    return _any_to_ready_processing();
}
bool p_node::any_to_force_stop_processing()
{
    return _any_to_force_stop_processing();
}
bool p_node::ready_to_repair_processing()
{   
    return _ready_to_repair_processing();
}
bool p_node::ready_to_stable_processing()
{
    return _ready_to_stable_processing();
}
bool p_node::repair_to_stable_processing()
{
    return _repair_to_stable_processing();
}
bool p_node::stable_to_repair_processing()
{
    return _stable_to_repair_processing();
}