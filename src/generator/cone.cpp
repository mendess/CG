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
    const float phi = (2 * M_PI) / slices;
    const float stackSpacing = height / stacks;
    const float theta = radius / stacks;

    const float beta = atan(height / radius);
    const float alpha_shift = 2 * M_PI / slices;

    const float texture_x_shift = 1.0 / slices;
    const float texture_y_shift = 1.0 / stacks;

    for (int i = 0; i < stacks; i++) {
        for (int k = 0; k < slices; k++) {
            const float currentTheta = theta * i;
            const float currentPhi = phi * k;
            const float nextTheta = theta * (i + 1);
            const float nextPhi = phi * (k + 1);
            if (!i) {
                //Base
                Point p0(0, 0, 0);
                Point p1(radius * sin(nextPhi), 0, radius * cos(nextPhi));
                Point p2(radius * sin(currentPhi), 0, radius * cos(currentPhi));
                Vector v = Vector(0, -1, 0);
                coordsCone.push_back(p0.setNormal(v));
                coordsCone.push_back(p1.setNormal(v));
                coordsCone.push_back(p2.setNormal(v));
            }
            const float texture_x = i * texture_x_shift;
            const float texture_y = k * texture_y_shift;
            const float next_texture_x = (i + 1) * texture_x_shift;
            const float next_texture_y = (k + 1) * texture_y_shift;
            float alpha = alpha_shift * k;
            Vector sideNorm = Vector(cos(beta) * sin(alpha), sin(beta), cos(beta) * cos(alpha)).normalize();
            Point p3((radius - currentTheta) * sin(currentPhi), i * stackSpacing, (radius - currentTheta) * cos(currentPhi));
            Point p4((radius - currentTheta) * sin(nextPhi), i * stackSpacing, (radius - currentTheta) * cos(nextPhi));
            Point p5(0, stacks * stackSpacing, 0);
            Point p6((radius - nextTheta) * sin(nextPhi), (i + 1) * stackSpacing, (radius - nextTheta) * cos(nextPhi));
            Point p7((radius - nextTheta) * sin(currentPhi), (i + 1) * stackSpacing, (radius - nextTheta) * cos(currentPhi));

            if (i == stacks - 1) {
                //Top
                coordsCone.push_back(p3.setNormal(sideNorm).setTexture(texture_x, texture_y));
                coordsCone.push_back(p4.setNormal(sideNorm).setTexture(texture_x, next_texture_y));
                coordsCone.push_back(p5.setNormal(sideNorm).setTexture(texture_x, 0));
            } else {
                //Around
                coordsCone.push_back(p3.setNormal(sideNorm).setTexture(texture_x, texture_y));
                coordsCone.push_back(p6.setNormal(sideNorm).setTexture(next_texture_x, next_texture_y));
                coordsCone.push_back(p7.setNormal(sideNorm).setTexture(next_texture_x, texture_y));
                coordsCone.push_back(p3.setNormal(sideNorm).setTexture(texture_x, texture_y));
                coordsCone.push_back(p4.setNormal(sideNorm).setTexture(texture_x, next_texture_y));
                coordsCone.push_back(p6.setNormal(sideNorm).setTexture(next_texture_x, next_texture_y));
            }
        }
    }

    return coordsCone;
}
