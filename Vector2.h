//
// Created by rweyra on 6/5/17.
//

#ifndef PATHTRACER_VECTOR2_H
#define PATHTRACER_VECTOR2_H

#include <iostream>
#include <cmath>

class Vector2 {
public:
    Vector2() { }

    Vector2(double e0, double e1) :
        e{e0, e1} { }

    inline double x() const { return e[0]; }
    inline double& x() { return e[0]; }

    inline double y() const { return e[1]; }
    inline double& y() { return e[1]; }

    inline double u() const { return e[0]; }
    inline double& u() { return e[0]; }

    inline double v() const { return e[1]; }
    inline double& v() { return e[1]; }

    inline const Vector2& operator+() const { return *this; }

    inline Vector2 operator-() const { return Vector2(-e[0], -e[1]); }

    inline double operator[](int i) const { return e[i]; }

    inline double& operator[](int i) { return e[i]; }

    inline Vector2& operator+=(const Vector2& v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        return *this;
    }

    inline Vector2& operator-=(const Vector2& v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        return *this;
    }

    inline Vector2& operator*=(const Vector2& v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        return *this;
    }

    inline Vector2& operator/=(const Vector2& v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        return *this;
    }

    inline Vector2& operator*=(const double s)
    {
        e[0] *= s;
        e[1] *= s;
        return *this;
    }

    inline Vector2& operator/=(const double s)
    {
        const double invS = 1/s;
        e[0] *= invS;
        e[1] *= invS;
        return *this;
    }

    inline double length() const
    {
        return sqrt(squared_length());
    }

    inline double squared_length() const
    {
        return e[0]*e[0] + e[1]*e[1];
    }

    inline void make_unit_vector();

    double e[2];
};

inline void Vector2::make_unit_vector()
{
    double k = 1.0 / length();
    e[0] *= k;
    e[1] *= k;
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1]);
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1]);
}

inline Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1]);
}

inline Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]/v2.e[0], v1.e[1]/v2.e[1]);
}

inline Vector2 operator*(double s, const Vector2& v2)
{
    return Vector2(s*v2.e[0], s*v2.e[1]);
}

inline Vector2 operator/(const Vector2& v1, double s)
{
    return Vector2(v1.e[0]/s, v1.e[1]/s);
}

inline Vector2 operator*(const Vector2& v1, double s)
{
    return Vector2(v1.e[0]*s, v1.e[1]*s);
}

inline double dot(const Vector2& v1, const Vector2& v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1];
}

inline Vector2 unit_vector(const Vector2& v)
{
    return v / v.length();
}

inline Vector2 reflect(const Vector2& v, const Vector2& n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vector2& v, const Vector2& n, double niOverNt, Vector2& refracted)
{
    Vector2 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

inline std::istream& operator>>(std::istream& is, Vector2& t)
{
    is >> t.e[0] >> t.e[1];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& t)
{
    os << t.e[0] << " " << t.e[1];
    return os;
}

#endif //PATHTRACER_VECTOR2_H
