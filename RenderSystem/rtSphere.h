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
/*! \addtogroup FrontEnd
 * @{
 */

#ifndef _rtSphere_h_
#define _rtSphere_h_

#include "Math/skBoundingSphere.h"
#include "rtBvObject.h"

class rtSphere : public rtBvObject
{
private:
    rtSphereVolume* m_sphere;

    void postUpdateImpl() override;

public:
    rtSphere(rtScene* sc);
    ~rtSphere() override;

    int getType() override;

    void setRadius(const skScalar& radius) const;

    skBoundingSphere getBoundingSphere() const;
};
/*! @} */

SK_INLINE int rtSphere::getType()
{
    return RT_AO_SHAPE_SPHERE;
}

SK_INLINE void rtSphere::setRadius(const skScalar& radius) const
{
    m_sphere->radius = radius;
}

SK_INLINE skBoundingSphere rtSphere::getBoundingSphere() const
{
    return {m_derived.location, m_sphere->radius};
}

#endif  //_rtSphere_h_
