#pragma once

#include "Utils/skHash.h"
struct bKey
{
    const char*  name;
    const SKhash hash;
};

constexpr bKey KW_Camera          = { "Camera", 0x3505840FE2B6883A};
constexpr bKey KW_CameraLibrary   = { "CameraLibrary", 0x417AABDB0D41F7D};
constexpr bKey KW_Cone            = { "Cone", 0x77FE1132DB1BBA3E};
constexpr bKey KW_Cube            = { "Cube", 0x105F6E866BFB30AC};
constexpr bKey KW_Cylinder        = { "Cylinder", 0x30B32CF66B0CE30B};
constexpr bKey KW_Global          = { "Global", 0x94238CF41711C45A};
constexpr bKey KW_IndexBuffer     = { "IndexBuffer", 0xA55805A64EA3F9D};
constexpr bKey KW_Light           = { "Light", 0x8B99A211D626983B};
constexpr bKey KW_LightLibrary    = { "LightLibrary", 0x64E6EABFA77E372E};
constexpr bKey KW_Material        = { "Material", 0xD351F5BAD02043BC};
constexpr bKey KW_MaterialLibrary = { "MaterialLibrary", 0x4B07C797F21E22C7};
constexpr bKey KW_Mesh            = { "Mesh", 0x69F6AA4717346628};
constexpr bKey KW_MeshLibrary     = { "MeshLibrary", 0x89BF342A89587B83};
constexpr bKey KW_Object          = { "Object", 0xA80AF576756150AE};
constexpr bKey KW_ObjectLibrary   = { "ObjectLibrary", 0x14AAC316965FA081};
constexpr bKey KW_Plane           = { "Plane", 0x886A93DF100A2BE1};
constexpr bKey KW_Scene           = { "Scene", 0x7863EEA1134ABDBF};
constexpr bKey KW_SceneLibrary    = { "SceneLibrary", 0x3BEF3E060D527A02};
constexpr bKey KW_ShapeLibrary    = { "ShapeLibrary", 0x43BAAF0F535C1185};
constexpr bKey KW_Sphere          = { "Sphere", 0x33E34E7FF6D15BE4};
constexpr bKey KW_VertexBuffer    = { "VertexBuffer", 0x9056E56C682F56C9};
constexpr bKey KW_ambient         = { "ambient", 0x578A7FFC4DCB69BF};
constexpr bKey KW_center          = { "center", 0x354FA33D80D9F7A0};
constexpr bKey KW_clip            = { "clip", 0x37485337B1BCA84F};
constexpr bKey KW_color           = { "color", 0xE361A53DE6635CD4};
constexpr bKey KW_creator         = { "creator", 0x83DB8B852F68DE63};
constexpr bKey KW_data            = { "data", 0x5E1344DE65CD4241};
constexpr bKey KW_decay           = { "decay", 0xA3FAC28E52E446B1};
constexpr bKey KW_diffuse         = { "diffuse", 0xE58EA78AE7C333C9};
constexpr bKey KW_elevation       = { "elevation", 0xF726CDD8FCC4B010};
constexpr bKey KW_extent          = { "extent", 0xC8A3C9DE18DEF0FD};
constexpr bKey KW_flags           = { "flags", 0xB80495492F4EA368};
constexpr bKey KW_fov             = { "fov", 0xAD46B842D4B9FC48};
constexpr bKey KW_hardness        = { "hardness", 0xD1BEB4A48DC4950B};
constexpr bKey KW_horizon         = { "horizon", 0x457155D0C2F68064};
constexpr bKey KW_indices         = { "indices", 0x5DF7F55ACD79C88E};
constexpr bKey KW_interactive     = { "interactive", 0x545BE756B06060CF};
constexpr bKey KW_location        = { "location", 0x710932E1C4A521AA};
constexpr bKey KW_material        = { "material", 0xA59FA4AA71395EDC};
constexpr bKey KW_materials       = { "materials", 0x64C171AEEC500D7D};
constexpr bKey KW_matrix          = { "matrix", 0x793F92E4B9623208};
constexpr bKey KW_mode            = { "mode", 0x9972BBECE810814E};
constexpr bKey KW_normal          = { "normal", 0x62F8F927E5913BBE};
constexpr bKey KW_normals         = { "normals", 0xF5D5CB0A30A123B7};
constexpr bKey KW_objects         = { "objects", 0x630FF537021A3887};
constexpr bKey KW_parent          = { "parent", 0x1108A6205BCFE011};
constexpr bKey KW_point           = { "point", 0xFB46E7DB45E0764D};
constexpr bKey KW_power           = { "power", 0x807D26922674EA52};
constexpr bKey KW_radius          = { "radius", 0xE9ED6EDE2BB9B82F};
constexpr bKey KW_rotation        = { "rotation", 0x28796058ED394AB3};
constexpr bKey KW_scale           = { "scale", 0xC78C9FDA71F75DE5};
constexpr bKey KW_scene           = { "scene", 0xB41E806AB9062F5F};
constexpr bKey KW_specular        = { "specular", 0x3427B9A93D4AE064};
constexpr bKey KW_type            = { "type", 0x61809EC5D6445499};
constexpr bKey KW_version         = { "version", 0xAC53B2343D45065B};
constexpr bKey KW_vertices        = { "vertices", 0x912E1D900A3D599A};
constexpr bKey KW_zenith          = { "zenith", 0xA9C56C25AFD4EAA9};
