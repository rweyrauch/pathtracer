//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H

#include "CudaCommon.h"
#include "Hitable.h"

class Sphere : public Hitable {
public:
    CALLABLE_FUNC Sphere() { }

    CALLABLE_FUNC Sphere(const Vector3& cen, float r, Material* m) :
        center(cen),
        radius(r),
        material(m) { }

    CALLABLE_FUNC virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
    {
        Vector3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b*b-a*c;
        if (discriminant > 0.0f)
        {
            float temp = (-b - sqrtf(discriminant)) / a;
            if (temp < tmax && temp > tmin)
            {
                rec.t = temp;
                rec.p = r.pointAt(rec.t);
                rec.normal = (rec.p-center)/radius;
                rec.material = material;
                return true;
            }
            temp = (-b + sqrtf(discriminant)) / a;
            if (temp<tmax && temp>tmin) {
                rec.t = temp;
                rec.p = r.pointAt(rec.t);
                rec.normal = (rec.p-center)/radius;
                rec.material = material;
                return true;
            }
        }
        return false;
    }

    Vector3 center;
    float radius;
    Material* material;
};

#endif //PATHTRACER_SPHERE_H