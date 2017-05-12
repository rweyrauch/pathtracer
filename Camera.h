//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "Vector3.h"
#include "Ray.h"

class Camera {
public:
    Camera(double vfov, double aspect)
            :
            origin(0, 0, 0),
            lowerLeftCorner(),
            horizontal(),
            vertical()
    {
        double theta = vfov*M_PI/180;
        double halfHeight = tan(theta/2);
        double halfWidth = aspect*halfHeight;
        lowerLeftCorner = Vector3(-halfWidth, -halfHeight, -1);
        horizontal = Vector3(2*halfWidth, 0, 0);
        vertical = Vector3(0, 2*halfHeight, 0);
    }

    Camera(const Vector3& from, const Vector3& to, const Vector3& vup, double vfov, double aspect)
            :
            origin(from),
            lowerLeftCorner(),
            horizontal(),
            vertical()
    {
        auto w = unit_vector(from-to);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);
        double theta = vfov*M_PI/180;
        double halfHeight = tan(theta/2);
        double halfWidth = aspect*halfHeight;
        lowerLeftCorner = origin-halfWidth*u-halfHeight*w;
        horizontal = 2*halfWidth*u;
        vertical = 2*halfHeight*v;
    }

    Ray getRay(double u, double v)
    {
        return Ray(origin, lowerLeftCorner+u*horizontal+v*vertical-origin);
    }

    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
};

#endif //PATHTRACER_CAMERA_H
