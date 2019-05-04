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
        Point a(radius * sin(d_alpha), base, radius * cos(d_alpha));
        Point b(0.0, base, 0.0);
        Point c(radius * sin(alpha), base, radius * cos(alpha));
        Point al(radius * sin(d_alpha), top_base, radius * cos(d_alpha));
        Point bl(0.0, top_base, 0.0);
        Point cl(radius * sin(alpha), top_base, radius * cos(alpha));
        // BASE
        coords.push_back(c);
        coords.push_back(b);
        coords.push_back(a);
        // TOP
        coords.push_back(al);
        coords.push_back(bl);
        coords.push_back(cl);
        // SIDE
        coords.push_back(a);
        coords.push_back(al);
        coords.push_back(c);

        coords.push_back(c);
        coords.push_back(al);
        coords.push_back(cl);
        alpha += _alpha;
    }

    return coords;
}
