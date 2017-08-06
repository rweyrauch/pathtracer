/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_RAY_H
#define PATHTRACER_RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray() = default;

    Ray(const Vector3& o, const Vector3& d, double t = 0) :
        m_origin(o),
        m_dir(d),
        m_time(t) { }

    const Vector3& origin() const { return m_origin; }

    Vector3& origin() { return m_origin; }

    const Vector3& direction() const { return m_dir; }

    Vector3& direction() { return m_dir; }

    double time() const { return m_time; }

    Vector3 pointAt(double t) const { return m_origin + t * m_dir; }

protected:
    Vector3 m_origin;
    Vector3 m_dir;
    double m_time;
};

#endif //PATHTRACER_RAY_H
