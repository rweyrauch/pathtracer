/*
 * Pathtracer based on Peter Shirley's 'Ray Tracing in One Weekend' e-book
 * series.
 *
 * Copyright (C) 2017 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PATHTRACER_NOISE_H
#define PATHTRACER_NOISE_H

class Vector3;

double Noise(const Vector3& p);
double Turbulence(const Vector3& p, int depth=7);

#endif //PATHTRACER_NOISE_H
