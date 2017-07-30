//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_HITABLE_H
#define PATHTRACER_HITABLE_H

#include "Vector3.h"
#include "Ray.h"
#include "Vector2.h"

class Material;

class AABB;

struct HitRecord
{
    double t{};
    Vector3 p{};
    Vector3 normal{};
    Material *material{};
    Vector2 uv;
};

class Hitable
{
public:

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const = 0;

    virtual bool bounds(double t0, double t1, AABB &bbox) const = 0;

    virtual int numChildren() const
    {
        return 0;
    }

    virtual double pdfValue(const Vector3 &o, const Vector3 &v) const
    {
        return 0;
    }

    virtual Vector3 random(const Vector3 &o) const
    {
        return {1, 0, 0};
    }

};

inline bool Quadradic(double a, double b, double c, double& t0, double& t1)
{
    const double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0)
    {
        return false;
    }

    const double sqrtDisc = sqrt(discriminant);

    double q;
    if (b < 0)
        q = -(b - sqrtDisc) * 0.5;
    else
        q = -(b + sqrtDisc) * 0.5;
    t0 = q / a;
    t1 = c / q;
    if (t0 > t1) std::swap(t0, t1);

    return true;
}

#endif //PATHTRACER_HITABLE_H
