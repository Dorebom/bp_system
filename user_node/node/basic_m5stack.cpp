#include "basic_m5stack.hpp"

void BasicM5stack::_update_state()
{
    //state_->encoder_count = std::rand();
}

void BasicM5stack::connect()
{
    if (!is_send_data_exist_) 
    {
        send_data_.cmd_code.source = node_state_->state_code.node_id;
        send_data_.cmd_code.destination = -1;
        send_data_.cmd_code.priority = 0;
        send_data_.cmd_code.cmd_id = 0;
        send_data_.cmd_code.cmd_type = (int)basic_m5stack_cmd_list::REQUEST_STATE;
        send_data_.cmd_code.data_size = 0;
        send_data_.cmd_code.is_sys_cmd = false;
        send_data_.cmd_code.is_used_msgpack = false;
        is_send_data_exist_ = true;
    }
}

void BasicM5stack::start_logging()
{
    if (state_->is_connected_device)
    {
        control_state_log.clear();

        is_logging_ = true;
    }
}

void BasicM5stack::stop_logging()
{
	is_logging_ = false;

    // ログを保存する
    std::ofstream ofs("control_state_log.csv");

    //ofs << "sensor_weight, sensor_weight_raw_adc" << std::endl;
    ofs << "timestamp, is_power_on, servo_id, ctrl_mode, act_joint_position, act_joint_velocity, act_joint_torque, cmd_joint_position, cmd_joint_velocity, cmd_joint_torque" << std::endl;
    for (size_t i = 0; i < control_state_log.size(); i++)
	{
		ofs << (control_state_log[i].timestamp - control_state_log[0].timestamp) * 0.000001 << ","
            << control_state_log[i].is_power_on << ","
            << control_state_log[i].servo_id << ","
            << (int)control_state_log[i].ctrl_mode << ","
            << control_state_log[i].act_joint_position << ","
            << control_state_log[i].act_joint_velocity << ","
            << control_state_log[i].act_joint_torque << ","
            << control_state_log[i].cmd_joint_position << ","
            << control_state_log[i].cmd_joint_velocity << ","
            << control_state_log[i].cmd_joint_torque << std::endl;
            //<< control_state_log[i].sensor_weight << "," 
            //<< control_state_log[i].sensor_weight_raw_adc << std::endl;
    }
	ofs.close();
}

void BasicM5stack::logging()
{
    if (is_logging_)
    {
        control_state_log.push_back(state_->m5_state.control);		
    }
}


BasicM5stack::BasicM5stack()
{
    change_node_running(false);
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    control_state_log.reserve(10000);
}

BasicM5stack::~BasicM5stack()
{
}

void BasicM5stack::_initialize_processing(){}
void BasicM5stack::_ready_processing()
{
    cmd_executor();
    _set_state();
    _update_state();

    if (!state_->is_connected_device)
	{
		connect();
	}

    //logging();
    //print_log("[encoder count] " + std::to_string(state_->encoder_count));
}
void BasicM5stack::_stable_processing()
{
    cmd_executor();
    _set_state();
    _update_state();
    //logging();
}
void BasicM5stack::_repair_processing(){}
void BasicM5stack::_force_stop_processing(){}
void BasicM5stack::_end_processing(){}

bool BasicM5stack::_any_to_initialize_processing(){
    node_state_->state_code.data_size = sizeof(st_basic_m5stack_state);
    return true;
}
bool BasicM5stack::_any_to_ready_processing(){return true;}
bool BasicM5stack::_any_to_force_stop_processing(){return true;}
bool BasicM5stack::_ready_to_repair_processing(){return true;}
bool BasicM5stack::_ready_to_stable_processing(){return true;}
bool BasicM5stack::_repair_to_stable_processing(){return true;}
bool BasicM5stack::_stable_to_repair_processing(){return true;}
bool BasicM5stack::_any_to_initialize_processing_after(){return true;}
bool BasicM5stack::_any_to_ready_processing_after(){return true;}
bool BasicM5stack::_any_to_force_stop_processing_after(){return true;}
bool BasicM5stack::_ready_to_repair_processing_after(){return true;}
bool BasicM5stack::_ready_to_stable_processing_after(){return true;}
bool BasicM5stack::_repair_to_stable_processing_after(){return true;}
bool BasicM5stack::_stable_to_repair_processing_after(){return true;}

void BasicM5stack::_set_config(nlohmann::json &json_data)
{
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
}

void BasicM5stack::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.sys_cmd_stack_size);
    state_ = (st_basic_m5stack_state*)node_state_->data;
}

void BasicM5stack::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
    state_->state_machine = node_state_machine_;
}

void BasicM5stack::cmd_executor()
{
    st_node_cmd cmd;
    st_basic_m5stack_cmd* m5stack_cmd;

    if (node_cmd_->cmd_stack_.size() != 0)
    {
        cmd = node_cmd_->cmd_stack_.pop();
        //print_log("[cmd] " + std::to_string(cmd.cmd_code.cmd_type));
        m5stack_cmd = (st_basic_m5stack_cmd*)cmd.data;
        switch ((basic_m5stack_cmd_list)cmd.cmd_code.cmd_type)
        {
        case basic_m5stack_cmd_list::REQUEST_STATE:
            connect();
			break;
        case basic_m5stack_cmd_list::START_LOGGING:
            print_log("START LOGGING");
            start_logging();
            break;
        case basic_m5stack_cmd_list::STOP_LOGGING:
            print_log("STOP LOGGING");
            stop_logging();
            break;
        default:
            break;
        }
    }



}

void BasicM5stack::_task_recv()
{
    //RecvData recv_data_;
    print_log("Recv Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_recv_periodic_time * 0.001)) + "ms");

    comm_m5stack_.set_recv_address(node_config_.recv_ip.c_str(), node_config_.recv_port);
    st_node_cmd temp_cmd;
    st_node_state temp_state;
    common_state_code temp_common_state_code;

    udp_frame recv_frame_;
    int recv_data_size = 0;
    int actual_marker_point = 0;
    while (check_node_running())
    {
        if (check_allowed_comm_udp())
        {
            recv_data_size = comm_m5stack_.recv_data(reinterpret_cast<uint8_t*>(&recv_frame_), sizeof(udp_frame));
            if (recv_data_size > 0 && recv_frame_.stack_marker_num <= recv_frame_.max_stack_marker_size)
			{
                if (state_->is_connected_device == false)
                {
					print_log("Device Connected");
                }
                state_->is_connected_device = true;

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
                        // State
                        memcpy_s(&temp_state,
								 sizeof(st_node_state),
								 &recv_frame_.data[actual_marker_point * recv_frame_.one_stack_size], 
								 static_cast<rsize_t>(marker_) * recv_frame_.one_stack_size);
                        switch (temp_state.state_code.node_id)
                        {
                            case 1:
                                state_->m5_state.system = *(st_m5_system_state*)temp_state.data;
    							break;

                            case 2:
                                state_->m5_state.control.deepcopy(*(st_m5_control_state*)temp_state.data);
                                //print_log("Control State: " + std::to_string(state_->m5_state.control.servo_id));
                                break;
                            case 3:
                                state_->m5_state.control.smalldeepcopy(*(st_m5_control_state_udp*)temp_state.data);
                                //print_log("Control State: " + std::to_string(state_->m5_state.control.servo_id));
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
        }
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_recv_periodic_time));
    }
    print_log("Recv Thread End");

}

