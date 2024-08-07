#include "node_store.hpp"

void NodeStore::set_node(b_node& stored_node, std::string node_name)
{
    auto it = stored_node_list.find(node_name);
    if (it != stored_node_list.end()) {
        // Alart
        std::cout << "[behavior_hub][NodeStore]Node name is already used." << std::endl;
    }
    else{
        stored_node_list[node_name] = stored_node.Clone();
        std::cout << "[behavior_hub][NodeStore]" << node_name << " is set." << std::endl;
    }
}

std::shared_ptr<b_node> NodeStore::get_node(std::string node_name)
{
    auto it = stored_node_list.find(node_name);

    if (it != stored_node_list.end()) {
        return it->second->Clone();
    } else {
        std::cout << "[behavior_hub][NodeStore]Node type name is not found." << std::endl;
        return nullptr;
    }
}

void NodeStore::show_stored_node()
{
    std::cout << "[behavior_hub][NodeStore]Stored node type's list is follows:" << std::endl;

    for (const auto & pair : stored_node_list)
    {
        std::cout << pair.first << std::endl;
    }
}
