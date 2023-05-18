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
#include "Threads/Windows/skWindowsThread.h"
#include "Threads/Windows/skWindowsMutex.h"
#include "Threads/skThread.h"
#include "Utils/skPlatformHeaders.h"

class Win32ThreadUtils
{
public:
    static DWORD WINAPI callback(LPVOID arg)
    {
        skWindowsThread* thread = static_cast<skWindowsThread*>(arg);
        if (thread)
            return thread->update();
        return 0;
    }
};

skWindowsThread::skWindowsThread() :
    m_thread(SK_THREAD_NULL),
    m_id(SK_NPOS)
{
}

skWindowsThread::~skWindowsThread()
{
    joinImpl();
}

void skWindowsThread::startImpl(void)
{
    if (m_thread != SK_THREAD_NULL)
        joinImpl();

    m_thread = (SKuintPtr)::CreateThread(
        nullptr,                     // No inheritance
        0,                           // Use the default stack size
        Win32ThreadUtils::callback,  // main routine
        this,                        // parameter
        0,                           // dwCreationFlags
        (LPDWORD)&m_id               // lpThreadId
    );
}

void skWindowsThread::waitImpl() const
{
    if (m_thread != SK_THREAD_NULL)
        ::WaitForSingleObject((HANDLE)m_thread, INFINITE);
}

void skWindowsThread::waitImpl(SKsize ms) const
{
    if (m_thread != SK_THREAD_NULL)
        ::WaitForSingleObject((HANDLE)m_thread, (DWORD)ms);
}

void skWindowsThread::joinImpl()
{
    if (m_thread != SK_THREAD_NULL)
    {
        waitImpl(INFINITE);

        if (CloseHandle((HANDLE)m_thread) == FALSE)
            tracef("Failed to close the thread handle : %d\n", GetLastError());

        m_id = SK_NPOS;
        m_thread = SK_THREAD_NULL;
    }
}
