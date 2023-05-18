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
/*! \addtogroup FrontEnd
 * @{
 */

#ifndef _rtNode_h_
#define _rtNode_h_

#include "Math/skQuaternion.h"
#include "Math/skVector3.h"
#include "Utils/skMap.h"
#include "rtClientObject.h"

class rtClientObject;
class rtScene;

/// <summary>
/// rtTransformState holds location, rotation and scale
/// information for rtNode objects.
/// </summary>
struct rtTransformState
{
    rtTransformState() :
        location(0, 0, 0),
        orientation(1, 0, 0, 0),
        scale(1, 1, 1)
    {
    }

    explicit rtTransformState(const rtTransformState& rhs) = default;

    skVector3    location;
    skQuaternion orientation;
    skVector3    scale;
};

/// <summary>
///
/// </summary>
class rtObject
{
public:
    typedef skHashSet<rtObject*> ObjectSet;

    enum Flags
    {
        ND_DIRTY = 1 << 0,
        ND_ROOT  = 1 << 1
    };

protected:
    rtTransformState m_transform;
    rtTransformState m_derived;

    ObjectSet m_children;
    SKint32   m_flags;
    rtObject* m_parent;
    rtScene*  m_scene;
    int       m_type;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="scene"></param>
    explicit rtObject(rtScene* scene);

    virtual ~rtObject();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    ObjectSet& getChildren();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtObject* getParent() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="flag"></param>
    void addFlag(SKint32 flag);

    /// <summary>
    ///
    /// </summary>
    /// <param name="flag"></param>
    void clearFlag(SKint32 flag);

    /// <summary>
    ///
    /// </summary>
    /// <param name="node"></param>
    void removeChild(rtObject* node);

    /// <summary>
    ///
    /// </summary>
    /// <param name="node"></param>
    void addChild(rtObject* node);

    /// <summary>
    ///
    /// </summary>
    /// <param name="node"></param>
    void setParent(rtObject* node);

    /// <summary>
    ///
    /// </summary>
    /// <param name="dLoc"></param>
    void translateLocal(const skVector3& dLoc);

    /// <summary>
    ///
    /// </summary>
    /// <param name="dLoc"></param>
    void translateParent(const skVector3& dLoc);

    /// <summary>
    ///
    /// </summary>
    /// <param name="dLoc"></param>
    void translate(const skVector3& dLoc);

    /// <summary>
    ///
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    void translate(skScalar x, skScalar y, skScalar z);

    /// <summary>
    ///
    /// </summary>
    /// <param name="xDeg"></param>
    void rotateX(skScalar xDeg);

    /// <summary>
    ///
    /// </summary>
    /// <param name="yDeg"></param>
    void rotateY(skScalar yDeg);

    /// <summary>
    ///
    /// </summary>
    /// <param name="zDeg"></param>
    void rotateZ(skScalar zDeg);

    /// <summary>
    ///
    /// </summary>
    /// <param name="xDeg"></param>
    /// <param name="yDeg"></param>
    /// <param name="zDeg"></param>
    void rotate(skScalar xDeg, skScalar yDeg, skScalar zDeg);

    /// <summary>
    ///
    /// </summary>
    /// <param name="dRot"></param>
    void rotate(const skVector3& dRot);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skVector3& getDerivedPosition() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skQuaternion& getDerivedOrientation() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skVector3& getDerivedScale() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skVector3& getPosition();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skQuaternion& getOrientation();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skVector3& getScale();

    /// <summary>
    ///
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    void setPosition(skScalar x, skScalar y, skScalar z);

    /// <summary>
    ///
    /// </summary>
    /// <param name="v"></param>
    void setPosition(const skVector3& v);

    /// <summary>
    ///
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    void setScale(skScalar x, skScalar y, skScalar z);

    /// <summary>
    ///
    /// </summary>
    /// <param name="v"></param>
    void setScale(const skVector3& v);

    /// <summary>
    ///
    /// </summary>
    /// <param name="rot"></param>
    void setOrientation(const skQuaternion& rot);

    /// <summary>
    ///
    /// </summary>
    /// <param name="xRad"></param>
    /// <param name="yRad"></param>
    /// <param name="zRad"></param>
    void setOrientation(skScalar xRad, skScalar yRad, skScalar zRad);

    /// <summary>
    ///
    /// </summary>
    void update();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    virtual int getType();

protected:
    bool isDirty() const;

    void invalidate();

    virtual bool updateImpl()
    {
        return false;
    }

    virtual void postUpdateImpl()
    {
    }

    void mark(bool expression);
};

/*! @} */

SK_INLINE void rtObject::addFlag(SKint32 flag)
{
    m_flags |= flag;
}

SK_INLINE void rtObject::clearFlag(SKint32 flag)
{
    m_flags &= ~flag;
}

SK_INLINE const skVector3& rtObject::getDerivedPosition() const
{
    return m_derived.location;
}

SK_INLINE const skQuaternion& rtObject::getDerivedOrientation() const
{
    return m_derived.orientation;
}

SK_INLINE const skVector3& rtObject::getDerivedScale() const
{
    return m_derived.scale;
}

SK_INLINE skVector3& rtObject::getPosition()
{
    return m_transform.location;
}

SK_INLINE skQuaternion& rtObject::getOrientation()
{
    return m_transform.orientation;
}

SK_INLINE skVector3& rtObject::getScale()
{
    return m_transform.scale;
}

SK_INLINE int rtObject::getType()
{
    return RT_AO_NODE;
}

#endif  //_rtNode_h_
