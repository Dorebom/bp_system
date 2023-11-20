#pragma once

#include <string>

enum class node_system_list : int
{
    // 必ず連番にすること
    MIND = 0,
    BEHAVIOR = 1,
    PHYSICS = 2,
    PERIOD_NODE // この行は削除しないこと
};

enum class behavior_node_list : int
{
    // 必ず連番にすること
    HUB = 0,
    PHYSICS_HUB = 1,
    SIMPLE_NODE_A = 2,
    SIMPLE_NODE_B = 3,    
    PERIOD_NODE // この行は削除しないこと
};

enum class physics_node_list : int
{
    // 必ず連番にすること
    HUB = 0,
    SIMPLE_NODE_A = 1,
    SIMPLE_NODE_B = 2,
    PERIOD_NODE // この行は削除しないこと
};