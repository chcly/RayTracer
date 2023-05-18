/*
-------------------------------------------------------------------------------
    http://gamekit.googlecode.com/

    Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "bAsciiLinker.h"
#include <complex>
#include <cstdio>
#include "bAscii.h"
#include "bAsciiArgument.h"
#include "bAsciiBlock.h"
#include "bAsciiMain.h"
#include "bAsciiOpCodes.h"

bAsciiLinker::bAsciiLinker() :
    m_parent(nullptr),
    m_main(nullptr),
    m_verbose(false)
{
}

int bAsciiLinker::link(bAscii* parent, bAsciiMain* main, bool verbose)
{
    SK_ASSERT(parent && main);

    m_verbose = verbose;
    m_main    = main;
    m_parent  = parent;

    bAsciiBlock* lib = m_parent->findBlock(LIB_CAMERA);
    if (lib)
    {
        if (verbose)
            printf("Linking Camera Library ==> %p\n", (void*)lib);
        linkCamera(lib);
    }
    lib = m_parent->findBlock(LIB_MATERIAL);
    if (lib)
    {
        if (verbose)
            printf("Linking Material Library ==> %p\n", (void*)lib);
        linkMaterial(lib);
    }

    lib = m_parent->findBlock(LIB_LIGHT);
    if (lib)
    {
        if (verbose)
            printf("Linking Light Library ==> %p\n", (void*)lib);
        linkLight(lib);
    }

    lib = m_parent->findBlock(LIB_MESH);
    if (lib)
    {
        if (verbose)
            printf("Linking Mesh Library ==> %p\n", (void*)lib);
        linkMesh(lib);
    }

    lib = m_parent->findBlock(LIB_SHAPE);
    if (lib)
    {
        if (verbose)
            printf("Linking Mesh Library ==> %p\n", (void*)lib);
        linkShape(lib);
    }

    lib = m_parent->findBlock(LIB_OBJECT);
    if (lib)
    {
        if (verbose)
            printf("Linking Object Library ==> %p\n", (void*)lib);
        linkObject(lib);
    }

    lib = m_parent->findBlock(LIB_SCENE);
    if (lib)
    {
        if (verbose)
            printf("Linking Scene Library ==> %p\n", (void*)lib);
        linkScene(lib);
    }

    lib = m_parent->findBlock(LIB_GLOBAL);
    if (lib)
    {
        m_main->m_creator = lib->getString(OP_GLOBAL_CREATOR, 0);
        m_main->m_version = lib->getInt32(OP_GLOBAL_VERSION, 0);
        m_main->m_scene   = m_main->getSceneLibrary().find(lib->getString(OP_GLOBAL_SCENE, 0));
    }

    return 0;
}

void bAsciiLinker::linkCamera(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        if (obj->getType() != LIB_BLOCK_CAMERA)
        {
            printf("Camera block mis-match\n");
            continue;
        }

        bCamera* ob     = new bCamera();
        ob->limits[0]   = obj->getFloat(OP_CAMERA_CLIP, 0, 0);
        ob->limits[1]   = obj->getFloat(OP_CAMERA_CLIP, 1, 1000);
        ob->fov         = obj->getFloat(OP_CAMERA_FOV, 0, 45);
        ob->interactive = obj->hasArgument(OP_CAMERA_INTERACTIVE);
        m_main->getCameraLibrary().insert(obj->getName(), ob);
    }
}

void bAsciiLinker::linkLight(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        if (obj->getType() != LIB_BLOCK_LIGHT)
        {
            printf("Light block mis-match\n");
            continue;
        }

        bLight* ob = new bLight();

        ob->type      = obj->getInt32(OP_LIGHT_TYPE, 0, bLight::POINT);
        ob->power     = obj->getFloat(OP_LIGHT_POWER, 0, 60);
        ob->elevation = obj->getFloat(OP_LIGHT_FALLOFF_ELEVATION, 0, 1.f);
        ob->decay     = obj->getFloat(OP_LIGHT_FALLOFF_DECAY, 0, .001f);
        ob->mode      = obj->getInt32(OP_LIGHT_MODE, 0);
        ob->radius    = obj->getFloat(OP_LIGHT_RADIUS, 0, 0.3f);

        obj->getFloatVector(ob->color, OP_LIGHT_COLOR, 0, 3);

        m_main->getLightLibrary().insert(obj->getName(), ob);
    }
}

void bAsciiLinker::linkShape(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        bShape* ob = nullptr;

        const bInt32 type = obj->getType();
        switch (type)
        {
        case LIB_BLOCK_CUBE:
        {
            bCube* cu = new bCube();
            obj->getFloatVector(cu->center, OP_OBJECT_CENTER, 0, 3);
            cu->extent = obj->getFloat(OP_OBJECT_EXTENT);

            ob = cu;
        }
        break;
        case LIB_BLOCK_SPHERE:
        {
            bSphere* sp = new bSphere();
            obj->getFloatVector(sp->center, OP_OBJECT_CENTER, 0, 3);
            sp->radius = obj->getFloat(OP_OBJECT_RADIUS);

            ob = sp;
        }
        break;
        case LIB_BLOCK_PLANE:
        {
            bPlane* ps = new bPlane();
            obj->getFloatVector(ps->normal, OP_OBJECT_NORMAL, 0, 3);
            obj->getFloatVector(ps->p0, OP_OBJECT_P0, 0, 3);
            ob = ps;
        }
        break;
        default:
            break;
        }

        if (ob != nullptr)
        {
            ob->material = m_main->getMaterialLibrary().find(
                obj->getString(OP_OBJECT_MATERIAL));
            m_main->getShapeLibrary().insert(obj->getName(), ob);
        }
    }
}

void bAsciiLinker::linkMesh(bAsciiBlock* lib) const
{
}

void bAsciiLinker::resolveData(const bString& string, bObject* ob) const
{
    bShape*  sh;
    bLight*  li;
    bCamera* ca;
    if ((ca = m_main->getCameraLibrary().find(string)) != nullptr)
    {
        ob->type = LIB_BLOCK_CAMERA;
        ob->data = ca;
    }
    else if ((li = m_main->getLightLibrary().find(string)) != nullptr)
    {
        ob->type = LIB_BLOCK_LIGHT;
        ob->data = li;
    }
    else if ((sh = m_main->getShapeLibrary().find(string)) != nullptr)
    {
        ob->type     = sh->type;
        ob->data     = sh;
        ob->material = sh->material;
    }
    else
    {
        printf("failed to resolve attached data '%s'\n",
               string.c_str());
    }
}

void bAsciiLinker::linkObject(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        const bCode type = obj->getType();
        if (type != LIB_BLOCK_OBJECT)
        {
            printf("object block mis-match\n");
            continue;
        }

        bObject* ob = new bObject();

        ob->type               = type;
        ob->material           = m_main->getMaterialLibrary().find(obj->getString(OP_OBJECT_MATERIAL));
        const bString& libData = obj->getString(OP_OBJECT_LIB);
        if (!libData.empty())
            resolveData(libData, ob);

        obj->getFloatVector(ob->location, OP_OBJECT_LOCATION, 0, 3);
        obj->getFloatVector(ob->rotation, OP_OBJECT_ROTATION, 0, 3);
        obj->getFloatVector(ob->scale, OP_OBJECT_SCALE, 0, 3);

        m_main->getObjectLibrary().insert(obj->getName(), ob);
    }
}

void bAsciiLinker::linkMaterial(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        if (obj->getType() != LIB_BLOCK_MATERIAL)
        {
            printf("Material block mis-match\n");
            continue;
        }

        bMaterial* ob = new bMaterial();
        ob->ambient   = obj->getFloat(OP_MATERIAL_AMBIENT, 0, 1);
        ob->diffuse   = obj->getFloat(OP_MATERIAL_DIFFUSE, 0, 1);
        ob->specular  = obj->getFloat(OP_MATERIAL_SPECULAR, 0, 0);
        ob->hardness  = obj->getFloat(OP_MATERIAL_HARDNESS, 0, 0);
        ob->flags     = obj->getInt32(OP_MATERIAL_FLAGS, 0, bMaterial::LIGHTING);
        obj->getFloatVector(ob->color, OP_MATERIAL_COLOR, 0, 3);

        m_main->getMaterialLibrary().insert(obj->getName(), ob);
    }
}

void bAsciiLinker::linkScene(bAsciiBlock* lib) const
{
    bAsciiBlockArray& children = lib->getChildren();
    for (bAsciiBlock* obj : children)
    {
        const bCode type = obj->getType();

        if (type != LIB_BLOCK_SCENE)
        {
            printf("Scene block mis-match\n");
            continue;
        }

        bScene* ob = new bScene();

        obj->getFloatVector(ob->horizon, OP_SCENE_HORIZON, 0, 3);
        obj->getFloatVector(ob->zenith, OP_SCENE_ZENITH, 0, 3);

        ob->flags = obj->getInt32(OP_SCENE_FLAGS, 0, 0);

        bAsciiArgument* scObj = obj->getArgument(OP_SCENE_OBJECTS);
        if (scObj && !scObj->strings.empty())
        {
            ob->objects     = new bObjectPtr[(SKsize)scObj->strings.size() + 1];
            ob->objectCount = 0;

            for (const bString& objName : scObj->strings)
            {
                bObject* lob = m_main->getObjectLibrary().find(objName);
                if (lob != nullptr)
                    ob->objects[ob->objectCount++] = lob;
                else
                    printf("Failed to find object '%s'\n", objName.c_str());
            }
        }

        m_main->getSceneLibrary().insert(obj->getName(), ob);
    }
}
