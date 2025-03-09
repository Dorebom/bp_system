#include "m5mf_api.hpp"

#define M5MF_API_LOG_DATA_SIZE 60000

void M5mfApi::check_error_and_alert()
{
    // >> ERROR
    is_occured_error_ = false;
    if (!state_->is_connected_device)
    {
        print_log("Device Disconnected");
        is_occured_error_ = true;
    }
    // >> WARNING
    is_occured_warning_ = false;
}

void M5mfApi::_update_state()
{
    //state_->encoder_count = std::rand();
}

void M5mfApi::cmd_connect()
{
    if (!is_send_data_exist_ && !state_->is_connected_device) 
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
        send_data_.cmd_code.destination = -1;
        send_data_.cmd_code.priority = 0;
        send_data_.cmd_code.cmd_id = 0;
        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CONNECT;
        send_data_.cmd_code.data_size = 0;
        send_data_.cmd_code.is_sys_cmd = false;
        send_data_.cmd_code.is_used_msgpack = false;
        is_send_data_exist_ = true;
    }
}

void M5mfApi::cmd_change_forcestop()
{
    if (!is_send_data_exist_)
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CHANGE_SM_FORCE_STOP;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_release_forcestop()
{
    if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::RELEASE_FORCE_STOP;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_start_stream()
{
    if (!is_send_data_exist_) 
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::START_STREAM_STATE;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
        is_streaming_ = true;
	}
}

void M5mfApi::cmd_stop_stream()
{
	if (!is_send_data_exist_) 
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::STOP_STREAM_STATE;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
        is_streaming_ = false;
	}
}

void M5mfApi::cmd_start_logging()
{
	// 通常時のstateを取得する
    /*
    if (!is_send_data_exist_)
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::START_LOGGING;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
    }
    */
    start_logging();
}

void M5mfApi::cmd_stop_logging()
{
    // 通常時のstateを取得する
    /*
    if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::STOP_LOGGING;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
    */
    stop_logging();
}

void M5mfApi::start_logging()
{
    if (state_->is_connected_device)
    {
        control_state_log.clear();

        is_logging_ = true;
    }
}

void M5mfApi::stop_logging()
{
	is_logging_ = false;

    // ログを保存する
    // 日時YYYYMMDDHHMMSSを取得
    time_t t = time(nullptr);
    struct tm* lt = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y%m%d%H%M%S", lt);
    // ファイル名に日時を付与
    std::string file_name = "log_" + std::string(date) + ".csv";
    std::ofstream ofs(file_name);

    //ofs << "sensor_weight, sensor_weight_raw_adc" << std::endl;
    //ofs << "timestamp, is_power_on, servo_id, ctrl_mode, act_joint_position, act_joint_velocity, act_joint_torque, cmd_joint_position, cmd_joint_velocity, cmd_joint_torque" << std::endl;
    ofs << "timestamp, is_power_on, is_force_stop, is_connecting_device, mf_type, ctrl_mode, data_size, "
		<< "act_jpos_1, act_jpos_2, act_jpos_3, "
        << "act_jvel_1, act_jvel_2, act_jvel_3, "
        << "act_jtrq_1, act_jtrq_2, act_jtrq_3, "
        << "act_x, act_y, act_rz, "
		<< "NONE" << std::endl;
    for (size_t i = 0; i < control_state_log.size(); i++)
	{

        if (i == 0)
		{
            ofs << (control_state_log[i].state_code.timestamp - control_state_log[0].state_code.timestamp) * 0.000001 << ","
                << control_state_log[i].state_code.is_power_on << ","
                << control_state_log[i].state_code.is_force_stop << ","
                << control_state_log[i].state_code.is_connecting_device << ","
                << (int)control_state_log[i].state_code.mf_type << ","
                << (int)control_state_log[i].state_code.ctrl_mode << ","
                //<< control_state_log[i].state_code.data_size << ","
                //
                << control_state_log[i].act_joint_position[0] << ","
                << control_state_log[i].act_joint_position[1] << ","
                << control_state_log[i].act_joint_position[2] << ","
                << control_state_log[i].act_joint_velocity[0] << ","
                << control_state_log[i].act_joint_velocity[1] << ","
                << control_state_log[i].act_joint_velocity[2] << ","
                << control_state_log[i].act_joint_torque[0] << ","
                << control_state_log[i].act_joint_torque[1] << ","
                << control_state_log[i].act_joint_torque[2] << ","
				<< control_state_log[i].act_mf_position[0] << ","
				<< control_state_log[i].act_mf_position[1] << ","
				<< control_state_log[i].act_mf_position[2] << ","
                << control_state_log[i].cmd_joint_value[0] << ","
                << control_state_log[i].cmd_joint_value[1] << ","
                << control_state_log[i].cmd_joint_value[2] << ","
                << control_state_log[i].cmd_mf_value[0] << ","
                << control_state_log[i].cmd_mf_value[1] << ","
                << control_state_log[i].cmd_mf_value[2] << ","
				<< control_state_log[i].debug_data_1[0] << ","
				<< control_state_log[i].debug_data_1[1] << ","
				<< control_state_log[i].debug_data_1[2] << ","
				<< control_state_log[i].debug_data_2[0] << ","
				<< control_state_log[i].debug_data_2[1] << ","
				<< control_state_log[i].debug_data_2[2] << ","
				<< control_state_log[i].debug_data_3[0] << ","
				<< control_state_log[i].debug_data_3[1] << ","
				<< control_state_log[i].debug_data_3[2] << ","
                //
                << 0 << std::endl;
        }
        else
        {
            ofs << (control_state_log[i].state_code.timestamp - control_state_log[0].state_code.timestamp) * 0.000001 << ","
				<< control_state_log[i].state_code.is_power_on << ","
				<< control_state_log[i].state_code.is_force_stop << ","
				<< control_state_log[i].state_code.is_connecting_device << "," 
                << (int)control_state_log[i].state_code.mf_type << ","
				<< (int)control_state_log[i].state_code.ctrl_mode << ","
				//<< control_state_log[i].state_code.data_size << ","
                //
                << control_state_log[i].act_joint_position[0] << ","
                << control_state_log[i].act_joint_position[1] << ","
                << control_state_log[i].act_joint_position[2] << ","
                << control_state_log[i].act_joint_velocity[0] << ","
                << control_state_log[i].act_joint_velocity[1] << ","
                << control_state_log[i].act_joint_velocity[2] << ","
                << control_state_log[i].act_joint_torque[0] << ","
                << control_state_log[i].act_joint_torque[1] << ","
                << control_state_log[i].act_joint_torque[2] << ","
				<< control_state_log[i].act_mf_position[0] << ","
				<< control_state_log[i].act_mf_position[1] << ","
				<< control_state_log[i].act_mf_position[2] << ","
                << control_state_log[i].cmd_joint_value[0] << ","
                << control_state_log[i].cmd_joint_value[1] << ","
                << control_state_log[i].cmd_joint_value[2] << ","
                << control_state_log[i].cmd_mf_value[0] << ","
                << control_state_log[i].cmd_mf_value[1] << ","
                << control_state_log[i].cmd_mf_value[2] << ","
                << control_state_log[i].debug_data_1[0] << ","
                << control_state_log[i].debug_data_1[1] << ","
                << control_state_log[i].debug_data_1[2] << ","
                << control_state_log[i].debug_data_2[0] << ","
                << control_state_log[i].debug_data_2[1] << ","
                << control_state_log[i].debug_data_2[2] << ","
                << control_state_log[i].debug_data_3[0] << ","
                << control_state_log[i].debug_data_3[1] << ","
                << control_state_log[i].debug_data_3[2] << ","
                << 0 << std::endl;
		}
		if (i % 1000 == 0)
		{
			std::cout << "Logging: " << i
				<< std::endl;
        }


    }
	ofs.close();

    control_state_log.erase(control_state_log.begin(), control_state_log.end());

    print_log("Logging End");
}

void M5mfApi::logging()
{
    if (is_logging_)
    {
		// 最大値を超えたらログを保存する
		if (control_state_log.size() > M5MF_API_LOG_DATA_SIZE)
		{
			stop_logging();
		}
        else
        {
            // ログを保存する
            control_state_log.push_back(state_->control_state);
        }
    }
}

void M5mfApi::cmd_servo_on()
{
	if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_POWER_ON;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_servo_off()
{
	if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_POWER_OFF;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST ctrl_mode)
{
    if (!is_send_data_exist_)
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
        send_data_.cmd_code.destination = -1;
        send_data_.cmd_code.priority = 0;
        send_data_.cmd_code.cmd_id = 0;
        switch (ctrl_mode)
        {
        case M5MF::CTRL_MODE_LIST::POSITION:
            send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_POSITION_CONTROL;
            break;
        case M5MF::CTRL_MODE_LIST::VELOCITY:
            send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_VELOCITY_CONTROL;
            break;
        case M5MF::CTRL_MODE_LIST::TORQUE:
            send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_TORQUE_CONTROL;
            break;
		case M5MF::CTRL_MODE_LIST::POSIPOSI:
			send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_POSIPOSI_CONTROL;
			break;
        case M5MF::CTRL_MODE_LIST::POSITRQ:
            send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_POSITRQ_CONTROL;
            break;
        case M5MF::CTRL_MODE_LIST::VELOTRQ:
            send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_VELOTRQ_CONTROL;
            break;
        case M5MF::CTRL_MODE_LIST::CIRCPOSIPOSI:
			send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_CIRCPOSIPOSI_CONTROL;
			break;
        case M5MF::CTRL_MODE_LIST::CIRCPOSITRQ:
			send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_CIRCPOSITRQ_CONTROL;
			break;
        default:
            break;
        }
        send_data_.cmd_code.data_size = 0;
        send_data_.cmd_code.is_sys_cmd = false;
        send_data_.cmd_code.is_used_msgpack = false;
        is_send_data_exist_ = true;
    }
}

void M5mfApi::cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST mf_type)
{
    if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		switch (mf_type)
		{
		case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
			send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_MF1;
			break;
		case M5MF::MECHANICAL_FRAME_LIST::SCARA:
			send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_MF2;
			break;
		default:
			break;
		}
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_set_mechanical_position_to_zero()
{
    if (!is_send_data_exist_)
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
        send_data_.cmd_code.destination = -1;
        send_data_.cmd_code.priority = 0;
        send_data_.cmd_code.cmd_id = 0;
        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SET_MECHANICAL_POSITION_TO_ZERO;
        send_data_.cmd_code.data_size = 0;
        send_data_.cmd_code.is_sys_cmd = false;
        send_data_.cmd_code.is_used_msgpack = false;
        is_send_data_exist_ = true;
    }
}

void M5mfApi::cmd_move(JoyconMoveCmd* cmd)
{
    if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.data_size = sizeof(JoyconMoveCmd);
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;

        MFAllJointPosCmd* ap_cmd_data = (MFAllJointPosCmd*)send_data_.data;
        MFAllJointVelCmd* av_cmd_data = (MFAllJointVelCmd*)send_data_.data;
        MFAllJointTrqCmd* at_cmd_data = (MFAllJointTrqCmd*)send_data_.data;

        MFScaraPosCmd* sp_cmd_data = (MFScaraPosCmd*)send_data_.data;
        MFScaraVelCmd* sv_cmd_data = (MFScaraVelCmd*)send_data_.data;

        switch (state_->control_state.state_code.mf_type)
        {
            case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
                //
                // print_log("CTRL MODE: " + std::to_string((int)state_->control_state.state_code.ctrl_mode));
                switch (state_->control_state.state_code.ctrl_mode)
                {
                    case M5MF::CTRL_MODE_LIST::POSITION:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_ALLJOINT_POSITION_CONTROL;
                        ap_cmd_data->ref_joint_position[0] = state_->control_state.act_joint_position[0] + 0.8 * cmd->x;
                        ap_cmd_data->ref_joint_position[1] = state_->control_state.act_joint_position[1] + 0.8 * cmd->y;
                        ap_cmd_data->ref_joint_position[2] = state_->control_state.act_joint_position[2] + 0.8 * cmd->z;
                        print_log("Move REF(P): " 
                            + std::to_string(ap_cmd_data->ref_joint_position[0]) + ", " 
                            + std::to_string(ap_cmd_data->ref_joint_position[1]) + ", " 
                            + std::to_string(ap_cmd_data->ref_joint_position[2]));
                        print_log("Move ACT(P): " 
							+ std::to_string(state_->control_state.act_joint_position[0]) + ", " 
							+ std::to_string(state_->control_state.act_joint_position[1]) + ", " 
							+ std::to_string(state_->control_state.act_joint_position[2]));
                        break;
                    case M5MF::CTRL_MODE_LIST::VELOCITY:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_ALLJOINT_VELOCITY_CONTROL;
                        av_cmd_data->ref_joint_velocity[0] = 5.0 * cmd->x;
                        av_cmd_data->ref_joint_velocity[1] = 5.0 * cmd->y;
                        av_cmd_data->ref_joint_velocity[2] = 5.0 * cmd->z;
					    break;
					case M5MF::CTRL_MODE_LIST::TORQUE:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_ALLJOINT_TORQUE_CONTROL;
                        at_cmd_data->ref_joint_torque[0] = 0.8 * cmd->x;
                        at_cmd_data->ref_joint_torque[1] = 0.8 * cmd->y;
                        at_cmd_data->ref_joint_torque[2] = 0.8 * cmd->z;
					    break;
                    default:
                        break;
                }
				break;
            case M5MF::MECHANICAL_FRAME_LIST::SCARA:
                switch (state_->control_state.state_code.ctrl_mode)
                {
                    case M5MF::CTRL_MODE_LIST::POSIPOSI:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_POSIPOSI_CONTROL;
                        sp_cmd_data->ref_scara_pos[0] = state_->control_state.act_mf_position[0] + 0.01 * cmd->x;
                        sp_cmd_data->ref_scara_pos[1] = state_->control_state.act_mf_position[1] + 0.01 * cmd->y;
                        sp_cmd_data->ref_scara_pos[2] = state_->control_state.act_mf_position[2] - 0.02 * cmd->z;
                        sp_cmd_data->is_adjustment = cmd->is_adjustment;
                        // display act mf position
                        /*
                        print_log("Move ACT(PP): "
                            + std::to_string(state_->control_state.act_mf_position[0]) + ", "
                            + std::to_string(state_->control_state.act_mf_position[1]) + ", "
                            + std::to_string(state_->control_state.act_mf_position[2]));
                        print_log("Move Cmd(PP): "
                            + std::to_string(sp_cmd_data->ref_scara_pos[0]) + ", "
                            + std::to_string(sp_cmd_data->ref_scara_pos[1]) + ", "
                            + std::to_string(sp_cmd_data->ref_scara_pos[2]));
                        */
                        break;
					case M5MF::CTRL_MODE_LIST::CIRCPOSIPOSI:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_CIRCPOSIPOSI_CONTROL;
						sp_cmd_data->ref_scara_pos[0] = state_->control_state.act_mf_position[0] + 0.01 * cmd->x;
						sp_cmd_data->ref_scara_pos[1] = state_->control_state.act_mf_position[1] + 0.01 * cmd->y;
						sp_cmd_data->ref_scara_pos[2] = state_->control_state.act_mf_position[2] - 0.02 * cmd->z;
						sp_cmd_data->is_adjustment = cmd->is_adjustment;
						break;
                    case M5MF::CTRL_MODE_LIST::POSITRQ:
						send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_POSITRQ_CONTROL;
						sp_cmd_data->ref_scara_pos[0] = state_->control_state.act_mf_position[0] + 0.01 * cmd->x;
						sp_cmd_data->ref_scara_pos[1] = state_->control_state.act_mf_position[1] + 0.01 * cmd->y;
						sp_cmd_data->ref_scara_pos[2] = state_->control_state.act_mf_position[2] - 0.02 * cmd->z;
						sp_cmd_data->is_adjustment = cmd->is_adjustment;
						sp_cmd_data->is_torque_limit_released = cmd->is_torque_limit_released;
                        break;
                    case M5MF::CTRL_MODE_LIST::VELOTRQ:
                        send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_VELOTRQ_CONTROL;
                        sv_cmd_data->ref_scara_vel[0] = 0.3 * cmd->x;
                        sv_cmd_data->ref_scara_vel[1] = 0.3 * cmd->y;
                        sv_cmd_data->ref_scara_vel[2] = -0.1 * cmd->z;
                        sv_cmd_data->is_adjustment = cmd->is_adjustment;
                        sv_cmd_data->is_torque_limit_released = cmd->is_torque_limit_released;
                        break;
                    case M5MF::CTRL_MODE_LIST::CIRCPOSITRQ:
						send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SCARA_CIRCPOSITRQ_CONTROL;
                        sp_cmd_data->ref_scara_pos[0] = state_->control_state.act_mf_position[0] + 0.01 * cmd->x;
                        sp_cmd_data->ref_scara_pos[1] = state_->control_state.act_mf_position[1] + 0.01 * cmd->y;
                        sp_cmd_data->ref_scara_pos[2] = state_->control_state.act_mf_position[2] - 0.02 * cmd->z;
						sp_cmd_data->is_adjustment = cmd->is_adjustment;
						sp_cmd_data->is_torque_limit_released = cmd->is_torque_limit_released;
						break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
		is_send_data_exist_ = true;
	}
}

void M5mfApi::cmd_alljoint_position_control()
{
    if (!is_send_data_exist_)
	{
		send_data_.cmd_code.source = node_state_->state_code.node_id;
		send_data_.cmd_code.destination = -1;
		send_data_.cmd_code.priority = 0;
		send_data_.cmd_code.cmd_id = 0;
		send_data_.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_ALLJOINT_POSITION_CONTROL;
		send_data_.cmd_code.data_size = 0;
		send_data_.cmd_code.is_sys_cmd = false;
		send_data_.cmd_code.is_used_msgpack = false;

        MFAllJointPosCmd* cmd_data = (MFAllJointPosCmd*)send_data_.data;
        


		is_send_data_exist_ = true;
	}
}

void M5mfApi::display_state()
{
	if (display_cnt % 6 == 0 && is_streaming_ )
	{
        print_log("Time: " + std::to_string(state_->control_state.state_code.timestamp));
        print_log("Pos: " + std::to_string(state_->control_state.act_joint_position[0]) + ", "
                          + std::to_string(state_->control_state.act_joint_position[1]) + ", " 
                          + std::to_string(state_->control_state.act_joint_position[2]));
        display_cnt = 0;
    }
    display_cnt++;
}


M5mfApi::M5mfApi()
{
    change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    control_state_log.reserve(M5MF_API_LOG_DATA_SIZE);
}

M5mfApi::~M5mfApi()
{
}

void M5mfApi::_initialize_processing(){}
void M5mfApi::_ready_processing()
{
    check_error_and_alert();
    cmd_executor();
    _set_state();
    _update_state();

    if (is_occured_error_)
    {
        // >> Check Connect device
        if (!state_->is_connected_device)
        {
            // 1秒ごとに接続を試みる
            std::this_thread::sleep_for(std::chrono::seconds(1));
            print_log("Waiting Device Connected ...");
            cmd_connect();
        }
    }
    else
    {
        if (is_occured_warning_)
		{
			ChangeRepair();
		}
        else
        {
            ChangeStable();
        }
    }
}
void M5mfApi::_stable_processing()
{
    check_error_and_alert();
    cmd_executor();
    _set_state();
    _update_state();

    //logging();
    //display_state();
}
void M5mfApi::_repair_processing()
{
    check_error_and_alert();
    cmd_executor();
    _set_state();
    _update_state();

    if (is_occured_warning_)
    {
    }
    else
    {
        ChangeStable();
    }
}
void M5mfApi::_force_stop_processing()
{
    check_error_and_alert();
	cmd_executor();
	_set_state();
	_update_state();
}
void M5mfApi::_end_processing(){}

bool M5mfApi::_any_to_initialize_processing(){
    node_state_->state_code.data_size = sizeof(M5MF::M5mfApiState);
    return true;
}
bool M5mfApi::_any_to_ready_processing(){return true;}
bool M5mfApi::_any_to_force_stop_processing(){return true;}
bool M5mfApi::_ready_to_repair_processing(){return true;}
bool M5mfApi::_ready_to_stable_processing(){return true;}
bool M5mfApi::_repair_to_stable_processing(){return true;}
bool M5mfApi::_stable_to_repair_processing(){return true;}
bool M5mfApi::_any_to_initialize_processing_after(){return true;}
bool M5mfApi::_any_to_ready_processing_after(){return true;}
bool M5mfApi::_any_to_force_stop_processing_after(){return true;}
bool M5mfApi::_ready_to_repair_processing_after(){return true;}
bool M5mfApi::_ready_to_stable_processing_after(){return true;}
bool M5mfApi::_repair_to_stable_processing_after(){return true;}
bool M5mfApi::_stable_to_repair_processing_after(){return true;}

void M5mfApi::_set_config(nlohmann::json &json_data)
{
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
}

void M5mfApi::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    //
    state_ = (M5MF::M5mfApiState*)node_state_->data;
}

void M5mfApi::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;
}

void M5mfApi::cmd_executor()
{
    st_node_cmd cmd;

    if (node_cmd_->cmd_stack_.size() != 0)
    {
        cmd = node_cmd_->cmd_stack_.pop();
        //
        JoyconMoveCmd* joycon_move_cmd;
        //

        //print_log("[cmd] " + std::to_string(cmd.cmd_code.cmd_type));
        //m5stack_cmd = cmd;
        switch ((M5MF::M5MF_CMD_LIST)cmd.cmd_code.cmd_type)
        {
        case M5MF::M5MF_CMD_LIST::CHANGE_SM_FORCE_STOP:
            print_log("CHANGE FORCE STOP");
			cmd_change_forcestop();
			break;
        case M5MF::M5MF_CMD_LIST::RELEASE_FORCE_STOP:
            print_log("RELEASE FORCE STOP");
            cmd_release_forcestop();
        case M5MF::M5MF_CMD_LIST::REQUEST_STATE:
            print_log("REQUEST STATE");
            cmd_connect();
			break;
        case M5MF::M5MF_CMD_LIST::START_LOGGING:
            print_log("START LOGGING");
            cmd_start_logging();
            break;
        case M5MF::M5MF_CMD_LIST::STOP_LOGGING:
            print_log("STOP LOGGING");
            cmd_stop_logging();
            break;
        case M5MF::M5MF_CMD_LIST::CONNECT:
            print_log("CONNECT");
            cmd_connect();
			break;
        case M5MF::M5MF_CMD_LIST::START_STREAM_STATE:
            print_log("START STREAM");
            cmd_start_stream();
            break;
        case M5MF::M5MF_CMD_LIST::STOP_STREAM_STATE:
            print_log("STOP STREAM");
            cmd_stop_stream();
            break;
            // >> Control System Command List 100 - 199
        case M5MF::M5MF_CMD_LIST::CS_POWER_ON:
            print_log("SERVO ON");
            if (is_streaming_)
			{
				cmd_servo_on();
			}
            break;
        case M5MF::M5MF_CMD_LIST::CS_POWER_OFF:
            print_log("SERVO OFF");
            if (is_streaming_)
            {
                cmd_servo_off();
            }
            break;
        case M5MF::M5MF_CMD_LIST::CS_SET_MECHANICAL_POSITION_TO_ZERO:
            print_log("SET_MECHANICAL_POSITION_TO_ZERO");
            cmd_set_mechanical_position_to_zero();
            break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_MF_UP:
			print_log("CHANGE MF UP");
            switch (state_->control_state.state_code.mf_type)
            {
			case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
                print_log("CHANGE MF2 SCARA");
                cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::SCARA);
				break;
			case M5MF::MECHANICAL_FRAME_LIST::SCARA:
                print_log("CHANGE MF1 ALL JOINT");
				cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::ALLJOINT);
				break;
            default:
                break;
            }
            break;
		case M5MF::M5MF_CMD_LIST::CS_CHANGE_MF_DOWN:
			print_log("CHANGE MF DOWN");
			switch (state_->control_state.state_code.mf_type)
			{
			case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
				print_log("CHANGE MF2 SCARA");
				cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::SCARA);
				break;
			case M5MF::MECHANICAL_FRAME_LIST::SCARA:
				print_log("CHANGE MF1 ALL JOINT");
				cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::ALLJOINT);
				break;
			default:
				break;
			}
			break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_CONTROL_MODE_UP:
			print_log("CHANGE CONTROL MODE UP");
            switch (state_->control_state.state_code.mf_type)
            {
            case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
                switch (state_->control_state.state_code.ctrl_mode)
                {
                case M5MF::CTRL_MODE_LIST::POSITION:
                    print_log("CHANGE VELOCITY CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::VELOCITY);
                    break;
                case M5MF::CTRL_MODE_LIST::VELOCITY:
                    print_log("CHANGE TORQUE CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::TORQUE);
                    break;
                case M5MF::CTRL_MODE_LIST::TORQUE:
                    print_log("CHANGE POSITION CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSITION);
                    break;
                //
                case M5MF::CTRL_MODE_LIST::STAY:
                    print_log("CHANGE POSITION CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSITION);
                    break;
                default:
                    print_log("CHANGE POSITION CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSITION);
                    break;
                }
                break;
            case M5MF::MECHANICAL_FRAME_LIST::SCARA:
                switch (state_->control_state.state_code.ctrl_mode)
                {
                case M5MF::CTRL_MODE_LIST::POSIPOSI:
                    print_log("CHANGE CIRCPOSIPOSI CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::CIRCPOSIPOSI);
                    break;
                case M5MF::CTRL_MODE_LIST::CIRCPOSIPOSI:
                    print_log("CHANGE POSITRQ CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSITRQ);
                    break;
				case M5MF::CTRL_MODE_LIST::POSITRQ:
					print_log("CHANGE VELOTRQ CONTROL");
					cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::VELOTRQ);
					break;
                case M5MF::CTRL_MODE_LIST::VELOTRQ:
                    print_log("CHANGE CIRCPOSITRQ CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::CIRCPOSITRQ);
                    break;
				case M5MF::CTRL_MODE_LIST::CIRCPOSITRQ:
					print_log("CHANGE POSIPOSI CONTROL");
					cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSIPOSI);
					break;
                //
                case M5MF::CTRL_MODE_LIST::STAY:
                    print_log("CHANGE POSIPOSI CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSIPOSI);
                    break;
                default:
                    print_log("CHANGE POSIPOSI CONTROL");
                    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSIPOSI);
                    break;
                }
                break;
            }
			break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_POSITION_CONTROL:
            print_log("CHANGE POSITION CONTROL");
		    cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::POSITION);
			break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_VELOCITY_CONTROL:
    		print_log("CHANGE VELOCITY CONTROL");
            cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::VELOCITY);
            break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_TORQUE_CONTROL:
            print_log("CHANGE TORQUE CONTROL");
            cmd_change_ctrl_mode(M5MF::CTRL_MODE_LIST::TORQUE);
            break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_MF1:
            print_log("CHANGE MF1");
			cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::ALLJOINT);
			break;
        case M5MF::M5MF_CMD_LIST::CS_CHANGE_MF2:
			print_log("CHANGE MF2");
			cmd_change_mf_type(M5MF::MECHANICAL_FRAME_LIST::SCARA);
			break;
        case M5MF::M5MF_CMD_LIST::CS_MOVE:
        	//print_log("MOVE");
			if (is_streaming_)
			{
				cmd_move((JoyconMoveCmd*)cmd.data);
			}
			break;
        case M5MF::M5MF_CMD_LIST::CS_ALLJOINT_POSITION_CONTROL:
			print_log("ALLJOINT POSITION CONTROL");
            if (is_streaming_)
			{
				cmd_alljoint_position_control();
			}
			break;
        default:
            break;
        }
    }
}

void M5mfApi::_task_recv()
{
    //RecvData recv_data_;
    print_log("Recv Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_recv_periodic_time * 0.001)) + "ms");

    comm_m5stack_.set_recv_address(node_config_.recv_ip.c_str(), node_config_.recv_port);
    st_node_cmd temp_cmd;
    st_node_state temp_state;
    common_state_code temp_common_state_code;

    UdpDataFrame recv_frame_;
    int recv_data_size = 0;
    int actual_marker_point = 0;
    //
    int data_watch_cnt = 0;

    while (check_node_running())
    {
        if (check_allowed_comm_udp())
        {
            recv_data_size = comm_m5stack_.recv_data(reinterpret_cast<uint8_t*>(&recv_frame_), sizeof(UdpDataFrame));
            if (recv_data_size > 0 && recv_frame_.stack_marker_num <= recv_frame_.max_stack_marker_size)
			{
                if (state_->is_connected_device == false)
                {
					print_log("Device Connected");
                    state_->is_connected_device = true;
                }

                for (size_t i = 0; i < recv_frame_.stack_marker_num; i++)
                {
                    auto marker_ = recv_frame_.stack_marker[i];
                    if (marker_ < 0)
                    {
                        // Command
                        // memcopyで一度取り出した後に、コマンドを実行するcmd stackに登録する
                        if (static_cast<unsigned long long>(std::abs((int)marker_)) * recv_frame_.one_stack_size > sizeof(st_node_cmd))
						{
							print_log("Error: Marker size is too large");
							continue;
						}
                        else {
                            memcpy_s(&temp_cmd, 
                                     sizeof(st_node_cmd), 
                                     &recv_frame_.data[actual_marker_point * recv_frame_.one_stack_size], 
                                     static_cast<rsize_t>(std::abs((int)marker_)) * recv_frame_.one_stack_size);
                            node_cmd_->cmd_stack_.push(temp_cmd);
                        }
                        actual_marker_point += marker_ * -1;
                    }
                    else
                    {
                        M5MF::ControlState* temp_c_state;
                        // State
                        memcpy_s(&temp_state,
								 sizeof(st_node_state),
								 &recv_frame_.data[actual_marker_point * recv_frame_.one_stack_size], 
								 static_cast<rsize_t>(marker_) * recv_frame_.one_stack_size);
                        switch (temp_state.state_code.node_id)
                        {
                            case 1:
                                state_->system_state = *(M5MF::SystemState*)temp_state.data;
    							break;
                            case 2:
                                temp_c_state = (M5MF::ControlState*)temp_state.data;
                                switch (temp_c_state->state_code.mf_type)
                                {
                                    case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
                                        switch (temp_c_state->state_code.ctrl_mode)
                                        {
                                            case M5MF::CTRL_MODE_LIST::POSITION:
											    state_->control_state.deepcopy(*(M5MF::MFAllJointPosState*)temp_c_state->data);
 											break;
                                            case M5MF::CTRL_MODE_LIST::VELOCITY:
												state_->control_state.deepcopy(*(M5MF::MFAllJointVelState*)temp_c_state->data);
												break;
											case M5MF::CTRL_MODE_LIST::TORQUE:
												state_->control_state.deepcopy(*(M5MF::MFAllJointTrqState*)temp_c_state->data);
												break;
                                        default:
                                            break;
                                        }
                                        break;                  
                                    case M5MF::MECHANICAL_FRAME_LIST::SCARA:
                                        state_->control_state.deepcopy(*(M5MF::MFScaraPosState*)temp_c_state->data);
                                        break;
                                    default:
                                        break;
                                }
                                state_->control_state.state_code.deepcopy(temp_c_state->state_code);
                                logging();
                                stream_watchdog_cnt = 0;
                                break;
                            case 3:
                                temp_c_state = (M5MF::ControlState*)temp_state.data;
                                switch (temp_c_state->state_code.mf_type)
                                {
                                case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
                                    switch (temp_c_state->state_code.ctrl_mode)
                                    {
                                    case M5MF::CTRL_MODE_LIST::POSITION:
                                        state_->control_state.deepcopy(*(M5MF::MFAllJointPosState*)temp_c_state->data);
                                        break;
                                    case M5MF::CTRL_MODE_LIST::VELOCITY:
                                        state_->control_state.deepcopy(*(M5MF::MFAllJointVelState*)temp_c_state->data);
                                        break;
                                    case M5MF::CTRL_MODE_LIST::TORQUE:
                                        state_->control_state.deepcopy(*(M5MF::MFAllJointTrqState*)temp_c_state->data);
                                        break;
                                    default:
                                        break;
                                    }
                                    break;
                                case M5MF::MECHANICAL_FRAME_LIST::SCARA:
                                    state_->control_state.deepcopy(*(M5MF::MFScaraPosState*)temp_c_state->data);
                                    break;
                                default:
                                    break;
                                }
                                logging();
                                break;
                            default:
                                break;
                        }
                        actual_marker_point += marker_;
                    }
                }
                actual_marker_point = 0;
                recv_frame_.stack_marker_num = 0;
			}
            stream_watchdog_cnt++;
            if (stream_watchdog_cnt > 10)
            {
                stream_watchdog_cnt = 0;
                print_log("Stream Watchdog Over");
                is_streaming_ = false;
            }
        }

        if (data_watch_cnt % 50 == 0)
        {
            //print_log("JPos: " + std::to_string(state_->control_state.act_joint_position[0]) + ", "
            //    + std::to_string(state_->control_state.act_joint_position[1]) + ", "
            //    + std::to_string(state_->control_state.act_joint_position[2]));
            print_log("Pos: " + std::to_string(state_->control_state.act_mf_position[0]) + ", "
                + std::to_string(state_->control_state.act_mf_position[1]) + ", "
                + std::to_string(state_->control_state.act_mf_position[2]));
            data_watch_cnt = 0;
        }
        data_watch_cnt++;

        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_recv_periodic_time));
    }
    print_log("Recv Thread End");

}

