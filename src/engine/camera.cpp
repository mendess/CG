#include "camera.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

namespace Camera {

float speed = 1.0;
float radius = 100;
float alpha = M_PI;
float beta = M_PI / 5;
float x = radius * cos(beta) * sin(alpha);
float y = radius * sin(beta);
float z = radius * cos(beta) * cos(alpha);
float look_x = 0.0;
float look_y = 0.0;
float look_z = 1.0;
CameraType mode = CameraType::EXPLORER;

void place_camera()
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

void update_cam_pos()
{
    x = radius * cos(beta) * sin(alpha);
    z = radius * cos(beta) * cos(alpha);
    y = radius * sin(beta);
}

void update_cam_look()
{
    look_x = cos(beta) * sin(alpha);
    look_z = cos(beta) * cos(alpha);
    look_y = sin(beta);
}

void fps_move_forward()
{
    x += speed * look_x;
    z += speed * look_z;
}

void fps_move_left()
{
    x += speed * look_z;
    z += speed * -1 * look_x;
}

void fps_move_back()
{
    x += speed * -1 * look_x;
    z += speed * -1 * look_z;
}

void fps_move_right()
{
    x += speed * -1 * look_z;
    z += speed * look_x;
}

void fps_look_left()
{
    alpha += 0.05;
}

void fps_look_down()
{
    if (beta - 0.1 > -M_PI / 2)
        beta -= 0.1;
}

void fps_look_up()
{
    if (beta + 0.1 < M_PI / 2)
        beta += 0.1;
}

void fps_look_right()
{
    alpha -= 0.05;
}

void fps_move_down()
{
    y -= 0.1;
}

void fps_move_up()
{
    y += 0.1;
}

void explorer_move_up()
{
    if (beta + 0.1 < M_PI / 2)
        beta += 0.1;
}

void explorer_move_down()
{
    if (beta - 0.1 > -M_PI / 2)
        beta -= 0.1;
}

void explorer_move_c_clockwise()
{
    alpha += 0.1;
}

void explorer_move_clockwise()
{
    alpha -= 0.1;
}

void explorer_move_in()
{
    radius -= 0.1;
}

void explorer_move_out()
{
    radius += 0.1;
}

void explorer_move_in_fast()
{
    radius -= 1;
}

void explorer_move_out_fast()
{
    radius += 1;
}
void set_explorer()
{
    mode = CameraType::EXPLORER;
    alpha = atan(x / z);
    if (z < 0.0)
        alpha += M_PI;
    beta = asin(y / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
    radius = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    update_cam_pos();
}

void set_fps()
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
    case 'I':
        Camera::explorer_move_in_fast();
        break;
    case 'O':
        Camera::explorer_move_out_fast();
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

auto current_cam = explorer_cam;

void process_key_bind(unsigned char key)
{
    current_cam(key);
}

void toggle_cam()
{
    switch (mode) {
    case CameraType::EXPLORER:
        current_cam = fps_cam;
        Camera::set_fps();
        break;
    case CameraType::FPS:
        current_cam = Camera::explorer_cam;
        Camera::set_explorer();
        break;
    }
}

CameraType current_camera() {
    return mode;
}

std::string to_string(CameraType c)
{
    return (std::string[]) { "Explorer", "FPS" }[(int)c];
}

}
