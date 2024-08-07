#pragma once

struct cmd_pick_shared_ptr
{
    int check_cmd_type;
    int node_type;
    cmd_pick_shared_ptr(/* args */)
    {
        check_cmd_type = 2;
        node_type = 0;
    }
};
