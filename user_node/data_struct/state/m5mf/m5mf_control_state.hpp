#pragma once

#include <cstdint>

//#include "_common/node_cmd.hpp"
//#include "m5mf_control_state_limited.hpp"
#include "m5mf_ctrl_mode_list.hpp"
#include "m5mf_mechanical_frame_list.hpp"

namespace M5MF
{

#define SERVO_NUM 3
#define POSE_DIM 3
#define MAX_CONTROL_STATE_DATA_SIZE 250

    struct ControlStateCode
    {
        //
        MECHANICAL_FRAME_LIST mf_type;
        CTRL_MODE_LIST ctrl_mode;
        CTRL_MODE_LIST servo_ctrl_mode;
        //
        unsigned long timestamp;
        bool is_force_stop;
        bool is_connecting_device;
        bool is_power_on;
        bool is_torque_limit_released;
        bool is_adjustment;
        uint8_t data_size;  //
        bool dummy[2];

        ControlStateCode() {
            init();
        }

        void init() {
            mf_type = MECHANICAL_FRAME_LIST::NONE;
            ctrl_mode = CTRL_MODE_LIST::STAY;
            timestamp = 0;
            is_force_stop = false;
            is_connecting_device = false;
            is_power_on = false;
            data_size = 0;
            //
            is_torque_limit_released = false;
            is_adjustment = false;
        }

        void deepcopy(const ControlStateCode& state) {
            mf_type = state.mf_type;
            ctrl_mode = state.ctrl_mode;
            timestamp = state.timestamp;
            is_force_stop = state.is_force_stop;
            is_connecting_device = state.is_connecting_device;
            is_power_on = state.is_power_on;
            data_size = state.data_size;
            is_torque_limit_released = state.is_torque_limit_released;
            is_adjustment = state.is_adjustment;
        }
    };

    struct ControlState
    {
        ControlStateCode state_code;
        std::uint8_t data[MAX_CONTROL_STATE_DATA_SIZE];

        ControlState() {
            init();
        }

        void init() {
            state_code.init();
        }

        void deepcopy(const ControlState& state) {
            state_code.deepcopy(state.state_code);
            memcpy(data, state.data, state_code.data_size);
        }
        bool check_data_size_over(std::uint8_t size) {
            return size > MAX_CONTROL_STATE_DATA_SIZE;
        }
    };

    struct MFAllJointPosState
    {
        float act_joint_position[SERVO_NUM];
        float act_joint_velocity[SERVO_NUM];
        float act_joint_torque[SERVO_NUM];
        //
        float act_mf_position[POSE_DIM];  // Position(2) + Orientation(1)
        float act_mf_velocity[POSE_DIM];
        float act_mf_acceleration[POSE_DIM];
        float act_mf_torque[POSE_DIM];      // Force(2) + Torque(1)
        float act_mf_ext_torque[POSE_DIM];  // Force(2) + Torque(1)
        //
        float cmd_joint_position[SERVO_NUM];
        //float cmd_joint_velocity[SERVO_NUM];
        //float cmd_joint_acceleraton[SERVO_NUM];

        /*
        void deepcopy(const LocalControlState& state) {
            for (int i = 0; i < SERVO_NUM; i++) {
                act_joint_position[i] = state.act_joint_position[i];
                act_joint_velocity[i] = state.act_joint_velocity[i];
                act_joint_torque[i] = state.act_joint_torque[i];
                cmd_joint_position[i] = state.cmd_joint_position[i];
                cmd_joint_velocity[i] = state.cmd_joint_velocity[i];
                cmd_joint_acceleraton[i] = state.cmd_joint_aceleration[i];
            }
            for (int i = 0; i < POSE_DIM; i++) {
                act_mf_position[i] = state.act_mf_position[i];
                act_mf_velocity[i] = state.act_mf_velocity[i];
                act_mf_aceleration[i] = state.act_mf_aceleration[i];
                act_mf_torque[i] = state.act_mf_torque[i];
                act_mf_ext_torque[i] = state.act_mf_ext_torque[i];
            }
        }
        */
    };

    struct MFAllJointVelState
    {
        float act_joint_position[SERVO_NUM];
        float act_joint_velocity[SERVO_NUM];
        float act_joint_torque[SERVO_NUM];
        //
        float act_mf_position[POSE_DIM];  // Position(2) + Orientation(1)
        float act_mf_velocity[POSE_DIM];
        float act_mf_acceleration[POSE_DIM];
        float act_mf_torque[POSE_DIM];      // Force(2) + Torque(1)
        float act_mf_ext_torque[POSE_DIM];  // Force(2) + Torque(1)
        //
        float cmd_joint_velocity[SERVO_NUM];
    };

    struct MFAllJointTrqState
    {
        float act_joint_position[SERVO_NUM];
        float act_joint_velocity[SERVO_NUM];
        float act_joint_torque[SERVO_NUM];
        //
        float act_mf_position[POSE_DIM];  // Position(2) + Orientation(1)
        float act_mf_velocity[POSE_DIM];
        float act_mf_acceleration[POSE_DIM];
        float act_mf_torque[POSE_DIM];      // Force(2) + Torque(1)
        float act_mf_ext_torque[POSE_DIM];  // Force(2) + Torque(1)
        //
        float cmd_joint_torque[SERVO_NUM];
    };

    struct MFScaraPosState
    {
        float act_joint_position[SERVO_NUM];
        float act_joint_velocity[SERVO_NUM];
        float act_joint_torque[SERVO_NUM];
        //
        float act_mf_position[POSE_DIM];  // Position(2) + Orientation(1)
        float act_mf_velocity[POSE_DIM];
        float act_mf_acceleration[POSE_DIM];
        float act_mf_torque[POSE_DIM];      // Force(2) + Torque(1)
        float act_mf_ext_torque[POSE_DIM];  // Force(2) + Torque(1)
        //
        float cmd_mf_value[POSE_DIM];
        float cmd_joint_value[SERVO_NUM];
        //
        float debug_data_1[3];
        float debug_data_2[3];
        float debug_data_3[3];
    };




    struct M5MFAPIControlState
    {
        ControlStateCode state_code;
        double act_joint_position[SERVO_NUM];
        double act_joint_velocity[SERVO_NUM];
        double act_joint_aceleration[SERVO_NUM];
        double act_joint_torque[SERVO_NUM];
        //
        double act_mf_position[POSE_DIM];
        double act_mf_velocity[POSE_DIM];
        double act_mf_acceleration[POSE_DIM];
        //
        float cmd_mf_value[POSE_DIM];
        float cmd_joint_value[SERVO_NUM];
        //
        float debug_data_1[3];
        float debug_data_2[3];
        float debug_data_3[3];

        void deepcopy(const ControlStateCode& state) {
			state_code.deepcopy(state);
		}

        void deepcopy(const MFAllJointPosState& state) {
			for (int i = 0; i < SERVO_NUM; i++) {
				act_joint_position[i] = state.act_joint_position[i];
				act_joint_velocity[i] = state.act_joint_velocity[i];
				act_joint_torque[i] = state.act_joint_torque[i];
			}
            for (int i = 0; i < POSE_DIM; i++) {
                act_mf_position[i] = state.act_mf_position[i];
                act_mf_velocity[i] = state.act_mf_velocity[i];
                act_mf_acceleration[i] = state.act_mf_acceleration[i];
            }
        }

        void deepcopy(const MFAllJointVelState& state) {
            for (int i = 0; i < SERVO_NUM; i++) {
                act_joint_position[i] = state.act_joint_position[i];
                act_joint_velocity[i] = state.act_joint_velocity[i];
                act_joint_torque[i] = state.act_joint_torque[i];
            }
            for (int i = 0; i < POSE_DIM; i++) {
                act_mf_position[i] = state.act_mf_position[i];
                act_mf_velocity[i] = state.act_mf_velocity[i];
                act_mf_acceleration[i] = state.act_mf_acceleration[i];
            }
        }

        void deepcopy(const MFAllJointTrqState& state) {
            for (int i = 0; i < SERVO_NUM; i++) {
                act_joint_position[i] = state.act_joint_position[i];
                act_joint_velocity[i] = state.act_joint_velocity[i];
                act_joint_torque[i] = state.act_joint_torque[i];
            }
            for (int i = 0; i < POSE_DIM; i++) {
                act_mf_position[i] = state.act_mf_position[i];
                act_mf_velocity[i] = state.act_mf_velocity[i];
                act_mf_acceleration[i] = state.act_mf_acceleration[i];
            }
        }

        void deepcopy(const MFScaraPosState& state) {
            for (int i = 0; i < SERVO_NUM; i++) {
                act_joint_position[i] = state.act_joint_position[i];
                act_joint_velocity[i] = state.act_joint_velocity[i];
                act_joint_torque[i] = state.act_joint_torque[i];
                cmd_joint_value[i] = state.cmd_joint_value[i];
            }
            for (int i = 0; i < POSE_DIM; i++) {
                act_mf_position[i] = state.act_mf_position[i];
                act_mf_velocity[i] = state.act_mf_velocity[i];
                act_mf_acceleration[i] = state.act_mf_acceleration[i];
                cmd_mf_value[i] = state.cmd_mf_value[i];
            }
			for (int i = 0; i < 3; i++) {
				debug_data_1[i] = state.debug_data_1[i];
				debug_data_2[i] = state.debug_data_2[i];
				debug_data_3[i] = state.debug_data_3[i];
			}
        }
    };

}