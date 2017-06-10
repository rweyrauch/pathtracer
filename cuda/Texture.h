//
// Created by rick on 5/11/17.
//

#ifndef PATHTRACER_TEXTURE_H
#define PATHTRACER_TEXTURE_H

#include "CudaCommon.h"
#include "Vector2.h"

class Texture
{
public:
    CALLABLE_FUNC virtual Vector3 value(const Vector2& uv, const Vector3 &p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    CALLABLE_FUNC ConstantTexture()
    {}

    CALLABLE_FUNC ConstantTexture(const Vector3 &c) :
        color(c)
    {}

    CALLABLE_FUNC virtual Vector3 value(const Vector2& uv, const Vector3 &p) const
    {
        return color;
    }

    Vector3 color;
};

class CheckerTexture : public Texture
{
public:
    CALLABLE_FUNC CheckerTexture()
    {}

    CALLABLE_FUNC CheckerTexture(Texture *t0, Texture *t1) :
        odd(t1),
        even(t0)
    {}

    CALLABLE_FUNC virtual Vector3 value(const Vector2& uv, const Vector3 &p) const
    {
        float sines = sinf(10.0f * p.x()) * sinf(10.0f * p.y()) * sinf(10.0f * p.z());
        if (sines < 0.0f)
            return odd->value(uv, p);
        else
            return even->value(uv, p);
    }

    Texture *odd;
    Texture *even;
};

#endif //PATHTRACER_TEXTURE_H
