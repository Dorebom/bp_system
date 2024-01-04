#pragma once

#include <string>
#include <map>

#include "b_node.hpp"

class NodeStore
{
private:
    std::map<std::string, std::shared_ptr<b_node>> stored_node_list;
public:
    void set_node(b_node& stored_node, std::string node_name);
    std::shared_ptr<b_node> get_node(std::string node_name);
    void show_stored_node();
};