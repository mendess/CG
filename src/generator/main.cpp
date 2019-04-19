#include "box.hpp"
#include "cone.hpp"
#include "cylinder.hpp"
#include "patches.hpp"
#include "plane.hpp"
#include "primitives.hpp"
#include "sphere.hpp"
#include "torus.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

static char* PROGRAM_NAME;

#define perror_and_exit(e, help_message)               \
    cerr << "Invalid args, " << help_message << endl; \
    cerr << e.what() << endl;                         \
    return 1;

void printHelpPage()
{
    cerr << "Usage: " << PROGRAM_NAME << " fileName Option [Params ...]" << endl
         << "Options:" << endl
         << "\tplane" << endl
         << "\tbox" << endl
         << "\tsphere" << endl
         << "\tcone" << endl
         << "\tcylinder" << endl
         << "\ttorus" << endl
         << "\tpatch" << endl
         << "\nUse --help Option to learn what params to pass to the options" << endl;
}

void write_to_file(char* fileName, vector<Point> points)
{
    ofstream file(fileName);
    for (vector<Point>::const_iterator it = points.begin(); it != points.end(); ++it) {
        file << (*it).to_string() << endl;
    }
}

int main(int argc, char** argv)
{
    PROGRAM_NAME = argv[0];
    if (argc < 3 || "--help" == string(argv[1]) || "-h" == string(argv[1])) {
        printHelpPage();
        return 1;
    }
    Primitive* p;
    string kind(argv[2]);
    if ("plane" == kind) {
        try {
            p = new Plane(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Plane::help_message)
        }
    } else if ("box" == kind) {
        try {
            p = new Box(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Box::help_message)
        }
    } else if ("sphere" == kind) {
        try {
            p = new Sphere(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Sphere::help_message)
        }
    } else if ("cone" == kind) {
        try {
            p = new Cone(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Cone::help_message)
        }
    } else if ("cylinder" == kind) {
        try {
            p = new Cylinder(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Cylinder::help_message)
        }
    } else if ("torus" == kind) {
        try {
            p = new Torus(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Torus::help_message)
        }
    } else if ("patch" == kind) {
        try {
            p = new Patches(argc - 3, argv + 3);
        } catch (invalid_argument e) {
            perror_and_exit(e, Patches::help_message)
        }
    } else {
        printHelpPage();
        return 1;
    }
    write_to_file(argv[1], p->draw());
    delete p;
    return 0;
}
