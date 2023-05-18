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
#ifndef _skScreenTransform_h_
#define _skScreenTransform_h_

#include "skRectangle.h"
#include "skVector2.h"

/// <summary>
/// Utility class for handling screen transforms like zoom, and pan
/// </summary>
class skScreenTransform
{
private:
    skVector2   m_origin;         // (0,0) of the coordinate system
    skVector2   m_center;         // offset from viewport center
    skRectangle m_viewport;       // fixed bounds of the viewport ([x] [x] [L] [L])
    skVector2   m_extent;         // storage to compute the change in m_viewport.getSize()
    skScalar    m_zoom;           // computed viewport change
    skScalar    m_scale;          // the current amount to scale the viewport by
    skVector2   m_scaleLimit;     // the range of the scale function
    skVector2   m_initialOrigin;  // the 'home' origin

public:
    skScreenTransform() :
        m_zoom(1),
        m_scale(1)
    {
        m_scaleLimit = skVector2::Unit;
    }

    void zoom(const skScalar factor, bool negate)
    {
        m_scale += factor * m_zoom * (negate ? skScalar(-1) : skScalar(1));

        if (m_scale < m_scaleLimit.x)
            m_scale = m_scaleLimit.x;
        if (m_scale > m_scaleLimit.y)
            m_scale = m_scaleLimit.y;

        if (m_scale > 0)
        {
            m_extent = m_viewport.getSize() + m_scale;
            m_zoom   = m_extent.x / m_viewport.width;
            if (m_zoom < 1)
                m_zoom = 1;
        }
    }


    void zoomLarge(const skScalar factor, bool negate)
    {
        m_scale += factor * m_zoom * (negate ? skScalar(-1) : skScalar(1));

        m_extent = m_viewport.getSize() + m_scale;
        m_zoom   = m_extent.x / m_viewport.width;
        if (skEqT(m_zoom, 0, SK_EPSILON))
            m_zoom = SK_EPSILON;
    }

    void pan(const skScalar px, const skScalar py)
    {
        m_origin.x += px * m_zoom;
        m_origin.y += py * m_zoom;
    }

    void setOrigin(const skScalar px, const skScalar py)
    {
        m_origin.x = px;
        m_origin.y = py;
    }

    void reset()
    {
        m_scale  = 1;
        m_origin = m_initialOrigin;
        m_extent = m_viewport.getSize();
        m_center = m_extent / 2;
        if (m_viewport.width > 0)
            m_zoom = m_extent.x / m_viewport.width;
        else
            m_zoom = 1;
    }

    SK_INLINE const skScalar& getZoom() const
    {
        return m_zoom;
    }

    SK_INLINE const skScalar& getScale() const
    {
        return m_scale;
    }

    SK_INLINE skScalar xOffs() const
    {
        return (-m_center.x + m_origin.x + m_extent.x / skScalar(2)) / m_zoom;
    }

    SK_INLINE skScalar yOffs() const
    {
        return (-m_center.y + m_origin.y + m_extent.y / skScalar(2)) / m_zoom;
    }

    SK_INLINE void xToView(skScalar& x) const
    {
        x = x / m_zoom + xOffs();
    }

    skScalar getViewX(const skScalar& x) const
    {
        skScalar xCpy = x;
        xToView(xCpy);
        return xCpy;
    }

    SK_INLINE void yToView(skScalar& y) const
    {
        y = y / m_zoom + yOffs();
    }

    skScalar getViewY(const skScalar& y) const
    {
        skScalar yCpy = y;
        yToView(yCpy);
        return yCpy;
    }

    SK_INLINE void xToScreen(skScalar& x) const
    {
        x = (x - xOffs()) * m_zoom;
    }

    skScalar getScreenX(const skScalar& x) const
    {
        skScalar xCpy = x;
        xToScreen(xCpy);
        return xCpy;
    }

    SK_INLINE void yToScreen(skScalar& y) const
    {
        y = (y - yOffs()) * m_zoom;
    }

    skScalar getScreenY(const skScalar& y) const
    {
        skScalar xCpy = y;
        yToScreen(xCpy);
        return xCpy;
    }

    void pointToScreen(skVector2& pt) const
    {
        xToScreen(pt.x);
        yToScreen(pt.y);
    }

    void pointToView(skVector2& pt) const
    {
        xToView(pt.x);
        yToView(pt.y);
    }

    skVector2 getOffset() const
    {
        return skVector2(xOffs(), yOffs());
    }

    SK_INLINE const skVector2& getOrigin() const
    {
        return m_origin;
    }

    SK_INLINE const skVector2& getExtent() const
    {
        return m_extent;
    }

    SK_INLINE const skVector2& getCenter() const
    {
        return m_center;
    }

    SK_INLINE const skRectangle& getViewport() const
    {
        return m_viewport;
    }

    SK_INLINE const skScalar& viewportLeft() const
    {
        return m_viewport.x;
    }

    SK_INLINE const skScalar& viewportTop() const
    {
        return m_viewport.y;
    }

    SK_INLINE skScalar viewportRight() const
    {
        return m_viewport.getRight();
    }

    SK_INLINE skScalar viewportBottom() const
    {
        return m_viewport.getBottom();
    }

    SK_INLINE const skScalar& viewportWidth() const
    {
        return m_viewport.width;
    }

    SK_INLINE const skScalar& viewportHeight() const
    {
        return m_viewport.height;
    }

    void setScaleLimit(const skScalar& lMin, const skScalar& lMax)
    {
        m_scaleLimit.x = lMin;
        m_scaleLimit.y = lMax;
    }

    void setInitialOrigin(const skScalar& oX, const skScalar& oY)
    {
        m_initialOrigin.x = oX;
        m_initialOrigin.y = oY;
    }

    void setViewport(const skRectangle& vp)
    {
        m_viewport = vp;
    }

    void setViewport(const skScalar& x,
                     const skScalar& y,
                     const skScalar& width,
                     const skScalar& height)
    {
        m_viewport.x      = x;
        m_viewport.y      = y;
        m_viewport.width  = width;
        m_viewport.height = height;
    }

    bool isInViewport(const skScalar& x1, const skScalar& y1, const skScalar& x2, const skScalar& y2) const
    {
        skRectangle r1, r2;
        r1.setSize(m_viewport.width, m_viewport.height);
        r2.setCorners(x1, y1, x2, y2);
        return r1.contains(r2);
    }
};

#endif  //_skScreenTransform_h_
