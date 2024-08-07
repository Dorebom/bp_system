#pragma once

enum class cmd_dummy_servo_type : int
{
    /* data */
    NONE = 0,
    RESET = 1,
    CHANGE_SM_STABLE = 2,
    CHANGE_SM_READY = 3,
    CHANGE_SM_FORCE_STOP = 4,
    RELEASE_FORCE_STOP = 5,
    RESET_ERROR = 6,
    RESET_ALERT = 7,
    ONE_SHOT = 8,
    STREAM_ON = 9,
    STREAM_OFF = 10,
    SET_CONFIG = 11,
    SERVO_ON = 12,
    SERVO_OFF = 13,
    STOP_MOTION = 14,
    CTRL_MODE_01 = 15
};

struct cmd_dummy_servo
{
    /* data */
    cmd_dummy_servo_type cmd_type;
    std::uint8_t data[50];
    cmd_dummy_servo(/* args */)
    {
        cmd_type = cmd_dummy_servo_type::NONE;
        for (size_t i = 0; i < 50; i++)
        {
            data[i] = 0;
        }
    }
};