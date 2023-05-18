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
#include "rtPlane.h"


#include "rtCamera.h"
#include "rtScene.h"
#include "Cpu/rtCpuMath.h"

rtPlane::rtPlane(rtScene* sc) :
    rtBvObject(sc)
{
    m_plane.p0 = skVector3::Zero;
    m_plane.n  = skVector3::UnitZ;
}

rtPlane::~rtPlane() = default;

void rtPlane::postUpdateImpl()
{
    m_derivedPlane.p0 = m_plane.p0 + m_derived.location;
    m_derivedPlane.n  = m_derived.orientation * m_plane.n;

    const skScalar ks = m_scene->getCameras().at(0)->getFar();
    const skScalar kS = SK_EPSILON;
    skVector3      p1 = m_plane.p0;

    const skScalar ma = m_derivedPlane.n.max3();
    if (skEq(ma, m_derivedPlane.n.x))
    {
        p1.y += ks;
        p1.z += ks;
        p1.x = kS;
    }
    else if (skEq(ma, m_derivedPlane.n.y))
    {
        p1.x += ks;
        p1.z += ks;
        p1.y = kS;
    }
    else
    {
        p1.x += ks;
        p1.y += ks;
        p1.z = kS;
    }


    rtCpuScalarCopy(m_data->bounds.bMin, (m_plane.p0 - p1).ptr(), 3);
    rtCpuScalarCopy(m_data->bounds.bMax, (m_plane.p0 + p1).ptr(), 3);

    ///dispatchEvent(RT_UT_POST_NODE_UPDATE);
}
