#include "generator/cylinder.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

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
    const double base = -1 * height / 2.0;
    const double top_base = base + height;
    const Vector top_norm = Vector(0, 1, 0);
    const Vector bot_norm = Vector(0, -1, 0);
    const float texture_x_shift = 1.0f / slices;
    const float texture_y_bot = .625f;
    const float texture_y_top = 0.f;
    const float texture_top_center_x = 0.4375;
    const float texture_top_center_y = 0.8125;
    const float texture_bot_center_x = 0.8125;
    const float texture_bot_center_y = 0.8125;
    for (int i = 0; i < slices; i++) {
        const float alpha = _alpha * i;
        const float alpha_next = _alpha * (i + 1);
        const Vector side_norm = Vector(sin(alpha), 0, cos(alpha));
        const Vector next_side_norm = Vector(sin(alpha_next), 0, cos(alpha_next));
        const float texture_x = texture_x_shift * i;
        const float next_texture_x = texture_x_shift * (i + 1);

        const Point bb = Point(0.0, base, 0.0)
                             .set_normal(bot_norm)
                             .set_texture(texture_bot_center_x, texture_bot_center_y);
        const Point tb = Point(0.0, top_base, 0.0)
                             .set_normal(top_norm)
                             .set_texture(texture_top_center_x, texture_top_center_y);

        const Point ba = Point(radius * sin(alpha_next), base, radius * cos(alpha_next))
                             .set_normal(next_side_norm)
                             .set_texture(next_texture_x, texture_y_bot);

        const Point bc = Point(radius * sin(alpha), base, radius * cos(alpha))
                             .set_normal(side_norm)
                             .set_texture(texture_x, texture_y_bot);

        const Point ta = Point(radius * sin(alpha_next), top_base, radius * cos(alpha_next))
                             .set_normal(next_side_norm)
                             .set_texture(next_texture_x, texture_y_top);

        const Point tc = Point(radius * sin(alpha), top_base, radius * cos(alpha))
                             .set_normal(side_norm)
                             .set_texture(texture_x, texture_y_top);

        // BASE
        coords.push_back(bc.set_normal(bot_norm)
                             .set_texture(texture_bot_center_x + sin(alpha) * 0.1875, texture_bot_center_y + cos(alpha) * 0.1875));
        coords.push_back(bb);
        coords.push_back(ba.set_normal(bot_norm)
                             .set_texture(texture_bot_center_x + sin(alpha_next) * 0.1875, texture_bot_center_y + cos(alpha_next) * 0.1875));
        // TOP
        coords.push_back(ta.set_normal(top_norm)
                             .set_texture(texture_top_center_x + sin(alpha_next) * 0.1875, texture_top_center_y + cos(alpha_next) * 0.1875));
        coords.push_back(tb);
        coords.push_back(tc.set_normal(top_norm)
                             .set_texture(texture_top_center_x + sin(alpha) * 0.1875, texture_top_center_y + cos(alpha) * 0.1875));
        // SIDE
        coords.push_back(ba);
        coords.push_back(ta);
        coords.push_back(bc);

        coords.push_back(bc);
        coords.push_back(ta);
        coords.push_back(tc);
    }

    return coords;
}
