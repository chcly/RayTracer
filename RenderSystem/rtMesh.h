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
#ifndef _rtMesh_h_
#define _rtMesh_h_

#include "Math/skBoundingBox.h"
#include "Math/skBoundingSphere.h"
#include "rtBvObject.h"

class rtMeshPrivate;

/// <summary>
///
/// </summary>
class rtMesh final : public rtBvObject
{
public:
    typedef skArray<SKuint32>  Indices;
    typedef skArray<skVector3> Vertices;

private:
    friend rtMeshPrivate;

    rtMeshPrivate*   m_private;
    skBoundingBox    m_derivedBoundingBox;
    skBoundingSphere m_derivedBoundingSphere;

    void postUpdateImpl() override;

public:
    /// <summary>
    ///
    /// </summary>
    /// <param name="sc"></param>
    explicit rtMesh(rtScene* sc);
    ~rtMesh() override;

    /// <summary>
    ///
    /// </summary>
    void beginAddTriangles() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="i0"></param>
    /// <param name="i1"></param>
    /// <param name="i2"></param>
    /// <param name="v0"></param>
    /// <param name="v1"></param>
    /// <param name="v2"></param>
    void addTriangle(const SKuint32&  i0,
                     const SKuint32&  i1,
                     const SKuint32&  i2,
                     const skVector3& v0,
                     const skVector3& v1,
                     const skVector3& v2) const;

    /// <summary>
    ///
    /// </summary>
    void endAddTriangles() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    Indices& getIndices() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    Vertices& getVertices() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    skBoundingBox getBoundingBox() const override;

    /// <summary>
    /// Returns a sphere that aligns with smallest volume
    /// of the mesh.
    /// </summary>
    skBoundingSphere getDerivedBoundingSphere() const;

    /// <summary>
    /// Returns a sphere that aligns with largest volume
    /// of the mesh.
    /// </summary>
    skBoundingSphere getFullBoundingSphere() const;

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    int getType() override;
};

/*! @} */

SK_INLINE skBoundingBox rtMesh::getBoundingBox() const
{
    return {m_data->bounds.bMin, m_data->bounds.bMax};
}

SK_INLINE skBoundingSphere rtMesh::getDerivedBoundingSphere() const
{
    return m_derivedBoundingSphere;
}

SK_INLINE skBoundingSphere rtMesh::getFullBoundingSphere() const
{
    return skBoundingSphere{
        m_derivedBoundingBox.center(),
        m_derivedBoundingBox.halfLength(),
    };
}

SK_INLINE int rtMesh::getType()
{
    return RT_AO_SHAPE_MESH;
}

#endif  //_rtMesh_h_
