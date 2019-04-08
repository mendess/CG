#include "group.hpp"
#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
using namespace rapidxml;

unique_ptr<Translate> parse_translate(xml_node<char>* node);

unique_ptr<Transformation> parse_rotate(xml_node<char>* node);

unique_ptr<Scale> parse_scale(xml_node<char>* node);

Group::Group(xml_node<char>* group, float r, float g, float b, float a)
{
    _levels = 1;
    random_color = false;
    if (group->first_attribute()) { // if there are color attributes, override them
        this->r = this->g = this->b = 0.0f;
        this->a = 1.0f;
    } else {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    for (auto attr = group->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        if (name == "R") {
            this->r = stof(value);
        } else if (name == "G") {
            this->g = stof(value);
        } else if (name == "B") {
            this->b = stof(value);
        } else if ("A" == name) {
            this->a = stof(attr->value());
        } else if ("RAND" == name && value != "FALSE") {
            random_color = true;
        }
    }
    for (auto node = group->first_node(); node != NULL; node = node->next_sibling()) {
        string name = string(node->name());
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if ("models" == name) {
            for (auto model = node->first_node(); model != NULL; model = model->next_sibling()) {
                char* file = model->first_attribute()->value();
                Model m = Model(file);
                if (m.loaded()) {
                    models.push_back(m);
                } else {
                    cerr << "Couldn't load '" << file << "': No such file or directory" << endl;
                }
            }
        } else if ("group" == name) {
            subgroups.push_back(Group(node, this->r, this->g, this->b, this->a));
        } else if ("translate" == name) {
            transformations.push_back(parse_translate(node));
        } else if ("rotate" == name) {
            transformations.push_back(parse_rotate(node));
        } else if ("scale" == name) {
            transformations.push_back(parse_scale(node));
        }
    }
    int max_level = 0;
    for (const auto& subgroup : subgroups) {
        if (subgroup.levels() > max_level)
            max_level = subgroup.levels();
    }
    _levels = max_level + 1;
}

void Group::draw(int max_depth)
{
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    glPushMatrix();
    if (max_depth > 0) {
        for (const auto& transformation : transformations) {
            transformation->transform(elapsed);
        }
        for (size_t i = 0; i < models.size(); i++) {
            glColor4f(r, g, b, a);
            models[i].draw();
        }
        for (size_t i = 0; i < subgroups.size(); i++) {
            subgroups[i].draw(max_depth - 1);
        }
    }
    glPopMatrix();
}

unique_ptr<Translate> parse_translate(xml_node<char>* node)
{
    float x, y, z;
    x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        if ("X" == name || "AXISX" == name) {
            x = stof(attr->value());
        } else if ("Y" == name || "AXISY" == name) {
            y = stof(attr->value());
        } else if ("Z" == name || "AXISZ" == name) {
            z = stof(attr->value());
        }
    }
    return make_unique<Translate>(x, y, z);
}

unique_ptr<Transformation> parse_rotate(xml_node<char>* node)
{
    float angle, x, y, z, dur;
    angle = x = y = z = 0.0f;
    bool staticR = true;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        if ("ANGLE" == name) {
            angle = stof(attr->value());
            staticR = true;
        } else if ("X" == name || "AXISX" == name) {
            x = stof(attr->value());
        } else if ("Y" == name || "AXISY" == name) {
            y = stof(attr->value());
        } else if ("Z" == name || "AXISZ" == name) {
            z = stof(attr->value());
        } else if ("TIME" == name) {
            dur = stof(attr->value());
            staticR = false;
        }
    }
    if (staticR)
        return make_unique<RotateStatic>(angle, x, y, z);
    else
        return make_unique<RotateAnimated>(dur, x, y, z);
}

unique_ptr<Scale> parse_scale(xml_node<char>* node)
{
    float x, y, z;
    x = y = z = 1.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        if ("X" == name || "AXISX" == name) {
            x = stof(attr->value());
        } else if ("Y" == name || "AXISY" == name) {
            y = stof(attr->value());
        } else if ("Z" == name || "AXISZ" == name) {
            z = stof(attr->value());
        }
    }
    return make_unique<Scale>(x, y, z);
}
