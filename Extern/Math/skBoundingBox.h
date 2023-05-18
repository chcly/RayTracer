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
#ifndef _skBoundingBox_h_
#define _skBoundingBox_h_

#include "Math/skBoundingSphere.h"
#include "Math/skMath.h"
#include "Math/skRay.h"
#include "Math/skVector3.h"

class skBoundingBox
{
public:
    skScalar bMin[3];
    skScalar bMax[3];

public:
    skBoundingBox()
    {
        clear();
    }

    skBoundingBox(const skScalar _bMin[3], const skScalar _bMax[3]);

    skBoundingBox(const skVector3& extent, const skVector3& center);

    void clear();

    void compare(const skVector3& pt);

    void compare(const skScalar* pt);

    void merge(const skBoundingBox& bb);

    void scale(const skScalar& sc);

    void translate(const skVector3& pt);

    void setMin(const skVector3& mi);

    void setMax(const skVector3& ma);

    skScalar signedLength() const;

    bool contains(const skBoundingBox& bb) const;

    static void majorAxis(skVector3& dest, const skVector3& src);

    bool hit(const skRay& ray, const skVector2& limit) const;

    bool hit(skScalar& r0, skScalar& r1, const skRay& ray, const skVector2& limit) const;

    bool hit(skRayHitTest& dest, const skRay& ray, const skVector2& limit) const;

    skVector3 min() const;

    skVector3 max() const;

    skVector3 extent() const;

    skScalar halfLength2() const;

    skScalar halfLength() const;

    skScalar max3() const;

    skScalar min3() const;

    skVector3 center() const;

    skScalar length() const;

    skScalar length2() const;

    bool less(const skBoundingBox& bb) const;
};

SK_INLINE skVector3 skBoundingBox::min() const
{
    return skVector3(bMin[0], bMin[1], bMin[2]);
}

SK_INLINE skVector3 skBoundingBox::max() const
{
    return skVector3(bMax[0], bMax[1], bMax[2]);
}

SK_INLINE skVector3 skBoundingBox::extent() const
{
    return skVector3(bMax[0] - bMin[0], bMax[1] - bMin[1], bMax[2] - bMin[2]);
}

SK_INLINE skScalar skBoundingBox::halfLength2() const
{
    return extent().length2() * skScalar(0.5);
}

SK_INLINE skScalar skBoundingBox::halfLength() const
{
    return extent().length() * skScalar(0.5);
}

SK_INLINE skScalar skBoundingBox::max3() const
{
    return skMax3(bMax[0], bMax[1], bMax[2]);
}

SK_INLINE skScalar skBoundingBox::min3() const
{
    return skMax3(bMin[0], bMin[1], bMin[2]);
}

SK_INLINE skVector3 skBoundingBox::center() const
{
    return skVector3(
        skScalar(0.5) * (bMax[0] + bMin[0]),
        skScalar(0.5) * (bMax[1] + bMin[1]),
        skScalar(0.5) * (bMax[2] + bMin[2]));
}

SK_INLINE skScalar skBoundingBox::length() const
{
    return extent().length();
}

SK_INLINE skScalar skBoundingBox::length2() const
{
    return extent().length();
}

SK_INLINE bool skBoundingBox::less(const skBoundingBox& bb) const
{
    return signedLength() < bb.signedLength();
}

#endif  //_skBoundingBox_h_
