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
#ifndef _skRectangle_h_
#define _skRectangle_h_

#include "skMath.h"
#include "skVector2.h"
#ifdef min
#undef min
#endif  //min
#ifdef max
#undef max
#endif  //min

class skRectangle
{
public:
    skScalar x, y;
    skScalar width, height;

public:
    skRectangle() :
        x(0),
        y(0),
        width(0),
        height(0)
    {
    }

    skRectangle(skScalar xv, skScalar yv, skScalar wv, skScalar hv) :
        x(xv),
        y(yv),
        width(wv),
        height(hv)
    {
    }

    explicit skRectangle(skScalar* ptr) :
        x(ptr[0]),
        y(ptr[1]),
        width(ptr[2]),
        height(ptr[3])
    {
    }

    skRectangle& operator=(const skRectangle& o) = default;

    skVector2 getSize() const
    {
        return skVector2(width, height);
    }

    skVector2 getPosition() const
    {
        return skVector2(x, y);
    }

    SK_INLINE skScalar hw() const
    {
        return width * skScalar(.5);
    }

    SK_INLINE skScalar hh() const
    {
        return height * skScalar(.5);
    }

    skScalar getWidth() const
    {
        return width;
    }

    SK_INLINE skScalar getHeight() const
    {
        return height;
    }

    SK_INLINE skScalar getLeft() const
    {
        return x;
    }

    SK_INLINE skScalar getRight() const
    {
        return x + width;
    }

    SK_INLINE skScalar getTop() const
    {
        return y;
    }

    SK_INLINE skScalar getBottom() const
    {
        return y + height;
    }

    void setSize(const skVector2& s)
    {
        width  = s.x;
        height = s.y;
    }

    void setSize(skScalar sx, skScalar sy)
    {
        width  = sx;
        height = sy;
    }

    void setPosition(const skVector2& p)
    {
        x = p.x;
        y = p.y;
    }

    void setPosition(skScalar px, skScalar py)
    {
        x = px;
        y = py;
    }

    skVector2 getLeftTop() const
    {
        return skVector2(x, y);
    }

    skVector2 getRightTop() const
    {
        return skVector2(x + width, y);
    }

    skVector2 getLeftBottom() const
    {
        return skVector2(x, y + height);
    }

    skVector2 getRightBottom() const
    {
        return skVector2(x + width, y + height);
    }

    skVector2 getCenter() const
    {
        return skVector2(x + width / skScalar(2),
                         y + height / skScalar(2));
    }

    void getCenter(skScalar& cx, skScalar& cy) const
    {
        cx = x + width / skScalar(2);
        cy = y + height / skScalar(2);
    }

    void getCorners(skVector2& lt, skVector2& rt, skVector2& lb, skVector2& rb) const
    {
        lt = getLeftTop();
        rt = getRightTop();
        lb = getLeftBottom();
        rb = getRightBottom();
    }

    void offset(const skVector2& v)
    {
        x += v.x;
        y += v.y;
    }

    void offset(skScalar vx, skScalar vy)
    {
        x += vx;
        y += vy;
    }

    void scale(skScalar vx, skScalar vy)
    {
        skScalar r = x + width;
        skScalar b = y + height;
        x *= vx;
        y *= vy;
        r *= vx;
        b *= vy;
        width  = r - x;
        height = b - y;
    }

    void expand(skScalar vx, skScalar vy)
    {
        skScalar r = x + width;
        skScalar b = y + height;
        x -= vx;
        y -= vy;
        r += vx;
        b += vy;
        width  = r - x;
        height = b - y;
    }

    void move(skScalar vx, skScalar vy)
    {
        x += vx;
        y += vy;
    }

    void expand(const skVector2& v)
    {
        expand(v.x, v.y);
    }

    void contract(const skVector2& v)
    {
        expand(-v.x, -v.y);
    }

    void contract(skScalar vx, skScalar vy)
    {
        expand(-vx, -vy);
    }

    SK_INLINE bool contains(skScalar vx, skScalar vy) const
    {
        return vx >= x && vy >= y && vx - x <= width && vy - y <= height;
    }

    SK_INLINE bool containsX(skScalar vx) const
    {
        return vx >= x && vx - x <= width;
    }

    SK_INLINE bool containsY(skScalar vy) const
    {
        return vy >= y && vy - y <= height;
    }

    bool contains(const skVector2& v) const
    {
        return contains(v.x, v.y);
    }

    bool contains(const skRectangle& rect) const;

    bool clipped(skScalar tx, skScalar ty, skScalar tw, skScalar th) const;
    bool clipped(skScalar tx, skScalar ty, skScalar tw, skScalar th, const int dir) const;

    bool clipped(const skRectangle& rct) const
    {
        return clipped(rct.x, rct.y, rct.width, rct.height);
    }

    void getBounds(skScalar& x1, skScalar& y1, skScalar& x2, skScalar& y2) const
    {
        x1 = x;
        y1 = y;
        x2 = x + width;
        y2 = y + height;
    }

    void getLocalBounds(skScalar& x1, skScalar& y1, skScalar& x2, skScalar& y2) const
    {
        x1 = x;
        y1 = y;
        x2 = width;
        y2 = height;
    }

    void getAbsBounds(skScalar& x1, skScalar& y1, skScalar& x2, skScalar& y2) const
    {
        x1 = skAbs(x);
        y1 = skAbs(y);
        x2 = x1 + width;
        y2 = y1 + height;
    }

    void setCorners(const skScalar& x1, const skScalar& y1, const skScalar& x2, const skScalar& y2)
    {
        x = x1;
        y = y1;

        width  = x2 - x1;
        height = y2 - y1;
    }

    void setAbsoluteCorners(const skScalar& x1, const skScalar& y1, const skScalar& x2, const skScalar& y2);

    SK_INLINE skScalar getAspect() const
    {
        return width / height;
    }

    skScalar cx() const
    {
        return x + width * skScalar(0.5);
    }

    skScalar cy() const
    {
        return y + height * skScalar(0.5);
    }

    skVector2 center() const
    {
        return skVector2(cx(), cy());
    }

    skRectangle& makeMinMax();

    skRectangle makeMinMaxCopy() const
    {
        skRectangle r = *this;
        r.makeMinMax();
        return r;
    }

    skRectangle min(const skRectangle& oth) const
    {
        skRectangle a = *this;
        return a.min(oth);
    }

    skRectangle& center(const skRectangle& oth)
    {
        if (!skIsZero(oth.width) && !skIsZero(oth.height))
            setPosition(getPosition() + oth.center());
        return *this;
    }

    skRectangle center(const skRectangle& oth) const
    {
        skRectangle a = *this;
        return a.center(oth);
    }

    skRectangle max(const skRectangle& oth) const
    {
        skRectangle a = *this;
        return a.max(oth);
    }

    skRectangle& max(const skRectangle& oth);
    skRectangle& min(const skRectangle& oth);
    skRectangle& normalize();

    SK_INLINE bool operator==(const skRectangle& rhs) const
    {
        return skEq(x, rhs.x) &&
               skEq(y, rhs.y) &&
               skEq(width, rhs.width) &&
               skEq(height, rhs.height);
    }

    SK_INLINE bool operator!=(const skRectangle& rhs) const
    {
        return !(skEq(x, rhs.x) && skEq(y, rhs.y) && skEq(width, rhs.width) && skEq(height, rhs.height));
    }

    void print() const;
};

#endif  //_skRectangle_h_
