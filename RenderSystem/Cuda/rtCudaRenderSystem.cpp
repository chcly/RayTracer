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
#include "RenderSystem/Cuda/rtCudaRenderSystem.h"
#include "RenderSystem/Cuda/rtCudaHostMath.h"
#include "RenderSystem/Cuda/rtCudaUtils.h"
#include "RenderSystem/rtCamera.h"


rtCudaRenderSystem::rtCudaRenderSystem() :
    m_frameBuffer{nullptr, 0, 0, 0},
    m_cudaTarget(nullptr)
{
}

rtCudaRenderSystem ::~rtCudaRenderSystem()
{
    if (m_cudaTarget)
    {
        m_cudaTarget->pixels = nullptr;
        rtCudaFreeTarget(m_cudaTarget);
    }
    rtCudaFreeFrameBuffer(&m_frameBuffer);
}

void rtCudaRenderSystem::initialize(rtScene* scene)
{
    if (!m_dirty)
        return;
    
    if (!m_target)
    {
        printf("No target was specified.\n");
        return;
    }

    if (m_cudaTarget)
    {
        m_cudaTarget->pixels = nullptr;
        rtCudaFreeTarget(m_cudaTarget);
    }
    if (m_frameBuffer.memory != nullptr)
        rtCudaFreeFrameBuffer(&m_frameBuffer);

    const rtFrameBufferInfo& fbi = m_target->getFrameBufferInfo();
    rtCudaAllocateFrameBuffer(&m_frameBuffer, fbi.maxSize, 512);

    if (!m_frameBuffer.memory)
    {
        // cannot render
        return;
    }

    m_cudaTarget             = rtCudaAllocateTarget();
    m_cudaTarget->dimensions = fbi.width * fbi.height;
    m_cudaTarget->length     = fbi.maxSize;
    m_cudaTarget->width      = fbi.width;
    m_cudaTarget->height     = fbi.height;
    m_cudaTarget->pitch      = fbi.pitch;
    m_cudaTarget->pixels     = (uint8_t*)m_frameBuffer.memory;

    m_scene = scene;
    if (!m_scene)
    {
        printf("Invalid supplied scene.\n");
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
    sc->flags  = m_mode;

    m_dirty = false;
}

void rtCudaRenderSystem::render(rtScene* scene)
{
    if (m_dirty)
        initialize(scene);

    if (m_dirty)
    {
        // cannot render
        return;
    }

    SK_ASSERT(m_frameBuffer.memory);
    SK_ASSERT(m_scene);

    if (m_frameBuffer.memory)
    {
        m_scene->updateCaches();

        rtCudaKernelMain(&m_frameBuffer, m_cudaTarget, m_scene->getPtr());
        rtCudaSwapBuffers(m_target->getFrameBufferInfo().pixels, m_cudaTarget);
    }
}
