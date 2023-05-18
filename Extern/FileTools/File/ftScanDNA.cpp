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
#include "ftScanDNA.h"
#include "Utils/skPlatformHeaders.h"
#include "ftChunk.h"
#include "ftEndianUtils.h"
#include "ftLogger.h"
#include "ftTable.h"

using namespace ftFlags;
using namespace ftEndianUtils;

ftScanDna::ftScanDna() :
    m_foundBlock(nullptr),
    m_foundLen(0),
    m_totalLen(0),
    m_headerFlags(0)
{
}

ftScanDna::~ftScanDna() = default;


int ftScanDna::findHeaderFlags(skStream* stream)
{
    int status = FS_OK;

    char  buf[13] = {};

    const SKsize read = stream->read(buf, 12);
    if (read != SK_NPOS && read > 0)
    {
        char* magic = &buf[7];

        if (*magic == FM_64_BIT || *magic == FM_32_BIT)
        {
            if (*magic == FM_64_BIT)
            {
                m_headerFlags |= FH_CHUNK_64;
                if (FT_VOID_4)
                    m_headerFlags |= FH_VAR_BITS;
            }
            else if (FT_VOID_8)
                m_headerFlags |= FH_VAR_BITS;

            const char endian = *++magic;
            if (endian == FM_BIG_ENDIAN || endian == FM_LITTLE_ENDIAN)
            {
                const int current = (int)getEndian();

                if (endian == FM_BIG_ENDIAN)
                {
                    if (current == FT_ENDIAN_IS_LITTLE)
                        m_headerFlags |= FH_ENDIAN_SWAP;
                }
                else if (endian == FM_LITTLE_ENDIAN)
                {
                    if (current == FT_ENDIAN_IS_BIG)
                        m_headerFlags |= FH_ENDIAN_SWAP;
                }
                else
                {
                    ftLogger::logF("The file header is missing the endian magic.");
                    status = FS_INV_HEADER_STR;
                }
            }
        }
        else
        {
            ftLogger::logF("The file header is missing the chunk size magic.");
            status = FS_INV_HEADER_STR;
        }
    }
    else
        status = FS_INV_READ;
    return status;
}

int ftScanDna::scan(skStream* stream)
{
    int         status = FS_OK;
    ftChunkScan scan   = {0, 0};

    m_totalLen         = stream->size();

    while (scan.code != ftIdNames::ENDB &&
           scan.code != ftIdNames::DNA1 &&
           status == FS_OK && !stream->eof())
    {
        const SKsize bytesRead = ftChunkUtils::scan(&scan, stream, m_headerFlags, m_totalLen);
        if ((int)bytesRead <= 0 || bytesRead == SK_NPOS)
            status = (int)bytesRead;
        else if (scan.code != ftIdNames::ENDB)
        {
            if (scan.code == ftIdNames::DNA1)
            {
                // This block needs to stay alive as long as m_file is valid.
                // The names of the types and the names of the type-name
                // declarations are referenced out of this block.
                void* found = malloc(scan.length);
                if (!found)
                    status = FS_BAD_ALLOC;
                else
                {
                    if (stream->read(found, scan.length) <= 0)
                    {
                        free(found);
                        status = FS_INV_READ;
                    }
                    else
                    {
                        m_foundBlock = found;
                        m_foundLen   = scan.length;
                    }
                }
            }
            else
            {
                if (scan.length > 0 && scan.length != SK_NPOS32)
                {
                    if (!stream->seek(scan.length, SEEK_CUR))
                        status = FS_INV_READ;
                }
                else
                    status = FS_INV_LENGTH;
            }
        }
    }
    return status;
}

bool ftScanDna::is64Bit() const
{
    return (m_headerFlags & FH_CHUNK_64) != 0;
}
