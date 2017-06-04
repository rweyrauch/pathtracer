//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_HITABLELIST_H
#define PATHTRACER_HITABLELIST_H

#include <vector>
#include "Hitable.h"

class HitableList : public Hitable {
public:
    HitableList() { }

    HitableList(const std::vector<Hitable*>& l)
            :list(l) { }

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        double closest_so_far = tmax;
        for (auto ip : list) {
            if (ip->hit(r, tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
    virtual bool bounds(double t0, double t1, AABB& bbox) const;

    virtual double pdfValue(const Vector3& o, const Vector3& v) const;
    virtual Vector3 random(const Vector3& o) const;

    virtual int numChildren() const
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
