#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../common/point.hpp"
#include "../dependencies/rapidxml.hpp"
#include "rgb.hpp"

class Light {
public:
    virtual void prepare() const = 0;
    virtual void render() const = 0;
};

class PointLight : public Light {
private:
    Point pos;
    RGB ambient;
    RGB diffuse;
    size_t number;

public:
    PointLight(rapidxml::xml_node<char>*);
    void prepare() const;
    void render() const;
};

class DirectionalLight : public Light {
private:
    Point pos;
    RGB ambient;
    RGB diffuse;
    size_t number;

public:
    DirectionalLight(rapidxml::xml_node<char>*);
    void prepare() const;
    void render() const;
};

class SpotLight : public Light {
private:
    Point direction;
    Point pos;
    RGB diffuse;
    size_t number;

public:
    SpotLight(rapidxml::xml_node<char>*);
    void prepare() const;
    void render() const;
};

#endif // LIGHT_HPP
