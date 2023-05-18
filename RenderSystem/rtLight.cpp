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
#include "rtLight.h"
#include "Data/rtAllocator.h"
#include "rtScene.h"

rtLight::rtLight(rtScene* sc) :
    rtObject(sc)
{
    m_data = rtAllocator::allocate<rtLightType>();

    m_data->energy = 60;
    m_data->radius = .3f;
    setDecay(10e-2f);
    setElevation(10e-3f);
}

rtLight::~rtLight()
{
    if (m_data)
        rtAllocator::free<rtLightType>(m_data);
}

void rtLight::postUpdateImpl()
{
    m_data->location = {
        m_derived.location.x,
        m_derived.location.y,
        m_derived.location.z,
    };

    // Make sure the limits are adhered to..

    m_data->energy    = skMax<rtScalar>(4, m_data->energy);
    m_data->radius    = skMax<rtScalar>(10e-4f, m_data->radius);
    m_data->elevation = skMax<rtScalar>(10e-3f, m_data->elevation);
    m_data->decay     = skMax<rtScalar>(10e-2f, m_data->decay);

    const rtScalar Ew = m_data->elevation;
    const rtScalar Ed = m_data->decay * m_data->decay;

    m_data->rr   = m_data->energy * m_data->radius * Ed * Ew;
    m_data->rre4 = skSqrt(m_data->energy) * Ed;
}
