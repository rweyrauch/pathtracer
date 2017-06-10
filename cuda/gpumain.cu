#include "CudaCommon.h"
#include <cuda.h>
#include <iostream>
#include <cfloat>
#include <vector>
#include "Sphere.h"
#include "HitableList.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"

const int MAX_DEPTH = 50;

CALLABLE_FUNC Vector3 color(const Ray& r, Hitable* world, unsigned int *seed0, unsigned int *seed1)
{
    Vector3 accumCol(1, 1, 1);
    Vector3 attenuation(0, 0, 0);

    Ray currentRay(r);

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        HitRecord rec;
        if (world->hit(currentRay, 0.001f, FLT_MAX, rec))
        {
            Ray scattered;
            if (rec.material->scatter(currentRay, rec, attenuation, scattered, seed0, seed1))
            {
                accumCol *= attenuation;
                currentRay = scattered;
            }
            else
            {
                accumCol = Vector3(0.0f, 0.0f, 0.0f);
                break;
            }
        }
        else
        {
            Vector3 unit_dir = unit_vector(r.direction());
            float t = 0.5f * (unit_dir.y() + 1.0f);

            attenuation = (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
            accumCol *= attenuation;
            break;
        }
    }
    return accumCol;
}

const int nx = 128*4;
const int ny = 128*4;

__device__ Camera g_cam;

__global__ void render_kernel(float3* pOutImage, Hitable** world, int nx, int ny, int ns)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= nx || y >= ny) return;

    unsigned int i = (ny - y - 1) * nx + x; // index of current pixel (calculated using thread index)

    unsigned int seed0 = x;  // seeds for random number generator
    unsigned int seed1 = y;

    Vector3 accumCol(0, 0, 0);
    for (int s = 0; s < ns; s++)
    {
        float u = (x + rand(&seed0, &seed1)) / float(nx);
        float v = (y + rand(&seed0, &seed1)) / float(ny);
        Ray r = g_cam.getRay(u, v, &seed0, &seed1);
        accumCol += color(r, *world, &seed0, &seed1);
    }
    accumCol /= float(ns);

    pOutImage[i] = make_float3(sqrtf(accumCol[0]), sqrtf(accumCol[1]), sqrtf(accumCol[2]));
}

__global__ void allocate_world_kernel(Hitable** world, float aspect)
{
    int i = 0;
    Hitable** list = new Hitable*[4];
    list[i++] = new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(new ConstantTexture(Vector3(0.1, 0.2, 0.5))));
    list[i++] = new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(new ConstantTexture(Vector3(0.8, 0.8, 0.0))));
    list[i++] = new Sphere(Vector3(1, 0, -1), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 0.3));
    list[i++] = new Sphere(Vector3(-1, 0, -1), 0.5, new Dielectric(1.5));

    *world = new HitableList(i, list);

    g_cam = Camera(Vector3(-2, 2, 1), Vector3(0, 0, -1), Vector3(0, 1, 0), 90, aspect, 0.0f, 10.0f);
}

/*
__global__ void cornell_box_kernel(Hitable **world, float aspect)
{
    int i = 0;
    Hitable **list = new Hitable*[8];
    Material *red = new Lambertian( new ConstantTexture(Vector3(0.65f, 0.05f, 0.05f)) );
    Material *white = new Lambertian( new ConstantTexture(Vector3(0.73, 0.73, 0.73)) );
    Material *green = new Lambertian( new ConstantTexture(Vector3(0.12, 0.45, 0.15)) );
    Material *light = new DiffuseLight( new ConstantTexture(Vector3(15, 15, 15)) );
    Material* aluminum = new Metal(Vector3(0.8, 0.85, 0.88), 0.0);

    list[i++] = new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green));
    list[i++] = new YZRectangle(0, 555, 0, 555, 0, red);
    list[i++] = new FlipNormals(new XZRectangle(213, 343, 227, 332, 554, light));
    list[i++] = new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white));
    list[i++] = new XZRectangle(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white));
    //Material *glass = new Dielectric(1.5);
    //list[i++] = new Sphere(Vector3(190, 90, 190),90 , glass);
    list[i++] = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65));
    list[i++] = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), aluminum),  90), Vector3(265,0,295));
    //list[i++] = new Translate(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), aluminum), Vector3(265,0,295));
    *world = new HitableList(list,i);
    Vector3 lookfrom(278, 278, -800);
    Vector3 lookat(278,278,0);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    float vfov = 40.0f;
    g_cam = Camera(lookfrom, lookat, Vector3(0,1,0),
                      vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
}
*/
int main()
{
    int ns = 1000;

    /*
    std::vector<Hitable*> list;
    list.push_back(new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vector3(0.1, 0.2, 0.5))));
    list.push_back(new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vector3(0.8, 0.8, 0.0))));
    list.push_back(new Sphere(Vector3(1, 0, -1), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 0.3)));
    list.push_back(new Sphere(Vector3(-1, 0, -1), 0.5, new Dielectric(1.5)));
    //list.push_back(new Sphere(Vector3(-1,0,-1), -0.45, new Dielectric(1.5)));

    //double R = cos(M_PI/4);
    //list.push_back(new Sphere(Vector3(-R,0,-1), R, new Lambertian(Vector3(0,0,1))));
    //list.push_back(new Sphere(Vector3(R,0,-1), R, new Lambertian(Vector3(1,0,0))));

    Hitable* world = new HitableList(list.size(), list.data());

    unsigned int seed0;
    unsigned int seed1;
    //Camera cam(90, double(nx)/ny);
    Camera cam(Vector3(-2, 2, 1), Vector3(0, 0, -1), Vector3(0, 1, 0), 90, float(nx)/float(ny), 0.0f, 10.0f);
    for (int j = ny-1; j>=0; j--)
    {
        seed0 = j;
        for (int i = 0; i<nx; i++)
        {
            seed1 = i;
            Vector3 col(0, 0, 0);
            for (int s = 0; s<ns; s++)
            {
                float u = (i+drand48())/float(nx);
                float v = (j+drand48())/float(ny);
                Ray r = cam.getRay(u, v, &seed0, &seed1);
                col += color(r, world, 0, &seed0, &seed1);
            }
            col /= float(ns);
            col = Vector3(sqrtf(col[0]), sqrtf(col[1]), sqrtf(col[2]));
            int ir = int(255.99f*col[0]);
            int ig = int(255.99f*col[1]);
            int ib = int(255.99f*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    */

    const float aspect = float(nx)/float(ny);

    float3* pOutImage = NULL;
    cudaMalloc(&pOutImage, nx * ny * sizeof(float3));

    Hitable** world = NULL;
    cudaMalloc(&world, sizeof(Hitable**));

    std::cerr << "Allocating world...";
    allocate_world_kernel<<<1, 1>>>(world, aspect);
    cudaError_t err = cudaDeviceSynchronize();
    std::cerr << "done" << std::endl;
    if (err != cudaSuccess)
    {
        std::cerr << "Failed to allocate world in GPU memory.  Error: " << cudaGetErrorName(err) << " Desc: " << cudaGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    dim3 block(8, 8, 1);
    dim3 grid(IDIVUP(nx, block.x), IDIVUP(ny, block.y), 1);
    std::cerr << "Rendering world...";
    render_kernel<<<grid, block>>>(pOutImage, world, nx, ny, ns);
    err = cudaDeviceSynchronize();
    std::cerr << "done" << std::endl;
    if (err != cudaSuccess)
    {
        std::cerr << "Failed to render on GPU.  Error: " << cudaGetErrorName(err) << " Desc: " << cudaGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    float3* pTemp = new float3[nx * ny];
    cudaMemcpy(pTemp, pOutImage, nx*ny*sizeof(float3), cudaMemcpyDeviceToHost);

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int i = 0; i < nx * ny; i++)
    {
        int ir = int(255.99f * pTemp[i].x);
        int ig = int(255.99f * pTemp[i].y);
        int ib = int(255.99f * pTemp[i].z);
        std::cout << ir << " " << ig << " " << ib << "\n";
    }

    std::cerr << "Done." << std::endl;
    cudaFree(pOutImage);
    delete[] pTemp;

    return EXIT_SUCCESS;
}
