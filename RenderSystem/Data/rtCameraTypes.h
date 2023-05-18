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

#ifndef _rtCameraTypes_h_
#define _rtCameraTypes_h_

#include "RenderSystem/Data/rtBackendTypes.h"
#include "RenderSystem/Math/rtVectorTypes.h"

/// <summary>
///
/// </summary>
struct rtCameraType
{
    /// <summary>
    /// The vertical field of view angle in degrees.
    /// </summary>
    rtScalar fovY;

    /// <summary>
    /// The aspect ratio of the viewport.
    /// </summary>
    rtScalar aspect;

    /// <summary>
    /// The clip limits for a ray test.
    ///
    /// \f$ \begin{bmatrix}
    /// z_{near},
    /// z_{far}
    /// \end{bmatrix}
    /// \f$
    /// </summary>
    rtVector2 limits;

    /// <summary>
    /// The camera's world location.
    ///
    /// \f$ \begin{bmatrix}
    /// x,
    /// y,
    /// z
    /// \end{bmatrix}
    /// \f$
    /// </summary>
    rtVector3 location;

    /// <summary>
    /// The camera's world orientation.
    ///
    /// \f$ \begin{bmatrix}
    /// x,
    /// y,
    /// z,
    /// w
    /// \end{bmatrix}
    /// \f$
    /// </summary>
    rtVector4 rotation;

    /// <summary>
    /// Offsets for calculating the ray direction.
    ///
    /// \f$ \begin{bmatrix}
    /// \frac{1}{w},
    /// \frac{1}{h},
    /// \frac{w}{h},
    /// -tan(\frac{fovY}{2})
    /// \end{bmatrix}
    /// \f$
    /// </summary>
    rtVector4 offset;
};

/*! @} */
#endif  //_rtCameraTypes_h_
