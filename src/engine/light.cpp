
#include "light.hpp"
#include "../common/util.hpp"

#include <unordered_map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace rapidxml;
using namespace std;

int gl_light(size_t number)
{
    return (int[]) { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 }[number];
}

DirectionalLight::DirectionalLight(xml_node<char>* light)
{
    float x, y, z;
    float amb_r, amb_g, amb_b;
    float dif_r, dif_g, dif_b;
    unordered_map<string, string> params = util::params_to_map(light);
    pos = Point(
        get_or_default(params["POSX"], 0),
        get_or_default(params["POSY"], 0),
        get_or_default(params["POSZ"], 0));
    RGB color = get_component("", 1);
    ambient = color / 0.2;
    diffuse = color;
    number = Light::next_number();
    glEnable(gl_light(number));
}

void DirectionalLight::render() const
{
    GLfloat amb[4] = { ambient.r, ambient.g, ambient.b, 1.0 };
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 0 };
    glLightfv(gl_light(number), GL_POSITION, position);
    glLightfv(gl_light(number), GL_AMBIENT, amb);
    glLightfv(gl_light(number), GL_DIFFUSE, diff);
}

PointLight::PointLight(xml_node<char>* light)
{
    float x, y, z;
    float amb_r, amb_g, amb_b;
    float dif_r, dif_g, dif_b;
    unordered_map<string, string> params = util::params_to_map(light);
    pos = Point(
        get_or_default(params["POSX"], 0),
        get_or_default(params["POSY"], 0),
        get_or_default(params["POSZ"], 0));
    RGB color = get_component("", 1);
    ambient = color * 0.2;
    diffuse = color;
    number = Light::next_number();
    glEnable(gl_light(number));
}

void PointLight::render() const
{
    GLfloat amb[4] = { ambient.r, ambient.g, ambient.b, 1.0 };
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 1.0 };
    glLightfv(gl_light(number), GL_POSITION, position);
    glLightfv(gl_light(number), GL_AMBIENT, amb);
    glLightfv(gl_light(number), GL_DIFFUSE, diff);
    GLfloat atten = 0;
    glLightfv(gl_light(number), GL_LINEAR_ATTENUATION, &atten);
}

SpotLight::SpotLight(xml_node<char>* light)
{
    float x, y, z;
    float dif_r, dif_g, dif_b;
    float dir_x, dir_y, dir_z;
    unordered_map<string, string> params = util::params_to_map(light);
    pos = Point(
        get_or_default(params["POSX"], 0),
        get_or_default(params["POSY"], 0),
        get_or_default(params["POSZ"], 0));
    diffuse = get_component("", 1);
    number = Light::next_number();
    glEnable(gl_light(number));
}

void SpotLight::render() const
{
    GLfloat diff[4] = { diffuse.r, diffuse.g, diffuse.b, 1.0 };
    GLfloat position[4] = { pos.x(), pos.y(), pos.z(), 1.0 };
    GLfloat spotDir[3] = { direction.x(), direction.y(), direction.z() };

    glLightfv(gl_light(number), GL_POSITION, position);
    glLightfv(gl_light(number), GL_DIFFUSE, diff);
    glLightfv(gl_light(number), GL_SPOT_DIRECTION, spotDir);
    glLightf(gl_light(number), GL_SPOT_CUTOFF, 45.0);
    glLightf(gl_light(number), GL_SPOT_EXPONENT, 0.0);
}
