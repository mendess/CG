#include "engine/model.hpp"
#include "dependencies/rapidxml.hpp"

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

GLuint Textures::load_texture(string texture)
{
    if (textures.find(texture) != textures.end()) {
        return textures[texture];
    }
    unsigned int t, tw, th;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)texture.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    unsigned char* texData = ilGetData();

    GLuint texture_slot;

    glGenTextures(1, &texture_slot);
    glBindTexture(GL_TEXTURE_2D, texture_slot);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    textures[texture] = texture_slot;
    return texture_slot;
}

ModelBuffer::ModelBuffer(string modelFile)
{
    vector<float> vbo;
    vector<float> normals;
    vector<float> texture_coords;
    ifstream infile(modelFile);
    if (!infile.good()) {
        string error = "Couldn't load '";
        error.append(modelFile);
        error.append("': No such file or directory");
        throw error;
    }
    float x, y, z, normal_x, normal_y, normal_z, texture_x, texture_y;
    while (infile >> x >> y >> z >> normal_x >> normal_y >> normal_z >> texture_x >> texture_y) {
        vbo.push_back(x);
        vbo.push_back(y);
        vbo.push_back(z);
        normals.push_back(normal_x);
        normals.push_back(normal_y);
        normals.push_back(normal_z);
        texture_coords.push_back(texture_x);
        texture_coords.push_back(texture_y);
    }
    _n_vertices = vbo.size() / 3;
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer());
    glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(float), texture_coords.data(), GL_STATIC_DRAW);
}

std::unique_ptr<ModelBuffer>& ModelBuffer::get(string modelFile)
{
    if (modelBuffers.find(modelFile) == modelBuffers.end()) {
        modelBuffers[modelFile] = make_unique<ModelBuffer>(modelFile);
    }
    return modelBuffers[modelFile];
}

ColoredModel::ColoredModel(string modelFile, RGBA diffuse, RGBA specular, RGBA emissive, RGBA ambient)
    : diffuse(diffuse)
    , specular(specular)
    , emissive(emissive)
    , ambient(ambient)
{
    auto& model = ModelBuffer::get(modelFile);
    n_vertices = model->n_vertices();
    buffers[0] = model->vbo_buffer();
    buffers[1] = model->normal_buffer();
}

void ColoredModel::draw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.to_array().data());

        glMaterialfv(GL_FRONT, GL_SPECULAR, specular.to_array().data());

        glMaterialfv(GL_FRONT, GL_EMISSION, emissive.to_array().data());

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.to_array().data());

        glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer());
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer());
        glNormalPointer(GL_FLOAT, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, n_vertices);
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

TexturedModel::TexturedModel(
    string modelFile,
    string texture_file,
    RGBA diffuse,
    RGBA specular,
    RGBA emissive,
    RGBA ambient)
    : texture_file(texture_file)
    , diffuse(diffuse)
    , specular(specular)
    , emissive(emissive)
    , ambient(ambient)
{
    auto& model = ModelBuffer::get(modelFile);
    n_vertices = model->n_vertices();
    buffers[0] = model->vbo_buffer();
    buffers[1] = model->normal_buffer();
    buffers[2] = model->texture_buffer();
    texture_slot = Textures::load_texture(texture_file);
}

void TexturedModel::draw() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.to_array().data());

        glMaterialfv(GL_FRONT, GL_SPECULAR, specular.to_array().data());

        glMaterialfv(GL_FRONT, GL_EMISSION, emissive.to_array().data());

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.to_array().data());

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
