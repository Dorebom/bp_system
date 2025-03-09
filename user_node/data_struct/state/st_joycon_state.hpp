#pragma once

#include <cmath>
#include <vector>

#include "_common/node_state.hpp"

struct st_joycon_state
{
    /* data */
    
    // TimeStamp
    double timestamp;
    int button[12];
    double axis[6];

    st_joycon_state(/* args */)
    {
        for (int i = 0; i < 12; i++)
        {
            button[i] = 0;
        }
        for (int i = 0; i < 6; i++)
        {
            axis[i] = 0.0;
        }
        timestamp = 0.0;
    }

    void deepcopy(const st_joycon_state& state)
	{
		timestamp = state.timestamp;
		for (int i = 0; i < 12; i++)
		{
			button[i] = state.button[i];
		}
		for (int i = 0; i < 6; i++)
		{
			axis[i] = state.axis[i];
		}
	}
};
