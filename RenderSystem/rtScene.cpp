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
#include "RenderSystem/rtScene.h"
#include "RenderSystem/Data/rtAllocator.h"
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtLight.h"
#include "RenderSystem/rtMesh.h"
#include "RenderSystem/rtObject.h"
#include "RenderSystem/rtRenderSystem.h"

rtScene::rtScene()
{
    m_data              = rtAllocator::allocate<rtSceneType>();
    m_data->flags       = RM_COLOR_AND_LIGHT;
    m_data->camera      = nullptr;
    m_data->horizon     = {1, 0, 0};
    m_data->zenith      = {0, 0, 0};
}

rtScene::~rtScene()
{
    if (m_data)
    {
        rtAllocator::free<rtSceneType>(m_data);
        m_data = nullptr;
    }

    for (rtObject* node : m_objects)
        delete node;

    m_lights.clear();
    m_cameras.clear();
    m_meshes.clear();
}

void rtScene::addBoundingObject(rtBvObject* bvo)
{
    if (bvo)
    {
        getData().objects.push_back(bvo->getPtr());
        m_objects.push_back((rtObject*)bvo);
    }
}

void rtScene::addCamera(rtCamera* cam)
{
    if (cam)
    {
        m_cameras.push_back(cam);
        m_objects.push_back((rtObject*)cam);
    }
}

void rtScene::addMesh(rtMesh* mesh)
{
    if (mesh)
    {
        addBoundingObject(mesh);
        m_meshes.push_back(mesh);
    }
}

void rtScene::addLight(rtLight* light)
{
    if (light)
    {
        getData().lights.push_back(light->getPtr());

        m_objects.push_back((rtObject*)light);
    }
}

void rtScene::updateCaches()
{
    if (!m_outOfDateTransforms.empty())
    {
        for (rtObject* element : m_outOfDateTransforms)
            element->update();

        m_outOfDateTransforms.resizeFast(0);
    }
}

void rtScene::pushOutOfDate(rtObject* node)
{
    m_outOfDateTransforms.push_back(node);
}
