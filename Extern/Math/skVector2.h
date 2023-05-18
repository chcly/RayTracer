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
#ifndef _skVector2_h_
#define _skVector2_h_

#include "skMath.h"

class skVector2
{
public:
    skVector2()
    {
    }

    skVector2(const skScalar nx, const skScalar ny) :
        x(nx),
        y(ny)
    {
    }

    explicit skVector2(skScalar* p) :
        x(p[0]),
        y(p[1])
    {
    }

    skVector2(const skVector2& v) = default;

    SK_INLINE skScalar* ptr()
    {
        return &x;
    }

    SK_INLINE const skScalar* ptr() const
    {
        return &x;
    }

    skVector2& operator=(const skVector2& v) = default;

    SK_INLINE bool operator==(const skVector2& v) const
    {
        return skEq(x, v.x) && skEq(y, v.y);
    }

    SK_INLINE bool operator!=(const skVector2& v) const
    {
        return !skEq(x, v.x) && !skEq(y, v.y);
    }

    SK_INLINE bool operator<(const skVector2& v) const
    {
        return x < v.x && y < v.y;
    }

    SK_INLINE bool operator>(const skVector2& v) const
    {
        return x > v.x && y > v.y;
    }

    SK_INLINE bool operator<=(const skVector2& v) const
    {
        return x <= v.x && y <= v.y;
    }

    SK_INLINE bool operator>=(const skVector2& v) const
    {
        return x >= v.x && y >= v.y;
    }

    SK_INLINE skVector2 operator+(skScalar v) const
    {
        return skVector2(x + v, y + v);
    }

    SK_INLINE skVector2 operator+(const skVector2& v) const
    {
        return skVector2(x + v.x, y + v.y);
    }

    SK_INLINE skVector2& operator+=(skScalar v)
    {
        x += v;
        y += v;
        return *this;
    }

    SK_INLINE skVector2& operator+=(const skVector2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    friend SK_INLINE skVector2 operator+(skScalar r, const skVector2& l)
    {
        return skVector2(l.x + r, l.y + r);
    }

    SK_INLINE skVector2 operator-(skScalar v) const
    {
        return skVector2(x - v, y - v);
    }

    SK_INLINE skVector2 operator-(const skVector2& v) const
    {
        return skVector2(x - v.x, y - v.y);
    }

    SK_INLINE skVector2& operator-=(skScalar v)
    {
        x -= v;
        y -= v;
        return *this;
    }

    SK_INLINE skVector2& operator-=(const skVector2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    SK_INLINE skVector2 operator-() const
    {
        return skVector2(-x, -y);
    }

    friend SK_INLINE skVector2 operator-(skScalar r, const skVector2& l)
    {
        return skVector2(l.x - r, l.y - r);
    }

    SK_INLINE skVector2 operator*(skScalar v) const
    {
        return skVector2(x * v, y * v);
    }

    SK_INLINE skVector2 operator*(const skVector2& v) const
    {
        return skVector2(x * v.x, y * v.y);
    }

    SK_INLINE skVector2& operator*=(skScalar v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    SK_INLINE skVector2& operator*=(const skVector2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    friend SK_INLINE skVector2 operator*(skScalar r, const skVector2& l)
    {
        return skVector2(l.x * r, l.y * r);
    }

    SK_INLINE skVector2 operator/(skScalar v) const
    {
        return skVector2(x / v, y / v);
    }

    SK_INLINE skVector2 operator/(const skVector2& v) const
    {
        return skVector2(x / v.x, y / v.y);
    }

    SK_INLINE skVector2& operator/=(skScalar v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    SK_INLINE skVector2& operator/=(const skVector2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    friend SK_INLINE skVector2 operator/(skScalar r, const skVector2& l)
    {
        return skVector2(l.x / r, l.y / r);
    }

    SK_INLINE skScalar length() const
    {
        return skSqrt(length2());
    }

    SK_INLINE skScalar length2() const
    {
        return dot(*this);
    }

    SK_INLINE skScalar dot(const skVector2& v) const
    {
        return x * v.x + y * v.y;
    }

    SK_INLINE skVector2 abs() const
    {
        return skVector2(skAbs(x), skAbs(y));
    }

    SK_INLINE skScalar distance(const skVector2& v) const
    {
        return skVector2(x - v.x, y - v.y).length();
    }

    SK_INLINE skScalar distance2(const skVector2& v) const
    {
        return skVector2(x - v.x, y - v.y).length2();
    }

    SK_INLINE skVector2 perpendicular() const
    {
        return skVector2(-y, x);
    }

    SK_INLINE bool isZero() const
    {
        return skIsZero(x) && skIsZero(y);
    }

    void normalize()
    {
        skScalar len = x * x + y * y;
        if (len > SK_EPSILON)
        {
            len = skRSqrt(len);
            x *= len;
            y *= len;
        }
    }

    skVector2 normalized() const
    {
        skVector2 v(x, y);
        v.normalize();
        return v;
    }

    SK_INLINE bool contains(skScalar x1, skScalar y1, skScalar x2, skScalar y2) const
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    SK_INLINE skScalar hx() const
    {
        return x * skScalar(0.5);
    }

    SK_INLINE skScalar hy() const
    {
        return y * skScalar(0.5);
    }

    void constrainX(skScalar min, skScalar max)
    {
        if (x < min)
            x = min;
        if (x > max)
            x = max;
    }

    void constrainY(skScalar min, skScalar max)
    {
        if (y < min)
            y = min;
        if (y > max)
            y = max;
    }

    void constrain(skScalar x1, skScalar y1, skScalar x2, skScalar y2)
    {
        constrainX(x1, x2);
        constrainY(y1, y2);
    }

    skScalar minValue() const
    {
        return x < y ? x : y;
    }

    skScalar maxValue() const
    {
        return x > y ? x : y;
    }


    void print() const;

public:
    skScalar x, y;

    static const skVector2 Unit;
    static const skVector2 UnitX;
    static const skVector2 UnitY;
    static const skVector2 Zero;
};

#endif  //_skVector2_h_
