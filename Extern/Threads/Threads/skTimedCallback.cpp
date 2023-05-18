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
#include "Threads/skTimedCallback.h"
#include "Utils/skMinMax.h"

skTimedCallback::skTimedCallback(Listener*      listener,
                                 const SKulong& interrupt) :
    m_interrupt(interrupt),
    m_listener(listener),
    m_running(false)
{
}

skTimedCallback::~skTimedCallback()
{
    end();
}

void skTimedCallback::begin()
{
    SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
    if (!m_running)
    {
        m_running = true;
        skRunnable::startImpl();
    }
}

void skTimedCallback::setInterval(const SKulong& interrupt)
{
    SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
    m_interrupt = skMax<SKint32>(interrupt, 1);
}

void skTimedCallback::end()
{
    {
        SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
        if (m_running)
            m_running = false;
    }
    joinImpl();
}

int skTimedCallback::update()
{
    m_timer.reset();
    while (m_running)
    {
        const SKulong tick = m_timer.getMicroseconds();

        m_listener->tickStart();
        do
        {
            if (!m_running)
                break;

            wait(1);
        } while (m_timer.getMicroseconds() - tick < m_interrupt);

        m_listener->tickEnd();
    }
    return 0;
}
