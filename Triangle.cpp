//
// Created by rick on 6/5/17.
//

#include "Triangle.h"

Triangle::Triangle(const Vector3& v0, const Vector2& t0,
         const Vector3& v1, const Vector2& t1,
         const Vector3& v2, const Vector2& t2,
         Material* mtl) :
    v0(v0),
    v1(v1),
    v2(v2),
    t0(t0),
    t1(t1),
    t2(t2),
    material(mtl)
    {
        calcBounds();
    }

bool Triangle::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    //
    // Tomas Moller and Ben Trumbore, "Fast Minimum Storage Ray-Triangle Intersection,"
    // Journal of Graphics Tools, Vol. 2, No. 1, pp. 21-28, 1997.
    //

    // Find vectors for two edges sharing v0.
    Vector3 edge1(v1 - v0);
    Vector3 edge2(v2 - v0);

    // Begin calculating determinant - also used to calculate U parameter.
    Vector3 pvec = cross(ray.direction(), edge2);

    // If determinant is near zero, ray lies in plane of triangle.
    auto det = dot(edge1, pvec);

    if (det < 0.0001)
        return false;

    // Calculate distance from v0 to ray origin.
    Vector3 tvec(ray.origin() - v0);

    // calculate U parameter and test bounds.
    float u = dot(tvec, pvec);
    if (u < 0 || u > det)
        return false;

    // Prepare to test V parameter.
    Vector3 qvec = cross(tvec, edge1);

    // Calculate V parameter and test bounds.
    auto v = dot(ray.direction(), qvec);
    if (v < 0 || u + v > det)
        return false;

    // Calculate t, scale parameters, ray intersects triangle.
    double t = dot(edge2, qvec);
    if (t < t_min || t > t_max) return false;

    const auto inv_det = 1 / det;
    u *= inv_det;
    v *= inv_det;

    rec.t = t;
    rec.p = (1 - u - v) * v0 + u * v1 + v * v2;
    rec.normal = cross(edge1, edge2);
    rec.normal.make_unit_vector();
    rec.material = material;

    Vector3 bary(1.0 - u - v, u, v);
    calcTexCoord(bary, rec.uv);

    return true;
}

bool Triangle::bounds(double t0, double t1, AABB &bbox) const
{
    bbox = this->bbox;
    return true;
}

double Triangle::area() const
{
    Vector3 u(v1 - v0);
    Vector3 v(v2 - v0);
    Vector3 uv = cross(u, v);
    return 0.5 * uv.length();
}

void Triangle::calcTexCoord(const Vector3& bary, Vector2& uv) const
{
    uv = t0 * bary.x() + t1 * bary.y() + t2 * bary.z();
}

void Triangle::calcBounds()
{
    Vector3 bmin, bmax;
    for (int i = 0; i < 3; i++)
    {
        bmin[i] = std::min(v0[i]-0.0001, std::min(v1[i]-0.0001, v2[i]-0.0001));
        bmax[i] = std::max(v0[i]+0.0001, std::max(v1[i]+0.0001, v2[i]+0.0001));
    }
    bbox = AABB(bmin, bmax);
}
