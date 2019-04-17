#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "../common/point.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <array>

typedef struct matrix {
    float matrix[4][4];
} Matrix;

class Transformation {
public:
    virtual ~Transformation() {};
    virtual void transform(double elapsed) const = 0;
    virtual struct matrix matrix(double elapsed) const = 0;
};

class RotateStatic : public Transformation {
private:
    float angle, x, y, z;

public:
    RotateStatic(float angle, float x, float y, float z)
        : angle(angle)
        , x(x)
        , y(y)
        , z(z)
    {
    }
    ~RotateStatic() override {};
    void transform(double) const override;
    struct matrix matrix(double elapsed) const override;
};

class RotateAnimated : public Transformation {
private:
    float dur, x, y, z;

public:
    RotateAnimated(float dur, float x, float y, float z)
        : dur(dur * 1000)
        , x(x)
        , y(y)
        , z(z)
    {
    }
    ~RotateAnimated() override {};
    void transform(double) const override;
    struct matrix matrix(double elapsed) const override;
};

class TranslateStatic : public Transformation {
private:
    float x, y, z;

public:
    TranslateStatic(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    ~TranslateStatic() override {};
    void transform(double) const override;
    struct matrix matrix(double elapsed) const override;
};

class TranslateAnimated : public Transformation {
private:
    std::vector<Point> points;
    float dur;

public:
    TranslateAnimated(std::vector<Point> points, float dur)
        : points(points)
        , dur(dur * 1000)
    {
    }
    ~TranslateAnimated() override {};
    void transform(double) const override;
    struct matrix matrix(double elapsed) const override;
};

class Scale : public Transformation {
private:
    float x, y, z;

public:
    Scale(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    ~Scale() override {};
    void transform(double) const override;
    struct matrix matrix(double elapsed) const override;
};

#endif // TRANSFORMATIONS_HPP
