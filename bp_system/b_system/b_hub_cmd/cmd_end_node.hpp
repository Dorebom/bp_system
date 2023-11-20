#pragma once

struct st_cmd_end_node
{
    int check_cmd_type;
    int node_type;
    st_cmd_end_node(/* args */)
    {
        check_cmd_type = 1;
        node_type = 0;
    }
};