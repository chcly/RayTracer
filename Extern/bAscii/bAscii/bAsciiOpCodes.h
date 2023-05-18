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
#ifndef _bAsciiOpCodes_h_
#define _bAsciiOpCodes_h_

// Compiler codes

enum bAsciiCompileCode
{
    OP_END = 0,
    OP_SECTION,
    OP_SEPARATOR,
    OP_ASSIGNMENT,
    OP_TERM,
    OP_BRACKET_OPEN,
    OP_BRACKET_CLOSE,
    OP_STRING,
    OP_ALPHA,
    OP_NUMBER,
};

// Library data

enum bAsciiLibraries
{
    LIB_GLOBAL = 0,
    LIB_SCENE,
    LIB_SHAPE,
    LIB_OBJECT,
    LIB_CAMERA,
    LIB_LIGHT,
    LIB_MESH,
    LIB_MATERIAL,
    LIB_SKELETON,
    LIB_IMAGE,
    LIB_TEXT,
    LIB_MAX,
};

// Library data types

enum bAsciiBlockCode
{
    LIB_BLOCK_SCENE = 0, 
    LIB_BLOCK_OBJECT,
    LIB_BLOCK_CUBE,
    LIB_BLOCK_SPHERE,
    LIB_BLOCK_PLANE,
    LIB_BLOCK_CONE,
    LIB_BLOCK_CYLINDER,
    LIB_BLOCK_CAMERA,
    LIB_BLOCK_LIGHT,
    LIB_BLOCK_MESH,
    LIB_BLOCK_SKELETON,
    LIB_BLOCK_MATERIAL,
    LIB_BLOCK_IMAGE,
    LIB_BLOCK_TEXT,
    LIB_BLOCK_TEXTURE,
};

// Argument types

enum bAsciiTypeCode
{
    OP_TYPE_NONE = 0,
    OP_TYPE_MIXED,
    OP_TYPE_STRING,
    OP_TYPE_BOOL,
    OP_TYPE_BOOL2,
    OP_TYPE_BOOL3,
    OP_TYPE_INT,
    OP_TYPE_INT2,
    OP_TYPE_INT3,
    OP_TYPE_FLOAT,
    OP_TYPE_FLOAT2,
    OP_TYPE_FLOAT3,
    OP_TYPE_FLOAT4,
    OP_TYPE_FLOAT9,
    OP_TYPE_FLOAT16,
    OP_TYPE_STRUCT,
    OP_TYPE_ARRAY_BEGIN,
    OP_TYPE_STRING_ARRAY,
    OP_TYPE_BOOL_ARRAY,
    OP_TYPE_INT_ARRAY,
    OP_TYPE_FLOAT_ARRAY,
    OP_TYPE_ARRAY_END,
    OP_TYPE_UNKNOWN,
};

// Global

enum bAsciiGlobalOp
{
    OP_GLOBAL_CREATOR = 0,
    OP_GLOBAL_VERSION,
    OP_GLOBAL_SCENE,
};

// Scene

enum bAsciiSceneOp
{
    OP_SCENE_OBJECTS = 0,
    OP_SCENE_HORIZON,
    OP_SCENE_ZENITH,
    OP_SCENE_FLAGS,

};

// Object

enum bAsciiObjectOp
{
    OP_OBJECT_LIB = 0,
    OP_OBJECT_LOCATION,
    OP_OBJECT_ROTATION,
    OP_OBJECT_SCALE,
    OP_OBJECT_MATRIX,
    OP_OBJECT_PARENT,
    OP_OBJECT_CENTER,
    OP_OBJECT_NORMAL,
    OP_OBJECT_P0,
    OP_OBJECT_EXTENT,
    OP_OBJECT_RADIUS,
    OP_OBJECT_MATERIAL,
};

// Camera

enum bAsciiCameraOp
{
    OP_CAMERA_CLIP = 0,
    OP_CAMERA_FOV,
    OP_CAMERA_INTERACTIVE,
};

// Light

enum bAsciiLightOp
{
    OP_LIGHT_TYPE = 0,
    OP_LIGHT_COLOR,
    OP_LIGHT_POWER,
    OP_LIGHT_FALLOFF_ELEVATION,
    OP_LIGHT_FALLOFF_DECAY,
    OP_LIGHT_RADIUS,
    OP_LIGHT_MODE,
};


// Material

enum bAsciiMaterialOp
{
    OP_MATERIAL_FLAGS = 0,
    OP_MATERIAL_COLOR,
    OP_MATERIAL_AMBIENT,
    OP_MATERIAL_DIFFUSE,
    OP_MATERIAL_SPECULAR,
    OP_MATERIAL_HARDNESS,
};


// MESH
enum bAsciiMeshOp
{
    OP_MESH_VERTEX_BUFFER = 0,
    OP_MESH_VERTEX_VERTICES,
    OP_MESH_VERTEX_NORMALS,
    OP_MESH_INDEX_BUFFER,
    OP_MESH_INDEX_INDICES,
    OP_MESH_INDEX_MATERIALS,
    OP_MESH_INDEX_FLAGS,
    OP_MESH_MATERIALS,
};

#endif  //_bAsciiOpCodes_h_
