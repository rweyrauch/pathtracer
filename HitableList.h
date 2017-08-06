/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_HITABLELIST_H
#define PATHTRACER_HITABLELIST_H

#include <utility>
#include <vector>
#include "Hitable.h"

class HitableList : public Hitable {
public:
    HitableList() = default;

    explicit HitableList(std::vector<Hitable *> l) :
        list(std::move(l)) { }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        double closest_so_far = tmax;
        for (auto ip : list)
        {
            if (ip->hit(r, tmin, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    bool bounds(double t0, double t1, AABB& bbox) const override;

    double pdfValue(const Vector3& o, const Vector3& v) const override;

    Vector3 random(const Vector3& o) const override;

    int numChildren() const override
    {
        int numChildren = 1;
        for (auto ip : list)
        {
            numChildren += ip->numChildren();
        }
        return numChildren;
    }

    std::vector<Hitable*> list;

};

#endif //PATHTRACER_HITABLELIST_H
