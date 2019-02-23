#ifndef POINT_H
#define POINT_H
#include <string>
#include <sstream>
#include <math.h>

class Point {
    private:
        float _x,_y,_z;
    public:
        Point();
        Point(float, float, float);
        std::string toString();
        float x(){ return _x; }
        float y(){ return _y; }
        float z(){ return _z; }
};


#endif// POINT_H
