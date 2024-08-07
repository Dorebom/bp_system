#pragma once

//#include <cmath>

//#include "_common/node_state.hpp"

enum class ctrl_target_mode : int
{
    /* data */
    POSITION = 0,
    VELOCITY = 1,
    TORQUE = 2
};

enum class operation_mode : int
{
    /* data */
    AUTO = 0,
    MANUAL = 1
};

enum class target_reaching_mode : int
{
    /* data */
    POINT = 0,
    TRAJECTORY = 1
};

enum class ctrl_law_mode : int
{
    /* data */
    PID = 0,
    LQI = 1
};

struct state_dummy_servo
{
    /* data */
    node_state_machine state_machine;
    int error_count;
    int alert_count;
    int system_cycle_time; // ms
    int ctrl_cycle_time;   // ms

    int encoder_count;      // エンコーダカウント
    int multi_turn_count;   // 多回転数

    double joint_position;
    double joint_velocity;
    double joint_torque;

    ctrl_target_mode ctrl_tar_mode;
    operation_mode op_mode;
    target_reaching_mode tar_reach_mode;
    ctrl_law_mode ctrl_law;

    bool is_servo_on;
    bool is_stop_motoion;
    bool is_over_current;   // 過電流
    bool is_over_speed;     // 速度超過
    bool is_out_of_range;   // 位置範囲外
    bool is_emergency_stop; // 非常停止

    state_dummy_servo(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        error_count = 0;
        alert_count = 0;

        system_cycle_time = 0;
        ctrl_cycle_time = 0;

        encoder_count = 0;
        multi_turn_count = 0;

        joint_position = 0.0;
        joint_velocity = 0.0;
        joint_torque = 0.0;

        ctrl_tar_mode = ctrl_target_mode::POSITION;
        op_mode = operation_mode::AUTO;
        tar_reach_mode = target_reaching_mode::POINT;
        ctrl_law = ctrl_law_mode::PID;

        is_servo_on = false;
        is_stop_motoion = false;
        is_over_current = false;
        is_over_speed = false;
        is_out_of_range = false;
        is_emergency_stop = false;
    }
};
