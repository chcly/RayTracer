/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "RenderSystem/Cuda/rtCudaUtils.h"
#include <cuda_runtime.h>
#include <immintrin.h>
#include <cstdio>
#include <cstring>
#include "Utils/Config/skConfig.h"

#if 0
static void rtCudaLogMode(int mode)
{
    switch (mode)
    {
    case cudaComputeModeDefault:
        printf("cudaDevAttrComputeMode = cudaComputeModeDefault\n");
        break;
    case cudaComputeModeExclusive:
        printf("cudaDevAttrComputeMode = cudaComputeModeExclusive\n");
        break;
    case cudaComputeModeProhibited:
        printf("cudaDevAttrComputeMode = cudaComputeModeProhibited\n");
        break;
    case cudaComputeModeExclusiveProcess:
        printf("cudaDevAttrComputeMode = cudaComputeModeExclusiveProcess\n");
        break;
    default:
        printf("cudaDevAttrComputeMode = %d (unknown)\n", mode);
        break;
    }
}
#endif

RT_CU_API int32_t rtCudaInitialize()
{
    cudaError_t status;

    int count;
    if ((status = cudaGetDeviceCount(&count)) != cudaSuccess)
    {
        printf("cudaGetDeviceCount failed(%d)\n", status);
        return status;
    }

    if (count == 0)
    {
        printf("CUDA is not supported\n");
        return status;
    }

    int firstDevice = -1;
    for (int i = 0; i < count; ++i)
    {
        int mode, maj, min;
        if ((status = cudaDeviceGetAttribute(&mode,
                                             cudaDevAttrComputeMode,
                                             i)) != cudaSuccess)
        {
            printf("cudaDeviceGetAttribute(cudaDevAttrComputeMode) failed(%d)\n", status);
            return false;
        }
        if ((status = cudaDeviceGetAttribute(&maj,
                                             cudaDevAttrComputeCapabilityMajor,
                                             i)) != cudaSuccess)
        {
            printf("cudaDeviceGetAttribute(cudaDevAttrComputeCapabilityMajor) failed(%d)\n", status);
            return false;
        }

        if ((status = cudaDeviceGetAttribute(&min,
                                             cudaDevAttrComputeCapabilityMinor,
                                             i)) != cudaSuccess)
        {
            printf("cudaDeviceGetAttribute(cudaDevAttrComputeCapabilityMinor) failed(%d)\n", status);
            return false;
        }

        //rtCudaLogMode(mode);

        if (mode != cudaComputeModeProhibited)
        {
            firstDevice = i;
            break;
        }
    }

    if (firstDevice != -1)
    {
        if ((status = cudaSetDevice(firstDevice)) != cudaSuccess)
        {
            printf("cudaSetDevice(%d) failed(%d)\n", firstDevice, status);
            return status;
        }
    }

    cudaDeviceReset();
    return cudaSuccess;
}

RT_CU_API void rtCudaAllocateFrameBuffer(rtCudaFrameBuffer* buffer, size_t length, int32_t blocks)
{
    SK_ASSERT(buffer && length > 0 && blocks > 0);

    size_t nl = length;
    size_t al = nl % 16;
    if (al > 0)
        al += 16 - al;
    nl += al;
    buffer->length = (size_t)-1;

    if (cudaMallocManaged(&buffer->memory, nl) == cudaSuccess)
    {
        buffer->blocks  = blocks;
        buffer->threads = (uint32_t)(nl / 4 + buffer->blocks) / buffer->blocks;
        buffer->length  = nl;
    }
    else
    {
        printf("Failed to allocate frame buffer.\n");
        memset(buffer, 0, sizeof(rtCudaFrameBuffer));
    }
}

RT_CU_API rtCudaTarget* rtCudaAllocateTarget()
{
    void* mem;
    if (cudaMallocManaged(&mem, sizeof(rtCudaTarget)) == cudaSuccess)
        return (rtCudaTarget*)mem;
    return nullptr;
}

RT_CU_API void rtCudaFreeTarget(rtCudaTarget* target)
{
    if (target)
        cudaFree(target);
}

RT_CU_API void rtCudaFreeFrameBuffer(rtCudaFrameBuffer* buffer)
{
    if (buffer && buffer->memory)
    {
        cudaFree(buffer->memory);

        memset(buffer, 0, sizeof(rtCudaFrameBuffer));
    }
}

RT_CU_API void rtCudaSwapBuffers(uint8_t*            screen,
                                 const rtCudaTarget* buffer)
{
    SK_ASSERT(screen && buffer);
    const SKubyte* src = (SKubyte*)buffer->pixels;
    SKubyte*       dst = (SKubyte*)screen;

#ifdef RT_USE_SIMD

    {
#pragma omp parallel for
        for (int32_t i = 0; i < buffer->length - 8; i += 8)
        {
            const __m256i iSrc = _mm256_loadu_epi32((const void*)&src[i]);
            _mm256_storeu_epi32((void*)&dst[i], iSrc);
        }
    }
#else

    {
#pragma omp parallel for
        for (int32_t i = 0; i < buffer->length; i++)
            dst[i] = src[i];
    }
#endif
}
