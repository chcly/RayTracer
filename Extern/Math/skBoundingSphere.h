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
#ifndef _skBoundingSphere_h_
#define _skBoundingSphere_h_

#include "Math/skMath.h"
#include "Math/skVector2.h"
#include "Math/skVector3.h"
#include "Math/skRay.h"

class skBoundingSphere
{
public:
    skVector3 center;
    skScalar  radius;

public:
    skBoundingSphere() :
        center{0,0,0},
        radius(0)
    {
    }

    skBoundingSphere(const skVector3& newCenter, const skScalar& newRadius);

    bool hit(skRayHitTest& dest, const skRay& ray, const skVector2& limit) const;

    bool hit(const skRay& ray, const skVector2& limit) const;
};

#endif  //_skBoundingSphere_h_
