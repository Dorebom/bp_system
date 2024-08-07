#include "node.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>

node::node(/* args */)
{
    is_main_thread_running_ = false;
    set_config_directory_name(CONFIG_FOLDER);
}

node::~node()
{
}

/* ↓-- PUBLIC OPERATION --↓ */
void node::Start()
{
    if (is_main_thread_running_ == false)
    {
        _preconfigure(config_file_name_);
        std::thread t_main(&node::_task_main, this);
        t_main.detach();
        while (is_main_thread_running_ == false)
        {
            print_log("Main Thread Executing...");
            std::this_thread::sleep_for(std::chrono::microseconds(100000));
        }
        if (use_udp_communication_){
            std::thread t_recv(&node::_task_recv, this);
            std::thread t_send(&node::_task_send, this);
            t_recv.detach();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            t_send.detach();
        }
    }
    else
    {
        print_log("Main Thread is already running");
    }
}

void node::End()
{
    end_processing();
    is_main_thread_running_ = false;
}

void node::Reset()
{
    signal_to_reset_node = true;
}

void node::PushForceStop()
{
    signal_to_execute_force_stop = true;
}

void node::ReleaseForceStop()
{
    signal_to_release_force_stop = true;
}

void node::ChangeStable()
{
    signal_to_change_stable = true;
}

void node::ChangeRepair()
{
    signal_to_change_repair = true;
}

void node::ChangeReady()
{
    signal_to_change_ready = true;
}

/* ↑-- END PUBLIC OPERATION --↑ */

/* ↓-- THREAD --↓ */

void node::_task_send()
{
    print_log("Send Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_send_periodic_time * 0.001)) + "ms");

    comm_udp_.set_send_address(node_config_.send_ip.c_str(), node_config_.send_port);

    while (is_main_thread_running_)
    {
        if (is_allowed_comm_udp_)
        {
            comm_udp_.send_data(reinterpret_cast<uint8_t*>(&send_data_), sizeof(st_node_cmd));
        }
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_send_periodic_time));
    }
    print_log("Send Thread End");
}

void node::_task_recv()
{
    //RecvData recv_data_;
    print_log("Recv Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_recv_periodic_time * 0.001)) + "ms");

    comm_udp_.set_recv_address(node_config_.recv_ip.c_str(), node_config_.recv_port);

    st_node_cmd recv_data_;
    int recv_data_size = 0;
    while (is_main_thread_running_)
    {
        if (is_allowed_comm_udp_)
        {
            recv_data_size = comm_udp_.recv_data(reinterpret_cast<uint8_t*>(&recv_data_), sizeof(st_node_cmd));
            if (recv_data_size > 0)
            {
                if (recv_data_.cmd_code.is_sys_cmd)
                {
                    node_sys_cmd_->cmd_stack_.push(recv_data_);
                }
                else
                {
                    node_cmd_->cmd_stack_.push(recv_data_);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_recv_periodic_time));
    }
    print_log("Recv Thread End");
}

void node::_task_main()
{
    print_log("Main Thread Start");
    print_log("Sleep time: " + std::to_string((int)(node_config_.task_main_periodic_time * 0.001)) + "ms");
    node_cycle_time_ = node_config_.task_main_periodic_time * 0.000001;

    is_main_thread_running_ = true;
    _reset_internal_status();
    _configure();
    is_allowed_comm_udp_ = true;

    while (is_main_thread_running_)
    {
        node_processing();
        std::this_thread::sleep_for(std::chrono::microseconds(node_config_.task_main_periodic_time));
    }
    print_log("Main Thread End");
    node_state_machine_ = node_state_machine::UNCONFIGURED;
}

/* ↑-- THREAD --↑ */

/* ↓-- FRAMEWORK --↓ */

void node::node_processing()
{
    // 1.0 内部状態を確認し、切り替えフラグを作成
    generate_signal_to_chenge_node_state();

    // 2.0 切り替え処理 か ループ処理を実行
    if (signal_to_change_node_state && node_state_machine_ != node_state_machine::TRANSITING)
    {   // 2.1. 切り替え処理を実行
        print_log("[change_processing]CHANGE NODE "
            + get_node_state_machine_name(node_state_machine_) + " -> "
            + get_node_state_machine_name(cmd_node_state_machine_));
        change_node_state(cmd_node_state_machine_, true);
        signal_to_change_node_state = false;
    }
    else
    {   // 2.2 切り替え処理が来てなかったら現在の状態に応じた処理を実行
        node_loop_processing();
    }
    _set_state();
}

void node::generate_signal_to_chenge_node_state()
{
    // この操作の中で signal_to_change_node_state を false にしてはならない
    if (signal_to_execute_force_stop)
    {
        signal_to_change_node_state = true;
        cmd_node_state_machine_ = node_state_machine::FORCE_STOP;
        signal_to_execute_force_stop = false;
    }
    else
    {
        switch (node_state_machine_)
        {
        case node_state_machine::STABLE:
            if (is_occured_error_)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::FORCE_STOP;
            }
            else if (is_occured_warning_)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::REPAIR;
            }
            else if (signal_to_reset_node)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::READY;
                signal_to_reset_node = false;
            }
            else if (signal_to_change_repair)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::REPAIR;
                signal_to_change_repair = false;
            }
            else if (signal_to_change_ready)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::READY;
                signal_to_change_ready = false;
            }
            break;
        case node_state_machine::REPAIR:
            if (is_occured_error_)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::FORCE_STOP;
            }
            else if (signal_to_reset_node)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::READY;
                signal_to_reset_node = false;
            }
            else if (signal_to_change_stable && is_occured_warning_ == false)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::STABLE;
                signal_to_change_stable = false;
            }
            else if (signal_to_change_ready)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::READY;
                signal_to_change_ready = false;
            }
            break;
        case node_state_machine::READY:
            if (signal_to_change_stable && is_occured_warning_ == false
                && is_occured_error_ == false)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::STABLE;
                signal_to_change_stable = false;
            }
            else if (signal_to_change_repair && is_occured_error_ == false)
            {
                signal_to_change_node_state = true;
                cmd_node_state_machine_ = node_state_machine::REPAIR;
                signal_to_change_repair = false;
            }
        default:
            break;
        }
    }
}


bool node::change_node_state(node_state_machine cmd_state_machine, bool use_transit_timer)
{
    bool is_success = false;
    if (cmd_state_machine == node_state_machine_)
    {
        is_success = true;
    }
    else
    {
        switch (cmd_state_machine)
        {
        case node_state_machine::INITIALIZING:
            if (node_state_machine_ != node_state_machine::FORCE_STOP)
            {
                is_success = any_to_initialize_processing();
            }
            break;
        case node_state_machine::READY:
            switch (node_state_machine_)
            {
            case node_state_machine::STABLE:
                is_success = any_to_ready_processing();
                break;
            case node_state_machine::REPAIR:
                is_success = any_to_ready_processing();
                break;
            case node_state_machine::FORCE_STOP:
                if (signal_to_release_force_stop)
                {
                    is_success = any_to_ready_processing();
                    signal_to_release_force_stop = false;
                    if (is_success == false)
                    {
                        print_log("<<ERROR>>Failed to change node state to READY");
                    }
                }
                else{
                    print_log("<<ERROR>>Failed to change node state to READY");
                }
                break;
            case node_state_machine::INITIALIZING:
                is_success = any_to_ready_processing();
                break;
            default:
                print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)READY)");
                break;
            }
            break;
        case node_state_machine::STABLE:
            switch (node_state_machine_)
            {
            case node_state_machine::REPAIR:
                if (is_occured_error_ == false)
                {
                    is_success = repair_to_stable_processing();
                }
                break;
            case node_state_machine::READY:
                if (is_occured_error_ == false && is_occured_warning_ == false)
                {
                    is_success = ready_to_stable_processing();
                }
                break;
            default:
                print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)STABLE)");
                break;
            }
            break;
        case node_state_machine::REPAIR:
            switch (node_state_machine_)
            {
            case node_state_machine::STABLE:
                is_success = stable_to_repair_processing();
                break;
            case node_state_machine::READY:
                if (is_occured_error_ == false)
                {
                    is_success = ready_to_repair_processing();
                }
            default:
                print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)REPAIR)");
                break;
            }
            break;
        case node_state_machine::FORCE_STOP:
            is_success = any_to_force_stop_processing();
            if (is_success == false)
            {
                print_log("<<ERROR>>Failed to change node state to FORCE_STOP");
                print_log("Quit main system");
                //signal_to_quit_main_system = true;
                End();
            }
            break;
        default:
            print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)defaults)");
            break;
        }

        if (is_success)
        {
            node_state_machine_ = cmd_node_state_machine_;
        }
        else if (use_transit_timer)
        {
            transit_start_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            //std::chrono::system_clock::now().time_since_epoch().count();
            prev_node_state_machine_ = node_state_machine_;
            transit_destination_node_state_machine_ = cmd_node_state_machine_;
            node_state_machine_ = node_state_machine::TRANSITING;
        }
    }
    return is_success;
}

void node::node_loop_processing()
{
    switch (node_state_machine_)
    {
    case node_state_machine::UNCONFIGURED:
        signal_to_change_node_state = true;
        cmd_node_state_machine_ = node_state_machine::INITIALIZING;
        break;
    case node_state_machine::INITIALIZING:
        initialize_processing();
        signal_to_change_node_state = true;
        cmd_node_state_machine_ = node_state_machine::READY;
        break;
    case node_state_machine::READY:
        ready_processing();
        break;
    case node_state_machine::STABLE:
        stable_processing();
        break;
    case node_state_machine::REPAIR:
        repair_processing();
        break;
    case node_state_machine::FORCE_STOP:
        force_stop_processing();
        break;
    case node_state_machine::TRANSITING:
        transit_processing();
        break;
    default:
        // Add error message
        break;
    }
}

/* ↑-- END FRAMEWORK --↑ */

void node::print_log(std::string log_message)
{
    if (node_config_.use_print_log)
    {
        std::cout << "[" << node_config_.node_name << "][" << get_node_state_machine_name(node_state_machine_) << "]" << log_message << std::endl;
    }
}

/* ↓-- SUB FUNCTION --↓ */
void node::_preconfigure(std::string config_file)
{
    _load_json_config(config_file);
}

void node::_load_json_config(std::string config_file)
{
    std::cout << std::filesystem::current_path() << std::endl;
    nlohmann::json j;
    std::ifstream i(config_directory_name_ + config_file);
    if (!i) {
        std::cerr << "[ERROR]Failed to open file: " << config_directory_name_ + config_file << std::endl;
        std::exit(1);
    }
    i >> j;

    node_config_.node_name = j["node_name"];
    node_config_.node_type = j["node_type"];
    // Threads
    node_config_.task_main_periodic_time = j["task_main_periodic_time"];
    // Communication
    use_udp_communication_ = (bool)j["use_udp_communication"];
    if (use_udp_communication_){
        node_config_.task_recv_periodic_time = j["task_recv_periodic_time"];
        node_config_.task_send_periodic_time = j["task_send_periodic_time"];
        node_config_.recv_port = j["recv_port"];
        node_config_.send_port = j["send_port"];
        node_config_.recv_ip = j["recv_ip"];
        node_config_.send_ip = j["send_ip"];
    }
    node_config_.transit_watch_dog_time = (double)j["transit_watch_dog_time"];
    node_config_.use_print_log = (bool)j["print_log"];

    set_config(j);

    j.clear();
    i.close();
}

void node::set_config_file_name(std::string config_file_name)
{
    config_file_name_ = config_file_name;
}

void node::set_config_directory_name(std::string config_directory)
{
    config_directory_name_ = config_directory;
}

void node::_reset_internal_status()
{
    transit_destination_node_state_machine_ = node_state_machine::READY;
    transit_start_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    is_occured_error_ = false;
    is_occured_warning_ = false;
    is_allowed_comm_udp_ = false;

    signal_to_release_force_stop = false;
    signal_to_execute_force_stop = false;
    //signal_to_quit_main_system = false;
    signal_to_change_node_state = false;
    signal_to_reset_node = false;
    signal_to_change_stable = false;
    signal_to_change_repair = false;
    signal_to_change_ready = false;
}

/* ↑-- END SUB FUNCTION --↑ */