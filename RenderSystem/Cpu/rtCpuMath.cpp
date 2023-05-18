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
#include "RenderSystem/Cpu/rtCpuMath.h"
#include "Math/skMath.h"

RT_CPU_API rtVector3 rtCpuVec3Add(const rtVector3& a, const rtVector3& b)
{
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}

RT_CPU_API rtVector3 rtCpuVec3Add(const rtVector3& a, const rtScalar& b)
{
    return {
        a.x + b,
        a.y + b,
        a.z + b,
    };
}

RT_CPU_API rtVector3 rtCpuAdd3Vec3(const rtVector3& a,
                                   const rtVector3& b,
                                   const rtVector3& c)
{
    return {
        a.x + b.x + c.x,
        a.y + b.y + c.y,
        a.z + b.z + c.z,
    };
}

RT_CPU_API rtVector3 rtCpuAdd3Vec3(const rtVector3& a,
                                   const rtVector3& b,
                                   const rtScalar&  c)
{
    return {
        a.x + b.x + c,
        a.y + b.y + c,
        a.z + b.z + c,
    };
}

RT_CPU_API rtVector3 rtCpuCrossVec3(const rtVector3& a, const rtVector3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

RT_CPU_API rtVector3 rtCpuMulVec3(const rtVector3& a, const rtScalar& s)
{
    return {
        a.x * s,
        a.y * s,
        a.z * s,
    };
}

RT_CPU_API rtVector3 rtCpuMulVec3v(const rtVector3& a, const rtVector3& v)
{
    return {
        a.x * v.x,
        a.y * v.y,
        a.z * v.z,
    };
}

RT_CPU_API rtScalar rtCpuVec3Dot(const rtVector3& a, const rtVector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

RT_CPU_API rtScalar rtCpuVec3Len(const rtVector3& a)
{
    const rtScalar dp = a.x * a.x + a.y * a.y + a.z * a.z;
    if (dp > SK_EPSILON)
        return sqrtf(dp);
    return 0;
}

RT_CPU_API rtVector3 rtCpuVec3Norm(const rtVector3& a)
{
    rtVector3 r  = a;
    rtScalar  dp = r.x * r.x + r.y * r.y + r.z * r.z;
    if (dp > SK_EPSILON)
    {
        dp  = skRSqrt(dp);
        r.x = a.x * dp;
        r.y = a.y * dp;
        r.z = a.z * dp;
    }
    return r;
}

RT_CPU_API void rtCpuVec3Norm(rtVector3* a)
{
    rtScalar dp = a->x * a->x + a->y * a->y + a->z * a->z;
    if (dp > SK_EPSILON)
    {
        dp = skRSqrt(dp);
        a->x *= dp;
        a->y *= dp;
        a->z *= dp;
    }
}

RT_CPU_API rtScalar rtCpuVec3DistSqu(const rtVector3& a, const rtVector3& b)
{
    const rtVector3 r = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
    return r.x * r.x + r.y * r.y + r.z * r.z;
}

RT_CPU_API rtVector3 rtCpuVec3Norm(const rtVector3& a, const rtVector3& b)
{
    rtVector3 r{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };

    rtScalar dp = r.x * r.x + r.y * r.y + r.z * r.z;
    if (dp > 0)
    {
        dp = skRSqrt(dp);
        r.x *= dp;
        r.y *= dp;
        r.z *= dp;
    }
    return r;
}

RT_CPU_API rtVector3 rtCpuVec3Sub(const rtVector3& a, const rtVector3& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

RT_CPU_API rtVector3 rtCpuVec3Sub(const rtVector3& a, const rtScalar& b)
{
    return {
        a.x - b,
        a.y - b,
        a.z - b,
    };
}

RT_CPU_API rtVector3 rtCpuReflect(const rtVector3& a, const rtVector3& b)
{
    const rtScalar d = 2.f * (a.x * b.x + a.y * b.y + a.z * b.z);
    return {
        d * b.x - a.x,
        d * b.y - a.y,
        d * b.z - a.z,
    };
}

RT_CPU_API void rtCpuCmpBounding(rtScalar* a, const rtScalar* b, int len)
{
    for (int i = 0; i < len; ++i)
        a[i] = b[i];
}

RT_CPU_API void rtCpuScalarCopy(rtScalar* a, const rtScalar* b, int len)
{
    for (int i = 0; i < len; ++i)
        a[i] = b[i];
}

RT_CPU_API void rtCpuScalarZero(rtScalar* a, int len)
{
    for (int i = 0; i < len; ++i)
        a[i] = 0.f;
}

RT_CPU_API void rtCpuCompareBounding(rtScalar*        bMin,
                                     rtScalar*        bMax,
                                     const skVector3& v)
{
    const skScalar* p = v.ptr();

    for (int i = 0; i < 3; ++i)
    {
        if (p[i] < bMin[i])
            bMin[i] = p[i];
        if (p[i] > bMax[i])
            bMax[i] = p[i];
    }
}

RT_CPU_API void rtCpuCompareBounding(rtScalar*        bMin,
                                     rtScalar*        bMax,
                                     const rtVector3& v)
{
    const skScalar* p = &v.x;

    for (int i = 0; i < 3; ++i)
    {
        if (p[i] < bMin[i])
            bMin[i] = p[i];
        if (p[i] < bMax[i])
            bMax[i] = p[i];
    }
}

RT_CPU_API void rtCpuMulQuaternion(rtVector3&       r,
                                   const rtVector3& v,
                                   const rtVector4& q)
{
    rtVector3 a = {
        q.y * v.z - q.z * v.y,
        q.z * v.x - q.x * v.z,
        q.x * v.y - q.y * v.x,
    };
    const rtVector3 b = {
        2.f * (q.y * a.z - q.z * a.y),
        2.f * (q.z * a.x - q.x * a.z),
        2.f * (q.x * a.y - q.y * a.x),
    };

    rtScalar v1 = 2.f * q.w;
    a.x *= v1;
    a.y *= v1;
    a.z *= v1;

    r.x = v.x + a.x + b.x;
    r.y = v.y + a.y + b.y;
    r.z = v.z + a.z + b.z;

    v1 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (v1 > SK_EPSILON)
    {
        v1 = skRSqrt(v1);
        r.x *= v1;
        r.y *= v1;
        r.z *= v1;
    }
}

RT_CPU_API bool rtCpuSphereTest(rtCpuHitResult*  dest,
                                const rtVector3& center,
                                const rtScalar&  radius,
                                const rtCpuRay&  ray,
                                const rtVector2& limit)
{
    const rtVector3 vec = {
        ray.origin.x - center.x,
        ray.origin.y - center.y,
        ray.origin.z - center.z,
    };
    const rtScalar a =
        ray.direction.x * ray.direction.x +
        ray.direction.y * ray.direction.y +
        ray.direction.z * ray.direction.z;

    const rtScalar b =
        vec.x * ray.direction.x +
        vec.y * ray.direction.y +
        vec.z * ray.direction.z;

    const rtScalar r = radius * radius;

    const rtScalar c =
        vec.x * vec.x - r +
        (vec.y * vec.y - r) +
        (vec.z * vec.z - r);

    rtScalar d = b * b - a * c;

    if (d > 10e-4f)
    {
        d = skSqrt(d);

        rtScalar x = (-b - d) / a;
        if (x >= limit.x && x <= limit.y)
        {
            dest->distance = x;
            dest->point.x  = ray.origin.x + ray.direction.x * x;
            dest->point.y  = ray.origin.y + ray.direction.y * x;
            dest->point.z  = ray.origin.z + ray.direction.z * x;
            dest->normal   = rtCpuVec3Norm(dest->point, center);
            return true;
        }

        x = (-b + d) / a;
        if (x >= limit.x && x <= limit.y)
        {
            dest->distance = x;
            dest->point.x  = ray.origin.x + ray.direction.x * x;
            dest->point.y  = ray.origin.y + ray.direction.y * x;
            dest->point.z  = ray.origin.z + ray.direction.z * x;
            dest->normal   = rtCpuVec3Norm(dest->point, center);
            return true;
        }
    }
    return false;
}

RT_CPU_API bool rtCpuSphereTest(const rtVector3& center,
                                const rtScalar&  radius,
                                const rtCpuRay&  ray,
                                const rtVector2& limit)
{
    const rtVector3 vec = {
        ray.origin.x - center.x,
        ray.origin.y - center.y,
        ray.origin.z - center.z,
    };
    const rtScalar a =
        ray.direction.x * ray.direction.x +
        ray.direction.y * ray.direction.y +
        ray.direction.z * ray.direction.z;

    const rtScalar b =
        vec.x * ray.direction.x +
        vec.y * ray.direction.y +
        vec.z * ray.direction.z;

    const rtScalar r = radius * radius;

    const rtScalar c =
        vec.x * vec.x - r +
        (vec.y * vec.y - r) +
        (vec.z * vec.z - r);

    rtScalar d = b * b - a * c;

    if (d > 10e-4f)
    {
        d = skSqrt(d);

        rtScalar x = (-b - d) / a;
        if (x >= limit.x && x <= limit.y)
            return true;

        x = (-b + d) / a;
        if (x >= limit.x && x <= limit.y)
            return true;
    }
    return false;
}

SK_INLINE rtScalar rtCpuAbsF(rtScalar x)
{
    SKuint32 xi = *(SKuint32*)&x;
    xi &= 0x7FFFFFFF;
    return *(rtScalar*)&xi;
}

RT_CPU_API bool rtCpuBoxTest(const rtScalar   bbMin[3],
                             const rtScalar   bbMax[3],
                             const rtCpuRay&  ray,
                             const rtVector2& limit)
{
    const rtScalar* o = (const rtScalar*)&ray.origin.x;
    const rtScalar* d = (const rtScalar*)&ray.direction.x;

    rtScalar tMin = limit.x;
    rtScalar tMax = limit.y;

    for (int i = 0; i < 3; ++i)
    {
        rtScalar t0 = 0, t1 = 0;

        if (rtCpuAbsF(d[i]) > SK_EPSILON)
        {
            const rtScalar t2 = 1.f / d[i];

            t0 = (bbMin[i] - o[i]) * t2;
            t1 = (bbMax[i] - o[i]) * t2;

            if (t2 < 10e-4f)
            {
                const rtScalar t = t0;

                t0 = t1;
                t1 = t;
            }
        }

        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax < tMin)
            return false;
    }
    return true;
}

RT_CPU_API bool rtCpuBoxTest(rtCpuHitResult*  dest,
                             const rtScalar   bbMin[3],
                             const rtScalar   bbMax[3],
                             const rtCpuRay&  ray,
                             const rtVector2& limit)
{
    const rtScalar* o = (const rtScalar*)&ray.origin.x;
    const rtScalar* d = (const rtScalar*)&ray.direction.x;

    rtScalar tMin = limit.x;
    rtScalar tMax = limit.y;

    for (int i = 0; i < 3; ++i)
    {
        rtScalar t0 = 0, t1 = 0;

        if (rtCpuAbsF(d[i]) > SK_EPSILON)
        {
            const rtScalar t2 = 1.f / d[i];

            t0 = (bbMin[i] - o[i]) * t2;
            t1 = (bbMax[i] - o[i]) * t2;

            if (t2 < 10e-4f)
            {
                const rtScalar t = t0;

                t0 = t1;
                t1 = t;
            }
        }

        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax < tMin)
            return false;
    }

    if (dest)
    {
        dest->distance = tMin;
        dest->point.x  = ray.origin.x + ray.direction.x * tMin;
        dest->point.y  = ray.origin.y + ray.direction.y * tMin;
        dest->point.z  = ray.origin.z + ray.direction.z * tMin;

        const rtVector3 p1 = {
            dest->point.x - (bbMax[0] + bbMin[0]) * 0.5f,
            dest->point.y - (bbMax[1] + bbMin[1]) * 0.5f,
            dest->point.z - (bbMax[2] + bbMin[2]) * 0.5f,
        };
        const rtVector3 p2 = {
            ((bbMax[0] - bbMin[0]) * 0.5f),
            ((bbMax[1] - bbMin[1]) * 0.5f),
            ((bbMax[2] - bbMin[2]) * 0.5f),
        };

        const rtVector3 p3 = {
            (p1.x / p2.x),
            (p1.y / p2.y),
            (p1.z / p2.z),
        };

        const rtVector3 p4 = {
            p3.x < SK_EPSILON ? -p3.x : p3.x,
            p3.y < SK_EPSILON ? -p3.y : p3.y,
            p3.z < SK_EPSILON ? -p3.z : p3.z,
        };

        const rtScalar m = rtCpuMax3(p4);
        dest->normal     = {0, 0, 0};
        if (fabs(p4.x - m) <= SK_EPSILON)
            dest->normal.x = rtCpuSign(p3.x);
        else if (fabs(p4.y - m) <= SK_EPSILON)
            dest->normal.y = rtCpuSign(p3.y);
        else
            dest->normal.z = rtCpuSign(p3.z);
    }
    return true;
}

RT_CPU_API void rtCpuComputeRayDirection(rtVector3&       dir,
                                         const rtVector4& ori,
                                         const rtScalar&  x,
                                         const rtScalar&  y,
                                         const rtVector4& offset)
{
    // x = -tan(theta/2)*(w-2*x)/h
    dir.x = offset.w * (x + 10e-4f) * offset.y;

    // y = -tan(theta/2)*(h-2*y)/h
    dir.y = offset.w * (y + 10e-4f) * offset.y;
    dir.z = -1;

    rtCpuMulQuaternion(dir, dir, ori);
}

RT_CPU_API void rtCpuComputeRayDirection(rtVector3&       dir,
                                         const rtVector4& ori,
                                         const rtScalar&  x,
                                         const rtScalar&  y,
                                         const rtScalar&  ox,
                                         const rtScalar&  oy,
                                         const rtVector4& offset)
{
    // x = -tan(theta/2)*(w-2*x)/h
    dir.x = offset.w * (x + ox + 10e-4f) * offset.y;

    // y = -tan(theta/2)*(h-2*y)/h
    dir.y = offset.w * (y + oy + 10e-4f) * offset.y;
    dir.z = -1;

    rtCpuMulQuaternion(dir, dir, ori);
}
