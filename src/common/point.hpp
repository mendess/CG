#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

class Vector {
private:
    float _x, _y, _z;

public:
    Vector(float, float, float);
    std::string to_string() const;
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
    Vector normalize() const;
    Vector cross(Vector) const;
    Vector operator/(float) const;
};

class Point {
private:
    float _x, _y, _z;
    Vector _normal;
    float _texture_x, _texture_y;

public:
    Point();
    Point(float, float, float);
    Point(float, float, float, Vector);
    Point(float, float, float, Vector, float, float);
    Point(float, float, float, Vector, std::tuple<float, float>);
    std::string to_string() const;
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
    Vector normal() const { return _normal; }
    float texture_x() const { return _texture_x; }
    float texture_y() const { return _texture_y; }
    Point setXYtoTexture(float, float, float, float) const;
    Point setXZtoTexture(float, float, float, float) const;
    Point setYZtoTexture(float, float, float, float) const;
    Point setNormalToOrigin() const;
    Point setTexture(float, float) const;
    std::array<float, 3> as_array() const;
    Point operator+(const Point) const;
    Point operator*(float)const;
    Point operator/(float) const;
    ~Point() {}
};

#endif // POINT_H
