#include "model.hpp"
#include "../dependencies/rapidxml.hpp"

#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
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
    while (infile >> x >> y >> z) {
        points.push_back(new Point(x, y, z));
    }
    modelName = modelFile;
}

void drawTriangle(Point* a, Point* b, Point* c)
{
    static default_random_engine generator;
    static uniform_int_distribution<int> distribution(0, 100);
    static auto rng = bind(distribution, generator);
    float r = (((float)rng()) / 100.0f);
    float g = (((float)rng()) / 100.0f);
    float u = (((float)rng()) / 100.0f);
    glColor3f(r, g, u);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a->x(), a->y(), a->z());
        glVertex3f(b->x(), b->y(), b->z());
        glVertex3f(c->x(), c->y(), c->z());
    }
    glEnd();
}

bool Model::draw_model() const
{
    if (points.size() % 3 != 0) {
        return false;
    }
    vector<Point*>::const_iterator it = points.begin();
    while (it != points.end()) {
        Point* a = *it++;
        Point* b = *it++;
        Point* c = *it++;
        drawTriangle(a, b, c);
    }
    return true;
}

using namespace rapidxml;

void Models::load(string config)
{
    ifstream file(config);
    if (!file.good()) {
        cerr << config << ": file not found" << endl;
        return;
    }
    string text;
    file.seekg(0, ios::end);
    text.reserve(file.tellg());
    file.seekg(0, ios::beg);

    text.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    xml_document<> doc;
    doc.parse<0>(strdup(text.data()));
    xml_node<>* node = doc.first_node("scene");
    for (node = node->first_node(); node; node = node->next_sibling()) {
        xml_attribute<>* attr = node->first_attribute();
        Models::get()->add_model(Model(attr->value()));
    }
}
