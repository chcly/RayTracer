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
#include "rtInteractiveCamera.h"

const skScalar kIn = 0.01f;
const skScalar kS  = 4.0f;

rtInteractiveCamera::rtInteractiveCamera(rtScene* sc) :
    rtCamera(sc),
    m_camOri(skQuaternion(skPiH, 0, 0)),
    m_camOffs(1, 0, 0, 0),
    m_fixedDistance(10)
{
    center();
}

rtInteractiveCamera::~rtInteractiveCamera() = default;

void rtInteractiveCamera::setFixedDistance(skScalar units)
{
    invalidate();
    m_fixedDistance        = units;
    m_transform.location.y = -m_fixedDistance;
}

void rtInteractiveCamera::setDefaultCameraOrientation(const skQuaternion& cOri)
{
    m_camOri = cOri.inverse();
    m_camOffs.normalize();
}

void rtInteractiveCamera::reset()
{
    invalidate();
    //center();
    m_camOri                = skQuaternion(skPiH, 0, 0);
    m_camOffs               = skQuaternion::Identity;
    m_fixedDistance         = 10;
    m_transform.orientation = skQuaternion::Identity;
}

void rtInteractiveCamera::center()
{
    invalidate();
    m_transform.location = skVector3(0, -m_fixedDistance, 0);
    m_vLoc               = skVector3::Zero;
}

void rtInteractiveCamera::pivot(skScalar dx, skScalar dy)
{
    invalidate();
    skQuaternion q0, q1;

    q0.makeRotZ(dx);
    q0.normalize();
    m_transform.orientation = q0 * m_transform.orientation;

    q1.makeRotX(dy);
    q1.normalize();
    m_transform.orientation = m_transform.orientation * q1;
}

void rtInteractiveCamera::pan(skScalar dx, skScalar dy)
{
    invalidate();
    skScalar sf = m_derived.location.distance2(m_vLoc);

    sf = skSqrt(sf) / kS;
    const skVector3 dLoc(sf * dx, 0, sf * dy);
    m_vLoc += m_transform.orientation * dLoc;
}

void rtInteractiveCamera::zoom(skScalar scale)
{
    invalidate();
    const skScalar sf = m_derived.location.distance2(m_vLoc);
    const skScalar dx = skSqrt(sf) / kS * scale;
    const skScalar dy = -m_transform.location.y;

    m_transform.location.y += dx;
    if (dy - kIn <= 0)
        m_transform.location.y += dy - kIn;
}

bool rtInteractiveCamera::updateImpl()
{
    m_derived.orientation = m_transform.orientation * m_camOffs * m_camOri;
    m_derived.orientation.normalize();

    m_derived.location = m_transform.orientation * m_camOffs * m_transform.location;
    m_derived.location += m_vLoc;

    m_flags &= ~ND_DIRTY;
    return true;
}
