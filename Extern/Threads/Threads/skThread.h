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
#ifndef _skThread_h_
#define _skThread_h_

#include "Threads/skThreadUtils.h"


#if SK_PLATFORM == SK_PLATFORM_WIN32
#include "Windows/skWindowsThread.h"
#define skPlatformThread skWindowsThread
#else
#include "Posix/skPosixThread.h"
#define skPlatformThread skPosixThread
#endif


class skThread : public skPlatformThread
{
public:
    skThread();
    ~skThread() override;


    void wait() const
    {
        waitImpl(0);
    }

    void wait(SKsize milliseconds) const
    {
        waitImpl(milliseconds);
    }

    void start()
    {
        startImpl();
    }

    void join()
    {
        joinImpl();
    }


    int update() override
    {
        // rejoin the thread if not overridden
        joinImpl();
        return 0;
    }
};

using skRunnable = skThread;

#endif  //_skThread_h_
