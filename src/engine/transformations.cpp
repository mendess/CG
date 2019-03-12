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
    cout << "angle: " << angle << " x: " << x << " y: " << y << " z: " << z << endl;
    glRotatef(angle, x, y, z);
}

void Translate::transform() const
{
    cout << " x: " << x << " y: " << y << " z: " << z << endl;
    glTranslatef(x, y, z);
}

void Scale::transform() const
{
    cout << " x: " << x << " y: " << y << " z: " << z << endl;
    glScalef(x, y, z);
}

void Color::transform() const
{
    cout << " r: " << r << " g: " << g << " b: " << b << endl;
    glColor3f(r, g, b);
}
