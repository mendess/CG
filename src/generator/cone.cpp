#include "cone.hpp"
#include <cmath>
#include <stdexcept>

#include <iostream>
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

#define sin_to_texture_b(a) (base_texture_x + sin(a) / 4)
#define cos_to_texture_b(a) (base_texture_y + cos(a) / 2)

#define sin_to_texture_s(a) (side_texture_x + (current_texture_x_radius * sin(a)))
#define cos_to_texture_s(a) (side_texture_y + (current_texture_y_radius * cos(a)))
#define sin_to_texture_sn(a) (side_texture_x + (next_texture_x_radius * sin(a)))
#define cos_to_texture_sn(a) (side_texture_y + (next_texture_y_radius * cos(a)))

std::vector<Point> Cone::draw() const
{
    vector<Point> coordsCone;
    const float phi = (2 * M_PI) / slices;
    const float stackHeight = height / stacks;
    const float stackRadius = radius / stacks;

    const float beta = atan(height / radius);
    const float alpha_shift = 2 * M_PI / slices;
    const float base_texture_x = .25;
    const float base_texture_y = .5;
    const float side_texture_x = .75;
    const float side_texture_y = .5;
    const float texture_x_radius = .25 / stacks;
    const float texture_y_radius = .5 / stacks;

    for (int i = 0; i < stacks; i++) {
        for (int k = 0; k < slices; k++) {
            const float currentStackRadius = stackRadius * i;
            const float currentPhi = phi * k;
            const float nextStackRadius = stackRadius * (i + 1);
            const float nextPhi = phi * (k + 1);
            const float current_texture_x_radius = texture_x_radius * i;
            const float current_texture_y_radius = texture_y_radius * i;
            const float next_texture_x_radius = texture_x_radius * (i + 1);
            const float next_texture_y_radius = texture_y_radius * (i + 1);
            const float currentStackHeight = (stacks - i) * stackHeight;
            const float nextStackHeight = (stacks - (i + 1)) * stackHeight;
            if (!i) {
                //Base
                Point p0(0, 0, 0);
                Point p1(radius * sin(nextPhi), 0, radius * cos(nextPhi));
                Point p2(radius * sin(currentPhi), 0, radius * cos(currentPhi));
                Vector v = Vector(0, -1, 0);
                coordsCone.push_back(p0.setNormal(v).setTexture(base_texture_x, base_texture_y));
                coordsCone.push_back(p1.setNormal(v).setTexture(sin_to_texture_b(nextPhi), cos_to_texture_b(nextPhi)));
                coordsCone.push_back(p2.setNormal(v).setTexture(sin_to_texture_b(currentPhi), cos_to_texture_b(currentPhi)));
            }
            float alpha = alpha_shift * k;
            float alpha_next = alpha_shift * (k + 1);
            Vector sideNorm = Vector(cos(beta) * sin(alpha), sin(beta), cos(beta) * cos(alpha)).normalize();
            Vector nextSideNorm = Vector(cos(beta) * sin(alpha_next), sin(beta), cos(beta) * cos(alpha_next)).normalize();

            Point p0 = Point(currentStackRadius * sin(currentPhi), currentStackHeight, currentStackRadius * cos(currentPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_s(currentPhi), cos_to_texture_s(currentPhi));

            Point p1 = Point(currentStackRadius * sin(nextPhi), currentStackHeight, currentStackRadius * cos(nextPhi))
                           .setNormal(nextSideNorm)
                           .setTexture(sin_to_texture_s(nextPhi), cos_to_texture_s(nextPhi));

            Point p2 = Point(nextStackRadius * sin(nextPhi), nextStackHeight, nextStackRadius * cos(nextPhi))
                           .setNormal(nextSideNorm)
                           .setTexture(sin_to_texture_sn(nextPhi), cos_to_texture_sn(nextPhi));

            Point p3 = Point(nextStackRadius * sin(currentPhi), nextStackHeight, nextStackRadius * cos(currentPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_sn(currentPhi), cos_to_texture_sn(currentPhi));

            if (i == stacks - 1) {
                //Top
                Point tip = Point(0, stacks * stackRadius, 0)
                                .setNormal(sideNorm)
                                .setTexture(side_texture_x, side_texture_y);

                coordsCone.push_back(p0);
                coordsCone.push_back(p1);
                coordsCone.push_back(tip);
            } else {
                //Around
                coordsCone.push_back(p0);
                coordsCone.push_back(p2);
                coordsCone.push_back(p3);
                coordsCone.push_back(p0);
                coordsCone.push_back(p1);
                coordsCone.push_back(p2);
            }
        }
    }

    return coordsCone;
}
