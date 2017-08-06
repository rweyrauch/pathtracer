/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "ONB.h"

void ONB::buildFromW(const Vector3 &n)
{
    axis[2] = unit_vector(n);
    Vector3 a{};
    if (fabs(w().x()) > 0.9)
        a = Vector3(0, 1, 0);
    else
        a = Vector3(1, 0, 0);
    axis[1] = unit_vector(cross(w(), a));
    axis[0] = cross(w(), v());
}