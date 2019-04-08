#ifndef __TRANSFORMATIONS_HPP__
#define __TRANSFORMATIONS_HPP__

#include <string>

class Transformation {
public:
    virtual ~Transformation() {};
    virtual void transform(double elapsed) const = 0;
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
};

class RotateAnimated : public Transformation {
private:
    float dur, x, y, z;

public:
    RotateAnimated(float dur, float x, float y, float z)
        : dur(dur)
        , x(x)
        , y(y)
        , z(z)
    {
    }
    ~RotateAnimated() override {};
    void transform(double) const override;
};

class Translate : public Transformation {
private:
    float x, y, z;

public:
    Translate(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    ~Translate() override {};
    void transform(double) const override;
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
};

#endif // __TRANSFORMATIONS_HPP__
