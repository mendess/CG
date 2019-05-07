#include "point.hpp"
#include <cmath>
#include <sstream>

using namespace std;

Vector::Vector(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
}

Vector Vector::normalize() const
{
    return *this / sqrt(_x * _x + _y * _y + _z * _z);
}

Vector Vector::cross(Vector other) const
{
    return Vector(
        _y * other._z - _z * other._y,
        _z * other._x - _x * other._z,
        _x * other._y - _y * other._z);
}

Vector Vector::operator/(float l) const
{
    return Vector(
        _x / l,
        _y / l,
        _z / l);
}

Point::Point()
    : _x(0)
    , _y(0)
    , _z(0)
    , _normal(Vector(0, 0, 0))
    , _texture_x(0)
    , _texture_y(0)
{
}

Point::Point(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
    , _normal(Vector(0, 0, 0))
    , _texture_x(0)
    , _texture_y(0)
{
}

Point::Point(float x, float y, float z, Vector normal)
    : _x(x)
    , _y(y)
    , _z(z)
    , _normal(normal)
    , _texture_x(0)
    , _texture_y(0)
{
}

Point::Point(float x, float y, float z, Vector normal, float texture_x, float texture_y)
    : _x(x)
    , _y(y)
    , _z(z)
    , _normal(normal)
    , _texture_x(texture_x)
    , _texture_y(texture_y)
{
}

Point::Point(float x, float y, float z, Vector normal, tuple<float, float> texture)
    : _x(x)
    , _y(y)
    , _z(z)
    , _normal(normal)
    , _texture_x(get<0>(texture))
    , _texture_y(get<1>(texture))
{
}

Point Point::setXYtoTexture(float maxX, float maxY, float x_offset, float y_offset) const
{
    // maxX == 0.5
    // maxY == 0.5
    // offset == 0
    return Point(
        _x,
        _y,
        _z,
        _normal,
        { x_offset + ((_x + maxX) * 0.33) / (2 * maxX),
            y_offset + ((_y + maxY) * 0.5) / (2 * maxY) });
}

Point Point::setXZtoTexture(float maxX, float maxZ, float x_offset, float z_offset) const
{
    return Point(
        _x,
        _y,
        _z,
        _normal,
        { x_offset + ((_x + maxX) * 0.33) / (2 * maxX), z_offset + ((_z + maxZ) * 0.5) / (2 * maxZ) });
}

Point Point::setYZtoTexture(float maxY, float maxZ, float y_offset, float z_offset) const
{
    return Point(
        _x,
        _y,
        _z,
        _normal,
        { y_offset + ((_y + maxY) * 0.33) / (2 * maxY), z_offset + ((_z + maxZ) * 0.5) / (2 * maxZ) });
}

array<float, 3> Point::as_array() const
{
    return { _x, _y, _z };
}

Point Point::operator*(float scalar) const
{
    return Point(
        _x * scalar,
        _y * scalar,
        _z * scalar,
        _normal,
        _texture_x,
        _texture_y);
}

Point Point::operator+(const Point other) const
{
    return Point(
        _x + other._x,
        _y + other._y,
        _z + other._z,
        _normal,
        _texture_x,
        _texture_y);
}

Point Point::operator/(float l) const
{
    return Point(
        _x / l,
        _y / l,
        _z / l,
        _normal,
        _texture_x,
        _texture_y);
}

string Point::to_string() const
{
    ostringstream sstream;
    sstream
        << _x << ' '
        << _y << ' '
        << _z << ' '
        << _normal.x() << ' '
        << _normal.y() << ' '
        << _normal.z() << ' '
        << _texture_x << ' '
        << _texture_y;
    return sstream.str();
}
