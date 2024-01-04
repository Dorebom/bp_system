#include "node_util.hpp"

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