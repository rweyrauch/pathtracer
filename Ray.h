//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_RAY_H
#define PATHTRACER_RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray() { }

    Ray(const Vector3& o, const Vector3& d)
            :
            m_origin(o),
            m_dir(d) { }

    const Vector3& origin() const { return m_origin; }

    Vector3& origin() { return m_origin; }

    const Vector3& direction() const { return m_dir; }

    Vector3& direction() { return m_dir; }

    Vector3 pointAt(double t) const { return m_origin+t*m_dir; }

protected:
    Vector3 m_origin;
    Vector3 m_dir;
};

#endif //PATHTRACER_RAY_H
