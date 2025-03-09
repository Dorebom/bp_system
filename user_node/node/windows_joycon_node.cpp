#include "windows_joycon_node.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>

// Windows とLinuxでヘッダファイルが異なる
/*
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <Xinput.h>
#include <dinput.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#endif // 
*/

#define JOY_DEV "/dev/input/js0"

using namespace std;

BasicJoyconNode::BasicJoyconNode(/* args */)
: joy_fd(-1),
  num_of_axis(0),
  num_of_buttons(0)
{
    is_main_thread_running_ = false;
    node_state_machine_ = node_state_machine::UNCONFIGURED;
    cmd_node_state_machine_ = node_state_machine::UNCONFIGURED;

    JoyInfoEx.dwSize = sizeof(JOYINFOEX);
    JoyInfoEx.dwFlags = JOY_RETURNALL;

    is_get_rel_shared_ptr = false;
}

BasicJoyconNode::~BasicJoyconNode()
{
}


void BasicJoyconNode::_initialize_processing()
{
}
void BasicJoyconNode::_ready_processing()
{
    m5mf_api_state_ = (M5MF::M5mfApiState*)b_node_state_map[m5mf_api_id]->data;
    get_state();
    //display_state();
    if (m5mf_api_state_->is_connected_device)
	{
        ChangeStable();
	}
}
void BasicJoyconNode::_repair_processing()
{
    get_state();
    display_state();
    if (m5mf_api_state_->is_connected_device)
    {
        ChangeStable();
    }
}
void BasicJoyconNode::_stable_processing()
{
    get_state();
    //display_state();

    // >> send cmd data to m5mf_node


    // << send cmd data to m5mf_node
}
void BasicJoyconNode::_force_stop_processing()
{
    get_state();
    //display_state();
}

void BasicJoyconNode::_end_processing()
{
    /*
    if(joy_fd!=-1)
    {
        close(joy_fd);
    }
    */
}

/* node_state change process */
// -> initialize
bool BasicJoyconNode::_any_to_initialize_processing() {


    /*
    if((joy_fd=open(JOY_DEV,O_RDONLY)) < 0)
    {
        cerr<<"Failed to open "<<JOY_DEV<<endl;
        exit(-1);
    }

    ioctl(joy_fd, JSIOCGAXES, &num_of_axis);
    ioctl(joy_fd, JSIOCGBUTTONS, &num_of_buttons);
    ioctl(joy_fd, JSIOCGNAME(80), &name_of_joystick);

    joy_button.resize(num_of_buttons,0);
    joy_axis.resize(num_of_axis,0);

    print_log("Joystick: " + std::string(name_of_joystick));

    fcntl(joy_fd, F_SETFL, O_NONBLOCK);   // using non-blocking mode
    */
    return true;
}
// -> ready (reset process)
bool BasicJoyconNode::_any_to_ready_processing() {return false;} // stable, repair and ready
// -> force stop
bool BasicJoyconNode::_any_to_force_stop_processing() {return true;} // stable, repair and ready
// -> normal flow
bool BasicJoyconNode::_ready_to_repair_processing() {return true;}
bool BasicJoyconNode::_ready_to_stable_processing() {return true;}
bool BasicJoyconNode::_repair_to_stable_processing() {return true;}
bool BasicJoyconNode::_stable_to_repair_processing() {return true;}

bool BasicJoyconNode::_any_to_initialize_processing_after(){return true;}
bool BasicJoyconNode::_any_to_ready_processing_after()
{
    if (!is_get_rel_shared_ptr)
    {
        m5mf_api_id = rel_node_jsonfile2id_map.find("m5mf_api.json")->second;

        is_get_rel_shared_ptr = true;
    }
    return true;
}
bool BasicJoyconNode::_any_to_force_stop_processing_after(){return true;}
bool BasicJoyconNode::_ready_to_repair_processing_after(){return true;}
bool BasicJoyconNode::_ready_to_stable_processing_after(){return true;}
bool BasicJoyconNode::_repair_to_stable_processing_after(){return true;}
bool BasicJoyconNode::_stable_to_repair_processing_after(){return true;}

void BasicJoyconNode::_set_config(nlohmann::json &json_data)
{
}

void BasicJoyconNode::_configure()
{
    node_state_ = std::make_shared<st_node_state>();
    node_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    node_sys_cmd_ = std::make_shared<node_cmd>(node_config_.cmd_stack_size);
    print_log("BasicJoyconNode Configure");
}

void BasicJoyconNode::_set_state()
{
    node_state_->state_code.state_machine = node_state_machine_;
}

void BasicJoyconNode::get_state()
{
    joycon_state_ = (st_joycon_state*)node_state_->data;

    if (JOYERR_NOERROR == joyGetPosEx(0, &JoyInfoEx)) { //0番のジョイスティックの情報を見る
        joycon_state_->axis[0] =  (double)JoyInfoEx.dwXpos / 32767.0 - 1.0;
        joycon_state_->axis[1] = -(double)JoyInfoEx.dwYpos / 32767.0 + 1.0;
        joycon_state_->axis[2] =  (double)JoyInfoEx.dwRpos / 32767.0 - 1.0;
        joycon_state_->axis[3] = -(double)JoyInfoEx.dwZpos / 32767.0 + 1.0;
    }

    for (int i = 0; i < 12; i++)
	{
		joycon_state_->button[i] = (int)JoyInfoEx.dwButtons & (1 << i);
        if (joycon_state_->button[i] != 0)
		{
			joycon_state_->button[i] = 1;
		}
	}

    // >> send cmd data to m5mf_node

    // >> 1 button
    if (!prev_joycon_state_.button[0] && joycon_state_->button[0]) {
        if (!is_power_on) {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_POWER_ON;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_power_on = true;
        }
        else {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_POWER_OFF;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_power_on = false;
        }
    }

    // >> 2 button
    if ( !prev_joycon_state_.button[1] && joycon_state_->button[1] ) {
        if (!is_force_stop)
        {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CHANGE_SM_FORCE_STOP;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_force_stop = true;
        }
        else {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::RELEASE_FORCE_STOP;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_force_stop = false;
        }
	}

	// >> 3 button / and 4 button
	if (!prev_joycon_state_.button[2] && joycon_state_->button[2]) {
		flag_c = true;
	}
	else if (prev_joycon_state_.button[2] && !joycon_state_->button[2]) {
		flag_c = false;
	}
	if (!prev_joycon_state_.button[3] && joycon_state_->button[3]) {
		flag_d = true;
	}
	else if (prev_joycon_state_.button[3] && !joycon_state_->button[3]) {
		flag_d = false;
	}
	if (flag_c && flag_d) {
		is_set_torque_limit_released = true;
	}
	else if (!flag_c && !flag_d) {
		is_set_torque_limit_released = false;
	}

    // >> 5 and 6 button
    if (!prev_joycon_state_.button[4] && joycon_state_->button[4]) {
        flag_a = true;
    }
    else if(prev_joycon_state_.button[4] && !joycon_state_->button[4]){
        flag_a = false;
    }
    if (!prev_joycon_state_.button[5] && joycon_state_->button[5]) {
        flag_b = true;
    }
    else if (prev_joycon_state_.button[5] && !joycon_state_->button[5]) {
        flag_b = false;
    }
    if (flag_a && flag_b) {
        is_set_joint_zero = true;
    }else if (!flag_a && !flag_b){ 
        is_set_joint_zero = false;
    }
    if (is_set_joint_zero && !prev_is_set_joint_zero)
    {
        st_node_cmd cmd;
        cmd.cmd_code.source = node_id_;
        cmd.cmd_code.destination = m5mf_api_id;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_SET_MECHANICAL_POSITION_TO_ZERO;
        cmd.cmd_code.data_size = 0;
        cmd.cmd_code.is_sys_cmd = false;
        push_cmd_to_m5mf_node(&cmd);
    }
    prev_is_set_joint_zero = is_set_joint_zero;

    // >> 7 button
    if (!prev_joycon_state_.button[6] && joycon_state_->button[6]) {
        st_node_cmd cmd;

        /*
        switch (mf_type)
        {
        case M5MF::MECHANICAL_FRAME_LIST::ALLJOINT:
            mf_type = M5MF::MECHANICAL_FRAME_LIST::SCARA;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_MF2;
            break;
        case M5MF::MECHANICAL_FRAME_LIST::SCARA:
            mf_type = M5MF::MECHANICAL_FRAME_LIST::ALLJOINT;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_MF1;
            break;
        default:
            break;
        }
        */
        cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_MF_UP;
        cmd.cmd_code.source = node_id_;
        cmd.cmd_code.destination = m5mf_api_id;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.data_size = 0;
        cmd.cmd_code.is_sys_cmd = false;
        push_cmd_to_m5mf_node(&cmd);
    }

    // >> 8 button
    if (!prev_joycon_state_.button[7] && joycon_state_->button[7]) {
        st_node_cmd cmd;
        /*
        switch (ctrl_mode_)
        {
            case M5MF::CTRL_MODE_LIST::STAY:
                ctrl_mode_ = M5MF::CTRL_MODE_LIST::POSITION;
                cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_POSITION_CONTROL;
                break;
            case M5MF::CTRL_MODE_LIST::POSITION:
                ctrl_mode_ = M5MF::CTRL_MODE_LIST::VELOCITY;
                cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_VELOCITY_CONTROL;
                break;
            case M5MF::CTRL_MODE_LIST::VELOCITY:
                ctrl_mode_ = M5MF::CTRL_MODE_LIST::TORQUE;
                cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_TORQUE_CONTROL;
                break;
            case M5MF::CTRL_MODE_LIST::TORQUE:
                ctrl_mode_ = M5MF::CTRL_MODE_LIST::POSITION;
                cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_POSITION_CONTROL;
                break;
            default:
                break;
        }
        */
        cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_CHANGE_CONTROL_MODE_UP;
        cmd.cmd_code.source = node_id_;
        cmd.cmd_code.destination = m5mf_api_id;
        cmd.cmd_code.priority = 0;
        cmd.cmd_code.cmd_id = 0;
        cmd.cmd_code.data_size = 0;
        cmd.cmd_code.is_sys_cmd = false;
        push_cmd_to_m5mf_node(&cmd);
	}

    // >> 11 button
    if ( !prev_joycon_state_.button[10] && joycon_state_->button[10] ) {
        if (!is_start_stream)
        {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::START_STREAM_STATE;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_start_stream = true;
        }
        else {
            st_node_cmd cmd;
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = m5mf_api_id;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::STOP_STREAM_STATE;
            cmd.cmd_code.data_size = 0;
            cmd.cmd_code.is_sys_cmd = false;
            push_cmd_to_m5mf_node(&cmd);
            is_start_stream = false;

        }
     }
    // >> 12 button
    if ( !prev_joycon_state_.button[11] && joycon_state_->button[11] ) {
		if (!is_start_logging)
		{
			st_node_cmd cmd;
			cmd.cmd_code.source = node_id_;
			cmd.cmd_code.destination = m5mf_api_id;
			cmd.cmd_code.priority = 0;
			cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::START_LOGGING;
			cmd.cmd_code.data_size = 0;
			cmd.cmd_code.is_sys_cmd = false;
			push_cmd_to_m5mf_node(&cmd);
            is_start_logging = true;
		}
		else {
			st_node_cmd cmd;
			cmd.cmd_code.source = node_id_;
			cmd.cmd_code.destination = m5mf_api_id;
			cmd.cmd_code.priority = 0;
			cmd.cmd_code.cmd_id = 0;
			cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::STOP_LOGGING;
			cmd.cmd_code.data_size = 0;
			cmd.cmd_code.is_sys_cmd = false;
			push_cmd_to_m5mf_node(&cmd);
            is_start_logging = false;

		}
	 }

    // >> Axis
    if ( std::abs(joycon_state_->axis[0]) > 0.1 || 
         std::abs(joycon_state_->axis[1]) > 0.1 ||
         std::abs(joycon_state_->axis[2]) > 0.1 ||
         std::abs(joycon_state_->axis[3]) > 0.1 ) {
		st_node_cmd cmd;
		cmd.cmd_code.source = node_id_;
		cmd.cmd_code.destination = m5mf_api_id;
		cmd.cmd_code.priority = 0;
		cmd.cmd_code.cmd_id = 0;
		cmd.cmd_code.cmd_type = (int)M5MF::M5MF_CMD_LIST::CS_MOVE;
		cmd.cmd_code.data_size = 1;
		cmd.cmd_code.is_sys_cmd = false;
        JoyconMoveCmd* move_cmd = (JoyconMoveCmd*)cmd.data;
        for (int i = 0; i < 4; i++)
		{
            if (std::abs(joycon_state_->axis[i]) < 0.1)
			{
				joycon_state_->axis[i] = 0.0;
			}
		}
        move_cmd->x = joycon_state_->axis[0];
        move_cmd->y = joycon_state_->axis[1];
        move_cmd->z = joycon_state_->axis[2];
        move_cmd->roll = joycon_state_->axis[3];
        if (is_set_torque_limit_released && !prev_is_set_torque_limit_released)
        {
			move_cmd->is_torque_limit_released = true;
		}
		else
		{
			move_cmd->is_torque_limit_released = false;
        }
		if (joycon_state_->button[2] && !joycon_state_->button[3])
		{
			move_cmd->is_adjustment = true;
		}
		else
		{
			move_cmd->is_adjustment = false;
		}
		push_cmd_to_m5mf_node(&cmd);
        //print_log("Move Command: " + std::to_string(move_cmd->x) + " " 
        //                           + std::to_string(move_cmd->y) + " " 
        //                           + std::to_string(move_cmd->z) + " " 
        //                           + std::to_string(move_cmd->roll));
	}

    prev_joycon_state_.deepcopy(*joycon_state_);
}

void BasicJoyconNode::display_state()
{
    print_log("Button Value: "
                + std::to_string(joycon_state_->button[0]) + " "
                + std::to_string(joycon_state_->button[1]) + " "
                + std::to_string(joycon_state_->button[2]) + " "
                + std::to_string(joycon_state_->button[3]) + " "
                + std::to_string(joycon_state_->button[4]) + " "
                + std::to_string(joycon_state_->button[5]) + " "
                + std::to_string(joycon_state_->button[6]) + " "
                + std::to_string(joycon_state_->button[7]) + " "
                + std::to_string(joycon_state_->button[8]) + " "
                + std::to_string(joycon_state_->button[9]) + " "
                + std::to_string(joycon_state_->button[10]) + " "
                + std::to_string(joycon_state_->button[11]));
    print_log("Axis Value: "
                + std::to_string(joycon_state_->axis[0]) + " "
                + std::to_string(joycon_state_->axis[1]) + " "
                + std::to_string(joycon_state_->axis[2]) + " "
                + std::to_string(joycon_state_->axis[3]));

}

void BasicJoyconNode::push_cmd_to_m5mf_node(st_node_cmd* cmd)
{
    if (is_get_rel_shared_ptr) {
        b_node_cmd_map[m5mf_api_id]->cmd_stack_.push(*cmd);
    }
}
