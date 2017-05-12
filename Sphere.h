//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable {
public:
    Sphere() { }

    Sphere(const Vector3& cen, double r, Material* m)
            :
            center(cen),
            radius(r),
            material(m) { }

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const
    {
        Vector3 oc = r.origin()-center;
        double a = dot(r.direction(), r.direction());
        double b = dot(oc, r.direction());
        double c = dot(oc, oc)-radius*radius;
        double discriminant = b*b-a*c;
        if (discriminant>0) {
            double temp = (-b-sqrt(discriminant))/a;
            if (temp<tmax && temp>tmin) {
                rec.t = temp;
                rec.p = r.pointAt(rec.t);
                rec.normal = (rec.p-center)/radius;
                rec.material = material;
                //rec.normal.make_unit_vector();
                return true;
            }
            temp = (-b+sqrt(discriminant))/a;
            if (temp<tmax && temp>tmin) {
                rec.t = temp;
                rec.p = r.pointAt(rec.t);
                rec.normal = (rec.p-center)/radius;
                rec.material = material;
                //rec.normal.make_unit_vector();
                return true;
            }
        }
        return false;
    }

    Vector3 center;
    double radius;
    Material* material;
};

#endif //PATHTRACER_SPHERE_H
