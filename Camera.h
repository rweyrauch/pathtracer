//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

class Camera
{
public:
    Camera() = default;

    Camera(double vfov, double aspect) :
        origin(0, 0, 0),
        lowerLeftCorner(),
        horizontal(),
        vertical()
    {
        double theta = vfov * M_PI / 180;
        double halfHeight = tan(theta / 2);
        double halfWidth = aspect * halfHeight;
        lowerLeftCorner = Vector3(-halfWidth, -halfHeight, -1);
        horizontal = Vector3(2 * halfWidth, 0, 0);
        vertical = Vector3(0, 2 * halfHeight, 0);
    }

    Camera(const Vector3 &from, const Vector3 &to, const Vector3 &vup, double vfov, double aspect, double aperture, double focal_dist, double t0 = 0,
           double t1 = 1) :
        origin(from),
        lowerLeftCorner(),
        horizontal(),
        vertical(),
        time0(t0),
        time1(t1)
    {
        lens_radius = aperture / 2;
        double theta = vfov * M_PI / 180;
        double halfHeight = tan(theta / 2);
        double halfWidth = aspect * halfHeight;
        w = unit_vector(from - to);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lowerLeftCorner = origin - halfWidth * focal_dist * u - halfHeight * focal_dist * v - focal_dist * w;
        horizontal = 2 * halfWidth * focal_dist * u;
        vertical = 2 * halfHeight * focal_dist * v;
    }

    Ray getRay(double s, double t)
    {
        Vector3 rd = lens_radius * randomInUnitDisk();
        Vector3 offset = u * rd.x() + v * rd.y();
        double time = time0 + drand48() * (time1 - time0);
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, time);
    }

private:
    Vector3 origin{};
    Vector3 lowerLeftCorner{};
    Vector3 horizontal{};
    Vector3 vertical{};
    Vector3 u{}, v{}, w{};
    double time0{}, time1{};
    double lens_radius{};
};

#endif //PATHTRACER_CAMERA_H
