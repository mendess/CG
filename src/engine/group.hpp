#ifndef __GROUP_HPP__
#define __GROUP_HPP__

#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <vector>

class Group {
private:
    std::vector<Transformation*> transformations; // unique_ptr
    std::vector<Model> models;
    std::vector<Group> subgroups;
    int _levels;
    float r, g, b, a;

    Group(rapidxml::xml_node<char>*, float, float, float, float);
    std::string internal_to_string(int) const;

public:
    Group(rapidxml::xml_node<char>* group)
        : Group(group, 0, 0, 0, 1)
    {
    }

    int levels() const { return _levels; };
    void draw(int) const;
    std::string to_string() const;
};

#endif // __GROUP_HPP__
