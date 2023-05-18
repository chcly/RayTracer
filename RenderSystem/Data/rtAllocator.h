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
/*! \addtogroup DataApi
 * @{
 */
#ifndef _rtAllocator_h_
#define _rtAllocator_h_

#include "RenderSystem/Data/rtBackendTypes.h"
#include "Utils/skDisableWarnings.h"
#include <exception>
#include <stdexcept>

#ifdef USING_CUDA
#include <cuda_runtime_api.h>

/// <summary>
///
/// </summary>
struct rtCudaObjectAllocator
{
    template <typename T>
    static T* allocate()
    {
        void* mem = nullptr;

        const int st = cudaMallocManaged(&mem, sizeof(T));
        if (st != cudaSuccess || mem == nullptr)
            throw std::runtime_error("Cuda allocation failed.");
        return (T*)mem;
    }

    template <typename T>
    static T* allocateArray(SKuint32 capacity)
    {
        void* mem = nullptr;

        const int st = cudaMallocManaged(&mem, sizeof(T) * capacity);
        if (st != cudaSuccess || mem == nullptr)
            throw std::runtime_error("Cuda allocation failed.");
        return (T*)mem;
    }

    template <typename T>
    static void free(T* ptr)
    {
        if (ptr)
            cudaFree(ptr);
    }

    template <typename T>
    static void freeArray(T* ptr)
    {
        if (ptr)
            cudaFree(ptr);
    }
};
#endif

/// <summary>
///
/// </summary>
struct rtCpuObjectAllocator
{
    template <typename T>
    static T* allocate()
    {
        void* mem = malloc(sizeof(T));
        if (mem == nullptr)
            throw std::runtime_error("allocation failed.");
        return (T*)mem;
    }

    template <typename T>
    static T* allocateArray(SKuint32 capacity)
    {
        void* mem = malloc(sizeof(T) * capacity);
        if (mem == nullptr)
            throw std::runtime_error("allocation failed.");
        return (T*)mem;
    }

    template <typename T>
    static void free(T* ptr)
    {
        if (ptr)
            ::free(ptr);
    }

    template <typename T>
    static void freeArray(T* ptr)
    {
        if (ptr)
            ::free(ptr);
    }
};

/// <summary>
///
/// </summary>
class rtAllocator
{
private:
    static SKint32 m_backend;

public:

    /// <summary>
    /// Sets the backend allocation type.
    /// </summary>
    /// <param name="allocator">One of the rtBackendTypes values.</param>
    /// <remarks>
    /// This needs to be set before allocating any GPU types.
    /// It is set to RT_CPU by default.
    /// If allocator parameter is set to RT_CUDA then
    /// the function rtCudaInitialize will be invoked.
    /// An runtime_error will be thrown the parameter is anything other
    /// than a rtBackendTypes value or if Cuda fails to initialize.
    /// </remarks>
    static void setBackend(SKint32 allocator);

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <returns></returns>
    template <typename T>
    static T* allocate()
    {
        T* mem;
        if (m_backend == RT_CPU)
            mem = rtCpuObjectAllocator::allocate<T>();
#ifdef USING_CUDA
        else if (m_backend == RT_CUDA)
            mem = rtCudaObjectAllocator::allocate<T>();
#endif
        else
            throw std::runtime_error("allocate: unknown allocator");
        if (mem)
            new (mem) T();
        return mem;
    }

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="capacity"></param>
    /// <returns></returns>
    template <typename T>
    static T* allocateArray(SKuint32 capacity)
    {
        T* mem;
        if (m_backend == RT_CPU)
            mem = rtCpuObjectAllocator::allocateArray<T>(capacity);
#ifdef USING_CUDA
        else if (m_backend == RT_CUDA)
            mem = rtCudaObjectAllocator::allocateArray<T>(capacity);
#endif
        else
            throw std::runtime_error("allocateArray: unknown allocator");

        return mem;
    }

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="oldPtr"></param>
    /// <param name="capacity"></param>
    /// <param name="oldSize"></param>
    /// <returns></returns>
    template <typename T>
    static T* reallocateArray(T* oldPtr, SKuint32 capacity, SKuint32 oldSize)
    {
        T* mem = nullptr;
        if (m_backend == RT_CPU)
            mem = rtCpuObjectAllocator::allocateArray<T>(capacity);
#ifdef USING_CUDA
        else if (m_backend == RT_CUDA)
            mem = rtCudaObjectAllocator::allocateArray<T>(capacity);
#endif
        else
            throw std::runtime_error("reallocateArray: unknown allocator");

        if (mem && oldPtr && oldSize > 0)
        {
            for (SKuint32 i = 0; i < oldSize; ++i)
                mem[i] = oldPtr[i];
        }
        return mem;
    }

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="ptr"></param>
    template <typename T>
    static void free(T* ptr)
    {
        if (ptr)
            ptr->~T();

        if (m_backend == RT_CPU)
            rtCpuObjectAllocator::free<T>(ptr);
#ifdef USING_CUDA
        else if (m_backend == RT_CUDA)
            rtCudaObjectAllocator::free<T>(ptr);
#endif
    }

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="ptr"></param>
    template <typename T>
    static void freeArray(T* ptr)
    {
        if (m_backend == RT_CPU)
            rtCpuObjectAllocator::freeArray<T>(ptr);
#ifdef USING_CUDA
        else if (m_backend == RT_CUDA)
            rtCudaObjectAllocator::freeArray<T>(ptr);
#endif
    }
};
/*! @} */

#endif  //_rtAllocator_h_
