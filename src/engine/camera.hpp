#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

enum class CameraType {
    EXPLORER,
    FPS
};

namespace Camera {

void place_camera();

void process_key_bind(unsigned char key);

void toggle_cam();

}

#endif // __CAMERA_HPP__
