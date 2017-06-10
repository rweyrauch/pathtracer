//
// Created by rweyra on 6/7/17.
//

#ifndef PATHTRACER_CUDACOMMON_H
#define PATHTRACER_CUDACOMMON_H

#include <cuda.h>

#ifdef __CUDACC__
#define CALLABLE_FUNC __host__ __device__
#else
#define CALLABLE_FUNC
#endif

inline int IDIVUP(int numer, int denom)
{
    return ((numer) % (denom) != 0) ? ((numer) / (denom) + 1) : ((numer) / (denom));
}

#endif // PATHTRACER_CUDACOMMON_H