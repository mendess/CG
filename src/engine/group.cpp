#include "group.hpp"
#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
using namespace rapidxml;

Translate* parse_translate(xml_node<char>* node);

Rotate* parse_rotate(xml_node<char>* node);

Scale* parse_scale(xml_node<char>* node);

Color* parse_color(xml_node<char>* node);

Group::Group(xml_node<char>* group)
{
    for (auto node = group->first_node(); node != NULL; node = node->next_sibling()) {
        string name = string(node->name());
        cerr << "loading: " << name << endl;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if ("models" == name) {
            for (auto model = node->first_node(); model != NULL; model = model->next_sibling()) {
                cerr << "\tloading model: " << model->first_attribute()->value() << endl;
                models.push_back(Model(model->first_attribute()->value()));
            }
        } else if ("group" == name) {
            subgroups.push_back(Group(node));
        } else if ("translate" == name) {
            transformations.push_back(parse_translate(node));
        } else if ("rotate" == name) {
            transformations.push_back(parse_rotate(node));
        } else if ("scale" == name) {
            transformations.push_back(parse_scale(node));
        } else if ("color" == name) {
            transformations.push_back(parse_color(node));
        }
    }
}

void Group::draw() const
{
    glPushMatrix();
    for (const auto& transformation : transformations) {
        transformation->transform();
    }
    for (const auto& model : models) {
        if (!model.draw()) {
            cerr << "model: " << model.name() << " failed to draw" << endl;
        }
    }
    for (const auto& subgroup : subgroups) {
        subgroup.draw();
    }
    glPopMatrix();
}

void Group::draw(int max_depth) const
{
    glPushMatrix();
    if (max_depth > 0) {
        for (const auto& transformation : transformations) {
            transformation->transform();
        }
        for (const auto& model : models) {
            model.draw();
        }
        for (const auto subgroup : subgroups) {
            subgroup.draw(max_depth - 1);
        }
    }
    glPopMatrix();
}

Translate* parse_translate(xml_node<char>* node)
{
    float x, y, z;
    x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string value = string(attr->value());
        if ("X" == value) {
            x = stof(value);
        } else if ("Y" == value) {
            y = stof(value);
        } else if ("Z" == value) {
            z = stof(value);
        }
    }
    return new Translate(x, y, z);
}

Rotate* parse_rotate(xml_node<char>* node)
{
    float angle, x, y, z;
    angle = x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string value = string(attr->value());
        if ("Angle" == value) {
            angle = stof(value);
        } else if ("X" == value) {
            x = stof(value);
        } else if ("Y" == value) {
            y = stof(value);
        } else if ("Z" == value) {
            z = stof(value);
        }
    }
    return new Rotate(angle, x, y, z);
}

Scale* parse_scale(xml_node<char>* node)
{
    float x, y, z;
    x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string value = string(attr->value());
        if ("X" == value) {
            x = stof(value);
        } else if ("Y" == value) {
            y = stof(value);
        } else if ("Z" == value) {
            z = stof(value);
        }
    }
    return new Scale(x, y, z);
}

Color* parse_color(xml_node<char>* node)
{
    float r, g, b;
    r = g = b = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string value = string(attr->value());
        if ("R" == value) {
            r = stof(value);
        } else if ("G" == value) {
            g = stof(value);
        } else if ("B" == value) {
            b = stof(value);
        }
    }
    return new Color(r, g, b);
}
