//
// Created by rick on 5/19/17.
//

#ifndef PATHTRACER_WORLD_H
#define PATHTRACER_WORLD_H

#include <cstddef>
#include <cstdint>
#include "Hitable.h"

enum class TextureType : int
{
    Constant = 0,
    Checker,
    Noise,
    Image
};

struct TextureInfo
{
    TextureType type;
    union
    {
        struct // Constant
        {
            float color[3];
        };
        struct // Checker
        {
            int textureOdd;
            int textureEven;
        };
        struct // Noise
        {
            float scale;
        };
        struct // Image
        {
            int imageData;
            int nx, ny;
        };
    };
};

enum class MaterialType : int
{
    Lambertian,
    Metal,
    Dielectric,
    DiffuseLight,
    Isotropic
};

struct MaterialInfo
{
    MaterialType type;
    union
    {
        struct // Lambertian, Isotropic
        {
            int textureAlbedo;
        };
        struct // Metal
        {
            float albedo[3];
            float fuzz;
        };
        struct // Dielectric
        {
            float reflIndex;
        };
        struct // DiffuseLight
        {
            int textureEmit;
        };
    };
};

enum class HitableType : int
{
    List,
    Sphere,
    MovingSphere,
    BVH,
    Medium,
    XYRect,
    XZRect,
    YZRect,
    FlipNormals,
    Box,
    Translate,
    RotateY
};

struct HitableInfo
{
    HitableType type;
    int material;
    union
    {
        struct
        {
            size_t numChildren;
            ptrdiff_t listOffset;
        } _List;
        struct
        {
            float center[3];
            float radius;
        } _Sphere;
        struct
        {
            float center0[3];
            float time0;
            float center1[3];
            float time1;
            float radius;
        }  _MovingSphere;
        struct
        {
            ptrdiff_t leftChild, rightChild;
            float bboxMin[3];
            float bboxMax[3];
        } _BVHNode;
        struct
        {
            ptrdiff_t boundaryChild;
            float density;
        } _Medium;
        struct
        {
            float a0, a1;
            float b0, b1;
            float k;
        } _Rect;
        struct
        {
            ptrdiff_t child;
        } _FlipNormal;
        struct
        {
            float pmin[3];
            float pmax[3];
            ptrdiff_t child;
        } _Box;
        struct
        {
            ptrdiff_t child;
            float offset[3];
        } _Translate;
        struct
        {
            ptrdiff_t child;
            float sinTheta, cosTheta;
            bool hasBox;
            float bboxMin[3];
            float bboxMax[3];
        } _RotateY;
    };
};

HitableInfo* BuildWorld(Hitable* world);

#endif //PATHTRACER_WORLD_H
