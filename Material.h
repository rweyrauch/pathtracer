//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_MATERIAL_H
#define PATHTRACER_MATERIAL_H

#include "Vector3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

inline Vector3 randomInUnitSphere()
{
    Vector3 p;
    do {
        p = 2.0*Vector3(drand48(), drand48(), drand48())-Vector3(1, 1, 1);
    }
    while (p.squared_length()>=1.0);
    return p;
}

inline Vector3 randomInUnitDisk()
{
    Vector3 p;
    do {
        p = 2 * Vector3(drand48(), drand48(), 0) - Vector3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

inline double schlick(double cs, double ri)
{
    double r0 = (1-ri)/(1+ri);
    r0 = r0*r0;
    return r0*(1-r0)*pow((1-cs), 5);
}

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
    virtual Vector3 emitted(double u, double v, const Vector3& p) const {
        return Vector3(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(Texture* a)
            :
            albedo(a) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 target = rec.p + rec.normal + randomInUnitSphere();
        scattered = Ray(rec.p, target-rec.p);
        attenuation = albedo->value(0, 0, rec.p);
        return true;
    }

    Texture* albedo;
};

class Metal : public Material {
public:
    Metal(const Vector3& a, double f)
            :albedo(a), fuzz(1)
    {
        if (f<1) { fuzz = f; }
    }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected+fuzz*randomInUnitSphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal)>0);
    }

    Vector3 albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ri)
            :refIndex(ri) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 outwardNormal;
        Vector3 reflected = reflect(r_in.direction(), rec.normal);
        double niOverNt;
        attenuation = Vector3(1, 1, 1);
        Vector3 refracted;
        double reflectProb, cosine;
        if (dot(r_in.direction(), rec.normal)>0) {
            outwardNormal = -rec.normal;
            niOverNt = refIndex;
            cosine = refIndex*dot(r_in.direction(), rec.normal)/r_in.direction().length();
        }
        else {
            outwardNormal = rec.normal;
            niOverNt = 1/refIndex;
            cosine = -dot(r_in.direction(), rec.normal)/r_in.direction().length();
        }
        if (refract(r_in.direction(), outwardNormal, niOverNt, refracted)) {
            reflectProb = schlick(cosine, refIndex);
        }
        else {
            reflectProb = 1.0;
        }
        if (drand48()<reflectProb) {
            scattered = Ray(rec.p, reflected);
        }
        else {
            scattered = Ray(rec.p, refracted);
        }
        return true;
    }

    double refIndex;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture* a)
        :
        emit(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        return false;
    }
    virtual Vector3 emitted(double u, double v, const Vector3& p) const
    {
        return emit->value(u, v, p);
    }

    Texture* emit;
};

#endif //PATHTRACER_MATERIAL_H
