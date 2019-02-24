#ifndef POINT_H
#define POINT_H
#include <math.h>
#include <sstream>
#include <string>

class Point {
private:
    float _x, _y, _z;

public:
    Point();
    Point(float, float, float);
    std::string toString();
    float x() { return _x; }
    float y() { return _y; }
    float z() { return _z; }
    ~Point() {}
};

#endif // POINT_H
