#include "box.hpp"
#include <stdexcept>
#include <tuple>

using namespace std;

Box::Box(int argc, char** args)
{

    if (argc < 3) {
        throw invalid_argument("Not enough arguments");
    }
    x = stod(args[0]);
    y = stod(args[1]);
    z = stod(args[2]);
    if (args[3]) {
        divisions = stoi(args[3]);
    } else {
        divisions = 1;
    }
}

tuple<float, float> xyToTexture(float x, float y, float maxX, float maxY, float x_offset, float y_offset)
{
    return { (x * (1 / 3) / maxX) + x_offset, (y * 0.5 / maxY) + y_offset };
}

std::vector<Point> Box::draw() const
{
    vector<Point> coordsBox;
    coordsBox.reserve(6 * 6 * divisions * divisions);
    const float halfX = x / 2;
    const float halfY = y / 2;
    const float halfZ = z / 2;
    const float spacingX = x / divisions;
    const float spacingY = y / divisions;
    const float spacingZ = z / divisions;
    for (int i = 0; i < divisions; i++) {
        for (int k = 0; k < divisions; k++) {
            const float currentX = -halfX + spacingX * i;
            const float nextX = -halfX + spacingX * (i + 1);
            const float currentY = -halfY + spacingY * k;
            const float nextY = -halfY + spacingY * (k + 1);
            const float currentZ = -halfZ + spacingZ * k;
            const float nextZ = -halfZ + spacingZ * (k + 1);
            const Vector frontV = Vector(0, 0, 1);
            const Vector backV = Vector(0, 0, -1);
            const Vector upV = Vector(0, 1, 0);
            const Vector downV = Vector(0, -1, 0);
            const Vector leftV = Vector(-1, 0, 0);
            const Vector rightV = Vector(1, 0, 0);
            //Front
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        ((p.x() + halfX) * 0.33) / x,
                        ((p.y() + halfY) * 0.5) / y);
                };
                coordsBox.push_back(f(Point(currentX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(currentX, nextY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, nextY, halfZ, frontV)));
                coordsBox.push_back(f(Point(currentX, nextY, halfZ, frontV)));
            }
            //Back
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        ((p.x() + halfX) * 0.33) / x,
                        (.5 + (p.y() + halfY) * 0.5) / y);
                };
                coordsBox.push_back(f(Point(currentX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(currentX, nextY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(currentX, nextY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, nextY, -halfZ, backV)));
            }
            //Up
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        (1.f / 3.f) + ((p.x() + halfX) * 0.33) / x,
                        0 + ((p.z() + halfZ) * 0.5) / z);
                };
                coordsBox.push_back(f(Point(currentX, halfY, currentZ, upV)));
                coordsBox.push_back(f(Point(currentX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(currentX, halfY, currentZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, currentZ, upV)));
            }
            //Down
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        (1.f / 3.f) + ((p.x() + halfX) * 0.33) / x,
                        .5f + ((p.z() + halfZ) * 0.5) / z);
                };
                coordsBox.push_back(f(Point(currentX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, nextZ, downV)));
                coordsBox.push_back(f(Point(currentX, -halfY, nextZ, downV)));
                coordsBox.push_back(f(Point(currentX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, nextZ, downV)));
            }
            //Left
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        (2.f / 3.f) + ((p.y() + halfY) * 0.33) / y,
                        ((p.z() + halfZ) * 0.5) / z);
                };
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), nextZ, leftV)));
            }
            //Right
            {
                const auto f = [=](Point p) {
                    return p.set_texture(
                        (2.f / 3.f) + ((p.y() + halfY) * 0.33) / y,
                        .5f + ((p.z() + halfZ) * 0.5) / z);
                };
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), nextZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)));
            }
        }
    }
    return coordsBox;
}
