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
#include "ftChunk.h"
#include <cstdio>
#include "Utils/skPlatformHeaders.h"
#include "ftEndianUtils.h"
#include "ftLogger.h"
#include "ftStreams.h"

using namespace ftEndianUtils;
using namespace ftFlags;

const ftChunk ftChunkUtils::BlankChunk = {};

SKsize ftChunkUtils::write(ftChunk* source, skStream* stream)
{
    SKsize size = 0;
    size += stream->write(source, BlockSize);
    size += stream->write((void*)source->address, source->length);
    return size;
}

SKsize ftChunkUtils::scan(ftChunkScan* dest, skStream* stream, int headerFlags, SKsize totalSize)
{
    SKsize bytesRead = 0;
    SKsize blockLen  = BlockSize;

    if (FT_VOID_8)
    {
        if (headerFlags & FH_VAR_BITS)
            blockLen = Block32;
    }
    else
    {
        if (headerFlags & FH_VAR_BITS)
            blockLen = Block64;
    }

    if ((bytesRead = stream->read(dest, BlockScan)) <= 0)
        return FS_INV_READ;

    bytesRead += blockLen - BlockScan;
    if (!stream->seek(blockLen - BlockScan, SEEK_CUR))
        return FS_INV_READ;

    if (headerFlags & FH_ENDIAN_SWAP)
    {
        if ((dest->code & 0xFFFF) == 0)
            dest->code >>= 16;
        dest->length = swap32(dest->length);
    }

    if (!isValidCode(dest->code))
        return FS_CODE_ERROR;

    if (dest->length > totalSize)
        return FS_INV_LENGTH;

    return bytesRead;
}

SKsize ftChunkUtils::read(ftChunk* dest, skStream* stream, int headerFlags, SKsize totalSize)
{
    SKsize   bytesRead = 0;
    ftChunk* tmp;

    if (FT_VOID_8)
    {
        ftChunk64 c64{};
        if (headerFlags & FH_VAR_BITS)
        {
            ftChunk32 src{};
            if ((bytesRead = stream->read(&src, Block32)) <= 0)
                return FS_INV_READ;

            ftByteInteger ptr = {0};

            c64.code     = src.code;
            c64.length   = src.length;
            ptr.int32[0] = src.address;
            c64.address  = ptr.int64;
            c64.structId = src.structId;
            c64.count    = src.count;
        }
        else
        {
            if ((bytesRead = stream->read(&c64, BlockSize)) <= 0)
                return FS_INV_READ;
        }

        tmp = (ftChunk*)&c64;
    }
    else
    {
        ftChunk32 c32{};
        if (headerFlags & FH_VAR_BITS)
        {
            ftChunk64 src{};
            if ((bytesRead = stream->read(&src, Block64)) <= 0)
                return FS_INV_READ;

            ftByteInteger ptr = {0};
            c32.code          = src.code;
            c32.length        = src.length;
            c32.structId      = src.structId;
            c32.count         = src.count;

            ptr.int64 = src.address;
            if (ptr.int32[0] != 0)
                c32.address = ptr.int32[0];
            else
                c32.address = ptr.int32[1];
        }
        else
        {
            if ((bytesRead = stream->read(&c32, BlockSize)) <= 0)
                return FS_INV_READ;
        }

        tmp = (ftChunk*)&c32;
    }

    if (headerFlags & FH_ENDIAN_SWAP)
    {
        if ((tmp->code & 0xFFFF) == 0)
            tmp->code >>= 16;

        tmp->length   = swap32(tmp->length);
        tmp->count    = swap32(tmp->count);
        tmp->structId = swap32(tmp->structId);
    }

    if (tmp->length == SK_NPOS32)
        return FS_INV_LENGTH;

    if (tmp->count < 1 || tmp->count > FileTools_MaxChunkArrayLength)
        return FS_INV_LENGTH;

    if ((SKsize)tmp->count * (SKsize)tmp->length > totalSize)
        return FS_INV_LENGTH;

    memcpy(dest, tmp, BlockSize);

    if (!isValidCode(tmp->code))
        return FS_CODE_ERROR;

    return bytesRead;
}

bool ftChunkUtils::isValidCode(const SKuint32& code)
{
    ftByteInteger32 bInt = {};
    bInt.int32           = code;

    const bool isSmallerCode = code < 0xFFFF;
    bool       result        = true;
    for (int i = 0; i < (isSmallerCode ? 2 : 4) && result; ++i)
    {
        const SKuint8 ch = bInt.int8[i];

        result = ch >= 'a' && ch <= 'z';
        result = result || ch >= 'A' && ch <= 'Z';
        result = result || ch >= '0' && ch <= '9';
        result = result || ch == '_' || ch == ' ';

        if (!result && isSmallerCode)
        {
            ftLogger::logF(
                "Found an invalid code sequence "
                "in the code 0x%04X [0x%02X, 0x%02X]\n",
                bInt.int32,
                bInt.int8[0],
                bInt.int8[1]);
        }
        else if (!result)
        {
            ftLogger::logF(
                "Found an invalid code sequence "
                "in the code 0x%04X [0x%02X, 0x%02X, 0x%02X, 0x%02X]\n",
                bInt.int32,
                bInt.int8[0],
                bInt.int8[1],
                bInt.int8[2],
                bInt.int8[3]);
        }
    }
    return result;
}
