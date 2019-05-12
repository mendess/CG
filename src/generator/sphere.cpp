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

    const float texture_x_shift = 1.0 / slices;
    const float texture_y_shift = 1.0 / stacks;

    for (int phi = 0; phi < slices; phi++) {
        for (int theta = 0; theta < stacks; theta++) {
            const float currentStack = theta * thetaMovement;
            const float currentSlice = phi * phiMovement;
            const float nextStack = currentStack + thetaMovement;
            const float nextSlice = currentSlice + phiMovement;

            const float texture_x = phi * texture_x_shift;
            const float texture_y = theta * texture_y_shift;
            const float next_texture_x = (phi + 1) * texture_x_shift;
            const float next_texture_y = (theta + 1) * texture_y_shift;

            Point p0 = Point(radius * sin(nextStack) * sin(nextSlice), radius * cos(nextStack), radius * sin(nextStack) * cos(nextSlice))
                           .setNormalToOrigin()
                           .setTexture(next_texture_x, next_texture_y);

            Point p1 = Point(radius * sin(nextStack) * sin(currentSlice), radius * cos(nextStack), radius * sin(nextStack) * cos(currentSlice))
                           .setNormalToOrigin()
                           .setTexture(texture_x, next_texture_y);

            Point p2 = Point(radius * sin(currentStack) * sin(nextSlice), radius * cos(currentStack), radius * sin(currentStack) * cos(nextSlice))
                           .setNormalToOrigin()
                           .setTexture(next_texture_x, texture_y);

            Point p3 = Point(radius * sin(currentStack) * sin(currentSlice), radius * cos(currentStack), radius * sin(currentStack) * cos(currentSlice))
                           .setNormalToOrigin()
                           .setTexture(texture_x, texture_y);
            //////
            coordsSphere.push_back(p0);
            coordsSphere.push_back(p3);
            coordsSphere.push_back(p1);
            coordsSphere.push_back(p2);
            coordsSphere.push_back(p3);
            coordsSphere.push_back(p0);
        }
    }
    return coordsSphere;
}
