#include <iostream>
#include <chrono>
#include <thread>

#include "bp_system/b_system/b_hub.hpp"

int main()
{
    std::cout << "[Main]Robot System Start..." << std::endl;

    b_hub b_hub_;
    b_hub_.Start(true);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    b_hub_.exec_node(behavior_node_list::SIMPLE_NODE_A, behavior_node_list::HUB);

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        b_hub_.ChangeStable();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        b_hub_.End();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
    }
    std::cout << "[Main]Robot System End..." << std::endl;
    return 0;
}
