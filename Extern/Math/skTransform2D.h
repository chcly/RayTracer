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
#ifndef _skTransform2D_h_
#define _skTransform2D_h_

#include "skMath.h"
#include "skRectangle.h"
#include "skVector2.h"

class skTransform2D
{
public:
    skTransform2D() 
    {
        skScalar t[] = {0, 0, 0, 0, 0, 0, 0, 0};
        copy(t);
    }

    skTransform2D(skScalar _xx,  
                  skScalar _xy,
                  skScalar _tx,
                  skScalar _yx,
                  skScalar _yy,
                  skScalar _ty,
                  skScalar _zx,
                  skScalar _zy,
                  skScalar _zz)
    {
        m[0][0] = _xx;
        m[0][1] = _xy;
        m[0][2] = _tx;
        m[1][0] = _yx;
        m[1][1] = _yy;
        m[1][2] = _ty;
        m[2][0] = _zx;
        m[2][1] = _zy;
        m[2][2] = _zz;
    }

    explicit skTransform2D(const skScalar* v)  // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        if (v)
            copy(v);
        else
        {
            skScalar t[] = {0, 0, 0, 0, 0, 0, 0, 0};
            copy(t);
        }
    }

    skTransform2D(const skTransform2D& v)
    {
        copy(v.p);
    }

    SK_INLINE void setTranslation(const skVector2& v)
    {
        m[0][2] = v.x;
        m[1][2] = v.y;
    }

    SK_INLINE void setTranslation(skScalar x, skScalar y)
    {
        m[0][2] = x;
        m[1][2] = y;
    }

    SK_INLINE void setScale(const skVector2& s)
    {
        m[0][0] = s.x;
        m[1][1] = s.y;
    }

    SK_INLINE void setScale(skScalar x, skScalar y)
    {
        m[0][0] = x;
        m[1][1] = y;
    }

    void setTransform(const skRectangle& v)
    {
        m[0][0] = v.width;
        m[0][1] = 0;
        m[0][2] = v.x;
        m[1][0] = 0;
        m[1][1] = v.height;
        m[1][2] = v.y;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
    }

    skTransform2D operator*(const skTransform2D& B) const
    {
        return skTransform2D(
            m[0][0] * B.m[0][0] + m[0][1] * B.m[1][0] + m[0][2] * B.m[2][0],
            m[0][0] * B.m[0][1] + m[0][1] * B.m[1][1] + m[0][2] * B.m[2][1],
            m[0][0] * B.m[0][2] + m[0][1] * B.m[1][2] + m[0][2] * B.m[2][2],

            m[1][0] * B.m[0][0] + m[1][1] * B.m[1][0] + m[1][2] * B.m[2][0],
            m[1][0] * B.m[0][1] + m[1][1] * B.m[1][1] + m[1][2] * B.m[2][1],
            m[1][0] * B.m[0][2] + m[1][1] * B.m[1][2] + m[1][2] * B.m[2][2],

            m[2][0] * B.m[0][0] + m[2][1] * B.m[1][0] + m[2][2] * B.m[2][0],
            m[2][0] * B.m[0][1] + m[2][1] * B.m[1][1] + m[2][2] * B.m[2][1],
            m[2][0] * B.m[0][2] + m[2][1] * B.m[1][2] + m[2][2] * B.m[2][2]);
    }

    SK_INLINE skVector2 operator*(const skVector2& v) const
    {
        return skVector2(m[0][0] * v.x + m[0][1] * v.y + m[0][2], m[1][0] * v.x + m[1][1] * v.y + m[1][2]);
    }

    bool operator==(const skTransform2D& v) const
    {
        return (
            m[0][0] == v.m[0][0] && m[0][1] == v.m[0][1] && m[0][2] == m[0][2] &&
            m[1][0] == v.m[1][0] && m[1][1] == v.m[1][1] && m[1][2] == m[1][2] &&
            m[2][0] == v.m[2][0] && m[2][1] == v.m[2][1] && m[2][2] == m[2][2]);
    }

    union
    {
        skScalar m[3][3];
        skScalar p[9]{};
    };

public:
    static const skTransform2D Identity;
    static const skTransform2D Zero;

private:
    void copy(const skScalar v[9])
    {
        p[0] = v[0];
        p[1] = v[1];
        p[2] = v[2];
        p[3] = v[3];
        p[4] = v[4];
        p[5] = v[5];
        p[6] = v[6];
        p[7] = v[7];
        p[8] = v[8];
    }
};

#endif  //_skTransform2D_h_
