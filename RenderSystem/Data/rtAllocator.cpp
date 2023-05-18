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
#include "rtAllocator.h"

#ifdef USING_CUDA
#include "RenderSystem/Cuda/rtCudaUtils.h"
#endif
SKint32 rtAllocator::m_backend = RT_CPU;

void rtAllocator::setBackend(SKint32 allocator)
{
    switch (allocator)
    {
    case RT_CPU:
        m_backend = RT_CPU;
        break;
#ifdef USING_CUDA
    case RT_CUDA:
        m_backend = RT_CUDA;
        if (rtCudaInitialize() != cudaSuccess)
            throw std::runtime_error("Cuda initialization failed.");
        break;
#endif
    default:
        throw std::runtime_error("Unknown allocator");
    }
}
