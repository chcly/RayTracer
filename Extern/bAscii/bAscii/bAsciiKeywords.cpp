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
#include "bAsciiKeywords.h"
#include "bAsciiKeywords.inl"
#include "bAsciiOpCodes.h"

constexpr bAsciiLocal NullRecord = {
    nullptr,
    SK_NPOS,
    OP_UNKNOWN,
    OP_UNKNOWN,
    OP_UNKNOWN,
    OP_UNKNOWN,
    0,
    0,
    nullptr,
};

constexpr bAsciiLocal MakeStruct(const bKey         key,
                                 bCode              op,
                                 const bAsciiLocal* libs)
{
    return {
        key.name,
        key.hash,
        op,
        OP_STRING,
        OP_UNKNOWN,
        OP_TYPE_STRUCT,
        -1,
        -1,
        libs,
    };
}

constexpr bAsciiLocal MakeStruct(const bKey         key,
                                 bCode              op,
                                 bCode              constructor,
                                 const bAsciiLocal* libs)
{
    return {key.name, key.hash, op, constructor, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, libs};
}
constexpr bAsciiLocal MakeStringArray(const bKey key,
                                      bCode      op,
                                      int        args    = -1,
                                      int        maxArgs = -1)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_STRING, OP_TYPE_STRING_ARRAY, args, maxArgs, nullptr};
}

constexpr bAsciiLocal MakeBool(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_BOOL, 1, 1, nullptr};
}

constexpr bAsciiLocal MakeFloat(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT, 1, 1, nullptr};
}

constexpr bAsciiLocal MakeFloat2(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT2, 2, 2, nullptr};
}

constexpr bAsciiLocal MakeFloat3(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT3, 3, 3, nullptr};
}

constexpr bAsciiLocal MakeFloat16(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT16, 16, 16, nullptr};
}

constexpr bAsciiLocal MakeFloatArray(const bKey key,
                                     bCode      op,
                                     SKint32    stride = -1)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT_ARRAY, stride, -1, nullptr};
}

constexpr bAsciiLocal MakeInt(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_INT, 1, 1, nullptr};
}

constexpr bAsciiLocal MakeIntArray(const bKey key,
                                   bCode      op,
                                   SKint32    stride = -1)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_NUMBER, OP_TYPE_INT_ARRAY, stride, -1, nullptr};
}

constexpr bAsciiLocal MakeString(const bKey key, bCode op)
{
    return {key.name, key.hash, op, OP_UNKNOWN, OP_STRING, OP_TYPE_STRING, 1, 1, nullptr};
}

// Global Tables

constexpr bAsciiLocal GlobalType[] = {
    MakeString(KW_creator, OP_GLOBAL_CREATOR),
    MakeString(KW_scene, OP_GLOBAL_SCENE),
    MakeInt(KW_version, OP_GLOBAL_VERSION),
    NullRecord,
};

// Scene Tables

constexpr bAsciiLocal SceneType[] = {
    MakeStringArray(KW_objects, OP_SCENE_OBJECTS),
    MakeFloat3(KW_horizon, OP_SCENE_HORIZON),
    MakeFloat3(KW_zenith, OP_SCENE_ZENITH),
    MakeInt(KW_flags, OP_SCENE_FLAGS),
    NullRecord,
};

constexpr bAsciiLocal SceneLibrary[] = {
    MakeStruct(KW_Scene, LIB_BLOCK_SCENE, SceneType),
    NullRecord,
};

// Object Tables

constexpr bAsciiLocal ObjectType[] = {
    MakeString(KW_data, OP_OBJECT_LIB),
    MakeFloat3(KW_location, OP_OBJECT_LOCATION),
    MakeFloat3(KW_rotation, OP_OBJECT_ROTATION),
    MakeFloat3(KW_scale, OP_OBJECT_SCALE),
    MakeFloat16(KW_matrix, OP_OBJECT_MATRIX),
    MakeString(KW_parent, OP_OBJECT_PARENT),
    MakeString(KW_material, OP_OBJECT_MATERIAL),
    NullRecord,
};

constexpr bAsciiLocal ObjectLibrary[] = {

    MakeStruct(KW_Object, LIB_BLOCK_OBJECT, ObjectType),
    NullRecord,
};

// Shape Tables

constexpr bAsciiLocal CubeType[] = {
    MakeFloat3(KW_center, OP_OBJECT_CENTER),
    MakeFloat(KW_extent, OP_OBJECT_EXTENT),
    MakeString(KW_material, OP_OBJECT_MATERIAL),
    NullRecord,
};

constexpr bAsciiLocal SphereType[] = {
    MakeFloat3(KW_center, OP_OBJECT_CENTER),
    MakeFloat(KW_radius, OP_OBJECT_RADIUS),
    MakeString(KW_material, OP_OBJECT_MATERIAL),
    NullRecord,
};

constexpr bAsciiLocal PlaneType[] = {
    MakeFloat3(KW_point, OP_OBJECT_P0),
    MakeFloat3(KW_normal, OP_OBJECT_NORMAL),
    MakeString(KW_material, OP_OBJECT_MATERIAL),
    NullRecord,
};

constexpr bAsciiLocal ConeType[] = {
    NullRecord,
};

constexpr bAsciiLocal CylinderType[] = {
    NullRecord,
};

constexpr bAsciiLocal ShapeLibrary[] = {
    MakeStruct(KW_Cube, LIB_BLOCK_CUBE, CubeType),
    MakeStruct(KW_Sphere, LIB_BLOCK_SPHERE, SphereType),
    MakeStruct(KW_Plane, LIB_BLOCK_PLANE, PlaneType),
    MakeStruct(KW_Cylinder, LIB_BLOCK_CONE, ConeType),
    MakeStruct(KW_Cylinder, LIB_BLOCK_CYLINDER, CylinderType),
    NullRecord,
};

// Camera Tables

constexpr bAsciiLocal CameraType[] = {
    MakeFloat2(KW_clip, OP_CAMERA_CLIP),
    MakeFloat(KW_fov, OP_CAMERA_FOV),
    MakeBool(KW_interactive, OP_CAMERA_INTERACTIVE),
    NullRecord,
};

constexpr bAsciiLocal CameraLibrary[] = {
    MakeStruct(KW_Camera, LIB_BLOCK_CAMERA, CameraType),
    NullRecord,
};

// Light Tables
constexpr bAsciiLocal LightType[] = {
    MakeInt(KW_type, OP_LIGHT_TYPE),
    MakeFloat3(KW_color, OP_LIGHT_COLOR),
    MakeFloat(KW_power, OP_LIGHT_POWER),
    MakeFloat(KW_elevation, OP_LIGHT_FALLOFF_ELEVATION),
    MakeFloat(KW_decay, OP_LIGHT_FALLOFF_DECAY),
    MakeFloat(KW_radius, OP_LIGHT_RADIUS),
    MakeInt(KW_mode, OP_LIGHT_MODE),
    NullRecord,
};

constexpr bAsciiLocal LightLibrary[] = {
    MakeStruct(KW_Light, LIB_BLOCK_LIGHT, LightType),
    NullRecord,
};

// Mesh Tables

constexpr bAsciiLocal MeshVertexBufferType[] = {
    MakeFloatArray(KW_vertices, OP_MESH_VERTEX_VERTICES, 3),
    MakeFloatArray(KW_normals, OP_MESH_VERTEX_NORMALS, 3),
    NullRecord,
};

constexpr bAsciiLocal MeshIndexBufferType[] = {
    MakeIntArray(KW_indices, OP_MESH_INDEX_INDICES, 4),
    MakeIntArray(KW_materials, OP_MESH_INDEX_MATERIALS, 1),
    MakeIntArray(KW_flags, OP_MESH_INDEX_FLAGS, 1),
    NullRecord,
};

constexpr bAsciiLocal MeshType[] = {
    MakeStruct(KW_VertexBuffer, OP_MESH_VERTEX_BUFFER, OP_NUMBER, MeshVertexBufferType),
    MakeStruct(KW_IndexBuffer, OP_MESH_INDEX_BUFFER, OP_NUMBER, MeshIndexBufferType),
    MakeStringArray(KW_materials, OP_MESH_MATERIALS, -1, 16),
    NullRecord,
};

constexpr bAsciiLocal MeshLibrary[] = {
    MakeStruct(KW_Mesh, LIB_BLOCK_MESH, MeshType),
    NullRecord,
};

constexpr bAsciiLocal MaterialType[] = {
    MakeInt(KW_flags, OP_MATERIAL_FLAGS),
    MakeFloat3(KW_color, OP_MATERIAL_COLOR),
    MakeFloat(KW_diffuse, OP_MATERIAL_DIFFUSE),
    MakeFloat(KW_ambient, OP_MATERIAL_AMBIENT),
    MakeFloat(KW_specular, OP_MATERIAL_SPECULAR),
    MakeFloat(KW_hardness, OP_MATERIAL_HARDNESS),
    NullRecord,
};

constexpr bAsciiLocal MaterialLibrary[] = {
    MakeStruct(KW_Material, LIB_BLOCK_MATERIAL, MaterialType),
    NullRecord,
};

constexpr bAsciiLocal bAsciiGlobals[] = {
    MakeStruct(KW_Global, LIB_GLOBAL, GlobalType),
    MakeStruct(KW_SceneLibrary, LIB_SCENE, SceneLibrary),
    MakeStruct(KW_ObjectLibrary, LIB_OBJECT, ObjectLibrary),
    MakeStruct(KW_ShapeLibrary, LIB_SHAPE, ShapeLibrary),
    MakeStruct(KW_CameraLibrary, LIB_CAMERA, CameraLibrary),
    MakeStruct(KW_LightLibrary, LIB_LIGHT, LightLibrary),
    MakeStruct(KW_MeshLibrary, LIB_MESH, MeshLibrary),
    MakeStruct(KW_MaterialLibrary, LIB_MATERIAL, MaterialLibrary),
    NullRecord,
};

const bAsciiLocal* bAsciiFindGlobal(SKhash code)
{
    const bAsciiLocal* type = bAsciiGlobals;

    while (type && type->name)
    {
        if (type->hash == code)
            return type;
        ++type;
    }
    return nullptr;
}

const bAsciiLocal* bAsciiFindLocal(const bAsciiLocal* st, SKhash code)
{
    if (st && st->types)
    {
        const bAsciiLocal* l = st->types;
        while (l && l->name)
        {
            if (l->hash == code)
                return l;
            ++l;
        }
    }
    return nullptr;
}

const bAsciiLocal* bAsciiGetGlobal(bCode code)
{
    const bAsciiLocal* type = bAsciiGlobals;
    while (type && type->name)
    {
        if (type->op == code)
            return type;
        ++type;
    }
    return nullptr;
}

const bAsciiLocal* bAsciiGetLocal(bAsciiLocal* parent, bCode code)
{
    if (parent && parent->types)
    {
        const bAsciiLocal* l = parent->types;

        while (l && l->name)
        {
            if (l->op == code)
                return l;
            ++l;
        }
    }
    return nullptr;
}

const char* bAsciiKeyword(bAsciiLocal* parent, bCode code)
{
    if (parent)
    {
        bAsciiLocal* l = parent;

        while (l && l->name)
        {
            if (l->op == code)
                return l->name;
            ++l;
        }
    }
    return "Unknown";
}
