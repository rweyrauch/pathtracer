/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_PDF_H
#define PATHTRACER_PDF_H


#include "Vector3.h"
#include "ONB.h"

inline Vector3 randomInUnitSphere()
{
    Vector3 p{};
    do
    {
        p = 2.0*Vector3(drand48(), drand48(), drand48())-Vector3(1, 1, 1);
    }
    while (p.squared_length()>=1.0);
    return p;
}

inline Vector3 randomInUnitDisk()
{
    Vector3 p{};
    do
    {
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
    return {x, y, z};
}

inline Vector3 randomToUnitSphere(double radius, double distSqrd)
{
    double r1 = drand48();
    double r2 = drand48();
    double z = 1 + r2 * (sqrt(1 - radius*radius/distSqrd) - 1);
    double phi = 2 * M_PI * r1;
    double x = cos(phi)*sqrt(1-z*z);
    double y = sin(phi)*sqrt(1-z*z);
    return {x, y, z};
}

class Pdf
{
public:
    virtual ~Pdf() = default;

    virtual double value(const Vector3& direction) const = 0;
    virtual Vector3 generate() const = 0;
};

class ConstPdf : public Pdf
{
public:
    double value(const Vector3& direction) const override
    { return 1; };

    Vector3 generate() const override
    { return randomToUnitSphere(1, 1); };
};

class CosinePdf : public Pdf
{
public:
    explicit CosinePdf(const Vector3& w) { uvw.buildFromW(w); }

    double value(const Vector3& direction) const override
    {
        double cosine = dot(unit_vector(direction), uvw.w());
        if (cosine > 0)
            return cosine / M_PI;

        return 0;
    }

    Vector3 generate() const override
    {
        return uvw.local(randomCosineDirection());
    }

private:
    ONB uvw{};
};

class HitablePdf : public Pdf
{
public:
    HitablePdf(Hitable* p, const Vector3& o) :
        origin(o),
        hitable(p) {}

    double value(const Vector3& direction) const override
    {
        return hitable->pdfValue(origin, direction);
    }

    Vector3 generate() const override
    {
        return hitable->random(origin);
    }

private:
    Vector3 origin;
    Hitable* hitable = nullptr;
};

class MixturePdf : public Pdf
{
public:
    MixturePdf(Pdf* p0, Pdf* p1) { p[0] = p0; p[1] = p1; }

    double value(const Vector3& direction) const override
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    Vector3 generate() const override
    {
        if (drand48() < 0.5)
            return p[0]->generate();

        return p[1]->generate();
    }

private:
    Pdf* p[2] = {nullptr, nullptr};
};
#endif //PATHTRACER_PDF_H
