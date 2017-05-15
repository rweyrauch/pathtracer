//
// Created by rick on 5/14/17.
//

#include "Texture.h"

Vector3 ImageTexture::value(double u, double v, const Vector3 &p) const
{
    int i = u * nx;
    int j = (1 - v) * ny - 0.001;
    i = std::max(0, i);
    j = std::max(0, j);
    i = std::min(i, nx-1);
    j = std::min(j, ny-1);
    double r = int(data[3*i + 3*nx*j + 0]) / 255.0;
    double g = int(data[3*i + 3*nx*j + 1]) / 255.0;
    double b = int(data[3*i + 3*nx*j + 2]) / 255.0;
    return Vector3(r, g, b);
}