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
#ifndef _skRational_h_
#define _skRational_h_

#include "Math/skMath.h"

class skRational
{
public:
    static const skRational Zero;
    int                     n, d;

public:
    skRational();
    skRational(int n, int d);
    skRational(const skRational& oth);

    skRational operator-() const;

    explicit operator skScalar() const
    {
        if (d == 0)
            return (skScalar)(SK_INFINITY * 0);
        return (skScalar)n / (skScalar)d;
    }

    static int gcd(int a, int b);

    void reduce();
    void print() const;
};

skRational operator-(const skRational& a, const skRational& b);
skRational operator+(const skRational& a, const skRational& b);
skRational operator*(const skRational& a, const skRational& b);
skRational operator/(const skRational& a, const skRational& b);

#endif  //_Rational_h_
