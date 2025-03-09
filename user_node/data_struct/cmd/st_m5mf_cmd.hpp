#pragma once

/*
 * GUI -> BHUB -> HERE -> M5Stack(PNode) と
 * 命令をスルーして渡すため、このノードへの命令として持つ部分と
 * M5Stack(PNode)で実行する部分が、共通したCmd＿Typeである。
 * また、M5Stack(PNode)からの状態量をRecvしたときに一旦Cmdに保存されるため
 * 。。。周囲1回分の処理を使うの勿体ないから、RecvしたときにStateかCmdか見分けて処理したほうがいい
 * ということで、受信用のプロトコルを被せることが設計上必要。
 */

// >> command list
#include "m5mf/m5mf_cmd_list.hpp"
// >> each command struct
#include "m5mf/m5mf_cmd_connect.hpp"	// CONNECT

#include "m5mf/m5mf_control_cmd.hpp"

namespace M5MF
{

	struct st_basic_m5stack_cmd
	{
		/* data */
		M5MF_CMD_LIST cmd_type;
		st_basic_m5stack_cmd(/* args */)
		{
			cmd_type = M5MF_CMD_LIST::NONE;
		}
	};

}