/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Texture.h"

Vector3 ImageTexture::value(const Vector2& uv, const Vector3 &p) const
{
    int i = uv.u() * nx;
    int j = (1 - uv.v()) * ny - 0.001;
    i = std::max(0, i);
    j = std::max(0, j);
    i = std::min(i, nx-1);
    j = std::min(j, ny-1);
    double r = int(data[3*i + 3*nx*j + 0]) / 255.0;
    double g = int(data[3*i + 3*nx*j + 1]) / 255.0;
    double b = int(data[3*i + 3*nx*j + 2]) / 255.0;
    return {r, g, b};
}