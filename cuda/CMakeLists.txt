
find_package(CUDA)

if(CUDA_FOUND)
    set(GPU_SOURCE_FILES
            gpumain.cu)

    set(CUDA_NVCC_FLAGS "-use_fast_math")
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_52,code=sm_52)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_61,code=sm_61)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-std=c++11)
    cuda_add_executable(gpupathtracer ${GPU_SOURCE_FILES})
endif()