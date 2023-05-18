/*
-------------------------------------------------------------------------------
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
#include "ftBlend.h"
#include "ftStreams.h"
#include "ftTable.h"

const SKuint32 GLOB = FT_TYPEID('G', 'L', 'O', 'B');

struct ftIdDB
{
    const SKuint16 code;
    ftList ftBlend::*ptr;
    const SKsize     integrityCheck;
};

const ftIdDB ftData[] = {
    {
        FT_TYPEID2('S', 'C'),
        &ftBlend::m_scene,
        sizeof(Blender::Scene),
    },
    {
        FT_TYPEID2('L', 'I'),
        &ftBlend::m_library,
        sizeof(Blender::Library),
    },

    {
        FT_TYPEID2('O', 'B'),
        &ftBlend::m_object,
        sizeof(Blender::Object),
    },

    {
        FT_TYPEID2('M', 'E'),
        &ftBlend::m_mesh,
        sizeof(Blender::Mesh),
    },
    {
        FT_TYPEID2('C', 'U'),
        &ftBlend::m_curve,
        sizeof(Blender::Curve),
    },
    {
        FT_TYPEID2('M', 'B'),
        &ftBlend::m_mball,
        sizeof(Blender::MetaBall),
    },
    {
        FT_TYPEID2('M', 'A'),
        &ftBlend::m_mat,
        sizeof(Blender::Material),
    },
    {
        FT_TYPEID2('T', 'E'),
        &ftBlend::m_tex,
        sizeof(Blender::Tex),
    },
    {
        FT_TYPEID2('I', 'M'),
        &ftBlend::m_image,
        sizeof(Blender::Image),
    },
    {
        FT_TYPEID2('L', 'T'),
        &ftBlend::m_latt,
        sizeof(Blender::Lattice),
    },
    {
        FT_TYPEID2('L', 'A'),
        &ftBlend::m_lamp,
        sizeof(Blender::Lamp),
    },
    {
        FT_TYPEID2('C', 'A'),
        &ftBlend::m_camera,
        sizeof(Blender::Camera),
    },
    {
        FT_TYPEID2('I', 'P'),
        &ftBlend::m_ipo,
        sizeof(Blender::Ipo),
    },
    {
        FT_TYPEID2('K', 'E'),
        &ftBlend::m_key,
        sizeof(Blender::Key),
    },
    {
        FT_TYPEID2('W', 'O'),
        &ftBlend::m_world,
        sizeof(Blender::World),
    },
    {
        FT_TYPEID2('S', 'N'),
        &ftBlend::m_screen,
        sizeof(Blender::bScreen),
    },
    {
        FT_TYPEID2('P', 'Y'),
        &ftBlend::m_script,
        sizeof(Blender::Script),
    },
    {
        FT_TYPEID2('V', 'F'),
        &ftBlend::m_vfont,
        sizeof(Blender::VFont),
    },
    {
        FT_TYPEID2('T', 'X'),
        &ftBlend::m_text,
        sizeof(Blender::Text),
    },
    {
        FT_TYPEID2('S', 'O'),
        &ftBlend::m_sound,
        sizeof(Blender::bSound),
    },
    {
        FT_TYPEID2('G', 'R'),
        &ftBlend::m_group,
        SK_NPOS, // Id struct ?
    },
    {
        FT_TYPEID2('A', 'R'),
        &ftBlend::m_armature,
        sizeof(Blender::bArmature),
    },
    {
        FT_TYPEID2('A', 'C'),
        &ftBlend::m_action,
        sizeof(Blender::bAction),
    },
    {
        FT_TYPEID2('N', 'T'),
        &ftBlend::m_nodetree,
        sizeof(Blender::bNodeTree),
    },
    {
        FT_TYPEID2('B', 'R'),
        &ftBlend::m_brush,
        sizeof(Blender::Brush),
    },
    {
        FT_TYPEID2('P', 'A'),
        &ftBlend::m_particle,
        SK_NPOS,  // Id struct ?
    },
    {
        FT_TYPEID2('G', 'D'),
        &ftBlend::m_gpencil,
        SK_NPOS,  // Id struct ?
    },
    {
        FT_TYPEID2('W', 'M'),
        &ftBlend::m_wm,
        sizeof(Blender::wmWindowManager),
    },
    {
        0,
        nullptr,
        SK_NPOS,
    },
};


ftBlend::ftBlend() :
    ftFile("BLENDER"),
    m_fg()
{
}

ftBlend::~ftBlend() = default;

int ftBlend::notifyDataRead(void* pointer, const SKsize sizeInBytes, const ftChunk& chunk)
{
    if (!pointer)
        return 0;

    if (chunk.code == GLOB)
    {
        if (sizeInBytes == sizeof(Blender::FileGlobal))
        {
            m_fg = (Blender::FileGlobal*)pointer;
            return ftFlags::FS_OK;
        }

        // it's not safe to cast this data
        return ftFlags::FS_INTEGRITY_FAIL;
    }

    if (chunk.code <= 0xFFFF)
    {

        int i = 0;
        while (ftData[i].code != 0)
        {
            if (ftData[i].code == chunk.code)
            {
                if (ftData[i].integrityCheck == SK_NPOS)
                    break;

                if (ftData[i].integrityCheck == sizeInBytes)
                {
                    (this->*ftData[i].ptr).pushBack(pointer);
                    return ftFlags::FS_OK;
                }

                return ftFlags::FS_INTEGRITY_FAIL;
            }
            ++i;
        }
    }

    // Ok here just means it was not processed 
    return ftFlags::FS_OK;
}

int ftBlend::serializeData(skStream* stream)
{
    for (ftMemoryChunk* node = (ftMemoryChunk*)m_chunks.first; node; node = node->next)
    {
        if (node->newTypeId > m_memory->getNumberOfTypes())
            continue;

        if (!node->memoryBlock)
            continue;

        void*   wd = node->memoryBlock;
        ftChunk ch{};

        ch.code     = node->chunk.code;
        ch.count    = node->chunk.count;
        ch.length   = node->chunk.length;
        ch.structId = node->newTypeId;
        ch.address  = (SKsize)wd;

        stream->write(&ch, sizeof(ftChunk));
        stream->write(wd, ch.length);
    }
    return ftFlags::FS_OK;
}

int ftBlend::save(const char* path, const int mode)
{
    m_memoryVersion = m_fileVersion;
    return ftFile::save(path, mode);
}

#include "bfBlender.inl"

void* ftBlend::getTables(void)
{
    return (void*)bfBlenderTable;
}

SKsize ftBlend::getTableSize(void)
{
    return bfBlenderLen;
}
