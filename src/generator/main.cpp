#include "generator.hpp"
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

static char* PROGRAM_NAME;

#define PARSE_OR_RETURN(a, t, h) \
    if (a == t) {                \
        h();                     \
        return false;            \
    }

void printHelpPlane()
{
    cerr << "Invalid args, plane requires: side_length" << endl;
}

void printHelpBox()
{
    cerr << "Invalid args, box requires: x y z [divisions]" << endl;
}

void printHelpSphere()
{
    cerr << "Invalid args, sphere requires: radius slices stacks" << endl;
}

void printHelpCone()
{
    cerr << "Invalid args, cone requires: radius height slices stacks" << endl;
}

void printHelpPage()
{
    cerr << "Usage: " << PROGRAM_NAME << " fileName Option [Params ...]" << endl
         << "Options:" << endl
         << "\tplane" << endl
         << "\tbox" << endl
         << "\tsphere" << endl
         << "\tcone" << endl
         << "\nUse --help Option to learn what params to pass to the options" << endl;
}

void write_to_file(char* fileName, vector<Point>* points)
{
    ofstream file(fileName);
    for (vector<Point>::const_iterator it = points->begin(); it != points->end(); ++it) {
        file << (*it).to_string() << endl;
    }
}

bool parse_plane(int argc, char** args, double* side_length)
{
    if (argc < 1) {
        printHelpPlane();
        return false;
    }
    char* test;
    *side_length = strtod(*args, &test);
    PARSE_OR_RETURN(*args, test, printHelpPlane);
    return true;
}

bool parse_box(int argc, char** args, double* x, double* y, double* z, int* divisions)
{
    if (argc < 3) {
        printHelpBox();
        return false;
    }
    char* test;
    *x = strtod(args[0], &test);
    PARSE_OR_RETURN(args[0], test, printHelpPlane)
    *y = strtod(args[1], &test);
    PARSE_OR_RETURN(args[1], test, printHelpPlane)
    *z = strtod(args[2], &test);
    PARSE_OR_RETURN(args[3], test, printHelpPlane)
    *divisions = strtod(args[3], &test);
    if (test == args[3]) {
        *divisions = 1;
    }
    return true;
}

bool parse_sphere(int argc, char** args, double* radius, double* slices, double* stacks)
{
    if (argc < 3) {
        printHelpSphere();
        return false;
    }
    char* test;
    *radius = strtod(args[0], &test);
    PARSE_OR_RETURN(args[0], test, printHelpSphere);
    *slices = strtod(args[1], &test);
    PARSE_OR_RETURN(args[1], test, printHelpSphere);
    *stacks = strtod(args[2], &test);
    PARSE_OR_RETURN(args[2], test, printHelpSphere);
    return true;
}

bool parse_cone(int argc, char** args, double* radius, double* height, int* slices, int* stacks)
{
    if (argc < 4) {
        printHelpCone();
        return 1;
    }
    char* test;
    *radius = strtod(args[0], &test);
    PARSE_OR_RETURN(args[0], test, printHelpCone);
    *height = strtod(args[1], &test);
    PARSE_OR_RETURN(args[1], test, printHelpCone);
    *slices = strtol(args[2], &test, 10);
    PARSE_OR_RETURN(args[2], test, printHelpCone);
    *stacks = strtol(args[3], &test, 10);
    PARSE_OR_RETURN(args[3], test, printHelpCone);
    return true;
}

int main(int argc, char** argv)
{
    PROGRAM_NAME = argv[0];
    if (argc < 2) {
        printHelpPage();
        return 1;
    }
    vector<Point> points;
    if (!strcmp("plane", argv[2])) {
        double side_length;
        if (parse_plane(argc - 3, argv + 3, &side_length)) {
            points = draw_plane(side_length);
        } else {
            return 1;
        }
    } else if (!strcmp("box", argv[2])) {
        double x, y, z;
        int divisions;
        if (parse_box(argc - 3, argv + 3, &x, &y, &z, &divisions)) {
            points = draw_box(x, y, z, divisions);
        } else {
            return 1;
        }
    } else if (!strcmp("sphere", argv[2])) {
        double radius, slices, stacks;
        if (parse_sphere(argc - 3, argv + 3, &radius, &slices, &stacks)) {
            points = draw_sphere(radius, slices, stacks);
        } else {
            return 1;
        }
    } else if (!strcmp("cone", argv[2])) {
        double radius = 0, height = 0;
        int slices = 0, stacks = 0;
        if (parse_cone(argc - 3, argv + 3, &radius, &height, &slices, &stacks)) {
            points = draw_cone(radius, height, slices, stacks);
        } else {
            return 1;
        }
    } else {
        printHelpPage();
        return 1;
    }
    write_to_file(argv[1], &points);
    points.clear();
    return 0;
}
