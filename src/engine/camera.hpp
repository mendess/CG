#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "../common/point.hpp"
#include <string>

enum class CameraType {
    FOLLOW,
    FPS
};

namespace Camera {

void place_camera();

void process_key_bind(unsigned char key);

void toggle_cam();

void set_follow(Point);

CameraType current_camera();

std::string to_string(CameraType c);

}

#endif // __CAMERA_HPP__
