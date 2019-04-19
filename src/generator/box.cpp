#include "box.hpp"
#include <stdexcept>

using namespace std;

const string Box::help_message = "box requires: x y z [divisions]";

Box::Box(int argc, char** args)
{

    if (argc < 3) {
        throw invalid_argument("Not enough arguments");
    }
    x = stod(args[0]);
    y = stod(args[1]);
    z = stod(args[2]);
    if (args[3])
        divisions = stoi(args[3]);
    else
        divisions = 1;
}

std::vector<Point> Box::draw() const
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
