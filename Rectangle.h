//
// Created by rick on 5/14/17.
//

#ifndef PATHTRACER_RECTANGLE_H
#define PATHTRACER_RECTANGLE_H

#include "Hitable.h"
#include "HitableList.h"
#include "AABB.h"

class XYRectangle : public Hitable
{
public:
    XYRectangle() {}
    XYRectangle(double X0, double X1, double Y0, double Y1, double K, Material* mat)
        :
        material(mat),
        x0(X0),
        x1(X1),
        y0(Y0),
        y1(Y1),
        k(K) {}

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Material* material;
    double x0, x1, y0, y1, k;
};

class XZRectangle : public Hitable
{
public:
    XZRectangle() {}
    XZRectangle(double X0, double X1, double Z0, double Z1, double K, Material* mat)
        :
        material(mat),
        x0(X0),
        x1(X1),
        z0(Z0),
        z1(Z1),
        k(K) {}

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Material* material;
    double x0, x1, z0, z1, k;
};

class YZRectangle : public Hitable
{
public:
    YZRectangle() {}
    YZRectangle(double Y0, double Y1, double Z0, double Z1, double K, Material* mat)
        :
        material(mat),
        y0(Y0),
        y1(Y1),
        z0(Z0),
        z1(Z1),
        k(K) {}

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Material* material;
    double y0, y1, z0, z1, k;
};

class FlipNormals : public Hitable
{
public:
    FlipNormals(Hitable* p) :
        hitable(p) {}

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const
    {
        if (hitable->hit(r_in, t0, t1, rec)) {
            rec.normal = -rec.normal;
            return true;
        }
        return false;
    }

    virtual bool bounds(double t0, double t1, AABB& bbox) const
    {
        return hitable->bounds(t0, t1, bbox);
    }

    Hitable* hitable;
};

class Box : public Hitable
{
public:
    Box() {}
    Box(const Vector3& p0, const Vector3& p1, Material* mat);

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Vector3 pmin, pmax;
    HitableList* list;
};

class Translate : public Hitable
{
public:
    Translate(Hitable *p, const Vector3 &displacement) :
        hitable(p),
        offset(displacement)
    {}

    virtual bool hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
    {
        Ray movedR(r_in.origin() - offset, r_in.direction(), r_in.time());
        if (hitable->hit(movedR, t0, t1, rec))
        {
            rec.p += offset;
            return true;
        }
        return false;
    }

    virtual bool bounds(double t0, double t1, AABB &bbox) const
    {
        if (hitable->bounds(t0, t1, bbox))
        {
            bbox = AABB(bbox.min() + offset, bbox.max() + offset);
            return true;
        }
        return false;
    }

    Hitable* hitable;
    Vector3 offset;
};

class RotateY : public Hitable
{
public:
    RotateY(Hitable* p, double angle);

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const
    {
        bbox = this->bbox;
        return hasBox;
    }

    Hitable* hitable;
    double sinTheta, cosTheta;
    bool hasBox;
    AABB bbox;
};
#endif //PATHTRACER_RECTANGLE_H
