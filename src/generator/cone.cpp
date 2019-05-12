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

std::vector<Point> Cone::draw() const
{
    vector<Point> coordsCone;
    const float phi = (2 * M_PI) / slices;
    const float stackHeight = height / stacks;
    const float stackWidth = radius / stacks;

    const float beta = atan(height / radius);
    const float alpha_shift = 2 * M_PI / slices;
    const float base_texture_x = .25;
    const float base_texture_y = .5;
    const float side_texture_x = .75;
    const float side_texture_y = .5;
    const float texture_x_radius = .25;
    const float texture_y_radius = .5;
    const float texture_x_width = texture_x_radius / stacks;
    const float texture_y_width = texture_y_radius / stacks;

    for (int i = 0; i < stacks; i++) {
        for (int k = 0; k < slices; k++) {
            const float currentStackWidth = stackWidth * i;
            const float currentPhi = phi * k;
            const float nextStackWidth = stackWidth * (i + 1);
            const float nextPhi = phi * (k + 1);
            const float current_texture_x_width = texture_x_width * i;
            const float current_texture_y_width = texture_y_width * i;
            const float next_texture_x_width = texture_x_width * (i + 1);
            const float next_texture_y_width = texture_y_width * (i + 1);
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
            Vector sideNorm = Vector(cos(beta) * sin(alpha), sin(beta), cos(beta) * cos(alpha)).normalize();

#define sin_to_texture_s(a) (side_texture_x + ((texture_x_radius - current_texture_x_width) * sin(a)))
#define cos_to_texture_s(a) (side_texture_y + ((texture_y_radius - current_texture_y_width) * cos(a)))
#define sin_to_texture_sn(a) (side_texture_x + ((texture_x_radius - next_texture_x_width) * sin(a)))
#define cos_to_texture_sn(a) (side_texture_y + ((texture_y_radius - next_texture_y_width) * cos(a)))

            Point p0 = Point((radius - currentStackWidth) * sin(currentPhi), i * stackHeight, (radius - currentStackWidth) * cos(currentPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_s(currentPhi), cos_to_texture_s(currentPhi));

            Point p1 = Point((radius - currentStackWidth) * sin(nextPhi), i * stackHeight, (radius - currentStackWidth) * cos(nextPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_sn(nextPhi), cos_to_texture_s(nextPhi));

            Point p2 = Point((radius - nextStackWidth) * sin(nextPhi), (i + 1) * stackHeight, (radius - nextStackWidth) * cos(nextPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_s(nextPhi), cos_to_texture_sn(nextPhi));

            Point p3 = Point((radius - nextStackWidth) * sin(currentPhi), (i + 1) * stackHeight, (radius - nextStackWidth) * cos(currentPhi))
                           .setNormal(sideNorm)
                           .setTexture(sin_to_texture_sn(currentPhi), cos_to_texture_sn(currentPhi));

            cout << "=================================================" << endl;
            cout << "P0: " << p0.to_string() << endl;
            cout << "P1: " << p1.to_string() << endl;
            cout << "P2: " << p2.to_string() << endl;
            cout << "P3: " << p3.to_string() << endl;
            cout << "=================================================" << endl;

            if (i == stacks - 1) {
                //Top
                Point tip = Point(0, stacks * stackWidth, 0)
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
