#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../common/point.hpp"
#include "../dependencies/rapidxml.hpp"
#include "rgb.hpp"

#include <stdexcept>

class Light {
private:
    static inline size_t number = 0;

public:
    static size_t next_number()
    {
        if (number >= 8) {
            throw std::invalid_argument("Too many lights !");
        }
        return number++;
    }
    virtual void render() const = 0;
    virtual std::string to_string() const = 0;
};

class PointLight : public Light {
private:
    Point pos;
    RGBA ambient;
    RGBA diffuse;
    size_t number;

public:
    PointLight(rapidxml::xml_node<char>*);
    void render() const;
    std::string to_string() const;
};

class DirectionalLight : public Light {
private:
    Point pos;
    RGBA ambient;
    RGBA diffuse;
    size_t number;

public:
    DirectionalLight(rapidxml::xml_node<char>*);
    void render() const;
    std::string to_string() const;
};

class SpotLight : public Light {
private:
    Point direction;
    Point pos;
    RGBA diffuse;
    size_t number;

public:
    SpotLight(rapidxml::xml_node<char>*);
    void render() const;
    std::string to_string() const;
};

#endif // LIGHT_HPP
