#pragma once

#include "_common/node_state.hpp"

/*
 * M5Stackの基本データ構造
 * ---
 * BNodeでもStateMachineとか使うので、M5Stack側のStateMachineとかは
 * st_m5_system_stateの中に含めてやる。
 */

enum class basic_servo_ctrl_cmd_list : int
{
    STAY,
    POSITION,
    VELOCITY,
    TORQUE,
    MOTION
};

struct st_m5_system_state
{
    bool is_init_scale;
	bool dummy[7];

	float battery_voltage;
	float battery_voltage_raw_adc;

	st_m5_system_state() {
		is_init_scale = false;
		battery_voltage = 0.0f;
		battery_voltage_raw_adc = 0.0;
        for (size_t i = 0; i < 7; i++)
        {
            dummy[i] = false;
        }
	}
};

struct st_m5_control_state_udp
{
    uint8_t servo_id;
    uint8_t ctrl_mode;
    bool is_power_on;
    bool dummy;
    unsigned long timestamp;

    float act_joint_position;
    float act_joint_velocity;
    float act_joint_torque;

    float cmd_joint_position;
    float cmd_joint_velocity;
    float cmd_joint_torque;
    float cmd_joint_current;
    float dummy2;

};

struct st_m5_control_state
{
    bool is_init_scale;
    bool is_power_on;
    bool act_can_connection_status;
    bool is_init_joint_pos;
    unsigned long timestamp;

    int servo_id;
    basic_servo_ctrl_cmd_list ctrl_mode;
    double act_joint_position;
    double act_joint_velocity;
    double act_joint_torque;

    double cmd_joint_position;
    double cmd_joint_velocity;
    double cmd_joint_torque;

    double act_joint_position_0;
    double act_joint_velocity_0;
    double act_joint_torque_0;

    // for control
    double sum_error_vel;
    double sum_error_torque;

    // servo info

    float sensor_weight;
    int32_t sensor_weight_raw_adc;

    int waiting_servo_id;

    st_m5_control_state() {
        // >> Servo
        act_can_connection_status = false;
        servo_id = 0;  // 0: not used, 1: servo1, 2: servo2
        is_init_joint_pos = false;
        is_power_on = false;
        ctrl_mode = basic_servo_ctrl_cmd_list::STAY;

        // >> weight scale
        is_init_scale = false;
        sensor_weight = 0.0f;
        sensor_weight_raw_adc = 0.0;

        act_joint_position = 0.0;
        act_joint_velocity = 0.0;
        act_joint_torque = 0.0;

        cmd_joint_position = 0.0;
        cmd_joint_velocity = 0.0;
        cmd_joint_torque = 0.0;

        act_joint_position_0 = 0.0;
        act_joint_velocity_0 = 0.0;
        act_joint_torque_0 = 0.0;

        sum_error_vel = 0.0;
    }

    void deepcopy(const st_m5_control_state& state) {
        // >> Servo
        act_can_connection_status = state.act_can_connection_status;
        servo_id = state.servo_id;
        is_init_joint_pos = state.is_init_joint_pos;
        is_power_on = state.is_power_on;
        ctrl_mode = state.ctrl_mode;
        timestamp = state.timestamp;

        // >> weight scale
        is_init_scale = state.is_init_scale;
        sensor_weight = state.sensor_weight;
        sensor_weight_raw_adc = state.sensor_weight_raw_adc;

        act_joint_position = state.act_joint_position;
        act_joint_velocity = state.act_joint_velocity;
        act_joint_torque = state.act_joint_torque;

        cmd_joint_position = state.cmd_joint_position;
        cmd_joint_velocity = state.cmd_joint_velocity;
        cmd_joint_torque = state.cmd_joint_torque;

        act_joint_position_0 = state.act_joint_position_0;
        act_joint_velocity_0 = state.act_joint_velocity_0;
        act_joint_torque_0 = state.act_joint_torque_0;

        sum_error_vel = state.sum_error_vel;
    }

    void smalldeepcopy(const st_m5_control_state_udp& state)
    {
        servo_id = (int)state.servo_id;
        ctrl_mode = (basic_servo_ctrl_cmd_list)((int)state.ctrl_mode);
        is_power_on = state.is_power_on;

        timestamp = state.timestamp;

        act_joint_position = state.act_joint_position;
        act_joint_velocity = state.act_joint_velocity;
        act_joint_torque = state.act_joint_torque;

        cmd_joint_position = state.cmd_joint_position;
        cmd_joint_velocity = state.cmd_joint_velocity;
        cmd_joint_torque = state.cmd_joint_torque;
    }
};

struct st_m5_state
{
    st_m5_system_state system;
    st_m5_control_state control;
};

struct st_basic_m5stack_state
{
    /* common data */
    node_state_machine state_machine;
    int error_count;
    int alert_count;
    /* data */
    bool is_connected_device;
    bool dummy1[3];
    /* 以下は、M5Stack本体側のデータ */
    st_m5_state m5_state;

    st_basic_m5stack_state(/* args */)
    {
        state_machine = node_state_machine::UNCONFIGURED;
        error_count = 0;
        alert_count = 0;
        is_connected_device = false;

    }
};
