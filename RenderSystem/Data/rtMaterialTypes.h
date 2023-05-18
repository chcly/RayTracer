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
/*! \addtogroup DataApi
 * @{
 */

#ifndef _rtMaterialTypes_h_
#define _rtMaterialTypes_h_

#include "RenderSystem/Math/rtVectorTypes.h"

/// <summary>
///
/// </summary>
struct rtMaterialType
{
    /// <summary>
    ///
    /// </summary>
    rtVector3 color;

    /// <summary>
    ///
    /// </summary>
    SKint32 shaderType;

    /// <summary>
    ///
    /// </summary>
    SKint32 flags;

    /// <summary>
    ///
    /// </summary>
    rtScalar ambient;

    /// <summary>
    ///
    /// </summary>
    rtScalar diffuse;

    /// <summary>
    ///
    /// </summary>
    rtScalar specular;

    /// <summary>
    ///
    /// </summary>
    rtScalar hardness;
};

/*! @} */
#endif  //_rtMaterialTypes_h_
