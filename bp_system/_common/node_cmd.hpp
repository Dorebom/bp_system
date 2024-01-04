#pragma once

#include "node_state_machine.hpp"
#include "stacker.hpp"

struct common_cmd_code
{
    /* data */
    int source;
    int destination;
    int priority;
    int cmd_id;
    int cmd_type;
    int data_size;
    bool is_sys_cmd;
    bool is_used_msgpack;
    common_cmd_code(/* args */)
    {
        source = 0;
        destination = 0;
        priority = 0;
        cmd_id = 0;
        cmd_type = 0;
        data_size = 0;
        is_sys_cmd = false;
        is_used_msgpack = false;
    }
};

struct st_node_cmd
{
    /* data */
    common_cmd_code cmd_code;
    std::uint8_t data[1024];
    st_node_cmd(/* args */)
    {
        cmd_code = common_cmd_code();
    }
};

class node_cmd
{
private:
    /* data */
public:
    circular_stacker<st_node_cmd> cmd_stack_;
    node_cmd(size_t stack_size) : cmd_stack_(stack_size)
    {
    }
    ~node_cmd()
    {}
};
