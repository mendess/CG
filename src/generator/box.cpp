#include "box.hpp"
#include <stdexcept>
#include <tuple>

#include <iostream>

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
                cout << "Front" << endl;
                const auto f = [=](Point p) { return p.setXYtoTexture(halfX, halfY, 0, 0); };
                coordsBox.push_back(f(Point(currentX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(currentX, nextY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, currentY, halfZ, frontV)));
                coordsBox.push_back(f(Point(nextX, nextY, halfZ, frontV)));
                coordsBox.push_back(f(Point(currentX, nextY, halfZ, frontV)));
                cout << f(Point(currentX, currentY, halfZ, frontV)).to_string() << endl;
                cout << f(Point(nextX, currentY, halfZ, frontV)).to_string() << endl;
                cout << f(Point(currentX, nextY, halfZ, frontV)).to_string() << endl;
                cout << f(Point(nextX, currentY, halfZ, frontV)).to_string() << endl;
                cout << f(Point(nextX, nextY, halfZ, frontV)).to_string() << endl;
                cout << f(Point(currentX, nextY, halfZ, frontV)).to_string() << endl;
            }
            //Back
            {
                cout << "Back" << endl;
                const auto f = [=](Point p) { return p.setXYtoTexture(halfX, halfY, 0, .5f); };
                coordsBox.push_back(f(Point(currentX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(currentX, nextY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, currentY, -halfZ, backV)));
                coordsBox.push_back(f(Point(currentX, nextY, -halfZ, backV)));
                coordsBox.push_back(f(Point(nextX, nextY, -halfZ, backV)));
                cout << f(Point(currentX, currentY, -halfZ, backV)).to_string() << endl;
                cout << f(Point(currentX, nextY, -halfZ, backV)).to_string() << endl;
                cout << f(Point(nextX, currentY, -halfZ, backV)).to_string() << endl;
                cout << f(Point(nextX, currentY, -halfZ, backV)).to_string() << endl;
                cout << f(Point(currentX, nextY, -halfZ, backV)).to_string() << endl;
                cout << f(Point(nextX, nextY, -halfZ, backV)).to_string() << endl;
            }
            //Up
            {
                cout << "Up" << endl;
                const auto f = [=](Point p) { return p.setXZtoTexture(halfX, halfZ, 1.f / 3.f, 0); };
                coordsBox.push_back(f(Point(currentX, halfY, currentZ, upV)));
                coordsBox.push_back(f(Point(currentX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(currentX, halfY, currentZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, nextZ, upV)));
                coordsBox.push_back(f(Point(nextX, halfY, currentZ, upV)));
                cout << f(Point(currentX, halfY, currentZ, upV)).to_string() << endl;
                cout << f(Point(currentX, halfY, nextZ, upV)).to_string() << endl;
                cout << f(Point(nextX, halfY, nextZ, upV)).to_string() << endl;
                cout << f(Point(currentX, halfY, currentZ, upV)).to_string() << endl;
                cout << f(Point(nextX, halfY, nextZ, upV)).to_string() << endl;
                cout << f(Point(nextX, halfY, currentZ, upV)).to_string() << endl;
            }
            //Down
            {
                cout << "Down" << endl;
                const auto f = [=](Point p) { return p.setXZtoTexture(halfX, halfZ, 1.f / 3.f, .5f); };
                coordsBox.push_back(f(Point(currentX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, nextZ, downV)));
                coordsBox.push_back(f(Point(currentX, -halfY, nextZ, downV)));
                coordsBox.push_back(f(Point(currentX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, currentZ, downV)));
                coordsBox.push_back(f(Point(nextX, -halfY, nextZ, downV)));
                cout << f(Point(currentX, -halfY, currentZ, downV)).to_string() << endl;
                cout << f(Point(nextX, -halfY, nextZ, downV)).to_string() << endl;
                cout << f(Point(currentX, -halfY, nextZ, downV)).to_string() << endl;
                cout << f(Point(currentX, -halfY, currentZ, downV)).to_string() << endl;
                cout << f(Point(nextX, -halfY, currentZ, downV)).to_string() << endl;
                cout << f(Point(nextX, -halfY, nextZ, downV)).to_string() << endl;
            }
            //Left
            {
                cout << "Left" << endl;
                const auto f = [=](Point p) { return p.setYZtoTexture(halfY, halfY, 2.f / 3.f, 0); };
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)));
                coordsBox.push_back(f(Point(-halfX, -halfY + spacingY * (i + 1), nextZ, leftV)));
                cout << f(Point(-halfX, -halfY + spacingY * i, currentZ, leftV)).to_string() << endl;
                cout << f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)).to_string() << endl;
                cout << f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)).to_string() << endl;
                cout << f(Point(-halfX, -halfY + spacingY * (i + 1), currentZ, leftV)).to_string() << endl;
                cout << f(Point(-halfX, -halfY + spacingY * i, nextZ, leftV)).to_string() << endl;
                cout << f(Point(-halfX, -halfY + spacingY * (i + 1), nextZ, leftV)).to_string() << endl;
            }
            //Right
            {
                cout << "Right" << endl;
                const auto f = [=](Point p) { return p.setYZtoTexture(halfY, halfZ, 2.f / 3.f, .5f); };
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * (i + 1), nextZ, rightV)));
                coordsBox.push_back(f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)));
                cout << f(Point(halfX, -halfY + spacingY * i, currentZ, rightV)).to_string() << endl;
                cout << f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)).to_string() << endl;
                cout << f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)).to_string() << endl;
                cout << f(Point(halfX, -halfY + spacingY * (i + 1), currentZ, rightV)).to_string() << endl;
                cout << f(Point(halfX, -halfY + spacingY * (i + 1), nextZ, rightV)).to_string() << endl;
                cout << f(Point(halfX, -halfY + spacingY * i, nextZ, rightV)).to_string() << endl;
            }
        }
    }
    return coordsBox;
}
