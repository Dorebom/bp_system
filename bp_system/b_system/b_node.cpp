#include "b_node.hpp"

void b_node::Start_Node(std::shared_ptr<node_cmd> hub_cmd)
{
    b_hub_cmd_ = hub_cmd;
    Start();
}

void b_node::_sys_cmd_executor()
{
    int user_state_machine_cnt = 0;
    if (node_sys_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = node_sys_cmd_->cmd_stack_.pop();
        switch ((b_node_sys_cmd_list)cmd.cmd_code.cmd_type)
        {
        // If it is not started, the command cannot be executed.
        //
        case b_node_sys_cmd_list::END:
            End();
            break;
        case b_node_sys_cmd_list::CHANGE_STABLE:
            switch (node_state_machine_)
            {
            case node_state_machine::READY:
                ChangeStable();
                break;
            case node_state_machine::REPAIR:
                for (auto &&user_node_state : user_b_node_state_map)
                {
                    if ((user_node_state.second->state_code.state_machine == node_state_machine::STABLE)
                        or (user_node_state.second->state_code.state_machine == node_state_machine::TRANSITING and
                            user_node_state.second->state_code.transit_destination_node_state == node_state_machine::STABLE)){}
                    else
                    {
                        user_state_machine_cnt++;
                    }
                }
                if (user_state_machine_cnt == 0)
                {
                    ChangeStable();
                }
            default:
                break;
            }
            break;
        case b_node_sys_cmd_list::CHANGE_REPAIR:
            ChangeRepair();
            break;
        case b_node_sys_cmd_list::CHANGE_READY:
            switch (node_state_machine_)
            {
            case node_state_machine::STABLE:
                for (auto &&user_node_state : user_b_node_state_map)
                {
                    if ((user_node_state.second->state_code.state_machine == node_state_machine::READY)
                        or (user_node_state.second->state_code.state_machine == node_state_machine::TRANSITING and
                            user_node_state.second->state_code.transit_destination_node_state == node_state_machine::READY)){}
                    else
                    {
                        user_state_machine_cnt++;
                    }
                }
                if (user_state_machine_cnt == 0)
                {
                    ChangeReady();
                }
                break;
            case node_state_machine::REPAIR:
                for (auto &&user_node_state : user_b_node_state_map)
                {
                    if ((user_node_state.second->state_code.state_machine == node_state_machine::READY)
                        or (user_node_state.second->state_code.state_machine == node_state_machine::TRANSITING and
                            user_node_state.second->state_code.transit_destination_node_state == node_state_machine::READY)){}
                    else
                    {
                        user_state_machine_cnt++;
                    }
                }
                if (user_state_machine_cnt == 0)
                {
                    ChangeReady();
                }
            default:
                break;
            }
            break;
        case b_node_sys_cmd_list::PUSH_FORCE_STOP:
            PushForceStop();
            break;
        case b_node_sys_cmd_list::RELEASE_FORCE_STOP:
            if (node_state_machine_ == node_state_machine::FORCE_STOP)
            {
                for (auto &&user_node_state : user_b_node_state_map)
                {
                    if (user_node_state.second->state_code.state_machine == node_state_machine::FORCE_STOP)
                    {
                        user_state_machine_cnt++;
                    }
                }
                if (user_state_machine_cnt == 0)
                {
                    ReleaseForceStop();
                }
            }
            break;
        case b_node_sys_cmd_list::RESET:
            Reset();
            break;
        case b_node_sys_cmd_list::SET_SHARED_PTR:
            print_log("[sys_cmd_executor]SET_SHARED_PTR");
            set_shared_ptr(cmd);
            break;
        case b_node_sys_cmd_list::REQUIRE_NODE_LIST:
            print_log("[sys_cmd_executor]REQUIRE_NODE_LIST");
            _start_node(node_id_);
            break;
        default:
            print_log("[sys_cmd_executor]Invalid cmd_type");
            break;
        }
    }
}

void b_node::_start_node(int source_node_id)
{
    int requirement_node_cnt = 0;
    st_node_cmd node_cmd;

    msgpack::sbuffer sbuf;

    st_cmd_start_node cmd_data;
    for (auto &&requirement : requirement_node_list)
    {
        print_log("[start_node][Send cmd to Hub][Type]" + requirement.node_type );
        print_log("[start_node][Send cmd to Hub][Json]" + requirement.node_jsonfile );
        print_log("[start_node][Send cmd to Hub][Folder]" + requirement.node_folder );

        cmd_data.node_type = requirement.node_type;
        cmd_data.setting_json_file_name = requirement.node_jsonfile;
        cmd_data.setting_json_folder_name = requirement.node_folder;

        msgpack::pack(sbuf, cmd_data);
        uint8_t* ptr = (uint8_t*)sbuf.data();
        size_t size = sbuf.size();

        print_log("[start_node][Send cmd to Hub][Data Size]" + std::to_string(size));
        std::memcpy(node_cmd.data, ptr, size);

        node_cmd.cmd_code.source = node_id_;
        node_cmd.cmd_code.destination = BEHAVIOR_HUB_ID;
        node_cmd.cmd_code.priority = 0;
        node_cmd.cmd_code.cmd_id = 0;
        node_cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::START_NODE;
        node_cmd.cmd_code.data_size = size;
        node_cmd.cmd_code.is_used_msgpack = true;

        b_hub_cmd_->cmd_stack_.push(node_cmd);

        sbuf.clear();
        requirement_node_cnt++;
    }
    if (requirement_node_cnt == 0)
    {
        print_log("[start_node]No requirement node");
    }
    else
    {
        print_log("[start_node]Requirement node count: " + std::to_string(requirement_node_cnt));
        if (requirement_node_cnt == requirement_node_list.size())
        {
            print_log("[start_node]All requirement node is sent");
        }
    }
}

void b_node::set_shared_ptr(st_node_cmd cmd)
{
    sys_cmd_set_shared_ptr* sys_cmd;
    sys_cmd = (sys_cmd_set_shared_ptr*)cmd.data;
    switch ((shared_ptr_type)sys_cmd->shared_ptr_type)
    {
    case shared_ptr_type::NODE_CMD:
        print_log("[set_shared_ptr]NODE_CMD");
        b_node_cmd_map[sys_cmd->node_type] = sys_cmd->node_cmd_ptr;
        break;
    case shared_ptr_type::NODE_STATE:
        print_log("[set_shared_ptr]NODE_STATE");
        b_node_state_map[sys_cmd->node_type] = sys_cmd->node_state_ptr;
        break;
    default:
        print_log("[set_shared_ptr]Invalid shared_ptr_type");
        break;
    }
}

void b_node::set_config(nlohmann::json json_data)
{
    print_log("set config");
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
    st_require_node_info requirement_node_info;
    for (auto &&requirement : json_data.at("requirement_node_list"))
    {
        requirement_node_info.node_type = requirement.at("node_type");
        requirement_node_info.node_jsonfile = requirement.at("setting_json_file_name");
        requirement_node_info.node_folder = requirement.at("setting_json_folder_name");
        requirement_node_list.push_back(requirement_node_info);
        print_log("[set requirement b node list]Node Type: " + requirement_node_info.node_type);
        print_log("[set requirement b node list]Node Json File: " + requirement_node_info.node_jsonfile);
        print_log("[set requirement b node list]Node Folder: " + requirement_node_info.node_folder);
    }
    _set_config(json_data);
}

common_cmd_code b_node::get_invoice_to_hub()
{
    common_cmd_code cmd_code;
    cmd_code.source = node_id_;
    cmd_code.destination = 0;
    cmd_code.priority = 0;
    cmd_code.cmd_id = 0;

    return cmd_code;
}

/*
void b_node::send_sys_cmd_to_node(
    b_hub_cmd_list cmd_type,
    std::uint8_t *cmd_data,
    behavior_node_list destination_node)
{
    st_node_cmd cmd;
    cmd.cmd_code.source = node_id_;
    cmd.cmd_code.destination = (int)destination_node;
    cmd.cmd_code.priority = 0;
    cmd.cmd_code.cmd_id = 0;
    cmd.cmd_code.cmd_type = (int)cmd_type;
    cmd.cmd_code.data_size = 0;
    //node_sys_cmd.second->cmd_stack_.push(cmd);
}
*/

void b_node::initialize_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
    _sys_cmd_executor();
    _initialize_processing();
}

void b_node::ready_processing()
{
    for (auto &&node_state : user_b_node_state_map)
    {
        if (node_state.second->state_code.state_machine != node_state_machine::READY)
        {
        }
    }
    node_state_->state_code.state_machine = node_state_machine_;
    _sys_cmd_executor();
    _ready_processing();
}

void b_node::repair_processing()
{
    bool is_usable_state_machine = true;
    for (auto &&node_state : user_b_node_state_map)
    {
        if (node_state.second->state_code.state_machine != node_state_machine::REPAIR or
            node_state.second->state_code.state_machine != node_state_machine::STABLE)
        {
            is_usable_state_machine = false;
        }
    }
    if (is_usable_state_machine == false)
    {
        ChangeReady();
    }
    node_state_->state_code.state_machine = node_state_machine_;
    _sys_cmd_executor();
    _repair_processing();
}

void b_node::stable_processing()
{
    bool is_same_state_machine = true;
    for (auto &&node_state : user_b_node_state_map)
    {
        if (node_state.second->state_code.state_machine != node_state_machine::STABLE)
        {
            is_same_state_machine = false;
        }
    }
    if (is_same_state_machine == false)
    {
        ChangeReady();
    }
    node_state_->state_code.state_machine = node_state_machine_;
    _sys_cmd_executor();
    _stable_processing();
}

void b_node::force_stop_processing()
{
    node_state_->state_code.state_machine = node_state_machine_;
    _sys_cmd_executor();
    _force_stop_processing();
}

void b_node::transit_processing()
{
    uint64_t now_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // Transit watch dog timer
    if (now_time - transit_start_time_ > node_config_.transit_watch_dog_time)
    {
        transit_start_time_ = now_time;
        node_state_machine_ = prev_node_state_machine_;

        st_node_cmd cmd;
        for (auto &&node_sys_cmd : b_node_sys_cmd_map)
        {
            if (node_sys_cmd.second != nullptr)
            {
                cmd.cmd_code.source = node_id_;
                cmd.cmd_code.destination = (int)node_sys_cmd.first;
                cmd.cmd_code.priority = 0;
                cmd.cmd_code.cmd_id = 0;
                switch (prev_node_state_machine_)
                {
                case node_state_machine::READY:
                    cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_READY;
                    break;
                case node_state_machine::REPAIR:
                    cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_REPAIR;
                    break;
                case node_state_machine::STABLE:
                    cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_STABLE;
                    break;
                default:
                    break;
                }
                cmd.cmd_code.data_size = 0;
                node_sys_cmd.second->cmd_stack_.push(cmd);
            }
        }

        print_log("[transit_processing]<<WARNING>>Watch dog timer expired");
    }
    switch (prev_node_state_machine_)
    {
    case node_state_machine::INITIALIZING:
        initialize_processing();
        break;
    case node_state_machine::READY:
        ready_processing();
        break;
    case node_state_machine::REPAIR:
        repair_processing();
        break;
    case node_state_machine::STABLE:
        stable_processing();
        break;
    case node_state_machine::FORCE_STOP:
        force_stop_processing();
        break;
    default:
        break;
    }

    // Transit destination node state machine check
    if (b_node_state_map.size() == requirement_node_list.size())
    {
        bool is_same_state_machine = true;
        for (auto &&node_state : b_node_state_map)
        {
            if (node_state.second != nullptr)
            {
                if (node_state.second->state_code.state_machine != transit_destination_node_state_machine_)
                {
                    is_same_state_machine = false;
                }
            }
            else
            {
                is_same_state_machine = false;
            }
        }
        // Transit destination node state machine is same
        if (is_same_state_machine == true)
        {
            _change_node_state(prev_node_state_machine_, transit_destination_node_state_machine_);
            node_state_machine_ = transit_destination_node_state_machine_;
            print_log("[transit_processing]Done: "
                    + get_node_state_machine_name(node_state_machine_)
                    + " -> " + get_node_state_machine_name(transit_destination_node_state_machine_));
        }
    }
}

void b_node::end_processing()
{
    st_node_cmd node_cmd;

    node_cmd.cmd_code.source = node_id_;
    node_cmd.cmd_code.destination = BEHAVIOR_HUB_ID;
    node_cmd.cmd_code.priority = 0;
    node_cmd.cmd_code.cmd_id = 0;
    node_cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::DELETE_SHARED_PTR;
    node_cmd.cmd_code.data_size = 0;
    b_hub_cmd_->cmd_stack_.push(node_cmd);
}

bool b_node::any_to_initialize_processing()
{
    st_node_cmd cmd;

    cmd.cmd_code.source = node_id_;
    cmd.cmd_code.destination = BEHAVIOR_HUB_ID;
    cmd.cmd_code.priority = 0;
    cmd.cmd_code.cmd_id = 0;
    cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::PICK_SHARED_PTR;
    cmd.cmd_code.data_size = 0;
    b_hub_cmd_->cmd_stack_.push(cmd);
    return _any_to_initialize_processing();
}

bool b_node::any_to_ready_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_READY;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _any_to_ready_processing();
}

bool b_node::any_to_force_stop_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::PUSH_FORCE_STOP;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _any_to_force_stop_processing();
}

bool b_node::ready_to_stable_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_STABLE;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _ready_to_stable_processing();
}

bool b_node::ready_to_repair_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_REPAIR;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _ready_to_repair_processing();
}

bool b_node::stable_to_repair_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_REPAIR;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _stable_to_repair_processing();
}

bool b_node::repair_to_stable_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = node_id_;
            cmd.cmd_code.destination = (int)node_sys_cmd.first;
            cmd.cmd_code.priority = 0;
            cmd.cmd_code.cmd_id = 0;
            cmd.cmd_code.cmd_type = (int)b_node_sys_cmd_list::CHANGE_STABLE;
            cmd.cmd_code.data_size = 0;
            node_sys_cmd.second->cmd_stack_.push(cmd);
        }
    }
    return _repair_to_stable_processing();
}
/*
void b_node::set_user_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state)
{
    user_b_node_state_map[(int)node_type] = state;
    print_log("[set_user_b_node_state_ptr]Total User:" + std::to_string(user_b_node_state_map.size()));
}
*/

void b_node::set_relative_node_name_and_id(
                    std::string json_file_name_of_requirement_node,
                    int node_id,
                    std::string node_name)
{
    print_log("[set_relative_node_name_and_id][Json file]" + json_file_name_of_requirement_node);
    print_log("[set_relative_node_name_and_id][Name]" + node_name);
    print_log("[set_relative_node_name_and_id][ID]" + std::to_string(node_id));
    rel_node_jsonfile2id_map[json_file_name_of_requirement_node] = node_id;
    rel_node_jsonfile2name_map[json_file_name_of_requirement_node] = node_name;
    rel_node_id2name_map[node_id] = node_name;
    rel_node_name2id_map[node_name] = node_id;
}
void b_node::set_relative_node_cmd_ptr(int node_id, std::shared_ptr<node_cmd> cmd)
{
    b_node_cmd_map[node_id] = cmd;
    print_log("[set_relative_node_cmd_ptr]" + rel_node_id2name_map[node_id]);
}
void b_node::set_relative_node_state_ptr(int node_id, std::shared_ptr<node_state> state)
{
    b_node_state_map[node_id] = state;
    print_log("[set_relative_node_state_ptr]" + rel_node_id2name_map[node_id]);
}
void b_node::set_relative_node_sys_cmd_ptr(int node_id, std::shared_ptr<node_cmd> sys_cmd)
{
    b_node_sys_cmd_map[node_id] = sys_cmd;
    print_log("[set_relative_node_sys_cmd_ptr]" + rel_node_id2name_map[node_id]);
}

void b_node::set_node_id(int node_id)
{
    node_id_ = node_id;
}

void b_node::_change_node_state(node_state_machine prev_, node_state_machine transit_destination_)
{
    bool is_success = true;
    switch (transit_destination_)
    {
    case node_state_machine::INITIALIZING:
        if (prev_ != node_state_machine::FORCE_STOP)
        {
            is_success = _any_to_initialize_processing_after();
        }
        break;
    case node_state_machine::READY:
        is_success = _any_to_ready_processing_after();
        break;
    case node_state_machine::STABLE:
        switch (prev_)
        {
        case node_state_machine::REPAIR:
            is_success = _repair_to_stable_processing_after();
            break;
        case node_state_machine::READY:
            is_success = _ready_to_stable_processing_after();
            break;
        default:
            print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)STABLE)");
            break;
        }
        break;
    case node_state_machine::REPAIR:
        switch (prev_)
        {
        case node_state_machine::STABLE:
            is_success = _stable_to_repair_processing_after();
            break;
        case node_state_machine::READY:
            is_success = _ready_to_repair_processing_after();
        default:
            print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)REPAIR)");
            break;
        }
        break;
    case node_state_machine::FORCE_STOP:
        is_success = _any_to_force_stop_processing_after();
        if (is_success == false)
        {
            print_log("<<ERROR>>Failed to change node state to FORCE_STOP");
            print_log("Quit main system");
            End();
        }
        break;
    default:
        print_log("<<WARNING>>Invalid node state(change_node_state:(cmd)defaults)");
        break;
    }
}
