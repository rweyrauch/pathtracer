/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_TEXTURE_H
#define PATHTRACER_TEXTURE_H

#include <cassert>
#include "Perlin.h"
#include "Noise.h"
#include "Vector2.h"

class Texture
{
public:
    virtual Vector3 value(const Vector2& uv, const Vector3 &p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() = default;

    explicit ConstantTexture(const Vector3 &c) :
        color(c)
    {}

    Vector3 value(const Vector2& uv, const Vector3 &p) const override
    {
        return color;
    }

private:
    Vector3 color{};
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;

    CheckerTexture(Texture *t0, Texture *t1) :
        odd(t1),
        even(t0)
    {}

    Vector3 value(const Vector2& uv, const Vector3 &p) const override
    {
        double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(uv, p);
        else
            return even->value(uv, p);
    }

private:
    Texture *odd = nullptr;
    Texture *even = nullptr;
};

class NoiseTexture : public Texture
{
public:
    explicit NoiseTexture(double sc) :
        scale(sc)
    {}

    Vector3 value(const Vector2& uv, const Vector3 &p) const override
    {
        //double n = (Noise(p) + 1) / 2;
        //double n = Turbulence(scale * p);
        //double n = 0.5 * (1 + sin(scale * p.z() + 10 * perlin.turbulence(p)));
        double n = 0.5 * (1 + sin(scale * p.z() + 10 * Turbulence(p)));
        assert(n >= 0.0);
        return {n, n, n};
    }

private:
    double scale = 1.0;
    Perlin perlin;
};

class ImageTexture : public Texture
{
public:
    ImageTexture()
    = default;

    ImageTexture(const unsigned char *pixels, int Nx, int Ny) :
        data(pixels),
        nx(Nx),
        ny(Ny)
    {}

    Vector3 value(const Vector2& uv, const Vector3 &p) const override;

private:
    const unsigned char *data{};
    int nx = 0, ny= 0;
};

#endif //PATHTRACER_TEXTURE_H
