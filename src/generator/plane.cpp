#include "plane.hpp"
#include <stdexcept>

using namespace std;

const string Plane::help_message = "plane requires: side_length";

Plane::Plane(int argc, char** args)
{
    if (argc < 1) {
        throw invalid_argument("Not enough arguments");
    }
    side_length = stod(*args);
}

std::vector<Point> Plane::draw() const
{
    vector<Point> coordsPlane;
    float axis = side_length / 2;

    //first triangle
    coordsPlane.push_back(Point(axis, 0, axis));
    coordsPlane.push_back(Point(-axis, 0, -axis));
    coordsPlane.push_back(Point(-axis, 0, axis));

    //second triangle
    coordsPlane.push_back(Point(-axis, 0, -axis));
    coordsPlane.push_back(Point(axis, 0, axis));
    coordsPlane.push_back(Point(axis, 0, -axis));

    return coordsPlane;
}
