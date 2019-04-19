#include "cone.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

const string Cone::help_message = "cone requires: radius height slices stacks";

Cone::Cone(int argc, char** args)
{
    if (argc < 4) {
        throw invalid_argument("Not enough arguments");
    }
    radius = stod(args[0]);
    height = stod(args[1]);
    slices = stoi(args[2]);
    stacks = stoi(args[3]);
}

std::vector<Point> Cone::draw() const
{
    vector<Point> coordsCone;
    float phi = (2 * M_PI) / slices;
    float stackSpacing = height / stacks;
    float theta = radius / stacks;

    for (int i = 0; i < stacks; i++) {
        for (int k = 0; k < slices; k++) {

            if (!i) {
                //Base
                coordsCone.push_back(Point(0, 0, 0));
                coordsCone.push_back(Point(radius * sin(phi * (k + 1)), 0, radius * cos(phi * (k + 1))));
                coordsCone.push_back(Point(radius * sin(phi * k), 0, radius * cos(phi * k)));
            }

            if (i == stacks - 1) {
                //Top
                coordsCone.push_back(Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
                coordsCone.push_back(Point((radius - theta * i) * sin(phi * (k + 1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k + 1))));
                coordsCone.push_back(Point(0, stacks * stackSpacing, 0));
            }

            else { //Around
                coordsCone.push_back(Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
                coordsCone.push_back(Point((radius - theta * (i + 1)) * sin(phi * (k + 1)), (i + 1) * stackSpacing, (radius - theta * (i + 1)) * cos(phi * (k + 1))));
                coordsCone.push_back(Point((radius - theta * (i + 1)) * sin(phi * k), (i + 1) * stackSpacing, (radius - theta * (i + 1)) * cos(phi * k)));
                coordsCone.push_back(Point((radius - theta * i) * sin(phi * k), i * stackSpacing, (radius - theta * i) * cos(phi * k)));
                coordsCone.push_back(Point((radius - theta * i) * sin(phi * (k + 1)), i * stackSpacing, (radius - theta * i) * cos(phi * (k + 1))));
                coordsCone.push_back(Point((radius - theta * (i + 1)) * sin(phi * (k + 1)), (i + 1) * stackSpacing, (radius - theta * (i + 1)) * cos(phi * (k + 1))));
            }
        }
    }

    return coordsCone;
}
