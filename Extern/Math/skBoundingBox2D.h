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
#ifndef _skBoundingBox2D_h_
#define _skBoundingBox2D_h_

#include "skMath.h"
#include "skRectangle.h"

class skBoundingBox2D
{
public:
    static const skBoundingBox2D Identity;

public:
    skBoundingBox2D()
    {
        clear();
    }

    skBoundingBox2D(const skRectangle& v)
    {
        v.getBounds(x1, y1, x2, y2);
    }

    skBoundingBox2D(skScalar _x1, skScalar _y1, skScalar _x2, skScalar _y2) :
        x1(_x1),
        y1(_y1),
        x2(_x2),
        y2(_y2)
    {
    }

    skBoundingBox2D(const skBoundingBox2D& obb) = default;

    SK_INLINE void clear()
    {
        *this = Identity;
    }

    SK_INLINE skScalar xLength() const
    {
        return x2 - x1;
    }

    SK_INLINE skScalar yLength() const
    {
        return y2 - y1;
    }

    SK_INLINE skRectangle getRect() const
    {
        return skRectangle(x1, y1, x2 - x1, y2 - y1);
    }

    SK_INLINE void normalize()
    {
        getRect().makeMinMax().getBounds(x1, y1, x2, y2);
    }

    SK_INLINE bool isValid() const
    {
        return skAbs((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < SK_EPSILON;
    }

    void compare(skScalar x, skScalar y);
    void compare(const skRectangle& rct);

    skBoundingBox2D& operator=(const skBoundingBox2D& v);

    skScalar x1{}, y1{}, x2{}, y2{};
};

#endif  //_skBoundingBox2D_h_
