/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <cfloat>
#include "Sphere.h"
#include "AABB.h"
#include "ONB.h"
#include "PDF.h"

void Sphere::get_uv(const Vector3& p, Vector2& uv) const
{
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    uv.u() = 1 - (phi + M_PI) / (2 * M_PI);
    uv.v() = (theta + M_PI/2) / M_PI;
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
            get_uv(rec.normal, rec.uv);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            get_uv(rec.normal, rec.uv);
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

double Sphere::pdfValue(const Vector3& o, const Vector3& v) const
{
   HitRecord rec;
    if (hit(Ray(o, v), 0.001, DBL_MAX, rec)) {
        double cosThetaMax = sqrt(1 - radius * radius / (center - o).squared_length());
        double solidAngle = 2 * M_PI * (1 - cosThetaMax);
        return 1 / solidAngle;
    }
    return 0;
}

Vector3 Sphere::random(const Vector3& o) const
{
    Vector3 direction = center - o;
    double distSqrd = direction.squared_length();
    ONB uvw;
    uvw.buildFromW(direction);
    return uvw.local(randomToUnitSphere(radius, distSqrd));
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
            get_uv(rec.p, rec.uv);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = ray.pointAt(rec.t);
            rec.normal = (rec.p - center(ray.time())) / radius;
            rec.material = material;
            get_uv(rec.p, rec.uv);
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

void MovingSphere::get_uv(const Vector3& p, Vector2& uv) const
{
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    uv.u() = 1 - (phi + M_PI) / (2 * M_PI);
    uv.v() = (theta + M_PI/2) / M_PI;
}

bool Cone::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const
{
    double k = radius / height;
    k = k * k;
    const auto dx = r.direction().x();
    const auto dy = r.direction().y();
    const auto dz = r.direction().z();
    const auto ox = r.origin().x();
    const auto oy = r.origin().y();
    const auto oz = r.origin().z();

    double a = dx * dx + dy * dy - k * dz * dz;
    double b = 2.0 * (dx * ox + dy * oy - k * dz * (oz - height));
    double c = ox * ox + oy * oy - k * (oz - height) * (oz - height);

    double discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            // TODO: compute correct normal and uv
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            get_uv(rec.p, rec.uv);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.material = material;
            get_uv(rec.p, rec.uv);
            return true;
        }
    }

    return false;
}

bool Cone::bounds(double t0, double t1, AABB &bbox) const
{
    return false;
}

double Cone::pdfValue(const Vector3 &o, const Vector3 &v) const
{
    return Hitable::pdfValue(o, v);
}

Vector3 Cone::random(const Vector3 &o) const
{
    return Hitable::random(o);
}

void Cone::get_uv(const Vector3& p, Vector2& uv) const
{
    double phi = atan2(p.y(), p.x());
    if (phi < 0.0)
        phi += 2.0 * M_PI;
    uv.u() = phi / (2 * M_PI);
    uv.v() = p.z() / height;
}
