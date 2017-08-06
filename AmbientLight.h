/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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

    explicit ConstantAmbient(const Vector3& c) :
        color(c) {}

    Vector3 emitted(const Ray& ray) const override
    {
        return color;
    }

private:
    Vector3 color;
};

class SkyAmbient : public AmbientLight
{
public:
    SkyAmbient() = default;

    Vector3 emitted(const Ray& ray) const override
    {
        Vector3 unit_dir = unit_vector(ray.direction());
        double t = 0.5*(unit_dir.y()+1.0);
        return (1.0-t)*Vector3(1.0, 1.0, 1.0)+t*Vector3(0.5, 0.7, 1.0);
    }
};

#endif //PATHTRACER_AMBIENTLIGHT_H
