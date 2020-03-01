#ifndef MODEL_H
#define MODEL_H

#include "../common/point.hpp"
#include "rgb.hpp"

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class Textures {
private:
    static inline std::unordered_map<std::string, GLuint> textures;

public:
    static GLuint load_texture(std::string);
};

class ModelBuffer {
private:
    static inline std::unordered_map<std::string, std::unique_ptr<ModelBuffer>> modelBuffers;
    GLuint buffers[3];
    size_t _n_vertices;

public:
    ModelBuffer(std::string);
    GLuint vbo_buffer() const { return buffers[0]; }
    GLuint normal_buffer() const { return buffers[1]; }
    GLuint texture_buffer() const { return buffers[2]; }
    size_t n_vertices() const { return _n_vertices; }
    static std::unique_ptr<ModelBuffer>& get(std::string);
};

class Model {
public:
    virtual void draw() const = 0;
    virtual std::string to_string() const = 0;
};

class ColoredModel : public Model {
private:
    GLuint buffers[2];
    RGBA diffuse;
    RGBA specular;
    RGBA emissive;
    RGBA ambient;
    size_t n_vertices;

    inline GLuint vbo_buffer() const { return buffers[0]; }
    inline GLuint normal_buffer() const { return buffers[1]; }

public:
    ColoredModel(std::string, RGBA, RGBA, RGBA, RGBA);
    void draw() const;
    std::string to_string() const
    {
        std::stringstream ss;
        ss << "ColoredModel{ "
           << "DIFF: { " << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << " },"
           << "SPEC: { " << specular.r << ", " << specular.g << ", " << specular.b << " },"
           << "EMIS: { " << emissive.r << ", " << emissive.g << ", " << emissive.b << " },"
           << "AMBI: { " << ambient.r << ", " << ambient.g << ", " << ambient.b << " } }";
        return ss.str();
    }
};

class TexturedModel : public Model {
private:
    GLuint buffers[3];
    GLuint texture_slot;
    std::string texture_file;
    RGBA diffuse;
    RGBA specular;
    RGBA emissive;
    RGBA ambient;
    size_t n_vertices;

    inline GLuint vbo_buffer() const { return buffers[0]; }
    inline GLuint normal_buffer() const { return buffers[1]; }
    inline GLuint texture_buffer() const { return buffers[2]; }

public:
    TexturedModel(std::string, std::string, RGBA, RGBA, RGBA, RGBA);
    void draw() const;
    std::string to_string() const
    {
        std::stringstream ss;
        ss << "TexturedModel{ "
           << "FILE: { " << texture_file << " },"
           << "DIFF: { " << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << " },"
           << "SPEC: { " << specular.r << ", " << specular.g << ", " << specular.b << " },"
           << "EMIS: { " << emissive.r << ", " << emissive.g << ", " << emissive.b << " },"
           << "AMBI: { " << ambient.r << ", " << ambient.g << ", " << ambient.b << " } }";
        return ss.str();
    }
};

#endif //MODEL_H
