#include "model.hpp"
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace std;

float CAM_D = 20;
float SCALE = 1;
float CAM_X = sin(0.0) * CAM_D;
float CAM_Y = 1.0;
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

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(CAM_X, CAM_Y, CAM_Z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
    glScalef(SCALE, SCALE, SCALE);

    Models* models = Models::get();
    for (int i = 0; i < models->num_models(); ++i) {
        const Model* m = models->get_model(i);
        if (!m->draw_model()) {
            cerr << m->name() << ": Invalid number of control points" << endl;
        }
    }
    glutSwapBuffers();
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
    Models::load("config.xml");
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
