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

#ifndef _rtLight_h_
#define _rtLight_h_

#include "bAscii/bApi.h"
#include "RenderSystem/rtObject.h"
#include "RenderSystem/Data/rtLightTypes.h"

/// <summary>
/// 
/// </summary>
class rtLight final : public rtObject
{
private:
    rtLightType* m_data;

    void postUpdateImpl() override;

public:
    rtLight(rtScene* sc);
    ~rtLight() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKint32 getType() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKint32 getMode() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="mode"></param>
    void setMode(SKint32 mode);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getEnergy() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="energy"></param>
    void setEnergy(const skScalar& energy);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getElevation() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="elevation"></param>
    void setElevation(const skScalar& elevation);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getDecay() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="decay"></param>
    void setDecay(const skScalar& decay);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getRadius() const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="radius"></param>
    void setRadius(const skScalar& radius);


    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtLightType* getPtr() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtLightType& getData();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtLightType& getData() const;
};
/*! @} */

SK_INLINE rtLightType* rtLight::getPtr() const
{
    return m_data;
}

SK_INLINE rtLightType& rtLight::getData()
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE const rtLightType& rtLight::getData() const
{
    SK_ASSERT(m_data);
    return *m_data;
}
SK_INLINE SKint32 rtLight::getType()
{
    return RT_AO_LIGHT;
}

SK_INLINE SKint32 rtLight::getMode() const
{
    return getData().mode;
}

SK_INLINE void rtLight::setMode(SKint32 mode)
{
    getData().mode = mode;
}


SK_INLINE const skScalar& rtLight::getEnergy() const
{
    return getData().energy;
}

SK_INLINE void rtLight::setEnergy(const skScalar& energy)
{
    getData().energy = skMax<skScalar>(4, energy);
}

SK_INLINE const skScalar& rtLight::getElevation() const
{
    return getData().elevation;
}

SK_INLINE void rtLight::setElevation(const skScalar& elevation)
{
    getData().elevation = 10.f*skClamp<skScalar>(elevation, 10e-3f, 1);
}

SK_INLINE const skScalar& rtLight::getDecay() const
{
    return getData().decay;
}

SK_INLINE void rtLight::setDecay(const skScalar& decay)
{
    getData().decay = 100.f * skClamp<skScalar>(decay, 10e-2f, 100);
}

SK_INLINE const skScalar& rtLight::getRadius() const
{
    return getData().radius;
}

SK_INLINE void rtLight::setRadius(const skScalar& radius)
{
    getData().radius = radius;
}

#endif  //_rtLight_h_
