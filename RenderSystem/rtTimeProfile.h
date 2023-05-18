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

#ifndef _rtTimeProfile_h_
#define _rtTimeProfile_h_

#include "Math/skScalar.h"

class skTimer;

/// <summary>
/// Measures an accumulating time span
/// </summary>
class rtTimeProfile final
{
private:
    skTimer* m_timer;

    SKulong m_start;
    SKulong m_end;
    SKulong m_frames;

    skScalar m_averageFps;
    skScalar m_lastFps;
    double   m_accumulatedFps;

public:
    rtTimeProfile();

    ~rtTimeProfile();

    /// <summary>
    ///
    /// </summary>
    void clear();

    /// <summary>
    ///
    /// </summary>
    void startSample();

    /// <summary>
    ///
    /// </summary>
    void endSample();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getLastFramesPerSecond() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skScalar& getAverageFramesPerSecond() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKulong getNumberOfSamples() const;
};

/*! @} */

SK_INLINE const skScalar& rtTimeProfile::getLastFramesPerSecond() const
{
    return m_lastFps;
}

SK_INLINE const skScalar& rtTimeProfile::getAverageFramesPerSecond() const
{
    return m_averageFps;
}

SK_INLINE SKulong rtTimeProfile::getNumberOfSamples() const
{
    return m_frames;
}
#endif  //_rtTimeProfile_h_
