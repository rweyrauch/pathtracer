//
// Created by rick on 6/2/17.
//

#ifndef PATHTRACER_ONB_H
#define PATHTRACER_ONB_H

#include "Vector3.h"

class ONB
{
public:
    ONB() = default;

    Vector3 operator[](int i) const { return axis[i]; }
    const Vector3& u() const { return axis[0]; }
    const Vector3& v() const { return axis[1]; }
    const Vector3& w() const { return axis[2]; }

    Vector3 local(double a, double b, double c) const { return a * u() + b * v() + c * w(); }
    Vector3 local(const Vector3& a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }
    void buildFromW(const Vector3& n);

private:
    Vector3 axis[3];
};

#endif //PATHTRACER_ONB_H
