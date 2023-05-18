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
#include "Threads/Posix/skPosixSemaphore.h"
#include <stdio.h>
#include "Threads/Posix/skPosixUtils.h"

void skPosixSemaphore::initialize()
{
    int st = sem_init(&m_handle, 0, 1);
    if (st == -1)
    {
        switch (errno)
        {
        case EINVAL:  // hard coded
            break;
        case ENOSPC:
            tracef("The maximum number of semaphores has been reached\n");
            break;
        case ENOSYS:
            tracef("sem_init is not supported\n");
            break;
        case EPERM:
            tracef("The process lacks the privileges to use sem_init\n");
            break;
        default:
            tracef("sem_init - unknown error\n");
            break;
        }

        m_handle = skPosixUtils::NullSemaphore;
    }
}

void skPosixSemaphore::finalize()
{
    int st = sem_destroy(&m_handle);
    if (st == -1)
    {
        switch (errno)
        {
        case EINVAL:
            tracef("Invalid handle\n");
            break;
        case ENOSYS:
            tracef("sem_destroy is not supported\n");
            break;
        case EBUSY:
            tracef("semaphore is in use\n");
            break;
        default:
            tracef("sem_destroy - unknown error\n");
            break;
        }

        m_handle = skPosixUtils::NullSemaphore;
    }
}

skPosixSemaphore::skPosixSemaphore() :
    m_handle(skPosixUtils::NullSemaphore)
{
    initialize();
}

skPosixSemaphore::~skPosixSemaphore()
{
    finalize();
}

void skPosixSemaphore::waitImpl()
{
    sem_wait(&m_handle);
}

void skPosixSemaphore::signalImpl()
{
    sem_post(&m_handle);
}
