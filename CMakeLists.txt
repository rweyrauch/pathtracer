cmake_minimum_required(VERSION 3.7)
project(pathtracer)

find_package(OpenMP)

set(CMAKE_CXX_STANDARD 11)

if (OpenMP_FOUND)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
endif()

set(SOURCE_FILES
        main.cpp
        Vector3.h
        Vector2.h
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
        ONB.h
        ONB.cpp
        PDF.h
        Triangle.cpp
        Triangle.h
        AmbientLight.h)

add_executable(pathtracer ${SOURCE_FILES})
