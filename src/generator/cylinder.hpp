#ifndef CYLINDER_H
#define CYLINDER_H
#include "primitives.hpp"

class Cylinder : public Primitive {
private:
    double radius, height;
    int slices;

public:
    Cylinder(int argc, char** args);
    std::vector<Point> draw() const;
    const static std::string help_message;
    ~Cylinder() {};
};

#endif // CYLINDER_H
