#ifndef MODEL_H
#define MODEL_H

#include "../common/point.hpp"
#include "rgb.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class Model {
public:
    virtual void prepare() = 0;
    virtual void draw() const = 0;
};

class SimpleModel : public Model {
private:
    GLuint buffer;
    std::vector<float> vbo;
    size_t n_vertices;

    void push(float, float, float);

public:
    SimpleModel(std::string);
    void prepare();
    void draw() const;
};

class ColoredModel : public Model {
private:
    GLuint buffers[2];
    std::vector<float> vbo;
    std::vector<float> normals;
    RGB diffuse;
    RGB specular;
    RGB emissive;
    RGB ambient;
    size_t n_vertices;

    void push(float, float, float, float, float, float);

    inline GLuint vbo_buffer() const { return buffers[0]; }
    inline GLuint normal_buffer() const { return buffers[1]; }

public:
    ColoredModel(std::string, RGB, RGB, RGB, RGB);
    void prepare();
    void draw() const;
};

class TexturedModel : public Model {
private:
    GLuint buffers[3];
    GLuint texture_slot;
    std::string texture_file;
    std::vector<float> vbo;
    std::vector<float> normals;
    std::vector<float> texture_coords;
    RGB diffuse;
    RGB specular;
    RGB emissive;
    RGB ambient;
    size_t n_vertices;

    void push(float, float, float, float, float, float, float, float);

    inline GLuint vbo_buffer() const { return buffers[0]; }
    inline GLuint normal_buffer() const { return buffers[1]; }
    inline GLuint texture_buffer() const { return buffers[2]; }

public:
    TexturedModel(std::string, std::string, RGB, RGB, RGB, RGB);
    void prepare();
    void draw() const;
};

#endif //MODEL_H
