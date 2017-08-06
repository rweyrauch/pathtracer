/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Perlin.h"

Vector3* Perlin::ranvec = Perlin::generate();
double* Perlin::ranfloat = Perlin::generate_double();
int* Perlin::perm_x = Perlin::generate_perm();
int* Perlin::perm_y = Perlin::generate_perm();
int* Perlin::perm_z = Perlin::generate_perm();

double Perlin::perlin_interp(Vector3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3-2*u);
    double vv = v * v * (3-2*v);
    double ww = w * w * (3-2*w);
    double accum = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                Vector3 weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu)) * (j*vv + (1-j)*(1-vv)) * (k*ww + (1-k)*(1-ww))*dot(c[i][j][k], weight_v);
            }
        }
    }
    return accum;
}

void permute(int* p, int n)
{
    for (int i = n-1; i > 0; i--)
    {
        auto target = int(drand48()*(i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}
int* Perlin::generate_perm()
{
    auto * p = new int[256];
    for (int i = 0; i < 256; i++)
        p[i] = i;
    permute(p, 256);
    return p;
}

Vector3* Perlin::generate()
{
    auto * p = new Vector3[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = unit_vector(Vector3(-1+2*drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
    }
    return p;
}

double* Perlin::generate_double()
{
    auto *p = new double[256];
    for (int i = 0; i < 256; i++)
        p[i] = drand48();
    return p;
}

double Perlin::noise(const Vector3& p) const
{
    double u = p.x() - floor(p.x());
    double v = p.y() - floor(p.y());
    double w = p.z() - floor(p.z());
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    Vector3 c[2][2][2];
    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                c[di][dj][dk] = ranvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

double Perlin::turbulence(const Vector3& p, int depth) const
{
    double accum = 0;
    Vector3 temp_p = p;
    double weight = 1;
    for (int i = 0; i < depth; i++)
    {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }
    return fabs(accum);
}
