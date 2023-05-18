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
#include "rtSphere.h"
#include "Cpu/rtCpuMath.h"
#include "Data/rtAllocator.h"

rtSphere::rtSphere(rtScene* sc) :
    rtBvObject(sc)
{
    m_sphere = rtAllocator::allocate<rtSphereVolume>();

    m_sphere->center = {0, 0, 0};
    m_sphere->radius = 1;

    m_data->type = RT_AO_SHAPE_SPHERE;
    m_data->bounds.data = m_sphere;
}

rtSphere::~rtSphere()
{
    rtAllocator::free<rtSphereVolume>(m_sphere);
}

void rtSphere::postUpdateImpl()
{
    rtCpuScalarCopy(&m_data->location.x, &m_derived.location.x, 3);

    m_sphere->center = m_data->location;
    rtVector3 v0     = rtCpuVec3Sub(m_sphere->center, m_sphere->radius);
    rtVector3 v1 = rtCpuVec3Add(m_sphere->center, m_sphere->radius);

    m_data->type = RT_AO_SHAPE_SPHERE;
    rtCpuScalarCopy(m_data->bounds.bMin, &v0.x, 3);
    rtCpuScalarCopy(m_data->bounds.bMax, &v1.x, 3);

    // dispatchEvent(RT_UT_POST_NODE_UPDATE);
}
