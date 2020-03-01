#include "common/util.hpp"

namespace util {

std::unordered_map<std::string, std::string> params_to_map(rapidxml::xml_node<char>* node)
{
    std::unordered_map<std::string, std::string> params;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        std::string name = attr->name();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        params[name] = attr->value();
    }
    return params;
}

}

