/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_TRIANGLE_H
#define PATHTRACER_TRIANGLE_H


#include <vector>
#include "Hitable.h"
#include "Texture.h"
#include "Vector2.h"
#include "AABB.h"

class Triangle : public Hitable
{
public:
    Triangle(const Vector3& v0, const Vector2& t0,
             const Vector3& v1, const Vector2& t1,
             const Vector3& v2, const Vector2& t2,
             Material* mtl);

    bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    bool bounds(double t0, double t1, AABB &bbox) const override;

    double area() const;

private:

    void calcTexCoord(const Vector3& xyz, Vector2& uv) const;
    void calcBounds();

    Vector3 v0;
    Vector3 v1;
    Vector3 v2;

    Vector2 t0;
    Vector2 t1;
    Vector2 t2;

    Material* material;

    AABB bbox;
};

struct TriIndex
{
    unsigned int i0, i1, i2;
};

class TriangleMesh : public Hitable
{
public:
    explicit TriangleMesh(Material* mtl) :
        material(mtl) { }

    bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    bool bounds(double t0, double t1, AABB &bbox) const override;

    void addVertex(const Vector3& p, const Vector3& n, const Vector2& tex);

    void addTriangle(const TriIndex& tri)
    {
        triangles.push_back(tri);
    }

    void complete();

private:

    struct TriangleFast
    {
        TriangleFast(const Vector3& v0, const Vector3& v1, const Vector3& v2);

        Vector3 m_v0;
        double  m_nu, m_nv, m_nd;   // Plane coeff of projection plane
        int     m_k;                // Projection plane index (X, Y, Z)

        double  m_bnu, m_bnv;       // Projection plane edge equations
        double  m_cnu, m_cnv;
    };

    bool hit(const Ray& r, const TriangleFast& accel, double& t, Vector3& bary) const;

    std::vector<Vector3> verts;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;

    std::vector<TriIndex> triangles;
    std::vector<TriangleFast> triAccel;

    Material* material;

    AABB bbox{};
};
#endif //PATHTRACER_TRIANGLE_H
