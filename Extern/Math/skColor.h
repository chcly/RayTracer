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
#ifndef _skColor_h_
#define _skColor_h_

#include "skMath.h"
class skVector3;

typedef unsigned int skColori;

class skColorHSV;
class skColor;

typedef union skColorU
{
    unsigned char b[4];
    unsigned int  i;
} skColorU;

typedef union skColorUF
{
    unsigned char b[4];
    float         f;
} skColorUF;

class skColorUtils
{
public:
    static const skScalar i100;
    static const skScalar i255;
    static const skScalar i360;

    static void convert(skColori& dst, const skColor& src);
    static void convert(skColor& dst, const skColori& src);
    static void convert(skColori& dst, const SKubyte* src);
    static void convert(skColor& dst, const skColorHSV& src);
    static void convert(skColorHSV& dst, const skColor& src);
    static void convert(SKubyte*& dst, const skColor& src);
    static void convert(skColor& dst, const SKubyte* src);
    static void convert(SKubyte*& dst, const skScalar& src);
    static void convert(SKubyte*& dst, const SKuint32& src);
    static void convert(skColor& dst, const skScalar& src);
};

class skColorHSV
{
public:
    skColorHSV() :
        h(0),
        s(0),
        v(0),
        a(0)
    {
    }

    skColorHSV(const skColori& _h,
               const skColori& _s,
               const skColori& _v,
               const skColori& _a = 100) :
        h(skScalar(_h) * skColorUtils::i360),
        s(skScalar(_s) * skColorUtils::i100),
        v(skScalar(_v) * skColorUtils::i100),
        a(skScalar(_a) * skColorUtils::i100)
    {
    }

    skColorHSV(const skColor& rgb) :
        h(0),
        s(0),
        v(0),
        a(0)
    {
        skColorUtils::convert(*this, rgb);
    }

    void setHue(const skColori& _h)
    {
        h = skScalar(_h);
    }

    void setSaturation(const skColori& _s)
    {
        s = skScalar(_s) * skColorUtils::i100;
    }
    void setValue(const skColori& _v)
    {
        v = skScalar(_v) * skColorUtils::i100;
    }
    void saturate(skScalar fac)
    {
        s = skClamp(s + fac, skScalar(0.0), skScalar(1.0));
    }
    void lighten(skScalar fac)
    {
        v = skClamp(v + fac, skScalar(0.0), skScalar(1.0));
    }

    void toInt(skColori& _h, skColori& _s, skColori& _v, skColori& _a) const
    {
        _h = skColori(h);
        _s = skColori(s * 100);
        _v = skColori(v * 100);
        _a = skColori(a * 100);
    }

    // h is 0, 360;
    // s, v, a [0,1]
    skScalar h, s, v, a;
};

class skColor
{
public:
    skScalar r, g, b, a;

    static const skColor White;
    static const skColor Black;

public:
    skColor()
    {
    }

    skColor(skScalar _r, skScalar _g, skScalar _b, skScalar _a = skScalar(1.0)) :
        r(_r),
        g(_g),
        b(_b),
        a(_a)
    {
    }

    skColor(const skColor& o) = default;

    explicit skColor(const skColori& color)
    {
        skColorUtils::convert(*this, color);
    }

    operator skColori() const
    {
        skColori i;
        skColorUtils::convert(i, *this);
        return i;
    }

    void set(skScalar _r, skScalar _g, skScalar _b, skScalar _a = skScalar(1.0))
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void setMonochrome(skScalar _c, skScalar _a = skScalar(1.0))
    {
        r = _c;
        g = _c;
        b = _c;
        a = _a;
    }

    void setUb(SKubyte _r, SKubyte _g, SKubyte _b, SKubyte _a = 255)
    {
        r = skScalar(_r) * skColorUtils::i255;
        g = skScalar(_g) * skColorUtils::i255;
        b = skScalar(_b) * skColorUtils::i255;
        a = skScalar(_a) * skColorUtils::i255;
    }

    explicit skColor(const skColorHSV& hsv)
    {
        skColorUtils::convert(*this, hsv);
    }

    explicit skColor(const skVector3& v);

    void setHue(const skColori& h)
    {
        skColorHSV hsv;
        skColorUtils::convert(hsv, *this);
        hsv.h = skScalar(h);
        skColorUtils::convert(*this, hsv);
    }

    void setSaturation(const skColori& s)
    {
        skColorHSV hsv;
        skColorUtils::convert(hsv, *this);
        hsv.s = skScalar(s) * skColorUtils::i100;
        skColorUtils::convert(*this, hsv);
    }

    void setValue(const skColori& v)
    {
        skColorHSV hsv;
        skColorUtils::convert(hsv, *this);
        hsv.v = skScalar(v) * skColorUtils::i100;
        skColorUtils::convert(*this, hsv);
    }

    void saturate(skScalar fac)
    {
        skColorHSV hsv;
        skColorUtils::convert(hsv, *this);
        hsv.saturate(fac);
        skColorUtils::convert(*this, hsv);
    }

    void lighten(skScalar fac)
    {
        skColorHSV hsv;
        skColorUtils::convert(hsv, *this);
        hsv.lighten(fac);
        skColorUtils::convert(*this, hsv);
    }

    skColori asInt() const
    {
        skColori i;
        skColorUtils::convert(i, *this);
        return i;
    }

    void limit()
    {
        r = skClamp<skScalar>(r, 0, 1);
        g = skClamp<skScalar>(g, 0, 1);
        b = skClamp<skScalar>(b, 0, 1);
        a = skClamp<skScalar>(a, 0, 1);
    }

    void limit(skColor& d) const
    {
        const skScalar* srcPtr = ptr();
        skScalar*       dstPtr = d.ptr();

        for (int i = 0; i < 4; ++i)
        {
            if (srcPtr[i] < skScalar(0))
                dstPtr[i] = 0;
            else if (srcPtr[i] > skScalar(1))
                dstPtr[i] = 1;
            else
                dstPtr[i] = srcPtr[i];
        }
    }

    void invertRGB()
    {
        r = 1 - r;
        g = 1 - g;
        b = 1 - b;
    }

    skColor limit() const
    {
        return skColor(
            skClamp<skScalar>(r, 0, 1),
            skClamp<skScalar>(g, 0, 1),
            skClamp<skScalar>(b, 0, 1),
            skClamp<skScalar>(a, 0, 1));
    }

    skColor operator+(skScalar v) const
    {
        return skColor(r + v, g + v, b + v, a);
    }

    skColor operator+(const skColor& v) const
    {
        return skColor(r + v.r, g + v.g, b + v.b, a);
    }

    skColor& operator+=(skScalar v)
    {
        r += v;
        g += v;
        b += v;
        return *this;
    }
    skColor& operator+=(const skColor& v)
    {
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    friend skColor operator+(const skScalar rc, const skColor& l)
    {
        return l + rc;
    }

    skColor operator-(skScalar v) const
    {
        return skColor(r - v, g - v, b - v, a);
    }

    skColor operator-(const skColor& v) const
    {
        return skColor(r - v.r, g - v.g, b - v.b, a);
    }

    skColor& operator-=(skScalar v)
    {
        r -= v;
        g -= v;
        b -= v;
        return *this;
    }

    skColor& operator-=(const skColor& v)
    {
        r -= v.r;
        g -= v.g;
        b -= v.b;
        return *this;
    }

    friend skColor operator-(const skScalar rc, const skColor& l)
    {
        return l - rc;
    }

    skColor operator*(skScalar v) const
    {
        return skColor(r * v, g * v, b * v, a);
    }

    skColor operator*(const skColor& v) const
    {
        return skColor(r * v.r, g * v.g, b * v.b, a);
    }

    skColor& operator*=(skScalar v)
    {
        r *= v;
        g *= v;
        b *= v;
        return *this;
    }

    skColor& operator*=(const skColor& v)
    {
        r *= v.r;
        g *= v.g;
        b *= v.b;
        return *this;
    }

    friend skColor operator*(skScalar rc, const skColor& l)
    {
        return l * rc;
    }

    skColor operator/(skScalar v) const
    {
        if (skIsZero(v))
            v = 1;
        return skColor(r / v, g / v, b / v, a);
    }

    skColor operator/(const skColor& v) const
    {
        return skColor(r / v.r, g / v.g, b / v.b, a);
    }

    skColor& operator/=(skScalar v)
    {
        r /= v;
        g /= v;
        b /= v;
        return *this;
    }

    skColor& operator/=(const skColor& v)
    {
        r /= v.r;
        g /= v.g;
        b /= v.b;
        return *this;
    }

    friend skColor operator/(skScalar rc, const skColor& l)
    {
        return l / rc;
    }

    skColor& operator=(const skColor& o) = default;

    skScalar* ptr()
    {
        return &r;
    }

    const skScalar* ptr() const
    {
        return &r;
    }

    void asInt8(SKuint8& vr, SKuint8& vg, SKuint8& vb, SKuint8& va) const;

    void asRGB888(SKuint8& vr, SKuint8& vg, SKuint8& vb) const;

    void print() const;

    skColor& mix(const skColor& rhs, const skScalar t)
    {
        const skScalar iT = 1 - t;

        r = iT * r + t * rhs.r;
        g = iT * g + t * rhs.g;
        b = iT * b + t * rhs.b;

        return *this;
    }
};

SK_INLINE void skLimitRGB(skColor& dest)
{
    // f(x) x {0 <= x <= 1}
    dest.r = dest.r < 0 ? 0 : dest.r > 1 ? 1
                                         : dest.r;
    dest.g = dest.g < 0 ? 0 : dest.g > 1 ? 1
                                         : dest.g;
    dest.b = dest.b < 0 ? 0 : dest.b > 1 ? 1
                                         : dest.b;
}

SK_INLINE void skMixRGB(skColor& dest, const skColor& a, const skColor& b, skScalar t)
{
    // f(a,b,t) (1 - t)a + (t)b {0 <= t <= 1}
    const skScalar iT = 1 - t;

    dest.r = iT * a.r + t * b.r;
    dest.g = iT * a.g + t * b.g;
    dest.b = iT * a.b + t * b.b;
}

SK_INLINE void skMulRGB(skColor& dest, const skColor& a, const skColor& b)
{
    // f(a,b) ab
    dest.r = a.r * b.r;
    dest.g = a.g * b.g;
    dest.b = a.b * b.b;
}

SK_INLINE void skMulRGB(skColor& dest, const skColor& a, const skScalar& b)
{
    // f(a,b) ab
    dest.r = a.r * b;
    dest.g = a.g * b;
    dest.b = a.b * b;
}

SK_INLINE void skMul3RGB(skColor&        dest,
                         const skColor&  a,
                         const skColor&  b,
                         const skScalar& c)
{
    // f(a,b,c) abc
    dest.r = a.r * b.r * c;
    dest.g = a.g * b.g * c;
    dest.b = a.b * b.b * c;
}

SK_INLINE void skAddRGB(skColor& dest, const skColor& a, const skColor& b)
{
    // f(a,b) a+b
    dest.r = a.r + b.r;
    dest.g = a.g + b.g;
    dest.b = a.b + b.b;
}

SK_INLINE void skHalfAddRGB(skColor& dest, const skColor& a, const skColor& b)
{
    // f(a,b) 9*(a+b)/16
    dest.r = (a.r + b.r) * 0.5625f;
    dest.g = (a.g + b.g) * 0.5625f;
    dest.b = (a.b + b.b) * 0.5625f;
}

SK_INLINE void skAdd3RGB(skColor& dest, const skColor& a, const skColor& b, const skColor& c)
{
    // f(a,b,c) a + b + c
    dest.r = a.r + b.r + c.r;
    dest.g = a.g + b.g + c.g;
    dest.b = a.b + b.b + c.b;
}

SK_INLINE void skAdd3RGB(skColor& dest, const skColor& a, const skColor& b, const skScalar& c)
{
    // f(a,b,c) a + b + c
    dest.r = a.r + b.r + c;
    dest.g = a.g + b.g + c;
    dest.b = a.b + b.b + c;
}

SK_INLINE void skSubRGB(skColor& dest, const skColor& a, const skScalar& c)
{
    // f(a,b) a - b
    dest.r = a.r - c;
    dest.g = a.g - c;
    dest.b = a.b - c;
}

SK_INLINE void skAvr3RGB(skColor& dest, const skColor& a, const skColor& b, const skColor& c)
{
    // f(a,b,c) a + b + c
    dest.r = (a.r + b.r + c.r) * .333333f;
    dest.g = (a.g + b.g + c.g) * .333333f;
    dest.b = (a.b + b.b + c.b) * .333333f;
}

#endif  //_skColor_h_
