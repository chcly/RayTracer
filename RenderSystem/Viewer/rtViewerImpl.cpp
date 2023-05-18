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
#include "Math/skColor.h"
#include "Math/skRectangle.h"
#include "Utils/skLogger.h"
#define SDL_MAIN_HANDLED
#include <SDL/SDL/src/render/SDL_sysrender.h>
#include <cstring>
#include "RenderSystem/Viewer/rtViewerImpl.h"
#include "RenderSystem/rtInteractiveCamera.h"
#include "RenderSystem/rtRenderSystem.h"
#include "RenderSystem/rtTickState.h"
#include "SDL.h"
#include "Utils/skString.h"

constexpr int RenderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

/// <summary>
/// Represents one step
/// </summary>
class rtFrameRateSmoother : public rtTickState
{
private:
    rtViewerImpl* m_parent;
    skScalar      m_tickVal;

public:
    explicit rtFrameRateSmoother(rtViewerImpl* tick) :
        m_parent(tick),
        m_tickVal(0.f)
    {
    }

    ~rtFrameRateSmoother() override = default;

    void tickImpl(const skScalar& rate) override
    {
        m_parent->step();
    }

    skScalar getTick() const
    {
        return m_tickVal;
    }
};

rtViewerImpl::rtViewerImpl(SKuint32        width,
                           SKuint32        height,
                           rtRenderSystem* system,
                           rtScene*        scene) :
    m_window(nullptr),
    m_renderer(nullptr),
    m_surface(nullptr),
    m_system(system),
    m_scene(scene),
    m_camera(nullptr),
    m_frameBuffer{
        nullptr,
        (SKuint16)width,
        (SKuint16)height,
        (SKuint16)(width * 4),
        (SKuint32)(width * height * 4),
    },
    m_tick(new rtFrameRateSmoother(this)),
    m_quit(false),
    m_left(false),
    m_ctl(false),
    m_shift(false),
    m_isInteractiveCamera(false)
{
}

rtViewerImpl::~rtViewerImpl()
{
    if (m_surface)
        SDL_FreeSurface(m_surface);

    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);

    if (m_window)
        SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void rtViewerImpl::updateMode(int set)
{
    const int mode = m_system->getMode();
    if (m_shift)
    {
        if (mode & set)
            m_system->setMode(mode & ~set);
        else
            m_system->setMode(mode | set);
    }
    else
        m_system->setMode(set);

    m_renderProfile.clear();
}

void rtViewerImpl::handleKeyDown(const SDL_Event& evt)
{
    const int mod = evt.key.keysym.mod;

    rtInteractiveCamera* icam = nullptr;
    if (m_isInteractiveCamera)
        icam = (rtInteractiveCamera*)m_camera;

    if (evt.key.keysym.sym == SDLK_LCTRL || (mod & KMOD_LCTRL) != 0)
        m_ctl = true;
    if (evt.key.keysym.sym == SDLK_LSHIFT || (mod & KMOD_LSHIFT) != 0)
        m_shift = true;

    if (evt.key.keysym.sym == SDLK_1)
        updateMode(RM_COLOR_AND_LIGHT);
    else if (evt.key.keysym.sym == SDLK_2)
        updateMode(RM_DISTANCE);
    else if (evt.key.keysym.sym == SDLK_3)
        updateMode(RM_COMPUTED_NORMAL);
    else if (evt.key.keysym.sym == SDLK_4)
        updateMode(RM_OUTLINE);
    else if (evt.key.keysym.sym == SDLK_5)
        updateMode(RM_INVERT);
    else if (evt.key.keysym.sym == SDLK_6)
        updateMode(RM_AA);
    else if (evt.key.keysym.sym == SDLK_c)
    {
        if (icam)
            icam->center();
    }
    else if (evt.key.keysym.sym == SDLK_KP_7)
    {
        skScalar r;

        if (m_shift)
            r = skPiH;
        else
            r = -skPiH;

        if (icam)
        {
            icam->reset();
            icam->pivot(0, r);
        }
    }
    else if (evt.key.keysym.sym == SDLK_KP_1)
    {
        if (icam)
        {
            icam->reset();
            if (m_shift)
                icam->pivot(skPi, 0);
        }
    }
    else if (evt.key.keysym.sym == SDLK_KP_3)
    {
        skScalar r;

        if (m_shift)
            r = -skPiH;
        else
            r = skPiH;

        if (icam)
        {
            icam->reset();
            icam->pivot(r, 0);
        }
    }
}

void rtViewerImpl::handleKeyUp(const SDL_Event& evt)
{
    const int mod = evt.key.keysym.mod;

    if (evt.key.keysym.sym == SDLK_LCTRL || (mod & KMOD_LCTRL) != 0)
        m_ctl = false;
    if (evt.key.keysym.sym == SDLK_LSHIFT || (mod & KMOD_LSHIFT) != 0)
        m_shift = false;
}

void rtViewerImpl::processEvents()
{
    rtInteractiveCamera* icam = nullptr;
    if (m_isInteractiveCamera)
        icam = (rtInteractiveCamera*)m_camera;

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_KEYDOWN:
            handleKeyDown(evt);
            break;
        case SDL_KEYUP:
            handleKeyUp(evt);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (evt.button.button == SDL_BUTTON_LEFT)
            {
                SDL_CaptureMouse(SDL_TRUE);
                m_left = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (evt.button.button == SDL_BUTTON_LEFT)
            {
                SDL_CaptureMouse(SDL_FALSE);
                m_left = false;
            }
            break;
        case SDL_MOUSEMOTION:
        {
            if (icam)
            {
                const skScalar dx = skScalar(-0.01) * skScalar(evt.motion.xrel);
                const skScalar dy = skScalar(-0.01) * skScalar(evt.motion.yrel);

                if (m_left)
                {
                    if (m_shift)
                        icam->pan(2 * dx, -2 * dy);
                    else if (m_ctl)
                        icam->zoom(5 * dy);
                    else
                        icam->pivot(dx, dy);
                }
            }
            break;
        }
        case SDL_MOUSEWHEEL:
            if (icam)
                icam->zoom(evt.wheel.y > 0 ? 1.f : -1.f);
            break;
        case SDL_QUIT:
            m_quit = true;
            break;
        default:
            break;
        }
    }
}

void rtViewerImpl::renderImpl()
{
    m_system->render(m_scene);
}

void rtViewerImpl::render()
{
    SDL_LockSurface(m_surface);
    renderImpl();
    SDL_UnlockSurface(m_surface);
    SDL_UpdateWindowSurface(m_window);
}

void rtViewerImpl::run()
{
    if (m_frameBuffer.width < 256 || m_frameBuffer.height < 256)
    {
        skLogf(LD_ERROR, "Frame buffer is to small\n");
        return;
    }
    if (!m_scene)
    {
        skLogf(LD_ERROR, "No scene was supplied\n");
        return;
    }

    if (!m_camera)
    {
        skLogf(LD_ERROR, "No camera was supplied\n");
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        skLogf(LD_ERROR, "SDL initialization failed:\n\t%s\n", SDL_GetError());
        return;
    }

    m_window = SDL_CreateWindow("rtViewer",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                m_frameBuffer.width,
                                m_frameBuffer.height,
                                SDL_WINDOW_SHOWN);
    if (!m_window)
    {
        skLogf(LD_ERROR, "Failed to create window:\n\t%s\n", SDL_GetError());
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, RenderFlags);
    if (!m_renderer)
    {
        skLogf(LD_ERROR, "Failed to create renderer:\n\t%s\n", SDL_GetError());
        return;
    }

    // the main screen pixels are a copy the window surface's memory address

    m_surface            = SDL_GetWindowSurface(m_window);
    m_frameBuffer.pixels = (SKubyte*)m_surface->pixels;

    // sanity checks to ensure that the assumed data
    // is what is expected.

    SK_ASSERT(m_frameBuffer.pixels);
    SK_ASSERT(m_surface->w == m_frameBuffer.width);
    SK_ASSERT(m_surface->h == m_frameBuffer.height);
    SK_ASSERT(m_surface->pitch == m_frameBuffer.pitch);

    m_tick->initialize(60);
    m_tick->reset();
    while (!m_quit)
    {
#ifdef RT_FULL_PROFILE
        m_loopProfile.startSample();
#endif
        processEvents();
        m_tick->tick();
#ifdef RT_FULL_PROFILE
        m_loopProfile.endSample();
#endif
        updateProfile();
    }
}

void rtViewerImpl::step()
{
    m_renderProfile.startSample();
    render();
    m_renderProfile.endSample();
}

void rtViewerImpl::updateProfile() const
{
#ifdef RT_FULL_PROFILE
    SDL_Delay(1);
    SDL_SetWindowTitle(m_window,
                       skString::format("rtViewer - Render [%2.1f, %2.1f, %d] - Main [%2.1f, %2.1f, %d]",
                                        (double)m_renderProfile.getAverageFramesPerSecond(),
                                        (double)m_renderProfile.getLastFramesPerSecond(),
                                        m_renderProfile.getNumberOfSamples(),
                                        (double)m_loopProfile.getAverageFramesPerSecond(),
                                        (double)m_loopProfile.getLastFramesPerSecond(),
                                        m_loopProfile.getNumberOfSamples())
                           .c_str());
#else
    SDL_Delay(1);
    SDL_SetWindowTitle(m_window,
                       skString::format("rtViewer - %2.1f (FPS)",
                                        (double)m_renderProfile.getAverageFramesPerSecond())
                           .c_str());

#endif
}
