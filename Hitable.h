//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_HITABLE_H
#define PATHTRACER_HITABLE_H

#include "Vector3.h"
#include "Ray.h"

class Material;
class AABB;

struct HitRecord {
  double t;
  Vector3 p;
  Vector3 normal;
  Material* material;
  double u, v;
};

class Hitable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounds(double t0, double t1, AABB& bbox) const = 0;
};

#endif //PATHTRACER_HITABLE_H
