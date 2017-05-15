//
// Created by rick on 5/14/17.
//

#include "AABB.h"

/*
bool AABB::hit(const Ray& r, double tmin, double tmax) const
{
    for (int a = 0; a < 3; a++)
    {
        auto t0 = fmin((m_min[a] - r.origin()[a]) / r.direction()[a],
                       (m_max[a] - r.origin()[a]) / r.direction()[a]);
        auto t1 = fmax((m_min[a] - r.origin()[a]) / r.direction()[a],
                       (m_max[a] - r.origin()[a]) / r.direction()[a]);
        tmin = fmax(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin) return false;
    }
    return true;
}
*/

bool AABB::hit(const Ray& r, double tmin, double tmax) const
{
    for (int a = 0; a < 3; a++)
    {
        const auto invD = 1.0 / r.direction()[a];
        auto t0 = (m_min[a] - r.origin()[a]) * invD;
        auto t1 = (m_max[a] - r.origin()[a]) * invD;
        if (invD < 0.0)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) return false;
    }
    return true;
}

AABB AABB::join(const AABB& box0, const AABB& box1)
{
    Vector3 small(fmin(box0.min().x(), box1.min().x()),
                  fmin(box0.min().y(), box1.min().y()),
                  fmin(box0.min().z(), box1.min().z()));
    Vector3 big(fmax(box0.max().x(), box1.max().x()),
                fmax(box0.max().y(), box1.max().y()),
                fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}
