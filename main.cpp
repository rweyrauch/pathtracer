#include <iostream>
#include <cfloat>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include "Vector3.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "cxxopts.hpp"
#include "World.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Rectangle.h"
#include "Medium.h"
#include "BVH.h"
#include "Progress.h"
#include "PDF.h"

Vector3 color(const Ray& r, Hitable* world, Hitable* lightShape, int depth)
{
    HitRecord rec;
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        ScatterRecord srec;
        Vector3 emitted = rec.material->emitted(r, rec, rec.u, rec.v, rec.p);
        if (depth<50 && rec.material->scatter(r, rec, srec)) {
            if (srec.isSpecular) {
                return srec.attenuation * color(srec.specularRay, world, lightShape, depth+1);
            }
            else {
                HitablePdf plight(lightShape, rec.p);
                MixturePdf p(&plight, srec.pdf);
                Ray scattered = Ray(rec.p, p.generate(), r.time());
                double pdfValue = p.value(scattered.direction());
                delete srec.pdf;
                return emitted + srec.attenuation * rec.material->scatteringPdf(r, rec, scattered) *
                                     color(scattered, world, lightShape, depth + 1) / pdfValue;
            }
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

Hitable* twoSpheres(double aspect, Camera& camera)
{
    const Vector3 lookFrom(13, 2, 3);
    const Vector3 lookAt(0, 0, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 20, aspect, aperture, dist_to_focus);

    Texture* checker = new CheckerTexture(new ConstantTexture(Vector3(0.2, 0.3, 0.1)), new ConstantTexture(Vector3(0.9, 0.9, 0.9)));
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0,-10, 0), 10, new Lambertian(checker)));
    list.push_back(new Sphere(Vector3(0, 10, 0), 10, new Lambertian(checker)));
    return new HitableList(list);
}

Hitable* randomScene(double aspect, Camera& camera)
{
    const Vector3 lookFrom(13, 2, 3);
    const Vector3 lookAt(0, 0, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 20, aspect, aperture, dist_to_focus, 0.0, 1.0);

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

Hitable* perlinSpheres(double aspect, Camera& camera)
{
    const Vector3 lookFrom(13, 2, 3);
    const Vector3 lookAt(0, 0, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 20, aspect, aperture, dist_to_focus);

    int nx, ny, nz;
    unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nz, 0);

    Texture* noise = new NoiseTexture(4); // ConstantTexture(Vector3(0.5, 0.5, 0.5)); //
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0,-1000, 0), 1000, new Lambertian(noise)));
    list.push_back(new Sphere(Vector3(0, 2, 0), 2, new Lambertian(new ImageTexture(tex_data, nx, ny))));
    return new HitableList(list);
}

Hitable* simpleLight(double aspect, Camera& camera)
{
    const Vector3 lookFrom(13, 2, 3);
    const Vector3 lookAt(0, 0, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 20, aspect, aperture, dist_to_focus);

    Texture* noise = new NoiseTexture(4); // ConstantTexture(Vector3(0.5, 0.5, 0.5)); //
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0,-1000, 0), 1000, new Lambertian(noise)));
    list.push_back(new Sphere(Vector3(0, 2, 0), 2, new Lambertian(noise)));

    list.push_back(new Sphere(Vector3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(Vector3(4, 4, 4)))));
    list.push_back(new XYRectangle(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(Vector3(4, 4, 4)))));

    return new HitableList(list);
}

Hitable* cornellBox(double aspect, Camera& camera)
{
    const Vector3 lookFrom(278, 278, -800);
    const Vector3 lookAt(278, 278, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 40, aspect, aperture, dist_to_focus);

    std::vector<Hitable*> list;

    Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vector3(15, 15, 15)));
    Material* aluminum = new Metal(Vector3(0.8, 0.85, 0.88), 0.0);
    Material* glass = new Dielectric(1.5);

    list.push_back(new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green)));
    list.push_back(new YZRectangle(0, 555, 0, 555, 0, red));
    list.push_back(new FlipNormals(new XZRectangle(213, 343, 227, 332, 554, light)));
    list.push_back(new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white)));
    list.push_back(new XZRectangle(0, 555, 0, 555, 0, white));
    list.push_back(new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white)));

    //list.push_back(new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65)));
    list.push_back(new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), aluminum), 15), Vector3(265, 0, 295)));
    list.push_back(new Sphere(Vector3(190, 90, 190), 90, glass));

    //list.push_back(new Translate(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), Vector3(130, 0, 65)));
    //list.push_back(new Translate(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), Vector3(265, 0, 295)));

    //Hitable* b1 = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65));
    //Hitable* b2 = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), 15), Vector3(265, 0, 295));

    //list.push_back(new ConstantMedium(b1, 0.01, new ConstantTexture(Vector3(1, 1, 1))));
    //list.push_back(new ConstantMedium(b2, 0.01, new ConstantTexture(Vector3(0, 0, 0))));

    return new HitableList(list);
}

Hitable* final(double aspect, Camera& camera)
{
    const Vector3 lookFrom(478, 278, -600); //(278, 278, -800); //(13, 2, 3);
    const Vector3 lookAt(278, 278, 0); //(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    camera = Camera(lookFrom, lookAt, Vector3(0, 1, 0), 40, aspect, aperture, dist_to_focus);

    int nb = 20;
    std::vector<Hitable *> list;
    Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
    Material* ground = new Lambertian(new ConstantTexture(Vector3(0.48, 0.83, 0.53)));
    std::vector<Hitable*> boxList, boxList2;
    for (int i = 0; i < nb; i++)
    {
        for (int j = 0; j < nb; j++)
        {
            double w = 100;
            double x0 = -1000 + i*w;
            double z0 = -1000 + j*w;
            double y0 = 0;
            double x1 = x0 + w;
            double y1 = 100*(drand48()+0.01);
            double z1 = z0 + w;
            boxList.push_back(new Box(Vector3(x0, y0, z0), Vector3(x1, y1, z1), ground));
        }
    }

    list.push_back(new BVH(boxList, 0, 1));
    Material* light = new DiffuseLight(new ConstantTexture(Vector3(6, 6, 6)));
    list.push_back(new XZRectangle(123, 423, 147, 412, 554, light));
    Vector3 center(400, 400, 200);
    list.push_back(new MovingSphere(center, center+Vector3(30, 0, 0), 0, 1, 50, new Lambertian(new ConstantTexture(Vector3(0.7, 0.3, 0.1)))));
    list.push_back(new Sphere(Vector3(260, 150, 45), 50, new Dielectric(1.5)));
    list.push_back(new Sphere(Vector3(0, 150, 145), 50, new Metal(Vector3(0.8, 0.8, 0.9), 10)));
    Hitable* boundary = new Sphere(Vector3(360, 150, 145), 70, new Dielectric(1.5));
    list.push_back(boundary);
    list.push_back(new ConstantMedium(boundary, 0.02, new ConstantTexture(Vector3(0.2, 0.4, 0.9))));
    boundary = new Sphere(Vector3(0, 0, 0), 5000, new Dielectric(1.5));
    list.push_back(new ConstantMedium(boundary, 0.0001, new ConstantTexture(Vector3(1.0, 1.0, 1.0))));
    int nx, ny, nz;
    unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nz, 0);
    Material* emat = new Lambertian(new ImageTexture(tex_data, nx, ny));
    list.push_back(new Sphere(Vector3(400, 200, 400), 100, emat));
    Texture* pertext = new NoiseTexture(0.1);
    list.push_back(new Sphere(Vector3(220, 280, 300), 80, new Lambertian(pertext)));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxList2.push_back(new Sphere(Vector3(165*drand48(), 165*drand48(), 165*drand48()), 10, white));
    }
    list.push_back(new Translate(new RotateY(new BVH(boxList2, 0.0, 1.0), 15), Vector3(-100, 270, 395)));
    return new HitableList(list);
}

inline Vector3 deNan(const Vector3& c) {
    Vector3 temp = c;
    if (!(temp[0] == temp[0])) temp[0] = 0;
    if (!(temp[1] == temp[1])) temp[1] = 0;
    if (!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}

int main(int argc, char** argv)
{
    cxxopts::Options options("pathtracer", "Implementation of Peter Shirley's Raytracing in One Weekend book series.");
    options.add_options()
        ("q,quick", "Quick render.")
        ("w,width", "Output width.", cxxopts::value<int>())
        ("h,height", "Output height.", cxxopts::value<int>())
        ("n,numsamples", "Number of sample rays per pixel.", cxxopts::value<int>())
        ("f,file", "Output filename.", cxxopts::value<std::string>());

    options.parse(argc, argv);

    bool quick = options.count("quick") > 0;

    int nx = 800;
    int ny = 800;
    int ns = 100 * 100;

    std::string outFile("foo.ppm");

    if (options.count("width"))
        nx = options["width"].as<int>();
    if (options.count("height"))
        ny = options["height"].as<int>();
    if (options.count("numsamples"))
        ns = options["numsamples"].as<int>();
    if (options.count("file"))
        outFile = options["file"].as<std::string>();

    if (quick)
    {
        nx /= 8;
        ny /= 8;
        ns /= 16;
    }

    Camera cam;
    const double aspect = double(nx)/double(ny);
    Hitable* world = cornellBox(aspect, cam);// cornellBox(); // simpleLight(); //randomScene(); //
    Hitable* lightShape = new XZRectangle(213, 343, 227, 332, 554, nullptr);
    Hitable* glassSphere = new Sphere(Vector3(190, 90, 190), 90, nullptr);
    std::vector<Hitable*> a;
    a.push_back(lightShape);
    a.push_back(glassSphere);
    HitableList* stuff = new HitableList(a);

    int numHitables = world->numChildren();
    std::cout << "Total Hitables: " << numHitables << std::endl;

    Vector3* outImage = new Vector3[nx * ny];

    Progress progress(nx*ny, "PathTracers");

    int index = 0;
    for (int j = ny-1; j>=0; j--)
    {
        for (int i = 0; i<nx; i++)
        {
            Vector3 col(0, 0, 0);
            for (int s = 0; s<ns; s++)
            {
                auto u = (i+drand48())/double(nx);
                auto v = (j+drand48())/double(ny);
                Ray r = cam.getRay(u, v);
                col += deNan(color(r, world, stuff, 0));
            }
            col /= double(ns);
            outImage[index++] = Vector3(sqrt(std::max(0.0, col[0])), sqrt(std::max(0.0, col[1])), sqrt(std::max(0.0, col[2])));
            progress.update(1);
        }
    }

    progress.completed();

    std::ofstream of(outFile.c_str());
    if (of.is_open())
    {
        of << "P3\n" << nx << " " << ny << "\n255\n";

        for (int i = 0; i < nx * ny; i++)
        {
            Vector3 col = outImage[i];

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            of << ir << " " << ig << " " << ib << "\n";
        }
    }
    of.close();

    delete[] outImage;

    return 0;
}
