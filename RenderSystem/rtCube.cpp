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
#include "rtCube.h"

rtCube::rtCube(rtScene* sc) :
    rtBvObject(sc),
    m_extent(1)
{
}

rtCube::~rtCube() = default;

void rtCube::postUpdateImpl()
{
    const skVector3 hs = m_derived.scale * 0.5f * m_extent;

    const skVector3 v1 = m_derived.orientation * (m_derived.location - hs);
    const skVector3 v2 = m_derived.orientation * (m_derived.location + hs);


    m_data->type = getType();

    m_data->bounds.bMin[0] = v1.x;
    m_data->bounds.bMin[1] = v1.y;
    m_data->bounds.bMin[2] = v1.z;

    m_data->bounds.bMax[0] = v2.x;
    m_data->bounds.bMax[1] = v2.y;
    m_data->bounds.bMax[2] = v2.z;
}
