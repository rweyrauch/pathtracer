//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_VECTOR3_H
#define PATHTRACER_VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3 {
public:
    Vector3() = default;

    Vector3(double e0, double e1, double e2) :
        e{e0, e1, e2} { }

    inline double x() const { return e[0]; }
    inline double& x() { return e[0]; }

    inline double y() const { return e[1]; }
    inline double& y() { return e[1]; }

    inline double z() const { return e[2]; }
    inline double& z() { return e[2]; }

    inline double r() const { return e[0]; }

    inline double g() const { return e[1]; }

    inline double b() const { return e[2]; }

    inline const Vector3& operator+() const { return *this; }

    inline Vector3 operator-() const { return {-e[0], -e[1], -e[2]}; }

    inline double operator[](int i) const { return e[i]; }

    inline double& operator[](int i) { return e[i]; }

    inline Vector3& operator+=(const Vector3& v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }

    inline Vector3& operator-=(const Vector3& v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        e[2] -= v2.e[2];
        return *this;
    }

    inline Vector3& operator*=(const Vector3& v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        e[2] *= v2.e[2];
        return *this;
    }

    inline Vector3& operator/=(const Vector3& v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        e[2] /= v2.e[2];
        return *this;
    }

    inline Vector3& operator*=(const double s)
    {
        e[0] *= s;
        e[1] *= s;
        e[2] *= s;
        return *this;
    }

    inline Vector3& operator/=(const double s)
    {
        const double invS = 1/s;
        e[0] *= invS;
        e[1] *= invS;
        e[2] *= invS;
        return *this;
    }

    inline double length() const
    {
        return sqrt(squared_length());
    }

    inline double squared_length() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    inline void make_unit_vector();

    double e[3];
};

inline void Vector3::make_unit_vector()
{
    double k = 1.0 / length();
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return {v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]};
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return {v1.e[0]-v2.e[0], v1.e[1]-v2.e[1], v1.e[2]-v2.e[2]};
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return {v1.e[0]*v2.e[0], v1.e[1]*v2.e[1], v1.e[2]*v2.e[2]};
}

inline Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return {v1.e[0]/v2.e[0], v1.e[1]/v2.e[1], v1.e[2]/v2.e[2]};
}

inline Vector3 operator*(double s, const Vector3& v2)
{
    return {s*v2.e[0], s*v2.e[1], s*v2.e[2]};
}

inline Vector3 operator/(const Vector3& v1, double s)
{
    return {v1.e[0]/s, v1.e[1]/s, v1.e[2]/s};
}

inline Vector3 operator*(const Vector3& v1, double s)
{
    return {v1.e[0]*s, v1.e[1]*s, v1.e[2]*s};
}

inline double dot(const Vector3& v1, const Vector3& v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline Vector3 cross(const Vector3& v1, const Vector3& v2)
{
    return {(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                   (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                   (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0])};
}

inline Vector3 unit_vector(const Vector3& v)
{
    return v / v.length();
}

inline Vector3 reflect(const Vector3& v, const Vector3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vector3& v, const Vector3& n, double niOverNt, Vector3& refracted)
{
    Vector3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

inline std::istream& operator>>(std::istream& is, Vector3& t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

#endif //PATHTRACER_VECTOR3_H
