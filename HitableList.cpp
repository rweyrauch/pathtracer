//
// Created by rick on 5/14/17.
//

#include "HitableList.h"
#include "AABB.h"

bool HitableList::bounds(double t0, double t1, AABB &bbox) const
{
    if (list.empty()) return false;

    AABB tempBox;
    bool first = list.front()->bounds(t0, t1, tempBox);
    if (!first)
        return false;
    else
        bbox = tempBox;

    for (int i = 1; i < list.size(); i++)
    {
        if (list[i]->bounds(t0, t1, tempBox))
        {
            bbox = AABB::join(bbox, tempBox);
        }
        else
            return false;
    }
    return true;
}
