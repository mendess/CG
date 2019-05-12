#include "camera.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>

namespace Camera {

void fps_cam(unsigned char key);
void follow_cam(unsigned char key);

float speed = 1.0;
float radius = 100;
float alpha = M_PI;
float beta = M_PI / 5;
float look_x = 0.0;
float look_y = 0.0;
float look_z = 0.0;
float x = look_x + radius * cos(beta) * sin(alpha);
float z = look_z + radius * cos(beta) * cos(alpha);
float y = look_y + radius * sin(beta);
CameraType mode = CameraType::FOLLOW;
auto current_cam = follow_cam;

// Public API
void place_camera()
{
    switch (mode) {
    case CameraType::FOLLOW:
        gluLookAt(x, y, z,
            look_x, look_y, look_z,
            0.0f, 1.0f, 0.0f);
        break;
    case CameraType::FPS:
        gluLookAt(x, y, z,
            x + look_x, y + look_y, z + look_z,
            0.0f, 1.0f, 0.0f);
        break;
    }
}

void process_key_bind(unsigned char key)
{
    current_cam(key);
}

void toggle_cam()
{
    switch (mode) {
    case CameraType::FOLLOW:
        current_cam = Camera::fps_cam;
        Camera::set_fps();
        break;
    case CameraType::FPS:
        current_cam = Camera::follow_cam;
        Camera::set_follow(Point());
        break;
    }
}

CameraType current_camera()
{
    return mode;
}

std::string to_string(CameraType c)
{
    return (std::string[]) { "Follow", "FPS" }[(int)c];
}

// Calculations
void update_cam_pos()
{
    x = look_x + radius * cos(beta) * sin(alpha);
    z = look_z + radius * cos(beta) * cos(alpha);
    y = look_y + radius * sin(beta);
}

void update_cam_look()
{
    look_x = cos(beta) * sin(alpha);
    look_z = cos(beta) * cos(alpha);
    look_y = sin(beta);
}

void set_follow(Point p)
{
    look_x = p.x();
    look_y = p.y();
    look_z = p.z();
    if (mode != CameraType::FOLLOW) {
        mode = CameraType::FOLLOW;
        radius = sqrt(pow(x - look_x, 2) + pow(y - look_y, 2) + pow(z - look_z, 2));
        beta = asin((y - look_y) / radius);
        alpha = atan((x - look_x) / (z - look_z));
        if (z - look_z < 0.0) {
            alpha += M_PI;
        }
    }
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

// Key bindings
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

void fps_move_forward_fast()
{
    x += speed * 2 * look_x;
    z += speed * 2 * look_z;
}

void fps_move_left_fast()
{
    x += speed * 2 * look_z;
    z += speed * 2 * -1 * look_x;
}

void fps_move_back_fast()
{
    x += speed * 2 * -1 * look_x;
    z += speed * 2 * -1 * look_z;
}

void fps_move_right_fast()
{
    x += speed * 2 * -1 * look_z;
    z += speed * 2 * look_x;
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

void follow_move_up()
{
    if (beta + 0.1 < M_PI / 2)
        beta += 0.1;
}

void follow_move_down()
{
    if (beta - 0.1 > -M_PI / 2)
        beta -= 0.1;
}

void follow_move_c_clockwise()
{
    alpha += 0.1;
}

void follow_move_clockwise()
{
    alpha -= 0.1;
}

void follow_move_in()
{
    radius -= 0.1;
}

void follow_move_out()
{
    radius += 0.1;
}

void follow_move_in_fast()
{
    radius -= 1;
}

void follow_move_out_fast()
{
    radius += 1;
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
    case 'W':
        Camera::fps_move_forward_fast();
        break;
    case 'A':
        Camera::fps_move_left_fast();
        break;
    case 'D':
        Camera::fps_move_right_fast();
        break;
    case 'S':
        Camera::fps_move_back_fast();
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

void follow_cam(unsigned char key)
{
    switch (key) {
    case 'k':
        Camera::follow_move_up();
        break;
    case 'j':
        Camera::follow_move_down();
        break;
    case 'l':
        Camera::follow_move_c_clockwise();
        break;
    case 'h':
        Camera::follow_move_clockwise();
        break;
    case 'i':
        Camera::follow_move_in();
        break;
    case 'o':
        Camera::follow_move_out();
        break;
    case 'I':
        Camera::follow_move_in_fast();
        break;
    case 'O':
        Camera::follow_move_out_fast();
        break;
    }
    Camera::update_cam_pos();
}

}
