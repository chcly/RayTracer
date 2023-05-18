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
#ifndef _rtColor_h_
#define _rtColor_h_

#include "Math/skColor.h"
#include "Math/skScalar.h"
#include "RenderSystem/Math/rtVectorTypes.h"
#include "Utils/Config/skConfig.h"

#ifdef RT_USE_SIMD

SK_INLINE void rtColorSet(rtScalar4& dest, skScalar r, skScalar g, skScalar b, skScalar a)
{
    dest = _mm_set_ps(a, b, g, r);
}

SK_INLINE void rtColorInit(rtScalar4& dest)
{
    dest = _mm_xor_ps(dest, dest);
}

SK_INLINE static rtScalar4 rtColorConst(const rtScalar constVal)
{
    return _mm_set1_ps(constVal);
}

const rtScalar4 rtByte255  = rtColorConst(255);
const rtScalar4 rtIByte255 = rtColorConst(1.f / 255.f);
const rtScalar4 rtColor1   = rtColorConst(1);

class rtColor
{
public:
    rtScalar4 rgba;

public:
    rtColor() :
        rgba(_mm_xor_ps(rgba, rgba))
    {
    }

    explicit rtColor(const skScalar& c) :
        rgba(_mm_set1_ps(c))
    {
    }

    explicit rtColor(const skColor& c) :
        rgba(_mm_set_ps(c.a, c.b, c.g, c.r))
    {
    }

    explicit rtColor(const rtVector3& c) :
        rgba(_mm_set_ps(1, c.z, c.y, c.x))
    {
    }

    ~rtColor() = default;

    SK_INLINE void setF(const skScalar& r,
                        const skScalar& g,
                        const skScalar& b,
                        const skScalar& a = skScalar(1))
    {
        rgba = _mm_set_ps(a, b, g, r);
    }

    SK_INLINE void setF(const skScalar& c)
    {
        rgba = _mm_set1_ps(c);
    }

    SK_INLINE void zero()
    {
        rgba = _mm_xor_ps(rgba, rgba);
    }

    SK_INLINE void setUb(const SKubyte& r,
                         const SKubyte& g,
                         const SKubyte& b,
                         const SKubyte& a = 255)
    {
        rgba = _mm_set_ps(a, b, g, r);
        rgba = _mm_mul_ps(rgba, rtIByte255);
    }

    SK_INLINE void toBytes(SKubyte& r,
                           SKubyte& g,
                           SKubyte& b,
                           SKubyte& a) const
    {
        const rtScalar4 c = _mm_mul_ps(rgba, rtByte255);
        a                 = (SKubyte)RTfx(c);
        b                 = (SKubyte)RTfy(c);
        g                 = (SKubyte)RTfz(c);
        r                 = (SKubyte)RTfw(c);
    }

    SK_INLINE void toBytes(SKubyte& r,
                           SKubyte& g,
                           SKubyte& b) const
    {
        const rtScalar4 c = _mm_mul_ps(rgba, rtByte255);
        b                 = (SKubyte)RTfy(c);
        g                 = (SKubyte)RTfz(c);
        r                 = (SKubyte)RTfw(c);
    }

    SK_INLINE void toBytes(rtScalar4& c) const
    {
        c = _mm_mul_ps(rgba, rtByte255);
    }

    SK_INLINE void mul(const rtColor& c)
    {
        rgba = _mm_mul_ps(rgba, c.rgba);
    }

    SK_INLINE void add(const rtColor& c)
    {
        rgba = _mm_add_ps(rgba, c.rgba);
    }

    SK_INLINE void add(const skScalar& c)
    {
        rgba = _mm_add_ps(rgba, rtColorConst(c));
    }

    SK_INLINE void mul(const rtColor& c, const skScalar& d)
    {
        const rtScalar4 mr = _mm_mul_ps(c.rgba, rtColorConst(d));
        rgba               = _mm_mul_ps(rgba, mr);
    }

    SK_INLINE void inv(const rtColor& c, const skScalar& d)
    {
        rgba = _mm_sub_ps(rtColorConst(d), c.rgba);
    }

    SK_INLINE void mul(const skScalar& d)
    {
        rgba = _mm_mul_ps(rgba, rtColorConst(d));
    }

    SK_INLINE const skScalar& r() const
    {
        return RTfw(rgba);
    }

    SK_INLINE const skScalar& g() const
    {
        return RTfz(rgba);
    }

    SK_INLINE const skScalar& b() const
    {
        return RTfy(rgba);
    }

    SK_INLINE const skScalar& a() const
    {
        return RTfx(rgba);
    }

    void mix(const rtColor& a, const rtColor& b, skScalar t)
    {
        rtScalar4 st = _mm_set1_ps(t);
        rtScalar4 en = _mm_set1_ps(1 - t);
        st           = _mm_mul_ps(a.rgba, st);
        en           = _mm_mul_ps(b.rgba, en);
        rgba         = _mm_add_ps(st, en);
    }

    void set(const rtColor& color)
    {
        rgba = color.rgba;
    }

    void set(const skScalar& color)
    {
        rgba = _mm_set1_ps(color);
    }

    void setLimit(const rtColor& color, skScalar mi, skScalar ma)
    {
        setF(
            skClampf(color.r(), mi, ma),
            skClampf(color.g(), mi, ma),
            skClampf(color.b(), mi, ma),
            skClampf(color.a(), mi, ma));
    }

    void saturate()
    {
        setF(skClampf(r(), 0, 1.f),
             skClampf(g(), 0, 1.f),
             skClampf(b(), 0, 1.f),
             skClampf(a(), 0, 1.f));
    }

    void invert()
    {
        rgba = _mm_sub_ps(rtColor1, rgba);
    }
};

#else

class rtColor
{
public:
    rtScalar _col[4];

public:
    rtColor() :
        _col{0, 0, 0, 0}
    {
    }

    explicit rtColor(const skScalar& c) :
        _col{c, c, c, c}
    {
    }

    explicit rtColor(const skColor& c) :
        _col{c.r, c.g, c.b, c.a}
    {
    }

    explicit rtColor(const rtVector3& c) :
        _col{c.x, c.y, c.z, 1}
    {
    }

    ~rtColor() = default;

    SK_INLINE void setF(const skScalar& r,
                        const skScalar& g,
                        const skScalar& b,
                        const skScalar& a = skScalar(1))
    {
        _col[0] = r;
        _col[1] = g;
        _col[2] = b;
        _col[3] = a;
    }

    SK_INLINE void setF(const skScalar& c)
    {
        _col[0] = c;
        _col[1] = c;
        _col[2] = c;
        _col[3] = c;
    }

    SK_INLINE void zero()
    {
        _col[0] = 0;
        _col[1] = 0;
        _col[2] = 0;
        _col[3] = 0;
    }

    SK_INLINE void setUb(const SKubyte& r,
                         const SKubyte& g,
                         const SKubyte& b,
                         const SKubyte& a = 255)
    {
        _col[0] = rtScalar(r) * skColorUtils::i255;
        _col[1] = rtScalar(g) * skColorUtils::i255;
        _col[2] = rtScalar(b) * skColorUtils::i255;
        _col[3] = rtScalar(a) * skColorUtils::i255;
    }

    SK_INLINE void toBytes(SKubyte& r,
                           SKubyte& g,
                           SKubyte& b,
                           SKubyte& a) const
    {
        r = (SKubyte)(rtScalar(_col[0]) * 255.0f);
        g = (SKubyte)(rtScalar(_col[1]) * 255.0f);
        b = (SKubyte)(rtScalar(_col[2]) * 255.0f);
        a = (SKubyte)(rtScalar(_col[3]) * 255.0f);
    }

    SK_INLINE void toBytes(SKubyte& r,
                           SKubyte& g,
                           SKubyte& b) const
    {
        r = (SKubyte)(rtScalar(_col[0]) * 255.0f);
        g = (SKubyte)(rtScalar(_col[1]) * 255.0f);
        b = (SKubyte)(rtScalar(_col[2]) * 255.0f);
    }

    SK_INLINE void mul(const rtColor& c)
    {
        _col[0] *= c._col[0];
        _col[1] *= c._col[1];
        _col[2] *= c._col[2];
        _col[3] *= c._col[3];
    }

    SK_INLINE void mul(const skScalar& d)
    {
        _col[0] *= d;
        _col[1] *= d;
        _col[2] *= d;
        _col[3] *= d;
    }

    SK_INLINE void add(const rtColor& c)
    {
        _col[0] += c._col[0];
        _col[1] += c._col[1];
        _col[2] += c._col[2];
        _col[3] += c._col[3];
    }

    SK_INLINE void add(const skScalar& c)
    {
        _col[0] += c;
        _col[1] += c;
        _col[2] += c;
        _col[3] += c;
    }

    SK_INLINE void mul(const rtColor& c, const skScalar& d)
    {
        _col[0] = c._col[0] * d;
        _col[1] = c._col[1] * d;
        _col[2] = c._col[2] * d;
        _col[3] = c._col[3] * d;
    }

    SK_INLINE void inv(const rtColor& c, const skScalar& d)
    {
        _col[0] = d - c._col[0];
        _col[1] = d - c._col[1];
        _col[2] = d - c._col[2];
        _col[3] = d - c._col[3];
    }

    SK_INLINE const skScalar& r() const
    {
        return _col[0];
    }

    SK_INLINE const skScalar& g() const
    {
        return _col[1];
    }

    SK_INLINE const skScalar& b() const
    {
        return _col[2];
    }

    SK_INLINE const skScalar& a() const
    {
        return _col[3];
    }

    void mix(const rtColor& a, const rtColor& b, skScalar t)
    {
        const rtScalar iT = 1.f - t;

        _col[0] = a._col[0] * t + b._col[0] * iT;
        _col[1] = a._col[1] * t + b._col[1] * iT;
        _col[2] = a._col[2] * t + b._col[2] * iT;
        _col[3] = a._col[3] * t + b._col[3] * iT;
    }

    void set(const rtColor& color)
    {
        _col[0] = color._col[0];
        _col[1] = color._col[1];
        _col[2] = color._col[2];
        _col[3] = color._col[3];
    }

    void set(const skScalar& color)
    {
        _col[0] = color;
        _col[1] = color;
        _col[2] = color;
        _col[3] = color;
    }

    void setLimit(const rtColor& color, skScalar mi, skScalar ma)
    {
        setF(
            skClampf(color.r(), mi, ma),
            skClampf(color.g(), mi, ma),
            skClampf(color.b(), mi, ma),
            skClampf(color.a(), mi, ma));
    }

    void saturate()
    {
        setF(skClampf(r(), 0, 1.f),
             skClampf(g(), 0, 1.f),
             skClampf(b(), 0, 1.f),
             skClampf(a(), 0, 1.f));
    }

    void invert()
    {
        inv(*this, 1.f);
    }
};
#endif
#endif  //_rtColor_h_
