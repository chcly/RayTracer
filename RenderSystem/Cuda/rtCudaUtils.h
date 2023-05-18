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
#ifndef _rtCudaUtils_h_
#define _rtCudaUtils_h_
#ifdef USING_CUDA

#include <cuda_runtime_api.h>
#include <cstdint>

#define RT_CU_API extern "C"
#define RT_ALIGN(x) __builtin_align__(x)
#define RT_DEVICE_API __device__
#define RT_KERNEL_API __global__
#define RT_SHARED_API __shared__
#define RT_HOST_API __host__
#define RT_CUD_API extern RT_DEVICE_API

struct rtSceneType;

struct rtCudaFrameBuffer
{
    void*    memory;
    size_t   length;
    uint32_t threads;
    uint32_t blocks;
};

struct rtCudaTarget
{
    uint8_t* pixels;
    int32_t  width;
    int32_t  height;
    int32_t  dimensions;
    int32_t  pitch;
    int32_t  length;
};

RT_CU_API int32_t rtCudaInitialize();

RT_CU_API void rtCudaAllocateFrameBuffer(rtCudaFrameBuffer* buffer, size_t length, int32_t blocks);
RT_CU_API void rtCudaFreeFrameBuffer(rtCudaFrameBuffer* buffer);

RT_CU_API rtCudaTarget* rtCudaAllocateTarget();

RT_CU_API void rtCudaFreeTarget(rtCudaTarget* target);

RT_CU_API void rtCudaKernelMain(rtCudaFrameBuffer* fb, rtCudaTarget* buffer, rtSceneType* scene);
RT_CU_API void rtCudaSwapBuffers(uint8_t* screen, const rtCudaTarget* buffer);

#endif
#endif  //_rtCudaUtils_h_
