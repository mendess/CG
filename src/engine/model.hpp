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
    GLuint buffer[1];
    std::vector<float> vbo;
    char* modelName;
    size_t n_vertices;
    bool prepared;

    void prepare();
    void push(float, float, float);

public:
    Model(char*);
    Model(const Model& other);
    bool loaded();
    bool draw();
    char* name() const { return modelName; };
    ~Model() { free(modelName); };
};

#endif //MODEL_H
