//
// Created by rick on 5/14/17.
//

#ifndef PATHTRACER_AABB_H
#define PATHTRACER_AABB_H

#include "Vector3.h"
#include "Ray.h"

class AABB
{
public:
    AABB() {}
    AABB(const Vector3& a, const Vector3& b)
        :
        m_min(a),
        m_max(b) {}

    const Vector3& min() const { return m_min; }
    const Vector3& max() const { return m_max; }

    bool hit(const Ray& r, double tmin, double tmax) const;

    static AABB join(const AABB& box0, const AABB& box1);

protected:
    Vector3 m_min;
    Vector3 m_max;
};


#endif //PATHTRACER_AABB_H
