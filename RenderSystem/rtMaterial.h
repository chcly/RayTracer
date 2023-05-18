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

#ifndef _rtMaterial_h_
#define _rtMaterial_h_

#include "Data/rtMaterialTypes.h"

struct rtMaterialType;

/// <summary>
///
/// </summary>
enum rtMaterialFlags
{
    /// <summary>
    ///
    /// </summary>
    RT_MA_SHADELESS = 0x00,
    /// <summary>
    ///
    /// </summary>
    RT_MA_REFLECT = 0x01,
    /// <summary>
    ///
    /// </summary>
    RT_MA_SHADOW = 0x02,
    /// <summary>
    ///
    /// </summary>
    RT_MA_SPECULAR = 0x04,
    /// <summary>
    ///
    /// </summary>
    RT_MA_AMBIENT = 0x08,
    /// <summary>
    ///
    /// </summary>
    RT_MA_DIFFUSE = 0x10,
    /// <summary>
    ///
    /// </summary>
    RT_MA_LIGHTING = RT_MA_SHADOW | RT_MA_SPECULAR | RT_MA_AMBIENT | RT_MA_DIFFUSE,
};

/// <summary>
/// 
/// </summary>
class rtMaterial
{
private:
    rtMaterialType* m_data;

public:
    /// <summary>
    /// 
    /// </summary>
    rtMaterial();
    ~rtMaterial();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtMaterialType* getPtr() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtMaterialType& getData();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtMaterialType& getData() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="flags"></param>
    void setFlags(SKint32 flags);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKint32 getFlags();

    /// <summary>
    ///
    /// </summary>
    /// <param name="color"></param>
    void setColor(float color[3]);

    /// <summary>
    ///
    /// </summary>
    /// <param name="diffuse"></param>
    void setDiffuse(rtScalar diffuse);

    /// <summary>
    ///
    /// </summary>
    /// <param name="ambient"></param>
    void setAmbient(rtScalar ambient);

    /// <summary>
    ///
    /// </summary>
    /// <param name="hardness"></param>
    void setHardness(rtScalar hardness);

    /// <summary>
    ///
    /// </summary>
    /// <param name="specular"></param>
    void setSpecular(rtScalar specular);
};
/*! @} */

SK_INLINE rtMaterialType* rtMaterial::getPtr() const
{
    return m_data;
}

SK_INLINE rtMaterialType& rtMaterial::getData()
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE const rtMaterialType& rtMaterial::getData() const
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE void rtMaterial::setFlags(SKint32 flags)
{
    getData().flags = flags;
}

SK_INLINE SKint32 rtMaterial::getFlags()
{
    return getData().flags;
}

SK_INLINE void rtMaterial::setColor(float color[3])
{
    getData().color = {color[0], color[1], color[2]};
}

SK_INLINE void rtMaterial::setDiffuse(rtScalar diffuse)
{
    getData().diffuse = diffuse;
}

SK_INLINE void rtMaterial::setAmbient(rtScalar ambient)
{
    getData().ambient = ambient;
}

SK_INLINE void rtMaterial::setHardness(rtScalar hardness)
{
    getData().hardness = hardness;
}

SK_INLINE void rtMaterial::setSpecular(rtScalar specular)
{
    getData().specular = specular;
}

#endif  //_rtMaterial_h_
