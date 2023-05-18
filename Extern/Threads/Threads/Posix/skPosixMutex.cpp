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
#include "Threads/Posix/skPosixMutex.h"
#include "Threads/Posix/skPosixUtils.h"
#include "Threads/skThreadUtils.h"
#include "Utils/skPlatformHeaders.h"

skPosixMutex::skPosixMutex() :
    m_mutex(skPosixUtils::NullMutex),
    m_isInit(false)
{
}

skPosixMutex::~skPosixMutex()
{
    int status;
    if (m_isInit)
    {
        status = pthread_mutex_destroy((pthread_mutex_t*)&m_mutex);
        if (status != SK_THREAD_NO_ERROR)
            tracef("pthread_mutex_destroy returned %i\n", status);
        m_isInit = false;
    }
}

void skPosixMutex::lockImpl()
{
    int status;
    if (!m_isInit)
    {
        status = pthread_mutex_init((pthread_mutex_t*)&m_mutex, nullptr);
        if (status != SK_THREAD_NO_ERROR)
        {
            tracef("pthread_mutex_init returned %i\n", status);
            return;
        }
        m_isInit = true;
    }

    status = pthread_mutex_lock((pthread_mutex_t*)&m_mutex);
    if (status != SK_THREAD_NO_ERROR)
        tracef("pthread_mutex_lock returned %i\n", status);
}

void skPosixMutex::unlockImpl()
{
    if (!m_isInit)
        return;

    int status = pthread_mutex_unlock((pthread_mutex_t*)&m_mutex);
    if (status != SK_THREAD_NO_ERROR)
        tracef("pthread_mutex_unlock returned %i\n", status);
}

void skPosixMutex::waitImpl()
{
    int status;
    if (m_isInit)
    {
        status = pthread_mutex_lock((pthread_mutex_t*)&m_mutex);
        if (status != SK_THREAD_NO_ERROR)
            tracef("pthread_mutex_lock returned %i\n", status);
    }
}

void skPosixMutex::waitImpl(SKsize milliseconds)
{
    int status;
    if (m_isInit)
    {
        struct timespec timed = {0, (long)(milliseconds * 1000)};

        status = pthread_mutex_timedlock((pthread_mutex_t*)&m_mutex, &timed);
        if (status != SK_THREAD_NO_ERROR)
            tracef("pthread_mutex_timedlock returned %i\n", status);
    }
}

void skPosixMutex::notifyImpl()
{
    int status;
    if (m_isInit)
    {
        status = pthread_mutex_lock((pthread_mutex_t*)&m_mutex);
        if (status != SK_THREAD_NO_ERROR)
            tracef("pthread_mutex_lock returned %i\n", status);
    }
}
