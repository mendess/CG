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
#include <IL/il.h>

using namespace std;

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
        push(x, y, z);
    }
    n_vertices = vbo.size() / 3;
    buffer = 0;
}

#include <iostream>

void SimpleModel::prepare()
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);
}

void SimpleModel::draw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, n_vertices);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

inline void SimpleModel::push(
    float x,
    float y,
    float z)
{
    vbo.push_back(x);
    vbo.push_back(y);
    vbo.push_back(z);
}

ColoredModel::ColoredModel(string modelFile, RGB diffuse, RGB specular, RGB emissive, RGB ambient)
    : diffuse(diffuse)
    , specular(specular)
    , emissive(emissive)
    , ambient(ambient)
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
        push(x, y, z, normal_x, normal_y, normal_z);
    }
    n_vertices = vbo.size() / 3;
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
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    {
        float diffuse_arr[] = { diffuse.r, diffuse.g, diffuse.b, 1 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_arr);

        float specular_arr[] = { specular.r, specular.g, specular.b, 1 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_arr);

        float emissive_arr[] = { emissive.r, emissive.g, emissive.b, 1 };
        glMaterialfv(GL_FRONT, GL_EMISSION, emissive_arr);

        float ambient_arr[] = { ambient.r, ambient.g, ambient.b, 1 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_arr);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
        glNormalPointer(GL_FLOAT, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, n_vertices);
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

inline void ColoredModel::push(
    float x,
    float y,
    float z,
    float normal_x,
    float normal_y,
    float normal_z)
{
    vbo.push_back(x);
    vbo.push_back(y);
    vbo.push_back(z);
    normals.push_back(normal_x);
    normals.push_back(normal_y);
    normals.push_back(normal_z);
}

TexturedModel::TexturedModel(
    string modelFile,
    string texture_file,
    RGB diffuse,
    RGB specular,
    RGB emissive,
    RGB ambient)
    : texture_file(texture_file)
    , diffuse(diffuse)
    , specular(specular)
    , emissive(emissive)
    , ambient(ambient)
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

    unsigned int t, tw, th;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)texture_file.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    unsigned char* texData = ilGetData();

    glGenTextures(1, &texture_slot);
    glBindTexture(GL_TEXTURE_2D, texture_slot);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
}

void TexturedModel::draw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    {
        float diffuse_arr[] = { diffuse.r, diffuse.g, diffuse.b, 1 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_arr);

        float specular_arr[] = { specular.r, specular.g, specular.b, 1 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_arr);

        float emissive_arr[] = { emissive.r, emissive.g, emissive.b, 1 };
        glMaterialfv(GL_FRONT, GL_EMISSION, emissive_arr);

        float ambient_arr[] = { ambient.r, ambient.g, ambient.b, 1 };
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_arr);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
        glNormalPointer(GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, texture_buffer());
        glTexCoordPointer(2, GL_FLOAT, 0, 0);

        glBindTexture(GL_TEXTURE_2D, texture_slot);
        glDrawArrays(GL_TRIANGLES, 0, n_vertices);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
