#include "box.hpp"
#include <stdexcept>

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

std::vector<Point> Box::draw() const
{
    vector<Point> coordsBox;
    coordsBox.reserve(6 * 6 * divisions * divisions);
    const float axisX = x / 2;
    const float axisY = y / 2;
    const float axisZ = z / 2;
    const float spacingX = x / divisions;
    const float spacingY = y / divisions;
    const float spacingZ = z / divisions;

    for (int i = 0; i < divisions; i++) {
        for (int k = 0; k < divisions; k++) {
            const float currentX = -axisX + spacingX * i;
            const float nextX    = -axisX + spacingX * (i + 1);
            const float currentY = -axisY + spacingY * k;
            const float nextY    = -axisY + spacingY * (k + 1);
            const float currentZ = -axisZ + spacingZ * k;
            const float nextZ    = -axisZ + spacingZ * (k + 1);

            //Front
            coordsBox.push_back(Point(currentX, currentY, axisZ));
            coordsBox.push_back(Point(nextX, currentY, axisZ));
            coordsBox.push_back(Point(currentX, nextY, axisZ));
            coordsBox.push_back(Point(nextX, currentY, axisZ));
            coordsBox.push_back(Point(nextX, nextY, axisZ));
            coordsBox.push_back(Point(currentX, nextY, axisZ));

            //Back
            coordsBox.push_back(Point(currentX, currentY, -axisZ));
            coordsBox.push_back(Point(currentX, nextY, -axisZ));
            coordsBox.push_back(Point(nextX, currentY, -axisZ));
            coordsBox.push_back(Point(nextX, currentY, -axisZ));
            coordsBox.push_back(Point(currentX, nextY, -axisZ));
            coordsBox.push_back(Point(nextX, nextY, -axisZ));

            //Up
            coordsBox.push_back(Point(currentX, axisY, currentZ));
            coordsBox.push_back(Point(currentX, axisY, nextZ));
            coordsBox.push_back(Point(nextX, axisY, nextZ));
            coordsBox.push_back(Point(currentX, axisY, currentZ));
            coordsBox.push_back(Point(nextX, axisY, nextZ));
            coordsBox.push_back(Point(nextX, axisY, currentZ));

            //Down
            coordsBox.push_back(Point(currentX, -axisY, currentZ));
            coordsBox.push_back(Point(nextX, -axisY, nextZ));
            coordsBox.push_back(Point(currentX, -axisY, nextZ));
            coordsBox.push_back(Point(currentX, -axisY, currentZ));
            coordsBox.push_back(Point(nextX, -axisY, currentZ));
            coordsBox.push_back(Point(nextX, -axisY, nextZ));

            //Left
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, currentZ));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, nextZ));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), currentZ));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), currentZ));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * i, nextZ));
            coordsBox.push_back(Point(-axisX, -axisY + spacingY * (i + 1), nextZ));

            //Right
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, currentZ));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), currentZ));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, nextZ));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), currentZ));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * (i + 1), nextZ));
            coordsBox.push_back(Point(axisX, -axisY + spacingY * i, nextZ));
        }
    }

    return coordsBox;
}
