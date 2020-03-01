#ifndef PARSER_H
#define PARSER_H
#include "group.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Parser {
std::unique_ptr<Group> load(std::vector<std::string> configs);
}

#endif // PARSER_H
