#include "model.hpp"
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

using namespace std;

enum class CameraType {
    EXPLORER,
    FPS
};

class Camera {
private:
    static float speed;
    static float radius;
    static float x;
    static float y;
    static float z;
    static float look_x;
    static float look_y;
    static float look_z;
    static float alpha;
    static float beta;
    static CameraType mode;

public:
    static void place_camera()
    {
        switch (mode) {
        case CameraType::EXPLORER:
            gluLookAt(x, y, z,
                0.0, 0.0, 0.0,
                0.0f, 1.0f, 0.0f);
            break;
        case CameraType::FPS:
            gluLookAt(x, y, z,
                x + look_x, y + look_y, z + look_z,
                0.0f, 1.0f, 0.0f);
            break;
        }
    }

    static void update_cam_pos()
    {
        x = radius * cos(beta) * sin(alpha);
        z = radius * cos(beta) * cos(alpha);
        y = radius * sin(beta);
    }

    static void update_cam_look()
    {
        look_x = cos(beta) * sin(alpha);
        look_z = cos(beta) * cos(alpha);
        look_y = sin(beta);
    }

    static void fps_move_forward()
    {
        x += speed * look_x;
        z += speed * look_z;
    }

    static void fps_move_left()
    {
        x += speed * look_z;
        z += speed * -1 * look_x;
    }

    static void fps_move_back()
    {
        x += speed * -1 * look_x;
        z += speed * -1 * look_z;
    }

    static void fps_move_right()
    {
        x += speed * -1 * look_z;
        z += speed * look_x;
    }

    static void fps_look_left()
    {
        alpha += 0.05;
    }

    static void fps_look_down()
    {
        if (beta - 0.1 > -M_PI / 2)
            beta -= 0.1;
    }

    static void fps_look_up()
    {
        if (beta + 0.1 < M_PI / 2)
            beta += 0.1;
    }

    static void fps_look_right()
    {
        alpha -= 0.05;
    }

    static void fps_move_down()
    {
        y -= 0.1;
    }

    static void fps_move_up()
    {
        y += 0.1;
    }

    static void explorer_move_up()
    {
        if (beta + 0.1 < M_PI / 2)
            beta += 0.1;
    }

    static void explorer_move_down()
    {
        if (beta - 0.1 > -M_PI / 2)
            beta -= 0.1;
    }

    static void explorer_move_c_clockwise()
    {
        alpha += 0.1;
    }

    static void explorer_move_clockwise()
    {
        alpha -= 0.1;
    }

    static void explorer_move_in()
    {
        radius -= 0.1;
    }

    static void explorer_move_out()
    {
        radius += 0.1;
    }

    static void set_explorer()
    {
        mode = CameraType::EXPLORER;
        alpha = atan(x / z);
        if (z < 0.0)
            alpha += M_PI;
        beta = asin(y / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
        radius = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        update_cam_pos();
    }

    static void set_fps()
    {
        mode = CameraType::FPS;
        if (alpha < 0.0) {
            alpha += M_PI;
        } else {
            alpha -= M_PI;
        }
        beta = -beta;
        update_cam_look();
    }

    static CameraType current_cam()
    {
        return mode;
    }
};

float Camera::speed = 1.0;
float Camera::radius = 10;
float Camera::alpha = 0.0;
float Camera::beta = 0.0;
float Camera::x = Camera::radius * cos(Camera::beta) * sin(Camera::alpha);
float Camera::y = Camera::radius * sin(Camera::beta);
float Camera::z = Camera::radius * cos(Camera::beta) * cos(Camera::alpha);
float Camera::look_x = 0.0;
float Camera::look_y = 0.0;
float Camera::look_z = 1.0;
CameraType Camera::mode = CameraType::EXPLORER;

static float SCALE = 1;

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
    Camera::place_camera();
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

void explorer_cam(unsigned char key)
{
    switch (key) {
    case 'k':
        Camera::explorer_move_up();
        break;
    case 'j':
        Camera::explorer_move_down();
        break;
    case 'l':
        Camera::explorer_move_c_clockwise();
        break;
    case 'h':
        Camera::explorer_move_clockwise();
        break;
    case 'i':
        Camera::explorer_move_in();
        break;
    case 'o':
        Camera::explorer_move_out();
        break;
    }
    Camera::update_cam_pos();
}

void fps_cam(unsigned char key)
{
    switch (key) {
    case 'w':
        Camera::fps_move_forward();
        break;
    case 'a':
        Camera::fps_move_left();
        break;
    case 'd':
        Camera::fps_move_right();
        break;
    case 's':
        Camera::fps_move_back();
        break;
    case 'h':
        Camera::fps_look_left();
        break;
    case 'l':
        Camera::fps_look_right();
        break;
    case 'j':
        Camera::fps_look_down();
        break;
    case 'k':
        Camera::fps_look_up();
        break;
    case 'g':
        Camera::fps_move_down();
        break;
    case 'G':
        Camera::fps_move_up();
        break;
    }
    Camera::update_cam_look();
}

void key_bindings(unsigned char key, int _x, int _y)
{
    static auto current_cam = explorer_cam;
    current_cam(key);
    switch (key) {
    case 'v':
        switch (Camera::current_cam()) {
        case CameraType::EXPLORER:
            current_cam = fps_cam;
            Camera::set_fps();
            break;
        case CameraType::FPS:
            current_cam = explorer_cam;
            Camera::set_explorer();
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
    if (argc < 2) {
        if(!Models::load("config.xml")) return 1;
    } else if ("--help" == string(argv[1]) || "-h" == string(argv[1])) {
        cerr << argv[0] << " [config.xml]" << endl;
        return 2;
    } else {
        if(!Models::load(string(argv[1]))) return 1;
    }
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
