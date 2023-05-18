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
#include "skBoundingBox2D.h"

const skBoundingBox2D skBoundingBox2D::Identity = skBoundingBox2D(SK_INFINITY, SK_INFINITY, -SK_INFINITY, -SK_INFINITY);

void skBoundingBox2D::compare(skScalar x, skScalar y)
{
    if (x < x1) x1 = x;
    if (x > x2) x2 = x;
    if (y < y1) y1 = y;
    if (y > y2) y2 = y;
}

void skBoundingBox2D::compare(const skRectangle& rct)
{
    skScalar _x1, _y1, _x2, _y2;
    rct.getBounds(_x1, _y1, _x2, _y2);

    if (_x1 < x1) x1 = _x1;
    if (_x2 > x2) x2 = _x2;
    if (_y1 < y1) y1 = _y1;
    if (_y2 > y2) y2 = _y2;
}

skBoundingBox2D& skBoundingBox2D::operator = (const skBoundingBox2D& v)
{
    x1 = v.x1; y1 = v.y1;
    x2 = v.x2; y2 = v.y2;
    return *this;
}
