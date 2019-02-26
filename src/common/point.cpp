#include "point.hpp"

Point::Point(): _x(0), _y(0), _z(0){}

Point::Point(float x, float y, float z): _x(x), _y(y), _z(z){}

std::string Point::to_string() const
{
    std::ostringstream sstream;
    sstream << _x << ' ' << _y << ' ' << _z << '\n';
    return sstream.str();
}
