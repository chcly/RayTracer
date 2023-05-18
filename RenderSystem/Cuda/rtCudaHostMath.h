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
#ifndef _rtCudaHostMath_h_
#define _rtCudaHostMath_h_
#ifdef USING_CUDA

#include <cuda_runtime_api.h>
#include <cstdint>
#include "Math/skColor.h"
#include "RenderSystem/Math/rtVectorTypes.h"
#include "Math/skQuaternion.h"
#include "Math/skVector2.h"
#include "Math/skVector3.h"
#include "Math/skVector4.h"
#include "Utils/Config/skConfig.h"

struct rtObjectType;

struct rtCudaRay
{
    rtVector3 origin;
    rtVector3 direction;
};

struct rtCudaHitResult
{
    rtVector3     point;
    rtVector3     normal;
    rtScalar      distance;
    rtObjectType* object;
};

/// <summary>
/// Constructor for a float2
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skVector2 class.</param>
SK_INLINE void rtCudaFloat2(float2& dest, const skVector2& src)
{
    dest.x = src.x;
    dest.y = src.y;
}

/// <summary>
/// Constructor for a float3
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skVector3 class.</param>
SK_INLINE void rtCudaFloat3(float3& dest, const skVector3& src)
{
    dest.x = src.x;
    dest.y = src.y;
    dest.z = src.z;
}

/// <summary>
/// Constructor for a float3
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skVector3 class.</param>
SK_INLINE void rtCudaFloat3(skScalar* dest, const skScalar src[3])
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

/// <summary>
/// Constructor for a float3
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skVector3 class.</param>
SK_INLINE void rtCudaFloat3(float3& dest, const skScalar src[3])
{
    dest.x = src[0];
    dest.y = src[1];
    dest.z = src[2];
}

/// <summary>
/// Constructor for a float3
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skColor class.</param>
SK_INLINE void rtCudaFloat3(float3& dest, const skColor& src)
{
    dest.x = src.r;
    dest.y = src.g;
    dest.z = src.b;
}

/// <summary>
/// Constructor for a float4
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skVector4 class.</param>
SK_INLINE void rtCudaFloat4(float4& dest, const skVector4& src)
{
    dest.x = src.x;
    dest.y = src.y;
    dest.z = src.z;
    dest.w = src.w;
}

/// <summary>
/// Constructor for a float4
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skColor class.</param>
SK_INLINE void rtCudaFloat4(float4& dest, const skColor& src)
{
    dest.x = src.r;
    dest.y = src.g;
    dest.z = src.b;
    dest.w = src.a;
}

/// <summary>
/// Constructor for a float4
/// </summary>
/// <param name="dest">The destination memory.</param>
/// <param name="src">The source skColor class.</param>
SK_INLINE void rtCudaFloat4(float4& dest, const skQuaternion& src)
{
    dest.x = src.x;
    dest.y = src.y;
    dest.z = src.z;
    dest.w = src.w;
}
#endif

#endif  //_rtCudaHostMath_h_
