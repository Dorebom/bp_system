#pragma once

// uint8_t

#define MAX_STACK_MARKER_NUM 20
#define ONE_STACK_SIZE 50
#define MAX_SEND_STATE_DATA_SIZE 1400 // > MAX_STACK_MARKER_NUM * ONE_STACK_SIZE

struct b_hub_send_state
{
	// 固定ヘッダーのデータサイズ
	int fixed_header_data_size;
	int max_stack_marker_size;
	int send_state_header_size;
	int one_stack_size;
	// バイナリデータのスタック目印の総数
	int stack_marker_num;
	// バイナリデータのスタック目印を記録する配列
	std::uint8_t stack_marker[MAX_STACK_MARKER_NUM];
	// バイナリデータをコピーする配列
	std::uint8_t data[MAX_SEND_STATE_DATA_SIZE];

	b_hub_send_state(/* args */)
	{
		fixed_header_data_size = 0;
		max_stack_marker_size = MAX_STACK_MARKER_NUM;
		send_state_header_size = sizeof(fixed_header_data_size) 
							   + sizeof(max_stack_marker_size) 
							   + sizeof(send_state_header_size)
							   + sizeof(stack_marker_num)
							   + sizeof(one_stack_size)
							   + MAX_STACK_MARKER_NUM;
		one_stack_size = ONE_STACK_SIZE;
		//
		stack_marker_num = 0;
		memset(stack_marker, 0, MAX_STACK_MARKER_NUM);
		memset(data, 0, MAX_SEND_STATE_DATA_SIZE);
	}
};