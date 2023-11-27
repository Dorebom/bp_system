#include "node_util.hpp"

/* 目的：デバッグ用の、名前を取得する関数群 */
/* Nodeを増やすときは更新してください */
/* #Add Node */

std::string get_b_node_name(behavior_node_list node)
{
    switch (node)
    {
    case behavior_node_list::HUB:
        return "b_hub";
        break;
    case behavior_node_list::PHYSICS_HUB:
        return "b_physics_hub";
        break;
    case behavior_node_list::SIMPLE_NODE_A:
        return "b_simple_node_a";
        break;
    case behavior_node_list::SIMPLE_NODE_B:
        return "b_simple_node_b";
        break;
    case behavior_node_list::EXAMPLE_SUB_SERVO:
        return "b_example_sub_servo";
        break;
    case behavior_node_list::EXAMPLE_PUB_CONTROL:
        return "b_example_pub_control";
        break;  
    default:
        return "unknown";
        break;
    }
}

std::string get_p_node_name(physics_node_list node)
{
    switch (node)
    {
    case physics_node_list::HUB:
        return "p_hub";
        break;
    case physics_node_list::SIMPLE_NODE_A:
        return "p_simple_node_a";
        break;
    case physics_node_list::SIMPLE_NODE_B:
        return "p_simple_node_b";
        break;
    default:
        return "unknown";
        break;
    }
}

std::string get_node_state_machine_name(node_state_machine state)
{
    switch (state)
    {
    case node_state_machine::INITIALIZING:
        return "INITIALIZING";
        break;
    case node_state_machine::READY:
        return "READY";
        break;
    case node_state_machine::STABLE:
        return "STABLE";
        break;
    case node_state_machine::REPAIR:
        return "REPAIR";
        break;
    case node_state_machine::FORCE_STOP:
        return "FORCE_STOP";
        break;
    case node_state_machine::TRANSITING:
        return "TRANSITING";
        break;
    case node_state_machine::UNCONFIGURED:
        return "UNCONFIGURED";
        break;    
    default:
        return "unknown";
        break;
    }
}