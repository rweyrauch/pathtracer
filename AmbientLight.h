//
// Created by rick on 6/6/17.
//

#ifndef PATHTRACER_AMBIENTLIGHT_H
#define PATHTRACER_AMBIENTLIGHT_H

#include "Vector3.h"
#include "Ray.h"

class AmbientLight
{
public:
    virtual Vector3 emitted(const Ray& ray) const = 0;
};

class ConstantAmbient : public AmbientLight
{
public:
    ConstantAmbient() :
        color(0, 0, 0) {}

    ConstantAmbient(const Vector3& c) :
        color(c) {}

    virtual Vector3 emitted(const Ray& ray) const {
        return color;
    }

private:
    Vector3 color;
};

class SkyAmbient : public AmbientLight
{
public:
    SkyAmbient() {}
    virtual Vector3 emitted(const Ray& ray) const {
        Vector3 unit_dir = unit_vector(ray.direction());
        double t = 0.5*(unit_dir.y()+1.0);
        return (1.0-t)*Vector3(1.0, 1.0, 1.0)+t*Vector3(0.5, 0.7, 1.0);
    }
};

#endif //PATHTRACER_AMBIENTLIGHT_H
