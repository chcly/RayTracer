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

#ifndef _rtImageTarget_h_
#define _rtImageTarget_h_

#include "Image/skImage.h"
#include "RenderSystem/rtTarget.h"

/// <summary>
/// Implementation of the rtTarget class for
/// file based images.
/// </summary>
class rtImageTarget : public rtTarget
{
private:
    skImage*          m_image;
    rtFrameBufferInfo m_frameBuffer;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="w"></param>
    /// <param name="h"></param>
    rtImageTarget(SKuint32 w, SKuint32 h);

    ~rtImageTarget() override;

    /// <summary>
    ///
    /// </summary>
    /// <param name="path"></param>
    void save(const char* path) const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getWidth() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getHeight() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getSizeInBytes() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getPitch() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtFrameBufferInfo& getFrameBufferInfo() override;
};
/*! @} */

SK_INLINE const rtFrameBufferInfo& rtImageTarget::getFrameBufferInfo()
{
    return m_frameBuffer;
}

#endif  //_rtImageTarget_h_
