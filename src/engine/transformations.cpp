#include "transformations.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <tuple>
#include <cstring>
#include <cmath>

using namespace std;

tuple<Point, Point> getGlobalCatmullRomPoint(vector<Point> points, float gt);
struct matrix translate_matrix(float x, float y, float z);
struct matrix scale_matrix(float x, float y, float z);
struct matrix rotation_matrix(float angle, float x, float y, float z);

void RotateStatic::transform(double elapsed) const
{
    glRotatef(angle, x, y, z);
}

struct matrix RotateStatic::matrix(double elapsed) const {
    return rotation_matrix(angle, x, y, z);
}

void RotateAnimated::transform(double elapsed) const
{
    float angle = (elapsed * 360 / dur);
    glRotatef(angle, x, y, z);
}

struct matrix RotateAnimated::matrix(double elapsed) const {
    float angle = (elapsed * 360 / dur);
    return rotation_matrix(angle, x, y, z);
}

void TranslateStatic::transform(double elapsed) const
{
    glTranslatef(x, y, z);
}

struct matrix TranslateStatic::matrix(double elapsed) const {
    return translate_matrix(x, y, z);
}

void TranslateAnimated::transform(double elapsed) const
{
    float elapsed_b = elapsed;
    while (elapsed_b > dur)
        elapsed_b -= dur;
    tuple<Point, Point> pos_deriv = getGlobalCatmullRomPoint(points, elapsed_b / dur);
    Point pos = get<0>(pos_deriv);
    glTranslatef(pos.x(), pos.y(), pos.z());
}

struct matrix TranslateAnimated::matrix(double elapsed) const {
    float elapsed_b = elapsed;
    while (elapsed_b > dur)
        elapsed_b -= dur;
    tuple<Point, Point> pos_deriv = getGlobalCatmullRomPoint(points, elapsed_b / dur);
    Point pos = get<0>(pos_deriv);
    return translate_matrix(pos.x(), pos.y(), pos.z());
}

void Scale::transform(double elapsed) const
{
    glScalef(x, y, z);
}

struct matrix Scale::matrix(double elapsed) const {
    return scale_matrix(x, y, z);
}

struct matrix translate_matrix(float x, float y, float z)
{
    return {.matrix = {
        { 1, 0, 0, x },
        { 0, 1, 0, y },
        { 0, 0, 1, z },
        { 0, 0, 0, 1 }
    }};
}

struct matrix scale_matrix(float x, float y, float z)
{
    return {.matrix = {
        { x, 0, 0, 0 },
        { 0, y, 0, 0 },
        { 0, 0, z, 0 },
        { 0, 0, 0, 1 }
    }};
}

struct matrix rotation_matrix(float angle, float x, float y, float z)
{
    /* float r[4][4] = { */
    /*     { x * x + (1 - (x * x)) * cos(angle)       , x * y * (1 - cos(angle)) - z * sin(angle), x * z * (1 - cos(angle)) + y * sin(angle), 0 }, */
    /*     { y * x * (1 - cos(angle)) + z * sin(angle), y * y + (1 - y * y) * cos(angle)         , y * z * (1 - cos(angle)) - x * sin(angle), 0 }, */
    /*     { z * x * (1 - cos(angle)) - y * sin(angle), z * y * (1 - cos(angle)) + x * sin(angle), z * z + (1 - z * z) * cos(angle)         , 0 }, */
    /*     { 0                                        , 0                                        , 0                                        , 1 } */
    /* }; */
    return {.matrix = {
        { x * x + (1 - (x * x)) * cos(angle)       , x * y * (1 - cos(angle)) - z * sin(angle), x * z * (1 - cos(angle)) + y * sin(angle), 0 },
        { y * x * (1 - cos(angle)) + z * sin(angle), y * y + (1 - y * y) * cos(angle)         , y * z * (1 - cos(angle)) - x * sin(angle), 0 },
        { z * x * (1 - cos(angle)) - y * sin(angle), z * y * (1 - cos(angle)) + x * sin(angle), z * z + (1 - z * z) * cos(angle)         , 0 },
        { 0                                        , 0                                        , 0                                        , 1 }
    }};
}

void buildRotMatrix(float* x, float* y, float* z, float* m)
{
    m[0] = x[0];
    m[1] = x[1];
    m[2] = x[2];
    m[3] = 0;
    m[4] = y[0];
    m[5] = y[1];
    m[6] = y[2];
    m[7] = 0;
    m[8] = z[0];
    m[9] = z[1];
    m[10] = z[2];
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void cross(const float* a, const float* b, float* res)
{
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a)
{
    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

float length(const float* v)
{
    float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return res;
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

tuple<Point, Point> getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3)
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
    float pY[4] = { p1.y(), p1.y(), p2.y(), p3.y() };
    multMatrixVector(m, pY, aY);
    float pZ[4] = { p2.z(), p1.z(), p2.z(), p3.z() };
    multMatrixVector(m, pZ, aZ);

    float tv[4] = { t * t * t, t * t, t, 1 };

    // Compute pos = T * A
    Point pos(
        tv[0] * aX[0] + tv[1] * aX[1] + tv[2] * aX[2] + tv[3] * aX[3],
        tv[0] * aY[0] + tv[1] * aY[1] + tv[2] * aY[2] + tv[3] * aY[3],
        tv[0] * aZ[0] + tv[1] * aZ[1] + tv[2] * aZ[2] + tv[3] * aZ[3]);
    // compute deriv = T' * A
    float tvl[4] = { 3 * t * t, 2 * t, 1, 0 };
    Point deriv(
        tvl[0] * aX[0] + tvl[1] * aX[1] + tvl[2] * aX[2] + tvl[3] * aX[3],
        tvl[0] * aY[0] + tvl[1] * aY[1] + tvl[2] * aY[2] + tvl[3] * aY[3],
        tvl[0] * aZ[0] + tvl[1] * aZ[1] + tvl[2] * aZ[2] + tvl[3] * aZ[3]);
    return make_tuple(pos, deriv);
}

// given  global t, returns the point in the curve
tuple<Point, Point> getGlobalCatmullRomPoint(vector<Point> points, float gt)
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
