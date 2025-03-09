#pragma once

struct M5mfCmdConnect
{
	/* data */
	uint32_t port;
	uint8_t ip_address[4];

	M5mfCmdConnect(/* args */)
	{
		port = 0;
		for (int i = 0; i < 4; i++)
		{
			ip_address[i] = 0;
		}
	}
};