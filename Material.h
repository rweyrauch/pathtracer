//
// Created by rweyra on 5/10/17.
//

#ifndef PATHTRACER_MATERIAL_H
#define PATHTRACER_MATERIAL_H

#include "Vector3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"
#include "ONB.h"

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

inline Vector3 randomCosineDirection()
{
    double r1 = drand48();
    double r2 = drand48();
    double z = sqrt(1 - r2);
    double phi = 2 * M_PI * r1;
    double x = cos(phi) * 2 * sqrt(r2);
    double y = sin(phi) * 2 * sqrt(r2);
    return Vector3(x, y, z);
}

inline double schlick(double cs, double ri)
{
    double r0 = (1-ri)/(1+ri);
    r0 = r0*r0;
    return r0*(1-r0)*pow((1-cs), 5);
}

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const = 0;
    virtual double scatteringPdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
        return 0.0;
    }
    virtual Vector3 emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Vector3& p) const {
        return Vector3(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(Texture* a)
            :
            albedo(a) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const
    {
        ONB uvw;
        uvw.buildFromW(rec.normal);

        Vector3 direction = uvw.local(randomCosineDirection());
        scattered = Ray(rec.p, unit_vector(direction), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        pdf = dot(rec.normal, scattered.direction()) / M_PI;
        return true;
    }
    virtual double scatteringPdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
        double cosine = dot(rec.normal, unit_vector(scattered.direction()));
        if (cosine < 0) cosine = 0;
        return cosine / M_PI;
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

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const
    {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*randomInUnitSphere());
        attenuation = albedo;
        pdf = 1;
        return (dot(scattered.direction(), rec.normal)>0);
    }

    Vector3 albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ri)
            :refIndex(ri) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const
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
            //cosine = refIndex*dot(r_in.direction(), rec.normal)/r_in.direction().length();
            cosine = dot(r_in.direction(), rec.normal)/r_in.direction().length();
            cosine = sqrt(1 - refIndex*refIndex*(1-cosine*cosine));
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
        pdf = 1;
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

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const
    {
        return false;
    }
    virtual Vector3 emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Vector3& p) const
    {
        if (dot(rec.normal, r_in.direction()) > 0)
            return emit->value(u, v, p);
        else
            return Vector3(0, 0, 0);
    }

    Texture* emit;
};

class Isotropic : public Material
{
public:
    Isotropic(Texture* a) :
        albedo(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered, double& pdf) const
    {
        scattered = Ray(rec.p, randomInUnitSphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        pdf = 1.0;
        return true;
    }

    Texture* albedo;
};
#endif //PATHTRACER_MATERIAL_H
