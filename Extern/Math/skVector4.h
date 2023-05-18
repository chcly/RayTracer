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
#ifndef _skVector4_h_
#define _skVector4_h_

#include "skMath.h"

class skVector4
{
public:
    skVector4() :
        x(0),
        y(0),
        z(0),
        w(0)
    {
    }

    skVector4(const skScalar nx, const skScalar ny, const skScalar nz, const skScalar nw) :
        x(nx),
        y(ny),
        z(nz),
        w(nw)
    {
    }

    explicit skVector4(const skScalar* p) :
        x(p[0]),
        y(p[1]),
        z(p[2]),
        w(p[3])
    {
    }

    skVector4(const skVector4& v) = default;

    SK_INLINE skScalar* ptr()
    {
        return &x;
    }

    SK_INLINE const skScalar* ptr() const
    {
        return &x;
    }

    SK_INLINE bool operator==(const skVector4& v) const
    {
        return skEq(x, v.x) && skEq(y, v.y) && skEq(z, v.z) && skEq(w, v.w);
    }

    SK_INLINE bool operator!=(const skVector4& v) const
    {
        return !skEq(x, v.x) && !skEq(y, v.y) && !skEq(z, v.z) && !skEq(w, v.w);
    }

    void print() const;

    skScalar x, y, z, w;

public:
    static const skVector4 Unit;
    static const skVector4 Zero;
};

#endif  //_skVector4_h_
