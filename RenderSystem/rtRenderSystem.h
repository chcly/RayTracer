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
#ifndef _rtSystem_h_
#define _rtSystem_h_

#include "Math/skVector4.h"
#include "RenderSystem/rtScene.h"
#include "RenderSystem/rtTarget.h"

class rtTileManager;
class rtTarget;
using rtCameras = rtScene::CameraArray;
using rtLights  = rtScene::LightArray;
using rtMeshes  = rtScene::MeshArray;
using rtBVOs    = rtScene::BoundingVolumeArray;

class skRay;
class skImage;

/// <summary>
/// Render mode flags.
/// </summary>
enum rtRenderMode
{
    /// <summary>
    /// Render the scene, computing
    /// light and shadow.
    /// </summary>
    RM_COLOR_AND_LIGHT = 0x001,

    /// <summary>
    /// Render using the parametric distance.
    /// </summary>
    RM_DISTANCE = 0x002,

    /// <summary>
    /// Render using the normals.
    /// </summary>
    RM_COMPUTED_NORMAL = 0x004,

    /// <summary>
    /// Invert the output color
    /// </summary>
    RM_INVERT = 0x008,

    /// <summary>
    /// Trace silhouette
    /// </summary>
    RM_OUTLINE = 0x010,

    /// <summary>
    /// Overlay the threaded tiles.
    /// </summary>
    RM_DEBUG_TILE = 0x020,

    /// <summary>
    /// Process 4 passes per RM_COLOR_AND_LIGHT pass
    /// </summary>
    RM_AA = 0x040,
};

/// <summary>
/// Base class for a custom renderer implementation.
/// </summary>
class rtRenderSystem
{
protected:
    rtTarget* m_target;
    rtScene*  m_scene;
    skVector2 m_limit;
    rtCamera* m_camera;
    skVector4 m_iPixelOffset;
    SKint32   m_mode;
    bool      m_dirty;

    /// <summary>
    /// Computes normalized pixel coordinates,
    /// </summary>
    void updatePixelOffset();

public:
    rtRenderSystem();
    virtual ~rtRenderSystem() = default;


    void invalidate()
    {
        m_dirty = true;
    }


    /// <summary>
    /// Sets the render mode flag.
    /// </summary>
    /// <param name="mode">Mode should be one or more of the bits in rtRenderMode</param>
    void setMode(const SKint32& mode);

    /// <summary>
    /// Gets the current render mode bit flag.
    /// </summary>
    /// <returns></returns>
    SKint32 getMode() const;

    /// <summary>
    /// Sets the target that will be rendered to.
    /// </summary>
    /// <param name="target">Instance of an implementation of the rtTarget class.</param>
    void setTarget(rtTarget* target);

    /// <summary>
    /// The main implementation method to render a scene.
    /// </summary>
    /// <param name="scene">The rtScene instance that should be rendered.</param>
    virtual void render(rtScene* scene) = 0;

    /// <summary>
    /// Returns the reciprocal of the current targets width and height.
    /// </summary>
    const skVector4& getPixelOffset() const;

    /// <summary>
    /// Returns the currently set render target.
    /// </summary>
    rtTarget* getTarget() const;

    /// <summary>
    /// Returns the scene that is supplied with the call to render.
    /// </summary>
    rtScene* getCurrentScene() const;

    /// <summary>
    /// Returns the scene that is supplied with the call to render.
    /// </summary>
    rtSceneType* getKernelScene() const;

    /// <summary>
    /// Returns the main camera in the scene.
    /// </summary>
    rtCamera* getCurrentCamera() const;

    /// <summary>
    /// Returns the current camera's limits.
    /// </summary>
    const skVector2& getLimits() const;
};
/*! @} */

SK_INLINE void rtRenderSystem::setMode(const SKint32& mode)
{
    m_mode = mode;
    if (m_scene)
        m_scene->setFlags(m_mode);
}

SK_INLINE SKint32 rtRenderSystem::getMode() const
{
    return m_mode;
}

SK_INLINE const skVector4& rtRenderSystem::getPixelOffset() const
{
    return m_iPixelOffset;
}

SK_INLINE rtTarget* rtRenderSystem::getTarget() const
{
    return m_target;
}

SK_INLINE rtScene* rtRenderSystem::getCurrentScene() const
{
    return m_scene;
}

SK_INLINE rtSceneType* rtRenderSystem::getKernelScene() const
{
    return m_scene->getPtr();
}

SK_INLINE rtCamera* rtRenderSystem::getCurrentCamera() const
{
    return m_camera;
}

SK_INLINE const skVector2& rtRenderSystem::getLimits() const
{
    return m_limit;
}

#endif  //_rtSystem_h_
