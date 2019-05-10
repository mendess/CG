
#include "light.hpp"

#include <algorithm>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace rapidxml;
using namespace std;

void enable_light(size_t number)
{
    cout << "number: " << number << endl;
    glEnable((int[]) { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 }[number]);
}

PointLight::PointLight(xml_node<char>* light)
{
    float x, y, z;
    x = y = z = 0;
    for (auto attr = light->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        if ("POSX" == name) {
            x = stof(value);
        } else if ("POSY" == name) {
            y = stof(value);
        } else if ("POSZ" == name) {
            z = stof(value);
        } else if ("TYPE" == name) {
            continue;
        } else {
            cerr << "Invalid attribute: " << name << endl;
        }
    }
    pos = Point(x, y, z);
}

void PointLight::enable() const
{
    enable_light(number);
}

void PointLight::render() const
{
    GLfloat amb[4] = { ambient.r, ambient.g, ambient.b, 1.0 };
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
}

DirectionalLight::DirectionalLight(xml_node<char>* light)
{
    float x, y, z;
    x = y = z = 0;
    for (auto attr = light->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        if ("POSX" == name) {
            x = stof(value);
        } else if ("POSY" == name) {
            y = stof(value);
        } else if ("POSZ" == name) {
            z = stof(value);
        } else if ("TYPE" == name) {
            continue;
        } else {
            cerr << "Invalid attribute: " << name << endl;
        }
    }
    pos = Point(x, y, z);
}

void DirectionalLight::enable() const
{
    enable_light(number);
}

void DirectionalLight::render() const
{
    GLfloat amb[4] = { ambient.r, ambient.g, ambient.b, 1.0 };
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
}

SpotLight::SpotLight(xml_node<char>* light)
{
    float x, y, z, dir_x, dir_y, dir_z;
    x = y = z = dir_x = dir_y = dir_z = 0;
    for (auto attr = light->first_attribute(); attr != NULL; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        if ("POSX" == name) {
            x = stof(value);
        } else if ("POSY" == name) {
            y = stof(value);
        } else if ("POSZ" == name) {
            z = stof(value);
        } else if ("DIRX" == name) {
            dir_x = stof(value);
        } else if ("DIRY" == name) {
            dir_y = stof(value);
        } else if ("DIRZ" == name) {
            dir_z = stof(value);
        } else if ("TYPE" == name) {
            continue;
        } else {
            cerr << "Invalid attribute: " << name << endl;
        }
    }
    pos = Point(x, y, z);
    direction = Point(dir_x, dir_y, dir_z);
}

void SpotLight::enable() const
{
    enable_light(number);
}

void SpotLight::render() const
{
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 1.0 };
    GLfloat spotDir[3] = { direction.x(), direction.y(), direction.z() };

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0);
}
