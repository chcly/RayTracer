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
#include "rtTileManager.h"
#include <thread>
#include "RenderSystem/Cpu/rtCpuRenderSystem.h"
#include "Utils/skRandom.h"
#include "rtCpuKernel.h"
#include "RenderSystem/rtCamera.h"
#include "Threads/skThread.h"

class rtCpuRenderSystem;
class rtTileManager;

/// <summary>
/// rtTile represents a subdivided region of the frame buffer
/// that can be rendered to in parallel.
/// </summary>
class rtTile final : public skRunnable
{
private:
    rtTileParams       m_params;
    rtFrameBufferInfo  m_frameBuffer;
    rtCpuRenderSystem* m_system;

#ifdef RT_EXTRA_DEBUG
    skColor m_color;
    bool    m_overlayGrid;
#endif

public:
    /// <summary>
    /// Primary constructor.
    /// </summary>
    /// <param name="sys">Handle to the CPU render system.</param>
    /// <param name="width">The width of this tile.</param>
    /// <param name="height">The height of this tile.</param>
    /// <param name="x">The starting x-coordinate.</param>
    /// <param name="y">The starting y-coordinate.</param>
    /// <param name="fbi">A const reference to the frame buffer.</param>
    rtTile(rtCpuRenderSystem*       sys,
           SKint32                 width,
           SKint32                 height,
           SKint32                 x,
           SKint32                 y,
           const rtFrameBufferInfo& fbi) :
        m_params{x, y, x + width, y + height},
        m_frameBuffer(fbi),
        m_system(sys)
#ifdef RT_EXTRA_DEBUG
        ,
        m_overlayGrid(false)
#endif
    {
    }

    ~rtTile() override = default;

#ifdef RT_EXTRA_DEBUG
    void setColor(const skColor& col)
    {
        m_overlayGrid = true;
        m_color       = col;
    }
#endif

    /// <summary>
    /// The overridden update method for this thread.
    /// </summary>
    int update() override
    {
        rtCpuKernelMain(m_frameBuffer,
                        m_system->getKernelScene(),
                        &m_params);
        return 0;
    }
};



rtTileManager::rtTileManager(rtCpuRenderSystem*       system,
                             const rtFrameBufferInfo& fbi) :
    m_frameBuffer(fbi),
    m_system(system)
#ifdef RT_EXTRA_DEBUG
    ,
    m_overlayGrid(false)
#endif

{
    // use the maximum number of cores possible. 
    m_subdivisions = skMax<SKuint32>(1, std::thread::hardware_concurrency());
}

rtTileManager::~rtTileManager()
{
    for (rtTile* element : m_threads)
        element->join();
    for (rtTile* element : m_threads)
        delete element;
};

void rtTileManager::initialize()
{
    if (!m_frameBuffer.pixels)
        return;

    // calculate the total number of subdivisions that can fit
    // in the height.  

    SKint32 tRh = m_frameBuffer.height % m_subdivisions;
    if (tRh > 0)
        tRh = m_subdivisions - tRh;

    const SKint32 h1 = (m_frameBuffer.height + tRh) / m_subdivisions;
    const SKint32 h2 = m_frameBuffer.height - h1 * (m_subdivisions - 1);

#ifdef RT_EXTRA_DEBUG
    m_overlayGrid = m_system->getMode() & RM_DEBUG_TILE;
#endif

    if (m_threads.empty())
    {
        m_threads.reserve(m_subdivisions);

        SKint32 h;
        SKint32 jv = 0;

        for (SKint32 j = 0; j < m_subdivisions; j++, jv += h)
        {
            if (j == 0)
                h = h2;
            else
                h = h1;

            rtTile* tile = new rtTile(m_system,
                                      m_frameBuffer.width,
                                      h,
                                      0,
                                      jv,
                                      m_frameBuffer);
#ifdef RT_EXTRA_DEBUG
            if (m_overlayGrid)
            {
                // Allow some debug overlay...
                tile->setColor({
                    (float)skRandomUnsignedInt(255) / 255.f,
                    (float)skRandomUnsignedInt(255) / 255.f,
                    (float)skRandomUnsignedInt(255) / 255.f,
                });
            }
#endif
            m_threads.push_back(tile);
        }
    }
}

void rtTileManager::synchronize()
{
    // kick them off then join them

    for (rtTile* element : m_threads)
        element->start();
    for (rtTile* element : m_threads)
        element->join();
}
