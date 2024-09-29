#pragma once

enum class servo_cmd_list : int
{
    /* data */
    NONE,
    ADD_COUNT,
    SUB_COUNT,
    RESET,
    CHANGE_SM_STABLE,
    CHANGE_SM_READY,
    CHANGE_SM_FORCE_STOP,
    RELEASE_FORCE_STOP,

    PERIOD_CMD // この行は削除しないこと
};


struct st_servo_cmd
{
    /* data */
    servo_cmd_list cmd_type;
    st_servo_cmd(/* args */)
    {
        cmd_type = servo_cmd_list::NONE;
    }
};
