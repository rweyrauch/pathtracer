//
// Created by rick on 5/14/17.
//

#include <cfloat>
#include "Rectangle.h"
#include "AABB.h"

bool XYRectangle::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    double t = (k - r_in.origin().z()) / r_in.direction().z();
    if (t < t0 || t > t1) return false;
    double x = r_in.origin().x() + t * r_in.direction().x();
    double y = r_in.origin().y() + t * r_in.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) return false;

    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.material = material;
    rec.p = r_in.pointAt(t);
    rec.normal = Vector3(0, 0, 1);

    return true;
}

bool XYRectangle::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = AABB(Vector3(x0, y0, k-0.0001), Vector3(x1, y1, k+0.0001));
    return true;
}

bool XZRectangle::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    double t = (k - r_in.origin().y()) / r_in.direction().y();
    if (t < t0 || t > t1) return false;
    double x = r_in.origin().x() + t * r_in.direction().x();
    double z = r_in.origin().z() + t * r_in.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1) return false;

    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.material = material;
    rec.p = r_in.pointAt(t);
    rec.normal = Vector3(0, 1, 0);

    return true;
}

bool XZRectangle::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = AABB(Vector3(x0, k-0.0001, z0), Vector3(x1, k+0.0001, z1));
    return true;
}

bool YZRectangle::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    double t = (k - r_in.origin().x()) / r_in.direction().x();
    if (t < t0 || t > t1) return false;
    double y = r_in.origin().y() + t * r_in.direction().y();
    double z = r_in.origin().z() + t * r_in.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1) return false;

    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.material = material;
    rec.p = r_in.pointAt(t);
    rec.normal = Vector3(1, 0, 0);

    return true;
}

bool YZRectangle::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = AABB(Vector3(k-0.0001, y0, z0), Vector3(k+0.0001, y1, z1));
    return true;
}

Box::Box(const Vector3 &p0, const Vector3 &p1, Material *mat) :
    pmin(p0),
    pmax(p1)
{
    std::vector<Hitable*> hl;
    hl.push_back(new XYRectangle(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
    hl.push_back(new FlipNormals(new XYRectangle(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat)));
    hl.push_back(new XZRectangle(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
    hl.push_back(new FlipNormals(new XZRectangle(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat)));
    hl.push_back(new YZRectangle(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
    hl.push_back(new FlipNormals(new YZRectangle(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat)));
    child = new HitableList(hl);
}

bool Box::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    return child->hit(r_in, t0, t1, rec);
}

bool Box::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = AABB(pmin, pmax);
    return true;
}

RotateY::RotateY(Hitable *p, double angle)
{
    hitable = p;
    double radians = (M_PI / 180.0) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasBox = p->bounds(0, 1, bbox);
    Vector3 min(DBL_MAX, DBL_MAX, DBL_MAX);
    Vector3 max(-DBL_MAX, -DBL_MAX, -DBL_MAX);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                double x = i * bbox.max().x() + (1-i)*bbox.min().x();
                double y = j * bbox.max().y() + (1-j)*bbox.min().y();
                double z = k * bbox.max().z() + (1-k)*bbox.min().z();
                double newx = cosTheta * x + sinTheta * z;
                double newz = -sinTheta * x + cosTheta * z;
                Vector3 tester(newx, y, newz);
                for (int c = 0; c < 3; c++)
                {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool RotateY::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    Vector3 origin = r_in.origin();
    Vector3 direction = r_in.direction();

    origin[0] = cosTheta*r_in.origin()[0] - sinTheta*r_in.origin()[2];
    origin[2] = sinTheta*r_in.origin()[0] + cosTheta*r_in.origin()[2];

    direction[0] = cosTheta*r_in.direction()[0] - sinTheta*r_in.direction()[2];
    direction[2] = sinTheta*r_in.direction()[0] + cosTheta*r_in.direction()[2];

    Ray rotatedR(origin, direction, r_in.time());
    if (hitable->hit(rotatedR, t0, t1, rec))
    {
        Vector3 p = rec.p;
        Vector3 normal = rec.normal;
        p[0] = cosTheta*rec.p[0] + sinTheta*rec.p[2];
        p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];
        normal[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
        normal[2] = -sinTheta*rec.normal[2] + cosTheta*rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    return false;
}
