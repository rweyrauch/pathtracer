//
// Created by rick on 5/14/17.
//

#ifndef PATHTRACER_BVH_H
#define PATHTRACER_BVH_H

#include <vector>
#include "Hitable.h"
#include "AABB.h"

class BVH : public Hitable
{
public:
    BVH() {}
    BVH(std::vector<Hitable*>& list, double time0, double time1);

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    Hitable* left;
    Hitable* right;
    AABB m_bbox;
};


#endif //PATHTRACER_BVH_H
