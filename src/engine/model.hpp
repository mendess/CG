#ifndef MODEL_H
#define MODEL_H

#include "../common/point.hpp"

#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class Model {
private:
    GLuint buffer;
    std::vector<float> vbo;
    size_t n_vertices;
    bool prepared;

    void push(float, float, float);

public:
    Model(char*);
    void draw();
};

#endif //MODEL_H
