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
#include "Threads/Windows/skWindowsSemaphore.h"
#include <Windows.h>

skWindowsSemaphore::skWindowsSemaphore() :
    m_handle(0)
{
    initialize();
}

skWindowsSemaphore::~skWindowsSemaphore()
{
    finalize();
}

void skWindowsSemaphore::initialize()
{
    m_handle = (SKuintPtr)CreateSemaphore(
        nullptr,    // Cannot be inherited by child processes.
        0,          // lInitialCount
        SK_NPOS16,  // lMaximumCount
        nullptr     // lpName -  No name
    );

    if (!m_handle)
    {
        tracef("Failed to create semaphore : %lu", GetLastError());
    }
}

void skWindowsSemaphore::finalize() const
{
    if (m_handle)
    {
        if (CloseHandle((HANDLE)m_handle) == FALSE)
            tracef("Failed to close the semaphore handle : %lu\n", GetLastError());
    }
}

void skWindowsSemaphore::waitImpl() const
{
    if (m_handle)
    {
        const int res = WaitForSingleObject((HANDLE)m_handle, INFINITE);
        if (res)
            tracef("WaitForSingleObject returned: %i %lu", res, GetLastError());
    }
}

void skWindowsSemaphore::signalImpl() const
{
    if (m_handle)
    {
        if (ReleaseSemaphore(
                (HANDLE)m_handle,  // hSemaphore
                1,                 // lReleaseCount,
                nullptr            // lpPreviousCount
                ) == FALSE)
        {
            tracef("Failed to release the semaphore handle : %lu\n", GetLastError());
        }
    }
}
