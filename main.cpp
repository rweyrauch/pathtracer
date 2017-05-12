#include <iostream>
#include <values.h>
#include "Sphere.h"
#include "HitableList.h"
#include "Vector3.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"

Vector3 color(const Ray& r, Hitable* world, int depth)
{
    HitRecord rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        Ray scattered;
        Vector3 attenuation;
        if (depth<50 && rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return Vector3(0, 0, 0);
        }
    }
    else {
        Vector3 unit_dir = unit_vector(r.direction());
        double t = 0.5*(unit_dir.y()+1.0);
        return (1.0-t)*Vector3(1.0, 1.0, 1.0)+t*Vector3(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vector3(0.1, 0.2, 0.5))));
    list.push_back(new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vector3(0.8, 0.8, 0.0))));
    list.push_back(new Sphere(Vector3(1, 0, -1), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 0.3)));
    list.push_back(new Sphere(Vector3(-1, 0, -1), 0.5, new Dielectric(1.5)));
    //list.push_back(new Sphere(Vector3(-1,0,-1), -0.45, new Dielectric(1.5)));

    //double R = cos(M_PI/4);
    //list.push_back(new Sphere(Vector3(-R,0,-1), R, new Lambertian(Vector3(0,0,1))));
    //list.push_back(new Sphere(Vector3(R,0,-1), R, new Lambertian(Vector3(1,0,0))));

    Hitable* world = new HitableList(list);

    //Camera cam(90, double(nx)/ny);
    Camera cam(Vector3(-2, 2, 1), Vector3(0, 0, -1), Vector3(0, 1, 0), 90, double(nx)/double(ny));
    for (int j = ny-1; j>=0; j--) {
        for (int i = 0; i<nx; i++) {
            Vector3 col(0, 0, 0);
            for (int s = 0; s<ns; s++) {
                auto u = (i+drand48())/double(nx);
                auto v = (j+drand48())/double(ny);
                Ray r = cam.getRay(u, v);
                col += color(r, world, 0);
            }
            col /= double(ns);
            col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}