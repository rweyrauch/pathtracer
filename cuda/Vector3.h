//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_VECTOR3_H
#define PATHTRACER_VECTOR3_H

#include <cmath>
#include "CudaCommon.h"

class Vector3 {
public:
    CALLABLE_FUNC Vector3() { }

    CALLABLE_FUNC Vector3(float e0, float e1, float e2)
    {
        e[0] = e0; e[1] = e1; e[2] = e2;
    }

    CALLABLE_FUNC inline float x() const { return e[0]; }
    CALLABLE_FUNC inline float& x() { return e[0]; }

    CALLABLE_FUNC inline float y() const { return e[1]; }
    CALLABLE_FUNC inline float& y() { return e[1]; }

    CALLABLE_FUNC inline float z() const { return e[2]; }
    CALLABLE_FUNC inline float& z() { return e[2]; }

    CALLABLE_FUNC inline float r() const { return e[0]; }
    CALLABLE_FUNC inline float g() const { return e[1]; }
    CALLABLE_FUNC inline float b() const { return e[2]; }

    CALLABLE_FUNC inline const Vector3& operator+() const { return *this; }

    CALLABLE_FUNC inline Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }

    CALLABLE_FUNC inline float operator[](int i) const { return e[i]; }

    CALLABLE_FUNC inline float& operator[](int i) { return e[i]; }

    CALLABLE_FUNC inline Vector3& operator+=(const Vector3& v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }

    CALLABLE_FUNC inline Vector3& operator-=(const Vector3& v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        e[2] -= v2.e[2];
        return *this;
    }

    CALLABLE_FUNC inline Vector3& operator*=(const Vector3& v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        e[2] *= v2.e[2];
        return *this;
    }

    CALLABLE_FUNC inline Vector3& operator/=(const Vector3& v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        e[2] /= v2.e[2];
        return *this;
    }

    CALLABLE_FUNC inline Vector3& operator*=(const float s)
    {
        e[0] *= s;
        e[1] *= s;
        e[2] *= s;
        return *this;
    }

    CALLABLE_FUNC inline Vector3& operator/=(const float s)
    {
        const float invS = 1.0f / s;
        e[0] *= invS;
        e[1] *= invS;
        e[2] *= invS;
        return *this;
    }

    CALLABLE_FUNC inline float length() const
    {
        return sqrtf(squared_length());
    }

    CALLABLE_FUNC inline float squared_length() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    CALLABLE_FUNC inline void make_unit_vector()
    {
        float k = 1.0f / length();
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
    }

    float e[3];
};

CALLABLE_FUNC inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]);
}

CALLABLE_FUNC inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1], v1.e[2]-v2.e[2]);
}

CALLABLE_FUNC inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1], v1.e[2]*v2.e[2]);
}

CALLABLE_FUNC inline Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.e[0]/v2.e[0], v1.e[1]/v2.e[1], v1.e[2]/v2.e[2]);
}

CALLABLE_FUNC inline Vector3 operator*(float s, const Vector3& v2)
{
    return Vector3(s*v2.e[0], s*v2.e[1], s*v2.e[2]);
}

CALLABLE_FUNC inline Vector3 operator/(const Vector3& v1, float s)
{
    return Vector3(v1.e[0]/s, v1.e[1]/s, v1.e[2]/s);
}

CALLABLE_FUNC inline Vector3 operator*(const Vector3& v1, float s)
{
    return Vector3(v1.e[0]*s, v1.e[1]*s, v1.e[2]*s);
}

CALLABLE_FUNC inline float dot(const Vector3& v1, const Vector3& v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

CALLABLE_FUNC inline Vector3 cross(const Vector3& v1, const Vector3& v2)
{
    return Vector3((v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                   (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                   (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}

CALLABLE_FUNC inline Vector3 unit_vector(const Vector3& v)
{
    return v / v.length();
}

CALLABLE_FUNC inline Vector3 reflect(const Vector3& v, const Vector3& n)
{
    return v - 2.0f * dot(v, n) * n;
}

CALLABLE_FUNC inline bool refract(const Vector3& v, const Vector3& n, float niOverNt, Vector3& refracted)
{
    Vector3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
    if (discriminant > 0.0f)
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrtf(discriminant);
        return true;
    }
    return false;
}

#endif //PATHTRACER_VECTOR3_H
