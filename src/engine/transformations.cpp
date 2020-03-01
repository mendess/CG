#include "engine/transformations.hpp"
#include "engine/render.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <tuple>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;

static tuple<Point, Vector> getGlobalCatmullRomPoint(vector<Point> points, float gt);
static Matrix translate_matrix(float x, float y, float z);
static Matrix scale_matrix(float x, float y, float z);
static Matrix rotation_matrix(float angle, float x, float y, float z);
static void buildRotMatrix(Vector, Vector, Vector, float* m);
static float length(const float* v);
static void multMatrixVector(const float m[4][4], const float v[4], float res[4]);

void RotateStatic::transform(double elapsed) const
{
    glRotatef(angle, x, y, z);
}

void RotateAnimated::transform(double elapsed) const
{
    float angle = (elapsed * 360 / dur);
    glRotatef(angle, x, y, z);
}

void TranslateStatic::transform(double elapsed) const
{
    glTranslatef(x, y, z);
}

tuple<Point, Vector> TranslateAnimated::get_position(double elapsed) const
{
    int elapsed_i = ((int) elapsed) % ((int) dur);
    tuple<Point, Vector> pos_deriv = getGlobalCatmullRomPoint(points, ((float) elapsed_i) / dur);
    return pos_deriv;
}

void TranslateAnimated::transform(double elapsed) const
{
    if (show_routes)
        draw_routes();
    auto pos_deriv = get_position(elapsed);
    Point pos = get<0>(pos_deriv);
    glTranslatef(pos.x(), pos.y(), pos.z());
    Vector X = get<1>(pos_deriv).normalize();
    Vector Z = X.cross({ 0, 1, 0 }).normalize();
    Vector Y = Z.cross(X).normalize();
    float m[16];
    buildRotMatrix(X, Y, Z, m);
    glMultMatrixf(m);
}

void TranslateAnimated::draw_routes() const
{
    glDisable(GL_LIGHTING);
    float gt = 0.0;
    const float NUM_STEPS = 100;
    const float gt_step = 1.0 / NUM_STEPS;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_STEPS; i++) {
        tuple<Point, Vector> pos_deriv = getGlobalCatmullRomPoint(points, gt);
        Point pos = get<0>(pos_deriv);
        glVertex3f(pos.x(), pos.y(), pos.z());
        gt += gt_step;
    }
    glEnd();
    if(Render::LIGHTS) {
        glEnable(GL_LIGHTING);
    }
}

void ScaleStatic::transform(double elapsed) const
{
    glScalef(x, y, z);
}

tuple<float, float, float> ScaleAnimated::get_ratio(double elapsed) const
{
    while (elapsed > dur)
        elapsed -= dur;
    const float t = abs(2 * (elapsed / dur) - 1);
    float mx = xi + t * (xf - xi);
    float my = yi + t * (yf - yi);
    float mz = zi + t * (zf - zi);
    return make_tuple(mx, my, mz);
}

void ScaleAnimated::transform(double elapsed) const
{
    auto r = get_ratio(elapsed);
    glScalef(get<0>(r), get<1>(r), get<2>(r));
}

void buildRotMatrix(Vector x, Vector y, Vector z, float* m)
{
    m[0] = x.x();
    m[1] = x.y();
    m[2] = x.z();
    m[4] = y.x();
    m[5] = y.y();
    m[6] = y.z();
    m[8] = z.x();
    m[9] = z.y();
    m[10] = z.z();
    m[15] = 1;
    m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
}

void multMatrixVector(const float m[4][4], const float v[4], float res[4])
{
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j][k];
        }
    }
}

tuple<Point, Vector> getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3)
{
    // catmull-rom matrix
    const float m[4][4] = {
        { -0.5f, +1.5f, -1.5f, +0.5f },
        { +1.0f, -2.5f, +2.0f, -0.5f },
        { -0.5f, +0.0f, +0.5f, +0.0f },
        { +0.0f, +1.0f, +0.0f, +0.0f }
    };

    // Compute A = M * P
    float aX[4];
    float aY[4];
    float aZ[4];
    float pX[4] = { p0.x(), p1.x(), p2.x(), p3.x() };
    multMatrixVector(m, pX, aX);
    float pY[4] = { p0.y(), p1.y(), p2.y(), p3.y() };
    multMatrixVector(m, pY, aY);
    float pZ[4] = { p0.z(), p1.z(), p2.z(), p3.z() };
    multMatrixVector(m, pZ, aZ);

    float tv[4] = { t * t * t, t * t, t, 1 };

    // Compute pos = T * A
    Point pos(
        tv[0] * aX[0] + tv[1] * aX[1] + tv[2] * aX[2] + tv[3] * aX[3],
        tv[0] * aY[0] + tv[1] * aY[1] + tv[2] * aY[2] + tv[3] * aY[3],
        tv[0] * aZ[0] + tv[1] * aZ[1] + tv[2] * aZ[2] + tv[3] * aZ[3]);
    // compute deriv = T' * A
    float tvl[4] = { 3 * t * t, 2 * t, 1, 0 };
    Vector deriv(
        tvl[0] * aX[0] + tvl[1] * aX[1] + tvl[2] * aX[2] + tvl[3] * aX[3],
        tvl[0] * aY[0] + tvl[1] * aY[1] + tvl[2] * aY[2] + tvl[3] * aY[3],
        tvl[0] * aZ[0] + tvl[1] * aZ[1] + tvl[2] * aZ[2] + tvl[3] * aZ[3]);
    return make_tuple(pos, deriv);
}

tuple<Point, Vector> getGlobalCatmullRomPoint(vector<Point> points, float gt)
{
    const size_t point_count = points.size();
    float t = gt * point_count; // this is the real global t
    int index = floor(t);       // which segment
    t = t - index;              // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + point_count - 1) % point_count;
    indices[1] = (indices[0] + 1) % point_count;
    indices[2] = (indices[1] + 1) % point_count;
    indices[3] = (indices[2] + 1) % point_count;

    return getCatmullRomPoint(t,
        points[indices[0]],
        points[indices[1]],
        points[indices[2]],
        points[indices[3]]);
}
