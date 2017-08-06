/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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
    Vector3 bmin{}, bmax{};
    for (int i = 0; i < 3; i++)
    {
        bmin[i] = std::min(v0[i]-0.0001, std::min(v1[i]-0.0001, v2[i]-0.0001));
        bmax[i] = std::max(v0[i]+0.0001, std::max(v1[i]+0.0001, v2[i]+0.0001));
    }
    bbox = AABB(bmin, bmax);
}


bool TriangleMesh::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    return false;
}

bool TriangleMesh::hit(const Ray& ray, const TriangleFast& accel, double& tHit, Vector3& bary) const
{
    //
    // "Real Time Ray Tracing and Interactive Global Illumination", Ingo Wald:
    // http://www.mpi-sb.mpg.de/~wald/PhD/
    //
    // Jakko Bikker
    // http://www.flipcode.com/articles/article_raytrace07.shtml
    //
    static int axisModulo[] = { 0, 1, 2, 0, 1 };
    const int ku = axisModulo[accel.m_k+1];
    const int kv = axisModulo[accel.m_k+2];

    const double nd = 1 / (ray.direction()[accel.m_k] + accel.m_nu * ray.direction()[ku] + accel.m_nv * ray.direction()[kv]);
    double t = (accel.m_nd - ray.origin()[accel.m_k] - accel.m_nu * ray.origin()[ku] - accel.m_nv * ray.origin()[kv]) * nd;

    if (t < 0)
    {
        return false;
    }

    const double hu = ray.origin()[ku] + t * ray.direction()[ku] - accel.m_v0[ku];
    const double hv = ray.origin()[kv] + t * ray.direction()[kv] - accel.m_v0[kv];

    const double u = hv * accel.m_bnu + hu * accel.m_bnv;
    if (u < 0)
        return false;

    const double v = hu * accel.m_cnu + hv * accel.m_cnv;
    if (v < 0)
        return false;

    if (u + v > 1)
        return false;

    tHit = t;
    bary = Vector3(1 - u - v, u, v);

    return true;
}

bool TriangleMesh::bounds(double t0, double t1, AABB &bbox) const
{
    return false;
}

void TriangleMesh::addVertex(const Vector3& p, const Vector3& n, const Vector2& tex)
{
    verts.push_back(p);
    normals.push_back(n);
    texCoords.push_back(tex);
}

void TriangleMesh::complete()
{
    triAccel.clear();
    for (const auto ip : triangles)
    {
        TriangleFast triFast(verts[ip.i0], verts[ip.i1],  verts[ip.i2]);
        triAccel.push_back(triFast);
    }
}

TriangleMesh::TriangleFast::TriangleFast(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
    m_v0 = v0;

    // Find vectors for two edges sharing v0.
    Vector3 c(v1 - v0);
    Vector3 b(v2 - v0);

    // Compute normal
    Vector3 N = cross(b, c);

    // Identify primary plane
    if (fabs(N.x()) > fabs(N.y()))
    {
        if (fabs(N.x()) > fabs(N.z()))
            m_k = 0;
        else
            m_k = 2;
    }
    else
    {
        if (fabs(N.y()) > fabs(N.z()))
            m_k = 1;
        else
            m_k = 2;
    }

    // Compute triangle plane coefficients in projection plane
    int u = (m_k+1) % 3;
    int v = (m_k+2) % 3;
    double invNormPP = 1.0 / N[m_k];

    m_nu = N[u] * invNormPP;
    m_nv = N[v] * invNormPP;
    m_nd = dot(N, v0) * invNormPP;

    // Compute projection plane edge equations
    double invDet = 1.0 / (b[u] * c[v] - b[v] * c[u]);
    m_bnu = b[u] * invDet;
    m_bnv = -b[v] * invDet;

    m_cnu = c[v] * invDet;
    m_cnv = -c[u] * invDet;
}
