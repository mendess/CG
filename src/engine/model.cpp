#include "model.hpp"
#include "../dependencies/rapidxml.hpp"

#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

Model::Model(char* modelFile)
{
    float x, y, z;
    ifstream infile(modelFile);
    if(!infile.good()) {
        modelName = NULL;
        return;
    }
    while (infile >> x >> y >> z) {
        points.push_back(Point(x, y, z));
    }
    modelName = strdup(modelFile);
    infile.close();
}

Model::Model(const Model& other)
    : points(other.points)
    , modelName(strdup(other.modelName))
{
}

bool Model::loaded() {
    return modelName != NULL;
}

void drawTriangle(const Point* a, const Point* b, const Point* c)
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a->x(), a->y(), a->z());
        glVertex3f(b->x(), b->y(), b->z());
        glVertex3f(c->x(), c->y(), c->z());
    }
    glEnd();
}

bool Model::draw() const
{
    if (points.size() % 3 != 0) {
        return false;
    }
    vector<Point>::const_iterator it = points.begin();
    while (it != points.end()) {
        const Point* a = &*it++;
        const Point* b = &*it++;
        const Point* c = &*it++;
        drawTriangle(a, b, c);
    }
    return true;
}
