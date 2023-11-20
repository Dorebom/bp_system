#pragma once

#include "node_state_machine.hpp"
#include "stacker.hpp"

struct p_cmd_code
{
    /* data */
    int source;
    int destination;
    int priority;
    int cmd_id;
    int cmd_type;
    int data_size;
    p_cmd_code(/* args */)
    {
        source = 0;
        destination = 0;
        priority = 0;
        cmd_id = 0;
        cmd_type = 0;
        data_size = 0;
    }
};

struct st_p_node_cmd
{
    /* data */
    p_cmd_code cmd_code;
    std::uint8_t data[1024];
    st_p_node_cmd(/* args */)
    {
    }
};

class p_node_cmd
{
private:
    /* data */
public:
    circular_stacker<st_p_node_cmd> cmd_stack_;
    p_node_cmd(size_t stack_size) : cmd_stack_(stack_size)
    {
    }
    ~p_node_cmd()
    {}
};
