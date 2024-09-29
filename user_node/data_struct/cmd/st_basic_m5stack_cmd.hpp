#pragma once

/*
 * GUI -> BHUB -> HERE -> M5Stack(PNode) と
 * 命令をスルーして渡すため、このノードへの命令として持つ部分と
 * M5Stack(PNode)で実行する部分が、共通したCmd＿Typeである。
 * また、M5Stack(PNode)からの状態量をRecvしたときに一旦Cmdに保存されるため
 * 。。。周囲1回分の処理を使うの勿体ないから、RecvしたときにStateかCmdか見分けて処理したほうがいい
 * ということで、受信用のプロトコルを被せることが設計上必要。
 */

enum class basic_m5stack_cmd_list : int
{
    /* data */
    // BASIC M5Stack Execute
    NONE,
    CHANGE_SM_STABLE,
    CHANGE_SM_READY,
    CHANGE_SM_FORCE_STOP,
    RELEASE_FORCE_STOP,
    // 
    REQUEST_STATE,
    START_STREAM_STATE,
    STOP_STREAM_STATE,
    SET_NODE_ID,
    SET_CONFIG,
    //
    RESET_ERROR,
    RESET_ALERT,
    // 以下、サーボ制御コマンド
    CONNECT_CAN,
    DISCONNECT_CAN,
    CHANGE_CONTROLLED_SRV_ID,
    CHANGE_SRV_POWER,  // ひとつだけサーボON/OFF
    CHANGE_SRV_CTRLMODE,

    SERVO_POSITION_CONTROL,
    SERVO_VELOCITY_CONTROL,
    SERVO_TORQUE_CONTROL,

    START_LOGGING,
    STOP_LOGGING,


    PERIOD_CMD // この行は削除しないこと
};


struct st_basic_m5stack_cmd
{
	/* data */
	basic_m5stack_cmd_list cmd_type;
	st_basic_m5stack_cmd(/* args */)
	{
		cmd_type = basic_m5stack_cmd_list::NONE;
	}
};
