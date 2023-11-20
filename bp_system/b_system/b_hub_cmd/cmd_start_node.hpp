#pragma once

struct st_cmd_start_node
{
    int check_cmd_type;
    int node_type;
    bool use_udp_communication;
    st_cmd_start_node(/* args */)
    {
        check_cmd_type = 0;
        node_type = 0;
        use_udp_communication = false;
    }
};