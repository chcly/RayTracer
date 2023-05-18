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

#ifndef _skInteractiveCamera_h_
#define _skInteractiveCamera_h_

#include "Math/skMatrix4.h"
#include "rtCamera.h"

/// <summary>
/// Implementation of a simple zoom / pan / pivot
/// camera system.
/// </summary>
class rtInteractiveCamera final : public rtCamera
{
private:
    skVector3    m_vLoc;
    skQuaternion m_camOri;
    skQuaternion m_camOffs;
    skScalar     m_fixedDistance;

    bool updateImpl() override;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="sc"></param>
    rtInteractiveCamera(rtScene* sc);
    ~rtInteractiveCamera() override;

    /// <summary>
    ///
    /// </summary>
    /// <param name="dx"></param>
    /// <param name="dy"></param>
    void pivot(skScalar dx, skScalar dy);

    /// <summary>
    ///
    /// </summary>
    /// <param name="dx"></param>
    /// <param name="dy"></param>
    void pan(skScalar dx, skScalar dy);

    /// <summary>
    ///
    /// </summary>
    /// <param name="scale"></param>
    void zoom(skScalar scale);

    /// <summary>
    ///
    /// </summary>
    void reset();

    /// <summary>
    ///
    /// </summary>
    void center();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skScalar getFixedDistance() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="units"></param>
    void setFixedDistance(skScalar units);

    /// <summary>
    ///
    /// </summary>
    /// <param name="cOri"></param>
    void setDefaultCameraOrientation(const skQuaternion& cOri);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int getType() override;
};
/*! @} */

SK_INLINE skScalar rtInteractiveCamera::getFixedDistance() const
{
    return m_fixedDistance;
}

SK_INLINE int rtInteractiveCamera::getType()
{
    return RT_AO_USER_CAMERA;
}

#endif  //_skInteractiveCamera_h_
