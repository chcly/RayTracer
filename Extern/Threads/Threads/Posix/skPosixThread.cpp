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
#include "Threads/Posix/skPosixThread.h"
#include <unistd.h>
#include <pthread.h>
#include "Threads/Posix/skPosixMutex.h"
#include "Threads/skThread.h"
#include "Utils/skPlatformHeaders.h"

class PosixStaticUtils
{
public:

    static void* threadExec(void *arg0)
    {
        skThread* thread = (skThread*)arg0;
        if (thread)
            thread->update();
        return nullptr;
    }

};

skPosixThread::skPosixThread() :
    m_id(SK_NPOS),
    m_thread(SK_THREAD_NULL)
{
}

skPosixThread::~skPosixThread()
{
    if (m_thread != SK_THREAD_NULL)
    {
        int status;
        status = pthread_cancel(m_thread);
        if (status != 0)
            tracef("pthread_cancel returned: %i\n", status);
    }
}

void skPosixThread::startImpl()
{
    if (m_thread != SK_THREAD_NULL)
        joinImpl();
    int status;

    if ((status = pthread_create((pthread_t*)&m_thread,
                                 nullptr,
                                 PosixStaticUtils::threadExec,
                                 this)) != 0)
    {
        tracef("pthread_cancel pthread_create: %i\n", status);
    }
}

void skPosixThread::waitImpl(SKsize milliseconds) const
{
    usleep(milliseconds * 1000);
}

void skPosixThread::joinImpl()
{
    if (m_thread != SK_THREAD_NULL)
        pthread_join(m_thread, nullptr);
    m_thread = SK_THREAD_NULL;
}
