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
#include "rtCpuRenderSystem.h"
#include <cstdio>
#include <thread>
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtScene.h"
#include "RenderSystem/rtTarget.h"
#include "rtTileManager.h"

rtCpuRenderSystem::rtCpuRenderSystem() :
    m_tiles(nullptr)
{
}

rtCpuRenderSystem::~rtCpuRenderSystem()
{
    delete m_tiles;
}

void rtCpuRenderSystem::initialize(rtScene* scene)
{
    m_scene = scene;
    if (!m_scene)
    {
        printf("Invalid supplied scene.\n");
        return;
    }

    if (!m_target)
    {
        printf("No target was specified.\n");
        return;
    }

    rtCameras& cameras = m_scene->getCameras();
    if (cameras.empty())
    {
        printf("No cameras were found in the scene.\n");
        return;
    }
    // Cache any persistent per frame variables before spiting into tiles.
    // Any render method / sub method must be immutable.
    m_camera = cameras.at(0);

    rtSceneType*  sc = m_scene->getPtr();
    rtCameraType* ca = m_camera->getPtr();
    updatePixelOffset();

    ca->offset = {
        m_iPixelOffset.x,
        m_iPixelOffset.y,
        m_iPixelOffset.z,
        m_iPixelOffset.w,
    };
    sc->camera = ca;

    delete m_tiles;

    m_tiles = new rtTileManager(this, m_target->getFrameBufferInfo());
    m_tiles->initialize();

    m_dirty = false;
}

void rtCpuRenderSystem::render(rtScene* scene)
{
    if (m_dirty)
        initialize(scene);

    m_scene->updateCaches();

    m_tiles->synchronize();
}
