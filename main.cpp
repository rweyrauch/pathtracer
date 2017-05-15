#include <iostream>
#include <cfloat>
#include "Sphere.h"
#include "HitableList.h"
#include "Vector3.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Rectangle.h"
#include "Medium.h"

Vector3 color(const Ray& r, Hitable* world, int depth)
{
    HitRecord rec;
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        Ray scattered;
        Vector3 attenuation(0, 0, 0);
        Vector3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
        if (depth<50 && rec.material->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation * color(scattered, world, depth+1);
        }
        else {
            return emitted;
        }
    }
    else {
        //Vector3 unit_dir = unit_vector(r.direction());
        //double t = 0.5*(unit_dir.y()+1.0);
        //return (1.0-t)*Vector3(1.0, 1.0, 1.0)+t*Vector3(0.5, 0.7, 1.0);
        return Vector3(0, 0, 0);
    }
}

Hitable* randomScene()
{
    std::vector<Hitable*> list;
    Texture* checker = new CheckerTexture(new ConstantTexture(Vector3(0.2, 0.3, 0.1)), new ConstantTexture(Vector3(0.9, 0.9, 0.9)));
    list.push_back(new Sphere(Vector3(0,-1000,0), 1000, new Lambertian(checker)));
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            double choose_mat = drand48();
            Vector3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if ((center-Vector3(4,0.2,0)).length() > 0.9)
            {
                if (choose_mat < 0.8) // diffuse
                {
                    list.push_back(new MovingSphere(center, center+Vector3(0, 0.5*drand48(),0), 0, 1, 0.2, new Lambertian(new ConstantTexture(Vector3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())))));
                }
                else if (choose_mat < 0.95) // metal
                {
                    list.push_back(new Sphere(center, 0.2, new Metal(Vector3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*drand48()), 0.3)));
                }
                else // glass
                {
                    list.push_back(new Sphere(center, 0.2, new Dielectric(1.5)));
                }
            }
        }
    }

    list.push_back(new Sphere(Vector3(0,1,0), 1.0, new Dielectric(1.5)));
    list.push_back(new Sphere(Vector3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vector3(0.4, 0.2, 0.1)))));
    list.push_back(new Sphere(Vector3(4, 1, 0), 1.0, new Metal(Vector3(0.7, 0.6, 0.5), 0.0)));

    return new HitableList(list);
}

Hitable* perlinSpheres()
{
    Texture* noise = new NoiseTexture(4); // ConstantTexture(Vector3(0.5, 0.5, 0.5)); //
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0,-1000, 0), 1000, new Lambertian(noise)));
    list.push_back(new Sphere(Vector3(0, 2, 0), 2, new Lambertian(noise)));
    return new HitableList(list);
}

Hitable* simpleLight()
{
    Texture* noise = new NoiseTexture(4); // ConstantTexture(Vector3(0.5, 0.5, 0.5)); //
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0,-1000, 0), 1000, new Lambertian(noise)));
    list.push_back(new Sphere(Vector3(0, 2, 0), 2, new Lambertian(noise)));

    list.push_back(new Sphere(Vector3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(Vector3(4, 4, 4)))));
    list.push_back(new XYRectangle(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(Vector3(4, 4, 4)))));

    return new HitableList(list);
}

Hitable* cornellBox()
{
    std::vector<Hitable*> list;

    Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vector3(15, 15, 15)));

    list.push_back(new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green)));
    list.push_back(new YZRectangle(0, 555, 0, 555, 0, red));
    list.push_back(new XZRectangle(213, 343, 227, 332, 554, light));
    list.push_back(new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white)));
    list.push_back(new XZRectangle(0, 555, 0, 555, 0, white));
    list.push_back(new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white)));

    //list.push_back(new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65)));
    //list.push_back(new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), 15), Vector3(265, 0, 295)));
    //list.push_back(new Translate(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), Vector3(130, 0, 65)));
    //list.push_back(new Translate(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), Vector3(265, 0, 295)));

    Hitable* b1 = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65));
    Hitable* b2 = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), 15), Vector3(265, 0, 295));

    list.push_back(new ConstantMedium(b1, 0.01, new ConstantTexture(Vector3(1, 1, 1))));
    list.push_back(new ConstantMedium(b2, 0.01, new ConstantTexture(Vector3(0, 0, 0))));

    return new HitableList(list);
}

int main()
{
    int nx = 320 * 4;
    int ny = 240 * 4;
    int ns = 100 * 10;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(new ConstantTexture(Vector3(0.1, 0.2, 0.5)))));
    list.push_back(new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(new ConstantTexture(Vector3(0.8, 0.8, 0.0)))));
    list.push_back(new Sphere(Vector3(1, 0, -1), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 0.3)));
    list.push_back(new Sphere(Vector3(-1, 0, -1), 0.5, new Dielectric(1.5)));
    list.push_back(new Sphere(Vector3(-1,0,-1), -0.45, new Dielectric(1.5)));

    //double R = cos(M_PI/4);
    //list.push_back(new Sphere(Vector3(-R,0,-1), R, new Lambertian(Vector3(0,0,1))));
    //list.push_back(new Sphere(Vector3(R,0,-1), R, new Lambertian(Vector3(1,0,0))));

    Hitable* world = cornellBox(); // simpleLight(); //randomScene(); //

    //Camera cam(90, double(nx)/ny);
    const Vector3 lookFrom(278, 278, -800); //(13, 2, 3);
    const Vector3 lookAt(278, 278, 0); //(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    Camera cam(lookFrom, lookAt, Vector3(0, 1, 0), 40, double(nx)/double(ny), aperture, dist_to_focus);

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
            col = Vector3(sqrt(std::max(0.0, col[0])), sqrt(std::max(0.0, col[1])), sqrt(std::max(0.0, col[2])));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}
