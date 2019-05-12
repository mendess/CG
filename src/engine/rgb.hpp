#ifndef RGBA_HPP
#define RGBA_HPP

#include <array>

class RGBA {
public:
    float r, g, b, a;
    RGBA()
        : r(0)
        , g(0)
        , b(0)
        , a(1)
    {
    }
    RGBA(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }
    RGBA operator/(float val)
    {
        return RGBA(
            r / val,
            g / val,
            b / val,
            a / val);
    }
    RGBA operator*(float val)
    {
        return RGBA(
            r * val,
            g * val,
            b * val,
            a / val);
    }
    std::array<float, 4> to_array() const
    {
        return { r, g, b, a };
    }
};

#endif // RGBA_HPP
