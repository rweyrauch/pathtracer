/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable
{
public:
    Sphere() = default;

    Sphere(const Vector3& cen, double r, Material* m) :
        center(cen),
        radius(r),
        material(m) { }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    double pdfValue(const Vector3& o, const Vector3& v) const override;

    Vector3 random(const Vector3& o) const override;

    void get_uv(const Vector3& p, Vector2& uv) const;

private:
    Vector3 center{};
    double radius = 0;
    Material* material = nullptr;

};

class MovingSphere : public Hitable
{
public:
    MovingSphere() = default;

    MovingSphere(const Vector3& cen0, const Vector3& cen1, double t0, double t1, double r, Material* mtl) :
        center0(cen0),
        center1(cen1),
        time0(t0),
        time1(t1),
        radius(r),
        material(mtl)
    {}

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    Vector3 center(double time) const;

    void get_uv(const Vector3& p, Vector2& uv) const;

private:
    Vector3 center0{}, center1{};
    double time0 = 0, time1 = 0;
    double radius = 0;
    Material* material = nullptr;
};

class Cone : public Hitable
{
public:
    Cone() = default;

    Cone(const Vector3& cen, double r, double h, Material* m) :
        center(cen),
        radius(r),
        height(h),
        material(m) { }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    double pdfValue(const Vector3& o, const Vector3& v) const override;

    Vector3 random(const Vector3& o) const override;

    void get_uv(const Vector3& p, Vector2& uv) const;

private:
    Vector3 center{};
    double radius = 0, height = 0;
    Material* material = nullptr;
};

#endif //PATHTRACER_SPHERE_H
