#ifndef GROUP_HPP
#define GROUP_HPP

#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <vector>
#include <memory>

class Group {
private:
    std::vector<std::unique_ptr<Transformation>> transformations;
    std::vector<Model> models;
    std::vector<Group> subgroups;
    int _levels;
    float r, g, b, a;
    bool random_color;

    Group(rapidxml::xml_node<char>*, float, float, float, float);

public:
    Group(rapidxml::xml_node<char>* group)
        : Group(group, 0, 0, 0, 1)
    {
    }

    int levels() const { return _levels; };
    void draw(int);
};

#endif // GROUP_HPP
