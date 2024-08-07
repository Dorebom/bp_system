#include <iostream>
#include <chrono>
#include <thread>

#include "bp_system/b_system/hub.hpp"

//#include "example_node/node/b_simple_node_a.hpp"
//#include "example_node/node/b_simple_node_b.hpp"
#include "user_node/node/basic_sub_servo.hpp"
#include "user_node/node/basic_pub_control.hpp"
#include "user_node/node/basic_gui_comm.hpp"

int main()
{
    std::cout << "[Main]Robot System Start..." << std::endl;

    //hub hub_("behavior_hub.json", "../bp_system/_config/");
    hub hub_("behavior_hub.json", "../../../../bp_system/_config/");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    hub_.show_node_json_file("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    hub_.show_node_json_file("b_hub", "behavior_hub.json", "../../../_config/");
    hub_.show_node_json_file("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    hub_.show_node_json_file("b_hub", "behavior_hub.json", "../../../_config/");
    hub_.show_node_json_file("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    hub_.show_node_json_file("b_hub", "behavior_hub.json", "../../../_config/");
    hub_.show_node_json_file("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    hub_.show_node_json_file("b_hub", "behavior_hub.json", "../../../_config/");


    std::this_thread::sleep_for(std::chrono::seconds(1));

    /* Store Nodes */
    //b_simple_node_a node_a;
    //b_simple_node_b node_b;

    BasicSubServo servo_node;
    BasicPubControl pub_node;

    BasicGuiComm gui_comm_node;

    //hub_.store_node(node_a, "b_simple_node_a");
    //hub_.store_node(node_b, "b_simple_node_b");

    hub_.store_node(servo_node, "basic_sub_servo");
    hub_.store_node(pub_node, "basic_pub_control");

    hub_.store_node(gui_comm_node, "basic_gui_comm");

    hub_.show_stored_node();
    // <<- END Store Nodes

    hub_.Start();

    hub_.ChangeStable();

    //hub_.show_node_json_file("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    //hub_.show_node_json_file("b_hub", "behavior_hub.json", "../../../_config/");

    //hub_.exec_node("basic_sub_servo", "sub_servo.json", "../../../user_node/config/");
    hub_.exec_node("basic_pub_control", "pub_control.json", "../../../user_node/config/");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    hub_.exit_node("pub_control");
    hub_.exit_node("sub_servo");

    //hub_.exec_node("basic_gui_comm", "gui_comm.json", "user_node/config/");

    while(1)
    {
        if (hub_.check_hub_running() == false)
        {
            break;
        }
        hub_.display_state();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        hub_.End();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Main]Robot System End..." << std::endl;
    return 1;
}
