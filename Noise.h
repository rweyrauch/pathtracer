//
// Created by rick on 5/14/17.
//

#ifndef PATHTRACER_NOISE_H
#define PATHTRACER_NOISE_H

class Vector3;

double Noise(const Vector3& p);
double Turbulence(const Vector3& p, int depth=7);

#endif //PATHTRACER_NOISE_H
