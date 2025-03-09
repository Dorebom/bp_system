#pragma once

#include "../data_struct/state/m5mf/m5mf_mechanical_frame_list.hpp"
//#include "../data_struct/state/m5mf/m5mf_ctrl_mode_list.hpp

#define SERVO_NUM_ 3
#define POSE_DIM 3

struct MFAllJointPosCmd
{
    float ref_joint_position[SERVO_NUM_];
	bool is_adjustment;
	bool dummy[3];
};

struct MFAllJointVelCmd
{
    float ref_joint_velocity[SERVO_NUM_];
    bool is_adjustment;
    bool dummy[3];
};

struct MFAllJointTrqCmd
{
    float ref_joint_torque[SERVO_NUM_];
    bool is_adjustment;
    bool dummy[3];
};

struct MFScaraPosCmd
{
    float ref_scara_pos[POSE_DIM];
	bool is_torque_limit_released;
	bool is_adjustment;
	bool dummy[2];
};

struct MFScaraVelCmd
{
    float ref_scara_vel[POSE_DIM];
    bool is_torque_limit_released;
    bool is_adjustment;
    bool dummy[2];
};
