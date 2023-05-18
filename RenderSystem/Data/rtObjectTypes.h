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

#ifndef _rtObjectTypes_h_
#define _rtObjectTypes_h_

#include "RenderSystem/Math/rtVectorTypes.h"
struct rtMaterialType;

/// <summary>
///
/// </summary>
struct rtPlaneVolume
{
    /// <summary>
    ///
    /// </summary>
    rtVector3 p0;

    /// <summary>
    ///
    /// </summary>
    rtVector3 n;
};

/// <summary>
///
/// </summary>
struct rtSphereVolume
{
    /// <summary>
    ///
    /// </summary>
    rtVector3 center;

    /// <summary>
    ///
    /// </summary>
    rtScalar radius;
};

/// <summary>
///
/// </summary>
struct rtBoundingVolume
{
    /// <summary>
    ///
    /// </summary>
    rtScalar bMin[3];

    /// <summary>
    ///
    /// </summary>
    rtScalar bMax[3];

    /// <summary>
    ///
    /// </summary>
    void* data;
};

/// <summary>
///
/// </summary>
struct rtObjectType
{
    /// <summary>
    ///
    /// </summary>
    rtBoundingVolume bounds;

    /// <summary>
    ///
    /// </summary>
    int type;

    /// <summary>
    ///
    /// </summary>
    rtVector3 location;

    /// <summary>
    ///
    /// </summary>
    rtVector4 rotation;

    /// <summary>
    ///
    /// </summary>
    rtVector3 scale;

    /// <summary>
    ///
    /// </summary>
    rtMaterialType* material;
};

/*! @} */
#endif  //_rtObjectTypes_h_
