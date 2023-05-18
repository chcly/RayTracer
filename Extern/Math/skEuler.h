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
#ifndef _skEuler_h_
#define _skEuler_h_

#include "skMath.h"

class skEuler
{
public:
    skScalar pitch;
    skScalar yaw;
    skScalar roll;

public:

    skEuler()
    {
        pitch = 0;
        yaw = 0;
        roll = 0;
    }

    skEuler(skScalar p, skScalar y, skScalar r)
    {
        pitch = p;
        yaw = y;
        roll = r;
    }

    skEuler(const skEuler& v)
    {
        pitch = v.pitch;
        yaw = v.yaw;
        roll = v.roll;
    }

    void makeIdentity()
    {
        pitch = 0.f;
        yaw = 0.f;
        roll = 0.f;
    }

    void normalize()
    {
        pitch = skMath::wrap2Pi(pitch);
        if (pitch < -skPiH)
        {
            pitch = -skPi - pitch;
            yaw += skPi;
            roll += skPi;
        }
        else if (pitch > skPiH)
        {
            pitch = skPi - pitch;
            yaw += skPi;
            roll += skPi;
        }

        if (skAbs(pitch) > skPiH - SK_EPSILON)
        {
            yaw += roll;
            roll = 0.f;
        }
        else
            roll = skMath::wrap2Pi(roll);

        yaw = skMath::wrap2Pi(yaw);
    }

    void print() const;
};

#endif//_skEuler_h_
