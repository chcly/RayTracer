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
#ifndef _skQuaternion_h_
#define _skQuaternion_h_

#include "skMath.h"
#include "skVector3.h"

class skQuaternion
{
public:
    static const skQuaternion Identity;
    static const skQuaternion Zero;

    skScalar w, x, y, z;

public:
    skQuaternion() = default;

    skQuaternion(const skScalar nw, const skScalar nx, const skScalar ny, const skScalar nz) :
        w(nw),
        x(nx),
        y(ny),
        z(nz)
    {
    }

    skQuaternion(const skScalar xRad, const skScalar yRad, const skScalar zRad)
    {
        (*this).makeRotXYZ(xRad, yRad, zRad);
    }

    skQuaternion(const skVector3& vec)
    {
        (*this).makeRotXYZ(vec.x, vec.y, vec.z);
    }

    explicit skQuaternion(const skScalar* p) :
        w(p[0]),
        x(p[1]),
        y(p[2]),
        z(p[3])
    {
    }

    skQuaternion(const skQuaternion& v) = default;

    void makeIdentity()
    {
        w = 1;
        x = y = z = 0;
    }

    void makeRotXYZ(const skScalar xRad, const skScalar yRad, const skScalar zRad)
    {
        skQuaternion q0, q1, q2;
        q0.makeRotX(xRad);
        q1.makeRotY(yRad);
        q2.makeRotZ(zRad);

        *this = q2 * q1 * q0;
        this->normalize();
    }

    void makeRotX(const skScalar v)
    {
        skMath::sinCos(v * skScalar(0.5), x, w);
        y = z = 0;
    }

    void makeRotY(const skScalar v)
    {
        skMath::sinCos(v * skScalar(0.5), y, w);
        x = z = 0;
    }

    void makeRotZ(const skScalar v)
    {
        skMath::sinCos(v * skScalar(0.5), z, w);
        x = y = 0;
    }

    skVector3 toAxis() const
    {
        skScalar wSq = skScalar(1.0) - w * w;
        if (wSq <= skScalar(0.0))
            return skVector3::UnitZ;

        wSq = skScalar(1.0) / wSq;
        return skVector3(x * wSq, y * wSq, z * wSq);
    }

    skScalar length() const
    {
        const skScalar len = length2();
        if (len > SK_EPSILON)
        {
            return skSqrt(len);
        }
        return skScalar(0.0);
    }

    void normalize()
    {
        skScalar len = length2();
        if (len > SK_EPSILON)
        {
            len = skRSqrt(len);
            w *= len;
            x *= len;
            y *= len;
            z *= len;
        }
    }

    skQuaternion normalized() const
    {
        skQuaternion q(w, x, y, z);
        q.normalize();
        return q;
    }

    skQuaternion inverse() const
    {
        return skQuaternion(w, -x, -y, -z);
    }

    skQuaternion operator-() const
    {
        return skQuaternion(w, -x, -y, -z);
    }

    skQuaternion& invert()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    skQuaternion operator*(const skScalar& v) const
    {
        return skQuaternion(w * v, x * v, y * v, z * v);
    }

    skQuaternion& operator*=(const skScalar& v)
    {
        w *= v;
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    skQuaternion& operator*=(const skQuaternion& v)
    {
        w = w * v.w - x * v.x - y * v.y - z * v.z;
        x = w * v.x + x * v.w + y * v.z - z * v.y;
        y = w * v.y + y * v.w + z * v.x - x * v.z;
        z = w * v.z + z * v.w + x * v.y - y * v.x;
        return *this;
    }

    skQuaternion operator*(const skQuaternion& v) const
    {
        return skQuaternion(
            w * v.w - x * v.x - y * v.y - z * v.z,
            w * v.x + x * v.w + y * v.z - z * v.y,
            w * v.y + y * v.w + z * v.x - x * v.z,
            w * v.z + z * v.w + x * v.y - y * v.x);
    }

    skVector3 operator*(const skVector3& v) const
    {
        const skVector3 c(x, y, z);

        skVector3 a = c.cross(v);
        skVector3 b = c.cross(a);
        a *= skScalar(2) * w;
        b *= skScalar(2);
        return v + a + b;
    }

    skQuaternion operator+(const skScalar& v) const
    {
        return skQuaternion(w + v, x + v, y + v, z + v);
    }

    skQuaternion operator+(const skQuaternion& v) const
    {
        return skQuaternion(w + v.w, x + v.x, y + v.y, z + v.z);
    }

    skQuaternion operator-(const skScalar& v) const
    {
        return skQuaternion(w - v, x - v, y - v, z - v);
    }

    skQuaternion operator-(const skQuaternion& v) const
    {
        return skQuaternion(w - v.w, x - v.x, y - v.y, z - v.z);
    }

    SK_INLINE bool operator==(const skQuaternion& v) const
    {
        return skEq(x, v.x) && skEq(y, v.y) && skEq(z, v.z) && skEq(w, v.w);
    }

    SK_INLINE bool operator!=(const skQuaternion& v) const
    {
        return skNeq(x, v.x) && skNeq(y, v.y) && skNeq(z, v.z) && skNeq(w, v.w);
    }

    SK_INLINE skScalar length2() const
    {
        return w * w + x * x + y * y + z * z;
    }

    SK_INLINE skScalar* ptr()
    {
        return &w;
    }

    SK_INLINE const skScalar* ptr() const
    {
        return &w;
    }

    void print() const;
};

#endif  //_skQuaternion_h_
