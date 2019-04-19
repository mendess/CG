#ifndef SPHERE_H
#define SPHERE_H
#include "primitives.hpp"

class Sphere : public Primitive {
private:
    double radius;
    int slices, stacks;

public:
    Sphere(int argc, char** args);
    std::vector<Point> draw() const;
    const static std::string help_message;
    ~Sphere() {};
};

#endif // SPHERE_H
