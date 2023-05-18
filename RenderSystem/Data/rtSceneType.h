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
#ifndef _rtSceneType_h_
#define _rtSceneType_h_

#include "RenderSystem/Math/rtVectorTypes.h"
#include "RenderSystem/Data/rtArray.h"
#include "RenderSystem/Data/rtLightTypes.h"
#include "RenderSystem/Data/rtCameraTypes.h"
#include "RenderSystem/Data/rtObjectTypes.h"


/// <summary>
///
/// </summary>
using rtLightArray = rtArray<rtLightType*>;

/// <summary>
///
/// </summary>
using rtObjectArray = rtArray<rtObjectType*>;

/// <summary>
///
/// </summary>
struct rtSceneType
{
    /// <summary>
    ///
    /// </summary>
    rtVector3 horizon;

    /// <summary>
    ///
    /// </summary>
    rtVector3 zenith;

    /// <summary>
    ///
    /// </summary>
    int flags;

    /// <summary>
    ///
    /// </summary>
    rtCameraType* camera;

    /// <summary>
    ///
    /// </summary>
    rtLightArray lights;

    /// <summary>
    ///
    /// </summary>
    rtObjectArray objects;
};

/*! @} */
#endif  //_rtSceneType_h_
