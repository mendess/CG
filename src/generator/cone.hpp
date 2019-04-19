#ifndef CONE_H
#define CONE_H
#include "primitives.hpp"

class Cone : public Primitive {
private:
    double radius, height;
    int slices, stacks;

public:
    Cone(int argc, char** args);
    std::vector<Point> draw() const;
    const static std::string help_message;
    ~Cone() {};
};

#endif // CONE_H
