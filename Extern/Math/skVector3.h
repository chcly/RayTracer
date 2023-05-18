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
#ifndef _skVector3_h_
#define _skVector3_h_

#include "skMath.h"
class skColor;

class skVector3
{
public:
    skVector3()
    {
    }

    skVector3(skScalar nx, skScalar ny, skScalar nz) :
        x(nx),
        y(ny),
        z(nz)
    {
    }

    explicit skVector3(const skColor& col);

    explicit skVector3(const float* p)
    {
        if (p)
        {
            x = (skScalar)p[0];
            y = (skScalar)p[1];
            z = (skScalar)p[2];
        }
        else
            x = y = z = 0;
    }

    explicit skVector3(const double* p)
    {
        if (p)
        {
            x = (skScalar)p[0];
            y = (skScalar)p[1];
            z = (skScalar)p[2];
        }
        else
            x = y = z = 0;
    }

    skVector3(const skVector3& v) = default;

    SK_INLINE skScalar* ptr()
    {
        return &x;
    }

    SK_INLINE const skScalar* ptr() const
    {
        return &x;
    }

    SK_INLINE bool operator==(const skVector3& v) const
    {
        return skEq(x, v.x) && skEq(y, v.y) && skEq(z, v.z);
    }

    SK_INLINE bool operator!=(const skVector3& v) const
    {
        return !skEq(x, v.x) && !skEq(y, v.y) && !skEq(z, v.z);
    }

    SK_INLINE skVector3 operator+(skScalar v) const
    {
        return skVector3(x + v, y + v, z + v);
    }

    SK_INLINE skVector3 operator+(const skVector3& v) const
    {
        return skVector3(x + v.x, y + v.y, z + v.z);
    }

    skVector3& operator+=(skScalar v)
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    skVector3& operator+=(const skVector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    static void majorAxis(skVector3& dest, const skVector3& src)
    {
        const skScalar m = skMax3(src.x, src.y, src.z);

        if (skEq(m, src.x))
            dest = UnitX;
        else if (skEq(m, src.y))
            dest = UnitY;
        else
            dest = UnitZ;
    }

    skVector3 majorAxis() const
    {
        skVector3 result;
        majorAxis(result, *this);
        return result;
    }

    skVector3 abs() const
    {
        return {
            skAbs(x),
            skAbs(y),
            skAbs(z)};
    }

    SK_INLINE skVector3 operator-(skScalar v) const
    {
        return skVector3(x - v, y - v, z - v);
    }

    SK_INLINE skVector3 operator-(const skVector3& v) const
    {
        return skVector3(x - v.x, y - v.y, z - v.z);
    }

    skVector3& operator-=(skScalar v)
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    skVector3& operator-=(const skVector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    SK_INLINE skVector3 operator-() const
    {
        return skVector3(-x, -y, -z);
    }

    SK_INLINE skVector3 operator*(skScalar v) const
    {
        return skVector3(x * v, y * v, z * v);
    }

    SK_INLINE skVector3 operator*(const skVector3& v) const
    {
        return skVector3(x * v.x, y * v.y, z * v.z);
    }

    skVector3& operator*=(skScalar v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    skVector3& operator*=(const skVector3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    SK_INLINE skVector3 operator/(skScalar v) const
    {
        return skVector3(x / v, y / v, z / v);
    }

    SK_INLINE skVector3 operator/(const skVector3& v) const
    {
        return skVector3(x / v.x, y / v.y, z / v.z);
    }

    skVector3& operator/=(skScalar v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    skVector3& operator/=(const skVector3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    SK_INLINE skScalar length() const
    {
        return skSqrt(length2());
    }

    SK_INLINE skScalar length2() const
    {
        return x * x + y * y + z * z;
    }

    SK_INLINE skScalar dot(const skVector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    SK_INLINE skScalar distance(const skVector3& v) const
    {
        return skVector3(x - v.x, y - v.y, z - v.z).length();
    }

    SK_INLINE skScalar distance2(const skVector3& v) const
    {
        return skVector3(x - v.x, y - v.y, z - v.z).length2();
    }

    skVector3 cross(const skVector3& v) const
    {
        return skVector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    SK_INLINE skScalar max3() const
    {
        return skMax3(x, y, z);
    }

    void normalize()
    {
        const skScalar sl = length2();
        if (sl > SK_EPSILON)
        {
            const skScalar rs = skRSqrt(sl);
            x *= rs;
            y *= rs;
            z *= rs;
        }
    }

    skVector3 normalized() const
    {
        const skScalar sl = length2();
        if (sl > SK_EPSILON)
        {
            const skScalar rs = skRSqrt(sl);
            return {x * rs, y * rs, z * rs};
        }

        return Zero;
    }

    void print() const;

    skScalar x, y, z;

public:
    static const skVector3 Unit;
    static const skVector3 UnitX;
    static const skVector3 UnitY;
    static const skVector3 UnitZ;
    static const skVector3 Zero;
};

SK_INLINE skVector3 operator-(skScalar r, const skVector3& l)
{
    return skVector3(l.x - r, l.y - r, l.z - r);
}

SK_INLINE skVector3 operator+(skScalar r, const skVector3& l)
{
    return skVector3(l.x + r, l.y + r, l.z + r);
}

SK_INLINE skVector3 operator/(skScalar r, const skVector3& l)
{
    return skVector3(l.x / r, l.y / r, l.z / r);
}

SK_INLINE skVector3 operator*(skScalar r, const skVector3& l)
{
    return skVector3(l.x * r, l.y * r, l.z * r);
}

#endif  //_skVector3_h_
