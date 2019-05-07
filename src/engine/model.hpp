#ifndef MODEL_H
#define MODEL_H

#include "../common/point.hpp"

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

class RGB {
public:
    float r, g, b;
    RGB()
        : r(0)
        , g(0)
        , b(0)
    {
    }
    RGB(float r, float g, float b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
};

class Model {
public:
    virtual void prepare() = 0;
    virtual void draw() const = 0;
};

class ModelBuilder {
private:
    std::optional<std::string> file;
    std::optional<std::string> texture_file;
    std::optional<RGB> diffuse;
    std::optional<RGB> specular;
    std::optional<RGB> emissive;
    std::optional<RGB> ambient;
    bool simple;
    void set_r(std::optional<RGB>& rgb, float val);
    void set_g(std::optional<RGB>& rgb, float val);
    void set_b(std::optional<RGB>& rgb, float val);

public:
    ModelBuilder()
        : file(std::nullopt)
        , texture_file(std::nullopt)
        , diffuse(std::nullopt)
        , specular(std::nullopt)
        , emissive(std::nullopt)
        , ambient(std::nullopt)
        , simple(true)
    {
    }
    ModelBuilder& withFile(std::string);
    ModelBuilder& withTexture(std::string);
    ModelBuilder& withDiffuseR(float);
    ModelBuilder& withDiffuseG(float);
    ModelBuilder& withDiffuseB(float);
    ModelBuilder& withSpecularR(float);
    ModelBuilder& withSpecularG(float);
    ModelBuilder& withSpecularB(float);
    ModelBuilder& withEmissiveR(float);
    ModelBuilder& withEmissiveG(float);
    ModelBuilder& withEmissiveB(float);
    ModelBuilder& withAmbientR(float);
    ModelBuilder& withAmbientG(float);
    ModelBuilder& withAmbientB(float);
    std::unique_ptr<Model> build() const;
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
