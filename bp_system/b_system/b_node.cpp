#include "b_node.hpp"

void b_node::Start_Node(bool use_udp_communication, std::shared_ptr<node_cmd> hub_cmd)
{
    b_hub_cmd_ = hub_cmd;
    //Start(use_udp_communication);
    Start(use_udp_communication);
}

void b_node::_sys_cmd_executor()
{
    int user_state_machine_cnt = 0;
    if (node_sys_cmd_->cmd_stack_.size() != 0)
    {
        auto cmd = node_sys_cmd_->cmd_stack_.pop();
        print_log("[sys_cmd_executor]" + get_b_node_name((behavior_node_list)cmd.cmd_code.source));
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
            _start_node(this_node);    
            break;
        default:
            print_log("[sys_cmd_executor]Invalid cmd_type");
            break;
        }
    }
}

void b_node::_start_node(behavior_node_list source_node)
{
    print_log("[start_node]Load request nodes");
    st_node_cmd node_cmd;
    for (auto &&requirement : requirement_b_node_list)
    {
        print_log("[start_node]" + get_b_node_name((behavior_node_list)requirement));
        node_cmd.cmd_code.source = (int)source_node;
        node_cmd.cmd_code.destination = (int)behavior_node_list::HUB;
        node_cmd.cmd_code.priority = 0;
        node_cmd.cmd_code.cmd_id = 0;
        node_cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::START_NODE;
        node_cmd.cmd_code.data_size = sizeof(st_cmd_start_node);

        st_cmd_start_node* cmd_data; 
        cmd_data = (st_cmd_start_node*)node_cmd.data;
        cmd_data->node_type = requirement;

        b_hub_cmd_->cmd_stack_.push(node_cmd);
    }
    for (auto &&requirement : requirement_p_node_list)
    {
        print_log("[start_node]" + get_p_node_name((physics_node_list)requirement));
        node_cmd.cmd_code.source = (int)source_node;
        node_cmd.cmd_code.destination = (int)behavior_node_list::HUB;
        node_cmd.cmd_code.priority = 0;
        node_cmd.cmd_code.cmd_id = 0;
        node_cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::START_P_NODE;
        node_cmd.cmd_code.data_size = sizeof(st_cmd_start_node);

        st_cmd_start_node* cmd_data; 
        cmd_data = (st_cmd_start_node*)node_cmd.data;
        cmd_data->node_type = requirement;

        b_hub_cmd_->cmd_stack_.push(node_cmd);
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

void b_node::hub_cmd_set_shared_ptr(behavior_node_list source_node)
{
    st_node_cmd node_cmd;
    node_cmd.cmd_code.source = (int)source_node;
    node_cmd.cmd_code.destination = (int)behavior_node_list::HUB;
    node_cmd.cmd_code.priority = 0;
    node_cmd.cmd_code.cmd_id = 0;
    node_cmd.cmd_code.cmd_type = (int)b_hub_cmd_list::PICK_SHARED_PTR;
    node_cmd.cmd_code.data_size = 0;
    b_hub_cmd_->cmd_stack_.push(node_cmd);
}

void b_node::set_config(nlohmann::json json_data)
{
    print_log("set config");
    node_config_.cmd_stack_size = json_data.at("cmd_stack_size");
    node_config_.sys_cmd_stack_size = json_data.at("sys_cmd_stack_size");
    for (auto &&requirement : json_data.at("requirement"))
    {
        if (requirement.at("system") == node_system_list::BEHAVIOR)
        {
            requirement_b_node_list.push_back(requirement.at("type"));
            print_log("[requirement b node list]Node Name: " + get_b_node_name((behavior_node_list)requirement.at("type")) + " (" + std::to_string((int)requirement.at("type")) + ")");
        }
        else if (requirement.at("system") == node_system_list::PHYSICS)
        {
            requirement_p_node_list.push_back(requirement.at("type"));
            print_log("[requirement p node list]Node Name: " + get_p_node_name((physics_node_list)requirement.at("type")) + " (" + std::to_string((int)requirement.at("type")) + ")");
        }
    }
    _set_config(json_data);
}

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
            print_log("[ready_processing]<<WARNING>>Not Ready: " + get_b_node_name((behavior_node_list)node_state.first));
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
            print_log("[repair_processing]<<WARNING>>Not Repair: " + get_b_node_name((behavior_node_list)node_state.first));
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
            print_log("[stable_processing]<<WARNING>>Not Stable: " + get_b_node_name((behavior_node_list)node_state.first));
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
    if (now_time - transit_start_time_ > node_config_.transit_watch_dog_time)
    {
        transit_start_time_ = now_time;
        node_state_machine_ = prev_node_state_machine_;
        switch (prev_node_state_machine_)
        {
        case node_state_machine::READY:
            any_to_ready_processing();
            break;
        case node_state_machine::REPAIR:
            stable_to_repair_processing();
            break;
        case node_state_machine::STABLE:
            repair_to_stable_processing();
            break;
        case node_state_machine::FORCE_STOP:
            any_to_ready_processing();
            break;
        case node_state_machine::INITIALIZING:
            any_to_initialize_processing();
            break;
        default:
            break;
        }
        print_log("[transit_processing]<<WARNING>>Watch dog timer expired");
    }
    if (b_node_state_map.size() == requirement_b_node_list.size())
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
        }
        if (is_same_state_machine == true)
        {
            node_state_machine_ = transit_destination_node_state_machine_;
            print_log("[transit_processing]Done: " 
                    + get_node_state_machine_name(node_state_machine_) 
                    + " -> " + get_node_state_machine_name(transit_destination_node_state_machine_));
        }
    }
}

bool b_node::any_to_initialize_processing()
{
    hub_cmd_set_shared_ptr(this_node);
    return _any_to_initialize_processing();
}

bool b_node::any_to_ready_processing()
{
    st_node_cmd cmd;
    for (auto &&node_sys_cmd : b_node_sys_cmd_map)
    {
        if (node_sys_cmd.second != nullptr)
        {
            cmd.cmd_code.source = (int)this_node;
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
            cmd.cmd_code.source = (int)this_node;
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
            cmd.cmd_code.source = (int)this_node;
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
            cmd.cmd_code.source = (int)this_node;
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
            cmd.cmd_code.source = (int)this_node;
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
            cmd.cmd_code.source = (int)this_node;
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

void b_node::set_relative_b_node_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> cmd)
{
    b_node_cmd_map[(int)node_type] = cmd;
    print_log("[set_relative_b_node_cmd_ptr]" + get_b_node_name(node_type));
}

void b_node::set_relative_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state)
{
    b_node_state_map[(int)node_type] = state;
    print_log("[set_relative_b_node_state_ptr]" + get_b_node_name(node_type));
}

void b_node::set_relative_b_node_sys_cmd_ptr(behavior_node_list node_type, std::shared_ptr<node_cmd> sys_cmd)
{
    b_node_sys_cmd_map[(int)node_type] = sys_cmd;
    print_log("[set_relative_b_node_sys_cmd_ptr]" + get_b_node_name(node_type));
}

void b_node::set_user_b_node_state_ptr(behavior_node_list node_type, std::shared_ptr<st_node_state> state)
{
    user_b_node_state_map[(int)node_type] = state;
    print_log("[set_user_b_node_state_ptr]" + get_b_node_name(node_type));
    print_log("[set_user_b_node_state_ptr]Total User:" + std::to_string(user_b_node_state_map.size()));
}

void b_node::set_relative_p_node_state_ptr(physics_node_list node_type, std::shared_ptr<node_state> state)
{
    p_node_state_map[(int)node_type] = state;
    print_log("[set_relative_p_node_state_ptr]" + get_p_node_name(node_type));
}

void b_node::set_relative_p_node_cmd_ptr(physics_node_list node_type, std::shared_ptr<node_cmd> cmd)
{
    p_node_cmd_map[(int)node_type] = cmd;
    print_log("[set_relative_p_node_cmd_ptr]" + get_p_node_name(node_type));
}

void b_node::set_relative_p_node_sys_cmd_ptr(physics_node_list node_type, std::shared_ptr<node_cmd> sys_cmd)
{
    p_node_sys_cmd_map[(int)node_type] = sys_cmd;
    print_log("[set_relative_p_node_sys_cmd_ptr]" + get_p_node_name(node_type));
}

void b_node::set_user_p_node_state_ptr(physics_node_list node_type, std::shared_ptr<node_state> state)
{
    user_p_node_state_map[(int)node_type] = state;
    print_log("[set_user_p_node_state_ptr]" + get_p_node_name(node_type));
    print_log("[set_user_p_node_state_ptr]Total User:" + std::to_string(user_p_node_state_map.size()));
}