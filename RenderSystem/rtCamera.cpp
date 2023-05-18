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
#include "RenderSystem/rtCamera.h"

#include "rtScene.h"
#include "RenderSystem/Data/rtAllocator.h"

rtCamera::rtCamera(rtScene* sc) :
    rtObject(sc),
    m_data(nullptr)
{
    m_data = rtAllocator::allocate<rtCameraType>();
    m_data->location = {};
    m_data->rotation = {};
    m_data->limits   = {};
    m_data->offset   = {};
    m_data->aspect   = 0;
    m_data->fovY     = 0;

}

rtCamera::~rtCamera()
{
    rtAllocator::free<rtCameraType>(m_data);
}

void rtCamera::setFieldOfViewAngle(skScalar theta)
{
    getData().fovY = skClampf(theta, 1, 360);
}

void rtCamera::setNear(const skScalar zNear)
{
    getData().limits.x = zNear;
}

void rtCamera::setFar(skScalar zFar)
{
    getData().limits.y = zFar;
}

void rtCamera::setAspect(skScalar aspect)
{
    getData().aspect = aspect;
}

void rtCamera::postUpdateImpl()
{
    getData().location = {
        m_derived.location.x,
        m_derived.location.y,
        m_derived.location.z,
    };
    getData().rotation = {
        m_derived.orientation.x,
        m_derived.orientation.y,
        m_derived.orientation.z,
        m_derived.orientation.w,
    };
}
