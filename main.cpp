#include <iostream>
#include <chrono>
#include <thread>

#include "bp_system/b_system/hub.hpp"

#include "example_node/node/b_simple_node_a.hpp"
#include "example_node/node/b_simple_node_b.hpp"

int main()
{
    std::cout << "[Main]Robot System Start..." << std::endl;

    hub hub_;
    hub_.Start();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    //b_hub_.exec_node(behavior_node_list::SIMPLE_NODE_A, behavior_node_list::HUB);

    b_simple_node_a node_a;
    b_simple_node_b node_b;

    hub_.store_node(node_a, "b_simple_node_a");
    hub_.store_node(node_b, "b_simple_node_b");

    hub_.show_stored_node();

    hub_.exec_node("b_simple_node_a", "b_simple_node_a.json", "../example_node/config/");

    //b_hub_.exec_node(behavior_node_list::EXAMPLE_PUB_CONTROL, behavior_node_list::HUB);

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        hub_.ChangeStable();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        hub_.End();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
    }
    std::cout << "[Main]Robot System End..." << std::endl;
    return 0;
}
