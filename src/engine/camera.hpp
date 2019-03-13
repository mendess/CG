#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <string>

enum class CameraType {
    EXPLORER,
    FPS
};

namespace Camera {

void place_camera();

void process_key_bind(unsigned char key);

void toggle_cam();

CameraType current_camera();

std::string to_string(CameraType c);

}

#endif // __CAMERA_HPP__
