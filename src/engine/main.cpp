#include "model.hpp"
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace std;

float CAM_R = 5;
float SCALE = 1;
float CAM_X = cos(0.0) * sin(0.0) * CAM_R;
float CAM_Y = 0.0;
float CAM_Z = cos(0.0) * cos(0.0) * CAM_R;

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
    // x axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    {
        glVertex3f(-1000, 0, 0);
        glVertex3f(1000, 0, 0);
    }
    glEnd();

    // y axis
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    {
        glVertex3f(0, -1000, 0);
        glVertex3f(0, 1000, 0);
    }
    glEnd();

    // z axis
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, -1000);
        glVertex3f(0, 0, 1000);
    }
    glEnd();
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
    static float _alpha = 0.0;
    static float _beta = 0.0;
    switch (key) {
    case 'j':
        if (_beta + 0.1 < M_PI / 2)
            _beta += 0.1;
        break;
    case 'k':
        if (_beta - 0.1 > -M_PI / 2)
            _beta -= 0.1;
        break;
    case 'l':
        _alpha += 0.1;
        break;
    case 'h':
        _alpha -= 0.1;
        break;
    case 'i':
        CAM_R -= 0.1;
        break;
    case 'o':
        CAM_R += 0.1;
        break;
    case '+':
        SCALE += 0.1;
        break;
    case '-':
        SCALE -= 0.1;
        break;
    case 'q':
        exit(0);
    }
    CAM_X = CAM_R * cos(_beta) * sin(_alpha);
    CAM_Z = CAM_R * cos(_beta) * cos(_alpha);
    CAM_Y = CAM_R * sin(_beta);
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
