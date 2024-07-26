#pragma once

#include <string>
#include <msgpack.hpp>

struct st_cmd_start_node
{
    std::string node_type;
    std::string setting_json_file_name;
    std::string setting_json_folder_name;
    st_cmd_start_node(/* args */)
    : node_type("")
    , setting_json_file_name("")
    , setting_json_folder_name("")
    {}
    st_cmd_start_node(std::string node_type_, std::string setting_json_file_name_, std::string setting_json_folder_name_)
    : node_type(node_type_)
    , setting_json_file_name(setting_json_file_name_)
    , setting_json_folder_name(setting_json_folder_name_)
    {}
    MSGPACK_DEFINE(node_type, setting_json_file_name, setting_json_folder_name);
};