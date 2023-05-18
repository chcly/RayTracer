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
#include "rtMesh.h"
#include "Cpu/rtCpuMath.h"

/// <summary>
/// Mesh sort
/// </summary>
class rtMeshPrivate
{
public:
    typedef skHashTable<SKuint32, SKuint32> Table;

private:
    rtMesh::Indices  m_indices;
    Table            m_lookup;
    rtMesh::Vertices m_vertices;

public:
    rtMeshPrivate() = default;

    ~rtMeshPrivate() = default;

    void clear()
    {
        m_indices.clear();
        m_lookup.clear();
        m_vertices.clear();
    }

    void freeLookup()
    {
        m_lookup.clear();
    }

    rtMesh::Indices& getIndices()
    {
        return m_indices;
    }

    rtMesh::Vertices& getVertices()
    {
        return m_vertices;
    }

    static bool equals(const skVector3& a, const skVector3& b)
    {
        bool res = true;
        res      = res && skEq(a.x, b.x);
        res      = res && skEq(a.y, b.y);
        res      = res && skEq(a.z, b.z);
        return res;
    }

    /// <summary>
    /// Tracks a new indexed vertex based on an old index.
    /// </summary>
    /// <param name="index">The old index.</param>
    /// <param name="v">The vertex to store</param>
    /// <returns>Returns SK_NPOS32 if the total number of indices exceeds
    /// 65,535, otherwise it returns the new index
    /// </returns>
    SKuint32 addVertex(const SKuint32& index, const skVector3& v)
    {
        if (index >= 0xFFFF)
            return -1;

        SKuint32 pos = (SKuint32)m_lookup.find(index);
        if (pos != SK_NPOS32)
        {
            pos = m_lookup.at(pos);
            if (pos < m_vertices.size() && !equals(m_vertices[pos], v))
                pos = -1;
        }

        if (pos == SK_NPOS32)
        {
            m_vertices.push_back(v);
            m_lookup.insert(index, (SKuint32)pos);
        }
        return pos;
    }

    void addTriangle(SKuint32         i1,
                     SKuint32         i2,
                     SKuint32         i3,
                     const skVector3& v1,
                     const skVector3& v2,
                     const skVector3& v3)
    {
        i1 = addVertex(i1, v1);
        if (i1 != SK_NPOS32)
            m_indices.push_back(i1);
        i2 = addVertex(i2, v2);
        if (i2 != SK_NPOS32)
            m_indices.push_back(i2);
        i3 = addVertex(i3, v3);
        if (i3 != SK_NPOS32)
            m_indices.push_back(i3);
    }
};

rtMesh::rtMesh(rtScene* sc) :
    rtBvObject(sc),
    m_private(new rtMeshPrivate())
{
    m_data->type = RT_AO_SHAPE_MESH;
}

rtMesh::~rtMesh()
{
    delete m_private;
}

void rtMesh::beginAddTriangles() const
{
    rtCpuScalarZero(m_data->bounds.bMin, 3);
    rtCpuScalarZero(m_data->bounds.bMax, 3);

    m_private->clear();
}

void rtMesh::addTriangle(const SKuint32&  i0,
                         const SKuint32&  i1,
                         const SKuint32&  i2,
                         const skVector3& v0,
                         const skVector3& v1,
                         const skVector3& v2) const
{
    m_private->addTriangle(i0, i1, i2, v0, v1, v2);

    rtCpuCompareBounding(&m_data->bounds.bMin[0],
                         &m_data->bounds.bMax[0],
                         v0);
    rtCpuCompareBounding(&m_data->bounds.bMin[0],
                         &m_data->bounds.bMax[0],
                         v1);
    rtCpuCompareBounding(&m_data->bounds.bMin[0],
                         &m_data->bounds.bMax[0],
                         v2);
}

void rtMesh::endAddTriangles() const
{
    m_private->freeLookup();
}

rtMesh::Indices& rtMesh::getIndices() const
{
    return m_private->getIndices();
}

rtMesh::Vertices& rtMesh::getVertices() const
{
    return m_private->getVertices();
}

void rtMesh::postUpdateImpl()
{
    m_derivedBoundingBox = getBoundingBox();
    m_derivedBoundingBox.translate(m_derived.location);

    m_derivedBoundingSphere = skBoundingSphere{
        m_derivedBoundingBox.center(),
        m_derivedBoundingBox.halfLength() * skScalar(0.5),
    };

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

    m_data->bounds.bMin[0] = m_derivedBoundingBox.bMin[0];
    m_data->bounds.bMin[1] = m_derivedBoundingBox.bMin[1];
    m_data->bounds.bMin[2] = m_derivedBoundingBox.bMin[2];

    m_data->bounds.bMax[0] = m_derivedBoundingBox.bMax[0];
    m_data->bounds.bMax[1] = m_derivedBoundingBox.bMax[1];
    m_data->bounds.bMax[2] = m_derivedBoundingBox.bMax[2];
}
