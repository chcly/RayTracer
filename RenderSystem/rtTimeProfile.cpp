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
#include "RenderSystem/rtTimeProfile.h"
#include "Utils/skTimer.h"

constexpr double secondsPerMicro = 1e-6;

rtTimeProfile::rtTimeProfile() :
    m_timer(new skTimer()),
    m_start(0),
    m_end(0),
    m_frames(0),
    m_averageFps(0),
    m_lastFps(0),
    m_accumulatedFps(0)
{
}

rtTimeProfile::~rtTimeProfile()
{
    delete m_timer;
}

void rtTimeProfile::clear()
{
    m_timer->reset();
    m_frames         = 0;
    m_averageFps     = 0;
    m_lastFps        = 0;
    m_accumulatedFps = 0;
}

void rtTimeProfile::startSample()
{
    m_frames++;
    m_start = m_timer->getMicroseconds();
}

void rtTimeProfile::endSample()
{
    m_end = m_timer->getMicroseconds();

    const double delta = (double(m_end) - double(m_start)) * secondsPerMicro;

    if (delta > 0)
    {
        double fps = 1.0 / delta;
        if (fps < DBL_EPSILON)
            fps = DBL_EPSILON;

        m_lastFps = skScalar(fps);
        m_accumulatedFps += fps;
        m_averageFps = skScalar(m_accumulatedFps / double(m_frames));
    }
}
