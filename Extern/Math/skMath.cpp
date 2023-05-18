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
#include "skMath.h"
#include "skMatrix3.h"
#include "skMatrix4.h"
#include "skTransform2D.h"

void skMath::ortho2D(class skTransform2D& dest, skScalar l, skScalar t, skScalar r, skScalar b)
{
    skScalar dx = r - l, dy = b - t;

    if (skIsZero(dx) || skIsZero(dy))
    {
        dest = skTransform2D::Identity;
        return;
    }

    dx   = skScalar(1) / dx;
    dy   = skScalar(1) / dy;
    dest = skTransform2D(
        skScalar(2) * dx,
        0,
        -(r + l) * dx,
        0,
        skScalar(2) * dy,
        -(t + b) * dy,
        0,
        0,
        1);
}

void skMath::ortho2D(class skMatrix4& dest, skScalar l, skScalar t, skScalar r, skScalar b)
{
    skScalar dx = r - l, dy = b - t;

    if (skIsZero(dx) || skIsZero(dy))
    {
        dest = skMatrix4::Identity;
        return;
    }

    dx = skScalar(1) / dx;
    dy = skScalar(1) / dy;

    dest = skMatrix4(
        skScalar(2) * dx,
        0,
        0,
        -(r + l) * dx,
        0,
        skScalar(2) * dy,
        0,
        -(t + b) * dy,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        1);
}

void skMath::projection(skMatrix4& dest,
                        skScalar   fov,
                        skScalar   aspect,
                        skScalar   zNear,
                        skScalar   zFar)
{
    // [S,   0,     0,          0]
    // [0,   S,     0,          0]
    // [0,   0,  -f/(f-N),     -1]
    // [0,   0,  -(f*n)/(f-N),  0]

    const skScalar A   = skScalar(1.0) / skTan(fov * skScalar(0.5));
    const skScalar oFN = skScalar(1.0) / (zFar - zNear);
    if (skIsZero(aspect))
        aspect = 1;

    dest.makeIdentity();
    dest.m[0][0] = A / aspect;
    dest.m[1][1] = A;
    dest.m[2][2] = -zFar * oFN;
    dest.m[2][3] = -(zFar * zNear) * oFN;
    dest.m[3][2] = -skScalar(1);
}

void skMath::view(skMatrix4& dest, const skVector3& pos, const skQuaternion& rot)
{
    skMatrix3 m3;
    m3.fromQuat(rot);
    view(dest, pos, m3);
}

void skMath::view(skMatrix4& dest, const class skVector3& pos, const class skMatrix3& rot)
{
    dest.m[0][0] = rot.m[0][0];
    dest.m[0][1] = rot.m[1][0];
    dest.m[0][2] = rot.m[2][0];
    dest.m[0][3] = -rot.row(0).dot(pos);
    dest.m[1][0] = rot.m[0][1];
    dest.m[1][1] = rot.m[1][1];
    dest.m[1][2] = rot.m[2][1];
    dest.m[1][3] = -rot.row(1).dot(pos);
    dest.m[2][0] = rot.m[0][2];
    dest.m[2][1] = rot.m[1][2];
    dest.m[2][2] = rot.m[2][2];
    dest.m[2][3] = -rot.row(2).dot(pos);
    dest.m[3][0] = dest.m[3][1] = dest.m[3][2] = skScalar(0);
    dest.m[3][3]                               = skScalar(1);
}

skScalar skMath::toMillimeters(const skScalar& deg)
{
    return skScalar(18) / skTan(deg * skPi / skScalar(360.0));
}

skScalar skMath::toFieldOfView(const skScalar& mm)
{
    // https://www.desmos.com/calculator/aed6jmo0fx
    return skScalar(360.0) * skATan(skScalar(18) / mm) / skPi;
}

skScalar skMath::wrap2Pi(skScalar x)
{
    return x - 2 * skFloor(x * skInvPi2) * skPi;
}

int skMath::pow2(int n)
{
    --n;
    n |= n >> 16;
    n |= n >> 8;
    n |= n >> 4;
    n |= n >> 2;
    n |= n >> 1;
    ++n;
    return n;
}

void skMath::sinCos(const skScalar& theta, skScalar& y, skScalar& x)
{
    x = skCos(theta);
    y = skSin(theta);
}

void skMath::forceAlign(skScalar& val, int mod)
{
    if (mod > 0)
    {
        SKsize       wpI = (SKsize)val;
        const SKsize wpR = wpI % SKsize(mod);
        if (wpR != 0)
        {
            wpI += mod - (mod - wpR);
            val = skScalar(wpI);
        }
    }
}
