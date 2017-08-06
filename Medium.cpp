/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <cfloat>
#include "Medium.h"

bool ConstantMedium::hit(const Ray &r_in, double t0, double t1, HitRecord &rec) const
{
    HitRecord rec1, rec2;
    if (boundary->hit(r_in, -DBL_MAX, DBL_MAX, rec1))
    {
        if (boundary->hit(r_in, rec1.t+0.0001, DBL_MAX, rec2))
        {
            if (rec1.t < t0) rec1.t = t0;
            if (rec2.t > t1) rec2.t = t1;
            if (rec1.t > rec2.t) return false;
            if (rec1.t < 0) rec1.t = 0;
            double distInsideBoundary = (rec2.t - rec1.t) * r_in.direction().length();
            double hitDist = -(1/density)*log(drand48());
            if (hitDist < distInsideBoundary)
            {
                rec.t = rec1.t + hitDist / r_in.direction().length();
                rec.p = r_in.pointAt(rec.t);
                rec.normal = Vector3(1, 0, 0);
                rec.material = phaseFunction;
                return true;
            }
        }
    }
    return false;
}
