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
    float x, y, z, normal_x, normal_y, normal_z, texture_x, texture_y;
    ifstream infile(modelFile);
    if (!infile.good()) {
        string error = "Couldn't load '";
        error.append(modelFile);
        error.append("': No such file or directory");
        throw error;
    }
    while (infile >> x >> y >> z >> normal_x >> normal_y >> normal_z >> texture_x >> texture_y) {
        push(x, y, z, normal_x, normal_y, normal_z, texture_x, texture_y);
    }
    n_vertices = vbo.size() / 3;
}

void Model::draw()
{
    if (!prepared) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);
        prepared = true;
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
}

inline void Model::push(
    float x,
    float y,
    float z,
    float normal_x,
    float normal_y,
    float normal_z,
    float texture_x,
    float texture_y)
{
    vbo.push_back(x);
    vbo.push_back(y);
    vbo.push_back(z);
    normals.push_back(normal_x);
    normals.push_back(normal_y);
    normals.push_back(normal_z);
    texture_coords.push_back(texture_x);
    texture_coords.push_back(texture_y);
}
