#ifndef RGB_HPP
#define RGB_HPP

class RGB {
public:
    float r, g, b;
    RGB()
        : r(0)
        , g(0)
        , b(0)
    {
    }
    RGB(float r, float g, float b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
    RGB operator/(float val)
    {
        return RGB(
            r / val,
            g / val,
            b / val);
    }
};

#endif // RGB_HPP
