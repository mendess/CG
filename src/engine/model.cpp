#include "model.hpp"
#include "../dependencies/rapidxml.hpp"

#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

void ModelBuilder::set_r(optional<RGB>& rgb, float val)
{
    simple = false;
    if (!rgb) {
        rgb = RGB();
    }
    rgb->r = val;
}

void ModelBuilder::set_g(optional<RGB>& rgb, float val)
{
    simple = false;
    if (!rgb) {
        rgb = RGB();
    }
    rgb->g = val;
}

void ModelBuilder::set_b(optional<RGB>& rgb, float val)
{
    simple = false;
    if (!rgb) {
        rgb = RGB();
    }
    rgb->b = val;
}

ModelBuilder& ModelBuilder::withFile(string file)
{
    this->file = file;
    return *this;
}

ModelBuilder& ModelBuilder::withTexture(string texture_file)
{
    simple = false;
    this->texture_file = texture_file;
    return *this;
}

ModelBuilder& ModelBuilder::withDiffuseR(float val)
{
    set_r(diffuse, val);
    return *this;
}

ModelBuilder& ModelBuilder::withDiffuseG(float val)
{
    set_g(diffuse, val);
    return *this;
}

ModelBuilder& ModelBuilder::withDiffuseB(float val)
{
    set_b(diffuse, val);
    return *this;
}

ModelBuilder& ModelBuilder::withSpecularR(float val)
{
    set_r(specular, val);
    return *this;
}

ModelBuilder& ModelBuilder::withSpecularG(float val)
{
    set_g(specular, val);
    return *this;
}

ModelBuilder& ModelBuilder::withSpecularB(float val)
{
    set_b(specular, val);
    return *this;
}

ModelBuilder& ModelBuilder::withEmissiveR(float val)
{
    set_r(emissive, val);
    return *this;
}

ModelBuilder& ModelBuilder::withEmissiveG(float val)
{
    set_g(emissive, val);
    return *this;
}

ModelBuilder& ModelBuilder::withEmissiveB(float val)
{
    set_b(emissive, val);
    return *this;
}

ModelBuilder& ModelBuilder::withAmbientR(float val)
{
    set_r(ambient, val);
    return *this;
}

ModelBuilder& ModelBuilder::withAmbientG(float val)
{
    set_g(ambient, val);
    return *this;
}

ModelBuilder& ModelBuilder::withAmbientB(float val)
{
    set_b(ambient, val);
    return *this;
}

unique_ptr<Model> ModelBuilder::build() const
{
    if (!file) {
        throw invalid_argument("No model file");
    } else if (simple) {
        return make_unique<SimpleModel>(*file);
    } else if (texture_file) {
        return make_unique<TexturedModel>(*file, *texture_file);
    } else {
        return make_unique<ColoredModel>(*file, *diffuse, *specular, *emissive, *ambient);
    }
}

SimpleModel::SimpleModel(string modelFile)
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
    buffer = 0;
}

#include <iostream>

void SimpleModel::prepare()
{
    glGenBuffers(1, &buffer);
    cout << "Generated buffer in: " << buffer << endl;

    cout << "Binding buffer: " << buffer << endl;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    cout << "Buffering data os size: " << vbo.size() * sizeof(float) << endl;
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);
}

void SimpleModel::draw() const
{
    cout << "Binding buffer: " << buffer << endl;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    cout << "Giving buffer semantics" << endl;
    glVertexPointer(3, GL_FLOAT, 0, 0);
    cout << "Drawing arrays" << endl;
    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
}

inline void SimpleModel::push(
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
}

ColoredModel::ColoredModel(string modelFile, RGB diffuse, RGB specular, RGB emissive, RGB ambient)
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
    this->diffuse = diffuse;
    this->specular = specular;
    this->emissive = emissive;
    this->ambient = ambient;
}

void ColoredModel::prepare()
{
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
}

void ColoredModel::draw() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
    glNormalPointer(GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
}

inline void ColoredModel::push(
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
}

TexturedModel::TexturedModel(string modelFile, string texture_file)
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

void TexturedModel::prepare()
{
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer());
    glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(float), texture_coords.data(), GL_STATIC_DRAW);
}

void TexturedModel::draw() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer());
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
}

inline void TexturedModel::push(
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
