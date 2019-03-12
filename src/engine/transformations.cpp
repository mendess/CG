#include "transformations.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
    glColor3f(r, g, b);
}
