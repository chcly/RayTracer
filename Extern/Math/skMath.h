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
#ifndef _skMath_h_
#define _skMath_h_

#include "skScalar.h"

#ifdef SK_DOUBLE
#define skSqrt (skScalar) sqrt
#define skInvSqrt (skScalar)1.0 / (skScalar)sqrt
#define skFloor (skScalar) floor
#define skCeil (skScalar) ceil
#define skSin (skScalar) sin
#define skCos (skScalar) cos
#define skTan (skScalar) tan
#define skExp (skScalar) exp
#define skLog (skScalar) log
#define skPow (skScalar) pow
#define skMod (skScalar) fmod
#define skASin (skScalar) asin
#define skACos (skScalar) acos
#define skATan (skScalar) atan
#define skATan2 (skScalar) atan2
#define skFmod (skScalar) fmod
#else
#define skSqrt (skScalar) sqrtf
#define skRSqrt(x) (powf((x), -0.5f))
#define skFloor (skScalar) floorf
#define skCeil (skScalar) ceilf
#define skSin (skScalar) sinf
#define skCos (skScalar) cosf
#define skTan (skScalar) tanf
#define skExp (skScalar) expf
#define skLog (skScalar) logf
#define skPow (skScalar) powf
#define skMod (skScalar) fmodf
#define skASin (skScalar) asinf
#define skACos (skScalar) acosf
#define skATan (skScalar) atanf
#define skATan2 (skScalar) atan2f
#define skFmod (skScalar) fmodf
#endif

SK_INLINE skScalar skAbs(const skScalar& v)
{
    return v < 0 ? -v : v;
}

SK_INLINE skScalar skSqu(const skScalar& v)
{
    return v * v;
}

SK_INLINE skScalar skSign(const skScalar& v)
{
    return v > 0 ? skScalar(1.) : skScalar(-1.);
}

SK_INLINE skScalar skIsInf(const skScalar v)
{
    return skAbs(v) >= SK_INFINITY;
}

SK_INLINE skScalar skIsNan(const skScalar v)
{
    return std::isnan(v);
}

SK_INLINE skScalar skDecimalPart(const skScalar v)
{
    return v - skFloor(v);
}

SK_INLINE void skSplitScalar(const skScalar v, skScalar& whole, skScalar& decimal)
{
    whole   = skFloor(v);
    decimal = v - whole;
}

SK_INLINE bool skIsZero(const skScalar v)
{
    return skAbs(v) < SK_EPSILON;
}

SK_INLINE bool skIsZero(skScalar v, skScalar tol)
{
    return skAbs(v) < tol;
}

SK_INLINE bool skEq(skScalar x, skScalar y)
{
    return skAbs(x - y) < SK_EPSILON;
}

SK_INLINE skScalar skClampf(const skScalar& v, const skScalar& vMin, const skScalar& vMax)
{
    return v < vMin ? vMin : v > vMax ? vMax : v;
}


SK_INLINE bool skNeq(skScalar x, skScalar y)
{
    return !skEq(x, y);
}

SK_INLINE bool skEqT(skScalar x, skScalar y, skScalar tol)
{
    return skAbs(x - y) < tol;
}


const skScalar skPi     = skScalar(4.0 * skATan(1.0));
const skScalar skPi4    = skScalar(4.0) * skPi;
const skScalar skPi2    = skScalar(2.0) * skPi;
const skScalar skPiH    = skScalar(0.5) * skPi;            // 90
const skScalar skPiO3   = skScalar(skPi) / skScalar(3.0);  // 60
const skScalar skPiO4   = skScalar(skPi) / skScalar(4.0);  // 45
const skScalar skPiO6   = skScalar(skPi) / skScalar(6.0);  // 30
const skScalar skDPR    = skScalar(180.0) / skPi;
const skScalar skRPD    = skScalar(1.0) / skDPR;
const skScalar skInvPi  = skScalar(1.0) / skPi;
const skScalar skInvPi2 = skScalar(1.0) / skPi2;

SK_INLINE skScalar skDegrees(skScalar v)
{
    return v * skDPR;
}

SK_INLINE skScalar skRadians(skScalar v)
{
    return v * skRPD;
}

class skMath
{
public:
    static void ortho2D(class skTransform2D& dest, skScalar l, skScalar t, skScalar r, skScalar b);
    static void ortho2D(class skMatrix4& dest, skScalar l, skScalar t, skScalar r, skScalar b);
    static void projection(class skMatrix4& dest, skScalar fov, skScalar aspect, skScalar zNear, skScalar zFar);

    static void view(class skMatrix4& dest, const class skVector3& pos, const class skQuaternion& rot);
    static void view(class skMatrix4& dest, const class skVector3& pos, const class skMatrix3& rot);

    static int  pow2(int n);
    static void forceAlign(skScalar& val, int mod);

    static skScalar wrap2Pi(skScalar x);
    static void     sinCos(const skScalar& theta, skScalar& y, skScalar& x);

    static skScalar toMillimeters(const skScalar& deg);
    static skScalar toFieldOfView(const skScalar& mm);
};

#endif  //_skMath_h_
