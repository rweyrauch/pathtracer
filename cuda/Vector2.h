//
// Created by rweyra on 6/5/17.
//

#ifndef PATHTRACER_VECTOR2_H
#define PATHTRACER_VECTOR2_H

#include <cmath>
#include "CudaCommon.h"

class Vector2 {
public:
    CALLABLE_FUNC Vector2() { }

    CALLABLE_FUNC Vector2(float e0, float e1) :
        e{e0, e1} { }

    CALLABLE_FUNC inline float x() const { return e[0]; }
    CALLABLE_FUNC inline float& x() { return e[0]; }

    CALLABLE_FUNC inline float y() const { return e[1]; }
    CALLABLE_FUNC inline float& y() { return e[1]; }

    CALLABLE_FUNC inline float u() const { return e[0]; }
    CALLABLE_FUNC inline float& u() { return e[0]; }

    CALLABLE_FUNC inline float v() const { return e[1]; }
    CALLABLE_FUNC inline float& v() { return e[1]; }

    CALLABLE_FUNC inline const Vector2& operator+() const { return *this; }

    CALLABLE_FUNC inline Vector2 operator-() const { return Vector2(-e[0], -e[1]); }

    CALLABLE_FUNC inline float operator[](int i) const { return e[i]; }

    CALLABLE_FUNC inline float& operator[](int i) { return e[i]; }

    CALLABLE_FUNC inline Vector2& operator+=(const Vector2& v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        return *this;
    }

    CALLABLE_FUNC inline Vector2& operator-=(const Vector2& v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        return *this;
    }

    CALLABLE_FUNC inline Vector2& operator*=(const Vector2& v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        return *this;
    }

    CALLABLE_FUNC inline Vector2& operator/=(const Vector2& v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        return *this;
    }

    CALLABLE_FUNC inline Vector2& operator*=(const float s)
    {
        e[0] *= s;
        e[1] *= s;
        return *this;
    }

    CALLABLE_FUNC inline Vector2& operator/=(const float s)
    {
        const float invS = 1/s;
        e[0] *= invS;
        e[1] *= invS;
        return *this;
    }

    CALLABLE_FUNC inline float length() const
    {
        return sqrt(squared_length());
    }

    CALLABLE_FUNC inline float squared_length() const
    {
        return e[0]*e[0] + e[1]*e[1];
    }

    CALLABLE_FUNC inline void make_unit_vector()
    {
        float k = 1.0 / length();
        e[0] *= k;
        e[1] *= k;
    }

    float e[2];
};

CALLABLE_FUNC inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1]);
}

CALLABLE_FUNC inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1]);
}

CALLABLE_FUNC inline Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1]);
}

CALLABLE_FUNC inline Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.e[0]/v2.e[0], v1.e[1]/v2.e[1]);
}

CALLABLE_FUNC inline Vector2 operator*(float s, const Vector2& v2)
{
    return Vector2(s*v2.e[0], s*v2.e[1]);
}

CALLABLE_FUNC inline Vector2 operator/(const Vector2& v1, float s)
{
    return Vector2(v1.e[0]/s, v1.e[1]/s);
}

CALLABLE_FUNC inline Vector2 operator*(const Vector2& v1, float s)
{
    return Vector2(v1.e[0]*s, v1.e[1]*s);
}

CALLABLE_FUNC inline float dot(const Vector2& v1, const Vector2& v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1];
}

CALLABLE_FUNC inline Vector2 unit_vector(const Vector2& v)
{
    return v / v.length();
}

CALLABLE_FUNC inline Vector2 reflect(const Vector2& v, const Vector2& n)
{
    return v - 2 * dot(v, n) * n;
}

CALLABLE_FUNC inline bool refract(const Vector2& v, const Vector2& n, float niOverNt, Vector2& refracted)
{
    Vector2 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

#endif //PATHTRACER_VECTOR2_H
