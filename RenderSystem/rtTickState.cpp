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
#include "rtTickState.h"
#include "Math/skMath.h"
#include "Utils/skTimer.h"

rtTickState::rtTickState() :
    m_ticks(0),
    m_rate(0),
    m_cur(0),
    m_next(0),
    m_fixed(0),
    m_lock(false),
    m_init(false),
    m_timer(nullptr)
{
    initialize(60);
}

rtTickState::~rtTickState()
{
    delete m_timer;
}

void rtTickState::initialize(int rate)
{
    m_rate  = skClamp<SKulong>(rate, 1, 200);
    m_fixed = skScalar(1) / skScalar(m_rate);
    m_ticks = (SKulong)(1000.f * m_fixed);
    m_timer = new skTimer();
    m_init  = false;
}

void rtTickState::reset()
{
    m_init = false;
}

void rtTickState::tickCallbacks()
{
    beginTickImpl();
    tick();
    endTickImpl();
}

void rtTickState::tick()
{
    m_lock = false;

    if (!m_init)
    {
        m_init = true;
        m_cur = m_next = m_timer->getMilliseconds();
    }

    if ((m_cur = m_timer->getMilliseconds()) > m_next)
    {
        tickImpl(m_fixed);
        m_next += m_ticks;
    }
}
