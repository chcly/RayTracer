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
#include "skRational.h"
#include <cstdio>

const skRational skRational::Zero = skRational();

skRational::skRational() :
    n(0),
    d(1)
{
}

skRational::skRational(int rn, int rd) :
    n(rn),
    d(rd)
{
    if (d == 0)
    {
        n = Zero.n;
        d = Zero.d;
    }
    else
        reduce();
}

skRational::skRational(const skRational& oth) :
    n(oth.n),
    d(oth.d)
{
    if (d == 0)
    {
        n = Zero.n;
        d = Zero.d;
    }
    else
        reduce();
}

int skRational::gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

void skRational::reduce()
{
    if (d == 0)
    {
        // Math Error: division by zero.
        n = 0;
        d = 1;
    }

    int g = gcd(n < 0 ? -n : n, d < 0 ? -d : d);
    n /= g;
    d /= g;

    if (d < 0)
    {
        n *= -1;
        d *= -1;
    }
    if (n == 0)
        d = 1;
    if (n == d)
        d = 1;
}

skRational skRational::operator-() const
{
    return skRational(-n, d);
}

void skRational::print() const
{
    if (d == 1)
        printf("%d\n", n);
    else
        printf("%d/%d\n", n, d);
}

skRational operator+(const skRational& a, const skRational& b)
{
    skRational r;
    if (a.d == b.d)
    {
        r.d = a.d;
        r.n = a.n + b.n;
    }
    else
    {
        int na, nb, cd;
        na  = a.n * b.d;
        nb  = b.n * a.d;
        cd  = b.d * a.d;
        r.n = na + nb;
        r.d = cd;
    }
    r.reduce();
    return r;
}

skRational operator-(const skRational& a, const skRational& b)
{
    return skRational(a + (-b));
}

skRational operator*(const skRational& a, const skRational& b)
{
    return skRational(a.n * b.n, a.d * b.d);
}

skRational operator/(const skRational& a, const skRational& b)
{
    if (b.n == 0)
    {
        //DivisionByZero(a, b, std::cout);
        return skRational::Zero;
    }
    const skRational t(b.d, b.n);
    return a * t;
}
