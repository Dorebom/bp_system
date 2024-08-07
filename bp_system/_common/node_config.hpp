#pragma once

#include <string>

struct node_config
{
    /* data */
    std::string node_name;
    std::string node_type;

    int task_recv_periodic_time;
    int task_send_periodic_time;
    int task_main_periodic_time;
    double transit_watch_dog_time;
    int cmd_stack_size;
    int sys_cmd_stack_size;
    int sys_state_stack_size;
    bool use_print_log;

    std::string recv_ip;
    std::string send_ip;
    int recv_port;
    int send_port;

    node_config()
    {
        node_name = "node_name";
        node_type = "node_type";
        task_recv_periodic_time = 100000;
        task_send_periodic_time = 100000;
        task_main_periodic_time = 100000;
        transit_watch_dog_time = 0.0;
        cmd_stack_size = 1;
        sys_cmd_stack_size = 1;
        sys_state_stack_size = 1;
        recv_ip = "127.0.0.1";
        send_ip = "127.0.0.1";
        recv_port = 50000;
        send_port = 50001;
        use_print_log = true;
    }
};