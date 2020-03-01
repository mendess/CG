#ifndef BOX_H
#define BOX_H
#include "primitives.hpp"

class Box : public Primitive {
private:
    double x, y, z;
    int divisions;

public:
    const inline static std::string help_message = "box requires: x y z [divisions]";
    Box(int argc, char** args);
    std::vector<Point> draw() const;
    ~Box() {};
};

#endif // BOX_H
