#ifndef __TRANSFORMATIONS_HPP__
#define __TRANSFORMATIONS_HPP__

#include <string>

class Transformation {
public:
    virtual void transform() const = 0;
    virtual std::string to_string() const = 0;
};

class Rotate : public Transformation {
private:
    float angle, x, y, z;

public:
    Rotate(float angle, float x, float y, float z)
        : angle(angle)
        , x(x)
        , y(y)
        , z(z)
    {
    }
    void transform() const override;
    std::string to_string() const override { return "Rotate"; }
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
    void transform() const override;
    std::string to_string() const override { return "Translate"; }
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
    void transform() const override;
    std::string to_string() const override { return "Scale"; }
};

class Color : public Transformation {
private:
    float r, g, b;

public:
    Color(float r, float g, float b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
    void transform() const override;
    std::string to_string() const override { return "Color"; }
};

#endif // __TRANSFORMATIONS_HPP__
