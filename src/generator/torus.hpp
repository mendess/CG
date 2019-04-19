#ifndef TORUS_H
#define TORUS_H
#include "primitives.hpp"

class Torus : public Primitive {
private:
    double innerRadius, outerRadius;
    int sides, rings;

public:
    Torus(int argc, char** args);
    std::vector<Point> draw() const;
    const static std::string help_message;
    ~Torus() {};
};

#endif // TORUS_H
