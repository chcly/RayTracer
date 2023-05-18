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
#include "rtObject.h"
#include "rtScene.h"

rtObject::rtObject(rtScene* scene) :
    m_flags(0),
    m_parent(nullptr),
    m_scene(scene)
{
    mark(true);
}

rtObject::~rtObject() = default;

rtObject::ObjectSet& rtObject::getChildren()
{
    return m_children;
}

rtObject* rtObject::getParent() const
{
    return m_parent;
}

void rtObject::removeChild(rtObject* node)
{
    if (node && node->m_parent == this)
    {
        m_children.erase(node);
        node->m_parent = nullptr;
    }
}

void rtObject::addChild(rtObject* node)
{
    if (node)
    {
        if (node->m_parent)
            node->m_parent->removeChild(node);
        if (!node->m_parent)
        {
            node->m_parent = this;
            m_children.insert(node);
        }
    }
}

void rtObject::setParent(rtObject* node)
{
    if (node && !m_parent)
    {
        node->update();
        node->addChild(this);
    }

    invalidate();
}

void rtObject::translate(const skVector3& dLoc)
{
    invalidate();
    if (m_parent)
        m_transform.location += dLoc;
    else
        m_transform.location += m_transform.orientation * dLoc;
}

void rtObject::translate(skScalar x, skScalar y, skScalar z)
{
    translate(skVector3(x, y, z));
}

void rtObject::translateLocal(const skVector3& dLoc)
{
    invalidate();
    m_transform.location += m_transform.orientation * dLoc;
}

void rtObject::translateParent(const skVector3& dLoc)
{
    invalidate();
    m_transform.location += dLoc;
}

void rtObject::rotate(const skScalar xDeg, const skScalar yDeg, const skScalar zDeg)
{
    invalidate();

    skQuaternion q0, q1, q2;
    q0.makeRotX(xDeg * skRPD);
    q1.makeRotY(yDeg * skRPD);
    q2.makeRotZ(zDeg * skRPD);
    skQuaternion q = q0 * q1 * q2;
    q.normalize();

    if (m_parent)
        m_transform.orientation = q * m_transform.orientation;
    else
        m_transform.orientation = m_transform.orientation * q;
    m_transform.orientation.normalize();
}

void rtObject::rotate(const skVector3& dRot)
{
    rotate(dRot.x, dRot.y, dRot.z);
}

void rtObject::rotateX(const skScalar xDeg)
{
    invalidate();

    skQuaternion q;
    q.makeRotX(xDeg * skRPD);

    if (m_parent)
        m_transform.orientation = q * m_transform.orientation;
    else
        m_transform.orientation = m_transform.orientation * q;
    m_transform.orientation.normalize();
}

void rtObject::rotateY(const skScalar yDeg)
{
    invalidate();

    skQuaternion q;
    q.makeRotY(yDeg * skRPD);

    if (m_parent)
        m_transform.orientation = q * m_transform.orientation;
    else
        m_transform.orientation = m_transform.orientation * q;
    m_transform.orientation.normalize();
}

void rtObject::rotateZ(const skScalar zDeg)
{
    invalidate();

    skQuaternion q;
    q.makeRotZ(zDeg * skRPD);
    if (m_parent)
        m_transform.orientation = q * m_transform.orientation;
    else
        m_transform.orientation = m_transform.orientation * q;

    m_transform.orientation.normalize();
}

void rtObject::setPosition(const skScalar x, const skScalar y, const skScalar z)
{
    const bool transformDirty = (m_flags & ND_DIRTY) != 0;
    mark(transformDirty || skNeq(m_transform.location.x, x) || skNeq(m_transform.location.y, y) || skNeq(m_transform.location.z, z));

    m_transform.location.x = x;
    m_transform.location.y = y;
    m_transform.location.z = z;
}

void rtObject::setPosition(const skVector3& v)
{
    setPosition(v.x, v.y, v.z);
}

void rtObject::setScale(const skScalar x, const skScalar y, const skScalar z)
{
    const bool transformDirty = (m_flags & ND_DIRTY) != 0;
    mark(transformDirty || skNeq(m_transform.scale.x, x) || skNeq(m_transform.scale.y, y) || skNeq(m_transform.scale.z, z));

    m_transform.scale.x = x;
    m_transform.scale.y = y;
    m_transform.scale.z = z;
}

void rtObject::setScale(const skVector3& v)
{
    setScale(v.x, v.y, v.z);
}

void rtObject::setOrientation(const skQuaternion& rot)
{
    const bool transformDirty = (m_flags & ND_DIRTY) != 0;

    mark(transformDirty || rot != m_transform.orientation);

    m_transform.orientation = rot;
    m_transform.orientation.normalize();
}

void rtObject::setOrientation(const skScalar xRad, const skScalar yRad, const skScalar zRad)
{
    const skQuaternion q(xRad, yRad, zRad);
    const bool         transformDirty = (m_flags & ND_DIRTY) != 0;

    mark(transformDirty || q != m_transform.orientation);

    m_transform.orientation = q;
    m_transform.orientation.normalize();
}

void rtObject::update()
{
    if (m_flags & ND_ROOT)
        return;

    if (isDirty())
    {
        if (updateImpl())
        {
            postUpdateImpl();
            return;
        }

        if (m_parent)
        {
            m_parent->update();
            m_derived.orientation = m_parent->m_derived.orientation * m_transform.orientation;
            m_derived.scale       = m_parent->m_derived.scale * m_transform.scale;
            m_derived.orientation.normalize();

            m_derived.location = m_parent->m_derived.orientation * (m_parent->m_derived.scale * m_transform.location);
            m_derived.location += m_parent->m_derived.location;
        }
        else
        {
            m_derived = rtTransformState(m_transform);
        }

        postUpdateImpl();
        m_flags &= ~ND_DIRTY;
    }
}

bool rtObject::isDirty() const
{
    const bool transformDirty = (m_flags & ND_DIRTY) != 0;
    if (m_parent)
        return transformDirty || m_parent->isDirty();
    return transformDirty;
}

void rtObject::invalidate()
{
    if (!(m_flags & ND_DIRTY))
    {
        if (m_scene)
            m_scene->pushOutOfDate(this);
    }

    addFlag(ND_DIRTY);

    if (!m_children.empty())
    {
        ObjectSet::Iterator it = m_children.iterator();
        while (it.hasMoreElements())
            it.getNext().first->invalidate();
    }
}

void rtObject::mark(const bool expression)
{
    if (expression)
        invalidate();
}
