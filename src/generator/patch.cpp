#include "patch.h"

Patch::Patch() {}

Patch::Patch(vector<Point*> v) : controlPoints(v) {}

vector<Point*> Patch::getControlPoints(){
  return controlPoints;
}

void Patch::addControlPoint(Point* p){
  controlPoints.push_back(p);
}