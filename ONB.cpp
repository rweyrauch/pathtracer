//
// Created by rick on 6/2/17.
//

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