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

#ifndef _rtPlane_h_
#define _rtPlane_h_

#include "Math/skPlane.h"
#include "RenderSystem/rtBvObject.h"

/// <summary>
/// 
/// </summary>
class rtPlane final : public rtBvObject
{
private:
    skPlane m_plane;
    skPlane m_derivedPlane;

    void postUpdateImpl() override;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="sc"></param>
    rtPlane(rtScene* sc);
    ~rtPlane() override;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    const skPlane& getPlane() const;


    /// <summary>
    /// 
    /// </summary>
    /// <param name="n"></param>
    /// <returns></returns>
    void setNormal(const skVector3& n);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="p0"></param>
    void setPoint(const skVector3& p0);

};
/*! @} */

SK_INLINE const skPlane& rtPlane::getPlane() const
{
    return m_derivedPlane;
}

SK_INLINE void rtPlane::setNormal(const skVector3& n)
{
    m_plane.n = n;
}

SK_INLINE void rtPlane::setPoint(const skVector3& p0)
{
    m_plane.p0 = p0;
}

#endif  //_rtPlane_h_
