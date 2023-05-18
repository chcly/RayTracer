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
#include "rtAsciiLoader.h"
#include <cstdio>
#include "RenderSystem/rtCamera.h"
#include "RenderSystem/rtCube.h"
#include "RenderSystem/rtInteractiveCamera.h"
#include "RenderSystem/rtLight.h"
#include "RenderSystem/rtObject.h"
#include "RenderSystem/rtPlane.h"
#include "RenderSystem/rtScene.h"
#include "RenderSystem/rtSphere.h"
#include "RenderSystem/rtMaterial.h"
#include "bAscii/bAscii.h"
#include "bAscii/bAsciiArgument.h"
#include "bAscii/bAsciiMain.h"
#include "bAscii/bAsciiOpCodes.h"

class rtAsciiLoaderPrivate
{
private:
    rtAsciiLoader* m_parent;
    rtScene*       m_current;
    bScene*        m_active;
    bAsciiMain*    m_main;

public:
    explicit rtAsciiLoaderPrivate(rtAsciiLoader* parent) :
        m_parent(parent),
        m_current(nullptr),
        m_active(nullptr),
        m_main(nullptr)
    {
    }

    ~rtAsciiLoaderPrivate() = default;

    void buildCamera(bObject* ob) const
    {
        bCamera*  ca = (bCamera*)ob->data;
        rtCamera* rc = nullptr;
        if (ca && ca->interactive)
            rc = new rtInteractiveCamera(m_current);
        else if (ca)
            rc = new rtCamera(m_current);

        if (rc && ca)
        {
            rc->setNear(ca->limits[0]);
            rc->setFar(ca->limits[1]);
            rc->setFieldOfViewAngle(ca->fov);

            setObjectProperties(ob, rc);
            m_current->addCamera(rc);
        }
    }

    void buildObject(bObject* ob) const
    {
        rtBvObject* bv = nullptr;
        if (ob->type == LIB_BLOCK_CUBE)
        {
            bCube*  cu = (bCube*)ob->data;
            rtCube* rc = new rtCube(m_current);
            bv         = rc;
            rc->setExtent(cu->extent);
        }
        else if (ob->type == LIB_BLOCK_SPHERE)
        {
            bSphere*  sp = (bSphere*)ob->data;
            rtSphere* rs = new rtSphere(m_current);
            bv           = rs;
            rs->setRadius(sp->radius);
        }
        else if (ob->type == LIB_BLOCK_PLANE)
        {
            bPlane*  pl = (bPlane*)ob->data;
            rtPlane* rp = new rtPlane(m_current);
            bv          = rp;

            rp->setNormal(skVector3(pl->normal));
            rp->setPoint(skVector3(pl->p0));
        }
        if (bv)
        {
            if (ob->material)
            {
                rtMaterial* mat = bv->getMaterial();

                bMaterial* ma = ob->material;

                mat->setFlags(ma->flags);
                mat->setColor(ma->color);
                mat->setDiffuse(ma->diffuse);
                mat->setAmbient(ma->ambient);
                mat->setHardness(ma->hardness);
                mat->setSpecular(ma->specular);

            }

            setObjectProperties(ob, bv);

            m_current->addBoundingObject(bv);
        }
    }

    void buildLight(bObject* ob) const
    {
        bLight* li = (bLight*)ob->data;
        if (li)
        {
            rtLight* light = new rtLight(m_current);

            light->setEnergy(li->power);
            light->setElevation(li->elevation);
            light->setDecay(li->decay);
            light->setMode(li->mode);
            light->setRadius(li->radius);

            setObjectProperties(ob, light);

            m_current->addLight(light);
        }
    }

    int buildScene(bAscii* ascii) const
    {
        for (bSize i = 0; i < m_active->objectCount; ++i)
        {
            bObject* ob = m_active->objects[i];

            if (!ob->data)
            {
                printf("missing object data\n");
                continue;
            }

            switch (ob->type)
            {
            case LIB_BLOCK_CAMERA:
                buildCamera(ob);
                break;
            case LIB_BLOCK_LIGHT:
                buildLight(ob);
                break;
            case LIB_BLOCK_OBJECT:
            case LIB_BLOCK_CUBE:
            case LIB_BLOCK_SPHERE:
            case LIB_BLOCK_PLANE:
                buildObject(ob);
                break;
            default:
                break;
            }
        }

        m_current->setHorizon(skColor(m_active->horizon[0], m_active->horizon[1], m_active->horizon[2]));
        m_current->setZenith(skColor(m_active->zenith[0], m_active->zenith[1], m_active->zenith[2]));
        return 0;
    }

    static void setObjectProperties(bObject* ob, rtObject* node)
    {
        node->setPosition(ob->location[0], ob->location[1], ob->location[2]);
        node->setOrientation(ob->rotation[0], ob->rotation[1], ob->rotation[2]);
        node->setScale(ob->scale[0], ob->scale[1], ob->scale[2]);
    }

    int load(const char* path)
    {
        bAscii ascii;
        m_main = ascii.loadApi(path);

        if (m_main == nullptr)
        {
            printf("Failed to load %s.\n", path);
            return OP_ERROR;
        }

        m_active = m_main->getActiveScene();
        if (m_active == nullptr)
        {
            printf("no active scene was set in '%s'.\n", path);
            return OP_ERROR;
        }
        if (m_active->objects == nullptr)
        {
            printf("no objects in the active scene. '%s'.\n", path);
            return OP_ERROR;
        }

        m_current = new rtScene();
        m_current->setFlags(m_active->flags);

        m_parent->addScene(m_current);
        return buildScene(&ascii);
    }
};

int rtAsciiLoader::load(const char* path)
{
    rtAsciiLoaderPrivate impl(this);
    return impl.load(path);
}
