#include "point.hpp"
#include <sstream>

using namespace std;

Point::Point()
    : _x(0)
    , _y(0)
    , _z(0)
{
}

Point::Point(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
}

array<float, 3> Point::as_array() const
{
    return { _x, _y, _z };
}

Point Point::operator*(float scalar) const
{
    return Point(_x * scalar, _y * scalar, _z * scalar);
}

Point Point::operator+(const Point other) const
{
    return Point(_x + other._x, _y + other._y, _z + other._z);
}

Point Point::operator/(float l) const
{
    return Point(_x / l, _y / l, _z / l);
}

string Point::to_string() const
{
    ostringstream sstream;
    sstream << _x << ' ' << _y << ' ' << _z;
    return sstream.str();
}
