/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley
    
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
#ifndef _bApi_h_
#define _bApi_h_

#include "Utils/skFixedString.h"
#include "bAsciiCommon.h"
#include "bAsciiOpCodes.h"

using bSize     = SKsize;
using bInt      = SKint64;
using bInt32    = SKint32;
using bScalar   = bNumber;
using bFloat2   = bScalar[2];
using bFloat3   = bScalar[3];
using bFloat4   = bScalar[4];
using bFloat16  = bScalar[16];
using bString32 = skFixedString<32>;

struct bGlobal;
struct bScene;
struct bObject;
struct bLight;
struct bMaterial;

struct bGlobal
{
    bString32 creator;
    bScene*   scene;
    bInt      version;
};

struct bGenericBoundingVolume
{
    bGenericBoundingVolume() :
        center{0, 0, 0},
        extent(0.5f),
        prop1(0),
        prop2(0)
    {
    }

    bFloat3 center;
    bScalar extent;
    bScalar prop1;
    bScalar prop2;
};

struct bShape
{
    bShape(const bInt32& _type) :
        type(_type),
        material(nullptr)
    {
    }

    const bInt32 type;
    bMaterial*   material;
};

struct bCube : bShape
{
    bCube() :
        bShape(LIB_BLOCK_CUBE),
        center{0, 0, 0},
        extent(0.5f)
    {
    }

    bFloat3 center;
    bScalar extent;
};

struct bSphere : bShape
{
    bSphere() :
        bShape(LIB_BLOCK_SPHERE),
        center{0, 0, 0},
        radius(0.5f)
    {
    }

    bFloat3 center;
    bScalar radius;
};

struct bPlane : bShape
{
    bPlane() :
        bShape(LIB_BLOCK_PLANE),
        p0{0, 0, 0},
        normal{0, 0, 0}
    {
    }

    bFloat3 p0;
    bFloat3 normal;
};

struct bObject
{
    bObject() :
        type(-1),
        location{0, 0, 0},
        rotation{0, 0, 0},
        scale{1, 1, 1},
        data(nullptr),
        material(nullptr)
    {
    }

    bInt32     type;
    bFloat3    location;
    bFloat3    rotation;
    bFloat3    scale;
    void*      data;
    bMaterial* material;
};

using bObjectPtr = bObject*;

struct bScene
{
    bScene() :
        objects(nullptr),
        objectCount(0),
        horizon{0, 0, 0},
        zenith{0, 0, 0},
        flags(0)
    {
    }

    ~bScene()
    {
        delete[] objects;
    }

    bObjectPtr* objects;
    bSize       objectCount;
    bFloat3     horizon;
    bFloat3     zenith;
    bInt32      flags;
};

struct bCamera
{
    bCamera() :
        limits{0, 1000},
        fov(0),
        interactive(0)
    {
    }

    bFloat2 limits;
    bScalar fov;
    bInt32  interactive;
};

struct bLight
{
    enum bLightType
    {
        POINT = 0,
        SPOT,
        AREA,
    };

    bLight() :
        type(POINT),
        color{1, 1, 1},
        power(60),
        radius(0.3f),
        elevation(1.f),
        decay(.001f),
        mode(0)
    {
    }

    bInt32  type;
    bFloat3 color;

    // https://www.desmos.com/calculator/l2ipt7pc9k
    bScalar power;
    bScalar radius;
    bScalar elevation;
    bScalar decay;

    bInt32 mode;
};

struct bMaterial
{
    enum Flags
    {
        SHADELESS  = 0,
        LIGHTING   = 0x001,
        REFLECTION = 0x002,
    };

    bMaterial() :
        color{1, 1, 1},
        flags(LIGHTING),
        ambient(1),
        diffuse(1),
        specular(0),
        hardness(0)
    {
    }

    bFloat3 color;
    bInt32  flags;
    bScalar ambient;
    bScalar diffuse;
    bScalar specular;
    bScalar hardness;
};

#endif  //_bApi_h_
