#include "group.hpp"
#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
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
    _levels = 1;
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
                    cerr << "\t" << file << ": No such file or directory" << endl;
                }
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
    int max_level = 0;
    for (const auto& subgroup : subgroups) {
        if (subgroup.levels() > max_level)
            max_level = subgroup.levels();
    }
    _levels = max_level + 1;
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
        string name = string(attr->name());
        if ("X" == name) {
            x = stof(attr->value());
        } else if ("Y" == name) {
            y = stof(attr->value());
        } else if ("Z" == name) {
            z = stof(attr->value());
        }
    }
    return new Translate(x, y, z);
}

Rotate* parse_rotate(xml_node<char>* node)
{
    float angle, x, y, z;
    angle = x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        if ("angle" == name) {
            angle = stof(attr->value());
        } else if ("axisX" == name) {
            x = stof(attr->value());
        } else if ("axisY" == name) {
            y = stof(attr->value());
        } else if ("axisZ" == name) {
            z = stof(attr->value());
        }
    }
    return new Rotate(angle, x, y, z);
}

Scale* parse_scale(xml_node<char>* node)
{
    float x, y, z;
    x = y = z = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        if ("X" == name) {
            x = stof(attr->value());
        } else if ("Y" == name) {
            y = stof(attr->value());
        } else if ("Z" == name) {
            z = stof(attr->value());
        }
    }
    return new Scale(x, y, z);
}

Color* parse_color(xml_node<char>* node)
{
    float r, g, b;
    r = g = b = 0.0f;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = string(attr->name());
        if ("R" == name) {
            r = stof(attr->value());
        } else if ("G" == name) {
            g = stof(attr->value());
        } else if ("B" == name) {
            b = stof(attr->value());
        }
    }
    return new Color(r, g, b);
}

string Group::internal_to_string(const int level) const
{
    stringstream gstr;
    const auto tab = [=, &gstr](const string t) {
        for (int i = 0; i < level; i++)
            gstr << "\t";
        gstr << t << endl;
    };
    tab("---");
    tab("Transformations [");
    for (const auto& t : transformations) {
        gstr << "\t";
        tab(t->to_string());
    }
    tab("]");
    tab("Models [");
    for (const auto& m : models) {
        gstr << "\t";
        tab(m.name());
    }
    tab("]");
    tab("SubGroups {");
    for (const auto& g : subgroups) {
        gstr << g.internal_to_string(level + 1);
    }
    tab("}");
    tab("---");
    return gstr.str();
}

string Group::to_string() const
{
    return internal_to_string(0);
}
