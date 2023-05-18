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
#ifndef _skFoot_h_
#define _skFoot_h_

#include "skMath.h"

class skFoot
{
public:
    int f{}, i{};

public:

    skFoot(const skFoot& ft) :
        f(ft.f),
        i(ft.i)
    {
    }

    skFoot()
    {
    }

    explicit skFoot(const int inches)
    {
        toFeet(inches);
    }

    skFoot(const int foot, const int inches)
    {
        f = foot;
        i = inches;
    }

    void toInches()
    {
        i = f * 12 + i;
        f = 0;
    }

    int getInches() const
    {
        return f * 12 + i;
    }

    void toInches(const int foot, const int inches)
    {
        f = 0;
        i = foot * 12 + inches;
    }

    void toFeet(const int inches)
    {
        f = inches / 12;
        skScalar dp = skScalar(inches) / skScalar(12);
        dp = dp - skScalar(f);
#ifdef SK_DOUBLE
        i = int(skCeil(12 * (double)dp));
#else
        i = static_cast<int>(12 * dp);
#endif
    }

    skScalar toDouble() const
    {
        return skScalar(f) + skScalar(i) / skScalar(12);
    }

    skFoot operator-(const skFoot& rhs) const
    {
        return (*this).sub(rhs);
    }

    skFoot operator+(const skFoot& rhs) const
    {
        return (*this).add(rhs);
    }

    skFoot add(const skFoot& rhs) const
    {
        skFoot lhs = skFoot();
        lhs.toInches(f, i);

        lhs.i = lhs.i + rhs.getInches();
        lhs.toFeet(lhs.i);
        return lhs;
    }

    skFoot sub(const skFoot& rhs) const
    {
        skFoot lhs;
        lhs.toInches(f, i);

        lhs.i = lhs.i - rhs.getInches();
        lhs.toFeet(lhs.i);
        return lhs;
    }

    skFoot& set(const skFoot& rhs)
    {
        f = rhs.f;
        i = rhs.i;
        return *this;
    }
};

#endif  //_skFoot_h_
