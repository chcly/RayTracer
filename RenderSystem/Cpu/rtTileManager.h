/*
-------------------------------------------------------------------------------
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
#ifndef _rtTileManager_h_
#define _rtTileManager_h_

#include "RenderSystem/rtCommon.h"
#include "RenderSystem/rtTarget.h"
#include "Utils/skArray.h"

class rtTile;
class rtCpuRenderSystem;

/// <summary>
/// Thread manager for rtTile
/// </summary>
class rtTileManager
{
public:
    typedef skArray<rtTile*> Threads;

private:
    friend class rtTile;

    Threads            m_threads;
    SKint32            m_subdivisions;
    rtFrameBufferInfo  m_frameBuffer;
    rtCpuRenderSystem* m_system;

#ifdef RT_EXTRA_DEBUG
    bool m_overlayGrid;
#endif

public:
    /// <summary>
    /// Primary constructor.
    /// </summary>
    /// <param name="system">Callback handle to the CPU renderer.</param>
    /// <param name="fbi">A const reference to the frame buffer </param>
    rtTileManager(rtCpuRenderSystem*       system,
                  const rtFrameBufferInfo& fbi);

    virtual ~rtTileManager();

#ifdef RT_EXTRA_DEBUG
    /// <summary>
    /// Enables a debug overlay. This must be set before the call to initialize.
    /// </summary>
    /// <param name="doIt"></param>
    void overlayGrid(const bool doIt)
    {
        m_overlayGrid = doIt;
    }
#endif

    /// <summary>
    /// Subdivides the frame buffer into rtTile instances.
    /// </summary>
    void initialize();

    /// <summary>
    /// Starts all tile threads, then joins them back to the main thread.
    /// </summary>
    /// <remarks>This is the main update method for a rtTile instance.</remarks>
    void synchronize();
};

#endif  //_rtTileManager_h_
