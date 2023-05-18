/*
-------------------------------------------------------------------------------
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

#ifndef _rtTarget_h_
#define _rtTarget_h_

/// <summary>
/// Structure to hold information about a frame buffer.
/// </summary>
struct rtFrameBufferInfo
{
    // R8G8B8A8
    /// <summary>
    /// The array of pixels, where 1 pixel is equal to 4 unsigned bytes
    /// [r, g, b, a]
    /// </summary>
    SKubyte* pixels;

    /// <summary>
    /// The total width of the buffer.
    /// </summary>
    SKuint16 width;

    /// <summary>
    /// The total height of the buffer.
    /// </summary>
    SKuint16 height;

    /// <summary>
    /// The pitch or the step in y for the buffer.
    /// width * 4
    /// </summary>
    SKuint16 pitch;

    /// <summary>
    /// The total size in bytes of the buffer.
    /// Set to width * height * 4
    /// </summary>
    SKuint32 maxSize;
};



/// <summary>
/// The rtTarget class is an abstract class to
/// implement a writable image buffer or drawing surface.
/// </summary>
class rtTarget
{
public:
    rtTarget()          = default;
    virtual ~rtTarget() = default;

    /// <summary>
    /// Gets the width of the target.
    /// </summary>
    /// <returns>The width in pixels.</returns>
    virtual SKuint32 getWidth() = 0;

    /// <summary>
    /// Gets the height of the target.
    /// </summary>
    /// <returns>The height in pixels.</returns>
    virtual SKuint32 getHeight() = 0;

    /// <summary>
    /// Gets the pitch of the target.
    /// </summary>
    /// <returns>The pitch or W *4.</returns>
    virtual SKuint32 getPitch() = 0;

    /// <summary>
    /// Gets total memory for the target.
    /// </summary>
    /// <returns>The W*H*4 </returns>
    virtual SKuint32 getSizeInBytes() = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    virtual const rtFrameBufferInfo& getFrameBufferInfo() = 0;
};
/*! @} */

#endif  //_rtTarget_h_
