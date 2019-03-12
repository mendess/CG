#ifndef __TRANSFORMATIONS_HPP__
#define __TRANSFORMATIONS_HPP__

class Transformation {
public:
    virtual void transform() const = 0;
};

class Rotate : Transformation {
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
};

class Translate : Transformation {
private:
    float x, y, z;

public:
    Translate(float x, float y, float z)
        : x(x)
        , y(y)
        , z(y)
    {
    }
    void transform() const override;
};

class Scale : Transformation {
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
};

class Color : Transformation {
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
};

#endif // __TRANSFORMATIONS_HPP__
