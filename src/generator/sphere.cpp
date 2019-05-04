#include "sphere.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

const string Sphere::help_message = "sphere requires: radius slices stacks";

Sphere::Sphere(int argc, char** args)
{
    if (argc < 3) {
        throw invalid_argument("Not enough arguments");
    }
    radius = stod(args[0]);
    slices = stoi(args[1]);
    stacks = stoi(args[2]);
}

std::vector<Point> Sphere::draw() const
{
    vector<Point> coordsSphere;

    //calculate the Phi Movement (triangle's up/down side length)
    //PI * 2 because it is necessary to go all around to count all of the slices
    float phiMovement = M_PI * 2 / slices;

    //calculate the Theta Movement (triangle's left/right side length)
    //only PI because it is only necessary to go half the perimeter in the
    //vertical direction to count all of the stacks;
    float thetaMovement = M_PI / stacks;

    for (int phi = 0; phi < slices; phi++)
        for (int theta = 0; theta < stacks; theta++) {

            const float currentStack = theta * thetaMovement;
            const float currentSlice = phi * phiMovement;
            const float nextStack = currentStack + thetaMovement;
            const float nextSlice = currentSlice + phiMovement;

            Point p0(radius * sin(nextStack) * sin(nextSlice), radius * cos(nextStack), radius * sin(nextStack) * cos(nextSlice));
            Point p1(radius * sin(nextStack) * sin(currentSlice), radius * cos(nextStack), radius * sin(nextStack) * cos(currentSlice));
            Point p2(radius * sin(currentStack) * sin(nextSlice), radius * cos(currentStack), radius * sin(currentStack) * cos(nextSlice));
            Point p3(radius * sin(currentStack) * sin(currentSlice), radius * cos(currentStack), radius * sin(currentStack) * cos(currentSlice));
            //////
            coordsSphere.push_back(p0);
            coordsSphere.push_back(p3);
            coordsSphere.push_back(p1);
            coordsSphere.push_back(p2);
            coordsSphere.push_back(p3);
            coordsSphere.push_back(p0);
        }

    return coordsSphere;
}
