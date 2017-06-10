//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_HITABLE_H
#define PATHTRACER_HITABLE_H

#include "CudaCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"

class Material;

struct HitRecord {
    float t;
    Vector3 p;
    Vector3 normal;
    Material* material;
    Vector2 uv;
};

class Hitable {
public:
    CALLABLE_FUNC virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};

#endif //PATHTRACER_HITABLE_H