#ifndef PARSER_H
#define PARSER_H

#include "../common/point.hpp"
#include "primitives.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr size_t N_CONTROL_POINTS = 16;

class Patch {
private:
public:
    std::array<size_t, N_CONTROL_POINTS> control_points;
    Patch(std::vector<size_t>);
    std::string to_string() const;
};

class Patches : public Primitive {
private:
    int tessellation;
    std::vector<Patch> patches;
    std::vector<Point> points;

    Point bezier_point(size_t, float, float) const;
    Point point_at(size_t, size_t) const;

public:
    Patches(int, char**);
    void construct(std::string, int);
    std::vector<Point> draw() const;
    std::string to_string() const;
    const static std::string help_message;
    ~Patches() {};
};

#endif // PARSER_H
