//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_HITABLELIST_H
#define PATHTRACER_HITABLELIST_H

#include "CudaCommon.h"
#include "Hitable.h"

class HitableList : public Hitable {
public:
    CALLABLE_FUNC HitableList() { }

    CALLABLE_FUNC HitableList(int c, Hitable** l) :
        count(c),
        list(l) {}

    CALLABLE_FUNC virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        float closest_so_far = tmax;
        for (int i = 0; i < count; i++) {
            if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    int count;
    Hitable** list;
};

#endif //PATHTRACER_HITABLELIST_H