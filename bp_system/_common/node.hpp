#pragma once

#include "json.hpp"

#include "../_common/comm/comm_udp.hpp" 

#include "node_state_machine.hpp"
#include "node_config.hpp"
#include "node_util.hpp"
#include "node_cmd.hpp"
#include "node_state.hpp"

#define CONFIG_FOLDER "../_config/"

class node
{
private:
    /* data */
    bool signal_to_release_force_stop;
    bool signal_to_execute_force_stop;
    //bool signal_to_quit_main_system;
    bool signal_to_change_node_state;
    bool signal_to_reset_node;
    bool signal_to_change_stable;
    bool signal_to_change_repair;
    bool signal_to_change_ready;

    bool use_udp_communication_;
    CommUdp comm_udp_;

    void node_loop_processing();
    void generate_signal_to_chenge_node_state();
    bool change_node_state(node_state_machine cmd_state_machine, bool use_transit_timer);
    bool is_changed_node_state;
protected:
    /* data */
    bool is_main_thread_running_;
    bool is_accepted_comm_udp_;
    bool is_occured_error_;
    bool is_occured_warning_;

    double node_cycle_time_;    // [s]

    /* My Item */
    std::shared_ptr<node_state> node_state_;
    std::shared_ptr<node_cmd> node_cmd_;
    std::shared_ptr<node_cmd> node_sys_cmd_;
    st_node_cmd send_data_;

    uint64_t transit_start_time_;
    node_state_machine node_state_machine_, prev_node_state_machine_;
    node_state_machine cmd_node_state_machine_, transit_destination_node_state_machine_;

    /* threads */
    std::string config_file_name_;
    void set_config_file_name(std::string config_file_name);
    node_config node_config_;
    virtual void _task_main();
    virtual void _task_recv();
    virtual void _task_send();

    void node_processing();

    /* node_state loop process */
    virtual void initialize_processing() = 0;
    virtual void ready_processing() = 0;
    virtual void repair_processing() = 0;
    virtual void stable_processing() = 0;
    virtual void force_stop_processing() = 0;
    virtual void transit_processing() = 0;
    // just once execute in End()
    virtual void end_processing() = 0;

    /* node state change process */
    // -> initialize
    virtual bool any_to_initialize_processing() = 0;
    // -> ready (reset process)
    virtual bool any_to_ready_processing() = 0; // stable, repair and ready
    // -> force stop
    virtual bool any_to_force_stop_processing() = 0; // stable, repair and ready
    // -> normal flow
    virtual bool ready_to_repair_processing() = 0;
    virtual bool ready_to_stable_processing() = 0;
    virtual bool repair_to_stable_processing() = 0;
    virtual bool stable_to_repair_processing() = 0;

    void print_log(std::string log_message);

    /* sub functions */
    void _preconfigure(std::string config_file);
    virtual void _configure() = 0;                          // 後で(継承先で)設定処理を記述する
    void _load_json_config(std::string config_file);
    virtual void set_config(nlohmann::json json_data) = 0; // 後で(継承先で)jsonファイルを読み込む処理を記述する
    virtual void _set_state() = 0;                          // 後で(継承先で)stateを設定する処理を記述する
    void _reset_internal_status();
public:
    node(/* args */);
    ~node();
    void Start(bool use_udp_communication);
    void End();
    void Reset();
    void PushForceStop();
    void ReleaseForceStop();
    void ChangeStable();
    void ChangeRepair();
    void ChangeReady();
};