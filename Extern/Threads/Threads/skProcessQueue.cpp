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
#include "Threads/skProcessQueue.h"
#include <stdio.h>
#include "Threads/skMutex.h"

class skProcessQueuePrivate : public skRunnable
{
public:
    typedef skQueue<skPacket*> PacketQueue;

private:
    enum RunStates
    {
        /// <summary>
        /// The background queue is not running
        /// </summary>
        PQS_STOPPED = 0x01,
        /// <summary>
        /// The background queue is running
        /// </summary>
        PQS_RUNNING = 0x02,
    };

    skCriticalSection m_criticalSection;
    skSemaphore       m_semaphore;
    skProcessQueue*   m_owner;
    PacketQueue       m_packets;
    SKint8            m_state;

public:
    skProcessQueuePrivate(skProcessQueue* owner) :
        m_owner(owner),
        m_state(PQS_STOPPED)
    {
    }

    ~skProcessQueuePrivate() override
    {
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="packet"></param>
    void enqueue(skPacket* packet)
    {
        SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
        m_packets.enqueue(packet);
        m_semaphore.signal();
    }

    /// <summary>
    ///
    /// </summary>
    void begin()
    {
        SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
        if (m_state & PQS_STOPPED)
        {
            m_state = PQS_RUNNING;
            start();
        }
    }

    /// <summary>
    ///
    /// </summary>
    void end()
    {
        SK_SCOPE_LOCK_CRITICAL_SECTION(&m_criticalSection);
        if (m_state & PQS_STOPPED)
            return;

        if (!m_packets.empty())
            m_state |= PQS_STOPPED;
        else
            m_state = PQS_STOPPED;

        m_semaphore.signal();
        join();
    }

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int update() override
    {
        try
        {
            return runProcess();
        }
        catch (...)
        {
            return -1;
        }
    }

private:
    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    int runProcess()
    {
        while (m_state & PQS_RUNNING)
        {
            if (!m_packets.empty())
            {
                while (!m_packets.empty())
                {
                    skPacket* pack = m_packets.dequeue();

                    (void)pack->handle();

                    delete pack;
                }
                m_semaphore.signal();
            }
            else
            {
                if (m_state & PQS_STOPPED)
                    m_state &= ~PQS_RUNNING;
                else
                    m_semaphore.wait();
            }
        }

        m_semaphore.signal();
        return 0;
    }
};

skProcessQueue::skProcessQueue()
{
    m_private = new skProcessQueuePrivate(this);
}

skProcessQueue::~skProcessQueue()
{
    delete m_private;
}

void skProcessQueue::enqueue(skPacket* packet)
{
    if (m_private)
        m_private->enqueue(packet);
}

void skProcessQueue::start()
{
    if (m_private)
        m_private->begin();
}

void skProcessQueue::stop()
{
    if (m_private)
        m_private->end();
}
