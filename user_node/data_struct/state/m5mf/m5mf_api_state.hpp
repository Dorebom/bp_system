#pragma once

#include "m5mf_system_state.hpp"
#include "m5mf_control_state.hpp"

namespace M5MF
{
	struct M5mfApiState
	{
		/* API data */
		node_state_machine state_machine;
		bool is_connected_device;
		bool dummy1[3];
		/* Device data */
		SystemState system_state;
		//ControlState control_state;
		M5MFAPIControlState control_state;

		M5mfApiState(/* args */)
		{
			state_machine = node_state_machine::UNCONFIGURED;
			is_connected_device = false;
			for (size_t i = 0; i < 3; i++)
			{
				dummy1[i] = false;
			}
		}
	};

}