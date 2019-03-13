#ifndef __GROUP_HPP__
#define __GROUP_HPP__

#include "model.hpp"
#include "transformations.hpp"
#include "../dependencies/rapidxml.hpp"
#include <vector>

class Group {
private:
    std::vector<Transformation *> transformations; // unique_ptr
    std::vector<Model> models;
    std::vector<Group> subgroups;
    int _levels;

    std::string internal_to_string(int) const;

public:
    Group(rapidxml::xml_node<char> *);
    int levels() const { return _levels; };
    void draw() const;
    void draw(int) const;
    std::string to_string() const;
};

#endif // __GROUP_HPP__
