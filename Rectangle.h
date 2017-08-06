/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_RECTANGLE_H
#define PATHTRACER_RECTANGLE_H

#include "Hitable.h"
#include "HitableList.h"
#include "AABB.h"

class XYRectangle : public Hitable
{
public:
    XYRectangle() = default;

    XYRectangle(double X0, double X1, double Y0, double Y1, double K, Material* mat) :
        material(mat),
        x0(X0),
        x1(X1),
        y0(Y0),
        y1(Y1),
        k(K) {}

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

private:
    Material* material{};
    double x0{}, x1{}, y0{}, y1{}, k{};
};

class XZRectangle : public Hitable
{
public:
    XZRectangle() = default;

    XZRectangle(double X0, double X1, double Z0, double Z1, double K, Material* mat) :
        material(mat),
        x0(X0),
        x1(X1),
        z0(Z0),
        z1(Z1),
        k(K) {}

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    double pdfValue(const Vector3& o, const Vector3& v) const override
    {
        HitRecord rec;
        if (hit(Ray(o, v), 0.001, DBL_MAX, rec))
        {
            double area = (x1-x0) * (z1-z0);
            double distSqrd = rec.t * rec.t * v.squared_length();
            double cosine = fabs(dot(v, rec.normal) / v.length());
            return distSqrd / (cosine * area);
        }
        else
            return 0;
    }

    Vector3 random(const Vector3& o) const override
    {
        Vector3 randPoint = Vector3(x0 + drand48() * (x1-x0), k, z0 + drand48() * (z1-z0));
        return randPoint - o;
    }

private:
    Material* material{};
    double x0{}, x1{}, z0{}, z1{}, k{};
};

class YZRectangle : public Hitable
{
public:
    YZRectangle() = default;

    YZRectangle(double Y0, double Y1, double Z0, double Z1, double K, Material* mat) :
        material(mat),
        y0(Y0),
        y1(Y1),
        z0(Z0),
        z1(Z1),
        k(K) {}

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

private:
    Material* material{};
    double y0{}, y1{}, z0{}, z1{}, k{};
};

class FlipNormals : public Hitable
{
public:
    explicit FlipNormals(Hitable* p) :
        hitable(p) {}

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override
    {
        if (hitable->hit(r_in, t0, t1, rec))
        {
            rec.normal = -rec.normal;
            return true;
        }
        return false;
    }

    bool bounds(double t0, double t1, AABB& bbox) const override
    {
        return hitable->bounds(t0, t1, bbox);
    }

    int numChildren() const override
    { return 1 + hitable->numChildren(); }

private:
    Hitable* hitable;
};

class Box : public Hitable
{
public:
    Box() = default;

    Box(const Vector3& p0, const Vector3& p1, Material* mat);

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    int numChildren() const override
    { return 1 + child->numChildren(); }

private:
    Vector3 pmin{}, pmax{};
    Hitable* child{};
};

class Translate : public Hitable
{
public:
    Translate(Hitable *p, const Vector3 &displacement) :
        hitable(p),
        offset(displacement)
    {}

    bool hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const override
    {
        Ray movedR(r_in.origin() - offset, r_in.direction(), r_in.time());
        if (hitable->hit(movedR, t0, t1, rec))
        {
            rec.p += offset;
            return true;
        }
        return false;
    }

    bool bounds(double t0, double t1, AABB &bbox) const override
    {
        if (hitable->bounds(t0, t1, bbox))
        {
            bbox = AABB(bbox.min() + offset, bbox.max() + offset);
            return true;
        }
        return false;
    }

    int numChildren() const override
    { return 1 + hitable->numChildren(); }

private:
    Hitable* hitable;
    Vector3 offset;
};

class RotateY : public Hitable
{
public:
    RotateY(Hitable* p, double angle);

    bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override
    {
        bbox = this->bbox;
        return hasBox;
    }

    int numChildren() const override
    { return 1 + hitable->numChildren(); }

private:
    Hitable* hitable;
    double sinTheta, cosTheta;
    bool hasBox;
    AABB bbox;
};

#endif //PATHTRACER_RECTANGLE_H
