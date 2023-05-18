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
#ifndef _skCriticalSection_h_
#define _skCriticalSection_h_

#include "Threads/skThreadUtils.h"

#if SK_PLATFORM == SK_PLATFORM_WIN32
#include "Windows/skWindowsCriticalSection.h"
#define skPlatformCriticalSection skWindowsCriticalSection
#else
#include "Posix/skPosixCriticalSection.h"
#define skPlatformCriticalSection skPosixCriticalSection
#endif

class skCriticalSection final : protected skPlatformCriticalSection
{
public:
    skCriticalSection()           = default;
    ~skCriticalSection() override = default;

    void lock()
    {
        lockImpl();
    }

    void unlock()
    {
        unlockImpl();
    }
};

class skCriticalSectionLock
{
private:
    skCriticalSection* m_cs;

public:
    explicit skCriticalSectionLock(skCriticalSection* cs) :
        m_cs(cs)
    {
        if (m_cs)
            m_cs->lock();
    }

    ~skCriticalSectionLock()
    {
        if (m_cs)
            m_cs->unlock();
    }
};

#define SK_SCOPE_LOCK_CRITICAL_SECTION(cs) \
    skCriticalSectionLock ____scope__lock__(cs)

#endif  //_skCriticalSection_h_
