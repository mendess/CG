#include "transformations.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

using namespace std;

void RotateStatic::transform(double elapsed) const
{
    glRotatef(angle, x, y, z);
}

void RotateAnimated::transform(double elapsed) const
{
    float angle = (elapsed * 360 / dur);
    glRotatef(angle, x, y, z);
}

void Translate::transform(double elapsed) const
{
    glTranslatef(x, y, z);
}

void Scale::transform(double elapsed) const
{
    glScalef(x, y, z);
}
