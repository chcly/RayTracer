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

#ifndef _rtBvObject_h_
#define _rtBvObject_h_

#include "Math/skBoundingBox.h"
#include "Math/skPlane.h"
#include "RenderSystem/Data/rtObjectTypes.h"
#include "RenderSystem/rtMaterial.h"
#include "RenderSystem/rtObject.h"

class rtBvObject : public rtObject
{
protected:
    rtObjectType* m_data;
    rtMaterial*   m_material;

public:
    explicit rtBvObject(rtScene* sc);
    ~rtBvObject() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int getType() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    virtual skBoundingBox getBoundingBox() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    virtual skScalar getMajorAxis();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtMaterial* getMaterial() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtObjectType* getPtr() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtObjectType& getData();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtObjectType& getData() const;
};

/*! @} */

SK_INLINE rtObjectType* rtBvObject::getPtr() const
{
    return m_data;
}

SK_INLINE rtObjectType& rtBvObject::getData()
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE const rtObjectType& rtBvObject::getData() const
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE rtMaterial* rtBvObject::getMaterial() const
{
    return m_material;
}

SK_INLINE int rtBvObject::getType()
{
    return RT_AO_BVO;
}

SK_INLINE skBoundingBox rtBvObject::getBoundingBox() const
{
    return {m_data->bounds.bMin, m_data->bounds.bMax};
}

SK_INLINE skScalar rtBvObject::getMajorAxis()
{
    return getBoundingBox().max3();
}

#endif  //_rtBvObject_h_
