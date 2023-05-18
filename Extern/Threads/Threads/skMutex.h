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
#ifndef _skMutex_h_
#define _skMutex_h_

#include "Threads/skThreadUtils.h"

#if SK_PLATFORM == SK_PLATFORM_WIN32
#include "Windows/skWindowsMutex.h"
#define skPlatformMutex skWindowsMutex
#else
#include "Posix/skPosixMutex.h"
#define skPlatformMutex skPosixMutex
#endif



class skMutex : public skPlatformMutex
{
public:
    skMutex() = default;

    ~skMutex() override = default;

    void lock()
    {
        lockImpl();
    }

    void unlock()
    {
        unlockImpl();
    }

    void wait()
    {
        waitImpl();
    }

    void wait(const SKsize milliseconds)
    {
        waitImpl(milliseconds);
    }


    void notify()
    {
        notifyImpl();
    }
};


class skScopeLock
{
public:
    skScopeLock(skMutex* mtx) :
        m_mtx(mtx)
    {
        if (m_mtx)
            m_mtx->lock();
    }

    ~skScopeLock()
    {
        if (m_mtx)
            m_mtx->unlock();
    }

private:
    skMutex* m_mtx;
};


#define SK_LOCK_SCOPE(mtx) skScopeLock ____scope(mtx)

#endif  //_skMutex_h_
