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
#include "PDF.h"

class Pdf;

struct ScatterRecord {
    Ray specularRay;
    bool isSpecular;
    Vector3 attenuation;
    Pdf* pdf;
};

inline double schlick(double cs, double ri)
{
    double r0 = (1-ri)/(1+ri);
    r0 = r0*r0;
    return r0*(1-r0)*pow((1-cs), 5);
}

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const = 0;
    virtual double scatteringPdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
        return 0.0;
    }
    virtual Vector3 emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Vector3& p) const {
        return Vector3(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(Texture* a) :
            albedo(a) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
    {
        srec.isSpecular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf = new CosinePdf(rec.normal);
        return true;
    }
    virtual double scatteringPdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
        double cosine = dot(rec.normal, unit_vector(scattered.direction()));
        if (cosine < 0) return 0;
        return cosine / M_PI;
    }

    Texture* albedo;
};

class Metal : public Material {
public:
    Metal(const Vector3& a, double f) :
        albedo(a),
        fuzz(1)
    {
        if (f<1) { fuzz = f; }
    }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
    {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specularRay = Ray(rec.p, reflected + fuzz*randomInUnitSphere());
        srec.attenuation = albedo;
        srec.isSpecular = true;
        srec.pdf = nullptr;
        return true;
    }

    Vector3 albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ri) :
        refIndex(ri) { }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
    {
        Vector3 outwardNormal;
        Vector3 reflected = -reflect(r_in.direction(), rec.normal);
        srec.pdf = nullptr;
        srec.isSpecular = true;
        srec.attenuation = Vector3(1, 1, 1);
        Vector3 refracted;
        double reflectProb, cosine;
        double niOverNt;
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
            srec.specularRay = Ray(rec.p, reflected);
        }
        else {
            srec.specularRay = Ray(rec.p, refracted);
        }
        return true;
    }

    double refIndex;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture* a) :
        emit(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
    {
        return false;
    }
    virtual Vector3 emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Vector3& p) const
    {
        if (dot(rec.normal, r_in.direction()) < 0)
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

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
    {
        // TODO: fix this for new ScatterRecord
        srec.isSpecular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf = new CosinePdf(rec.normal);
        return true;
    }

    Texture* albedo;
};


#endif //PATHTRACER_MATERIAL_H
