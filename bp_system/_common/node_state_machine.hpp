#pragma once

#include <string>

enum class node_state_machine
{
    UNCONFIGURED,
    INITIALIZING,
    READY,
    STABLE,
    REPAIR,
    FORCE_STOP,
    TRANSITING
};