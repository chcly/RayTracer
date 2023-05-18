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

#ifndef _rtTickState_h_
#define _rtTickState_h_

#include "Math/skScalar.h"

class skTimer;

/// <summary>
/// Runs a sub loop at a specified fixed rate.
/// </summary>
/// <remarks>The default rate is 60 ticks per second.</remarks>
class rtTickState
{
private:
    SKulong  m_ticks, m_rate;
    SKulong  m_cur, m_next;
    skScalar m_fixed;
    bool     m_lock, m_init;
    skTimer* m_timer;

protected:
    /// <summary>
    /// Main class callback.
    /// </summary>
    /// <param name="rate">Constant at 1/rate.</param>
    virtual void tickImpl(const skScalar& rate) = 0;

    /// <summary>
    /// Optional start callback.
    /// </summary>
    /// <remarks>Runs outside the sub loop at the full frame rate.</remarks>
    virtual void beginTickImpl()
    {
    }

    /// <summary>
    /// Optional end callback.
    /// </summary>
    /// <remarks>Runs outside the sub loop at the full frame rate.</remarks>
    virtual void endTickImpl()
    {
    }

public:
    /// <summary>
    /// Default constructor
    /// </summary>
    /// <remarks>Initializes the step rate to 60.</remarks>
    rtTickState();

    virtual ~rtTickState();

    /// <summary>
    /// Zeros all counters and starts anew the next tick.
    /// </summary>
    void reset();

    /// <summary>
    /// Zeros all counters and starts anew the next tick with the specified rate.
    /// </summary>
    /// <param name="rate">Value in number of seconds.</param>
    void initialize(int rate);

    /// <summary>
    /// Executes one timer step with beginTickImpl,  tickImpl, endTickImpl.
    /// </summary>
    void tickCallbacks();

    /// <summary>
    /// Executes one timer step with tickImpl.
    /// </summary>
    void tick();
};

/*! @} */
#endif  //_rtTickState_h_
