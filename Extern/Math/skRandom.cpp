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
#include <cstdio>
#include <cstdlib>
#include "skScalar.h"

#if SK_PLATFORM == SK_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <limits>
#else
#include <sys/time.h>
#endif
#undef max

void skRandInit()
{
    long sRandVal = 0;

#if SK_PLATFORM == SK_PLATFORM_WIN32
    sRandVal = (long)((ULONGLONG)GetTickCount64() % std::numeric_limits<long>::max());
#else
    static struct timeval now;
    gettimeofday(&now, NULL);
    sRandVal = (now.tv_sec) / 1000;
#endif
    srand(sRandVal);
}

skScalar skUnitRand()
{
    return skScalar(rand()) / skScalar(RAND_MAX);
}

skScalar skUnitNRand()
{
    return skScalar(2.0) * skUnitRand() - skScalar(1.0);
}

SKint32 skRandIntRange(SKint32 rmi, SKint32 rma)
{
    return SKint32((skScalar(rma) - skScalar(rmi)) * skUnitRand() + skScalar(rmi));
}
