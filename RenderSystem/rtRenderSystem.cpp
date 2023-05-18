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
#include "RenderSystem/rtRenderSystem.h"
#include "Math/rtColor.h"
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtScene.h"
#include "RenderSystem/rtTarget.h"

rtRenderSystem::rtRenderSystem() :
    m_target(nullptr),
    m_scene(nullptr),
    m_limit(0, SK_INFINITY),
    m_camera(nullptr),
    m_mode(RM_COLOR_AND_LIGHT),
    m_dirty(true)
{
}

void rtRenderSystem::setTarget(rtTarget* target)
{
    m_target = target;
    updatePixelOffset();
}

void rtRenderSystem::updatePixelOffset()
{
    const skScalar iW = skScalar(m_target->getWidth());
    const skScalar iH = skScalar(m_target->getHeight());

    if (m_camera)
    {
        m_limit.x = m_camera->getNear();
        m_limit.y = m_camera->getFar();

        m_iPixelOffset.x = skScalar(1.0) / iW;
        m_iPixelOffset.y = skScalar(1.0) / iH;

        m_iPixelOffset.z = iW / iH;
        m_iPixelOffset.w = -skTan(skRPD * m_camera->getFieldOfViewAngle() * 0.5f);

        m_camera->setAspect(m_iPixelOffset.z);
    }
    else
    {
        m_iPixelOffset.x = skScalar(1.0) / iW;
        m_iPixelOffset.y = skScalar(1.0) / iH;
    }
}
