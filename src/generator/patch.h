#ifndef __PATCH_H__
#define __PATCH_H__

#include <vector>
#include <math.h>
#include "../common/point.hpp"

using namespace std;

class Patch {
    private:
        vector<Point*> controlPoints;
    public:
        Patch();
        Patch(vector<Point*>);
        vector<Point*> getControlPoints();
        void addControlPoint(Point*);
};

#endif