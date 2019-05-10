#ifndef UTIL_H
#define UTIL_H

#include "../dependencies/rapidxml.hpp"
#include <algorithm>
#include <unordered_map>

namespace util {

#define get_or_default(getting, default_val) (getting != "" ? stof(getting) : default_val)

#define get_component(comp, def) (                \
    RGB(get_or_default(params[comp "R"], def), \
        get_or_default(params[comp "G"], def), \
        get_or_default(params[comp "B"], def)))

std::unordered_map<std::string, std::string> params_to_map(rapidxml::xml_node<char>*);

}

#endif // UTIL_H
