#ifndef UTIL_H
#define UTIL_H

#include "../dependencies/rapidxml.hpp"
#include <algorithm>
#include <unordered_map>

namespace util {

#define get_or_default(getting, default_val) (getting != "" ? stof(getting) : default_val)

#define get_component(comp) (                \
    RGB(get_or_default(params[comp "R"], 1), \
        get_or_default(params[comp "G"], 1), \
        get_or_default(params[comp "B"], 1)))

std::unordered_map<std::string, std::string> params_to_map(rapidxml::xml_node<char>*);

}

#endif // UTIL_H
