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
#ifndef _rtScene_h_
#define _rtScene_h_

#include "Math/skColor.h"
#include "RenderSystem/Data/rtSceneType.h"
#include "RenderSystem/rtCommon.h"
#include "RenderSystem/rtMesh.h"
#include "Utils/skArray.h"

struct rtSceneType;
class rtPlane;
class rtSphere;
class rtCube;
class rtOctree;

/// <summary>
/// rtScene is defined as a container for rtObject types
/// </summary>
class rtScene final
{
public:
    typedef skArray<rtObject*>   ObjectArray;
    typedef skArray<rtCamera*>   CameraArray;
    typedef skArray<rtLight*>    LightArray;
    typedef skArray<rtMesh*>     MeshArray;
    typedef skArray<rtBvObject*> BoundingVolumeArray;

protected:
    /// <summary>
    /// Protected storage for loaded scenes.
    /// </summary>
    ObjectArray m_objects;

    ObjectArray m_outOfDateTransforms;

    MeshArray           m_meshes;
    LightArray          m_lights;
    CameraArray         m_cameras;
    BoundingVolumeArray m_boundingVolumes;
    skColor             m_horizon;
    skColor             m_zenith;
    rtSceneType*        m_data;

public:
    rtScene();
    ~rtScene();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtSceneType* getPtr() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    rtSceneType& getData();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const rtSceneType& getData() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    ObjectArray& getObjects();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    CameraArray& getCameras();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    MeshArray& getMeshes();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    LightArray& getLights();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const LightArray& getLights() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    BoundingVolumeArray& getBoundingVolumes();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    SKuint32 getFlags() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="flags"></param>
    void setFlags(const SKuint32& flags);

    /// <summary>
    ///
    /// </summary>
    /// <param name="bvo"></param>
    void addBoundingObject(rtBvObject* bvo);

    /// <summary>
    ///
    /// </summary>
    /// <param name="cam"></param>
    void addCamera(rtCamera* cam);

    /// <summary>
    ///
    /// </summary>
    /// <param name="mesh"></param>
    void addMesh(rtMesh* mesh);

    /// <summary>
    ///
    /// </summary>
    /// <param name="light"></param>
    void addLight(rtLight* light);

    /// <summary>
    ///
    /// </summary>
    void updateCaches();

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skColor& getHorizon() const;

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skColor& getZenith() const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="col"></param>
    void setHorizon(const skColor& col);

    /// <summary>
    ///
    /// </summary>
    /// <param name="col"></param>
    void setZenith(const skColor& col);

    /// <summary>
    ///
    /// </summary>
    /// <param name="node"></param>
    void pushOutOfDate(rtObject* node);
};

/*! @} */

SK_INLINE rtSceneType* rtScene::getPtr() const
{
    SK_ASSERT(m_data);
    return m_data;
}

SK_INLINE rtSceneType& rtScene::getData()
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE const rtSceneType& rtScene::getData() const
{
    SK_ASSERT(m_data);
    return *m_data;
}

SK_INLINE rtScene::ObjectArray& rtScene::getObjects()
{
    return m_objects;
}

SK_INLINE rtScene::CameraArray& rtScene::getCameras()
{
    return m_cameras;
}

SK_INLINE rtScene::MeshArray& rtScene::getMeshes()
{
    return m_meshes;
}

SK_INLINE rtScene::LightArray& rtScene::getLights()
{
    return m_lights;
}

SK_INLINE const rtScene::LightArray& rtScene::getLights() const
{
    return m_lights;
}

SK_INLINE rtScene::BoundingVolumeArray& rtScene::getBoundingVolumes()
{
    return m_boundingVolumes;
}

SK_INLINE SKuint32 rtScene::getFlags() const
{
    return getData().flags;
}

SK_INLINE void rtScene::setFlags(const SKuint32& flags)
{
    getData().flags = flags;
}

SK_INLINE const skColor& rtScene::getHorizon() const
{
    return m_horizon;
}

SK_INLINE const skColor& rtScene::getZenith() const
{
    return m_zenith;
}

SK_INLINE void rtScene::setHorizon(const skColor& col)
{
    m_horizon         = col.limit();
    getData().horizon = {m_horizon.r, m_horizon.g, m_horizon.b};
}

SK_INLINE void rtScene::setZenith(const skColor& col)
{
    m_zenith         = col.limit();
    getData().zenith = {m_zenith.r, m_zenith.g, m_zenith.b};
}

#endif  //_rtLoader_h_
