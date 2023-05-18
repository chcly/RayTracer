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
#include "RenderSystem/Cuda/rtCudaHostMath.h"
#include "RenderSystem/Cuda/rtCudaUtils.h"

#define RT_EPSILON 10e-4f

#define rtRsqrt(x) (__frsqrt_rn(x))

RT_CUD_API rtScalar rtMax(const rtScalar& a, const rtScalar& b)
{
    return a > b ? a : b;
}

RT_CUD_API void rtCuMix(rtVector3* a, rtVector3* b, rtScalar fac)
{
    const rtScalar iFac = 1.f - fac;

    a->x = iFac * a->x + fac * b->x;
    a->y = iFac * a->y + fac * b->y;
    a->z = iFac * a->z + fac * b->z;
}

RT_CUD_API void rtCuMix3(rtVector3* a, const rtVector3* b, const rtVector3* c, rtScalar fac)
{
    if (fac < 0)
        fac = 0;
    if (fac > 1)
        fac = 1;
    const rtScalar iFac = 1.f - fac;

    a->x = fac * b->x + iFac * c->x;
    a->y = fac * b->y + iFac * c->y;
    a->z = fac * b->z + iFac * c->z;
}

RT_CUD_API void rtCuLimit3(rtVector3* a, const rtScalar& mi, const rtScalar& ma)
{
    a->x = a->x < mi ? mi : a->x > ma ? ma
                                      : a->x;
    a->y = a->y < mi ? mi : a->y > ma ? ma
                                      : a->y;
    a->z = a->z < mi ? mi : a->z > ma ? ma
                                      : a->z;
}

RT_CUD_API void rtCuSaturate(rtVector3* a)
{
    a->x = (a->x) < 0.f ? 0 : a->x > 1.f ? 1.f
                                         : a->x;
    a->y = (a->y) < 0.f ? 0 : a->y > 1.f ? 1.f
                                         : a->y;
    a->z = (a->z) < 0.f ? 0 : a->z > 1.f ? 1.f
                                         : a->z;
}

RT_CUD_API void rtCuSaturate(rtScalar* a)
{
    *a = *a < 0.f ? 0.f : *a > 1.f ? 1.f
                                   : *a;
}

RT_CUD_API rtVector3 rtCuAddFloat3(const rtVector3& a, const rtVector3& b)
{
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}
RT_CUD_API rtVector3 rtCuAddFloat3(rtVector3 a, rtScalar b)
{
    return {
        a.x + b,
        a.y + b,
        a.z + b,
    };
}

RT_CUD_API rtVector3 rtCuAdd3Float3(const rtVector3& a, const rtVector3& b, const rtVector3& c)
{
    return {
        a.x + b.x + c.x,
        a.y + b.y + c.y,
        a.z + b.z + c.z,
    };
}

RT_CUD_API rtVector3 rtCuAdd3Float3(const rtVector3& a, const rtVector3& b, const rtScalar& c)
{
    return {
        a.x + b.x + c,
        a.y + b.y + c,
        a.z + b.z + c,
    };
}

RT_CUD_API rtVector3 rtCuCrossFloat3(const rtVector3& a, const rtVector3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

RT_CUD_API rtVector3 rtCuMulFloat3(const rtVector3& a, const rtScalar& s)
{
    return {
        a.x * s,
        a.y * s,
        a.z * s,
    };
}

RT_CUD_API rtVector3 rtCuMulFloat3v(const rtVector3& a, const rtVector3& v)
{
    return {
        a.x * v.x,
        a.y * v.y,
        a.z * v.z,
    };
}

RT_CUD_API rtScalar rtCuFloat3Dot(const rtVector3& a, const rtVector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

RT_CUD_API rtScalar rtCuFloat3Len(const rtVector3& a)
{
    const rtScalar dp = a.x * a.x + a.y * a.y + a.z * a.z;
    if (dp > RT_EPSILON)
        return sqrtf(dp);
    return 0;
}

RT_CUD_API rtVector3 rtCuFloat3Norm(const rtVector3& a)
{
    rtVector3 r{a.x, a.y, a.z};
    rtScalar  dp = r.x * r.x + r.y * r.y + r.z * r.z;
    if (dp > RT_EPSILON)
    {
        dp  = rtRsqrt(dp);
        r.x *= dp;
        r.y *= dp;
        r.z *= dp;
    }
    return r;
}

RT_CUD_API void rtCuFloat3Norm(rtVector3* a)
{
    rtScalar dp = a->x * a->x + a->y * a->y + a->z * a->z;
    if (dp > RT_EPSILON)
    {
        dp = rtRsqrt(dp);
        a->x *= dp;
        a->y *= dp;
        a->z *= dp;
    }
}

RT_CUD_API rtScalar rtCuFloat3DistSqu(const rtVector3& a, const rtVector3& b)
{
    const rtVector3 r{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
    return r.x * r.x + r.y * r.y + r.z * r.z;
}

RT_CUD_API rtVector3 rtCuFloat3Norm(const rtVector3& a, const rtVector3& b)
{
    rtVector3 r{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };

    rtScalar dp = r.x * r.x + r.y * r.y + r.z * r.z;
    if (dp > 0)
    {
        dp = rtRsqrt(dp);
        r.x *= dp;
        r.y *= dp;
        r.z *= dp;
    }
    return r;
}

RT_CUD_API rtVector3 rtCuFloat3Sub(const rtVector3& a, const rtVector3& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

RT_CUD_API void rtCuMulQuat(rtVector3& d, const rtVector3& v, const rtVector4& q)
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

    d.x = v.x + a.x + b.x;
    d.y = v.y + a.y + b.y;
    d.z = v.z + a.z + b.z;

    v1 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (v1 > SK_EPSILON)
    {
        v1 = rtRsqrt(v1);
        d.x *= v1;
        d.y *= v1;
        d.z *= v1;
    }
}

RT_CUD_API rtVector3 rtCuReflectFloat3(const rtVector3& a,
                                       const rtVector3& b)
{
    // R = 2*(L.N)*N-L, where a = (l-p), and b = surface normal

    const rtScalar d = 2.f * (a.x * b.x + a.y * b.y + a.z * b.z);
    return {
        b.x * d - a.x,
        b.y * d - a.y,
        b.z * d - a.z};
}

RT_CUD_API bool rtCuSphereTest(const rtVector3& center,
                               const rtScalar&  radius,
                               const rtCudaRay& ray,
                               const rtVector2& limit)
{
    const rtVector3 vec = {
        ray.origin.x - center.x,
        ray.origin.y - center.y,
        ray.origin.z - center.z,
    };

    rtScalar a =
        ray.direction.x * ray.direction.x +
        ray.direction.y * ray.direction.y +
        ray.direction.z * ray.direction.z;

    const rtScalar b =
        vec.x * ray.direction.x +
        vec.y * ray.direction.y +
        vec.z * ray.direction.z;

    const rtScalar r = radius * radius;

    const rtScalar c =
        (vec.x * vec.x - r) +
        (vec.y * vec.y - r) +
        (vec.z * vec.z - r);

    rtScalar d = b * b - a * c;

    if (d > RT_EPSILON)
    {
        d = sqrtf(d);

        rtScalar x = (-b - d) / a;
        if (x >= limit.x && x <= limit.y)
            return true;

        x = (-b + d) / a;
        if (x >= limit.x && x <= limit.y)
            return true;
    }
    return false;
}

RT_CUD_API bool rtCuSphereTest(rtCudaHitResult* dest,
                               const rtVector3& center,
                               const rtScalar&  radius,
                               const rtCudaRay& ray,
                               const rtVector2& limit)
{
    const rtVector3 vec = {
        ray.origin.x - center.x,
        ray.origin.y - center.y,
        ray.origin.z - center.z,
    };

    rtScalar a =
        ray.direction.x * ray.direction.x +
        ray.direction.y * ray.direction.y +
        ray.direction.z * ray.direction.z;

    const rtScalar b =
        vec.x * ray.direction.x +
        vec.y * ray.direction.y +
        vec.z * ray.direction.z;

    const rtScalar r = radius * radius;

    const rtScalar c =
        (vec.x * vec.x - r) +
        (vec.y * vec.y - r) +
        (vec.z * vec.z - r);

    rtScalar d = b * b - a * c;

    if (d > RT_EPSILON)
    {
        d          = sqrtf(d);
        rtScalar x = (-b - d) / a;
        if (x >= limit.x && x <= limit.y)
        {
            dest->distance = x;
            dest->point.x  = ray.origin.x + ray.direction.x * x;
            dest->point.y  = ray.origin.y + ray.direction.y * x;
            dest->point.z  = ray.origin.z + ray.direction.z * x;
            dest->normal   = rtCuFloat3Norm(dest->point, center);
            return true;
        }

        x = (-b + d) / a;
        if (x >= limit.x && x <= limit.y)
        {
            dest->distance = x;
            dest->point.x  = ray.origin.x + ray.direction.x * x;
            dest->point.y  = ray.origin.y + ray.direction.y * x;
            dest->point.z  = ray.origin.z + ray.direction.z * x;
            dest->normal   = rtCuFloat3Norm(dest->point, center);
            return true;
        }
    }
    return false;
}

RT_CUD_API rtScalar rtCuMaxFloat3(const rtVector3& a)
{
    return rtMax(a.x, rtMax(a.y, a.z));
}

RT_CUD_API rtScalar rtSign(const rtScalar& a)
{
    return a < 0.f ? -1.0 : 1.0;
}

RT_CUD_API rtScalar rtCuAbsF(rtScalar x)
{
    SKuint32 xi = *(SKuint32*)&x;
    xi &= 0x7FFFFFFF;
    return *(rtScalar*)&xi;
}


RT_CUD_API bool rtCuBoxTest(rtCudaHitResult* dest,
                            const rtScalar   bbMin[3],
                            const rtScalar   bbMax[3],
                            const rtCudaRay& ray,
                            const rtVector2& limit)
{
    const rtScalar* o = &ray.origin.x;
    const rtScalar* d = &ray.direction.x;

    rtScalar tMin = limit.x;
    rtScalar tMax = limit.y;

    for (int i = 0; i < 3; ++i)
    {
        rtScalar t0 = 0, t1 = 0;

        if (rtCuAbsF(d[i]) > SK_EPSILON)
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

    dest->distance = tMin;
    dest->point.x  = ray.origin.x + ray.direction.x * tMin;
    dest->point.y  = ray.origin.y + ray.direction.y * tMin;
    dest->point.z  = ray.origin.z + ray.direction.z * tMin;

    const rtVector3 p1 = {
        dest->point.x - ((bbMax[0] + bbMin[0]) * 0.5f),
        dest->point.y - ((bbMax[1] + bbMin[1]) * 0.5f),
        dest->point.z - ((bbMax[2] + bbMin[2]) * 0.5f),
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

    rtVector3 p4 = {
        p3.x < SK_EPSILON ? -p3.x : p3.x,
        p3.y < SK_EPSILON ? -p3.y : p3.y,
        p3.z < SK_EPSILON ? -p3.z : p3.z,
    };
    const rtScalar m   = rtCuMaxFloat3(p4);
    dest->normal = {0, 0, 0};
    if (fabs(p4.x - m) <= SK_EPSILON)
        dest->normal.x = rtSign(p3.x);
    else if (fabs(p4.y - m) <= SK_EPSILON)
        dest->normal.y = rtSign(p3.y);
    else
        dest->normal.z = rtSign(p3.z);
    return true;
}

RT_CUD_API bool rtCuBoxTest(const rtScalar   bbMin[3],
                            const rtScalar   bbMax[3],
                            const rtCudaRay& ray,
                            const rtVector2& limit)
{
    const rtScalar* o = &ray.origin.x;
    const rtScalar* d = &ray.direction.x;

    rtScalar tMin = limit.x;
    rtScalar tMax = limit.y;

    for (int i = 0; i < 3; ++i)
    {
        rtScalar t0 = 0, t1 = 0;

        if (rtCuAbsF(d[i]) > SK_EPSILON)
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
