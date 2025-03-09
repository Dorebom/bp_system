#pragma once

struct JoyconMoveCmd
{
	/* data */
	float x;
	float y;
	float z;
	float roll;
	bool is_torque_limit_released;
	bool is_adjustment;
	bool dummy[2];
	JoyconMoveCmd(/* args */)
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		roll = 0.0;
		is_torque_limit_released = false;
		is_adjustment = false;
	}
};