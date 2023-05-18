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
#include "skRectangle.h"
#include <cstdio>

void skRectangle::print() const
{
    printf("[%3.3f, %3.3f, %3.3f, %3.3f]\n", (double)x, (double)y, (double)width, (double)height);
}

skRectangle& skRectangle::normalize()
{
    if (!skIsZero(width) && !skIsZero(height))
    {
        width  = skScalar(1) / width;
        height = skScalar(1) / height;
        x *= width;
        y *= height;
        makeMinMax();
    }
    return *this;
}

skRectangle& skRectangle::max(const skRectangle& oth)
{
    if (!skIsZero(width) && !skIsZero(height))
    {
        skScalar ax1, ay1, ax2, ay2;
        skScalar bx1, by1, bx2, by2;

        getAbsBounds(ax1, ay1, ax2, ay2);
        oth.getAbsBounds(bx1, by1, bx2, by2);
        const skScalar x1 = skMax(ax1, bx1);
        const skScalar y1 = skMax(ay1, by1);
        const skScalar x2 = skMax(ax2, bx2);
        const skScalar y2 = skMax(ay2, by2);

        x = x < 0 ? -x1 : x1;
        y = y < 0 ? -y1 : y1;

        width  = x2 - x1;
        height = y2 - y1;
    }

    return *this;
}

skRectangle& skRectangle::min(const skRectangle& oth)
{
    if (!skIsZero(width) && !skIsZero(height))
    {
        skScalar ax1, ay1, ax2, ay2;
        skScalar bx1, by1, bx2, by2;

        getAbsBounds(ax1, ay1, ax2, ay2);
        oth.getAbsBounds(bx1, by1, bx2, by2);
        const skScalar x1 = skMin(ax1, bx1);
        const skScalar y1 = skMin(ay1, by1);
        const skScalar x2 = skMin(ax2, bx2);
        const skScalar y2 = skMin(ay2, by2);

        x = x < 0 ? -x1 : x1;
        y = y < 0 ? -y1 : y1;

        width  = x2 - x1;
        height = y2 - y1;
    }

    return *this;
}

skRectangle& skRectangle::makeMinMax()
{
    const skScalar x1 = getLeft();
    const skScalar x2 = getRight();
    const skScalar y1 = getTop();
    const skScalar y2 = getBottom();

    x = skMin(x1, x2);
    y = skMin(y1, y2);

    width  = skMax(x1, x2) - skMin(x1, x2);
    height = skMax(y1, y2) - skMin(y1, y2);
    return *this;
}

bool skRectangle::contains(const skRectangle& rect) const
{
    if (contains(rect.getLeftTop()))
        return true;
    if (contains(rect.getRightTop()))
        return true;
    if (contains(rect.getRightBottom()))
        return true;
    if (contains(rect.getLeftBottom()))
        return true;
    if (contains(rect.center()))
        return true;
    return false;
}

bool skRectangle::clipped(skScalar tx, skScalar ty, skScalar tw, skScalar th) const
{
    if (tx > x + width)
        return true;
    if (ty > y + height)
        return true;
    if (tx + tw < x)
        return true;
    if (ty + th < y)
        return true;
    return false;
}

bool skRectangle::clipped(skScalar tx, skScalar ty, skScalar tw, skScalar th, const int dir) const
{
    switch (dir)
    {
    case 0:  // left

        if (tx + tw < x)
            return true;

        break;
    case 1:  // bottom

        if (ty > y + height)
            return true;

        break;
    case 2:  // right

        if (tx > x + width)
            return true;

        break;
    case 3:  // top

        if (ty + th < y)
            return true;

        break;
    default:
        break;
    }

    return false;
}

void skRectangle::setAbsoluteCorners(const skScalar& x1,
                                     const skScalar& y1,
                                     const skScalar& x2,
                                     const skScalar& y2)
{
    if (x1 > x2)
    {
        x     = x2;
        width = x1 - x2;
    }
    else
    {
        x     = x1;
        width = x2 - x1;
    }

    if (y1 > y2)
    {
        y     = y2;
        width = y1 - y2;
    }
    else
    {
        y      = y1;
        height = y2 - y1;
    }
}
