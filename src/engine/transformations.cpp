#include "transformations.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

using namespace std;

void Rotate::transform() const
{
    glRotatef(angle, x, y, z);
}

void Translate::transform() const
{
    glTranslatef(x, y, z);
}

void Scale::transform() const
{
    glScalef(x, y, z);
}

void Color::transform() const
{
    glColor4f(r, g, b, a);
}
