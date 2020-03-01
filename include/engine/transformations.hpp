#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "../common/point.hpp"
#include <iostream>
#include <string>
#include <vector>

typedef struct matrix {
    float matrix[4][4];
} Matrix;

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
        : dur(dur * 1000)
        , x(x)
        , y(y)
        , z(z)
    {
    }
    ~RotateAnimated() override {};
    void transform(double) const override;
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
};

class TranslateAnimated : public Transformation {
private:
    static inline bool show_routes = false;
    std::vector<Point> points;
    float dur;

    std::tuple<Point, Vector> get_position(double) const;

public:
    TranslateAnimated(std::vector<Point> points, float dur)
        : points(points)
        , dur(dur * 1000)
    {
    }
    ~TranslateAnimated() override {};
    void transform(double) const override;
    void draw_routes() const;
    static void toggle_routes() { show_routes = !show_routes; };
    template<typename F>
    static void run_without_routes(F& action) {
        bool routes_val = show_routes;
        show_routes = false;
        action();
        show_routes = routes_val;
    }
};

class ScaleStatic : public Transformation {
private:
    float x, y, z;

public:
    ScaleStatic(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    ~ScaleStatic() override {};
    void transform(double) const override;
};

class ScaleAnimated : public Transformation {
private:
    float xi, yi, zi, xf, yf, zf, dur;

    std::tuple<float, float, float> get_ratio(double) const;

public:
    ScaleAnimated(float xi, float yi, float zi,
        float xf, float yf, float zf,
        float dur)
        : xi(xi)
        , yi(yi)
        , zi(zi)
        , xf(xf)
        , yf(yf)
        , zf(zf)
        , dur(dur * 1000)
    {
    }
    ~ScaleAnimated() override {};
    void transform(double) const override;
};
#endif // TRANSFORMATIONS_HPP
