//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H

#include "Hitable.h"

void get_uv(const Vector3& p, double& u, double& v);

class Sphere : public Hitable {
public:
    Sphere() { }

    Sphere(const Vector3& cen, double r, Material* m)
            :
            center(cen),
            radius(r),
            material(m) { }

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    virtual double pdfValue(const Vector3& o, const Vector3& v) const;
    virtual Vector3 random(const Vector3& o) const;

    Vector3 center;
    double radius;
    Material* material;

};

class MovingSphere : public Hitable
{
public:
    MovingSphere() {}
    MovingSphere(const Vector3& cen0, const Vector3& cen1, double t0, double t1, double r, Material* mtl)
        :
        center0(cen0),
        center1(cen1),
        time0(t0),
        time1(t1),
        radius(r),
        material(mtl)
    {}

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Vector3 center(double time) const;

    Vector3 center0, center1;
    double time0, time1;
    double radius;
    Material* material;
};

#endif //PATHTRACER_SPHERE_H
