//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_MATERIAL_H
#define PATHTRACER_MATERIAL_H

#include "CudaCommon.h"
#include "Vector3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

CALLABLE_FUNC float rand(unsigned int *seed0, unsigned int *seed1)
{
    *seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);  // hash the seeds using bitwise AND and bitshifts
    *seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

    unsigned int ires = ((*seed0) << 16) + (*seed1);

    // Convert to float
    union {
        float f;
        unsigned int ui;
    } res;

    res.ui = (ires & 0x007fffff) | 0x40000000;  // bitwise AND, bitwise OR

    return (res.f - 2.0f) / 2.0f;
}

CALLABLE_FUNC inline Vector3 randomInUnitSphere(unsigned int *seed0, unsigned int *seed1)
{
    const float phi = rand(seed0, seed1) * 2.0f * CUDART_PI_F;
    const float z = 1.0f - 2.0f * rand(seed0, seed1);
    const float r = sqrtf(max(0.0f, 1.0f - z * z));
    return Vector3(r * cosf(phi), r * sinf(phi), z);
}

CALLABLE_FUNC inline Vector3 randomInUnitDisk(unsigned int *seed0, unsigned int *seed1)
{
    const float r = sqrtf(rand(seed0, seed1));
    const float theta = rand(seed0, seed1) * 2.0f * CUDART_PI_F;
    return Vector3(r * cosf(theta), r * sinf(theta), 0.0f);
}

CALLABLE_FUNC inline float schlick(float cs, float ri)
{
    float r0 = (1.0f - ri) / (1.0f + ri);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * powf((1.0f - cs), 5.0f);
}

class Material
{
public:
    CALLABLE_FUNC virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, unsigned int *seed0, unsigned int *seed1) const = 0;
    CALLABLE_FUNC virtual Vector3 emitted(const Vector2& uv, const Vector3& p) const { return Vector3(0.0f, 0.0f, 0.0f); }
};

class Lambertian : public Material
{
public:
    CALLABLE_FUNC Lambertian(Texture* a) :
        albedo(a) { }

    CALLABLE_FUNC virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, unsigned int *seed0, unsigned int *seed1) const
    {
        Vector3 target = rec.p + rec.normal * randomInUnitSphere(seed0, seed1);
        scattered = Ray(rec.p, target-rec.p);
        attenuation = albedo->value(rec.uv, rec.p);
        return true;
    }

    Texture* albedo;
};

class Metal : public Material
{
public:
    CALLABLE_FUNC Metal(const Vector3& a, float f) :
        albedo(a),
        fuzz(1)
    {
        if (f < 1.0f) { fuzz = f; }
    }

    CALLABLE_FUNC virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, unsigned int *seed0, unsigned int *seed1) const
    {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected+fuzz*randomInUnitSphere(seed0, seed1));
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0.0f);
    }

    Vector3 albedo;
    float fuzz;
};

class Dielectric : public Material
{
public:
    CALLABLE_FUNC Dielectric(float ri) :
        refIndex(ri) { }

    CALLABLE_FUNC virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, unsigned int *seed0, unsigned int *seed1) const
    {
        Vector3 outwardNormal;
        Vector3 reflected = reflect(r_in.direction(), rec.normal);
        float niOverNt;
        attenuation = Vector3(1.0f, 1.0f, 1.0f);
        Vector3 refracted;
        float reflectProb, cosine;
        if (dot(r_in.direction(), rec.normal) > 0.0f)
        {
            outwardNormal = -rec.normal;
            niOverNt = refIndex;
            cosine = refIndex * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.0f / refIndex;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectProb = schlick(cosine, refIndex);
        }
        else
        {
            reflectProb = 1.0f;
        }
        if (rand(seed0, seed1) < reflectProb)
        {
            scattered = Ray(rec.p, reflected);
        }
        else
        {
            scattered = Ray(rec.p, refracted);
        }
        return true;
    }

    float refIndex;
};

class DiffuseLight : public Material
{
public:
    CALLABLE_FUNC DiffuseLight(Texture* a) :
        emit(a) {}

    CALLABLE_FUNC virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, unsigned int *seed0, unsigned int *seed1) const
    {
        return false;
    }
    CALLABLE_FUNC virtual Vector3 emitted(const Vector2& uv, const Vector3& p) const
    {
        return emit->value(uv, p);
    }

private:
    Texture* emit;
};


#endif //PATHTRACER_MATERIAL_H