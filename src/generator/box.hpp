#ifndef BOX_H
#define BOX_H
#include "primitives.hpp"

class Box : public Primitive {
private:
    double x, y, z;
    int divisions;

public:
    Box(int argc, char** args);
    std::vector<Point> draw() const;
    const static std::string help_message;
    ~Box() {};
};

#endif // BOX_H
