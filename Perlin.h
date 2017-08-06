/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_PERLIN_H
#define PATHTRACER_PERLIN_H


#include "Vector3.h"

inline double trilinear_interp(double c[2][2][2], double u, double v, double w)
{
    double accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i*u + (1-i)*(1-u))*(j*v + (1-j)*(1-v))*(k*w + (1-k)*(1-w))*c[i][j][k];
    return accum;
}

class Perlin
{
public:
    double noise(const Vector3& p) const;
    double turbulence(const Vector3& p, int depth=7) const;

    static int* generate_perm();
    static Vector3* generate();
    static double* generate_double();
    static double perlin_interp(Vector3 c[2][2][2], double u, double v, double w);

    static Vector3* ranvec;
    static double* ranfloat;
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
};


#endif //PATHTRACER_PERLIN_H
