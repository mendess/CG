#include "generator.hpp"
//#include <cstdlib>
//#include <point.hpp>
//#include <math.h>

using namespace std;

std::vector<Point> draw_plane(double side_length)
{
    vector<Point> coordsPlane;
    float axis = side_length / 2;

    //first triangle
    coordsPlane.push_back(Point(axis, 0, axis));
    coordsPlane.push_back(Point(-axis, 0, -axis));
    coordsPlane.push_back(Point(-axis, 0, axis));

    //second triangle
    coordsPlane.push_back(Point(-axis, 0, -axis));
    coordsPlane.push_back(Point(axis, 0, axis));
    coordsPlane.push_back(Point(axis, 0, -axis));

    return coordsPlane;
}

std::vector<Point> draw_box(double x, double y, double z, int divisions)
{
    vector<Point> coordsBox;
    double axisX = x / 2;
    double axisY = y / 2;
    double axisZ = z / 2;
    double spacingX = x / divisions;
    double spacingY = y / divisions;
    double spacingZ = z / divisions;

    for (int i = 0; i < divisions; i++) {
        for (int k = 0; k < divisions; k++) {
            //Front
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * k, axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * k, axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * (k + 1), axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * k, axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * (k + 1), axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * (k + 1), axisZ));

            //Back
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * k, -axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * (k + 1), -axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * k, -axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * k, -axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY + spacingY * (k + 1), -axisZ));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY + spacingY * (k + 1), -axisZ));

            //Up
            coordsBox.push_back(Point(-axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX + spacingX * i, axisY, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), axisY, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX + spacingX * i, axisY, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), axisY, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), axisY, -axisZ + spacingZ * k));

            //Down
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX + spacingX * i, -axisY, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX + spacingX * (i + 1), -axisY, -axisZ + spacingZ * (k + 1)));

            //Left
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * k));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * (k + 1)));

            //Right
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, -axisZ + spacingZ * k));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * k));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * k));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), -axisZ + spacingZ * (k + 1)));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, -axisZ + spacingZ * (k + 1)));
        }
    }

    return coordsBox;
}

std::vector<Point> draw_sphere(double radius, double slices, double stacks)
{
    vector<Point> coordsSphere;

    //calculate the Phi Movement (triangle's up/down side length)
    //PI * 2 because it is necessary to go all around to count all of the slices
    float phiMovement = M_PI * 2 / slices;

    //calculate the Theta Movement (triangle's left/right side length)
    //only PI because it is only necessary to go half the perimeter in the
    //vertical direction to count all of the stacks;
    float thetaMovement = M_PI / stacks;

    for (float phi = 0; phi < slices; phi++)
        for (float theta = 0; theta < stacks; theta++) {

            float currentStack = theta * thetaMovement;
            float currentSlice = phi * phiMovement;

            coordsSphere.push_back(Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
                radius * cos(currentStack + thetaMovement),
                radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement)));

            coordsSphere.push_back(Point(radius * sin(currentStack) * sin(currentSlice),
                radius * cos(currentStack),
                radius * sin(currentStack) * cos(currentSlice)));

            coordsSphere.push_back(Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice),
                radius * cos(currentStack + thetaMovement),
                radius * sin(currentStack + thetaMovement) * cos(currentSlice)));

            coordsSphere.push_back(Point(radius * sin(currentStack) * sin(currentSlice + phiMovement),
                radius * cos(currentStack),
                radius * sin(currentStack) * cos(currentSlice + phiMovement)));

            coordsSphere.push_back(Point(radius * sin(currentStack) * sin(currentSlice),
                radius * cos(currentStack),
                radius * sin(currentStack) * cos(currentSlice)));

            coordsSphere.push_back(Point(radius * sin(currentStack + thetaMovement) * sin(currentSlice + phiMovement),
                radius * cos(currentStack + thetaMovement),
                radius * sin(currentStack + thetaMovement) * cos(currentSlice + phiMovement)));
        }

    return coordsSphere;
}

std::vector<Point> draw_cone(double radius, double height, int slices, int stacks)
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

std::vector<Point> draw_cylinder(double radius, double height, int slices)
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

std::vector<Point> draw_torus(double innerRadius, double outerRadius, int sides, int rings)
{
    vector<Point> coords;
    const double alpha_step = (2 * M_PI) / rings;
    const double beta_step = (2 * M_PI) / sides;
    double alpha = 0;
    double radius = outerRadius - innerRadius;
    for (int r = 0; r < rings; r++) {
        double beta = 0;
        double ring_x = radius * cos(beta) * sin(alpha);
        double ring_y = radius * cos(beta) * cos(alpha);
        double ring_z = 0;
        double next_ring_x = radius * cos(beta) * sin(alpha + alpha_step);
        double next_ring_y = radius * cos(beta) * cos(alpha + alpha_step);
        double next_ring_z = 0;
        for (int s = 0; s < sides; s++) {
            Point p0 = Point(
                    ring_x + (innerRadius * cos(beta) * sin(alpha)),
                    ring_y + (innerRadius * cos(beta) * cos(alpha)),
                    ring_z + (innerRadius * sin(beta))
                    );
            Point p1 = Point(
                    next_ring_x + (innerRadius * cos(beta) * sin(alpha + alpha_step)),
                    next_ring_y + (innerRadius * cos(beta) * cos(alpha + alpha_step)),
                    next_ring_z + (innerRadius * sin(beta))
                    );
            Point p2 = Point(
                    ring_x + (innerRadius * cos(beta + beta_step) * sin(alpha)),
                    ring_y + (innerRadius * cos(beta + beta_step) * cos(alpha)),
                    ring_z + (innerRadius * sin(beta + beta_step))
                    );
            Point p3 = Point(
                    next_ring_x + (innerRadius * cos(beta + beta_step) * sin(alpha + alpha_step)),
                    next_ring_y + (innerRadius * cos(beta + beta_step) * cos(alpha + alpha_step)),
                    next_ring_z + (innerRadius * sin(beta + beta_step))
                    );
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
