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
#include "skColor.h"
#include <cstdio>
#include "skVector3.h"
#if SK_ENDIAN == SK_ENDIAN_BIG
#define SK_R 0
#define SK_G 1
#define SK_B 2
#define SK_A 3
#else
#define SK_R 3
#define SK_G 2
#define SK_B 1
#define SK_A 0
#endif

const skColor skColor::White = skColor(1, 1, 1, 1);
const skColor skColor::Black = skColor(0, 0, 0, 1);

const skScalar skColorUtils::i100 = skScalar(1.0 / 100.0);
const skScalar skColorUtils::i255 = skScalar(1.0 / 255.0);
const skScalar skColorUtils::i360 = skScalar(1.0 / 360.0);

// constants for rotating
const skScalar i1 = skPiO3;
const skScalar i2 = 2 * skPiO3;
const skScalar i3 = 3 * skPiO3;
const skScalar i4 = 4 * skPiO3;
const skScalar i5 = 5 * skPiO3;

skColor::skColor(const skVector3& v) :
    r(v.x),
    g(v.y),
    b(v.z),
    a(1)
{
}

void skColor::print() const
{
    SKuint8 vr, vg, vb, va;
    asInt8(vr, vg, vb, va);
    printf("#%02X%02X%02X%02X\n", vr, vg, vb, va);
}

void skColor::asInt8(SKuint8& vr, SKuint8& vg, SKuint8& vb, SKuint8& va) const
{
    vr = SKuint8(r * skScalar(255));
    vg = SKuint8(g * skScalar(255));
    vb = SKuint8(b * skScalar(255));
    va = SKuint8(a * skScalar(255));
}

void skColor::asRGB888(SKuint8& vr, SKuint8& vg, SKuint8& vb) const
{
    vr = SKuint8(r * skScalar(255));
    vg = SKuint8(g * skScalar(255));
    vb = SKuint8(b * skScalar(255));
}

void skColorUtils::convert(skColori& dst, const skColor& src)
{
    skColorU color{};
    color.b[3] = (unsigned char)(src.r * 255);
    color.b[2] = (unsigned char)(src.g * 255);
    color.b[1] = (unsigned char)(src.b * 255);
    color.b[0] = (unsigned char)(src.a * 255);
    dst        = color.i;
}

void skColorUtils::convert(skColor& dst, const skColori& src)
{
    skColorU color{};
    color.i = src;
    dst.r   = (skScalar)color.b[SK_R] * i255;
    dst.g   = (skScalar)color.b[SK_G] * i255;
    dst.b   = (skScalar)color.b[SK_B] * i255;
    dst.a   = (skScalar)color.b[SK_A] * i255;
}


void skColorUtils::convert(skColori& dst, const SKubyte* src)
{
    skColorU color{};
    color.b[SK_R] = src[SK_R];
    color.b[SK_G] = src[SK_G];
    color.b[SK_B] = src[SK_B];
    color.b[SK_A] = src[SK_A];
    dst           = color.i;
}

void skColorUtils::convert(skColor& dst, const skColorHSV& src)
{
    // https://en.wikipedia.org/w/index.php?title=HSL_and_HSV&oldid=941280606

    const skScalar h = src.h / skScalar(60.0);

    skScalar c = src.v * src.s;
    if (c > skScalar(1.0))
        c = skScalar(1.0);

    const skScalar x = c * (skScalar(1.0) - skAbs(skFmod(h, skScalar(2.0)) - skScalar(1.0)));
    skScalar       m = src.v - c;
    if (m > skScalar(1.0))
        m = skScalar(1.0);
    if (m < skScalar(0.0))
        m = skScalar(0.0);

    if (h >= skScalar(0.0) && h <= i1)
    {
        dst.r = c;
        dst.g = x;
        dst.b = 0;
    }
    else if (h >= i1 && h <= i2)
    {
        dst.r = x;
        dst.g = c;
        dst.b = 0;
    }
    else if (h >= i2 && h <= i3)
    {
        dst.r = 0;
        dst.g = c;
        dst.b = x;
    }
    else if (h >= i3 && h <= i4)
    {
        dst.r = 0;
        dst.g = x;
        dst.b = c;
    }
    else if (h >= i4 && h <= i5)
    {
        dst.r = x;
        dst.g = 0;
        dst.b = c;
    }
    else
    {
        dst.r = c;
        dst.g = 0;
        dst.b = x;
    }

    dst.r += m;
    dst.g += m;
    dst.b += m;

    dst.limit();
    dst.a = src.a;
}

void skColorUtils::convert(SKubyte*& dst, const skColor& src)
{
    dst[SK_R] = (unsigned char)(src.r * 255.99f);
    dst[SK_G] = (unsigned char)(src.g * 255.99f);
    dst[SK_B] = (unsigned char)(src.b * 255.99f);
    dst[SK_A] = (unsigned char)(src.a * 255.99f);
}

void skColorUtils::convert(skColor& dst, const SKubyte* src)
{
    dst.r = (skScalar)src[SK_R] * i255;
    dst.g = (skScalar)src[SK_G] * i255;
    dst.b = (skScalar)src[SK_B] * i255;
    dst.a = (skScalar)src[SK_A] * i255;
}

void skColorUtils::convert(SKubyte*& dst, const skScalar& src)
{
    skColorUF uf{};
    uf.f      = src;
    dst[SK_R] = uf.b[SK_R];
    dst[SK_G] = uf.b[SK_G];
    dst[SK_B] = uf.b[SK_B];
    dst[SK_A] = uf.b[SK_A];
}

void skColorUtils::convert(SKubyte*& dst, const SKuint32& src)
{
    skColorU uf{};
    uf.i      = src;
    dst[SK_R] = uf.b[SK_R];
    dst[SK_G] = uf.b[SK_G];
    dst[SK_B] = uf.b[SK_B];
    dst[SK_A] = uf.b[SK_A];
}

void skColorUtils::convert(skColor& dst, const skScalar& src)
{
    skColorUF uf{};
    uf.f = src;

    dst.r = (skScalar)uf.b[SK_R] * i255;
    dst.g = (skScalar)uf.b[SK_G] * i255;
    dst.b = (skScalar)uf.b[SK_B] * i255;
    dst.a = (skScalar)uf.b[SK_A] * i255;
}

void skColorUtils::convert(skColorHSV& dst, const skColor& src)
{
    dst.v = skMax3<skScalar>(src.r, src.g, src.b);
    dst.a = dst.v - skMin3<skScalar>(src.r, src.g, src.b);

    if (dst.a <= skScalar(0.0))
        dst.h = skScalar(0.0);
    else if (skEq(src.r, dst.v))
        dst.h = skPiO3 * ((src.g - src.b) / dst.a);
    else if (skEq(src.g, dst.v))
        dst.h = skPiO3 * (skScalar(2) + (src.b - src.r) / dst.a);
    else
        dst.h = skPiO3 * (skScalar(4) + (src.r - src.g) / dst.a);

    dst.h *= skDPR;
    dst.h = skCeil(dst.h);
    dst.s = dst.v;
    if (dst.v > 0)
        dst.s = dst.a / dst.v;

    dst.a = src.a;
}
