//
// Created by rick on 5/15/17.
//

#ifndef PATHTRACER_MEDIUM_H
#define PATHTRACER_MEDIUM_H

#include "Hitable.h"
#include "Texture.h"
#include "Material.h"

class ConstantMedium : public Hitable
{
public:
    ConstantMedium(Hitable* b, double d, Texture* a) :
        boundary(b),
        density(d)
    {
        phaseFunction = new Isotropic(a);
    }

    virtual bool hit(const Ray& r_in, double t0, double t1, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const
    {
        return boundary->bounds(t0, t1, bbox);
    }

    Hitable* boundary;
    double density;
    Material* phaseFunction;
};


#endif //PATHTRACER_MEDIUM_H
