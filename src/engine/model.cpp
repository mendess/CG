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
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

Model::Model(char* modelFile)
    : prepared(false)
{
    float x, y, z;
    ifstream infile(modelFile);
    if (!infile.good()) {
        modelName = NULL;
        return;
    }
    modelName = strdup(modelFile);
    while (infile >> x >> y >> z) {
        push(x, y, z);
    }
    infile.close();
    n_vertices = vbo.size() / 3;
    buffer[0] = 0;
}

Model::Model(const Model& other)
    : vbo(other.vbo)
    , modelName(strdup(other.modelName))
    , n_vertices(other.n_vertices)
    , prepared(other.prepared)
{
    this->buffer[0] = other.buffer[0];
}

bool Model::loaded()
{
    return modelName != NULL;
}

void Model::prepare()
{
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);
    prepared = true;
}

bool Model::draw()
{
    if (!prepared) {
        prepare();
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
    return true;
}

void Model::push(float x, float y, float z)
{
    vbo.push_back(x);
    vbo.push_back(y);
    vbo.push_back(z);
}
