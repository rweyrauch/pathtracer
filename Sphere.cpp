//
// Created by rick on 5/11/17.
//
#include "Sphere.h"
#include "AABB.h"

void get_uv(const Vector3& p, double& u, double& v)
{
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI/2) / M_PI;
}

bool Sphere::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const
{
    Vector3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            get_uv(rec.p, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            get_uv(rec.p, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool Sphere::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
    return true;
}

Vector3 MovingSphere::center(double time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    Vector3 oc = ray.origin() - center(ray.time());
    double a = dot(ray.direction(), ray.direction());
    double b = dot(oc, ray.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = ray.pointAt(rec.t);
            rec.normal = (rec.p - center(ray.time())) / radius;
            rec.material = material;
            get_uv(rec.p, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = ray.pointAt(rec.t);
            rec.normal = (rec.p - center(ray.time())) / radius;
            rec.material = material;
            get_uv(rec.p, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool MovingSphere::bounds(double t0, double t1, AABB &bbox) const
{
    AABB box0 = AABB(center0 - Vector3(radius, radius, radius), center0 + Vector3(radius, radius, radius));
    AABB box1 = AABB(center1 - Vector3(radius, radius, radius), center1 + Vector3(radius, radius, radius));
    bbox = AABB::join(box0, box1);
    return true;
}