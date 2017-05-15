//
// Created by rick on 5/11/17.
//

#ifndef PATHTRACER_TEXTURE_H
#define PATHTRACER_TEXTURE_H

#include <cassert>
#include "Perlin.h"
#include "Noise.h"

class Texture
{
public:
    virtual Vector3 value(double u, double v, const Vector3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() {}
    ConstantTexture(const Vector3& c)
        :
        color(c) {}

    virtual Vector3 value(double u, double v, const Vector3& p) const {
        return color;
    }

    Vector3 color;
};

class CheckerTexture : public Texture{
public:
    CheckerTexture() {}
    CheckerTexture(Texture* t0, Texture* t1)
        :
        odd(t1),
        even(t0) {}

    virtual Vector3 value(double u, double v, const Vector3& p) const {
        double sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

    Texture* odd;
    Texture* even;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(double sc)
        :
        scale(sc) {}

    virtual Vector3 value(double u, double v, const Vector3& p) const {
        //double n = (Noise(p) + 1) / 2;
        //double n = Turbulence(scale * p);
        double n = 0.5 * (1 + sin(scale * p.z() + 10*perlin.turbulence(p)));
        assert(n >= 0.0);
        return Vector3(n, n, n);
    }

    double scale = 1.0;
    Perlin perlin;
};

class ImageTexture : public Texture
{
public:
    ImageTexture() {}
    ImageTexture(const unsigned char* pixels, int Nx, int Ny)
        :
        data(pixels),
        nx(Nx),
        ny(Ny) {}

    virtual Vector3 value(double u, double v, const Vector3& p) const;

    const unsigned char* data;
    int nx, ny;
};
#endif //PATHTRACER_TEXTURE_H
