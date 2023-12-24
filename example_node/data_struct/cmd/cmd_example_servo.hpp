#pragma once

enum class cmd_example_servo_type : int
{
    /* data */
    NONE,
    ADD_COUNT,
    SUB_COUNT,
    RESET,
    CHANGE_SM_STABLE,
    CHANGE_SM_READY,
    CHANGE_SM_FORCE_STOP,
    RELEASE_FORCE_STOP
};

struct cmd_example_servo
{
    /* data */
    cmd_example_servo_type cmd_type;
    cmd_example_servo(/* args */)
    {
        cmd_type = cmd_example_servo_type::NONE;
    }
};
