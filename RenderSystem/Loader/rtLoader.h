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
#ifndef _rtLoader_h_
#define _rtLoader_h_

#include "Utils/skArray.h"
#include "RenderSystem/rtCommon.h"

/// <summary>
/// rtLoader is the base loader class.
/// </summary>
/// <remarks>
/// Its only responsibility is to take a specific input file
/// and output rtScene instances.
/// </remarks>
class rtLoader
{
public:
    typedef skArray<rtScene*> SceneArray;

protected:
    /// <summary>
    /// Protected storage for loaded scenes.
    /// </summary>
    SceneArray m_scenes;

public:
    rtLoader();
    virtual ~rtLoader();

    /// <summary>
    /// Load method
    /// </summary>
    /// <param name="path">File system path to the file</param>
    /// <returns>Should return -1 on error or 0 on success.</returns>
    virtual int load(const char* path) = 0;


    /// <summary>
    /// Access to scene instances
    /// </summary>
    /// <returns>An array of scenes.</returns>
    SceneArray& getScenes()
    {
        return m_scenes;
    }
};


/*! @} */

#endif  //_rtLoader_h_
