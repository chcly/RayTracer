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

#ifndef _rtCube_h_
#define _rtCube_h_

#include "Math/skBoundingBox.h"
#include "rtBvObject.h"

class rtCube final : public rtBvObject
{
private:
    void postUpdateImpl() override;

    skScalar m_extent;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="sc"></param>
    explicit rtCube(rtScene* sc);
    ~rtCube() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int getType() override;

    /// <summary>
    ///
    /// </summary>
    /// <param name="extent"></param>
    /// <returns></returns>
    void setExtent(const skScalar& extent);
};

/*! @} */

SK_INLINE int rtCube::getType()
{
    return RT_AO_SHAPE_CUBE;
}

SK_INLINE void rtCube::setExtent(const skScalar& extent)
{
    m_extent = extent;
}

#endif  //_rtCube_h_
