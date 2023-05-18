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

#ifndef _rtCamera_h_
#define _rtCamera_h_

#include "Math/skVector2.h"
#include "RenderSystem/Data/rtCameraTypes.h"
#include "RenderSystem/rtObject.h"

/// <summary>
///
/// </summary>
class rtCamera : public rtObject
{
private:
    rtCameraType* m_data;

    void postUpdateImpl() override;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="sc"></param>
    rtCamera(rtScene* sc);
    ~rtCamera() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int getType() override
    {
        return RT_AO_CAMERA;
    }

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skScalar getFieldOfViewAngle() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skScalar getNear() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skScalar getFar() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skScalar getAspect() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skVector2 getLimits() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="rTheta"></param>
    void setFieldOfViewAngle(skScalar rTheta);

    /// <summary>
    ///
    /// </summary>
    /// <param name="zNear"></param>
    void setNear(skScalar zNear);

    /// <summary>
    ///
    /// </summary>
    /// <param name="zFar"></param>
    void setFar(skScalar zFar);

    /// <summary>
    ///
    /// </summary>
    /// <param name="aspect"></param>
    void setAspect(skScalar aspect);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtCameraType* getPtr() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtCameraType& getData();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtCameraType& getData() const;
};
/*! @} */

SK_INLINE rtCameraType* rtCamera::getPtr() const
{
    return m_data;
}

SK_INLINE rtCameraType& rtCamera::getData()
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE const rtCameraType& rtCamera::getData() const
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE skScalar rtCamera::getFieldOfViewAngle() const
{
    return getData().fovY;
}

SK_INLINE skScalar rtCamera::getNear() const
{
    return getData().limits.x;
}

SK_INLINE skScalar rtCamera::getFar() const
{
    return getData().limits.y;
}

SK_INLINE skScalar rtCamera::getAspect() const
{
    return getData().aspect;
}

SK_INLINE skVector2 rtCamera::getLimits() const
{
    return skVector2(getData().limits.x, getData().limits.y);
}

#endif  //_rtCamera_h_
