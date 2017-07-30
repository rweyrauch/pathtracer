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
    BVH() = default;

    BVH(std::vector<Hitable*>& list, double time0, double time1);

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;

    bool bounds(double t0, double t1, AABB& bbox) const override;

    double pdfValue(const Vector3& o, const Vector3& v) const override;

    Vector3 random(const Vector3& o) const override;

    int numChildren() const override
    { return 2 + left->numChildren() + right->numChildren(); }

    Hitable* left{};
    Hitable* right{};
    AABB m_bbox{};
};


#endif //PATHTRACER_BVH_H
