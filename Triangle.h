//
// Created by rick on 6/5/17.
//

#ifndef PATHTRACER_TRIANGLE_H
#define PATHTRACER_TRIANGLE_H


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

#endif //PATHTRACER_TRIANGLE_H
