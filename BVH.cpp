//
// Created by rick on 5/14/17.
//

#include <algorithm>
#include "BVH.h"

bool BVH::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const
{
    if (m_bbox.hit(r, tmin, tmax))
    {
        HitRecord leftRec, rightRec;
        bool hitLeft = left->hit(r, tmin, tmax, leftRec);
        bool hitRight = right->hit(r, tmin, tmax, rightRec);
        if (hitLeft && hitRight)
        {
            if (leftRec.t < rightRec.t)
                rec = leftRec;
            else
                rec = rightRec;
            return true;
        }
        else if (hitLeft)
        {
            rec = leftRec;
            return true;
        }
        else if (hitRight)
        {
            rec = rightRec;
            return true;
        }
        else
            return false;
    }
    return false;
}

bool BVH::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = m_bbox;
    return true;
}

BVH::BVH(std::vector<Hitable *> &list, double time0, double time1)
{
    int axis = int(3 * drand48());
    if (axis == 0)
    {
        auto boxXCompare = [](Hitable* a, Hitable* b)->bool
        {
            AABB boxLeft, boxRight;
            if (!a->bounds(0, 0, boxLeft) || !b->bounds(0, 0, boxRight))
                std::cerr << "No bounding box in BVH construction." << std::endl;
            return (boxLeft.min().x() < boxRight.min().x());
        };
        std::sort(list.begin(), list.end(), boxXCompare);
    }
    else if (axis == 1)
    {
        auto boxYCompare = [](Hitable* a, Hitable* b)->bool
        {
            AABB boxLeft, boxRight;
            if (!a->bounds(0, 0, boxLeft) || !b->bounds(0, 0, boxRight))
                std::cerr << "No bounding box in BVH construction." << std::endl;
            return (boxLeft.min().y() < boxRight.min().y());
        };
        std::sort(list.begin(), list.end(), boxYCompare);
    }
    else
    {
        auto boxZCompare = [](Hitable* a, Hitable* b)->bool
        {
            AABB boxLeft, boxRight;
            if (!a->bounds(0, 0, boxLeft) || !b->bounds(0, 0, boxRight))
                std::cerr << "No bounding box in BVH construction." << std::endl;
            return (boxLeft.min().z() < boxRight.min().z());
        };
        std::sort(list.begin(), list.end(), boxZCompare);
    }
    const auto n = list.size();
    if (n == 1)
    {
        left = right = list.front();
    }
    else if (n == 2)
    {
        left = list.front();
        right = list.back();
    }
    else
    {
        auto leftNodes = std::vector<Hitable*>(list.begin(), list.begin()+n/2);
        left = new BVH(leftNodes, time0, time1);
        auto rightNodes = std::vector<Hitable*>(list.begin()+n/2, list.end());
        right = new BVH(rightNodes, time0, time1);
    }
    AABB boxLeft, boxRight;
    if (!left->bounds(time0, time1, boxLeft) || !right->bounds(time0, time1, boxRight))
        std::cerr << "no bound box in BVH node constructor." << std::endl;

    m_bbox = AABB::join(boxLeft, boxRight);
}

double BVH::pdfValue(const Vector3& o, const Vector3& v) const
{
    double weight = 0.5;
    double sum = weight * left->pdfValue(o, v) + weight * right->pdfValue(o, v);
    return sum;
}

Vector3 BVH::random(const Vector3& o) const
{
    if (drand48() < 0.5)
        return left->random(o);
    else
        return right->random(o);
}
