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
        Camera.h
        Material.h
        Perlin.h
        Perlin.cpp
        Noise.cpp
        Noise.h
        Rectangle.cpp
        Rectangle.h
        AABB.cpp
        AABB.h
        HitableList.cpp
        HitableList.h
        BVH.cpp
        BVH.h
        Medium.cpp
        Medium.h
        Texture.h
        Texture.cpp
        Progress.cpp
        Progress.h
        World.h
        World.cpp
        ONB.h
        ONB.cpp)

add_executable(pathtracer ${SOURCE_FILES})