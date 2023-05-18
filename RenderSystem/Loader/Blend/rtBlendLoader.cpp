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
#include "rtBlendLoader.h"
#include <cstdio>
#include "Math/skColor.h"
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtInteractiveCamera.h"
#include "RenderSystem/rtLight.h"
#include "RenderSystem/rtMesh.h"
#include "RenderSystem/rtObject.h"
#include "RenderSystem/rtScene.h"
#include "Utils/skLogger.h"
#include "ftBlend.h"

constexpr SKint32 OB_TYPE_MESH   = 1;
constexpr SKint32 OB_TYPE_CAMERA = 11;
constexpr SKint32 OB_TYPE_LIGHT  = 10;

using namespace ftFlags;

class rtBlendLoaderPrivate
{
private:
    rtBlendLoader*  m_parent;
    ftBlend*        m_blend;
    rtScene*        m_current;
    Blender::Scene* m_blenderScene;

public:
    rtBlendLoaderPrivate(rtBlendLoader* parent) :
        m_parent(parent),
        m_blend(nullptr),
        m_current(nullptr),
        m_blenderScene(nullptr)
    {
    }

    ~rtBlendLoaderPrivate()
    {
        delete m_blend;
        m_blend = nullptr;
    }

    int load(const char* path)
    {
        m_blend = new ftBlend();
        m_blend->setFileFlags(LF_DIAGNOSTICS | LF_MIS_REPORTED);

        int status = m_blend->load(path, RM_COMPRESSED);
        if (status != FS_OK)
        {
            printf("File loading failed\n");
            return status;
        }

        Blender::FileGlobal* fg = m_blend->m_fg;
        if (fg && fg->curscene)
        {
            m_blenderScene = fg->curscene;

            status = bfLoadScene();
        }
        else
            status = FS_FAILED;
        return status;
    }

    int bfLoadLight(Blender::Object* ob, Blender::Lamp* la) const
    {
        rtLight* light = new rtLight(m_current);

        m_current->addLight(light);

        light->setElevation(1.f);
        light->setDecay(la->energy*0.5f);



        light->setEnergy(la->energy);

        setObjectProperties(ob, light);
        return FS_OK;
    }

    int bfLoadMesh(Blender::Mesh* me, Blender::Object* ob) const
    {
        rtMesh* mesh = new rtMesh(m_current);

        if (me->mat && *me->mat)
        {
            Blender::Material* mat = *me->mat;



            rtMaterial* rmat = mesh->getMaterial();

            float col[3] = {mat->r, mat->g, mat->b};
            rmat->setColor(col);

            rmat->setFlags(RT_MA_LIGHTING);

            rmat->setDiffuse(mat->roughness * mat->metallic);
            rmat->setAmbient(1);
            rmat->setSpecular(mat->spec);
            rmat->setHardness(255);
        }

        const skVector3& bakedScale = skVector3(ob->size[0], ob->size[1], ob->size[2]);
        mesh->beginAddTriangles();

        for (int f = 0; f < me->totpoly; ++f)
        {
            Blender::MPoly& cp = me->mpoly[f];
            if (cp.totloop == 4)
            {
                const Blender::MLoop& i0 = me->mloop[cp.loopstart];
                const Blender::MLoop& i1 = me->mloop[cp.loopstart + 1];
                const Blender::MLoop& i2 = me->mloop[cp.loopstart + 2];
                const Blender::MLoop& i3 = me->mloop[cp.loopstart + 3];

                skVector3 v0 = skVector3(&me->mvert[i0.v].co[0]) * bakedScale;
                skVector3 v1 = skVector3(&me->mvert[i1.v].co[0]) * bakedScale;
                skVector3 v2 = skVector3(&me->mvert[i2.v].co[0]) * bakedScale;
                skVector3 v3 = skVector3(&me->mvert[i3.v].co[0]) * bakedScale;

                mesh->addTriangle(i0.v, i1.v, i2.v, v0, v1, v2);
                mesh->addTriangle(i2.v, i3.v, i0.v, v2, v3, v0);
            }
            else if (cp.totloop == 3)
            {
                const Blender::MLoop& i0 = me->mloop[cp.loopstart];
                const Blender::MLoop& i1 = me->mloop[cp.loopstart + 1];
                const Blender::MLoop& i2 = me->mloop[cp.loopstart + 2];

                skVector3 v0 = skVector3(&me->mvert[i0.v].co[0]) * bakedScale;
                skVector3 v1 = skVector3(&me->mvert[i1.v].co[0]) * bakedScale;
                skVector3 v2 = skVector3(&me->mvert[i2.v].co[0]) * bakedScale;

                mesh->addTriangle(i0.v, i1.v, i2.v, v0, v1, v2);
            }
        }

        mesh->endAddTriangles();

        setObjectProperties(ob, mesh);
        m_current->addMesh(mesh);
        return FS_OK;
    }

    static void setObjectProperties(Blender::Object* ob, rtObject* sceneNode)
    {
        if (sceneNode && ob)
        {
            sceneNode->setPosition(ob->loc[0], ob->loc[1], ob->loc[2]);
            sceneNode->setOrientation(ob->rot[0], ob->rot[1], ob->rot[2]);
            sceneNode->setScale(ob->size[0], ob->size[1], ob->size[2]);
        }
    }

    int bfLoadCamera(Blender::Object* ob, Blender::Camera* bc) const
    {
        rtInteractiveCamera* cam = new rtInteractiveCamera(m_current);

        cam->setFieldOfViewAngle(skMath::toFieldOfView(bc->lens));
        cam->setNear(bc->clipsta);
        cam->setFar(bc->clipend);
        

        setObjectProperties(ob, cam);
        cam->pivot(0, -skPiH);
        cam->center();

        m_current->addCamera(cam);
        return FS_OK;
    }

    int bfLoadObject(Blender::Object* ob) const
    {
        if (ob)
        {
            if (ob->data)
            {
                switch (ob->type)
                {
                case OB_TYPE_MESH:
                    bfLoadMesh((Blender::Mesh*)ob->data, ob);
                    break;
                case OB_TYPE_CAMERA:

                    // only interested in the main camera for now
                    if (m_blenderScene->camera == ob)
                        bfLoadCamera(ob, (Blender::Camera*)ob->data);
                    break;
                case OB_TYPE_LIGHT:
                    bfLoadLight(ob, (Blender::Lamp*)ob->data);
                    break;
                default:
                    break;
                }
            }
        }
        return FS_OK;
    }

    int bfIterateCollectionObject(Blender::CollectionObject* colObject) const
    {
        while (colObject)
        {
            if (colObject->ob)
                bfLoadObject(colObject->ob);
            colObject = colObject->next;
        }
        return FS_OK;
    }

    int bfIterateCollectionChild(Blender::Collection* root) const
    {
        if (root)
        {
            Blender::CollectionChild* cc = (Blender::CollectionChild*)root->children.first;
            while (cc)
            {
                if (cc->collection)
                    bfIterateCollectionObject((Blender::CollectionObject*)cc->collection->gobject.first);
                cc = cc->next;
            }

            Blender::CollectionObject* obj = (Blender::CollectionObject*)root->gobject.first;
            while (obj)
            {
                if (obj->ob)
                    bfLoadObject(obj->ob);
                obj = obj->next;
            }
        }
        return FS_OK;
    }

    int bfLoadScene()
    {
        if (!m_blend || !m_blenderScene)
            return FS_FAILED;

        m_current = new rtScene();
        m_parent->addScene(m_current);

        int status = FS_OK;

        if (m_blenderScene->base.first)
        {
            for (Blender::Base* base = (Blender::Base*)m_blenderScene->base.first;
                 base && status == FS_OK;
                 base = (Blender::Base*)base->next)
            {
                if (base->object)
                    status = bfLoadObject(base->object);
            }
        }

        if (status == FS_OK)
        {
            Blender::World* wo = m_blenderScene->world;
            if (wo != nullptr)
            {
                m_current->setHorizon(skColor(wo->horr, wo->horg, wo->horb));
                m_current->setZenith(m_current->getHorizon());
            }

            if (m_blenderScene->master_collection)
                status = bfIterateCollectionChild(m_blenderScene->master_collection);
        }
        return status;
    }
};

int rtBlendLoader::load(const char* path)
{
    rtBlendLoaderPrivate impl(this);
    return impl.load(path);
}
