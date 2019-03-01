#include "model.hpp"
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace std;

enum class Camera {
    EXPLORER,
    FPS
};

static float CAM_R = 5;
static float SCALE = 1;
static float CAM_X = cos(0.0) * sin(0.0) * CAM_R;
static float CAM_Y = 0.0;
static float CAM_Z = cos(0.0) * cos(0.0) * CAM_R;
static float CAM_LOOK_X = 0.0;
static float CAM_LOOK_Y = 0.0;
static float CAM_LOOK_Z = 1.0;
static float ALPHA = 0.0;
static float BETA = 0.0;
static Camera CURRENT_CAM = Camera::EXPLORER;

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
    switch (CURRENT_CAM) {
    case Camera::EXPLORER:
        gluLookAt(CAM_X, CAM_Y, CAM_Z,
            0.0, 0.0, 0.0,
            0.0f, 1.0f, 0.0f);
        break;
    case Camera::FPS:
        gluLookAt(CAM_X, CAM_Y, CAM_Z,
            CAM_X + CAM_LOOK_X, CAM_Y + CAM_LOOK_Y, CAM_Z + CAM_LOOK_Z,
            0.0f, 1.0f, 0.0f);
        break;
    }
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

void update_cam_pos()
{
    CAM_X = CAM_R * cos(BETA) * sin(ALPHA);
    CAM_Z = CAM_R * cos(BETA) * cos(ALPHA);
    CAM_Y = CAM_R * sin(BETA);
}

void update_cam_look()
{
    CAM_LOOK_X = cos(BETA) * sin(ALPHA);
    CAM_LOOK_Z = cos(BETA) * cos(ALPHA);
    CAM_LOOK_Y = sin(BETA);
}

void explorer_cam(unsigned char key)
{
    switch (key) {
    case 'k':
        if (BETA + 0.1 < M_PI / 2)
            BETA += 0.1;
        break;
    case 'j':
        if (BETA - 0.1 > -M_PI / 2)
            BETA -= 0.1;
        break;
    case 'l':
        ALPHA += 0.1;
        break;
    case 'h':
        ALPHA -= 0.1;
        break;
    case 'i':
        CAM_R -= 0.1;
        break;
    case 'o':
        CAM_R += 0.1;
        break;
    }
    update_cam_pos();
}

void fps_cam(unsigned char key)
{
    const double speed = 1.0;
    switch (key) {
    case 'w':
        CAM_X += speed * CAM_LOOK_X;
        CAM_Z += speed * CAM_LOOK_Z;
        break;
    case 'a':
        CAM_X += speed * CAM_LOOK_Z;
        CAM_Z += speed * -1 * CAM_LOOK_X;
        break;
    case 'd':
        CAM_X += speed * -1 * CAM_LOOK_Z;
        CAM_Z += speed * CAM_LOOK_X;
        break;
    case 's':
        CAM_X += speed * -1 * CAM_LOOK_X;
        CAM_Z += speed * -1 * CAM_LOOK_Z;
        break;
    case 'h':
        ALPHA += 0.05;
        break;
    case 'l':
        ALPHA -= 0.05;
        break;
    case 'j':
        if (BETA - 0.1 > -M_PI / 2)
            BETA -= 0.01;
        break;
    case 'k':
        if (BETA + 0.1 < M_PI / 2)
            BETA += 0.01;
        break;
    case 'G':
        CAM_Y += 0.1;
        break;
    case 'g':
        CAM_Y -= 0.1;
        break;
    }
    update_cam_look();
}

void key_bindings(unsigned char key, int _x, int _y)
{
    static auto current_cam = explorer_cam;
    current_cam(key);
    switch (key) {
    case 'v':
        switch (CURRENT_CAM) {
        case Camera::FPS:
            current_cam = explorer_cam;
            CURRENT_CAM = Camera::EXPLORER;
            break;
        case Camera::EXPLORER:
            current_cam = fps_cam;
            CURRENT_CAM = Camera::FPS;
            ALPHA = ALPHA + M_PI;
            BETA = BETA + M_PI;
            cerr << "alpha: " << ALPHA << " beta: " << BETA << endl;
            CAM_LOOK_X = cos(BETA) * sin(ALPHA);
            CAM_LOOK_Z = cos(BETA) * cos(ALPHA);
            CAM_LOOK_Y = sin(BETA);
            break;
        }
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
