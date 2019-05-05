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

    Vector normal(0, 1, 0);
    Point p0(-axis, 0, -axis, normal, 0, 0);
    Point p1(-axis, 0, axis, normal, 0, 1);
    Point p2(axis, 0, -axis, normal, 1, 0);
    Point p3(axis, 0, axis, normal, 1, 1);
    //first triangle
    coordsPlane.push_back(p3);
    coordsPlane.push_back(p0);
    coordsPlane.push_back(p1);

    //second triangle
    coordsPlane.push_back(p0);
    coordsPlane.push_back(p3);
    coordsPlane.push_back(p2);

    return coordsPlane;
}
