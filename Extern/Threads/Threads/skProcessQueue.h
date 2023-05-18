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
#ifndef _skProcessQueue_h_
#define _skProcessQueue_h_

#include "Threads/skCriticalSection.h"
#include "Threads/skSemaphore.h"
#include "Threads/skThread.h"
#include "Threads/skThreadUtils.h"
#include "Utils/skQueue.h"

class skPacket
{
private:
    SKuint32 m_id;
    skPacket(const skPacket& pak) = delete;

public:
    skPacket() :
        m_id(0)
    {
    }

    skPacket(const SKuint32& id) :
        m_id(id)
    {
    }

    virtual ~skPacket() = default;


    int getId() const
    {
        return m_id;
    }

    /// <summary>
    /// Handle method for a background packet
    /// </summary>
    /// <returns>
    /// A return value of 0 means finished or done and this packet can be removed.
    /// A non-zero return value means keep the packet alive for further processing .
    /// </returns>
    virtual int handle()
    {
        return 0;
    }

};

class skProcessQueuePrivate;


class skProcessQueue
{

private:
    friend class skProcessQueuePrivate;

    skProcessQueuePrivate* m_private;

public:
    skProcessQueue();
    virtual ~skProcessQueue();

    void enqueue(skPacket* packet);

    void start();

    void stop();
};

#endif  //_skProcessQueue_h_
