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
#include <limits>
#include "skWindowsMutex.h"
#include "Utils/skPlatformHeaders.h"

skWindowsMutex::skWindowsMutex() :
    m_isLocked(false)
{
    m_mutex = (SKsize)CreateMutex(NULL, false, NULL);
    if (!m_mutex)
    {
        tracef("CreateMutex returned: %d\n", ::GetLastError());
    }
}

skWindowsMutex::~skWindowsMutex()
{
    if (m_mutex)
    {
        ::CloseHandle((HANDLE)m_mutex);
        m_mutex = 0;
    }
}

void skWindowsMutex::lockImpl(void)
{
    if (!m_isLocked)
    {
        m_isLocked = true;
        waitImpl();
    }
}

void skWindowsMutex::unlockImpl(void)
{
    if (m_isLocked)
    {
        m_isLocked = false;
        notifyImpl();
    }
}

void skWindowsMutex::waitImpl(void)
{
    if (m_mutex)
    {
        int res = ::WaitForSingleObject((HANDLE)m_mutex, INFINITE);
        if (res)
            tracef("WaitForSingleObject returned: %i %d", res, ::GetLastError());
    }
}

void skWindowsMutex::waitImpl(SKsize milliseconds)
{
    if (m_mutex)
    {
        int res = ::WaitForSingleObject((HANDLE)m_mutex, (DWORD)milliseconds);
        if (res)
            tracef("WaitForSingleObject returned: %i %d", res, ::GetLastError());
    }
}

void skWindowsMutex::notifyImpl(void)
{
    if (m_mutex)
        ::ReleaseMutex((HANDLE)m_mutex);
}
