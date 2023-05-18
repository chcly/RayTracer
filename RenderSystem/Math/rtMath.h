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
#ifndef _rtMath_h_
#define _rtMath_h_

#include "RenderSystem/rtConfig.h"
#include "Utils/Config/skConfig.h"

#if SK_PLATFORM == SK_PLATFORM_WIN32
#define rtAlign(x) __declspec(align(x))
#else
#define rtAlign(x) __attribute__((aligned(x)))
#endif

using rtScalar = float;

////////////////////////////////////////////////////////////////////////

#ifdef RT_USE_SIMD
#include <immintrin.h>
#include <xmmintrin.h>

constexpr const rtScalar& RTf(const __m128& x, SKint32 i)
{
    return x.m128_f32[3 - i];
}

constexpr const rtScalar& RTfx(const __m128& x)
{
    return x.m128_f32[3];
}

constexpr const rtScalar& RTfy(const __m128& x)
{
    return x.m128_f32[2];
}

constexpr const rtScalar& RTfz(const __m128& x)
{
    return x.m128_f32[1];
}

constexpr const rtScalar& RTfw(const __m128& x)
{
    return x.m128_f32[0];
}

#define UC(x, i) ((unsigned char)(x).m128_i8[i])

using rtScalar4 = __m128;
using rtScalar8 = __m256;

#endif

#define RT_DEBUG

////////////////////////////////////////////////////////////////////////

#ifdef RT_DEBUG
#include <cstdio>
#endif

#endif  //_rtMath_h_
