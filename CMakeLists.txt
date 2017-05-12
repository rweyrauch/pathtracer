cmake_minimum_required(VERSION 3.7)
project(pathtracer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        Vector3.h
        Ray.h
        Hitable.h
        Sphere.h
        HitableList.h
        Camera.h
        Material.h)

add_executable(pathtracer ${SOURCE_FILES})