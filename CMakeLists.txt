cmake_minimum_required(VERSION 3.7)
project(pathtracer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        Vector3.h
        Ray.h
        Hitable.h
        Sphere.h
        Sphere.cpp
        HitableList.h
        Camera.h
        Material.h
        Perlin.h
        Perlin.cpp
        Noise.cpp
        Noise.h
        Rectangle.cpp
        Rectangle.h
        AABB.cpp
        AABB.h HitableList.cpp BVH.cpp BVH.h)

add_executable(pathtracer ${SOURCE_FILES} HitableList.cpp BVH.cpp BVH.h)