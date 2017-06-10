//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include <math_constants.h>
#include "CudaCommon.h"
#include "Material.h"
#include "Vector3.h"
#include "Ray.h"

class Camera {
public:
    CALLABLE_FUNC Camera() {}

    CALLABLE_FUNC Camera(float vfov, float aspect) :
        origin(0.0f, 0.0f, 0.0f),
        lowerLeftCorner(),
        horizontal(),
        vertical()
    {
        float theta = vfov * CUDART_PI_F / 180.0f;
        float halfHeight = tanf(theta / 2.0f);
        float halfWidth = aspect * halfHeight;
        lowerLeftCorner = Vector3(-halfWidth, -halfHeight, -1.0f);
        horizontal = Vector3(2.0f * halfWidth, 0.0f, 0.0f);
        vertical = Vector3(0.0f, 2.0f * halfHeight, 0.0f);
    }

    CALLABLE_FUNC Camera(const Vector3& from, const Vector3& to, const Vector3& vup, float vfov, float aspect, float aperture, float focal_dist, float t0 = 0.0f, float t1 = 1.0f) :
        origin(from),
        lowerLeftCorner(),
        horizontal(),
        vertical(),
        time0(t0),
        time1(t1)
    {
        lens_radius = aperture / 2.0f;
        float theta = vfov * CUDART_PI_F / 180.0f;
        float halfHeight = tanf(theta / 2.0f);
        float halfWidth = aspect * halfHeight;
        w = unit_vector(from - to);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lowerLeftCorner = origin - halfWidth * focal_dist * u - halfHeight * focal_dist * v - focal_dist * w;
        horizontal = 2 * halfWidth * focal_dist * u;
        vertical = 2 * halfHeight * focal_dist * v;
    }

    CALLABLE_FUNC Ray getRay(float s, float t, unsigned int *seed0, unsigned int *seed1)
    {
        Vector3 rd = lens_radius * randomInUnitDisk(seed0, seed1);
        Vector3 offset = u * rd.x() + v * rd.y();
        float time = time0 + rand(seed0, seed1) * (time1 - time0);
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, time);
    }

    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u, v, w;
    float time0, time1;
    float lens_radius;
};

#endif //PATHTRACER_CAMERA_H