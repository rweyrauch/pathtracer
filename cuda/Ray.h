//
// Created by rweyra on 5/9/17.
//

#ifndef PATHTRACER_RAY_H
#define PATHTRACER_RAY_H

#include "CudaCommon.h"
#include "Vector3.h"

class Ray {
public:
    CALLABLE_FUNC Ray() { }

    CALLABLE_FUNC Ray(const Vector3& o, const Vector3& d, float t = 0.0f) :
        m_origin(o),
        m_dir(d),
        m_time(t) { }

    CALLABLE_FUNC const Vector3& origin() const { return m_origin; }

    CALLABLE_FUNC Vector3& origin() { return m_origin; }

    CALLABLE_FUNC const Vector3& direction() const { return m_dir; }

    CALLABLE_FUNC Vector3& direction() { return m_dir; }

    CALLABLE_FUNC float time() const { return m_time; }

    CALLABLE_FUNC Vector3 pointAt(float t) const { return m_origin + t * m_dir; }

protected:
    Vector3 m_origin;
    Vector3 m_dir;
    float m_time;
};

#endif //PATHTRACER_RAY_H