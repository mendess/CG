#ifndef POINT_H
#define POINT_H
#include <string>

class Point {
private:
    float _x, _y, _z;

public:
    Point();
    Point(float, float, float);
    std::string to_string() const;
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
    ~Point() {}
};

#endif // POINT_H
