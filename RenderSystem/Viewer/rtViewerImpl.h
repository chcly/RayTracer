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
#ifndef _rtViewerImpl_h_
#define _rtViewerImpl_h_

#include "RenderSystem/rtInteractiveCamera.h"
#include "RenderSystem/rtRenderSystem.h"
#include "RenderSystem/rtTarget.h"
#include "RenderSystem/rtTimeProfile.h"

class rtTickState;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;
union SDL_Event;

/// <summary>
///
/// </summary>
class rtViewerImpl : public rtTarget
{
private:
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface*  m_surface;
    rtTimeProfile m_renderProfile;
#ifdef RT_FULL_PROFILE
    rtTimeProfile m_loopProfile;
#endif

    void processEvents();

    void render();

    virtual void renderImpl();

    void updateMode(int set);

    void handleKeyDown(const SDL_Event& evt);
    void handleKeyUp(const SDL_Event& evt);

protected:
    rtRenderSystem*   m_system;
    rtScene*          m_scene;
    rtCamera*         m_camera;
    rtFrameBufferInfo m_frameBuffer;
    rtTickState*      m_tick;
    bool              m_quit;
    bool              m_left;
    bool              m_ctl;
    bool              m_shift;
    bool              m_isInteractiveCamera;

    void updateProfile() const;
    void run();

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <param name="system"></param>
    /// <param name="scene"></param>
    rtViewerImpl(SKuint32 width, SKuint32 height, rtRenderSystem* system = nullptr, rtScene* scene = nullptr);
    ~rtViewerImpl() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtFrameBufferInfo& getFrameBufferInfo() override;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    SKuint32 getWidth() override;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    SKuint32 getHeight() override;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    SKuint32 getPitch() override;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getSizeInBytes() override;

    /// <summary>
    ///
    /// </summary>
    void step();
};

SK_INLINE const rtFrameBufferInfo& rtViewerImpl::getFrameBufferInfo()
{
    return m_frameBuffer;
}

SK_INLINE SKuint32 rtViewerImpl::getWidth()
{
    return m_frameBuffer.width;
}

SK_INLINE SKuint32 rtViewerImpl::getHeight()
{
    return m_frameBuffer.height;
}

SK_INLINE SKuint32 rtViewerImpl::getPitch()
{
    return m_frameBuffer.pitch;
}

SK_INLINE SKuint32 rtViewerImpl::getSizeInBytes()
{
    return m_frameBuffer.maxSize;
}

#endif  //_rtViewerImpl_h_
