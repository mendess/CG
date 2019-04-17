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

void mutl_matrix(const float a[4][4], float b[4][4]);

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
    if (max_depth > 0) {
        float elapsed = glutGet(GLUT_ELAPSED_TIME);
        glPushMatrix();
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
        glPopMatrix();
    }
}

void Group::draw_no_models(size_t index) const
{
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    for (const auto& t : transformations) {
        t->transform(elapsed);
    }
    if (index < models.size()) {
        return;
    } else {
        size_t models_skiped = models.size();
        for (const auto& sg : subgroups) {
            if (index < models_skiped + sg.model_count()) {
                sg.draw_no_models(index - models_skiped);
            }
            models_skiped += sg.model_count();
        }
    }
}

optional<Point> Group::get_model_position(size_t index) const
{
    Matrix m = { .matrix = {
                     { 1, 0, 0, 0 },
                     { 0, 1, 0, 0 },
                     { 0, 0, 1, 0 },
                     { 0, 0, 0, 1 } } };
    auto p = get_model_position(index, m);
    if (p.has_value()) {
        const Matrix m = p.value();
        float coords[4];
        const float base[4] = { 0, 0, 0, 1 };
        for (int i = 0; i < 4; ++i) {
            coords[i] = 0;
            for (int j = 0; j < 4; ++j) {
                coords[i] += base[j] * m.matrix[i][j];
            }
        }
        return Point(coords[0], coords[1], coords[2]);
    } else {
        return nullopt;
    }
}

optional<Matrix> Group::get_model_position(size_t index, Matrix position) const
{
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    if (index >= models.size()) {
        size_t models_skiped = models.size();
        for (const auto& sg : subgroups) {
            if (index < models_skiped + sg.model_count()) {
                auto p = sg.get_model_position(index - models_skiped, position);
                if (!p.has_value()) {
                    return nullopt;
                } else {
                    position = p.value();
                }
                for (size_t i = transformations.size(); i > 0; --i) {
                    mutl_matrix(transformations[i - 1]->matrix(elapsed).matrix, position.matrix);
                }
                return position;
            }
            models_skiped += sg.model_count();
        }
    } else {
        for (size_t i = transformations.size(); i > 0; --i) {
            mutl_matrix(transformations[i - 1]->matrix(elapsed).matrix, position.matrix);
        }
        return position;
    }
    return nullopt;
}

void mutl_matrix(const float a[4][4], float b[4][4])
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
