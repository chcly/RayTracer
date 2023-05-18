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
#ifndef _skPlane_h_
#define _skPlane_h_

#include "Math/skMath.h"
#include "Math/skRay.h"
#include "Math/skVector2.h"
#include "Math/skVector3.h"

/// <summary>
/// p*n = d
/// a*x + b*y + c*z = d
/// n = [a,b,c]
/// </summary>
class skPlane
{
public:
    skVector3 p0;
    skVector3 n;

public:
    skPlane()                 = default;
    skPlane(const skPlane& n) = default;

    explicit skPlane(const skVector3& point, const skVector3& normal) :
        p0(point),
        n(normal)
    {
    }


    bool hit(skScalar& t, const skRay& ray, const skVector2& limit) const
    {
        skScalar kn = n.x * (ray.origin.x - p0.x);
        skScalar kd = ray.direction.x * n.x;

        kn += n.y * (ray.origin.y - p0.y);
        kd += ray.direction.y * n.y;
        kn += -n.z * (ray.origin.z - p0.z);
        kd += ray.direction.z * -n.z;

        // > 0 in < 0 out == 0 on

        if (!skIsZero(kd)) 
        {
            t = (-kn + p0.length()) / kd;
            if (t >= limit.x && t <= limit.y)
                return true;
        }
        return false;
    }

    bool hit(skRayHitTest& ht, const skRay& ray, const skVector2& limit) const
    {
        if (hit(ht.distance, ray, limit))
        {
            ht.normal = n;
            ht.point  = ray.at(ht.distance);
            return true;
        }
        return false;
    }

    void print() const;
};

#endif  //_skPlane_h_
