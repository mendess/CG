#include "torus.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

const string Torus::help_message = "torus requires: innerRadius outerRadius sides rings";

Torus::Torus(int argc, char** args)
{
    if (argc < 4) {
        throw invalid_argument("Not enought arguments");
    }
    innerRadius = stod(args[0]);
    outerRadius = stod(args[1]);
    sides = stoi(args[2]);
    rings = stoi(args[3]);
}

std::vector<Point> Torus::draw() const
{
    vector<Point> coords;
    const double alpha_step = (2 * M_PI) / rings;
    const double beta_step = (2 * M_PI) / sides;
    const double ring_distance = outerRadius - innerRadius;
    const double ring_radius = innerRadius;
    double alpha = 0;
    for (int r = 0; r < rings; r++) {
        double beta = 0;
        double ring_x = ring_distance * cos(beta) * sin(alpha);
        double ring_y = ring_distance * cos(beta) * cos(alpha);
        double ring_z = 0;
        double next_ring_x = ring_distance * cos(beta) * sin(alpha + alpha_step);
        double next_ring_y = ring_distance * cos(beta) * cos(alpha + alpha_step);
        double next_ring_z = 0;
        for (int s = 0; s < sides; s++) {
            Point p0 = Point(
                ring_x + (ring_radius * cos(beta) * sin(alpha)),
                ring_y + (ring_radius * cos(beta) * cos(alpha)),
                ring_z + (ring_radius * sin(beta)))
                           .setNormal(
                               Vector(
                                   ring_radius * cos(beta) * sin(alpha),
                                   ring_radius * cos(beta) * cos(alpha),
                                   ring_radius * sin(beta))
                                   .normalize());
            Point p1 = Point(
                next_ring_x + (ring_radius * cos(beta) * sin(alpha + alpha_step)),
                next_ring_y + (ring_radius * cos(beta) * cos(alpha + alpha_step)),
                next_ring_z + (ring_radius * sin(beta)))
                           .setNormal(
                               Vector(
                                   ring_radius * cos(beta) * sin(alpha + alpha_step),
                                   ring_radius * cos(beta) * cos(alpha + alpha_step),
                                   ring_radius * sin(beta))
                                   .normalize());
            Point p2 = Point(
                ring_x + (ring_radius * cos(beta + beta_step) * sin(alpha)),
                ring_y + (ring_radius * cos(beta + beta_step) * cos(alpha)),
                ring_z + (ring_radius * sin(beta + beta_step)))
                           .setNormal(
                               Vector(
                                   ring_radius * cos(beta + beta_step) * sin(alpha),
                                   ring_radius * cos(beta + beta_step) * cos(alpha),
                                   ring_radius * sin(beta + beta_step))
                                   .normalize());
            Point p3 = Point(
                next_ring_x + (ring_radius * cos(beta + beta_step) * sin(alpha + alpha_step)),
                next_ring_y + (ring_radius * cos(beta + beta_step) * cos(alpha + alpha_step)),
                next_ring_z + (ring_radius * sin(beta + beta_step)))
                           .setNormal(
                               Vector(
                                   ring_radius * cos(beta + beta_step) * sin(alpha + alpha_step),
                                   ring_radius * cos(beta + beta_step) * cos(alpha + alpha_step),
                                   ring_radius * sin(beta + beta_step))
                                   .normalize());
            // T1
            coords.push_back(p1);
            coords.push_back(p0);
            coords.push_back(p2);
            // T2
            coords.push_back(p2);
            coords.push_back(p3);
            coords.push_back(p1);
            beta += beta_step;
        }
        alpha += alpha_step;
    }
    return coords;
}
