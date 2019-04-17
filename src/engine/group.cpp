#include "group.hpp"
#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
using namespace rapidxml;

unique_ptr<Transformation> parse_translate(xml_node<char>* node);

unique_ptr<Transformation> parse_rotate(xml_node<char>* node);

unique_ptr<Transformation> parse_scale(xml_node<char>* node);

void mutl_matrix(float a[4][4], float b[4][4]);

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
    _model_count = models.size();
    for (const auto& subgroup : subgroups) {
        _model_count += subgroup.model_count();
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

optional<Point> Group::get_model_position(size_t index) const
{
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    if (index < models.size()) {
        float position[4][4] = {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        };
        for (const auto& t : transformations) {
            mutl_matrix(t->matrix(elapsed).matrix, position);
        }
        return Point(position[0][3], position[1][3], position[2][3]);
    } else {
        size_t models_skiped = models.size();
        for (const auto& sg : subgroups) {
            if (index < models_skiped + sg.model_count()) {
                optional<Point> op = sg.get_model_position(index - models_skiped);
                if (op.has_value()) {
                    Point p = op.value();
                    float position[4][4] = {
                        { 1, 0, 0, p.x() },
                        { 0, 1, 0, p.y() },
                        { 0, 0, 1, p.z() },
                        { 0, 0, 0, 1 }
                    };
                    for (const auto& t : transformations) {
                        mutl_matrix(t->matrix(elapsed).matrix, position);
                    }
                    return Point(position[0][3], position[1][3], position[2][3]);
                } else {
                    return op;
                }
            }
            models_skiped += sg.model_count();
        }
    }
    return nullopt;
}

void mutl_matrix(float a[4][4], float b[4][4])
{
    float tmp[4][4];
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            float op = 0;
            for (size_t k = 0; k < 4; k++) {
                op += a[i][k] * b[k][j];
            }
            tmp[i][j] = op;
        }
    }
    /* cout << "+---------+" << endl */
    /*      << "| " << a[0][0] << " " << a[0][1] << " " << a[0][2] << " " << a[0][3] << " | " << b[0][0] << " " << b[0][1] << " " << b[0][2] << " " << b[0][3] << " | " << tmp[0][0] << " " << tmp[0][1] << " " << tmp[0][2] << " " << tmp[0][3] << " |" << endl */
    /*      << "| " << a[1][0] << " " << a[1][1] << " " << a[1][2] << " " << a[1][3] << " | " << b[1][0] << " " << b[1][1] << " " << b[1][2] << " " << b[1][3] << " | " << tmp[1][0] << " " << tmp[1][1] << " " << tmp[1][2] << " " << tmp[1][3] << " |" << endl */
    /*      << "| " << a[2][0] << " " << a[2][1] << " " << a[2][2] << " " << a[2][3] << " | " << b[2][0] << " " << b[2][1] << " " << b[2][2] << " " << b[2][3] << " | " << tmp[2][0] << " " << tmp[2][1] << " " << tmp[2][2] << " " << tmp[2][3] << " |" << endl */
    /*      << "| " << a[3][0] << " " << a[3][1] << " " << a[3][2] << " " << a[3][3] << " | " << b[3][0] << " " << b[3][1] << " " << b[3][2] << " " << b[3][3] << " | " << tmp[3][0] << " " << tmp[3][1] << " " << tmp[3][2] << " " << tmp[3][3] << " |" << endl */
    /*      << "+---------+" << endl; */
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            b[i][j] = tmp[i][j];
        }
    }
}

unique_ptr<Transformation> parse_translate(xml_node<char>* node)
{
    float x, y, z;
    unordered_map<string, string> params;
    for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = attr->name();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        params[name] = attr->value();
    }
    if (params["TIME"] != "") {
        float dur = stof(params["TIME"]);
        vector<Point> points;
        for (auto point = node->first_node(); point != NULL; point = point->next_sibling()) {
            x = y = z = 0.0f;
            for (auto attr = point->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
                string name = attr->name();
                std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                if ("X" == name || "AXISX" == name) {
                    x = stof(attr->value());
                } else if ("Y" == name || "AXISY" == name) {
                    y = stof(attr->value());
                } else if ("Z" == name || "AXISZ" == name) {
                    z = stof(attr->value());
                }
            }
            points.push_back(Point(x, y, z));
        }
        return make_unique<TranslateAnimated>(points, dur);
    } else {
        x = params["X"] != "" ? stof(params["X"]) : 0;
        y = params["Y"] != "" ? stof(params["Y"]) : 0;
        z = params["Z"] != "" ? stof(params["Z"]) : 0;
        return make_unique<TranslateStatic>(x, y, z);
    }
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

unique_ptr<Transformation> parse_scale(xml_node<char>* node)
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
