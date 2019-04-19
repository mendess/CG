#include "cylinder.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

const string Cylinder::help_message = "cylinder requires: radius height slices";

Cylinder::Cylinder(int argc, char** args)
{
    if (argc < 3) {
        throw invalid_argument("Not enought arguments");
    }
    radius = stod(args[0]);
    height = stod(args[1]);
    slices = stoi(args[2]);
}

std::vector<Point> Cylinder::draw() const
{
    vector<Point> coords;
    const double _alpha = (2 * M_PI) / slices;
    double alpha = 0.0;
    const double base = -1 * height / 2.0;
    const double top_base = base + height;
    for (int i = 0; i < slices; i++) {
        float d_alpha = alpha + _alpha;
        double a[3] = { radius * sin(d_alpha), base, radius * cos(d_alpha) };
        double b[3] = { 0.0, base, 0.0 };
        double c[3] = { radius * sin(alpha), base, radius * cos(alpha) };
        double al[3] = { radius * sin(d_alpha), top_base, radius * cos(d_alpha) };
        double bl[3] = { 0.0, top_base, 0.0 };
        double cl[3] = { radius * sin(alpha), top_base, radius * cos(alpha) };
        // BASE
        coords.push_back(Point(c[0], c[1], c[2]));
        coords.push_back(Point(b[0], b[1], b[2]));
        coords.push_back(Point(a[0], a[1], a[2]));
        // TOP
        coords.push_back(Point(al[0], al[1], al[2]));
        coords.push_back(Point(bl[0], bl[1], bl[2]));
        coords.push_back(Point(cl[0], cl[1], cl[2]));
        // SIDE
        coords.push_back(Point(a[0], a[1], a[2]));
        coords.push_back(Point(al[0], al[1], al[2]));
        coords.push_back(Point(c[0], c[1], c[2]));
        coords.push_back(Point(c[0], c[1], c[2]));
        coords.push_back(Point(al[0], al[1], al[2]));
        coords.push_back(Point(cl[0], cl[1], cl[2]));
        alpha += _alpha;
    }

    return coords;
}
