#ifndef __TRANSFORMATIONS_HPP__
#define __TRANSFORMATIONS_HPP__

#include <string>

class Transformation {
public:
    virtual void transform() const = 0;
    virtual std::string to_string() const = 0;
    virtual ~Transformation() {};
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
    ~Rotate() override {};
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
    ~Translate() override {};
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
    ~Scale() override {};
};

#endif // __TRANSFORMATIONS_HPP__
