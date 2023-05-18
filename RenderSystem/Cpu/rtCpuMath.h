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
/*! \addtogroup CpuKernel
 * @{
 */

#ifndef _rtCpuMath_h_
#define _rtCpuMath_h_

#include "Math/skVector3.h"
#include "RenderSystem/Math/rtVectorTypes.h"

#define RT_CPU_API extern

struct rtObjectType;


/// <summary>
/// 
///
/// \f$
/// R = \begin{bmatrix}
/// t * direction_{x} + origin_{x} \\
/// t * direction_{y} + origin_{y} \\
/// t * direction_{z} + origin_{z}
/// \end{bmatrix}
/// \f$
/// </summary>
struct rtCpuRay
{
    rtVector3 origin;
    rtVector3 direction;
};

/// <summary>
/// </summary>
struct rtCpuHitResult
{
    rtVector3     point;
    rtVector3     normal;
    rtScalar      distance;
    rtObjectType* object;
};

/// <summary>
///
/// \f$
/// R =
/// \begin{bmatrix}
/// 2 &InvisibleTimes; (a_{x} &InvisibleTimes; b_{x} + a_{y} &InvisibleTimes; b_{y} + a_{z} &InvisibleTimes; b_{z}) &InvisibleTimes; b_{x} &minus; a_{x} \\
/// 2 &InvisibleTimes; (a_{x} &InvisibleTimes; b_{x} + a_{y} &InvisibleTimes; b_{y} + a_{z} &InvisibleTimes; b_{z}) &InvisibleTimes; b_{y} &minus; a_{y} \\
/// 2 &InvisibleTimes; (a_{x} &InvisibleTimes; b_{x} + a_{y} &InvisibleTimes; b_{y} + a_{z} &InvisibleTimes; b_{z}) &InvisibleTimes; b_{z} &minus; a_{z}
/// \end{bmatrix}
/// \f$
/// </summary>
/// <param name="a">
/// \f$
/// a =
/// \begin{pmatrix}
/// l_{x} - p_{x} \\
/// l_{y} - p_{y} \\
/// l_{z} - p_{z}
/// \end{pmatrix}
/// \f$
/// where l is the light's position and p is the hit point of the ray. 
/// </param>
/// <param name="b">Is the hit normal.</param>
/// <returns>R</returns>
RT_CPU_API rtVector3 rtCpuReflect(const rtVector3& a, const rtVector3& b);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="len"></param>
/// <returns></returns>
RT_CPU_API void rtCpuScalarZero(rtScalar* a, int len);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="len"></param>
/// <returns></returns>
RT_CPU_API void rtCpuScalarCopy(rtScalar* a, const rtScalar* b, int len);

/// <summary>
///
/// </summary>
/// <param name="bMin"></param>
/// <param name="bMax"></param>
/// <param name="v"></param>
/// <returns></returns>
RT_CPU_API void rtCpuCompareBounding(rtScalar*        bMin,
                                     rtScalar*        bMax,
                                     const skVector3& v);

/// <summary>
///
/// </summary>
/// <param name="bMin"></param>
/// <param name="bMax"></param>
/// <param name="v"></param>
/// <returns></returns>
RT_CPU_API void rtCpuCompareBounding(rtScalar*        bMin,
                                     rtScalar*        bMax,
                                     const rtVector3& v);

/// <summary>
/// \f$
/// \begin{pmatrix}
/// a_{x} + b_{x} \\
/// a_{y} + b_{y} \\
/// a_{z} + b_{z}
/// \end{pmatrix}
/// \f$
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Add(const rtVector3& a, const rtVector3& b);

/// <summary>
/// \f$
/// \begin{pmatrix}
/// a_{x} + b \\
/// a_{y} + b \\
/// a_{z} + b
/// \end{pmatrix}
/// \f$
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Add(const rtVector3& a, const rtScalar& b);

/// <summary>
/// \f$
/// \begin{pmatrix}
/// a_{x} + b_{x} + c_{x} \\
/// a_{y} + b_{y} + c_{y} \\
/// a_{z} + b_{z} + c_{z} 
/// \end{pmatrix}
/// \f$
//// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuAdd3Vec3(const rtVector3& a,
                                   const rtVector3& b,
                                   const rtVector3& c);

/// <summary>
/// \f$
/// \begin{pmatrix}
/// a_{x} + b_{x} + c \\
/// a_{y} + b_{y} + c \\
/// a_{z} + b_{z} + c
/// \end{pmatrix}
/// \f$
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuAdd3Vec3(const rtVector3& a,
                                   const rtVector3& b,
                                   const rtScalar&  c);

/// <summary>
/// \f$
/// \begin{pmatrix}
/// a_{y} &InvisibleTimes; b_{z} + a_{z} &InvisibleTimes; b_{y} \\
/// a_{z} &InvisibleTimes; b_{x} + a_{x} &InvisibleTimes; b_{z} \\
/// a_{x} &InvisibleTimes; b_{y} + a_{y} &InvisibleTimes; b_{x}
/// \end{pmatrix}
/// \f$
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuCrossVec3(const rtVector3& a, const rtVector3& b);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="s"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuMulVec3(const rtVector3& a, const rtScalar& s);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="v"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuMulVec3v(const rtVector3& a, const rtVector3& v);

/// <summary>
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtScalar rtCpuVec3Dot(const rtVector3& a, const rtVector3& b);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
RT_CPU_API rtScalar rtCpuVec3Len(const rtVector3& a);
/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Norm(const rtVector3& a);

/// <summary>
///
/// </summary>
/// <param name="dest"></param>
/// <param name="center"></param>
/// <param name="radius"></param>
/// <param name="ray"></param>
/// <param name="limit"></param>
/// <returns></returns>
RT_CPU_API bool rtCpuSphereTest(rtCpuHitResult*  dest,
                                const rtVector3& center,
                                const rtScalar&  radius,
                                const rtCpuRay&  ray,
                                const rtVector2& limit);

/// <summary>
///
/// </summary>
/// <param name="center"></param>
/// <param name="radius"></param>
/// <param name="ray"></param>
/// <param name="limit"></param>
/// <returns></returns>
RT_CPU_API bool rtCpuSphereTest(const rtVector3& center,
                                const rtScalar&  radius,
                                const rtCpuRay&  ray,
                                const rtVector2& limit);

/// <summary>
///
/// </summary>
/// <param name="dest"></param>
/// <param name="bbMin"></param>
/// <param name="bbMax"></param>
/// <param name="ray"></param>
/// <param name="limit"></param>
/// <returns></returns>
RT_CPU_API bool rtCpuBoxTest(rtCpuHitResult*  dest,
                             const rtScalar   bbMin[3],
                             const rtScalar   bbMax[3],
                             const rtCpuRay&  ray,
                             const rtVector2& limit);

RT_CPU_API bool rtCpuBoxTest(const rtScalar   bbMin[3],
                             const rtScalar   bbMax[3],
                             const rtCpuRay&  ray,
                             const rtVector2& limit);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
RT_CPU_API void rtCpuVec3Norm(rtVector3* a);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtScalar rtCpuVec3DistSqu(const rtVector3& a, const rtVector3& b);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Norm(const rtVector3& a, const rtVector3& b);

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Sub(const rtVector3& a, const rtVector3& b);
/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
RT_CPU_API rtVector3 rtCpuVec3Sub(const rtVector3& a, const rtScalar& b);

/// <summary>
///
/// </summary>
/// <param name="r"></param>
/// <param name="v"></param>
/// <param name="q"></param>
/// <returns></returns>
RT_CPU_API void rtCpuMulQuaternion(rtVector3&       r,
                                   const rtVector3& v,
                                   const rtVector4& q);

/// <summary>
///
/// </summary>
/// <param name="dir"></param>
/// <param name="ori"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="offset"></param>
RT_CPU_API void rtCpuComputeRayDirection(rtVector3&       dir,
                                         const rtVector4& ori,
                                         const rtScalar&  x,
                                         const rtScalar&  y,
                                         const rtVector4& offset);
/// <summary>
/// 
/// </summary>
/// <param name="dir"></param>
/// <param name="ori"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="ox"></param>
/// <param name="oy"></param>
/// <param name="offset"></param>
/// <returns></returns>
RT_CPU_API void rtCpuComputeRayDirection(rtVector3&       dir,
                                         const rtVector4& ori,
                                         const rtScalar&  x,
                                         const rtScalar&  y,
                                         const rtScalar&  ox,
                                         const rtScalar&  oy,
                                         const rtVector4& offset);
    /// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
SK_INLINE rtScalar rtCpuMax(const rtScalar& a, const rtScalar& b)
{
    return a > b ? a : b;
}

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
SK_INLINE rtScalar rtCpuMax3(const rtVector3& a)
{
    return rtCpuMax(a.x, rtCpuMax(a.y, a.z));
}

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
SK_INLINE rtScalar rtCpuMax3(const rtScalar& a,
                             const rtScalar& b,
                             const rtScalar& c)
{
    return rtCpuMax(a, rtCpuMax(b, c));
}

/// <summary>
///
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
SK_INLINE rtScalar rtCpuSign(const rtScalar& a)
{
    return a < 0.f ? -1.f : 1.f;
}
/*!
 * @}
 */

#endif  //_rtCpuMath_h_
