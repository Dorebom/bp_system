#pragma once

enum class gui_comm_cmd_list : int
{
    /* data */
    NONE,
    CHANGE_SM_STABLE,
    CHANGE_SM_READY,
    CHANGE_SM_FORCE_STOP,
    RELEASE_FORCE_STOP,

    PERIOD_CMD // この行は削除しないこと
};


struct st_gui_comm_cmd
{
    /* data */
    gui_comm_cmd_list cmd_type;
    st_gui_comm_cmd(/* args */)
    {
        cmd_type = gui_comm_cmd_list::NONE;
    }
};
