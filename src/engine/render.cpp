#include "camera.hpp"
#include "group.hpp"
#include "model.hpp"

#include <iostream>
#include <sstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

using namespace std;

namespace Render {

static float SCALE = 1;
static Group* SCENE;
static int DRAW_LEVEL = -1;
static bool SHOW_AXIS = false;
static size_t FOLLOW_TARGET = 0;
static bool FOLLOWING = false;
static bool PAUSED = false;
static double TIME_SCALE = 1.0;

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

double deltaTime()
{
    static double elapsed = 0;
    double new_elapsed = glutGet(GLUT_ELAPSED_TIME);
    double delta = new_elapsed - elapsed;
    elapsed = new_elapsed;
    return delta;
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    static double elapsed = 0;
    double delta = deltaTime() * TIME_SCALE;
    if (!PAUSED)
        elapsed += delta;
    if (FOLLOWING) {
        auto p = SCENE->get_model_position(FOLLOW_TARGET, elapsed);
        if (p.has_value()) {
            Camera::set_follow(p.value());
        }
    }
    Camera::place_camera();
    if (SHOW_AXIS) {
        // x axis
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f(-10000, 0, 0);
            glVertex3f(10000, 0, 0);
        }
        glEnd();
        // y axis
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        {
            glVertex3f(0, -10000, 0);
            glVertex3f(0, 10000, 0);
        }
        glEnd();
        // z axis
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        {
            glVertex3f(0, 0, -10000);
            glVertex3f(0, 0, 10000);
        }
        glEnd();
    }
    glScalef(SCALE, SCALE, SCALE);

    SCENE->draw(DRAW_LEVEL, elapsed);

    stringstream title;
    title << "CG-Engine | Draw Level: "
          << DRAW_LEVEL
          << " | Camera Mode: "
          << Camera::to_string(Camera::current_camera())
          << " | Time Scale: " << TIME_SCALE
          << " | Model Scale: " << SCALE;
    if (FOLLOWING) {
        title << " | FOLLOW TARGET: "
              << FOLLOW_TARGET;
    }
    if (PAUSED)
        title << " | PAUSED |";

    glutSetWindowTitle(title.str().data());

    glutSwapBuffers();
}

void key_bindings(unsigned char key, int _x, int _y)
{
    Camera::process_key_bind(key);
    switch (key) {
    case 'v':
        if (FOLLOWING) {
            FOLLOWING = false;
            Camera::set_follow(Point());
        } else {
            Camera::toggle_cam();
        }
        break;
    case '+':
        SCALE += 0.1;
        break;
    case '-':
        SCALE -= 0.1;
        break;
    case '[':
        if (DRAW_LEVEL > 1)
            DRAW_LEVEL--;
        break;
    case ']':
        if (DRAW_LEVEL < SCENE->levels())
            DRAW_LEVEL++;
        break;
    case '.':
        SHOW_AXIS = !SHOW_AXIS;
        break;
    case '}':
        if (!FOLLOWING) {
            FOLLOWING = true;
            FOLLOW_TARGET = 0;
        } else if (FOLLOW_TARGET < SCENE->model_count() - 1) {
            FOLLOW_TARGET++;
        } else {
            FOLLOW_TARGET = 0;
        }
        break;
    case '{':
        if (!FOLLOWING) {
            FOLLOWING = true;
            FOLLOW_TARGET = SCENE->model_count() - 1;
        } else if (FOLLOW_TARGET > 0) {
            FOLLOW_TARGET--;
        } else {
            FOLLOW_TARGET = SCENE->model_count() - 1;
        }
        break;
    case '(':
        TIME_SCALE -= 0.1;
        break;
    case ')':
        TIME_SCALE += 0.1;
        break;
    case ',':
        TranslateAnimated::toggle_routes();
        break;
    case 'p':
        PAUSED = !PAUSED;
        break;
    case 'q':
        exit(0);
    }
    renderScene();
}

int render(int argc, char** argv, Group* scene)
{
    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(10, 25);
    glutInitWindowSize(1200, 1050);
    glutCreateWindow("CG-Engine");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // Required callback registry
    SCENE = scene;
    DRAW_LEVEL = SCENE->levels();
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(key_bindings);

    glewInit();
    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnableClientState(GL_VERTEX_ARRAY);
    // enter GLUT's main cycle
    glutMainLoop();
    return 1;
}

}
