//
// Created by rick on 5/14/17.
//

#include "HitableList.h"
#include "AABB.h"

bool HitableList::bounds(double t0, double t1, AABB &bbox) const
{
    if (list.empty()) return false;

    AABB tempBox{};
    bool first = list.front()->bounds(t0, t1, tempBox);
    if (!first)
        return false;

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

double HitableList::pdfValue(const Vector3& o, const Vector3& v) const
{
    double weight = 1 / (double)list.size();
    double sum = 0;
    for (const auto ip : list)
    {
        sum += weight * ip->pdfValue(o, v);
    }
    return sum;
}

Vector3 HitableList::random(const Vector3& o) const
{
    auto index = size_t(drand48() * list.size());
    return list.at(index)->random(o);
}
