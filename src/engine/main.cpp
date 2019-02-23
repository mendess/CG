#include "../common/point.hpp"
#include "../dependencies/rapidxml.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace rapidxml;
using namespace std;

static vector<vector<Point*>> models;

float CAM_D = 5;
float SCALE = 1;
float CAM_X = sin(0.0) * CAM_D;
float CAM_Y = 5.0;
float CAM_Z = cos(0.0) * CAM_D;

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

char* read_file()
{
    char* file_contents;
    FILE* input_file = fopen("config.xml", "rb");
    if (input_file == NULL) {
        return NULL;
    }
    fseek(input_file, 0, SEEK_END);
    long input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = (char*)malloc(input_file_size * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);
    return file_contents;
}

void drawTriangle(Point* a, Point* b, Point* c)
{
    static default_random_engine generator;
    static uniform_int_distribution<int> distribution(0, 100);
    static auto rng = std::bind(distribution, generator);
    float r = (((float)rng()) / 100.0f);
    float g = (((float)rng()) / 100.0f);
    float u = (((float)rng()) / 100.0f);
    glColor3f(r, g, u);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a->x(), a->y(), a->z());
        glVertex3f(b->x(), b->y(), b->z());
        glVertex3f(c->x(), c->y(), c->z());
    }
    glEnd();
}

vector<Point*> loadModel(char* modelFile)
{
    float x, y, z;
    vector<Point*> model;
    ifstream infile(modelFile);
    while (infile >> x >> y >> z) {
        model.push_back(new Point(x, y, z));
    }

    return model;
}

void drawModel(vector<Point*> model)
{
    vector<Point*>::iterator it = model.begin();
    while (it != model.end()) {
        Point* a = *it++;
        Point* b = *it++;
        Point* c = *it++;
        drawTriangle(a, b, c);
    }
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(CAM_X, CAM_Y, CAM_Z,
        0.0, 0.0, 0.0,
        0.0f, 1.0f, 0.0f);
    glScalef(SCALE, SCALE, SCALE);
    for (vector<vector<Point*>>::iterator it = models.begin(); it != models.end(); ++it) {
        drawModel(*it);
    }

    glutSwapBuffers();
}

void loadModels()
{
    char* text = read_file();
    if (text == NULL) {
        printf("config.xml file not found");
        return;
    }
    xml_document<> doc;
    doc.parse<0>(text);
    xml_node<>* node = doc.first_node("scene");
    for (node = node->first_node(); node; node = node->next_sibling()) {
        xml_attribute<>* attr = node->first_attribute();
        models.push_back(loadModel(attr->value()));
    }
}

void key_bindings(unsigned char key, int _x, int _y)
{
    static float px = 0.0;
    static float pz = 0.0;
    bool moveCam = false;
    switch (key) {
    case 'q':
        exit(0);
    case 'j':
        CAM_Y -= 0.1;
        break;
    case '+':
        SCALE += 0.1;
        break;
    case '-':
        SCALE -= 0.1;
        break;
    case 'k':
        CAM_Y += 0.1;
        break;
    case 'l':
        px += 0.1;
        pz += 0.1;
        moveCam = true;
        break;
    case 'h':
        px -= 0.1;
        pz -= 0.1;
        moveCam = true;
        break;
    case 'o':
        CAM_D += 0.1;
        moveCam = true;
        break;
    case 'i':
        CAM_D -= 0.1;
        moveCam = true;
        break;
    }
    if (moveCam) {
        CAM_X = sin(px) * CAM_D;
        CAM_Z = cos(pz) * CAM_D;
    }
    renderScene();
}

int main(int argc, char** argv)
{
    loadModels();
    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(key_bindings);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
