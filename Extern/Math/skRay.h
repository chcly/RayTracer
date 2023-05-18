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
#ifndef _skRay_h_
#define _skRay_h_

#include "Math/skMath.h"
#include "Math/skVector3.h"

class skRay;

struct skRayHitTest
{
    skRayHitTest() :
        distance(0)
    {
    }

    skRayHitTest(const skRayHitTest& rhs) = default;

    skScalar  distance;
    skVector3 point;
    skVector3 normal;
};


/// <summary>
/// R = O + tD 
/// </summary>
class skRay
{
public:
    skVector3 origin;
    skVector3 direction;

public:
    skRay() = default;

    skRay(const skVector3& vOrig, const skVector3& vDir) :
        origin(vOrig),
        direction(vDir)
    {
    }

    SK_INLINE skVector3 at(const skScalar& t) const
    {
        return {
            origin.x + direction.x * t,
            origin.y + direction.y * t,
            origin.z + direction.z * t,
        };
    }

    SK_INLINE void at(skVector3& dest, const skScalar& t) const
    {
        dest.x = origin.x + direction.x * t;
        dest.y = origin.y + direction.y * t;
        dest.z = origin.z + direction.z * t;
    }


    void print() const;
};

#endif  //_skRay_h_
